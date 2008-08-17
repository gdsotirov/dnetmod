/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cid.cpp                   Type        : source            *
 *  Description : CIdentificator class implementation.                      *
 *  CVS         : $Id: cid.cpp,v 1.2 2008/08/17 19:15:58 gsotirov Exp $
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

