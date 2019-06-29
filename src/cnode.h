/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cnode.h                   Type        : header            *
 *  Description : CNode class declaration.                                  *
 ****************************************************************************/

/**
 * @file cnode.h
 * @brief CNode class declaration.
 */

#ifndef CNODE_H
#define CNODE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cnode.h requires c++ compiler."
#endif

#include "cid.h"

/**
 * @brief Represents a node in DeviceNet™ network
 *
 * This class is provided for generalization of nodes in a DeviceNet™ network.
 * It is a base class for CInterface and CDevice classes representing
 * respectively interface (master device) and slave device in the network.
 * Each of these has a MAC ID, a consumed and produced connection size on
 * the network and a state which to tell you if the node is active on
 * the network or not.
 * @remarks Copy constructor and assignment operator not supported for this class.
 */
class DNETMOD_API CNode : public CIdentificator {
private:
    CNode(const CNode&);
    CNode& operator =(const CNode&);
protected:
    /** Class's ID */
    static unsigned long ulClassID;
    /** Class's name */
    static char strClassName[];
protected:
    /** Flag showing whether node is active or not. */
    bool bActive;
    /** MAC identifier of the node */
    unsigned char ucMacID;
    /** Consumed connection size */
    unsigned char ucConsumedConnSize;
    /** Produced connection size */
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
    virtual bool IsA(const char *strCompareName) const;
    /* destructor */
    virtual ~CNode();
};

/**
 * @brief Checks node activity on the network
 *
 * Use this function to find if node is active on the network or not.
 * @return True if active, false if not.
 */
inline bool CNode::IsActive(void) const {
    return bActive;
}

/**
 * @brief Retrieves MAC ID of the node
 *
 * Use this functions to retrieve MAC ID of the node.
 * @return MAC ID of the node.
 */
inline unsigned char CNode::GetMacID(void) const {
    return ucMacID;
}

/**
 * @brief Retrieves count of consumed bytes by the node from the network
 *
 * Use this functions to get the count of bytes consumed by the node from
 * the network.
 * @return Consumed data bytes from the network.
 */
inline unsigned char CNode::GetConsumedConnSize(void) const {
    return ucConsumedConnSize;
}

/**
 * @brief Retrieves count of node produced bytes to the network
 *
 * Use this functions to get the count of produced bytes by the node to
 * the network.
 * @return Node produced data bytes to the network.
 */
inline unsigned char CNode::GetProducedConnSize(void) const {
    return ucProducedConnSize;
}

#endif /* cnode.h */

