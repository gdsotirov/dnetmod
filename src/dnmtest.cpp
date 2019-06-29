/****************************************************************************
 *  DeviceNet Module 0.2                                                    *
 ****************************************************************************
 *  Copyright   : Georgi D. Sotirov, gdsotirov@gmail.com                    *
 *  File        : dnmtest.h                 Type        : source            *
 *  Description : Test application for the module.                          *
 ****************************************************************************/

/**
 * @file dnmtest.cpp
 * @brief Test application for the module.
 *
 * Provides and interface to configure interface and device, read and write
 * data, read and write of attributes and executions of services.
 */

#include <stdio.h>
#include <string.h>

#include "dnmerrs.h"
#include "dnetmod.h"

/** Maximum length of short values */
#define DN_SHORTS_MAX_LEN 256
/** Maximum length of bytes */
#define DN_BYTES_MAX_LEN  256

/**
 * DeviceNet data types
 */
typedef enum DNTYPESTag {
    DN_SINT,  //!< DN_SINT
    DN_USINT, //!< DN_USINT
    DN_INT,   //!< DN_INT
    DN_UINT,  //!< DN_UINT
    DN_DINT,  //!< DN_DINT
    DN_UDINT, //!< DN_UDINT
    DN_REAL,  //!< DN_REAL
    DN_SHORTS,//!< DN_SHORTS
    DN_BYTES  //!< DN_BYTES
} DNTYPES;

/** Accepts the result of getchar function called to clear the input buffer */
int iBufClearCR = 0;

void PrintError(int);
int SetBufferLen(void **, DNTYPES *, unsigned short *);
int SetBufferValue(void **, DNTYPES, unsigned short);
int PrintBuffer(DNTYPES, void *, unsigned short);
unsigned char BaudRate(void);
unsigned char ConnType(void);

/**
 * @brief Main test program
 * @return Zero on success, non-zero otherwise.
 */
