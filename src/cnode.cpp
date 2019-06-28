/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cnode.cpp                 Type        : header            *
 *  Description : CNode class implementation.                               *
 ****************************************************************************/

#include <string.h>

#include "cnode.h"

unsigned long CNode::ulClassID = 201;
char CNode::strClassName[] = "CNode";

CNode::CNode() : CIdentificator() {
    bActive = false;
    ucMacID = 0;
    ucConsumedConnSize = 0;
    ucProducedConnSize = 0;
}

CNode::CNode(unsigned char ucMID, unsigned char ucCCS, unsigned char ucPCS) {
    bActive = false;
    ucMacID = 0;
    SetMacID(ucMID);
    ucConsumedConnSize = ucCCS;
    ucProducedConnSize = ucPCS;
}

void CNode::SetMacID(unsigned char ucMID) {
    if ( !bActive && ucMID < DEVICENET_MAX_DEVICES )
        ucMacID = ucMID;
}

void CNode::SetConsumedConnSize(unsigned char ucCCS) {
    ucConsumedConnSize = ucCCS;
}

void CNode::SetProducedConnSize(unsigned char ucPCS) {
    ucProducedConnSize = ucPCS;
}

bool CNode::IsA(unsigned long ulCompareID) const {
    return (ulClassID == ulCompareID) ? true : CIdentificator::IsA(ulCompareID);
}

bool CNode::IsA(const char *strCompareName) const {
    return (!strcmp(strClassName, strCompareName)) ? true : CIdentificator::IsA(strCompareName);
}

CNode::~CNode() {}

