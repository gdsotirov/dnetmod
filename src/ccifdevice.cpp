/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : ccifdevice.cpp            Type        : header            *
 *  Description : CCIFDevice class implementation.                          *
 *  CVS         : $Id: ccifdevice.cpp,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#include <string.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "ccifintf.h"
#include "ccifdevice.h"

#if defined(OS_LINUX)
#include <unistd.h>
#define msleep(ms) ( usleep(ms*1000) )
#elif defined(OS_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define msleep(ms) ( Sleep(ms) )
#endif

#if defined(OS_LINUX)
#include "cif_user.h"
#elif defined(OS_WIN32)
#include "cifuser.h"
#endif
#include "rcs_user.h"
#include "dnm_user.h"

#define DEV_0       0x01
#define DEV_1       0x02
#define DEV_2       0x04
#define DEV_3       0x08
#define DEV_4       0x10
#define DEV_5       0x20
#define DEV_6       0x40
#define DEV_7       0x80

/* Macro   : CT_TO_CT
 * Purpose : Translate DNetMod connection types to CIF connection types.
 */
#define CT_TO_CT(ct, cif_ct) ( ( cif_ct = 0 )                        ,   \
    ( ct & DEVICENET_CONN_POLLED ) ? cif_ct |= DNM_TYPE_POLLED       :0, \
    ( ct & DEVICENET_CONN_STRBED ) ? cif_ct |= DNM_TYPE_BIT_STROBED  :0, \
    ( ct & DEVICENET_CONN_COS    ) ? cif_ct |= DNM_TYPE_CHG_OF_STATE :0, \
    ( ct & DEVICENET_CONN_CYCLIC ) ? cif_ct |= DNM_TYPE_CYCLIC       :0 )

unsigned long CCIFDevice::ulClassID = 352;
char CCIFDevice::strClassName[] = "CCIFDevice";

CCIFDevice::CCIFDevice() : CDevice() {
    usInputOffset = usOutputOffset = 0;
}

CCIFDevice::CCIFDevice(
    unsigned char  ucMID,
    unsigned char  ucCCS,
    unsigned char  ucPCS,
    unsigned char  ucCT,
    unsigned short usEPR,
    CInterface     *pIntf)
: CDevice(ucMID, ucCCS, ucPCS, ucCT, usEPR, pIntf) {
    usInputOffset = usOutputOffset = 0;
}

