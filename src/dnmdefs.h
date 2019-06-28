/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : dnmdefs.h                 Type        : header            *
 *  Description : Definitions for the module.                               *
 ****************************************************************************/

#ifndef DNETMOD_DEFINITIONS_HEADER
#define DNETMOD_DEFINITIONS_HEADER 1

/* Compiler type and features */
#if defined(__cplusplus) || defined(_cplusplus) || defined(c_plusplus)
#define COMPILER_CPP        1
#else
#define COMPILER_C          1
#endif

#ifdef __STDC__
#define COMPILER_STDC       __STDC__
#endif

#ifdef _MSC_VER
#define COMPILER_MSC        _MSC_VER
#endif

#ifdef __GNUC__
/* e.g. 30201 for gcc 3.2.1 or 20700 for gcc 2.7 */
#ifdef __GNUC_PATCHLEVEL__
#define COMPILER_GNUC ( __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ )
#define COMPILER_GNUC_VER(mj, mi, pl) ( mj * 10000 + mi * 100 + pl )
#else
#define COMPILER_GNUC ( __GNUC__ * 10000 + __GNUC_MINOR___ * 100 )
#define COMPILER_GNUC_VER(mj, mi, pl) ( mj * 10000 + mi * 100 )
#endif
#endif

#ifdef __GNUG__
#define COMPILER_GNUG       __GNUG__
#endif

#ifdef __WATCOMC__
#define COMPILER_WATCOMC    __WATCOMC__
#endif

#ifdef __BORLANDC__
#define COMPILER_BORLANDC   __BORLANDC__
#endif

#ifdef __INTEL_COMPILER
#define COMPILER_INTELC     __INTEL_COMPILER
#endif

/* Determine OS */
#if defined(_WIN32) || defined(__WIN32__)
#define OS_WIN32            1
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#define OS_LINUX            1
#endif

#if defined(unix) || defined(__unix) || defined(__unix__)
#define OS_UNIX             1
#endif

/* DeviceNet constants */
#define DEVICENET_MAX_DEVICES   64

#define DEVICENET_BAUD_125K     1
#define DEVICENET_BAUD_250K     2
#define DEVICENET_BAUD_500K     3

#define DEVICENET_CONN_POLLED   0x0001 /*  1dec, 00000001bin */
#define DEVICENET_CONN_STRBED   0x0002 /*  2dec, 00000010bin */
#define DEVICENET_CONN_COS      0x0004 /*  4dec, 00000100bin */
#define DEVICENET_CONN_CYCLIC   0x0008 /*  8dec, 00001000bin */

/* Module specific definitions */
#ifdef OS_WIN32
#ifdef DNETMOD_EXPORTS
#define DNETMOD_API __declspec( dllexport )
#else
#define DNETMOD_API __declspec( dllimport )
#endif
#else
#define DNETMOD_API
#endif

#ifdef OS_WIN32
#define DNETMOD_CC __cdecl
#else
#define DNETMOD_CC
#endif

/* Limits */
#define DNETMOD_MAX_VENDID          829
#define DNETMOD_MAX_VENDSTR_LEN     54
#define DNETMOD_MAX_DEVTYPE         28
#define DNETMOD_MAX_DTSTR_LEN       25
#define DNETMOD_MAX_ERRMSG_LEN      200
#define DNETMOD_MAX_DERRMSG_LEN     32

/* Macro   : NULLPTR
 * Purpose : Retrive NULL pinter from any type.
 */
#define NULLPTR(type) ( static_cast<type *>(0) )

/* Macro   : ZERO
 * Purpose : Retrive zero value for any type.
 */
#define ZERO(type) ( static_cast<type>(0) )

/* Macro   : ISPTRVALID
 * Purpose : Check for valid pointer.
 */
#define ISPTRVALID(ptr, type) (    \
    ((ptr != NULLPTR(type))     && \
    (ptr != reinterpret_cast<type *>(0xcdcdcdcd)) && \
    (ptr != reinterpret_cast<type *>(0xcccccccc)))   )

/* Externals */
extern long DNETMOD_CC SetError(long...);

#endif /* dnmdefs.h */

