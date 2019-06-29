/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cintf.cpp                 Type        : source            *
 *  Description : CInterface class implementation.                          *
 ****************************************************************************/

/**
 * @file cintf.cpp
 * @brief CInterface class implementation.
 */

#include <string.h>

#include "cintf.h"

/**
 * @brief Validates baud rate values
 */
#define VALID_BR(br) ( br == DEVICENET_BAUD_125K ||\
                       br == DEVICENET_BAUD_250K ||\
                       br == DEVICENET_BAUD_500K )

unsigned long CInterface::ulClassID = 301;
char CInterface::strClassName[] = "CInterface";

/**
 * @brief Default constructor
 *
 * Initializes members accordingly.
 */
CInterface::CInterface() : CNode() {
    ucBaudRate = 0;
}

/**
 * @brief Constructor with parameters
 *
 * First three parameters are for initializing of CNode.
 * @param ucMID MAC ID of the node.
 * @param ucCCS Consumed connection size of the node.
 * @param ucPCS Produced connection size of the node.
 * @param ucBR Baud rate for the interface. Possible values are same as for CInterface::SetBaudRate.
 */
CInterface::CInterface(
    unsigned char ucMID,
    unsigned char ucCCS,
    unsigned char ucPCS,
    unsigned char ucBR)
: CNode(ucMID, ucCCS, ucPCS) {
    ucBaudRate = 0;
    SetBaudRate(ucBR);
}

/**
 * @brief Set baud rate for the interface
 *
 * Use this functions to set baud rate of the interface. If provided baud rate
 * is invalid the function sets baud rate of 250 Kb.
 * @remark The function will set the new value only if the interface is not
 * active.
 * @param ucBR New baud rate of the interface. Possible values are defined as constants in dnmdefs.h:
 * <ul>
 * <li><code>DEVICENET_BAUD_125K</code> - for 125 000 bytes per second</li>
 * <li><code>DEVICENET_BAUD_250K</code> - for 250 000 bytes per second</li>
 * <li><code>DEVICENET_BAUD_500K</code> - for 500 000 bytes per second</li>
 * </ul>
 */
void CInterface::SetBaudRate(unsigned char ucBR) {
    if ( !bActive && VALID_BR(ucBR) )
        ucBaudRate = ucBR;
    else ucBaudRate = DEVICENET_BAUD_250K; /* default */
}

/**
 * @brief Checks if class can identify itself with the specified number.
 *
 * If not then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CInterface::IsA(unsigned long ulCompareID) const {
    return (ulCompareID == ulClassID) ? true : CIdentificator::IsA(ulCompareID);
}

/**
 * @brief Checks if class can identify itself with the specified name.
 *
 * If not then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CInterface::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CIdentificator::IsA(strCompareName);
}

/**
 * @brief Destructor
 *
 * Does nothing.
 * @remarks It is good to stop communication on the interface and to do
 * finalization on it when destroying an object of class descended from
 * CInterface. A call to Close method, if not invoked by the user, is proposed
 * for implementing in descendants destructors.
 */
CInterface::~CInterface() {}

