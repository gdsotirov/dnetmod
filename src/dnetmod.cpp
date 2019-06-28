/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Author      : George D. Sotirov, gds@sd-bul.bg                          *
 *  File        : dnetmod.cpp               Type        : source            *
 *  Description : Implementation of module functions.                       *
 *  CVS         : $Id: dnetmod.cpp,v 1.2 2010/06/13 15:54:38 gsotirov Exp $
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "dnmdefs.h"

#if defined(OS_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "dnetmod.h"
#include "dnmerrs.h"
#include "dnmsd.h"

#if defined(OS_WIN32)
#include "nidnet.h"
#endif

#if defined(OS_LINUX)
#include "cif_user.h"
#elif defined(OS_WIN32)
#include "cifuser.h"
#endif
#include "rcs_user.h"
#include "dnm_user.h"

/* Globals */
char errmsg[DNETMOD_MAX_ERRMSG_LEN];

/* Function: VendIdToString
 * Purpose : Retrive vendor identification string from vendor ID.
 */
int DNETMOD_CC VendIdToString(
    unsigned short usVendId,
    unsigned long  ulStrSz,
    char           *strName) {
    if ( usVendId <= DNETMOD_MAX_VENDID ) {
        strncpy(strName, VendStr[usVendId], ulStrSz);
        return SetError(ERR_NOERR);
    }
    else {
        strncpy(strName, "Unknown", ulStrSz);
        return SetError(ERR_VENDID, usVendId);
    }
}

/* Function: DevTypeToString
 * Purpose : Retrive device type string from device type ID.
 */
int DNETMOD_CC DevTypeToString(
    unsigned short usDvType,
    unsigned long  ulStrSz,
    char           *strName) {
    if ( usDvType <= DNETMOD_MAX_DEVTYPE ) {
        strncpy(strName, DevTypeStr[usDvType], ulStrSz);
        return SetError(ERR_NOERR);
    }
    else {
        strncpy(strName, "Unknown", ulStrSz);
        return SetError(ERR_DEVTYPE, usDvType);
    }
}

/* Function: DevErrToString
 * Purpose : Retrive device error message from error id.
 */
void DNETMOD_CC DevErrToString(
    unsigned char ucErrId,
    unsigned long ulStrSz,
    char          *pstrEStr) {
    switch ( ucErrId ) {
      case DERR_OK        : strncpy(pstrEStr, DESTR_OK, ulStrSz);
                            break;
      case DERR_RES_UNAV  : strncpy(pstrEStr, DESTR_RESUNAV, ulStrSz);
                            break;
      case DERR_SRV_UNAV  : strncpy(pstrEStr, DESTR_SRVUNAV, ulStrSz);
                            break;
      case DERR_ATT_INVLD : strncpy(pstrEStr, DESTR_ATTINVLD, ulStrSz);
                            break;
      case DERR_ALR_REQ   : strncpy(pstrEStr, DESTR_ALRREQ, ulStrSz);
                            break;
      case DERR_STAT_CONFL: strncpy(pstrEStr, DESTR_STATCONFL, ulStrSz);
                            break;
      case DERR_ATT_NOT_SET:strncpy(pstrEStr, DESTR_ATTNOTSET, ulStrSz);
                            break;
      case DERR_PERM      : strncpy(pstrEStr, DESTR_PERM, ulStrSz);
                            break;
      case DERR_SPE       : strncpy(pstrEStr, DESTR_SPE, ulStrSz);
                            break;
      case DERR_NO_RESP   : strncpy(pstrEStr, DESTR_NORESP, ulStrSz);
                            break;
      case DERR_NED       : strncpy(pstrEStr, DESTR_NED, ulStrSz);
                            break;
      case DERR_ATT_NOT_SUP:strncpy(pstrEStr, DESTR_ATTNOTSUP, ulStrSz);
                            break;
      case DERR_TMD       : strncpy(pstrEStr, DESTR_TMD, ulStrSz);
                            break;
      case DERR_OBJ_DONT_EXST:strncpy(pstrEStr, DESTR_OBJDONTEXST, ulStrSz);
                            break;
      case DERR_DLBS      : strncpy(pstrEStr, DESTR_DLBS, ulStrSz);
                            break;
      case DERR_VENDSPEC  : strncpy(pstrEStr, DESTR_VENDSPEC, ulStrSz);
                            break;
      case 0xFF           : strncpy(pstrEStr, "No", ulStrSz);
                            break;
      default             : strncpy(pstrEStr, "Unknown", ulStrSz);
    }
}