int main(void) {
    int i = 0;
    int iMRes = 0;
    int iErr = 0;
    int iSize = 0;
    DNTYPES Type;
    unsigned short usBrdNum = 0;
    unsigned short usMACID = 0;
    unsigned short usILen = 0;
    unsigned short usOLen = 0;
    unsigned short usEPR = 0;
    unsigned short usSrvCode = 0;
    unsigned short usSDLen = 0;
    unsigned short usClsId = 0;
    unsigned short usInstId = 0;
    unsigned short usParamId = 0;
    unsigned short usBufLen = 0;
    CCIFInterface *pInterface = new CCIFInterface(0, 8, 8, 0, 0);
    CCIFDevice *pDevice = NULL;
    char *IOBuf = NULL;

    do {
        printf("------------------------------\n");
        printf(" DeviceNet Tester 0.1 by GDS  \n");
        printf("------------------------------\n");
        printf(" i - Interface configuration\n");
        printf(" d - Device configuration\n");
        printf(" r - Read data\n");
        printf(" w - Write data\n");
        printf(" g - Get device attribute\n");
        printf(" s - Set device attribute\n");
        printf(" e - Execute service\n");
        printf(" q - Quit\n");
        printf("------------------------------\n");
        printf(" > ");
        iMRes = getchar();
        iBufClearCR = getchar();
        switch ( iMRes ) {
            /***** Interface configuration *****/
            case 'i':
                if ( pInterface == NULL )
                    pInterface = new CCIFInterface(0, 8, 8, 0, 0);
                else pInterface->Close();
                printf("Board number: ");
                scanf("%hu", &usBrdNum);
                pInterface->SetBoardNum(usBrdNum);
                printf("Interface MACID: ");
                scanf("%hu", &usMACID);
                pInterface->SetMacID(static_cast<unsigned char>(usMACID));
                pInterface->SetBaudRate(BaudRate());
                printf("Opening interface. Please wait...\n");
                if ( (iErr = pInterface->Open()) != ERR_NOERR )
                    PrintError(iErr);
            break;
            /***** Device configuration *****/
            case 'd':
                if ( pDevice == NULL )
                    pDevice = new CCIFDevice(0, 0, 0, 0, 0, pInterface);
                else pDevice->Unallocate();
                printf("Device MACID: ");
                scanf("%hu", &usMACID);
                pDevice->SetMacID(static_cast<unsigned char>(usMACID));
                pDevice->SetConnType(ConnType());
                printf("Input length, B: ");
                scanf("%hu", &usILen);
                pDevice->SetConsumedConnSize(static_cast<unsigned char>(usILen));
                printf("Output length, B: ");
                scanf("%hu", &usOLen);
                pDevice->SetProducedConnSize(static_cast<unsigned char>(usOLen));
                printf("EPR, ms: ");
                scanf("%hu", &usEPR);
                iBufClearCR = getchar();
                pDevice->SetEPR(usEPR);
                printf("Starting communication with device. Please wait...\n");
                if ( (iErr = pDevice->Allocate()) != ERR_NOERR )
                    PrintError(iErr);
            break;
            /***** Read data *****/
            case 'r':
                if ( pDevice != NULL ) {
                    iSize = pDevice->GetConsumedConnSize();
                    IOBuf = new char[iSize];
                    if ( IOBuf == NULL ) {
                        printf("Can't allocate memory for operation!\n");
                        break;
                    }
                    printf("Reading data. Please wait...\n");
                    if ( (iErr = pDevice->ReadIOData(iSize, IOBuf)) == ERR_NOERR ) {
                        printf("Data bytes returned from device:\n");
                        printf("-");
                        for ( i = 0; i < iSize; i++ ) {
                            unsigned short usVal = *(reinterpret_cast<unsigned char*>(IOBuf + i));
                            printf("%hu-", usVal);
                        }
                        printf("\n\n");
                    }
                    else PrintError(iErr);
                    if ( IOBuf != NULL )
                        delete [] IOBuf;
                    IOBuf = NULL;
                }
                else printf("Please configure your device first.\n");
            break;
            /***** Write data *****/
            case 'w':
                if ( pDevice != NULL ) {
                   iSize = pDevice->GetProducedConnSize();
                   IOBuf = new char[iSize];
                   if ( IOBuf == NULL ) {
                       printf("Can't allocate memory for operation!\n");
                       break;
                   }
                   for ( i = 0; i < iSize; i++ ) {
                       unsigned short usVal = 0;
                       do {
                           printf("Enter value for byte #%d [0-255]: ", i);
                           scanf("%hu", &usVal);
                           iBufClearCR = getchar();
                       } while ( usVal > 255 );
                       *(IOBuf + i) = static_cast<unsigned char>(usVal);
                   }
                   printf("Writing data. Please wait...\n");
                   if ( (iErr = pDevice->WriteIOData(iSize, IOBuf)) == ERR_NOERR )
                       printf("Data written successfuly.\n");
                   else PrintError(iErr);
                   if ( IOBuf != NULL )
                       delete [] IOBuf;
                   IOBuf = NULL;
               }
               else printf("Please configure your device first.\n");
            break;
            /***** Get device attribute *****/
            case 'g':
                if ( pDevice != NULL ) {
                    printf("Class ID: ");
                    scanf("%hu", &usClsId);
                    iBufClearCR = getchar();
                    printf("Instance ID: ");
                    scanf("%hu", &usInstId);
                    iBufClearCR = getchar();
                    printf("Parameter ID: ");
                    scanf("%hu", &usParamId);
                    iBufClearCR = getchar();
                    if ( SetBufferLen(reinterpret_cast<void **>(&IOBuf), &Type, &usBufLen) )
                        break;
                    if ( (iErr = pDevice->GetAttribute(usClsId, usInstId, static_cast<unsigned char>
                        (usParamId), usBufLen, IOBuf, NULL)) == ERR_NOERR ) {
                        PrintBuffer(Type, IOBuf, usBufLen);
                        printf("\n");
                    }
                    else PrintError(iErr);
                    if ( IOBuf != NULL )
                        delete [] IOBuf;
                    IOBuf = NULL;
                }
                else printf("Please configure your device first.\n");
            break;
            /***** Set device attribute *****/
            case 's':
                if ( pDevice != NULL ) {
                   printf("Class ID: ");
                   scanf("%hu", &usClsId);
                   printf("Instance ID: ");
                   scanf("%hu", &usInstId);
                   printf("Parameter ID: ");
                   scanf("%hu", &usParamId);
                   iBufClearCR = getchar();
                   if ( SetBufferLen(reinterpret_cast<void **>(&IOBuf), &Type, &usBufLen) )
                       break;
                   if ( SetBufferValue(reinterpret_cast<void **>(&IOBuf), Type, usBufLen) )
                       break;
                   if ( (iErr = pDevice->SetAttribute(usClsId, usInstId,
                       static_cast<unsigned char>(usParamId), usBufLen, IOBuf)) == ERR_NOERR )
                       printf("Parameter set.\n");
                   else PrintError(iErr);
                   if ( IOBuf != NULL )
                       delete [] IOBuf;
                   IOBuf = NULL;
                }
                else printf("Please configure your device first.\n");
            break;
            /***** Execute service *****/
            case 'e':
                if ( pDevice != NULL ) {
                    printf("Service code: ");
                    scanf("%hu", &usSrvCode);
                    printf("Class ID: ");
                    scanf("%hu", &usClsId);
                    printf("Instance ID: ");
                    scanf("%hu", &usInstId);
                    printf("Service data length: ");
                    do {
                        scanf("%hu", &usSDLen);
                    }
                    while ( usSDLen > 256 );
                    if ( usSDLen == 0 )
                        IOBuf = new char;
                    else IOBuf = new char[usSDLen];
                    if ( IOBuf == NULL ) {
                        printf("Can't allocate memory for operation!\n");
                        break;
                    }
                    for ( i = 0; i < usSDLen; i++ ) {
                        unsigned short usVal = 0;
                        do {
                            printf("Enter value for byte #%d [0-255]: ", i);
                            scanf("%hu", &usVal);
                        }
                        while ( usVal > 255 );
                        *(IOBuf + i) = static_cast<unsigned char>(usVal);
                    }
                    iBufClearCR = getchar();
                    printf("Executing service. Please wait...\n");
                    if ( (iErr = pDevice->ExecService(static_cast<unsigned char>(usSrvCode),
                        usClsId, usInstId, usSDLen, IOBuf)) == 0 ) {
                        printf("Service executed successfully!\n");
                        printf("Received data:\n");
                        for ( i = 0; i < usSDLen; i++ )
                            printf("%d - %d\n", i, *(IOBuf + i));
                    }
                    else PrintError(iErr);
                    if ( IOBuf != NULL )
                        delete [] IOBuf;
                    IOBuf = NULL;
                }
                else printf("Please configure your device first.\n");
            break;
            /***** Quit *****/
            case 'q':
                if ( ISPTRVALID(pDevice, CDevice) )
                    delete pDevice;
                if ( ISPTRVALID(pInterface, CInterface) )
                    delete pInterface;
                if ( ISPTRVALID(IOBuf, char) )
                    delete [] IOBuf;
            break;
            default: break;
        } /* switch */
    }
    while ( iMRes != 'q' );
} /* main */

