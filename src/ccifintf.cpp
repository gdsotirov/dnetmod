/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : ccifintf.cpp              Type        : source            *
 *  Description : CCIFInterface class implementation.                       *
 *  CVS         : $Id: ccifintf.cpp,v 1.2 2008/08/17 19:15:58 gsotirov Exp $
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "ccifintf.h"

#if defined(OS_LINUX)
#include "cif_user.h"
#elif defined(OS_WIN32)
/* APIENTRY and DWORD are definied here to break need of inclusion
 * of windows.h because of cifuser.h. */
#ifndef APIENTRY
#if (_MSC_VER >= 800)
#define APIENTRY            __stdcall
#else
#define APIENTRY
#endif
#endif

#ifndef DWORD
typedef unsigned long       DWORD;
#endif

#include "cifuser.h"
#endif /* elif defined(OS_WIN32) */
#include "rcs_user.h"
#include "dnm_user.h"

#define F_COMERR    0x08
#define F_COM       0x20
#define F_RUN       0x40
#define F_RDY       0x80

#define BR_TO_CONST(br) (                                       \
    (br == DEVICENET_BAUD_500K) ? DNM_BAUD_500 :                \
    (br == DEVICENET_BAUD_500K) ? DNM_BAUD_250 : DNM_BAUD_125 )

unsigned long CCIFInterface::ulClassID = 402;
char CCIFInterface::strClassName[] = "CCIFInterface";

CCIFInterface::CCIFInterface() : CInterface() {
    usBoardNum = 0;
    bAutoClear = false; /* default */
    usInputOffset = usOutputOffset = 0;
}

CCIFInterface::CCIFInterface(
    unsigned char  ucMID,
    unsigned char  ucCCS,
    unsigned char  ucPCS,
    unsigned char  ucBR,
    unsigned short usBrdNum)
: CInterface(ucMID, ucCCS, ucPCS, ucBR) {
    usBoardNum = 0;
    SetBoardNum(usBrdNum);
    bAutoClear = DNM_ACLR_INACTIVE;
    usInputOffset = usOutputOffset = 0;
}

void CCIFInterface::SetBoardNum(unsigned short usBrdNum) {
    if ( !bActive && (usBrdNum < MAX_DEV_BOARDS) )
        usBoardNum = usBrdNum;
}

void CCIFInterface::SetAutoClear(bool bAutoClr) {
    if ( !bActive )
        bAutoClr ? bAutoClear = DNM_ACLR_ACTIVE : bAutoClear = DNM_ACLR_INACTIVE;
}

bool CCIFInterface::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CInterface::IsA(ulCompareID);
}

bool CCIFInterface::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CInterface::IsA(strCompareName);
}

/* Function: CCIFInterface::SetProtocolParameters
 * Purpose : Set protocol parameters in DEVICE.
 */
inline int CCIFInterface::SetProtocolParameters(void) {
    short sStatus = 0;
    int   iErr    = 0;
    DNM_INIT_PARAMETERS IniParam;

    IniParam.bMode           = DNM_SET_MODE_BUFFERED_DEVICE_CONTROLLED;
    IniParam.usWatchDogTime  = 1000;
    //IniParam.bExtSlaveStatus = ...

    sStatus = DevPutTaskParameter(usBoardNum, 2, sizeof(IniParam), &IniParam);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    if ( sStatus < 0 || sStatus >= 1000 )
        return iErr;

    sStatus = DevReset(usBoardNum, WARMSTART, 8000L);
    return SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
}

/* Function: CCIFInterface::ClearDEVDB
 * Purpose : Clear the DEVICE DataBase.
 */
inline int CCIFInterface::ClearDEVDB(void) {
    int   iErr    = 0;
    short sStatus = 0;
    RCS_MESSAGE MsgBuf;

    MsgBuf.rx   = 0;        // OS Task
    MsgBuf.tx   = 16;       // PC Programm
    MsgBuf.ln   = 2;        // 2 Bytes length
    MsgBuf.nr   = MsgBuf.a = MsgBuf.f = MsgBuf.e = 0;
    MsgBuf.b    = 6;        // Database access
    MsgBuf.d[0] = 4;        // Clear database
    MsgBuf.d[1] = 8;        // Offset

    sStatus = DevPutMessage(usBoardNum, (MSG_STRUC*)&MsgBuf, 500L);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    sStatus = DevGetMessage(usBoardNum, sizeof(MsgBuf), (MSG_STRUC*)&MsgBuf, 3000L);
    return SetError(ERR_CIF, sStatus, MsgBuf.f, usBoardNum, ucMacID);
}

/* Function: CCIFInterface::DownloadParameters
 * Purpose : Download parameters to DEVICE.
 */
