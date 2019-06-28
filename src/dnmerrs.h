/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : dnmerrs.h                 Type        : header            *
 *  Description : Error codes and error message formats for the module.     *
 ****************************************************************************/

#ifndef DNETMOD_ERRORS_HEADER
#define DNETMOD_ERRORS_HEADER 1

/*** E R R O R   C O D E S ***/
#define ERR_NOERR           0
#define ERR_UKNOW           1
#define ERR_EXT             2
#define ERR_NIDNET          4
#define ERR_CIF             5
#define ERR_EXPLCT          6

#define ERR_CNTALM          10

#define ERR_INVFPTR         20
#define ERR_INVFPRM         21

#define ERR_IOPER           101
#define ERR_INOPER          102
#define ERR_INVPTR          103
#define ERR_INVITF          104
#define ERR_NOALOC          105
#define ERR_NOALOCIO        106
#define ERR_NOALOCEM        107
#define ERR_VENDID          108
#define ERR_DEVTYPE         109

/* Device specific error codes */
#define  DERR_OK            0x00
#define  DERR_RES_UNAV      0x02
#define  DERR_SRV_UNAV      0x08
#define  DERR_ATT_INVLD     0x09
#define  DERR_ALR_REQ       0x0B
#define  DERR_STAT_CONFL    0x0C
#define  DERR_ATT_NOT_SET   0x0E
#define  DERR_PERM          0x0F
#define  DERR_SPE           0x10
#define  DERR_NO_RESP       0x11
#define  DERR_NED           0x13
#define  DERR_ATT_NOT_SUP   0x14
#define  DERR_TMD           0x15
#define  DERR_OBJ_DONT_EXST 0x16
#define  DERR_DLBS          0x17
#define  DERR_VENDSPEC      0x1F
#define  DERR_ERRRESP       0x23

#define DEV_NOT_RESPONDING      -501
#define DEV_ATTR_ACCESS_DENIED  -502
#define DEV_CONN_LEN_DIFF       -503


/*** E R R O R   M E S S A G E S ***/
#define ESTR_NOERR          ""
#define ESTR_UKNOW          "Unknown error."
#define ESTR_EXT            "EXT: %s"
#define ESTR_NIDNET         "NI-DNET (Intf:dnet%hu Dev:%hu): %s"
#define ESTR_CIF            "CIF (Board:%hu Dev:%hu): %s%d: %s"
#define ESTR_EXPLCT         "EM (Dev:%hu): 0x%hX - %s, 0x%hX - %s"

#define ESTR_CNTALM         "Can't allocate memory."

#define ESTR_INVFPTR        "Invalid function pointer (NULL)."
#define ESTR_INVFPRM        "Invalid value for parameter '%s'=%s of function %s."

#define ESTR_IOPER          "%s: Interface must not be operational."
#define ESTR_INOPER         "%s: Interface must be operational."
#define ESTR_INVPTR         "Dev:%hu : Invalid pointer %s = 0x%08X."
#define ESTR_INVITF         "Dev:%hu : Invalid interface. Must be of type %s."
#define ESTR_NOALOC         "Dev:%hu : Device not allocated."
#define ESTR_NOALOCIO       "Dev:%hu : Device not using I/O connection."
#define ESTR_NOALOCEM       "Dev:%hu : Device not using EM connection."
#define ESTR_VENDID         "%d - No such vendor ID!"
#define ESTR_DEVTYPE        "%d - No such device type ID!"

/* DeviceNet device errors */
#define DESTR_OK            "OK"
#define DESTR_RESUNAV       "Resource unavailable"
#define DESTR_SRVUNAV       "Service unavailable"
#define DESTR_ATTINVLD      "Invalid attribute value"
#define DESTR_ALRREQ        "Already in request mode"
#define DESTR_STATCONFL     "Object state conflict"
#define DESTR_ATTNOTSET     "Attribute not settable"
#define DESTR_PERM          "Permision check failed"
#define DESTR_SPE           "State prohibits the execution"
#define DESTR_NORESP        "No response from device"
#define DESTR_NED           "Not enough data received"
#define DESTR_ATTNOTSUP     "Attribute not supported"
#define DESTR_TMD           "To much data received"
#define DESTR_OBJDONTEXST   "Object does't exists"
#define DESTR_DLBS          "Data large buffer small"
#define DESTR_VENDSPEC      "Vendor specific. See additional"