/**
 * @brief Prints error message by code
 *
 * Prints the corresponding to the error code message.
 * @param iErrCode Error code
 */
void PrintError(int iErrCode) {
    char strErrMsg[DNETMOD_MAX_ERRMSG_LEN];

    GetErrMsg(sizeof(strErrMsg), strErrMsg);
    printf("*** ERROR ***\n");
    printf("Code    : %d\n", iErrCode);
    printf("Message : %s\n", strErrMsg);
}

/**
 * @brief Selects baud rate
 *
 * Manages proper selection of baud rate by the user.
 * @return Baud rate identifier
 */
unsigned char BaudRate(void) {
    short val = 0;

    printf("------------------------------\n");
    printf(" Select baud rate\n");
    printf("------------------------------\n");
    printf(" 1 - 125 000 kbps\n");
    printf(" 2 - 250 000 kbps\n");
    printf(" 5 - 500 000 kbps\n");
    printf("------------------------------\n");
    do {
        printf(" > ");
        scanf("%hd", &val);
        iBufClearCR = getchar();
    } while ( val != 1 && val != 2 && val != 5 );
    if ( val == 5 ) {
        return DEVICENET_BAUD_500K;
    }
    else if ( val == 2 ) {
            return DEVICENET_BAUD_250K;
         }
         else return DEVICENET_BAUD_125K;
}