inline int CCIFDevice::ExchangeIOData(
    bool          bInput,
    unsigned long ulBufSz,
    void          *pvBuf)
{
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface)  ) {
      if ( pInterface->IsA("CCIFInterface") ) {
        if ( pInterface->IsActive() ) {
          if ( bActive ) {
            CCIFInterface *pCIFIntf = (CCIFInterface*)pInterface;
            short sStatus = 0;

            if ( bInput )
                sStatus = DevExchangeIO(pCIFIntf->GetBoardNum(), 0, 0, NULL, usInputOffset, (unsigned short)ulBufSz, pvBuf, 500L);
            else sStatus = DevExchangeIO(pCIFIntf->GetBoardNum(), usOutputOffset, (unsigned short)ulBufSz, pvBuf,0,0,NULL,500L);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;
          }
          else iErr = SetError(ERR_NOALOC, ucMacID);
        }
        else iErr = SetError(ERR_INOPER, "ReadIOData");
      }
      else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::Diagnostics(void) {
    short sStatus = 0;
    int iErr      = 0;
    CCIFInterface           *pCIFIntf  = (CCIFInterface *)pInterface;
    RCS_MESSAGETELEGRAM_10  MsgBuff;
    DNM_DEVICE_DIAG_CONFIRM *pDiagData = (DNM_DEVICE_DIAG_CONFIRM *)MsgBuff.d;

    MsgBuff.rx = 3;  // DNM-Task
    MsgBuff.tx = 16; // User at HOST
    MsgBuff.ln = sizeof(DNM_DEVICE_DIAG_CONFIRM);
    MsgBuff.nr = MsgBuff.a = MsgBuff.f = MsgBuff.e = 0; // unused
    MsgBuff.b  = DNM_Device_Diag; // 66dec = 42hex
    MsgBuff.device_adr = ucMacID;

    // Gather diagnostics data for the device
    sStatus = DevPutMessage(pCIFIntf->GetBoardNum(), (MSG_STRUC *)&MsgBuff, 500L);
    iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    sStatus = DevGetMessage(pCIFIntf->GetBoardNum(), sizeof(MsgBuff), (MSG_STRUC *)&MsgBuff, 1000L);
    iErr = SetError(ERR_CIF, sStatus, MsgBuff.f, pCIFIntf->GetBoardNum(), ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    // Analyse data
    if ( pDiagData->tDiagData.bDevStatus1.bDvNoResponse )
        iErr = SetError(ERR_CIF, DEV_NOT_RESPONDING, 0, pCIFIntf->GetBoardNum(), ucMacID);
    else if ( pDiagData->tDiagData.bDevStatus1.bPrmFault )
        iErr = SetError(ERR_CIF, DEV_ATTR_ACCESS_DENIED, 0, pCIFIntf->GetBoardNum(), ucMacID);
    else if ( pDiagData->tDiagData.bDevStatus1.bCfgFault )
        iErr = SetError(ERR_CIF, DEV_CONN_LEN_DIFF, 0, pCIFIntf->GetBoardNum(), ucMacID);

    if ( pDiagData->tDiagData.bOnlineError == DERR_ERRRESP ) {
        iErr = SetError(ERR_EXPLCT, ucMacID, pDiagData->tDiagData.bGeneralErrorCode, pDiagData->tDiagData.bAdditionalCode);
    }
    else iErr = SetError(ERR_CIF, 0, pDiagData->tDiagData.bOnlineError, pCIFIntf->GetBoardNum(), ucMacID);

    return iErr;
}

bool CCIFDevice::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CDevice::IsA(ulCompareID);
}

bool CCIFDevice::IsA(char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CDevice::IsA(strCompareName);
}

int CCIFDevice::Allocate(unsigned char /*ucFlags*/) {
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsA("CCIFInterface") ) {
        if ( pInterface->IsActive() ) {
            CCIFInterface *pCIFIntf = ((CCIFInterface*)pInterface);
            short                        sStatus             = 0;
            RCS_MESSAGE                  MsgBuf;
            DNM_DOWNLOAD_REQUEST         *pDownloadReq       = 0;
            DNM_DEV_PRM_HEADER           *pDevPrmHdr         = 0;
            DNM_PRED_MSTSL_CFG_DATA      *pPredMstslCfgData  = 0;
            DNM_PRED_MSTSL_IO_OBJ_HEADER *pPredMstslIoObjHdr = 0;
            DNM_IO_MODULE                *pIOModule          = 0;
            DNM_PRED_MSTSL_CONNOBJ       *pPredMstslConnObj  = 0;
            DNM_PRED_MSTSL_ADD_TAB       *pPredMstslAddTab   = 0;
            DNM_EXPL_SET_ATTR_DATA       *pExplSetAttrData   = 0;
            DNM_UCMM_CONN_OBJ_CFG_DATA   *pUcmmConnObjCfgData= 0;
            DNM_UCMM_CONN_OBJ_ADD_TAB    *pUcmmConnObjAddTab = 0;

            // Message header
            MsgBuf.rx = 3;  // DNM-Task
            MsgBuf.tx = 16; // User at HOST
            MsgBuf.ln = MsgBuf.a = MsgBuf.f = MsgBuf.e = 0;
            MsgBuf.nr = 8;
            MsgBuf.b  = DNM_Download; // 68dec = 44hex

            pDownloadReq = (DNM_DOWNLOAD_REQUEST *)MsgBuf.d;

            // Download Request Header
            pDownloadReq->bReq_Add     = 0;                     // d[0]
            pDownloadReq->bArea_Code   = ucMacID;               // d[1]
            pDownloadReq->usAdd_Offset = 0;                     // d[2-3]

            pDevPrmHdr = (DNM_DEV_PRM_HEADER *)pDownloadReq->abData;

            // Device Parameters Header
            pDevPrmHdr->usDevParaLen         = sizeof(DNM_DEV_PRM_HEADER);
            memset(&pDevPrmHdr->bDvFlag, 0, sizeof(pDevPrmHdr->bDvFlag));
            pDevPrmHdr->bDvFlag.bActive      = 1;               // d[6]:7
            pDevPrmHdr->bDvFlag.bDeviceType  = 0;               // d[6]:6
            pDevPrmHdr->bDvFlag.bProductCode = 0;               // d[6]:5
            pDevPrmHdr->bDvFlag.bVendorID    = 0;               // d[6]:4
            pDevPrmHdr->bDvFlag.bRevision    = 0;               // d[6]:3
            pDevPrmHdr->bDvFlag.bReserved1   = 0;               // d[6]:2
            pDevPrmHdr->bDvFlag.bReserved2   = 0;               // d[6]:1
            pDevPrmHdr->bDvFlag.bUcmmSup     = 0;               // d[6]:0
            pDevPrmHdr->bUcmmGroup           = 0;               // d[7]
            // Configurable?
            pDevPrmHdr->usRecFragTimer       = 1000;            // d[8-9]
            pDevPrmHdr->usVendorID           = 0;               // d[10-11]
            pDevPrmHdr->usDeviceType         = 0;               // d[12-13]
            pDevPrmHdr->usProductCode        = 0;               // d[14-15]
            pDevPrmHdr->bMajorRevision       = 0;               // d[16]
            pDevPrmHdr->bMinorRevision       = 0;               // d[17]
            pDevPrmHdr->bOctetString[0]      = 0;               // d[18]
            pDevPrmHdr->bOctetString[1]      = 0;               // d[19]

            pPredMstslCfgData = (DNM_PRED_MSTSL_CFG_DATA *)((unsigned char *)pDevPrmHdr + pDevPrmHdr->usDevParaLen);

            // Predefined Master-Slave Config Data
            pPredMstslCfgData->usPredMstslCfgDataLen = sizeof(pPredMstslCfgData->usPredMstslCfgDataLen);
            pPredMstslConnObj = (DNM_PRED_MSTSL_CONNOBJ *)pPredMstslCfgData->atConnObjInst;
            pPredMstslIoObjHdr = (DNM_PRED_MSTSL_IO_OBJ_HEADER *)&pPredMstslConnObj->tPredMstSlObjHeader;
            pPredMstslCfgData->usPredMstslCfgDataLen += sizeof( DNM_PRED_MSTSL_IO_OBJ_HEADER );

            // Predefined Master-Slave IO Object Header
            CT_TO_CT(ucConnType, pPredMstslIoObjHdr->bConnectionType);// d[22]
            pPredMstslIoObjHdr->bWatchdogTimeoutAction = DNM_TRANSMISSION_TO_TIMEOUT;
            pPredMstslIoObjHdr->usProdInhibitTime = 10;        // d[24-25]
            pPredMstslIoObjHdr->usExpPacketRate   = usEPR;     // d[26-27]
            pPredMstslIoObjHdr->bNumOfIOModules   = 0;         // d[28]

            pIOModule = (DNM_IO_MODULE *)((unsigned char *)pPredMstslIoObjHdr + sizeof(DNM_PRED_MSTSL_IO_OBJ_HEADER));

            // IO Modules
            if ( ucConsumedConnSize ) {
                pPredMstslIoObjHdr->bNumOfIOModules++;          // d[28]
                pIOModule->bDataType = TASK_TDT_STRING;
                pIOModule->bDataSize = ucConsumedConnSize;
                pIOModule++;
                pPredMstslCfgData->usPredMstslCfgDataLen += sizeof(DNM_IO_MODULE);
            }
            if ( ucProducedConnSize ) {
                pPredMstslIoObjHdr->bNumOfIOModules++;          // d[28]
                pIOModule->bDataType = TASK_TDT_STRING|DNM_OUTPUT;
                pIOModule->bDataSize = ucProducedConnSize;
                pIOModule++;
                pPredMstslCfgData->usPredMstslCfgDataLen += sizeof(DNM_IO_MODULE);
            }

            pPredMstslAddTab = (DNM_PRED_MSTSL_ADD_TAB *)((unsigned char *)pPredMstslCfgData + pPredMstslCfgData->usPredMstslCfgDataLen);

            // Predefined Master-Slave Address Table
            pPredMstslAddTab->usAddTabLen  = 4;
            pPredMstslAddTab->bInputCount  = 0;
            pPredMstslAddTab->bOutputCount = 0;

            if ( ucConsumedConnSize ) {
                pPredMstslAddTab->ausIOOffsets[pPredMstslAddTab->bInputCount] = pCIFIntf->usInputOffset;
                usInputOffset = pCIFIntf->usInputOffset;
                pCIFIntf->usInputOffset = pCIFIntf->usInputOffset + ucConsumedConnSize;
                pPredMstslAddTab->bInputCount++;
                pPredMstslAddTab->usAddTabLen += sizeof(unsigned short);
            }
            if ( ucProducedConnSize ) {
                pPredMstslAddTab->ausIOOffsets[pPredMstslAddTab->bOutputCount + pPredMstslAddTab->bInputCount] = pCIFIntf->usOutputOffset;
                usOutputOffset = pCIFIntf->usOutputOffset;
                pCIFIntf->usOutputOffset = pCIFIntf->usOutputOffset + ucProducedConnSize;
                pPredMstslAddTab->bOutputCount++;
                pPredMstslAddTab->usAddTabLen += sizeof(unsigned short);
            }

            pExplSetAttrData = (DNM_EXPL_SET_ATTR_DATA *)((unsigned char *)pPredMstslAddTab + pPredMstslAddTab->usAddTabLen);
            // Explicit Set Attribute Data
            pExplSetAttrData->usAttrDataLen = sizeof(pExplSetAttrData->usAttrDataLen);

            pUcmmConnObjCfgData = (DNM_UCMM_CONN_OBJ_CFG_DATA *)((unsigned char *)pExplSetAttrData + pExplSetAttrData->usAttrDataLen);
            // UCMM Connection Object Config Data
            pUcmmConnObjCfgData->usCfgDataLen = sizeof(pUcmmConnObjCfgData->usCfgDataLen);

            pUcmmConnObjAddTab = (DNM_UCMM_CONN_OBJ_ADD_TAB *)((unsigned char *)pUcmmConnObjCfgData + pUcmmConnObjCfgData->usCfgDataLen);

            // UCMM Connection Object Address Table
            pUcmmConnObjAddTab->usAddTabLen  = 4;
            pUcmmConnObjAddTab->bInputCount  = 0;
            pUcmmConnObjAddTab->bOutputCount = 0;

            pDevPrmHdr->usDevParaLen = pDevPrmHdr->usDevParaLen + pPredMstslCfgData->usPredMstslCfgDataLen;
            pDevPrmHdr->usDevParaLen = pDevPrmHdr->usDevParaLen + pPredMstslAddTab->usAddTabLen;
            pDevPrmHdr->usDevParaLen = pDevPrmHdr->usDevParaLen + pExplSetAttrData->usAttrDataLen;
            pDevPrmHdr->usDevParaLen = pDevPrmHdr->usDevParaLen + pUcmmConnObjCfgData->usCfgDataLen;
            pDevPrmHdr->usDevParaLen = pDevPrmHdr->usDevParaLen + pUcmmConnObjAddTab->usAddTabLen;

            MsgBuf.ln = (unsigned char)pDevPrmHdr->usDevParaLen + sizeof(DNM_DOWNLOAD_REQUEST) - MAX_LEN_DATA_UNIT;

            /* Download data to DEVICE */
            sStatus = DevPutMessage(pCIFIntf->GetBoardNum(), (MSG_STRUC *)&MsgBuf, 500L);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            sStatus = DevGetMessage(pCIFIntf->GetBoardNum(), sizeof(MsgBuf), (MSG_STRUC *)&MsgBuf, 3000L);
            iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            if ( !iErr )
                iErr = pCIFIntf->Open();

            /* =============================================================
               Note:
               This sleep is required here to ensure that the information
               in DevDiag will be updated by the DEVICE after its config.
               Because the DEVICE handler cylce is betwen 400 microseconds
               to 1 millisecond so I use 100 ms. This here must be dummy but
               I don't find other way to do this.
               =============================================================*/
            msleep(100); // sleep for 100 ms

            // Device diagnostics
            DNM_DIAGNOSTICS DevDiag;
            sStatus = DevGetTaskState(pCIFIntf->GetBoardNum(), 2, sizeof(DevDiag), &DevDiag);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            // Check if DEVICE is in operating state
            if ( DevDiag.bDNM_state & OPERATE ) {
                unsigned char ucOffset = ucMacID / 8;
                unsigned char ucMask = 0;

                switch ( ucMacID % 8 ) {
                    case 0: ucMask = DEV_0;
                            break;
                    case 1: ucMask = DEV_1;
                            break;
                    case 2: ucMask = DEV_2;
                            break;
                    case 3: ucMask = DEV_3;
                            break;
                    case 4: ucMask = DEV_4;
                            break;
                    case 5: ucMask = DEV_5;
                            break;
                    case 6: ucMask = DEV_6;
                            break;
                    case 7: ucMask = DEV_7;
                            break;
                }

                // Check if device is configured
                if ( DevDiag.abDv_cfg[ucOffset] & ucMask ) {
                    // Check if DEVICE has established I/O connection to device
                    if ( DevDiag.abDv_state[ucOffset + 8] & ucMask )
                        bActive = true;
                    else bActive = false;
                    // Check if there is new diagnostics data for device
                    if ( DevDiag.abDv_diag[ucOffset] & ucMask )
                        iErr = Diagnostics();
                }
            }
            if ( !bActive && !iErr )
                iErr = SetError(ERR_UKNOW);
        }
        else iErr = SetError(ERR_INOPER, "Allocate");
      }
      else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::Unallocate(void) {
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface)  ) {
      if ( pInterface->IsA("CCIFInterface") ) {
        if ( pInterface->IsActive() ) {
            CCIFInterface *pCIFIntf  = (CCIFInterface*)pInterface;
            unsigned char ucSSBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            unsigned char *ucBytePtr = ucSSBuf + (unsigned char)(ucMacID/8);
            short         sStatus    = 0;

            // Initialize slave status buffer
            sStatus = DevReadWriteDPMRaw(pCIFIntf->GetBoardNum(), PARAMETER_READ, 0x2F8, sizeof(ucSSBuf), ucSSBuf);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            // Change device I/O connection status bit ot 0 (off)
            switch ( ucMacID % 8 ) {
                case 0: *ucBytePtr &= ~DEV_0;
                        break;
                case 1: *ucBytePtr &= ~DEV_1;
                        break;
                case 2: *ucBytePtr &= ~DEV_2;
                        break;
                case 3: *ucBytePtr &= ~DEV_3;
                        break;
                case 4: *ucBytePtr &= ~DEV_4;
                        break;
                case 5: *ucBytePtr &= ~DEV_5;
                        break;
                case 6: *ucBytePtr &= ~DEV_6;
                        break;
                case 7: *ucBytePtr &= ~DEV_7;
                        break;
            }

            // Write slave status buffer to the board
            sStatus = DevReadWriteDPMRaw(pCIFIntf->GetBoardNum(), PARAMETER_WRITE, 0x2F8, sizeof(ucSSBuf), ucSSBuf);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            bActive = false;
        }
        else iErr = SetError(ERR_INOPER, "ReadIOData");
      }
      else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::ReadIOData(unsigned long ulBufSz, void *pvBuf) {
    return ExchangeIOData(true, ulBufSz, pvBuf);
}

int CCIFDevice::WriteIOData(unsigned long ulBufSz, void *pvBuf) {
    return ExchangeIOData(false, ulBufSz, pvBuf);
}

int CCIFDevice::GetAttribute(
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned char  ucAttrId,
    unsigned short usDataSz,
    void           *pvData,
    unsigned short *pusActDataSz) {
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsActive() ) {
        if ( pInterface->IsA("CCIFInterface") ) {
          if ( bActive ) {
            CCIFInterface *pCIFIntf = (CCIFInterface*)pInterface;
            short sStatus = 0;
            RCS_MESSAGETELEGRAM_10 MsgBuf;

            MsgBuf.rx = 3;
            MsgBuf.tx = 16;
            MsgBuf.ln = sizeof(RCS_MESSAGETELEGRAMHEADER_10) + (unsigned char)usDataSz;
            MsgBuf.nr = MsgBuf.a = MsgBuf.f = MsgBuf.e = 0;
            MsgBuf.b  = DNM_Get_Set_Attribute;

            MsgBuf.device_adr = ucMacID;
            MsgBuf.data_area  = (unsigned char)usClsId;
            MsgBuf.data_adr   = usInstId;
            MsgBuf.data_idx   = ucAttrId;
            MsgBuf.data_cnt   = (unsigned char)usDataSz;
            MsgBuf.data_type  = 0;
            MsgBuf.function   = TASK_TFC_READ;

            sStatus = DevPutMessage(pCIFIntf->GetBoardNum(), (MSG_STRUC*)&MsgBuf, 500L);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            sStatus = DevGetMessage(pCIFIntf->GetBoardNum(), sizeof(MsgBuf), (MSG_STRUC*)&MsgBuf, 3000L);

            iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;
            // Handle device errors
            if ( MsgBuf.f > DERR_OK && MsgBuf.f <= DERR_VENDSPEC )
                iErr = SetError(ERR_EXPLCT, ucMacID, MsgBuf.f, MsgBuf.d[0]);
            // Handle DEVICE task errors
            else iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, pCIFIntf->GetBoardNum(), ucMacID);
            if ( ISPTRVALID(pusActDataSz, unsigned short) )
                *pusActDataSz = MsgBuf.data_cnt;
            if ( !iErr )
                memcpy(pvData, MsgBuf.d, usDataSz);

            iErr = Diagnostics();
          }
          else iErr = SetError(ERR_NOALOC, ucMacID);
        }
        else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
      }
      else iErr = SetError(ERR_INOPER, "GetAttribute");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::SetAttribute(
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned char  ucAttrId,
    unsigned short usDataSz,
    void           *pvData) {
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsActive() ) {
        if ( pInterface->IsA("CCIFInterface") ) {
          if ( bActive ) {
            CCIFInterface *pCIFIntf = (CCIFInterface*)pInterface;
            short sStatus = 0;
            RCS_MESSAGETELEGRAM_10 MsgBuf;

            MsgBuf.rx = 3;
            MsgBuf.tx = 16;
            MsgBuf.ln = sizeof(RCS_MESSAGETELEGRAMHEADER_10) + (unsigned char)usDataSz;
            MsgBuf.nr = MsgBuf.a  = MsgBuf.f  = MsgBuf.e  = 0;
            MsgBuf.b  = DNM_Get_Set_Attribute;

            MsgBuf.device_adr = ucMacID;
            MsgBuf.data_area  = (unsigned char)usClsId;
            MsgBuf.data_adr   = usInstId;
            MsgBuf.data_idx   = ucAttrId;
            MsgBuf.data_cnt   = (unsigned char)usDataSz;
            MsgBuf.data_type  = 0;
            MsgBuf.function   = TASK_TFC_WRITE;

            memcpy(MsgBuf.d, pvData, usDataSz);

            sStatus = DevPutMessage(pCIFIntf->GetBoardNum(), (MSG_STRUC*)&MsgBuf, 500L);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            sStatus = DevGetMessage(pCIFIntf->GetBoardNum(), sizeof(MsgBuf), (MSG_STRUC*)&MsgBuf, 500L);
            // Handle device errors
            if ( MsgBuf.f > DERR_OK && MsgBuf.f <= DERR_VENDSPEC )
                iErr = SetError(ERR_EXPLCT, ucMacID, MsgBuf.f, MsgBuf.d[0]);
            // Handle DEVICE task errors
            else iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, pCIFIntf->GetBoardNum(), ucMacID);

            iErr = Diagnostics();
          }
          else iErr = SetError(ERR_NOALOC, ucMacID);
        }
        else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
      }
      else iErr = SetError(ERR_INOPER, "GetAttribute");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::ExecService(
    unsigned char  ucSrvCode,
    unsigned short usClsId,
    unsigned short usInstId,
    unsigned short usDataSz,
    void           *pvData) {
    int iErr = 0;

    if ( ISPTRVALID(pInterface, CInterface) ) {
      if ( pInterface->IsActive() ) {
        if ( pInterface->IsA("CCIFInterface") ) {
          if ( bActive ) {
            CCIFInterface *pCIFIntf = (CCIFInterface*)pInterface;
            short sStatus = 0;
            RCS_MESSAGETELEGRAM_10 MsgBuf;

            MsgBuf.rx = 3;
            MsgBuf.tx = 16;
            MsgBuf.ln = sizeof(RCS_MESSAGETELEGRAMHEADER_10) + (unsigned char)usDataSz;
            MsgBuf.nr = MsgBuf.a  = MsgBuf.f  = MsgBuf.e  = 0;
            MsgBuf.b  = 79;

            MsgBuf.device_adr = ucMacID;
            MsgBuf.data_area  = (unsigned char)usClsId;
            MsgBuf.data_adr   = usInstId;
            MsgBuf.data_idx   = 0;
            MsgBuf.data_cnt   = (unsigned char)usDataSz;
            MsgBuf.data_type  = 0;
            MsgBuf.function   = ucSrvCode;
            memmove(MsgBuf.d, pvData, usDataSz);

            sStatus = DevPutMessage(pCIFIntf->GetBoardNum(), (MSG_STRUC*)&MsgBuf, 500L);
            iErr = SetError(ERR_CIF, sStatus, 0, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            sStatus = DevGetMessage(pCIFIntf->GetBoardNum(), sizeof(MsgBuf), (MSG_STRUC*)&MsgBuf, 500L);
            // Handle device errors
            if ( MsgBuf.f > DERR_OK && MsgBuf.f <= DERR_VENDSPEC )
                iErr = SetError(ERR_EXPLCT, ucMacID, MsgBuf.f, MsgBuf.d[0]);
            // Handle DEVICE task errors
            else iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, pCIFIntf->GetBoardNum(), ucMacID);
            if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
                return iErr;

            iErr = Diagnostics();
          }
          else iErr = SetError(ERR_NOALOC, ucMacID);
        }
        else iErr = SetError(ERR_INVITF, ucMacID, "CCIFInterface");
      }
      else iErr = SetError(ERR_INOPER, "GetAttribute");
    }
    else iErr = SetError(ERR_INVPTR, ucMacID, "pInterface", pInterface);

    return iErr;
}

int CCIFDevice::Reset(void) {
    char cBuf = 0;

    return ExecService(5, 1, 1, sizeof(cBuf), &cBuf);
}

CCIFDevice::~CCIFDevice() {
    if ( bActive )
        Unallocate();
}

