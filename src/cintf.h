/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cintf.h                   Type        : header            *
 *  Description : CInterface class declaration.                             *
 ****************************************************************************/

#ifndef CINTERFACE_H
#define CINTERFACE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cintf.h reguires c++ compiler."
#endif

#include "cnode.h"

/* Class   : CInterface
 * Purpose : Base and abstract class for all DeviceNet interfaces.
 */
class DNETMOD_API CInterface : public CNode {
private:
    /* Copy constructor and assignment operator not supported for this class */
    CInterface(const CInterface&);
    CInterface& operator =(const CInterface&);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
protected:
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
    virtual int Open(void) = 0;
    virtual int Close(void) = 0;
    virtual int Reset(void * = 0) = 0;
    /* destructor */
    virtual ~CInterface();
};

inline unsigned char CInterface::GetBaudRate(void) const {
    return ucBaudRate;
}

#endif /* cintf.h */

