/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cnode.cpp                 Type        : header            *
 *  Description : CNode class implementation.                               *
 ****************************************************************************/

/**
 * @file cnode.cpp
 * @brief CNode class implementation.
 */

#include <string.h>

#include "cnode.h"

unsigned long CNode::ulClassID = 201;
char CNode::strClassName[] = "CNode";

/**
 * @brief Default constructor
 *
 * Initializes all members accordingly.
 */
CNode::CNode() : CIdentificator() {
    bActive = false;
    ucMacID = 0;
    ucConsumedConnSize = 0;
    ucProducedConnSize = 0;
}

/**
 * @brief Constructor with parameters
 *
 * Initializes members with provided values.
 * @param ucMID MAC ID of the node.
 * @param ucCCS Consumed connection size of the node.
 * @param ucPCS Produced connection size of the node.
 */
CNode::CNode(unsigned char ucMID, unsigned char ucCCS, unsigned char ucPCS) {
    bActive = false;
    ucMacID = 0;
    SetMacID(ucMID);
    ucConsumedConnSize = ucCCS;
    ucProducedConnSize = ucPCS;
}

/**
 * @brief Set MAC ID of the node
 *
 * Use this functions to set MAC ID of the node.
 * @remark The function will set the new value only if node is not active.
 * @param ucMID MAC ID of the node.
 */
void CNode::SetMacID(unsigned char ucMID) {
    if ( !bActive && ucMID < DEVICENET_MAX_DEVICES )
        ucMacID = ucMID;
}

/**
 * @brief Sets count of consumed bytes by the node from the network
 *
 * Use this functions to set the count of bytes consumed by the node from
 * the network.
 * @param ucCCS Consumed data bytes from the network.
 */
void CNode::SetConsumedConnSize(unsigned char ucCCS) {
    ucConsumedConnSize = ucCCS;
}

/**
 * @brief Set count of node produced bytes to the network
 *
 * Use this functions to set count of produced bytes by the node to
 * the network.
 * @param ucPCS Node produced data bytes to the network.
 */
void CNode::SetProducedConnSize(unsigned char ucPCS) {
    ucProducedConnSize = ucPCS;
}

/**
 * @brief Checks if class can identify itself with the specified number.
 *
 * If not then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CNode::IsA(unsigned long ulCompareID) const {
    return (ulClassID == ulCompareID) ? true : CIdentificator::IsA(ulCompareID);
}

/**
 * @brief Checks if class can identify itself with the specified name.
 *
 * If not then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CNode::IsA(const char *strCompareName) const {
    return (!strcmp(strClassName, strCompareName)) ? true : CIdentificator::IsA(strCompareName);
}

/**
 * @brief Destructor
 *
 * Does nothing.
 */
CNode::~CNode() {}

