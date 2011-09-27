/* Microsoft Developer Support
   Copyright (c) 1992-1995 Microsoft Corporation */

#include <windows.h>
#include <stdio.h>

/* messages.h is created by mc.exe when compiling messages.mc */
#include "messages.h"

#define MAX_MESSAGES 5
#define MAX_MSG_LENGTH 1024
#define SEVERITY_MASK 0xC0000000
#define FACILITY_MASK 0x0FFF0000
#define MSG_ID_MASK 0x0000FFFF


/********************************************************************
* FUNCTION: myPutMsg(HINSTANCE hLib, LPVOID lpArgs, DWORD dwMsgId)  *
*                                                                   *
* PURPOSE: format and output error dwMsgId, with insert strings     *
*          lpArgs, from the messagetable resource in the DLL        *
*          referenced by handle hLib                                *
*                                                                   *
* INPUT: Library handle, insert strings, and message ID number      *
*                                                                   *
* RETURNS: none                                                     *
********************************************************************/

void myPutMsg(HINSTANCE hLib, LPVOID lpArgs, DWORD dwMsgId)
{
  BOOL bSuccess;
  LPTSTR msgBuf;  /* hold text of the error message that we build */
  int dwCode;  /* hold various codes extracted from dwMsgId */

  /* Here is the layout of the message ID:

   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
  +---+-+-+-----------------------+-------------------------------+
  |Sev|C|R|     Facility          |               Code            |
  +---+-+-+-----------------------+-------------------------------+

  where

      Sev - is the severity code
      C - is the Customer code flag
      R - is a reserved bit
      Facility - is the facility code
      Code - is the facility's status code
  */

  printf("Severity: ");
  /* output the severity and facility code. Mask off the severity */
  /* bits with SEVERITY_MASK and shift them down */
  dwCode = (dwMsgId & SEVERITY_MASK) >> 30;
  switch (dwCode)
    {
    case STATUS_SEVERITY_WARNING:
      printf("STATUS_SEVERITY_WARNING"); break;
    case STATUS_SEVERITY_SUCCESS:
      printf("STATUS_SEVERITY_SUCCESS"); break;
    case STATUS_SEVERITY_INFORMATIONAL:
      printf("STATUS_SEVERITY_INFORMATIONAL"); break;
    case STATUS_SEVERITY_ERROR:
      printf("STATUS_SEVERITY_ERROR"); break;
    default:
      printf("Unknown!"); break;
    }
  printf ("\nFacility: ");
  /* Mask off the facility bits with FACILITY_MASK and shift them down */
  dwCode = (dwMsgId & FACILITY_MASK) >> 16;
  switch (dwCode)
    {
    case FACILITY_SYSTEM:
      printf("FACILITY_SYSTEM"); break;
    case FACILITY_STUBS:
      printf("FACILITY_STUBS"); break;
    case FACILITY_RUNTIME:
      printf("FACILITY_RUNTIME"); break;
    case FACILITY_IO_ERROR_CODE:
      printf("FACILITY_IO_ERROR_CODE"); break;
    default:
      printf("Unknown!"); break;
    }
  /* retrieve and format the message from the messagetable DLL */
  bSuccess = FormatMessage(
      FORMAT_MESSAGE_FROM_HMODULE | /* get the message from the DLL */
      FORMAT_MESSAGE_ALLOCATE_BUFFER | /* allocate the msg buffer for us */
      FORMAT_MESSAGE_ARGUMENT_ARRAY | /* lpArgs is an array of pointers */
      60, /* line length for the mesages */
      hLib, /* the messagetable DLL handle */
      dwMsgId, /* message ID */
      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), /* language ID */
      (LPTSTR) &msgBuf, /* address of pointer to buffer for message */
      MAX_MSG_LENGTH, /* maximum size of the message buffer */
      lpArgs); /* array of insert strings for the message */
  if (!bSuccess)
    printf("Error %d from FormatMessage\n", GetLastError());
  else
    {
    /* mask off the actual message number with MSG_ID_MASK and show it */
    printf("\nError: %d: %s", dwMsgId & MSG_ID_MASK, msgBuf);
    /* Free the buffer that FormatMessage allocated for us. */
    LocalFree((HLOCAL) msgBuf);
    }
  puts("\n__________\n");
  CloseHandle(hLib);
}

/********************************************************************
* FUNCTION: main()                                                  *
*                                                                   *
* PURPOSE: Load the message resource DLL, and call myPutMsg() to    *
*          format and output error messages to the user             *
*                                                                   *
* INPUT: none                                                       *
*                                                                   *
* RETURNS: none                                                     *
********************************************************************/

int main()
{
  HINSTANCE hLib;  /* handle to the messagetable DLL */
  char *aInsertStrs[8];  /* array of pointers to insert strings */

  // check if Win32s, if so display notice and terminate
    if( GetVersion() & 0x80000000 )
    {
      MessageBox( NULL,
         "This application cannot run on Windows 3.1.\n"
         "This application will now terminate.",
         "MsgTable",
         MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      return( 1 );
    }

  /* Load the resource library without calling any entry points since */
  /* this is a resource-only DLL */
  hLib = LoadLibraryEx("messages.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
  if (!hLib)
    printf("Error %d from LoadLibrary\n", GetLastError());

  /* Output some error messages from the messagetable DLL */
  /* The first three have no insert strings */
  myPutMsg(hLib, NULL, MSG_BAD_COMMAND);
  myPutMsg(hLib, NULL, MSG_BAD_PARM1);
  myPutMsg(hLib, NULL, MSG_STRIKE_ANY_KEY);
  /* wait for user to hit enter as per last error message */
  getchar();
  /* The next two messages contain insert strings - set up our array */
  /* of insert strings and pass the array to myPutMsg() */
  aInsertStrs[0] = "foo.c";
  aInsertStrs[1] = "BAR";
  myPutMsg(hLib, aInsertStrs, MSG_CMD_DELETE);
  aInsertStrs[0] = "47";
  aInsertStrs[1] = "5";
  myPutMsg(hLib, aInsertStrs, MSG_RETRYS);
  return(0);
}