/* Function: CIFErrToString
 * Purpose : Retrive CIF error description.
 */
void DNETMOD_CC CIFErrToString(
    short         sErr,
    unsigned long ulStrSz,
    char          *strMsg) {
    switch ( sErr ) {
        case DRV_NO_ERROR :
            strncpy(strMsg, ESTR_DRV_NO_ERROR, ulStrSz);
            break;
        case DRV_BOARD_NOT_INITIALIZED :
            strncpy(strMsg, ESTR_DRV_BOARD_NOT_INITIALIZED, ulStrSz);
            break;
        case DRV_INIT_STATE_ERROR :
            strncpy(strMsg, ESTR_DRV_INIT_STATE_ERROR, ulStrSz);
            break;
        case DRV_READ_STATE_ERROR :
            strncpy(strMsg, ESTR_DRV_READ_STATE_ERROR, ulStrSz);
            break;
        case DRV_CMD_ACTIVE :
            strncpy(strMsg, ESTR_DRV_CMD_ACTIVE, ulStrSz);
            break;
        case DRV_PARAMETER_UNKNOWN :
            strncpy(strMsg, ESTR_DRV_PARAMETER_UNKNOWN, ulStrSz);
            break;
        case DRV_WRONG_DRIVER_VERSION :
            strncpy(strMsg, ESTR_DRV_WRONG_DRIVER_VERSION, ulStrSz);
            break;
        case DRV_PCI_SET_CONFIG_MODE :
            strncpy(strMsg, ESTR_DRV_PCI_SET_CONFIG_MODE, ulStrSz);
            break;
        case DRV_PCI_READ_DPM_LENGTH :
            strncpy(strMsg, ESTR_DRV_PCI_READ_DPM_LENGTH, ulStrSz);
            break;
        case DRV_PCI_SET_RUN_MODE :
            strncpy(strMsg, ESTR_DRV_PCI_SET_RUN_MODE, ulStrSz);
            break;
        case DRV_DEV_DPM_ACCESS_ERROR :
            strncpy(strMsg, ESTR_DRV_DEV_DPM_ACCESS_ERROR, ulStrSz);
            break;
        case DRV_DEV_NOT_READY :
            strncpy(strMsg, ESTR_DRV_DEV_NOT_READY, ulStrSz);
            break;
        case DRV_DEV_NOT_RUNNING :
            strncpy(strMsg, ESTR_DRV_DEV_NOT_RUNNING, ulStrSz);
            break;
        case DRV_DEV_WATCHDOG_FAILED :
            strncpy(strMsg, ESTR_DRV_DEV_WATCHDOG_FAILED, ulStrSz);
            break;
        case DRV_DEV_OS_VERSION_ERROR :
            strncpy(strMsg, ESTR_DRV_DEV_OS_VERSION_ERROR, ulStrSz);
            break;
        case DRV_DEV_SYSERR :
            strncpy(strMsg, ESTR_DRV_DEV_SYSERR, ulStrSz);
            break;
        case DRV_DEV_MAILBOX_FULL :
            strncpy(strMsg, ESTR_DRV_DEV_MAILBOX_FULL, ulStrSz);
            break;
        case DRV_DEV_PUT_TIMEOUT :
            strncpy(strMsg, ESTR_DRV_DEV_PUT_TIMEOUT, ulStrSz);
            break;
        case DRV_DEV_GET_TIMEOUT :
            strncpy(strMsg, ESTR_DRV_DEV_GET_TIMEOUT, ulStrSz);
            break;
        case DRV_DEV_GET_NO_MESSAGE :
            strncpy(strMsg, ESTR_DRV_DEV_GET_NO_MESSAGE, ulStrSz);
            break;
        case DRV_DEV_RESET_TIMEOUT :
            strncpy(strMsg, ESTR_DRV_DEV_RESET_TIMEOUT, ulStrSz);
            break;
        case DRV_DEV_NO_COM_FLAG :
            strncpy(strMsg, ESTR_DRV_DEV_NO_COM_FLAG, ulStrSz);
            break;
        case DRV_DEV_EXCHANGE_FAILED :
            strncpy(strMsg, ESTR_DRV_DEV_EXCHANGE_FAILED, ulStrSz);
            break;
        case DRV_DEV_EXCHANGE_TIMEOUT :
            strncpy(strMsg, ESTR_DRV_DEV_EXCHANGE_TIMEOUT, ulStrSz);
            break;
        case DRV_DEV_COM_MODE_UNKNOWN :
            strncpy(strMsg, ESTR_DRV_DEV_COM_MODE_UNKNOWN, ulStrSz);
            break;
        case DRV_DEV_FUNCTION_FAILED :
            strncpy(strMsg, ESTR_DRV_DEV_FUNCTION_FAILED, ulStrSz);
            break;
        case DRV_DEV_DPMSIZE_MISMATCH :
            strncpy(strMsg, ESTR_DRV_DEV_DPMSIZE_MISMATCH, ulStrSz);
            break;
        case DRV_DEV_STATE_MODE_UNKNOWN :
            strncpy(strMsg, ESTR_DRV_DEV_STATE_MODE_UNKNOWN, ulStrSz);
            break;
        case DRV_USR_OPEN_ERROR :
            strncpy(strMsg, ESTR_DRV_USR_OPEN_ERROR, ulStrSz);
            break;
        case DRV_USR_INIT_DRV_ERROR :
            strncpy(strMsg, ESTR_DRV_USR_INIT_DRV_ERROR, ulStrSz);
            break;
        case DRV_USR_NOT_INITIALIZED :
            strncpy(strMsg, ESTR_DRV_USR_NOT_INITIALIZED, ulStrSz);
            break;
        case DRV_USR_COMM_ERR :
            strncpy(strMsg, ESTR_DRV_USR_COMM_ERR, ulStrSz);
            break;
        case DRV_USR_DEV_NUMBER_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_DEV_NUMBER_INVALID, ulStrSz);
            break;
        case DRV_USR_INFO_AREA_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_INFO_AREA_INVALID, ulStrSz);
            break;
        case DRV_USR_NUMBER_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_NUMBER_INVALID, ulStrSz);
            break;
        case DRV_USR_MODE_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_MODE_INVALID, ulStrSz);
            break;
        case DRV_USR_MSG_BUF_NULL_PTR :
            strncpy(strMsg, ESTR_DRV_USR_MSG_BUF_NULL_PTR, ulStrSz);
            break;
        case DRV_USR_MSG_BUF_TOO_SHORT :
            strncpy(strMsg, ESTR_DRV_USR_MSG_BUF_TOO_SHORT, ulStrSz);
            break;
        case DRV_USR_SIZE_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_SIZE_INVALID, ulStrSz);
            break;
        case DRV_USR_SIZE_ZERO :
            strncpy(strMsg, ESTR_DRV_USR_SIZE_ZERO, ulStrSz);
            break;
        case DRV_USR_SIZE_TOO_LONG :
            strncpy(strMsg, ESTR_DRV_USR_SIZE_TOO_LONG, ulStrSz);
            break;
        case DRV_USR_DEV_PTR_NULL :
            strncpy(strMsg, ESTR_DRV_USR_DEV_PTR_NULL, ulStrSz);
            break;
        case DRV_USR_BUF_PTR_NULL :
            strncpy(strMsg, ESTR_DRV_USR_BUF_PTR_NULL, ulStrSz);
            break;
        case DRV_USR_SENDSIZE_TOO_LONG :
            strncpy(strMsg, ESTR_DRV_USR_SENDSIZE_TOO_LONG, ulStrSz);
            break;
        case DRV_USR_RECVSIZE_TOO_LONG :
            strncpy(strMsg, ESTR_DRV_USR_RECVSIZE_TOO_LONG, ulStrSz);
            break;
        case DRV_USR_SENDBUF_PTR_NULL :
            strncpy(strMsg, ESTR_DRV_USR_SENDBUF_PTR_NULL, ulStrSz);
            break;
        case DRV_USR_RECVBUF_PTR_NULL :
            strncpy(strMsg, ESTR_DRV_USR_RECVBUF_PTR_NULL, ulStrSz);
            break;
        case DRV_USR_FILE_OPEN_FAILED :
            strncpy(strMsg, ESTR_DRV_USR_FILE_OPEN_FAILED, ulStrSz);
            break;
        case DRV_USR_FILE_SIZE_ZERO :
            strncpy(strMsg, ESTR_DRV_USR_FILE_SIZE_ZERO, ulStrSz);
            break;
        case DRV_USR_FILE_NO_MEMORY :
            strncpy(strMsg, ESTR_DRV_USR_FILE_NO_MEMORY, ulStrSz);
            break;
        case DRV_USR_FILE_READ_FAILED :
            strncpy(strMsg, ESTR_DRV_USR_FILE_READ_FAILED, ulStrSz);
            break;
        case DRV_USR_INVALID_FILETYPE :
            strncpy(strMsg, ESTR_DRV_USR_INVALID_FILETYPE, ulStrSz);
            break;
        case DRV_USR_FILENAME_INVALID :
            strncpy(strMsg, ESTR_DRV_USR_FILENAME_INVALID, ulStrSz);
            break;
        case DEV_NOT_RESPONDING :
            strncpy(strMsg, ESTR_DEV_NOT_RESPONDING, ulStrSz);
            break;
        case DEV_ATTR_ACCESS_DENIED :
            strncpy(strMsg, ESTR_DEV_ATTR_ACCESS_DENIED, ulStrSz);
            break;
        case DEV_CONN_LEN_DIFF :
            strncpy(strMsg, ESTR_DEV_CONN_LEN_DIFF, ulStrSz);
            break;
        default :
            if ( sErr >= DRV_RCS_ERROR_OFFSET )
                sprintf(strMsg, ESTR_DRV_RCS_ERROR_OFFSET, sErr);
            else strncpy(strMsg, "Unknown", ulStrSz);
    }
}

