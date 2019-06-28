/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cdevice.cpp               Type        : source            *
 *  Description : CDevice class implementation.                             *
 ****************************************************************************/

#include <string.h>

#include "dnmdefs.h"

#include "cdevice.h"

unsigned long CDevice::ulClassID = 351;
char CDevice::strClassName[] = "CDevice";

CDevice::CDevice() : CNode() {
    ucConnType = 0;
    usEPR = 0;
    pInterface = 0;
}

CDevice::CDevice(
    unsigned char  ucMID,
    unsigned char  ucCCS,
    unsigned char  ucPCS,
    unsigned char  ucCT,
    unsigned short usEPR_,
    CInterface     *pIntf)
: CNode(ucMID, ucCCS, ucPCS) {
    ucConnType = 0;
    SetConnType(ucCT);
    usEPR = usEPR_;
    pInterface = pIntf;
}

void CDevice::SetConnType(unsigned char ucCT) {
    if ( !bActive )
        ucConnType = ucCT;
}

void CDevice::SetEPR(unsigned short usEPR_) {
    if ( !bActive )
        usEPR = usEPR_;
}

bool CDevice::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CIdentificator::IsA(ulCompareID);
}

bool CDevice::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CIdentificator::IsA(strCompareName);
}

CDevice::~CDevice() {}

