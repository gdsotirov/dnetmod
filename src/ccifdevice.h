/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : ccifdevice.h              Type        : header            *
 *  Description : CCIFDevice class declaration.                             *
 *  CVS         : $Id: ccifdevice.h,v 1.1 2002/12/15 09:31:39 gsotirov Exp $
 ****************************************************************************/

#ifndef CCIFDEVICE_H
#define CCIFDEVICE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File ccifdevice.h requires c++ compiler!"
#endif

#include "cdevice.h"

class DNETMOD_API CCIFDevice : public CDevice {
private:
    unsigned short usInputOffset;
    unsigned short usOutputOffset;
private:
    /* Copy constructor and assignment operator not supported for this class */
    CCIFDevice(const CCIFDevice&);
    CCIFDevice& operator =(const CCIFDevice&);
private:
    int ExchangeIOData(bool, unsigned long, void *);
    int Diagnostics(void);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
public:
    /* constructors */
    CCIFDevice();
    CCIFDevice(unsigned char  ucMID,
               unsigned char  ucCCS,
               unsigned char  ucPCS,
               unsigned char  ucCT,
               unsigned short usEPR,
               CInterface     *pIntf);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(char *strCompareName) const;
    virtual int Allocate(unsigned char ucFlags = 0);
    virtual int Unallocate(void);
    virtual int ReadIOData(unsigned long ulBufSz, void *pvBuf);
    virtual int WriteIOData(unsigned long ulBufSz, void *pvBuf);
    virtual int GetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData,
                             unsigned short *pusActDataSz);
    virtual int SetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData);
    virtual int ExecService(unsigned char  ucSrvCode,
                            unsigned short usClsId,
                            unsigned short usInstId,
                            unsigned short usDataSz,
                            void           *pvData);
    virtual int Reset(void);
    /* destructor */
    virtual ~CCIFDevice();
};

#endif /* ccifdevice.h */