/* Function: CIFTskErrToString
 * Purpose : Retrieve CIF task error description.
 */
void DNETMOD_CC CIFTskErrToString(
    unsigned char ucTskError,
    unsigned long ulStrSz,
    char          *strTskErrStr) {
    switch ( ucTskError ) {
      case TASK_F_UNKNOWN_MODE :
        strncpy(strTskErrStr, ESTR_TASK_F_UNKNOWN_MODE, ulStrSz);
        break;
      case TASK_F_BAUDRATE_OUT_RANGE :
        strncpy(strTskErrStr, ESTR_TASK_F_BAUDRATE_OUT_RANGE, ulStrSz);
        break;
      case TASK_F_OWN_MAC_ID_OUT_RANGE :
        strncpy(strTskErrStr, ESTR_TASK_F_OWN_MAC_ID_OUT_RANGE, ulStrSz);
        break;
      case TASK_F_DUPLICATE_MAC_ID :
        strncpy(strTskErrStr, ESTR_TASK_F_DUPLICATE_MAC_ID, ulStrSz);
        break;
      case TASK_F_NO_DEV_TAB :
        strncpy(strTskErrStr, ESTR_TASK_F_NO_DEV_TAB, ulStrSz);
        break;
      case TASK_F_ADR_DOUBLE :
        strncpy(strTskErrStr, ESTR_TASK_F_ADR_DOUBLE, ulStrSz);
        break;
      case TASK_F_DATA_SET_FIELD_LEN :
        strncpy(strTskErrStr, ESTR_TASK_F_DATA_SET_FIELD_LEN, ulStrSz);
        break;
      case TASK_F_PRED_MST_SL_ADD_LEN :
        strncpy(strTskErrStr, ESTR_TASK_F_PRED_MST_SL_ADD_LEN, ulStrSz);
        break;
      case TASK_F_PRED_MSTSL_CFG_FIELD_LEN :
        strncpy(strTskErrStr, ESTR_TASK_F_PRED_MSTSL_CFG_FIELD_LEN, ulStrSz);
        break;
      case TASK_F_PRED_MST_SL_ADD_TAB_INCONS :
        strncpy(strTskErrStr, ESTR_TASK_F_PRED_MST_SL_ADD_TAB_INCONS, ulStrSz);
      break;
      case TASK_F_EXPL_PRM_FIELD_LEN :
        strncpy(strTskErrStr, ESTR_TASK_F_EXPL_PRM_FIELD_LEN, ulStrSz);
        break;
      case TASK_F_PRED_MSTSL_CFG_ADD_INPUT_INCONS :
        strncpy(strTskErrStr, ESTR_TASK_F_PRED_MSTSL_CFG_ADD_INPUT_INCONS, ulStrSz);
        break;
      case TASK_F_PRED_MSTSL_CFG_ADD_OUTPUT_INCONS :
        strncpy(strTskErrStr, ESTR_TASK_F_PRED_MSTSL_CFG_ADD_OUTPUT_INCONS, ulStrSz);
        break;
      case TASK_F_UNKNOWN_DATA_TYPE :
        strncpy(strTskErrStr, ESTR_TASK_F_UNKNOWN_DATA_TYPE, ulStrSz);
        break;
      case TASK_F_MODULE_DATA_SIZE :
        strncpy(strTskErrStr, ESTR_TASK_F_MODULE_DATA_SIZE, ulStrSz);
        break;
      case TASK_F_OUTPUT_OFF_RANGE :
        strncpy(strTskErrStr, ESTR_TASK_F_OUTPUT_OFF_RANGE, ulStrSz);
        break;
      case TASK_F_INPUT_OFF_RANGE :
        strncpy(strTskErrStr, ESTR_TASK_F_INPUT_OFF_RANGE, ulStrSz);
        break;
      case TASK_F_WRONG_TYPE_OF_CONNECTION :
        strncpy(strTskErrStr, ESTR_TASK_F_WRONG_TYPE_OF_CONNECTION, ulStrSz);
        break;
      case TASK_F_TYPE_CONNECTION_REDEFINITION :
        strncpy(strTskErrStr, ESTR_TASK_F_TYPE_CONNECTION_REDEFINITION, ulStrSz);
        break;
      case TASK_F_EXP_PACKET_LESS_PROD_INHIBIT :
        strncpy(strTskErrStr, ESTR_TASK_F_EXP_PACKET_LESS_PROD_INHIBIT, ulStrSz);
        break;
      case TASK_F_PRM_FIELD_LEN_INCONSISTENT :
        strncpy(strTskErrStr, ESTR_TASK_F_PRM_FIELD_LEN_INCONSISTENT, ulStrSz);
        break;
      case TASK_F_NO_CAN :
        strncpy(strTskErrStr, ESTR_TASK_F_NO_CAN, ulStrSz);
        break;
      case TASK_F_DATA_AREA :
        strncpy(strTskErrStr, ESTR_TASK_F_DATA_AREA, ulStrSz);
        break;
      case TASK_F_DATA_ADR :
        strncpy(strTskErrStr, ESTR_TASK_F_DATA_ADR, ulStrSz);
        break;
      case TASK_F_DATA_CNT :
        strncpy(strTskErrStr, ESTR_TASK_F_DATA_CNT, ulStrSz);
        break;
      case TASK_F_DATA_TYPE :
        strncpy(strTskErrStr, ESTR_TASK_F_DATA_TYPE, ulStrSz);
        break;
      case TASK_F_FUNCTION :
        strncpy(strTskErrStr, ESTR_TASK_F_FUNCTION, ulStrSz);
        break;
      case TASK_F_DATABASE :
        strncpy(strTskErrStr, ESTR_TASK_F_DATABASE, ulStrSz);
        break;
      case TASK_F_DATABASE_READ :
        strncpy(strTskErrStr, ESTR_TASK_F_DATABASE_READ, ulStrSz);
        break;
      default : sprintf(strTskErrStr, "Task error. Code %d.", ucTskError);
    }
}