/**
 * @brief Selects connection type
 *
 * Manages proper selection of connection type by the user.
 * @return Connection type identifier
 */
unsigned char ConnType(void) {
    unsigned short usChoice = 0;
    unsigned char  ucResult = 0;

    printf("------------------------------\n");
    printf(" Select connection type\n");
    printf("------------------------------\n");
    printf(" 1 - Poll\n");
    printf(" 2 - Strobe\n");
    printf(" 3 - COS\n");
    printf(" 4 - Cyclic\n");
    printf("------------------------------\n");
    do {
        printf(" > ");
        scanf("%hu", &usChoice);
        iBufClearCR = getchar();
    }
    while ( usChoice < 1 && usChoice > 4 );
    switch ( usChoice ) {
        case 1 : ucResult |= DEVICENET_CONN_POLLED;
                 break;
        case 2 : ucResult |= DEVICENET_CONN_STRBED;
                 break;
        case 3 : ucResult |= DEVICENET_CONN_COS;
                 break;
        case 4 : ucResult |= DEVICENET_CONN_CYCLIC;
                 break;
    }

    return ucResult;
}

/**
 * @brief Sets buffer length
 *
 * Manages proper selection of data type by the user and allocates
 * the necessary buffer.
 * @param pBuf Pointer to the buffer.
 * @param Type Data type identifier.
 * @param pusBufLen Size of the buffer in bytes.
 * @return Zero on success, non-zero otherwise.
 */
