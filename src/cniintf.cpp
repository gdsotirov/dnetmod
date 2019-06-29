/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cniintf.cpp               Type        : source            *
 *  Description : CNIInterface class implementation.                        *
 ****************************************************************************/

/**
 * @file cniintf.cpp
 * CNIInterface class implementation.
 */

#include <stdlib.h>
#include <string.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "cniintf.h"

#include "nidnet.h"

/** Baud rate 125 Kb */
#define BAUD_125K           1
/** Baud rate 250 Kb */
#define BAUD_250K           2
/** Baud rate 500 Kb */
#define BAUD_500K           3

/**
 * @brief Converts module baud rate to NI constant
 */
#define BR_TO_CONST(br) (                                   \
    (br == DEVICENET_BAUD_500K) ? BAUD_500K :               \
    (br == DEVICENET_BAUD_250K) ? BAUD_250K : BAUD_125K )

unsigned long CNIInterface::ulClassID = 401;
char CNIInterface::strClassName[] = "CNIInterface";

/**
 * @brief Default constructor.
 *
 * Initializes members accordingly.
 */
CNIInterface::CNIInterface() : CInterface() {
    ucIntfID = 0;
    ulNIDNET_ID = 0;
}

/**
 * @brief Constructor with parameters.
 *
 * Initializes members from parameters.
 * @param ucMID MAC ID of the interface node.
 * @param ucBR Boud rate.
 * @param ucIID Interface's identifier.
 */
CNIInterface::CNIInterface(
    unsigned char ucMID,
    unsigned char ucBR,
    unsigned char ucIID)
: CInterface(ucMID, 0, 0, ucBR) {
    ucIntfID = 0;
    SetIntfID(ucIID);
    ulNIDNET_ID = 0;
}

/**
 * @brief Sets NI-DNET™ interface identifier
 *
 * Used to set NI-DNET™ interface identifier.
 * @param ucIID NI-DNET™ interface identifier. Must be a number between 0
 * and 31.
 */
void CNIInterface::SetIntfID(unsigned char ucIID) {
    if ( !bActive && (ucIID < 32) )
        ucIntfID = ucIID;
}

/**
 * @brief Retrieves NI-DNET™ interface name
 *
 * Used to retrieve NI-DNET™ interface name. Name is a string with
 * format "dnetXX", where XX is the interface identifier. So valid names
 * are dnet1, dnet2, dnet3, ... dnet31.
 * @param ulStrSz   Size in bytes of the string passed as next parameter.
 * @param strName Pointer to a string where to copy interface name.
 */
void CNIInterface::GetName(unsigned long ulStrSz, char *strName) {
    char strIName[7] = "dnet";
    char strID[3] = {0};

    itoa(ucIntfID, strID, 10);
    strcat(strIName, strID);
    strncpy(strName, strIName, ulStrSz);
}

/**
 * Checks if class can identify itself with the specified number. If not
 * then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CNIInterface::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CInterface::IsA(ulCompareID);
}

/**
 * Checks if class can identify itself with the specified name. If not
 * then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CNIInterface::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CInterface::IsA(strCompareName);
}

/* Function: CNIInterface::Open
 * Purpose : Open NI-DNET interface object and start communication.
 */
/**
 * @brief Configure interface and start communication
 *
 * Open NI-DNET interface object and starts communication.
 * @return Error from \ref SetError function.
 */
int CNIInterface::Open(void) {
    int iErr = 0;

    if ( !bActive ) {
        int iStatus = 0;
        char strName[7] = {0};

        GetName(sizeof(strName), strName);
        iStatus = ncOpenDnetIntf(strName, ucMacID, BR_TO_CONST(ucBaudRate),
                    NC_POLL_INDIV, &ulNIDNET_ID);
        iErr = SetError(ERR_NIDNET, iStatus, ucIntfID, ucMacID);
        if ( iStatus < 0 )
            return iErr;

        iStatus = ncOperateDnetIntf(ulNIDNET_ID, NC_OP_START, 0);
        iErr = SetError(ERR_NIDNET, iStatus, ucIntfID, ucMacID);
        if ( iStatus >= 0 )
            bActive = true;
    }

    return iErr;
}

/* Function: CNIInterface::Close
 * Purpose : Stop communication on the interface and close NIDNET interface
 *           object.
 */
int CNIInterface::CloseInterface(void) {
    int iErr = 0;

    if ( bActive ) {
        int iStatus = 0;
        int iErr = 0;

        iStatus = ncOperateDnetIntf(ulNIDNET_ID, NC_OP_STOP, 0);
        iErr = SetError(ERR_NIDNET, iStatus, ucIntfID, ucMacID);

        iStatus = ncCloseObject(ulNIDNET_ID);
        iErr = SetError(ERR_NIDNET, iStatus, ucIntfID, ucMacID);

        ulNIDNET_ID = 0;
        bActive = false;
    }

    return iErr;
}

int CNIInterface::Close(void) {
  return CloseInterface();
}

/* Function: CNIInterface::Reset
 * Purpose : Reset the NI-DNET interface.
 */
int CNIInterface::Reset(void *) {
    int iStatus = 0;
    char strName[7] = {0};

    GetName(sizeof(strName), strName);
    iStatus = ncReset(strName, 0);
    return SetError(ERR_NIDNET, iStatus, ucIntfID, ucMacID);
}

CNIInterface::~CNIInterface() {
    if ( bActive )
        CloseInterface();
}

