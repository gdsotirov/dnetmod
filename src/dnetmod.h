/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : dnetmod.h                 Type        : header            *
 *  Description : Main header file for the module.                          *
 ****************************************************************************/

/**
 * @file dnetmod.h
 * @brief Main header file for the module.
 */

#ifndef DNETMOD_H
#define DNETMOD_H 1

#include <stdarg.h>

#include "dnmdefs.h"
#include "cid.h"
#include "cnode.h"
#include "cintf.h"
#include "cdevice.h"

/* NI-DNET Interfaces have support only on Win32 platform */
#if defined(OS_WIN32)
#include "cniintf.h"
#include "cnidevice.h"
#endif

/* CIF Interfaces supporting both Linux and Win32 */
#if defined(OS_LINUX) || defined(OS_WIN32)
#include "ccifintf.h"
#include "ccifdevice.h"
#endif

#ifdef COMPILER_GNUC
/** Specifies structure alignment to the byte */
#define DI_PACKED __attribute__ ((aligned(1)))
#else
#define DI_PACKED
#endif

#if defined(COMPILER_MSC) || defined(COMPILER_WATCOMC) || ( defined(COMPILER_BORLANDC) && COMPILER_BORLANDC > 0x520 )
#pragma pack(1)
#endif

#if defined(COMPILER_BORLANDC) && COMPILER_BORLANDC <= 0x520
#pragma option -a1
#endif

/** @brief Device information structure */
typedef struct DeviceInfoTag {
    unsigned short usVendId        DI_PACKED; /**< (1) Vendor ID            */
    unsigned short usDevTypeId     DI_PACKED; /**< (2) Device Type          */
    unsigned short usProdCode      DI_PACKED; /**< (3) Product Code         */
    /** @brief Revision information structure */
    struct RevisionTag {
        unsigned char ucMajor      DI_PACKED; /**<     Revision Major       */
        unsigned char usMinor      DI_PACKED; /**<     Revision Minor       */
    } Rev;                                    /**< (4) Revision             */
    unsigned short usStatus        DI_PACKED; /**< (5) Device Status        */
    unsigned long ulSerialNumber   DI_PACKED; /**< (6) Device Serial Number */
    char strProdName[80]           DI_PACKED; /**< (7) Product Name         */
    /** @brief Connection info structure */
    struct _ConnInfo {
        bool bSupported            DI_PACKED; /**<     Supported flag       */
        unsigned char ucInputSize  DI_PACKED; /**<     Input size in bytes  */
        unsigned char ucOutputSize DI_PACKED; /**<     Output size in bytes */
    } EM      /**< Connection info for EM                   */,
      Poll    /**< Connection info for Polling mode         */,
      Strobe  /**< Connection info for Strobe mode          */,
      Cyclic  /**< Connection info for Cyclic mode          */,
      COS     /**< Connection info for Change of State mode */
    ;
} DeviceInfo, *pDeviceInfo;

#if defined(COMPILER_MSC) || defined(COMPILER_WATCOMC) || ( defined(COMPILER_BORLANDC) && COMPILER_BORLANDC > 0x520 )
#pragma pack()
#endif

#if defined(COMPILER_BORLANDC) && COMPILER_BORLANDC <= 0x520
#pragma option -a.
#endif

/* Helpfull functions */
void DNETMOD_CC CIFErrToString(short, unsigned long, char *);
void DNETMOD_CC CIFTskErrToString(unsigned char, unsigned long, char *);
void DNETMOD_CC DevErrToString(unsigned char, unsigned long, char *);
long DNETMOD_CC SetCIFError(short sStatus, char *strFmt, va_list vaList);
#if defined(OS_WIN32)
long DNETMOD_CC SetNIError(long lStatus, char *strFmt, va_list vaList);
#endif
long DNETMOD_CC SetError(long lErrCode...);

/* API functions */
DNETMOD_API int DNETMOD_CC
DevTypeToString(unsigned short usDvType, unsigned long ulStrSz, char *strName);

DNETMOD_API void DNETMOD_CC GetErrMsg(unsigned long ulStrSz, char *strMsg);

DNETMOD_API int DNETMOD_CC
VendIdToString(unsigned short usVendId, unsigned long ulStrSz, char *strName);

#endif /* dnetmod.h */

