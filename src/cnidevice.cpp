/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cnidevice.cpp             Type        : source            *
 *  Description : CNIDevice class implementation.                           *
 ****************************************************************************/

/**
 * @file cnidevice.cpp
 * @brief CNIDevice class implementation.
 */

#include <string.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "cniintf.h"
#include "cnidevice.h"

#include "inc/nidnet.h"

/**
 * @brief Converts module connection type to National Instruments API
 * connection type constants.
 */
#define CT_TO_CT(ct, ni_ct) ( ( ni_ct = 0 )                 ,   \
    (ct & DEVICENET_CONN_POLLED) ? ni_ct |= NC_CONN_POLL    :0, \
    (ct & DEVICENET_CONN_STRBED) ? ni_ct |= NC_CONN_STROBE  :0, \
    (ct & DEVICENET_CONN_COS   ) ? ni_ct |= NC_CONN_COS     :0, \
    (ct & DEVICENET_CONN_CYCLIC) ? ni_ct |= NC_CONN_CYCLIC  :0)

unsigned long CNIDevice::ulClassID = 351;
char CNIDevice::strClassName[] = "CNIDevice";

/**
 * @brief Default constructor
 *
 * Initializes members accordingly.
 */
CNIDevice::CNIDevice() : ulHIO(0), ulHEM(0), CDevice() {}

/**
 * @brief Constructors with parameters.
 *
 * Initializes member from parameters.
 * @param ucMID
 * @param ucCCS
 * @param ucPCS
 * @param ucCT
 * @param usEPR
 * @param pIntf
 */
CNIDevice::CNIDevice(
    unsigned char  ucMID,
    unsigned char  ucCCS,
    unsigned char  ucPCS,
    unsigned char  ucCT,
    unsigned short usEPR,
    CInterface     *pIntf)
: ulHIO(0), ulHEM(0), CDevice(ucMID, ucCCS, ucPCS, ucCT, usEPR, pIntf) {}

/**
 * @brief Checks if class can identify itself with the specified number.
 *
 * If not then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CNIDevice::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CDevice::IsA(ulCompareID);
}

/**
 * @brief Checks if class can identify itself with the specified name.
 *
 * If not then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CNIDevice::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CDevice::IsA(strCompareName);
}

/**
 * @brief Allocates a NI device.
 *
 * The function attaches device to interface and opens connection objects.
 * @param ucFlags
 * @return Error from \ref SetError function.
 */
int CNIDevice::Allocate(unsigned char ucFlags) {
    int iErr = 0;
    unsigned long ulCurrState = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
            int iStatus = 0;
            char strIName[7] = {0};
            unsigned short usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();
            (dynamic_cast<CNIInterface *>(pInterface))->GetName(sizeof(strIName), strIName);

            if ( (ulHIO == 0) && (ucFlags & NI_CONN_IO) ) {
                unsigned char ucCnType = 0;
                CT_TO_CT(ucConnType, ucCnType);
                iStatus = ncOpenDnetIO(strIName, ucMacID, ucCnType,
                    ucConsumedConnSize, ucProducedConnSize, usEPR, &ulHIO);
                iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
                if ( iStatus < 0 )
                    return iErr;
            }
            if ( (ulHEM == 0) && (ucFlags & NI_CONN_EM) ) {
                iStatus = ncOpenDnetExplMsg(strIName, ucMacID, &ulHEM);
                iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
                if ( iStatus != 0 )
                    return iErr;
            }
            if ( ulHEM != 0 ) {
                iStatus = ncWaitForState(ulHEM, NC_ST_ESTABLISHED, 10000,
                            &ulCurrState);
                iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
            }
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    if ( iErr == 0 )
        bActive = true;

    return iErr;
}

/**
 * @brief Unallocates a NI device.
 *
 * The function closes connection objects and releases interface.
 * @return
 */
int CNIDevice::UnallocateDevice(void) {
    int iStatus = 0;
    int iErr = 0;
    unsigned short usIID = 0;

    if ( ISPTRVALID(pInterface, CInterface) )
        if ( pInterface->IsA("CNIInterface") )
            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

    if ( ulHIO != 0 ) {
        iStatus = ncCloseObject(ulHIO);
        iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
        ulHIO = 0;
    }
    if ( ulHEM != 0 ) {
        iStatus = ncCloseObject(ulHEM);
        iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
        ulHEM = 0;
    }
    bActive = false;

    return iErr;
}

/**
 * @brief Unalloates the NI device.
 * @return Error from \ref SetError function.
 */
int CNIDevice::Unallocate(void) {
  return UnallocateDevice();
}

/**
 * @brief Reads data from NI device.
 *
 * Reads data from device using I/O messaging object.
 * @param ulBufSz Size of the buffer.
 * @param pvBuf Pointer to the buffer where to store read I/O data.
 * @return Error from \ref SetError function.
 */
int CNIDevice::ReadIOData(unsigned long ulBufSz, void *pvBuf) {
    unsigned long ulCurrState = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHIO != 0 ) {
            int iStatus = 0;
            int iErr = 0;
            unsigned short usIID = 0;

            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

            iStatus = ncWaitForState(ulHIO, NC_ST_READ_AVAIL, 10000,
                &ulCurrState);
            iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
            if ( iStatus < 0 )
                return iErr;

            iStatus = ncReadDnetIO(ulHIO, ulBufSz, pvBuf);
            return SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
          }
          else return SetError(ERR_NOALOCIO, ucMacID);
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);
}