#if defined(OS_WIN32)
/* Function: SetNIError
 * Purpose : Process NI-DNET error code.
 */
long DNETMOD_CC SetNIError(long lError, char *strMsgFmt, va_list pArgList) {
    if ( lError != 0 ) {
        unsigned short usIID = 0;
        unsigned short usDevMacId = 0;
        char strMessage[80] = {0};

        usIID      = (unsigned short)va_arg(pArgList, int);
        usDevMacId = (unsinged short)va_arg(pArgList, int);
        ncStatusToString(lError, sizeof(strMessage), strMessage);
        if ( ISPTRVALID(errmsg, char) )
            sprintf(errmsg, strMsgFmt, usIID, usDevMacId, strMessage);
        return ERR_NIDNET;
    }
    if ( ISPTRVALID(errmsg, char) )
        *errmsg = 0;
    return ERR_NOERR;
}
#endif /* if defined(OS_WIN32) */

/* Function: SetCIFError
 * Purpose : Process CIF error code.
 */
long DNETMOD_CC SetCIFError(short sCIFError, char * strMsgFmt, va_list pArgList) {
    unsigned char ucTskErr = 0;
    unsigned short usBID = 0;
    unsigned short usDevMacId = 0;

    ucTskErr   = static_cast<unsigned char>(va_arg(pArgList, int));
    usBID      = static_cast<unsigned short>(va_arg(pArgList, int));
    usDevMacId = static_cast<unsigned short>(va_arg(pArgList, int));
    if ( sCIFError != DRV_NO_ERROR || ucTskErr != TASK_F_OK ) {
        char strMessage[80] = {0};
        if ( sCIFError != DRV_NO_ERROR ) {
            CIFErrToString(sCIFError, sizeof(strMessage), strMessage);
            if ( ISPTRVALID(errmsg, char) )
                sprintf(errmsg, strMsgFmt, usBID, usDevMacId, "E", sCIFError, strMessage);
        }
        else if ( ucTskErr != TASK_F_OK ) {
            CIFTskErrToString(ucTskErr, sizeof(strMessage), strMessage);
            if ( ISPTRVALID(errmsg, char) )
                sprintf(errmsg, strMsgFmt, usBID, usDevMacId, "TE", ucTskErr, strMessage);
        }
        return ERR_CIF;
    }
    if ( ISPTRVALID(errmsg, char) )
        *errmsg = 0;
    return ERR_NOERR;
}

