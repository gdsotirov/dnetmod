/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cid.h                     Type        : header            *
 *  Description : CIdentificator class declaration.                         *
 ****************************************************************************/

#ifndef CIDENTIFICATOR_H
#define CIDENTIFICATOR_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cid.h requires c++ compiler!"
#endif

/* Class  : CIdentificator
 * Purpose: Type identification.
 */
class DNETMOD_API CIdentificator {
private:
    /* Copy constructor and assignment operator not supported for this class */
    CIdentificator(const CIdentificator &);
    CIdentificator & operator = (CIdentificator &);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
public:
    /* constructors */
    CIdentificator();
    /* get/set */
    unsigned long GetClassID(void) const;
    void GetClassName(unsigned long ulStrSz, char *strName) const;
    /* identification */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    /* destructor */
    virtual ~CIdentificator();
};

inline unsigned long CIdentificator::GetClassID(void) const {
    return ulClassID;
}

#endif /* cid.h */

