/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cid.h                     Type        : header            *
 *  Description : CIdentificator class declaration.                         *
 ****************************************************************************/

/**
 * @file cid.h
 * @brief CIdentificator class declaration.
 */

#ifndef CIDENTIFICATOR_H
#define CIDENTIFICATOR_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cid.h requires c++ compiler!"
#endif

/**
 * @brief Base class for identification
 *
 * This class is used as a base for all other classes in the project. It
 * provides general identification mechanism for his descendants. A
 * descendant of CIdentificator can identify itself with a single unsigned
 * decimal number or with a name.
 *
 * @remark This class doesn't support copy constructor and assignment
 * operator. They are declared in the private section but are not defined.
 * Check class members to see more information about inheriting this class.
 */
class DNETMOD_API CIdentificator {
private:
    CIdentificator(const CIdentificator &);
    CIdentificator & operator = (CIdentificator &);
protected:
    /** Class's identifier */
    static unsigned long ulClassID;
    /** CLass's name */
    static char strClassName[];
public:
    CIdentificator();
    unsigned long GetClassID(void) const;
    void GetClassName(unsigned long ulStrSz, char *strName) const;

    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;

    virtual ~CIdentificator();
};

/**
 * @brief Retrieves class identification number
 *
 * Retrieves class identification number (ClassID).
 * @remarks For the identification the static member #ulClassID is used.
 * Thus member's value is the same for all objects of type CIdentificator and
 * it descendants. If you want to identify a descendant of CIdentificator
 * differently you must redeclare this member and initialize it like you want.
 * @return Class identification number
 */
inline unsigned long CIdentificator::GetClassID(void) const {
    return ulClassID;
}

#endif /* cid.h */

