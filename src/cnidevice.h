/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : cnidevice.h               Type        : header            *
 *  Description : CNIDevice class declaration.                              *
 ****************************************************************************/

#ifndef CNIDEVICE_H
#define CNIDEVICE_H 1

#include "dnmdefs.h"

#ifndef COMPILER_CPP
#error "error: File cnidevice.h requires c++ compiler."
#endif

#include "cdevice.h"

#define NI_CONN_IO          0x0001
#define NI_CONN_EM          0x0002

class DNETMOD_API CNIDevice : public CDevice {
private:
    unsigned long ulHIO;
    unsigned long ulHEM;
private:
    /* Copy constructor and assignment operator not supported for this class */
    CNIDevice(const CDevice&);
    CNIDevice& operator =(const CNIDevice&);
    int UnallocateDevice(void);
protected:
    static unsigned long ulClassID;
    static char strClassName[];
public:
    /* constructors */
    CNIDevice();
    CNIDevice(unsigned char  ucMID,
              unsigned char  ucCCS,
              unsigned char  ucPCS,
              unsigned char  ucCT,
              unsigned short usEPR,
              CInterface     *pIntf);
    /* overrides */
    virtual bool IsA(unsigned long ulCompareID) const;
    virtual bool IsA(const char *strCompareName) const;
    virtual int Allocate(unsigned char ucFlags = NI_CONN_IO);
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
    virtual ~CNIDevice();
};

#endif /* cnidevice.h */

