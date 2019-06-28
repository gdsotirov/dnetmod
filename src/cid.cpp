/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cid.cpp                   Type        : source            *
 *  Description : CIdentificator class implementation.                      *
 ****************************************************************************/

#include <string.h>

#include "cid.h"

unsigned long CIdentificator::ulClassID = 101;
char CIdentificator::strClassName[] = "CIdentificator";

CIdentificator::CIdentificator() {}

void CIdentificator::GetClassName(unsigned long ulStrSz, char *strName) const {
    strncpy(strName, strClassName, ulStrSz);
}

bool CIdentificator::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID );
}

bool CIdentificator::IsA(const char *strCompareName) const {
    return ( strcmp(strCompareName, strClassName) == 0 );
}

CIdentificator::~CIdentificator() {}

