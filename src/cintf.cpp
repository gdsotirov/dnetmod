/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cintf.cpp                 Type        : source            *
 *  Description : CInterface class implementation.                          *
 *  CVS         : $Id: cintf.cpp,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#include <string.h>

#include "cintf.h"

#define VALID_BR(br) ( ucBR == DEVICENET_BAUD_125K ||\
                       ucBR == DEVICENET_BAUD_250K ||\
                       ucBR == DEVICENET_BAUD_500K )

unsigned long CInterface::ulClassID = 301;
char CInterface::strClassName[] = "CInterface";

CInterface::CInterface() : CNode() {
    ucBaudRate = 0;
}

CInterface::CInterface(
    unsigned char ucMID,
    unsigned char ucCCS,
    unsigned char ucPCS,
    unsigned char ucBR)
: CNode(ucMID, ucCCS, ucPCS) {
    ucBaudRate = 0;
    SetBaudRate(ucBR);
}

void CInterface::SetBaudRate(unsigned char ucBR) {
    if ( !bActive && VALID_BR(ucBR) )
        ucBaudRate = ucBR;
    else ucBaudRate = DEVICENET_BAUD_250K; /* default */
}

bool CInterface::IsA(unsigned long ulCompareID) const {
    return (ulCompareID == ulClassID) ? true : CIdentificator::IsA(ulCompareID);
}

bool CInterface::IsA(char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CIdentificator::IsA(strCompareName);
}

CInterface::~CInterface() {}

