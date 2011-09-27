/*++ BUILD Version: 0001    // Increment this if a change has global effects

Copyright (c) 1996  Microsoft Corporation

Module Name:

    pdhmsg.h
       (generated from pdhmsg.mc)

Abstract:

   Event message definititions used by routines by PDH.DLL

Created:

    6-Feb-96   Bob Watson (a-robw)

Revision History:

--*/
#ifndef _PDH_MSG_H_
#define _PDH_MSG_H_
//
//     PDH DLL messages
//
//
//      Success Messages
//
//         the Win32 error value ERROR_SUCCESS is used for success returns
//
//      MESSAGE NAME FORMAT
//
//          PDH_CSTATUS_...   messages are data item status message and
//                     are returned in reference to the status of a data 
//                     item
//          PDH_...           messages are returned by FUNCTIONS only and
//                     not used as data item status values
//
//      Success Messages
//         These messages are normally returned when the operation completed
//         successfully.
//
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: PDH_CSTATUS_VALID_DATA
//
// MessageText:
//
//  The returned data is valid.
//
#define PDH_CSTATUS_VALID_DATA           ((DWORD)0x00000000L)

//
// MessageId: PDH_CSTATUS_NEW_DATA
//
// MessageText:
//
//  The return data value is valid and different from the last sample.
//
#define PDH_CSTATUS_NEW_DATA             ((DWORD)0x00000001L)

//
//        Informational messages
//
//  None
//
//      Warning Messages
//         These messages are returned when the function has completed 
//         successfully but the results may be different than expected.
//
//
// MessageId: PDH_CSTATUS_NO_MACHINE
//
// MessageText:
//
//  Unable to connect to specified machine or machine is off line.
//
#define PDH_CSTATUS_NO_MACHINE           ((DWORD)0x800007D0L)

//
// MessageId: PDH_CSTATUS_NO_INSTANCE
//
// MessageText:
//
//  The specified instance is not present.
//
#define PDH_CSTATUS_NO_INSTANCE          ((DWORD)0x800007D1L)

//
// MessageId: PDH_MORE_DATA
//
// MessageText:
//
//  There is more data to return than would fit in the supplied buffer. Allocate
//  a larger buffer and call the function again.
//
#define PDH_MORE_DATA                    ((DWORD)0x800007D2L)

//
// MessageId: PDH_CSTATUS_ITEM_NOT_VALIDATED
//
// MessageText:
//
//  The data item has been added to the query, but has not been validated nor 
//  accessed. No other status information on this data item is available.
//
#define PDH_CSTATUS_ITEM_NOT_VALIDATED   ((DWORD)0x800007D3L)

//
// MessageId: PDH_RETRY
//
// MessageText:
//
//  The selected operation should be retried.
//
#define PDH_RETRY                        ((DWORD)0x800007D4L)

//
// MessageId: PDH_NO_DATA
//
// MessageText:
//
//  No data to return.
//
#define PDH_NO_DATA                      ((DWORD)0x800007D5L)

//
// MessageId: PDH_CALC_NEGATIVE_DENOMINATOR
//
// MessageText:
//
//  A counter with a negative denominator value was detected.
//
#define PDH_CALC_NEGATIVE_DENOMINATOR    ((DWORD)0x800007D6L)

//
// MessageId: PDH_CALC_NEGATIVE_TIMEBASE
//
// MessageText:
//
//  A counter with a negative timebase value was detected.
//
#define PDH_CALC_NEGATIVE_TIMEBASE       ((DWORD)0x800007D7L)

//
// MessageId: PDH_CALC_NEGATIVE_VALUE
//
// MessageText:
//
//  A counter with a negative value was detected.
//
#define PDH_CALC_NEGATIVE_VALUE          ((DWORD)0x800007D8L)

//
// MessageId: PDH_DIALOG_CANCELLED
//
// MessageText:
//
//  The user cancelled the dialog box.
//
#define PDH_DIALOG_CANCELLED             ((DWORD)0x800007D9L)

//
//     Error Messages
//        These messages are returned when the function could not complete
//        as requested and some corrective action may be required by the
//        the caller or the user.
//
//
// MessageId: PDH_CSTATUS_NO_OBJECT
//
// MessageText:
//
//  The specified object is not found on the system.
//
#define PDH_CSTATUS_NO_OBJECT            ((DWORD)0xC0000BB8L)

