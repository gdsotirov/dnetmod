/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cdevice.cpp               Type        : source            *
 *  Description : CDevice class implementation.                             *
 ****************************************************************************/

/**
 * @file cdevice.cpp
 * @brief CDevice class implementation.
 */

#include <string.h>

#include "dnmdefs.h"

#include "cdevice.h"

/**
 * @brief Validates connection type values
 */
#define VALID_CT(ct) ( ct == DEVICENET_CONN_POLLED ||\
                       ct == DEVICENET_CONN_STRBED ||\
                       ct == DEVICENET_CONN_COS    ||\
                       ct == DEVICENET_CONN_CYCLIC )

unsigned long CDevice::ulClassID = 351;
char CDevice::strClassName[] = "CDevice";

/**
 * @brief Default constructor
 *
 * Initializes members accordingly.
 */
CDevice::CDevice() : CNode() {
    ucConnType = 0;
    usEPR = 0;
    pInterface = 0;
}

/**
 * @brief Constructors with parameters
 *
 * Constructs a new device initializing the members with the parameters.
 * First three parameters are for initializing of CNode.
 * @param ucMID MAC ID of the device.
 * @param ucCCS Consumed connection size of the device.
 * @param ucPCS Produced connection size of the device.
 * @param ucCT Connection type. Possible values are same as for function CDevice::SetConnType.
 * @param usEPR_ Expected Packed Rate (EPR) from device. See #usEPR.
 * @param pIntf Pointer to interface to which the device is connected.
 */
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

/**
 * @brief Sets connection type
 *
 * This functions can be used set the I/O connection type established to
 * device with CDevice::Allocate function. If the provided connection type is
 * invalid the function sets connection type Polled.
 * @remark Connection type can be set only if device is not active on
 * the network (i.e. not allocated).
 * @param ucCT Connection type to be set. Possible values for this parameter
 * defined in defined in dnmdefs.h are:
 * <ul>
 *   <li><code>DEVICENET_CONN_POLLED</code> - 0x0001</li>
 *   <li><code>DEVICENET_CONN_STRBED</code> - 0x0002</li>
 *   <li><code>DEVICENET_CONN_COS</code> - 0x0004</li>
 *   <li><code>DEVICENET_CONN_CYCLIC</code> - 0x0008</li>
 * </ul>
 */
void CDevice::SetConnType(unsigned char ucCT) {
    if ( !bActive && VALID_CT(ucCT) )
        ucConnType = ucCT;
    else ucConnType = DEVICENET_CONN_POLLED; /* default */
}

/**
 * @brief Sets Expected Packet Rate
 *
 * This function can be used to set the EPR for the I/O connection established
 * to device.
 * @remark EPR can be set only when device is not active on the network
 * (i.e. not allocated).
 * @param usEPR_ EPR value.
 */
void CDevice::SetEPR(unsigned short usEPR_) {
    if ( !bActive )
        usEPR = usEPR_;
}

/**
 * @brief Checks if class can identify itself with the specified number.
 *
 * If not then passes the check to the base class.
 * @param ulCompareID ID to be compared.
 * @return True when match otherwise false.
 */
bool CDevice::IsA(unsigned long ulCompareID) const {
    return ( ulCompareID == ulClassID ) ? true : CIdentificator::IsA(ulCompareID);
}

/**
 * @brief Checks if class can identify itself with the specified name.
 *
 * If not then passes the check to the base class.
 * @param strCompareName Name to be compared.
 * @return True when match otherwise false.
 */
bool CDevice::IsA(const char *strCompareName) const {
    return ( !strcmp(strClassName, strCompareName) ) ? true : CIdentificator::IsA(strCompareName);
}

/**
 * @brief Destructor
 *
 * Does nothing.
 * @remarks It is good to unallocate device from the network before destroying
 * a descendant object. A call to CDevice::Unallocate function if not invoked by
 * the user is proposed for implementing in descendants destructors.
 */
CDevice::~CDevice() {}

