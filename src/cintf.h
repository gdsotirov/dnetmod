/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cintf.h                   Type        : header            *
 *  Description : CInterface class declaration.                             *
 ****************************************************************************/

/**
 * @file cintf.h
 * @brief CInterface class declaration.
 */

#ifndef CINTERFACE_H
#define CINTERFACE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cintf.h reguires c++ compiler."
#endif

#include "cnode.h"

/**
 * @brief Base and abstract class for all DeviceNet™ interfaces
 *
 * The interface class abstractly represents a DeviceNet interface and/or a
 * master device on the network.
 * @remark Copy constructor and assignment operator not supported for this class.
 */
class DNETMOD_API CInterface : public CNode {
private:
    CInterface(const CInterface&);
    CInterface& operator =(const CInterface&);
protected:
    /** Class's ID */
    static unsigned long ulClassID;
    /** Class's name */
    static char strClassName[];
protected:
    /** Interface's baud rate */
    unsigned char ucBaudRate;
public:
    /* constructors */
    CInterface();
    CInterface(unsigned char ucMID, unsigned char ucCCS, unsigned char ucPCS, unsigned char ucBR);
    /* get/set */
    unsigned char GetBaudRate(void) const;
    void SetBaudRate(unsigned char ucBR);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    /* main */
    /**
     * @brief Configure interface and start communication
     *
     * In the descendants of CInterface this function must initialize
     * the interface and to start communication on it. After execution of this
     * function the interface must be prepared and ready for work.
     * @remarks For the moment this function doesn't support any formal
     * parameters. The properties of CNode and CInterface must be sufficient
     * for this operation.
     * @return Function must return an error code and to format an error
     * message which the user can print out to show description for the error
     * occurred. Error codes may vary from function to function depending on
     * the interface used.
     */
    virtual int Open(void) = 0;
    /**
     * @brief Stop communication on interface
     *
     * In the descendants of CInterface this function must stop communication
     * on the interface and to finalize the work with it.
     * @remarks For the moment this function doesn't support any formal
     * parameters. The properties of CNode and CInterface must be sufficient
     * for this operation.
     * @return Generally this function is not meant to return an error code.
     * Nevertheless it's good to have information on errors occurred in
     * the finalization process for the interface. Error codes may vary from
     * function to function depending on the interface used.
     */
    virtual int Close(void) = 0;
    /**
     * @brief Reset interface
     *
     * This function is provided to allow reset of the interface. There are
     * different types of resets and interfaces. Some of them will allow only
     * software reset, others may provide hardware reset, others may provide
     * several types of software and hardware resets. To specify which kind of
     * reset must be done the function has one parameter of type void pointer,
     * which can be used as the descendant needs.
     * @param vpParam Any parameter meaningful for reset of the interface.
     * @return Function must return an error code and to format an error
     * message thus indicating unsuccessful reset of the interface. Return
     * values may vary from function to function depending of the interface
     * used.
     */
    virtual int Reset(void * vpParam = 0) = 0;
    /* destructor */
    virtual ~CInterface();
};

/**
 * @brief Retrieves baud rate for the interface
 *
 * Use this function to get baud rate of the interface.
 * @return Baud rate of the interface.
 */
inline unsigned char CInterface::GetBaudRate(void) const {
    return ucBaudRate;
}

#endif /* cintf.h */

