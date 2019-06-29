/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : ccifintf.h                Type        : header            *
 *  Description : CCIFInterface class declaration.                          *
 ****************************************************************************/

/**
 * @file ccifintf.h
 * @brief CCIFInterface class declaration.
 */

#ifndef CCIFINTF_H
#define CCIFINTF_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File ccifintf.h requires c++ compiler."
#endif

#include "cintf.h"

class CCIFDevice;

/**
 * @brief Represents a Hilscher CIF board.
 *
 * This class is shell for the Hilscher CIF-*DNM interface boards. It is based
 * on CInterface class and uses CIF device drivers for Win32 and Linux.
 * @remark Copy constructor and assignment operator not supported for
 * this class.
 */
class DNETMOD_API CCIFInterface : public CInterface {
private:
    /** Board number */
    unsigned short usBoardNum;
    /* Bus parameters */
    /** Bus auto clear flag */
    bool bAutoClear;
    /** Bus input offset in bytes*/
    unsigned short usInputOffset;
    /** Bus output offset in bytes */
    unsigned short usOutputOffset;
private:
    CCIFInterface(const CCIFInterface&);
    CCIFInterface& operator =(const CCIFInterface&);
    int SetProtocolParameters(void);
    int ClearDEVDB(void);
    int DownloadParameters(void);
    int CloseInterface(void);
protected:
    /** Class's ID */
    static unsigned long ulClassID;
    /** Clsss's name */
    static char strClassName[];
    friend class CCIFDevice;
public:
    /* constructors */
    CCIFInterface();
    CCIFInterface(unsigned char  ucMID,
                  unsigned char  ucCCS,
                  unsigned char  ucPCS,
                  unsigned char  ucBR,
                  unsigned short usBrdNum);
    /* get/set */
    unsigned short GetBoardNum(void) const;
    void SetBoardNum(unsigned short usBrdNum);
    bool GetAutoClear(void) const;
    void SetAutoClear(bool bAutoClr);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    virtual int Open(void);
    virtual int Close(void);
    virtual int Reset(void *);
    /* destructor */
    virtual ~CCIFInterface();
};

/**
 * @brief Retrieves Hilscher board number.
 *
 * Used to retrieve currently used board number.
 * @return Board number. A value between 0 and 3.
 */
inline unsigned short CCIFInterface::GetBoardNum(void) const {
    return usBoardNum;
}

/**
 * @brief Retrieves AutoClear flag
 *
 * Used to retrieve the value of AutoClear flag.
 * @return True or false.
 */
inline bool CCIFInterface::GetAutoClear(void) const {
    return bAutoClear;
}

#endif /* ccifintf.h */