/* CIF error messages */
#define ESTR_DRV_NO_ERROR                            ""
#define ESTR_DRV_BOARD_NOT_INITIALIZED               "(DRIVER) Board not initialized"
#define ESTR_DRV_INIT_STATE_ERROR                    "(DRIVER) Error in internal init state"
#define ESTR_DRV_READ_STATE_ERROR                    "(DRIVER) Error in internal read state"
#define ESTR_DRV_CMD_ACTIVE                          "(DRIVER) Command on this channel is active"
#define ESTR_DRV_PARAMETER_UNKNOWN                   "(DRIVER) Unknown parameter in function occured"
#define ESTR_DRV_WRONG_DRIVER_VERSION                "(DRIVER) Version is incompatible with DLL"
#define ESTR_DRV_PCI_SET_CONFIG_MODE                 "(DRIVER) Error during PCI set run mode"
#define ESTR_DRV_PCI_READ_DPM_LENGTH                 "(DRIVER) Could not read PCI dual port memory length"
#define ESTR_DRV_PCI_SET_RUN_MODE                    "(DRIVER) Error during PCI set run mode"

#define ESTR_DRV_DEV_DPM_ACCESS_ERROR                "(DEVICE) Dual port ram not accessable(board not found)"
#define ESTR_DRV_DEV_NOT_READY                       "(DEVICE) Not ready (ready flag failed)"
#define ESTR_DRV_DEV_NOT_RUNNING                     "(DEVICE) Not running (running flag failed)"
#define ESTR_DRV_DEV_WATCHDOG_FAILED                 "(DEVICE) Watchdog test failed"
#define ESTR_DRV_DEV_OS_VERSION_ERROR                "(DEVICE) Signals wrong OS version"
#define ESTR_DRV_DEV_SYSERR                          "(DEVICE) Error in dual port flags"
#define ESTR_DRV_DEV_MAILBOX_FULL                    "(DEVICE) Send mailbox is full"
#define ESTR_DRV_DEV_PUT_TIMEOUT                     "(DEVICE) PutMessage timeout"
#define ESTR_DRV_DEV_GET_TIMEOUT                     "(DEVICE) GetMessage timeout"
#define ESTR_DRV_DEV_GET_NO_MESSAGE                  "(DEVICE) No message available"
#define ESTR_DRV_DEV_RESET_TIMEOUT                   "(DEVICE) RESET command timeout"
#define ESTR_DRV_DEV_NO_COM_FLAG                     "(DEVICE) COM-flag not set"
#define ESTR_DRV_DEV_EXCHANGE_FAILED                 "(DEVICE) IO data exchange failed"
#define ESTR_DRV_DEV_EXCHANGE_TIMEOUT                "(DEVICE) IO data exchange timeout"
#define ESTR_DRV_DEV_COM_MODE_UNKNOWN                "(DEVICE) IO data mode unknown"
#define ESTR_DRV_DEV_FUNCTION_FAILED                 "(DEVICE) Function call failed"
#define ESTR_DRV_DEV_DPMSIZE_MISMATCH                "(DEVICE) DPM size differs from configuration"
#define ESTR_DRV_DEV_STATE_MODE_UNKNOWN              "(DEVICE) State mode unknown"

#define ESTR_DRV_USR_OPEN_ERROR                      "(USER) Driver not opened (device driver not loaded)"
#define ESTR_DRV_USR_INIT_DRV_ERROR                  "(USER) Can't connect with device"
#define ESTR_DRV_USR_NOT_INITIALIZED                 "(USER) Board not initialized (DevInitBoard not called)"
#define ESTR_DRV_USR_COMM_ERR                        "(USER) IOCTRL function failed"
#define ESTR_DRV_USR_DEV_NUMBER_INVALID              "(USER) Parameter DeviceNumber  invalid"
#define ESTR_DRV_USR_INFO_AREA_INVALID               "(USER) Parameter InfoArea unknown"
#define ESTR_DRV_USR_NUMBER_INVALID                  "(USER) Parameter Number invalid"
#define ESTR_DRV_USR_MODE_INVALID                    "(USER) Parameter Mode invalid"
#define ESTR_DRV_USR_MSG_BUF_NULL_PTR                "(USER) NULL pointer assignment"
#define ESTR_DRV_USR_MSG_BUF_TOO_SHORT               "(USER) Message buffer too short"
#define ESTR_DRV_USR_SIZE_INVALID                    "(USER) Parameter Size invalid"
#define ESTR_DRV_USR_SIZE_ZERO                       "(USER) Parameter Size with zero length"
#define ESTR_DRV_USR_SIZE_TOO_LONG                   "(USER) Parameter Size too long"
#define ESTR_DRV_USR_DEV_PTR_NULL                    "(USER) Device address null pointer"
#define ESTR_DRV_USR_BUF_PTR_NULL                    "(USER) Pointer to buffer is a null pointer"
#define ESTR_DRV_USR_SENDSIZE_TOO_LONG               "(USER) Parameter SendSize too long"
#define ESTR_DRV_USR_RECVSIZE_TOO_LONG               "(USER) Parameter ReceiveSize too long"
#define ESTR_DRV_USR_SENDBUF_PTR_NULL                "(USER) Pointer to send buffer is a null pointer"
#define ESTR_DRV_USR_RECVBUF_PTR_NULL                "(USER) Pointer to receive buffer is a null pointer"
#define ESTR_DRV_USR_FILE_OPEN_FAILED                "(USER) file not opened"
#define ESTR_DRV_USR_FILE_SIZE_ZERO                  "(USER) file size zero"
#define ESTR_DRV_USR_FILE_NO_MEMORY                  "(USER) not enough memory to load file"
#define ESTR_DRV_USR_FILE_READ_FAILED                "(USER) file read failed"
#define ESTR_DRV_USR_INVALID_FILETYPE                "(USER) file type invalid"
#define ESTR_DRV_USR_FILENAME_INVALID                "(USER) file name not valid"
#define ESTR_DRV_RCS_ERROR_OFFSET                    "(RCS) RCS error %hi"

