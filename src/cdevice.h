/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : cdevice.h                 Type        : header            *
 *  Description : CDevice class declaration.                                *
 *  CVS         : $Id: cdevice.h,v 1.2 2008/08/17 19:15:58 gsotirov Exp $
 ****************************************************************************/

#ifndef CDEVICE_H
#define CDEVICE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cdevice.h requires c++ compiler."
#endif

#include "cnode.h"
#include "cintf.h"

/* Class   : CDevice
 * Purpose : Base and abstract class for representing a DeviceNet device.
 */
class DNETMOD_API CDevice : public CNode {
private:
    /* Copy constructor and assignment operator not supported for this class */
    CDevice(const CDevice&);
    CDevice& operator =(const CDevice&);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
protected:
    unsigned char ucConnType;
    unsigned short usEPR;
    CInterface *pInterface;
public:
    /* constructors */
    CDevice();
    CDevice(unsigned char  ucMID,
            unsigned char  ucCCS,
            unsigned char  ucPCS,
            unsigned char  ucCT,
            unsigned short usEPR_,
            CInterface     *pIntf);
    /* get/set */
    unsigned char GetConnType(void) const;
    void SetConnType(unsigned char ucCT);
    unsigned short GetEPR(void) const;
    void SetEPR(unsigned short usEPR_);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    /* main */
    virtual int Allocate(unsigned char ucFlags) = 0;
    virtual int Unallocate(void) = 0;
    virtual int ReadIOData(unsigned long ulBufSz, void *pvBuf) = 0;
    virtual int WriteIOData(unsigned long ulBufSz, void *pvBuf) = 0;
    virtual int GetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData,
                             unsigned short *pusActDataSz) = 0;
    virtual int SetAttribute(unsigned short usClsId,
                             unsigned short usInstId,
                             unsigned char  ucAttrId,
                             unsigned short usDataSz,
                             void           *pvData) = 0;
    virtual int ExecService(unsigned char  ucSrvCode,
                            unsigned short usClsId,
                            unsigned short usInstId,
                            unsigned short usDataSz,
                            void           *pvData) = 0;
    virtual int Reset(void) = 0;
    /* destructor */
    virtual ~CDevice();
};

inline unsigned char CDevice::GetConnType(void) const {
    return ucConnType;
}

inline unsigned short CDevice::GetEPR(void) const {
    return usEPR;
}

#endif /* cdevice.h */