/**
 * @brief Writes data to NI device
 *
 * Write data to device using I/O object.
 * @param ulBufSz Size of the buffer.
 * @param pvBuf Pointer to the buffer from where to read I/O data.
 * @return Error from \ref SetError function.
 */
int CNIDevice::WriteIOData(unsigned long ulBufSz, void *pvBuf) {
    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHIO != 0 ) {
            int iStatus = 0;
            unsigned short usIID = 0;

            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

            iStatus = ncWriteDnetIO(ulHIO, ulBufSz, pvBuf);
            return SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
          }
          else return SetError(ERR_NOALOCIO, ucMacID);
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);
}

/**
 * @brief Reads NI device attribute
 *
 * Reads device attribute using Get_Attribute_Single service.
 * @param usClsId Class identifier of the attribute.
 * @param usInstId Instance identifier of the attribute.
 * @param ucAttrId Attribute identifier.
 * @param usDataSz Length in bytes of the attribute data.
 * @param pvData Pointer to a buffer where to copy attribute data.
 * @param pusActDataSz Actual size of the attribute data in bytes.
 * @return Error from \ref SetError function.
 */
int CNIDevice::GetAttribute(
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned char  ucAttrId,
    unsigned short usDataSz,
    void           *pvData,
    unsigned short *pusActDataSz)
{
    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHEM != 0 ) {
            unsigned short usDevError = 0;
            unsigned short usIID = 0;
            int iStatus = 0;

            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

            iStatus = ncGetDnetAttribute(ulHEM, usClsId, usInstId, ucAttrId,
                2000, usDataSz, pvData, pusActDataSz, &usDevError);
            if ( (unsigned short)iStatus == NC_ERR_DNET_ERR_RESP )
                return SetError(ERR_EXPLCT, ucMacID, usDevError);
            else return SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
          }
          else return SetError(ERR_NOALOCEM, ucMacID);
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);
}

/**
 * @brief Writes NI device attribute
 *
 * Writes device attribute with Set_Attribute_Single service.
 * @param usClsId Class identifier of the parameter.
 * @param usInstId Instance identifier of the parameter.
 * @param ucAttrId Parameter identifier.
 * @param usDataSz Size of parameter's data in bytes.
 * @param pvData Pointer to a buffer where to copy parameter's data.
 * @return Error from \ref SetError function.
 */
int CNIDevice::SetAttribute(
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned char  ucAttrId,
    unsigned short usDataSz,
    void           *pvData)
{
    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHEM != 0 ) {
            unsigned short usDevError = 0;
            unsigned short usIID = 0;
            int iStatus = 0;

            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

            iStatus = ncSetDnetAttribute(ulHEM, usClsId, usInstId, ucAttrId,
                2000, usDataSz, pvData, &usDevError);
            if ( (unsigned short)iStatus == NC_ERR_DNET_ERR_RESP )
                return SetError(ERR_EXPLCT, ucMacID, usDevError);
            else return SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
          }
          else return SetError(ERR_NOALOCEM, ucMacID);
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);
}

/**
 * @brief Execute DeviceNet™ service in the NI device.
 *
 * Writes and explicit message to the NI device, waits for read and reads
 * status.
 * @param ucSrvCode Service code.
 * @param usClsId Class identifier of the service.
 * @param usInstId Instance identifier of the service.
 * @param usDataSz Size of service's data in bytes.
 * @param pvData Pointer to a buffer where to copy service's data.
 * @return Error from \ref SetError function.
 */
int CNIDevice::ExecService(
    unsigned char  ucSrvCode,
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned short usDataSz,
    void           *pvData)
{
    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHEM != 0 ) {
            int iStatus = 0;
            int iErr = 0;
            unsigned char ucRSrvCode = 0;
            unsigned short usRDataSize = 0;
            unsigned short usIID = 0;
            unsigned long ulCurrState = 0;

            usIID = (dynamic_cast<CNIInterface *>(pInterface))->GetIntfID();

            iStatus = ncWriteDnetExplMsg(ulHEM, ucSrvCode, usClsId, usInstId,
                usDataSz, pvData);
            iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
            if ( iStatus < 0 )
                return iErr;

            iStatus = ncWaitForState(ulHEM, NC_ST_READ_AVAIL, 10000, &ulCurrState);
            iErr = SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
            if ( iStatus < 0 )
                return iErr;

            iStatus = ncReadDnetExplMsg(ulHEM, &ucRSrvCode, usDataSz, pvData,
                &usRDataSize);
            return SetError(ERR_NIDNET, iStatus, usIID, ucMacID);
            /*if ( iStatus < 0 )
                return iErr;
            else if ( ucRSrvCode != ucSrvCode )
                SetError(ERR_NIDNET, iStatus, 0, ucMacID);
            else if ( usRDataSize > usDataSize ) {
                SetError(ERR_NIDNET, iStatus, 0, ucMacID);
            }*/
          }
          else return SetError(ERR_NOALOCEM, ucMacID);
        }
        else return SetError(ERR_INOPER, "Allocate");
      }
      else return SetError(ERR_INVITF, ucMacID, "CNIInterface");
    }
    else return SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);
}

/**
 * @brief Resets the NI device.
 *
 * Executes service 5 with empty character buffer. Calls CNIDevice::ExecService.
 * @return
 */
int CNIDevice::Reset(void) {
    char cBuf = 0;

    return ExecService(5, 1, 1, sizeof(cBuf), &cBuf);
}

/**
 * @brief Destructor
 *
 * Unallocats device if active.
 */
CNIDevice::~CNIDevice() {
    if ( bActive )
        UnallocateDevice();
}