int SetBufferLen(void **pBuf, DNTYPES *Type, unsigned short *pusBufLen) {
    int iRes = 0;

    printf("-----------------------------------\n");
    printf(" Chose type\n");
    printf("-----------------------------------\n");
    printf(" C type             DeviceNet type\n");
    printf("-----------------------------------\n");
    printf(" 0 - char           SINT\n");
    printf(" 1 - unsigned char  USINT\n");
    printf(" 2 - short          INT\n");
    printf(" 3 - unsigned short UINT\n");
    printf(" 4 - int            DINT\n");
    printf(" 5 - unsigned int   UDINT\n");
    printf(" 6 - float          REAL\n");
    printf(" 7 - string         SHORT-STRING\n");
    printf(" 8 - bytes\n");
    printf("---------------------------------\n");
    do {
        printf(" > ");
        scanf("%d", &iRes);
        iBufClearCR = getchar();
    }
    while ( iRes < 0 && iRes > 7 );
    switch ( iRes ) {
        case 0:
            *pBuf = new char;
            *Type = DN_SINT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(char);
        break;
        case 1:
            *pBuf = new unsigned char;
            *Type = DN_USINT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(unsigned char);
        break;
        case 2:
            *pBuf = new short;
            *Type = DN_INT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(short);
        break;
        case 3:
            *pBuf = new unsigned short;
            *Type = DN_UINT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(unsigned short);
        break;
        case 4:
            *pBuf = new int;
            *Type = DN_DINT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(int);
        break;
        case 5:
            *pBuf = new unsigned int;
            *Type = DN_UDINT;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(unsigned int);
        break;
        case 6:
            *pBuf = new float;
            *Type = DN_REAL;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = sizeof(float);
        break;
        case 7:
            *pBuf = new char[DN_SHORTS_MAX_LEN];
            *Type = DN_SHORTS;
            if ( ISPTRVALID(pusBufLen, unsigned short) )
                *pusBufLen = DN_SHORTS_MAX_LEN;
        break;
        case 8:
            if ( ISPTRVALID(pusBufLen, unsigned short) ) {
                do {
                    printf("Count bytes [1-%d]: ", DN_BYTES_MAX_LEN);
                    scanf("%hu", pusBufLen);
                    iBufClearCR = getchar();
                } while ( *pusBufLen < 1 || *pusBufLen > DN_BYTES_MAX_LEN );
                *pBuf = new char[*pusBufLen];
                *Type = DN_BYTES;
            }
        break;
    }
    if ( *pBuf == NULL ) {
        printf("ERROR: Can't allocate memory for operation!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Sets buffer value
 *
 * Manages reading data for the buffer depending on the data type and stores
 * the value in the buffer.
 * @param buf Pinter to the buffer.
 * @param typ Data type identifier.
 * @param cbytes Size of the buffer in bytes.
 * @return
 */
int SetBufferValue(void **buf, DNTYPES typ, unsigned short cbytes) {
    int lval;
    float fval;

    switch ( typ ) {
        case DN_SINT  : printf("SINT value: ");
                        scanf("%d", &lval);
                        *(static_cast<char *>(*buf)) = static_cast<char>(lval);
        break;

        case DN_USINT : printf("USINT value: ");
                        scanf("%d", &lval);
                        *(static_cast<unsigned char *>(*buf)) = static_cast<unsigned char>(lval);
        break;

        case DN_INT   : printf("INT value: ");
                        scanf("%d", &lval);
                        *(static_cast<short *>(*buf)) = static_cast<short>(lval);
        break;

        case DN_UINT  : printf("UINT value: ");
                        scanf("%d", &lval);
                        *(static_cast<unsigned short *>(*buf)) = static_cast<unsigned short>(lval);
        break;

        case DN_DINT  : printf("DINT value: ");
                        scanf("%d", &lval);
                        *(static_cast<int *>(*buf)) = static_cast<int>(lval);
        break;

        case DN_UDINT : printf("UDINT value: ");
                        scanf("%d", &lval);
                        *(static_cast<unsigned int *>(*buf)) = static_cast<unsigned int>(lval);
        break;

        case DN_REAL  : printf("Real value: ");
                        scanf("%f", &fval);
                        *(static_cast<float *>(*buf)) = fval;
        break;

        case DN_SHORTS: printf("String value: ");
                        scanf("%256s", (static_cast<char *>(*buf)) + 1); // Scan up to DN_SHORTS_MAX_LEN
                        *(static_cast<char *>(*buf)) = static_cast<unsigned char>(strlen((static_cast<char *>(*buf)) + 1));
        break;

        case DN_BYTES:
            printf("Bytes values: ");
            for ( int i = 0; i < cbytes; i++ ) {
                unsigned short val = 0;

                do {
                    printf("Enter value for byte #%d [0-255]: ", i);
                    scanf("%hu", &val);
                }
                while ( val > 255 );
                *(reinterpret_cast<unsigned char *>(buf) + i) = static_cast<unsigned char>(val);
            }
        break;
    }
    iBufClearCR = getchar();
    return 0;
}

/**
 * @brief Prints buffer
 *
 * Prints the value from the buffer properly depending on the data type.
 * @param Type Data type.
 * @param Buf Pointer to the buffer.
 * @param usBufLen Size of the buffer in bytes.
 * @return
 */
int PrintBuffer(DNTYPES Type, void *Buf, unsigned short usBufLen) {
    char * cBuf = NULLPTR(char);

    printf(" Value: ");
    switch ( Type ) {
        case DN_SINT  : printf("%hi", *(reinterpret_cast<char *>(Buf)));
                        break;
        case DN_USINT : printf("%hu", *(reinterpret_cast<unsigned char *>(Buf)));
                        break;
        case DN_INT   : printf("%hi" , *(reinterpret_cast<short *>(Buf)));
                        break;
        case DN_UINT  : printf("%hu" , *(reinterpret_cast<unsigned short *>(Buf)));
                        break;
        case DN_DINT  : printf("%i" , *(reinterpret_cast<int *>(Buf)));
                        break;
        case DN_UDINT : printf("%ui" , *(reinterpret_cast<unsigned int *>(Buf)));
                        break;
        case DN_REAL  : printf("%f" , *(reinterpret_cast<float *>(Buf)));
                        break;
        case DN_SHORTS: cBuf = static_cast<char *>(Buf);
                        cBuf[*cBuf + 1] = 0;
                        printf("%s", &cBuf[1]);
                        break;
        case DN_BYTES:
            printf("Bytes:\n");
            for ( int i = 0; i < usBufLen; i++ ) {
                printf("#%d: %hu\n", i, *(reinterpret_cast<unsigned char *>(Buf) + i));
            }
        break;
    }
    return 0;
}

