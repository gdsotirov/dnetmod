/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cniintf.cpp               Type        : source            *
 *  Description : CNIInterface class implementation.                        *
 *  CVS         : $Id: cniintf.cpp,v 1.2 2008/08/17 19:15:58 gsotirov Exp $
 ****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "dnmdefs.h"
#include "dnmerrs.h"
#include "cniintf.h"

#include "nidnet.h"

#define BAUD_125K           1
#define BAUD_250K           2
#define BAUD_500K           3

#define BR_TO_CONST(br) (                                   \
    (br == DEVICENET_BAUD_500K) ? BAUD_500K :               \
    (br == DEVICENET_BAUD_250K) ? BAUD_250K : BAUD_125K )

unsigned long CNIInterface::ulClassID = 401;
char CNIInterface::strClassName[] = "CNIInterface";

CNIInterface::CNIInterface() : CInterface() {
    ucIntfID = 0;
    ulNIDNET_ID = 0;
}

CNIInterface::CNIInterface(
    unsigned char ucMID,
    unsigned char ucBR,
    unsigned char ucIID)
: CInterface(ucMID, 0, 0, ucBR) {
    ucIntfID = 0;
    SetIntfID(ucIID);
    ulNIDNET_ID = 0;
}

void CNIInterface::SetIntfID(unsigned char ucIID) {
    if ( !bActive && (ucIID < 32) )
        ucIntfID = ucIID;
}

void CNIInterface::GetName(unsigned long ulStrSz, char *strName) {
    char strIName[7] = "dnet";
    char strID[3] = {0};

    itoa(ucIntfID, strID, 10);
    strcat(strIName, strID);
    strncpy(strName, strIName, ulStrSz);
}

bool CNIInterface::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CInterface::IsA(ulCompareID);
}

bool CNIInterface::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CInterface::IsA(strCompareName);
}

/* Function: CNIInterface::Open
 * Purpose : Open NI-DNET interface object and start communication.
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