//
// MessageId: PDH_CSTATUS_NO_COUNTER
//
// MessageText:
//
//  The specified counter could not be found.
//
#define PDH_CSTATUS_NO_COUNTER           ((DWORD)0xC0000BB9L)

//
// MessageId: PDH_CSTATUS_INVALID_DATA
//
// MessageText:
//
//  The returned data is not valid.
//
#define PDH_CSTATUS_INVALID_DATA         ((DWORD)0xC0000BBAL)

//
// MessageId: PDH_MEMORY_ALLOCATION_FAILURE
//
// MessageText:
//
//  A PDH function could not allocate enough temporary memory to complete the
//  operation. Close some applications or extend the pagefile and retry the 
//  function.
//
#define PDH_MEMORY_ALLOCATION_FAILURE    ((DWORD)0xC0000BBBL)

//
// MessageId: PDH_INVALID_HANDLE
//
// MessageText:
//
//  The handle is not a valid PDH object.
//
#define PDH_INVALID_HANDLE               ((DWORD)0xC0000BBCL)

//
// MessageId: PDH_INVALID_ARGUMENT
//
// MessageText:
//
//  A required argument is missing or incorrect.
//
#define PDH_INVALID_ARGUMENT             ((DWORD)0xC0000BBDL)

//
// MessageId: PDH_FUNCTION_NOT_FOUND
//
// MessageText:
//
//  Unable to find the specified function.
//
#define PDH_FUNCTION_NOT_FOUND           ((DWORD)0xC0000BBEL)

//
// MessageId: PDH_CSTATUS_NO_COUNTERNAME
//
// MessageText:
//
//  No counter was specified.
//
#define PDH_CSTATUS_NO_COUNTERNAME       ((DWORD)0xC0000BBFL)

//
// MessageId: PDH_CSTATUS_BAD_COUNTERNAME
//
// MessageText:
//
//  Unable to parse the counter path. Check the format and syntax of the 
//  specified path.
//
#define PDH_CSTATUS_BAD_COUNTERNAME      ((DWORD)0xC0000BC0L)

//
// MessageId: PDH_INVALID_BUFFER
//
// MessageText:
//
//  The buffer passed by the caller is invalid.
//
#define PDH_INVALID_BUFFER               ((DWORD)0xC0000BC1L)

//
// MessageId: PDH_INSUFFICIENT_BUFFER
//
// MessageText:
//
//  The requested data is larger than the buffer supplied. Unable to return the
//  requested data.
//
#define PDH_INSUFFICIENT_BUFFER          ((DWORD)0xC0000BC2L)

//
// MessageId: PDH_CANNOT_CONNECT_MACHINE
//
// MessageText:
//
//  Unable to connect to the requested machine.
//
#define PDH_CANNOT_CONNECT_MACHINE       ((DWORD)0xC0000BC3L)

//
// MessageId: PDH_INVALID_PATH
//
// MessageText:
//
//  The specified counter path could not be interpreted.
//
#define PDH_INVALID_PATH                 ((DWORD)0xC0000BC4L)

//
// MessageId: PDH_INVALID_INSTANCE
//
// MessageText:
//
//  The instance name could not be read from the specified counter path.
//
#define PDH_INVALID_INSTANCE             ((DWORD)0xC0000BC5L)

//
// MessageId: PDH_INVALID_DATA
//
// MessageText:
//
//  The data is not valid.
//
#define PDH_INVALID_DATA                 ((DWORD)0xC0000BC6L)

//
// MessageId: PDH_NO_DIALOG_DATA
//
// MessageText:
//
//  The dialog box data block was missing or invalid.
//
#define PDH_NO_DIALOG_DATA               ((DWORD)0xC0000BC7L)

//
// MessageId: PDH_CANNOT_READ_NAME_STRINGS
//
// MessageText:
//
//  Unable to read the counter and/or explain text from the specified machine.
//
#define PDH_CANNOT_READ_NAME_STRINGS     ((DWORD)0xC0000BC8L)

#endif //_PDH_MSG_H_
// end of generated file
