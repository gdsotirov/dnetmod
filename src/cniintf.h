/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cniintf.h                 Type        : header            *
 *  Description : CNIInterface class declaration.                           *
 ****************************************************************************/

/**
 * @file cniintf.h
 * CNIInterface class declaration.
 */

#ifndef CNIINTF_H
#define CNIINTF_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cniintf.h requires c++ compiler."
#endif

#include "cintf.h"

/**
 * @brief Representation of National Instruments DeviceNet interface
 *
 * This class is a shell for NationalInstruments NI-DNET™ interface. It is
 * based on CInterface class and is available only on Win32 platform, because
 * the C API of the interface driver is only for this platform.
 * @remark Copy constructor and assignment operator not supported for
 * this class.
 */
class DNETMOD_API CNIInterface : public CInterface {
private:
    unsigned char ucIntfID;
    unsigned long ulNIDNET_ID;
private:
    CNIInterface(const CNIInterface&);
    CNIInterface& operator =(const CNIInterface&);
    int CloseInterface(void);
protected:
    /** Class's ID */
    static unsigned long ulClassID;
    /** Class's name */
    static char strClassName[];
public:
    /* constructors */
    CNIInterface();
    CNIInterface(unsigned char ucMID, unsigned char ucBR, unsigned char ucIID);
    /* get/set functions */
    unsigned char GetIntfID(void) const;
    void GetName(unsigned long ulStrSz, char *strName);
    void SetIntfID(unsigned char ucIID);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    virtual int Open(void);
    virtual int Close(void);
    virtual int Reset(void *);
    /* destructor */
    virtual ~CNIInterface();
};

/**
 * @brief Retrieves NI-DNET™ interface identifier
 *
 * Used to retrieve or set NI-DNET™ interface identifier.
 * @return NI-DNET™ interface identifier. A number between 0 and 31.
 */
inline unsigned char CNIInterface::GetIntfID(void) const {
    return ucIntfID;
}

#endif /* cniintf.h */

