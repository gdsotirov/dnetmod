/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cdevice.h                 Type        : header            *
 *  Description : CDevice class declaration.                                *
 ****************************************************************************/

/**
 * @file cdevice.h
 * CDevice class declaration.
 */

#ifndef CDEVICE_H
#define CDEVICE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cdevice.h requires c++ compiler."
#endif

#include "cnode.h"
#include "cintf.h"

/**
 * @brief Base and abstract class for representing a DeviceNet device
 *
 * The device class abstractly represents a slave device on the network.
 * @remark Copy constructor and assignment operator not supported for this class.
 */
class DNETMOD_API CDevice : public CNode {
private:
    CDevice(const CDevice&);
    CDevice& operator =(const CDevice&);
protected:
    /** Class's ID */
    static unsigned long ulClassID;
    /** Class's name */
    static char strClassName[];
protected:
    /** Connection type */
    unsigned char ucConnType;
    /** Expected Packet Rate (EPR) */
    unsigned short usEPR;
    /** Interface to which device is connected */
    CInterface *pInterface;
public:
    /* constructors */
    CDevice();
    CDevice(unsigned char  ucMID,
            unsigned char  ucCCS,
            unsigned char  ucPCS,
            unsigned char  ucCT,
            unsigned short usEPR_,
            CInterface     *pIntf);
    /* get/set */
    unsigned char GetConnType(void) const;
    void SetConnType(unsigned char ucCT);
    unsigned short GetEPR(void) const;
    void SetEPR(unsigned short usEPR_);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    /* main operations */
    /**
     * @brief Allocates device on the network
     *
     * This function must allow the user to allocate device on the network.
     * Allocation in that sense means to make some predefined master/slave
     * connections to device, e.g. I/O connections or Explicit Messaging
     * connection. After invoking this function the user must be capable of
     * communicating with the device.
     *
     * After establishing connections to device object must become in 'active'
     * state. Setting device class parameters at this time must be forbidden.
     * @param ucFlags This parameters enables the user to pass bit flags to
     * the function to request some additional actions to be done by
     * the function. Flags can vary from function to function depending on
     * the interface used.
     * @return An error code must be returned from this function to indicate
     * unsuccessful allocation of the device on the network. If this is not
     * possible (e.g. interface don't support error information at this time)
     * this must be done when communication starts, e.g. when some of
     * the communications functions like CDevice::ReadIOData,
     * CDevice::WriteIOData, CDevice::GetAttribute, CDevice::SetAttribute or
     * CDevice::ExecService is invoked.
     */
    virtual int Allocate(unsigned char ucFlags) = 0;
    /**
     * @brief Unallocates device off the network
     *
     * This function must allow the user to unallocate device off the network.
     * Unallocation in that sense means to stop all I/O and/or Explicit
     * Messaging connections established to device with CDevice::Allocate
     * function. After invoking this function user must not be capable of
     * communicating with the device.
     *
     * After all connections to device are stopped device object must become
     * in 'inactive' state. Setting device object parameters at this time must
     * be possible thus ensuring reusability of the classes.
     * @return Function is not meant to return a value indicating an error.
     * However, it's good to give information to the user of errors occurred
     * in the process of stop using the device on the network.
     */
    virtual int Unallocate(void) = 0;
    /**
     * @brief Reads I/O data from device
     *
     * This function must allow the user to read I/O data from device. User
     * provides a buffer with appropriate size for the data. This size must be
     * the produced connection size member on the device object. This function
     * must only try to read data if the device object is in 'active' state.
     * @remarks If size of the data received is larger than the size of
     * the buffer passed as second parameter function must copy only buffer
     * size bytes in that buffer and indicate an error message.
     * @param ulBufSz Size of the buffer.
     * @param pvBuf Pointer to the buffer where to store read I/O data.
     * @return Function must return an error code indicating failure when
     * receiving data from device. When error detected function must format an
     * error message in #errmsg global variable using \ref SetError function.
     * Error code may vary from function to function depending on
     * the interface used.
     */
    virtual int ReadIOData(unsigned long ulBufSz, void *pvBuf) = 0;
    /**
     * @brief Writes I/O data to device
     *
     * This function must allow the user to write I/O data to device. User
     * provides a buffer with appropriate size and with the data to be
     * written. This size must be the consumed connection size member on
     * the device object. Function must only try to write data if the device
     * object is in 'active' state.
     * @remarks If size of the data that user wants to send to device is
     * smaller or larger than the consumed connection size member function
     * must return an error.
     * @param ulBufSz Size of the buffer.
     * @param pvBuf Pointer to the buffer from where to read I/O data.
     * @return Function must return an error code indicating failure when
     * sending data to device. When error detected function must format an
     * error message in #errmsg global variable using \ref SetError function.
     * Error code may vary from function to function depending on
     * the interface used.
     */
    virtual int WriteIOData(unsigned long ulBufSz, void *pvBuf) = 0;
    /**
     * @brief Reads attribute from device
     *
     * This function must allow the user to retrieve device's attribute. User
     * must provide buffer with appropriate size for the attribute data.
     * @param usClsId Class identifier of the attribute.
     * @param usInstId Instance identifier of the attribute.
     * @param ucAttrId Attribute identifier.
     * @param usDataSz Length in bytes of the attribute data.
     * @param pvData Pointer to a buffer where to copy attribute data.
     * @param pusActDataSz Actual size of the attribute data in bytes.
     * @return Function must return an error code and to format an error
     * message when error detected in process of retrieving device's
     * attribute. Function must not report as an error inconsistency between
     * <code>usDataSz</code> and actual size of the attribute data. Actual
     * length of the attribute data in bytes must be fixed in
     * <code>pusActDataSz</code>.
     */
    virtual int GetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData,
                             unsigned short *pusActDataSz) = 0;
    /**
     * @brief Writes attribute in device
     *
     * This function must allow the user to set device's attribute.
     *
     * @param usClsId Class identifier of the parameter.
     * @param usInstId Instance identifier of the parameter.
     * @param ucAttrId Parameter identifier.
     * @param usDataSz Size of parameter's data in bytes.
     * @param pvData Pointer to a buffer where to copy parameter's data.
     * @return Function must return a value and to format an error message
     * when error detected in the process of setting device's attribute.
     * Error must be returned if <code>usDataSz</code> is less or greater
     * than actual size of attribute's data.
     */
    virtual int SetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData) = 0;
    /**
     * @brief Execute DeviceNet™ service in device.
     *
     * This function must allow the execution of DeviceNet™ service in device.
     * @param ucSrvCode Service code.
     * @param usClsId Class identifier of the service.
     * @param usInstId Instance identifier of the service.
     * @param usDataSz Size of service's data in bytes.
     * @param pvData Pointer to a buffer where to copy service's data.
     * @return Function must return a value and to format an error message
     * when error detected in the process of executing device's service.
     * Error must be returned if <code>usDataSz</code> is less or greater
     * than actual size of service's data.
     */
    virtual int ExecService(unsigned char  ucSrvCode,
                            unsigned short usClsId,
                            unsigned short usInstId,
                            unsigned short usDataSz,
                            void           *pvData) = 0;
    /**
     * @brief Execute DeviceNet™ Reset service in device
     *
     * With this function the user must be able to execute DeviceNet™ Reset
     * service in device. It could be implemented by simply invoking
     * CDevice::ExecService with appropriate parameters.
     * @return Same as CDevice::ExecService.
     */
    virtual int Reset(void) = 0;
    /* destructor */
    virtual ~CDevice();
};

/**
 * @brief Retrieves connection type
 *
 * This functions can be used get the I/O connection type established to
 * device with CDevice::Allocate function.
 * @return Connection type for device.
 */
inline unsigned char CDevice::GetConnType(void) const {
    return ucConnType;
}

/**
 * @brief Retrieves Expected Packet Rate
 *
 * This function can be used to get the EPR for the I/O connection established
 * to device.
 * @return EPR of device.
 */
inline unsigned short CDevice::GetEPR(void) const {
    return usEPR;
}

#endif /* cdevice.h */

