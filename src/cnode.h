/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cnode.h                   Type        : header            *
 *  Description : CNode class declaration.                                  *
 *  CVS         : $Id: cnode.h,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#ifndef CNODE_H
#define CNODE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cnode.h requires c++ compiler."
#endif

#include "cid.h"

/* Class  : CNode
 * Purpose: Representing a node in DeviceNet network.
 */
class DNETMOD_API CNode : public CIdentificator {
private:
    /* Copy constructor and assignment operator not supported for this class */
    CNode(const CNode&);
    CNode& operator =(const CNode&);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
protected:
    bool bActive;
    unsigned char ucMacID;
    unsigned char ucConsumedConnSize;
    unsigned char ucProducedConnSize;
public:
    /* constructors */
    CNode();
    CNode(unsigned char ucMID, unsigned char ucCCS, unsigned char ucPCS);
    /* get/set */
    bool IsActive(void) const;
    unsigned char GetMacID(void) const;
    void SetMacID(unsigned char ucMID);
    unsigned char GetConsumedConnSize(void) const;
    void SetConsumedConnSize(unsigned char ucCCS);
    unsigned char GetProducedConnSize(void) const;
    void SetProducedConnSize(unsigned char ucPCS);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(char *strCompareName) const;
    /* destructor */
    virtual ~CNode();
};

inline bool CNode::IsActive(void) const {
    return bActive;
}

inline unsigned char CNode::GetMacID(void) const {
    return ucMacID;
}

inline unsigned char CNode::GetConsumedConnSize(void) const {
    return ucConsumedConnSize;
}

inline unsigned char CNode::GetProducedConnSize(void) const {
    return ucProducedConnSize;
}

#endif /* cnode.h */
