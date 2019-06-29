/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : ccifintf.cpp              Type        : source            *
 *  Description : CCIFInterface class implementation.                       *
 ****************************************************************************/

/**
 * @file ccifintf.cpp
 * @brief CCIFInterface class implementation.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "ccifintf.h"

#if defined(OS_LINUX)
#include "cif_user.h"
#elif defined(OS_WIN32)
/* APIENTRY and DWORD are defined here to break need of inclusion
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

/** Flag communication error */
#define F_COMERR    0x08
/** Flag communication */
#define F_COM       0x20
/** Flag run */
#define F_RUN       0x40
/** Flag ready */
#define F_RDY       0x80

/**
 * @brief Converts module baud rate to CIF constants
 */
#define BR_TO_CONST(br) (                                       \
    (br == DEVICENET_BAUD_500K) ? DNM_BAUD_500 :                \
    (br == DEVICENET_BAUD_500K) ? DNM_BAUD_250 : DNM_BAUD_125 )

unsigned long CCIFInterface::ulClassID = 402;
char CCIFInterface::strClassName[] = "CCIFInterface";

/**
 * @brief Default constructor
 *
 * Initializes members accordingly.
 */
CCIFInterface::CCIFInterface() : CInterface() {
    usBoardNum = 0;
    bAutoClear = false; /* default */
    usInputOffset = usOutputOffset = 0;
}

/**
 * @brief Constructor with parameters
 *
 * Initializes members from parameters.
 * @param ucMID MAC ID of the interface node.
 * @param ucCCS Consumed connection size of the interface node.
 * @param ucPCS Produced connection size of the interface node.
 * @param ucBR Baud rate for interface.
 * @param usBrdNum Board number. Must be a number between 0 and 3.
 */
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

/**
 * @brief Sets Hilscher board number.
 *
 * Used to set the number of the Hilscher board.
 * @param usBrdNum Hilscher board number. Must be a value between 0 and 3.
 */
void CCIFInterface::SetBoardNum(unsigned short usBrdNum) {
    if ( !bActive && (usBrdNum < MAX_DEV_BOARDS) )
        usBoardNum = usBrdNum;
}

/**
 * @brief Sets AutoClear flag
 *
 * Used to set the value of AutoClear flag.
 * @param bAutoClr True or false.
 */
void CCIFInterface::SetAutoClear(bool bAutoClr) {
    if ( !bActive )
        bAutoClr ? bAutoClear = DNM_ACLR_ACTIVE : bAutoClear = DNM_ACLR_INACTIVE;
}

/**
 * @brief Checks if class can identify itself with the specified number.
 *
 * If not then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CCIFInterface::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CInterface::IsA(ulCompareID);
}

/**
 * @brief Checks if class can identify itself with the specified name.
 *
 * If not then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CCIFInterface::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CInterface::IsA(strCompareName);
}

/**
 * @brief Writes communication protocol parameters in device
 *
 * Prepares communication parameters and writes them to the board.
 * @return Error from \ref SetError function.
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

/**
 * @brief Clears device's DataBase
 *
 * Prepares RCS message for clearing of database and sends it to device.
 * @return Error from \ref SetError function.
 */
inline int CCIFInterface::ClearDEVDB(void) {
    int   iErr    = 0;
    short sStatus = 0;
    RCS_MESSAGE MsgBuf;

    MsgBuf.rx   = 0;        // OS Task
    MsgBuf.tx   = 16;       // PC Program
    MsgBuf.ln   = 2;        // 2 Bytes length
    MsgBuf.nr   = MsgBuf.a = MsgBuf.f = MsgBuf.e = 0;
    MsgBuf.b    = 6;        // Database access
    MsgBuf.d[0] = 4;        // Clear database
    MsgBuf.d[1] = 8;        // Offset

    sStatus = DevPutMessage(usBoardNum, reinterpret_cast<MSG_STRUC *>(&MsgBuf), 500L);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    sStatus = DevGetMessage(usBoardNum, sizeof(MsgBuf), reinterpret_cast<MSG_STRUC *>(&MsgBuf), 3000L);
    return SetError(ERR_CIF, sStatus, MsgBuf.f, usBoardNum, ucMacID);
}

/**
 * @brief Downloads parameters to device.
 *
 * Prepares RCS message with download request for setting bus parameters and
 * sends the message to devlice.
 * @return Error from \ref SetError function.
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

    pDownloadReq = reinterpret_cast<DNM_DOWNLOAD_REQUEST *>(&MsgBuf.d[0]);

    pDownloadReq->bReq_Add     = 0;
    pDownloadReq->bArea_Code   = DNM_DEVICE_PRM;
    pDownloadReq->usAdd_Offset = 0;

    pBusParam = reinterpret_cast<BUS_DNM *>(&pDownloadReq->abData[0]);

    pBusParam->bOwnMacId  = ucMacID;
    pBusParam->usVendorId = 283;    // Hilscher Vendor ID
    pBusParam->bBaudrate  = BR_TO_CONST(ucBaudRate);
    pBusParam->bAutoClear = bAutoClear;
    pBusParam->bSrvConsConnSize = ucConsumedConnSize;
    pBusParam->usConsOffset     = usInputOffset;
    pBusParam->bSrvProdConnSize = ucProducedConnSize;
    pBusParam->usProdOffset     = usOutputOffset;

    MsgBuf.ln = sizeof(BUS_DNM) + sizeof(DNM_DOWNLOAD_REQUEST) - MAX_LEN_DATA_UNIT;

    sStatus = DevPutMessage(usBoardNum, reinterpret_cast<MSG_STRUC *>(&MsgBuf), 500L);
    iErr = SetError(ERR_CIF, sStatus, 0, usBoardNum, ucMacID, 0);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    sStatus = DevGetMessage(usBoardNum, sizeof(MsgBuf), reinterpret_cast<MSG_STRUC *>(&MsgBuf), 3000L);
    iErr = SetError(ERR_CIF, sStatus, MsgBuf.f, usBoardNum, ucMacID, 0);
    if ( sStatus < 0 || sStatus >= DRV_RCS_ERROR_OFFSET )
        return iErr;

    return iErr;
}

/**
 * @brief Configures and gets operative a CIF device.
 *
 * This function uses algorithm for configuring and getting operative
 * a CIF DEVICE described in the document "DeviceNet Master -
 * Protocol Interface Manual" (dnm_pim.pdf) on page 85 (7.1).
 * @return Error from \ref SetError function.
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

/**
 * @brief Stops communication and exits the board.
 *
 * The function sets host's state to 'not ready', closes the connection to
 * the device board and closes the connection to the driver.
 * @return Error from \ref SetError function.
 */
int CCIFInterface::CloseInterface(void) {
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

/**
 * @brief Stops communication on interface
 * @return Error from \ref SetError function.
 */
int CCIFInterface::Close(void) {
  return CloseInterface();
}

/**
 * @brief Resets the device board
 *
 * If not active this function opens a connection to the device driver and
 * initializes the board. Then makes reset of the device with the specified
 * reset mode. Finally if not active closes the connection to the board and
 * the device driver.
 * @param vpParam Reset mode. Could be either BOOTSTART, COLDSTART or
 * WARMSTART (defined in cif_user.h).
 * @return Error from \ref SetError function.
 */
int CCIFInterface::Reset(void *vpParam) {
    short sStatus            = 0;
    int iErr                 = 0;
    unsigned short usMode    = *(static_cast<unsigned short *>(vpParam));
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

/**
 * @brief Destructor.
 *
 * Closes the interface if active.
 */
CCIFInterface::~CCIFInterface() {
    if ( bActive )
        CloseInterface();
}