inline int CCIFInterface::DownloadParameters(void) {
    short                sStatus       = 0;
    int                  iErr          = 0;
    RCS_MESSAGE          MsgBuf;
    DNM_DOWNLOAD_REQUEST *pDownloadReq = NULL;
    BUS_DNM              *pBusParam    = NULL;

    MsgBuf.rx = 3;
    MsgBuf.tx = 16;
    MsgBuf.nr = MsgBuf.a = MsgBuf.f = MsgBuf.e = 0;
    MsgBuf.b  = DNM_Download;

    pDownloadReq = (DNM_DOWNLOAD_REQUEST *)&MsgBuf.d[0];

    pDownloadReq->bReq_Add     = 0;
    pDownloadReq->bArea_Code   = DNM_DEVICE_PRM;
    pDownloadReq->usAdd_Offset = 0;

    pBusParam = (BUS_DNM *)&pDownloadReq->abData[0];

    pBusParam->bOwnMacId  = ucMacID;
    pBusParam->usVendorId = 283;    // Hilscher Vendor ID
    pBusParam->bBaudrate  = BR_TO_CONST(ucBaudRate);
    pBusParam->bAutoClear = bAutoClear;
    pBusParam->bSrvConsConnSize = ucConsumedConnSize;
    pBusParam->usConsOffset     = usInputOffset;
    pBusParam->bSrvProdConnSize = ucProducedConnSize;
    pBusParam->usProdOffset     = usOutputOffset;

    MsgBuf.ln = sizeof(BUS_DNM) + sizeof(DNM_DOWNLOAD_REQUEST) - MAX_LEN_DATA_UNIT;

    sStatus = DevPutMessage(usBoardNum, (MSG_STRUC *)&MsgBuf, 500L);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID, 0);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    sStatus = DevGetMessage(usBoardNum, sizeof(MsgBuf), (MSG_STRUC *)&MsgBuf, 3000L);
    iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, usBoardNum, ucMacID, 0);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    return iErr;
}

/* Function: CCIFInterface::Open
 * Purpose : Configure and get operative a CIF DEVICE.
 * Remarks : This function uses algorithm for configuring and getting operative
 *           a CIF DEVICE described in the document "DeviceNet Master -
 *           Protocol Interface Manual" (dnm_pim.pdf) on page 85 (7.1).
 */
int CCIFInterface::Open(void) {
    short           sStatus = 0;
    int             iErr    = 0;
    DRIVERINFO      DrvInfo;
    DNM_DIAGNOSTICS DevDiag;

    if ( !bActive ) {
        // STAGE1: Open driver
#if defined(OS_LINUX)
        sStatus = DevOpenDriver();
#elif defined(OS_WIN32)
        sStatus = DevOpenDriver(0);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
        if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
            return iErr;

        // STAGE2: Initialize board
#if defined(OS_LINUX)
        sStatus = DevInitBoard(usBoardNum);
#elif defined(OS_WIN32)
        sStatus = DevInitBoard(usBoardNum, NULL);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
        if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
            return iErr;

        STAGE3: //: Set up protocol parameter
        iErr = SetProtocolParameters();
        if ( iErr != ERR_NOERR )
            return Close();

        // STAGE5: Read driver state
        sStatus = DevGetInfo(usBoardNum, GET_DRIVER_INFO, sizeof(DrvInfo), &DrvInfo);
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
        if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
            return Close();

        // STAGE6: Check for bit RUN and RDY
        if ( (DrvInfo.bHostFlags & F_RUN) && (DrvInfo.bHostFlags & F_RDY) ) {
        // STAGE7: Clear the database in DEVICE
            iErr = ClearDEVDB();
            if ( iErr == ERR_NOERR )
                goto STAGE3;
            else return Close();
        }
    }
    // STAGE8: Download parameters to DEVICE
    iErr = DownloadParameters();
    if ( iErr != ERR_NOERR )
        return iErr;

    if ( !bActive )  // Signal DEVICE that application is running
        sStatus = DevSetHostState(usBoardNum, HOST_READY, 1000L);

    // Read diagnostic information
    sStatus = DevGetTaskState(usBoardNum, 2, sizeof(DevDiag), &DevDiag);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return Close();

    // Check DEVICE global state
    bActive = DevDiag.bDNM_state == OPERATE;

    return iErr;
}

/* Function: CCIFInterface::Close
 * Purpose : Stop communication and exit board.
 */
int CCIFInterface::Close(void) {
    int iErr = 0;

    if ( bActive ) {
        short sStatus = 0;

        sStatus = DevSetHostState(usBoardNum, HOST_NOT_READY, 1000L);
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);

        sStatus = DevExitBoard(usBoardNum);
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);

#if defined(OS_LINUX)
        sStatus = DevCloseDriver();
#elif defined(OS_WIN32)
        sStatus = DevCloseDriver(0);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    }

    return iErr;
}

int CCIFInterface::Reset(void *vpParam) {
    short sStatus            = 0;
    int iErr                 = 0;
    unsigned short usMode    = *((unsigned short *)vpParam);
    unsigned long  ulTimeout = 0;

    if ( !bActive ) {
#if defined(OS_LINUX)
        sStatus = DevOpenDriver();
#elif defined(OS_WIN32)
        sStatus = DevOpenDriver(0);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
        if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
            return iErr;

#if defined(OS_LINUX)
        sStatus = DevInitBoard(usBoardNum);
#elif defined(OS_WIN32)
        sStatus = DevInitBoard(usBoardNum, 0);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
        if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
            return iErr;
    }

    if ( usMode == COLDSTART || usMode == BOOTSTART )
        ulTimeout = 10000;
    else if ( usMode == WARMSTART )
        ulTimeout = 8000;
    else {
        char cBuf[6] = {0};
        sprintf(cBuf, "%hu", usMode);
        SetError(ERR_INVFPRM, "vpParam", cBuf, "Reset");
    }

    sStatus = DevReset(usBoardNum, usMode, ulTimeout);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    if ( !bActive ) {
        sStatus = DevExitBoard(usBoardNum);
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);

#if defined(OS_LINUX)
        sStatus = DevCloseDriver();
#elif defined(OS_WIN32)
        sStatus = DevCloseDriver(0);
#endif
        iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    }

    return iErr;
}

CCIFInterface::~CCIFInterface() {
    if ( bActive )
        Close();
}

