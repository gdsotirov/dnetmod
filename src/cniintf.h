/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cniintf.h                 Type        : header            *
 *  Description : CNIInterface class declaration.                           *
 *  CVS         : $Id: cniintf.h,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#ifndef CNIINTF_H
#define CNIINTF_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cniintf.h requires c++ compiler."
#endif

#include "cintf.h"

/* Class   : CNIInterface
 * Purpose : Representation of National Instruments DeviceNet interface.
 */
class DNETMOD_API CNIInterface : public CInterface {
private:
    unsigned char ucIntfID;
    unsigned long ulNIDNET_ID;
private:
    /* Copy constructor and assignment operator not supported for this class */
    CNIInterface(const CNIInterface&);
    CNIInterface& operator =(const CNIInterface&);
protected:
    static unsigned long ulClassID;
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
    virtual bool IsA(char *strCompareName) const;
    virtual int Open(void);
    virtual int Close(void);
    virtual int Reset(void *);
    /* destructor */
    virtual ~CNIInterface();
};

inline unsigned char CNIInterface::GetIntfID(void) const {
    return ucIntfID;
}

#endif /* cniintf.h */