#define ESTR_DEV_NOT_RESPONDING                      "Device not responding"
#define ESTR_DEV_ATTR_ACCESS_DENIED                   "Device has denied write access to configured attribute"
#define ESTR_DEV_CONN_LEN_DIFF                       "Configured and actual connection lenghts differs"

#define ESTR_TASK_F_UNKNOWN_MODE                     "Unknown handshake mode configured"
#define ESTR_TASK_F_BAUDRATE_OUT_RANGE               "Configured baudrate not supported"
#define ESTR_TASK_F_OWN_MAC_ID_OUT_RANGE             "DEVICE MAC-ID out of range"
#define ESTR_TASK_F_DUPLICATE_MAC_ID                 "Duplicate MAC-ID detected"
#define ESTR_TASK_F_NO_DEV_TAB                       "Data base in the DEVICE has no entries included"
#define ESTR_TASK_F_ADR_DOUBLE                       "Double MAC-ID configured internally"
#define ESTR_TASK_F_DATA_SET_FIELD_LEN               "Size of one device data set invalid"
#define ESTR_TASK_F_PRED_MST_SL_ADD_LEN              "Offset table for predef.mst slave conn. invalid"
#define ESTR_TASK_F_PRED_MSTSL_CFG_FIELD_LEN         "Configured table length for predefined mster/slave conn invalid"
#define ESTR_TASK_F_PRED_MST_SL_ADD_TAB_INCONS       "Offset table do not correspond to I/O configuration table"
#define ESTR_TASK_F_EXPL_PRM_FIELD_LEN               "Size indicator of parameter data table corrupt"
#define ESTR_TASK_F_PRED_MSTSL_CFG_ADD_INPUT_INCONS  "Num of inputs in add tab not equal I/O configuration"
#define ESTR_TASK_F_PRED_MSTSL_CFG_ADD_OUTPUT_INCONS "Num of outputs in add tab not equal I/O configuration"
#define ESTR_TASK_F_UNKNOWN_DATA_TYPE                "Unknown data type in I/O configuration"
#define ESTR_TASK_F_MODULE_DATA_SIZE                 "Data type does not correspond to its configured length"
#define ESTR_TASK_F_OUTPUT_OFF_RANGE                 "Configured output offset address out of range"
#define ESTR_TASK_F_INPUT_OFF_RANGE                  "Configured input offset address out of range"
#define ESTR_TASK_F_WRONG_TYPE_OF_CONNECTION         "One predefined connection type is unknown"
#define ESTR_TASK_F_TYPE_CONNECTION_REDEFINITION     "Multiple connections defined in parallel"
#define ESTR_TASK_F_EXP_PACKET_LESS_PROD_INHIBIT     "Configured EXP_PCKT_RATE less then PROD_INHIBIT_TIME"
#define ESTR_TASK_F_PRM_FIELD_LEN_INCONSISTENT       "Parameter field DNM_SET_ATTR_DATA in data set inconsitent"
#define ESTR_TASK_F_NO_CAN                           "No device responding on CAN network at cfgured baudrate"
#define ESTR_TASK_F_DATA_AREA                        "Illegal address area"
#define ESTR_TASK_F_DATA_ADR                         "Overflow in the buffer length"
#define ESTR_TASK_F_DATA_CNT                         "Illegal data count"
#define ESTR_TASK_F_DATA_TYPE                        "Unknown data type"
#define ESTR_TASK_F_FUNCTION                         "Unknown function"
#define ESTR_TASK_F_DATABASE                         "No database found on the system"
#define ESTR_TASK_F_DATABASE_READ                    "Failure in reading database"

#endif /* dnmerrs.h */

