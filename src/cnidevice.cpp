/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cnidevice.cpp             Type        : source            *
 *  Description : CNIDevice class implementation.                           *
 *  CVS         : $Id: cnidevice.cpp,v 1.2 2008/08/17 19:15:58 gsotirov Exp $
 ****************************************************************************/

#include <string.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "cniintf.h"
#include "cnidevice.h"

#include "inc/nidnet.h"

#define CT_TO_CT(ct, ni_ct) ( ( ni_ct = 0 )                 ,   \
    (ct & DEVICENET_CONN_POLLED) ? ni_ct |= NC_CONN_POLL    :0, \
    (ct & DEVICENET_CONN_STRBED) ? ni_ct |= NC_CONN_STROBE  :0, \
    (ct & DEVICENET_CONN_COS   ) ? ni_ct |= NC_CONN_COS     :0, \
    (ct & DEVICENET_CONN_CYCLIC) ? ni_ct |= NC_CONN_CYCLIC  :0)

unsigned long CNIDevice::ulClassID = 351;
char CNIDevice::strClassName[] = "CNIDevice";

CNIDevice::CNIDevice() : CDevice() {}

CNIDevice::CNIDevice(
    unsigned char  ucMID,
    unsigned char  ucCCS,
    unsigned char  ucPCS,
    unsigned char  ucCT,
    unsigned short usEPR,
    CInterface     *pIntf)
: CDevice(ucMID, ucCCS, ucPCS, ucCT, usEPR, pIntf) {}

bool CNIDevice::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CDevice::IsA(ulCompareID);
}

bool CNIDevice::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CDevice::IsA(strCompareName);
}

/* Function: CNIDevice::Allocate
 * Purpose : Attach device to interface and open connection objects.
 */
int CNIDevice::Allocate(unsigned char ucFlags) {
    int iStatus = 0;
    int iErr = 0;
    unsigned long ulCurrState = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
            char strIName[7] = {0};
            unsigned short usIID = ((CNIInterface*)pInterface)->GetIntfID();
            ((CNIInterface*)pInterface)->GetName(sizeof(strIName), strIName);

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

/* Function: CNIDevice::Unallocate
 * Purpose : Close connection objects and release interface.
 */
int CNIDevice::Unallocate(void) {
    int iStatus = 0;
    int iErr = 0;
    unsigned short usIID = 0;

    if ( ISPTRVALID(pInterface, CInterface) )
        if ( pInterface->IsA("CNIInterface") )
            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

/* Function: CNIDevice::ReadIOData
 * Purpose : Read data from device using I/O messaging object.
 */
int CNIDevice::ReadIOData(unsigned long ulBufSz, void *pvBuf) {
    int iStatus = 0;
    unsigned long ulCurrState = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHIO != 0 ) {
            int iErr = 0;
            unsigned short usIID = 0;

            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

/* Function: CNIDevice::WriteIOData
 * Purpose : Write data to device using I/O object.
 */
int CNIDevice::WriteIOData(unsigned long ulBufSz, void *pvBuf) {
    int iStatus = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CNIInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( ulHIO != 0 ) {
            unsigned short usIID = 0;

            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

/* Function: CNIDevice::GetAttribute
 * Purpose : Read device attrubute using Get_Attribute_Single service.
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

            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

/* Function: CNIDevice::SetAttribute
 * Purpose : Write device attribute with Set_Attribute_Single service.
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

            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

/* Function: CNIDevice::ExecService
 * Purpose : Execute DeviceNet service for the device.
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

            usIID = ((CNIInterface*)pInterface)->GetIntfID();

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

int CNIDevice::Reset(void) {
    char cBuf = 0;

    return ExecService(5, 1, 1, sizeof(cBuf), &cBuf);
}

CNIDevice::~CNIDevice() {
    if ( bActive )
        Unallocate();
}

