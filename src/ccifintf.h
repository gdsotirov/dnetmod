/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : ccifintf.h                Type        : header            *
 *  Description : CCIFInterface class declaration.                          *
 *  CVS         : $Id: ccifintf.h,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#ifndef CCIFINTF_H
#define CCIFINTF_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File ccifintf.h requires c++ compiler."
#endif

#include "cintf.h"

class CCIFDevice;

/* Class   : CCIFInterface
 * Purpose : Representation of a Hilscher CIF board.
 */
class DNETMOD_API CCIFInterface : public CInterface {
private:
    unsigned short usBoardNum;
    /* Bus parameters */
    bool bAutoClear;
    unsigned short usInputOffset;
    unsigned short usOutputOffset;
private:
    /* Copy constructor and assignment operator not supported for this class */
    CCIFInterface(const CCIFInterface&);
    CCIFInterface& operator =(const CCIFInterface&);
    int SetProtocolParameters(void);
    int ClearDEVDB(void);
    int DownloadParameters(void);
protected:
    static unsigned long ulClassID;
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
    virtual bool IsA(char *strCompareName) const;
    virtual int Open(void);
    virtual int Close(void);
    virtual int Reset(void *);
    /* destructor */
    virtual ~CCIFInterface();
};

inline unsigned short CCIFInterface::GetBoardNum(void) const {
    return usBoardNum;
}

inline bool CCIFInterface::GetAutoClear(void) const {
    return bAutoClear;
}

#endif /* ccifintf.h */