/* Function: SetError
 * Purpose : Set errmsg global variable.
 */
long DNETMOD_CC SetError(long lErrCode...) {
    va_list pArgList;
    char strErrFmt[DNETMOD_MAX_ERRMSG_LEN] = {0};

    va_start(pArgList, lErrCode);
    switch ( lErrCode ) {
        case ERR_NOERR  :
            strncpy(strErrFmt, ESTR_NOERR, sizeof(strErrFmt));
            break;
        case ERR_UKNOW  :
            strncpy(strErrFmt, ESTR_UKNOW, sizeof(strErrFmt));
            break;
        case ERR_EXT    :
            strncpy(strErrFmt, ESTR_EXT, sizeof(strErrFmt));
            break;
#if defined(OS_WIN32)
        case ERR_NIDNET : {
            long lNIError = va_arg(pArgList, long);
            strncpy(strErrFmt, ESTR_NIDNET, sizeof(strErrFmt));
            if ( SetNIError(lNIError, strErrFmt, pArgList) == 0 )
                lErrCode = 0;
        }
        break;
#endif /* if defined(OS_WIN32) */
        case ERR_CIF    : {
            short lCIFError = static_cast<short>(va_arg(pArgList, int));
            strncpy(strErrFmt, ESTR_CIF, sizeof(strErrFmt));
            if ( SetCIFError(lCIFError, strErrFmt, pArgList) == 0 )
                lErrCode = 0;
        }
        break;
        case ERR_EXPLCT : {
            char strGen[DNETMOD_MAX_DERRMSG_LEN] = {0};
            char strAdd[DNETMOD_MAX_DERRMSG_LEN] = {0};
            unsigned char ucMacID = static_cast<unsigned char>(va_arg(pArgList, int));
            unsigned char ucGen   = static_cast<unsigned char>(va_arg(pArgList, int));
            unsigned char ucAdd   = static_cast<unsigned char>(va_arg(pArgList, int));

            DevErrToString(ucGen, sizeof(strGen), strGen);
            DevErrToString(ucAdd, sizeof(strAdd), strAdd);
            strncpy(strErrFmt, ESTR_EXPLCT, sizeof(strErrFmt));
            if ( ISPTRVALID(errmsg, char) ) sprintf(errmsg, strErrFmt,
                ucMacID, ucGen, strGen, ucAdd, strAdd);
        }
        break;
        case ERR_CNTALM :
            strncpy(strErrFmt, ESTR_CNTALM, sizeof(strErrFmt));
            break;
        case ERR_INVFPTR:
            strncpy(strErrFmt, ESTR_INVFPTR, sizeof(strErrFmt));
            break;
        case ERR_INVFPRM:
            strncpy(strErrFmt, ESTR_INVFPRM, sizeof(strErrFmt));
            break;
        case ERR_IOPER  :
            strncpy(strErrFmt, ESTR_IOPER, sizeof(strErrFmt));
            break;
        case ERR_INOPER :
            strncpy(strErrFmt, ESTR_INOPER, sizeof(strErrFmt));
            break;
        case ERR_INVPTR :
            strncpy(strErrFmt, ESTR_INVPTR, sizeof(strErrFmt));
            break;
        case ERR_INVITF :
            strncpy(strErrFmt, ESTR_INVITF, sizeof(strErrFmt));
            break;
        case ERR_NOALOC :
            strncpy(strErrFmt, ESTR_NOALOC, sizeof(strErrFmt));
            break;
        case ERR_NOALOCIO:
            strncpy(strErrFmt, ESTR_NOALOCIO, sizeof(strErrFmt));
            break;
        case ERR_NOALOCEM:
            strncpy(strErrFmt, ESTR_NOALOCEM, sizeof(strErrFmt));
            break;
        case ERR_VENDID :
            strncpy(strErrFmt, ESTR_VENDID, sizeof(strErrFmt));
            break;
        case ERR_DEVTYPE:
            strncpy(strErrFmt, ESTR_DEVTYPE, sizeof(strErrFmt));
            break;
    }
    if ( lErrCode != ERR_NOERR  && lErrCode != ERR_NIDNET && lErrCode != ERR_CIF && lErrCode != ERR_EXPLCT )
        if ( ISPTRVALID(errmsg, char) )
            vsprintf(errmsg, strErrFmt, pArgList);
    va_end(pArgList);

    return lErrCode;
}

/* Function: GetErrMsg
 * Purpose : Retrieve error message string.
 */
void DNETMOD_CC GetErrMsg(unsigned long ulStrSz, char *strMsg) {
    strncpy(strMsg, errmsg, ulStrSz);
}

