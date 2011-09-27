/****************************************************************************\
*  INCLUDES, DEFINES
\****************************************************************************/
#define STRICT
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define PERR(api) printf("\n%s: Error %d from %s on line %d",  \
    __FILE__, GetLastError(), api, __LINE__);
#define PMSG(msg) printf("\n%s line %d: %s",  \
    __FILE__, __LINE__, msg);

// this event is signalled when the
//  worker thread ends
//
HANDLE                  hServDoneEvent = NULL;
SERVICE_STATUS          ssStatus;       // current status of the service

SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwGlobalErr;
DWORD                   TID = 0;
HANDLE                  threadHandle = NULL;
HANDLE                  pipeHandle;


/****************************************************************************\
* FUNCTION PROTOTYPES
\****************************************************************************/

VOID    WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
VOID    WINAPI service_ctrl(DWORD dwCtrlCode);
BOOL    ReportStatusToSCMgr(DWORD dwCurrentState,
                            DWORD dwWin32ExitCode,
                            DWORD dwCheckPoint,
                            DWORD dwWaitHint);
VOID    die(char *reason);
VOID    worker_thread(VOID *notUsed);
VOID    StopSimpleService(LPTSTR lpszMsg);
BOOL    WriteSD_ToA_File(PSECURITY_DESCRIPTOR psdAbsoluteSD, LPTSTR lpszFileName);


/****************************************************************************\
* GLOBAL VARIABLES AND TYPEDEFS
\****************************************************************************/

#define                               SZ_SD_BUF   100
#define                               SZ_SID_BUF   75
#define                               SZ_ACL_BUF  150

UCHAR                ucAbsSDBuf      [SZ_SD_BUF]  = "";
UCHAR                ucEvrSDBuf      [SZ_SD_BUF]  = "";
UCHAR                ucSIDBuf        [SZ_SID_BUF] = "";
UCHAR                ucPwrUsrsSIDBuf [SZ_SID_BUF] = "";
UCHAR                ucACLBuf        [SZ_ACL_BUF] = "";

DWORD                dwSID          = SZ_SID_BUF;
DWORD                dwDACL         = SZ_ACL_BUF;
BOOL                 bFloppiesAreLocked;

PSECURITY_DESCRIPTOR psdAbsoluteSD      = (PSECURITY_DESCRIPTOR)&ucAbsSDBuf;
PSECURITY_DESCRIPTOR psdEveryoneSD      = (PSECURITY_DESCRIPTOR)&ucEvrSDBuf;
PSID                 psidAdministrators = (PSID)&ucSIDBuf;
PSID                 psidPowerUsers     = (PSID)&ucPwrUsrsSIDBuf;
PACL                 pNewDACL           = (PACL)&ucACLBuf;



//  main() --
//      all main does is call StartServiceCtrlDispatcher
//      to register the main service thread.  When the
//      API returns, the service has stopped, so exit.
//
int
main()
{

    SERVICE_TABLE_ENTRY dispatchTable[] = {
        { TEXT("SimpleService"), (LPSERVICE_MAIN_FUNCTION)service_main },
        { NULL, NULL }
    };

  // check if Win32s, if so, display notice and terminate
        if( GetVersion() & 0x80000000 )
        {
          MessageBox( NULL,
             "This application cannot run on Windows 3.1 or Windows 95.\n"
             "This application will now terminate.",
             "SD_FLPPY",
             MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
          return( 1 );
        }

    #define FILE_TO_REDIRECT_STDOUT_TO "c:\\floplock.out"
//  freopen(FILE_TO_REDIRECT_STDOUT_TO,"w+",stdout);

    if (!StartServiceCtrlDispatcher(dispatchTable)) {
        StopSimpleService("StartServiceCtrlDispatcher failed.");
    }
}



//  service_main() --
//      this function takes care of actually starting the service,
//      informing the service controller at each step along the way.
//      After launching the worker thread, it waits on the event
//      that the worker thread will signal at its termination.
//
VOID WINAPI
service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{
    DWORD                   dwWait;
    SECURITY_ATTRIBUTES     sa;

    // register our service control handler:
    //
    sshStatusHandle = RegisterServiceCtrlHandler(
                                    TEXT("SimpleService"),
                                    service_ctrl);

    if (!sshStatusHandle)
        goto cleanup;

    // SERVICE_STATUS members that don't change in example
    //
    ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ssStatus.dwServiceSpecificExitCode = 0;


    // report the status to Service Control Manager.
    //
    if (!ReportStatusToSCMgr(
        SERVICE_START_PENDING, // service state
        NO_ERROR,              // exit code
        1,                     // checkpoint
        3000))                 // wait hint
        goto cleanup;

    // create the event object. The control handler function signals
    // this event when it receives the "stop" control code.
    //
    hServDoneEvent = CreateEvent(
        NULL,    // no security attributes
        TRUE,    // manual reset event
        FALSE,   // not-signalled
        NULL);   // no name

    if (hServDoneEvent == (HANDLE)NULL)
        goto cleanup;

    // report the status to the service control manager.
    //
    if (!ReportStatusToSCMgr(
        SERVICE_START_PENDING, // service state
        NO_ERROR,              // exit code
        2,                     // checkpoint
        3000))                 // wait hint
        goto cleanup;

    // Create a security descriptor that allows only local Administrators
    //   to do anything with the pipe.  Since Domain administrators are
    //   normally also local Administrators, this will serve most needs

    /************************************************************************\
    *
    * Build SIDs of local Administrators and Power Users.  Note that the Power
    *   Users group is defined on Windows NT machines, but not on Advanced
    *   Server machines.  This means that we will on Advanced Server machines
    *   have a DACL that has an ACE (for Power Users) that will never be used
    *
    \************************************************************************/

    {
      SID_IDENTIFIER_AUTHORITY siaNtAuthority = SECURITY_NT_AUTHORITY;

      InitializeSid(        psidAdministrators, &siaNtAuthority, 2 );
      InitializeSid(        psidPowerUsers,     &siaNtAuthority, 2 );

      *(GetSidSubAuthority( psidAdministrators, 0 )) = SECURITY_BUILTIN_DOMAIN_RID;
      *(GetSidSubAuthority( psidPowerUsers,     0 )) = SECURITY_BUILTIN_DOMAIN_RID;

      *(GetSidSubAuthority( psidAdministrators, 1 )) = DOMAIN_ALIAS_RID_ADMINS;
      *(GetSidSubAuthority( psidPowerUsers,     1 )) = DOMAIN_ALIAS_RID_POWER_USERS;
    }


    /************************************************************************\
    *
    * Initialize new DACL
    *
    \************************************************************************/

    if (!InitializeAcl(pNewDACL,
                       dwDACL,
                       ACL_REVISION2))
    { StopSimpleService("InitializeAcl");
    }

    /************************************************************************\
    *
    * Allow All access for local Administrators only
    *
    \************************************************************************/

    if (!AddAccessAllowedAce(pNewDACL,
                             ACL_REVISION2,
                             FILE_ALL_ACCESS,
                             psidAdministrators))
    { StopSimpleService("AddAccessAllowedAce");
    }

    /************************************************************************\
    *
    * If we unlock the floppies when the service stops, then for the sake of
    *   consistency, we have to also allow Power Users on the Admin-only DACL,
    *   since Power Users can stop services.  It would be inconsistent to try
    *   to lock Power Users away from their floppies if Power Users could get
    *   to the floppies simply by stopping the service
    *
    * It's still OK to use the same DACL for the pipe as for the floppies,
    *   that is, it's OK to let Power Users on the DACL for the pipe too.  The
    *   reason is that it is not generally (and certainly not by default) the
    *   case that an account is a member of Power Users on more than their own
    *   machines.  So, putting Power Users on the pipe let's Power Users admin
    *   the floppies via the pipe only on the machines on which they are
    *   actually Power Users, and again, on those machines they can stop the
    *   floppy-locking service as well
    *
    \************************************************************************/

    #define UNLOCK_AT_SERVICE_STOP (0==0)
    if     (UNLOCK_AT_SERVICE_STOP)
    { if (!AddAccessAllowedAce(pNewDACL,
                               ACL_REVISION2,
                               FILE_ALL_ACCESS,
                               psidPowerUsers))
      { StopSimpleService("AddAccessAllowedAce");
      }
    }

    /************************************************************************\
    *
    * Build SD in absolute format - first the Admins-only then the Everyone SD
    *
    \************************************************************************/

    if (!InitializeSecurityDescriptor(psdAbsoluteSD,
                                      SECURITY_DESCRIPTOR_REVISION))
    { StopSimpleService("InitializeSecurityDescriptor");
    }

    if (!InitializeSecurityDescriptor(psdEveryoneSD,
                                      SECURITY_DESCRIPTOR_REVISION))
    { StopSimpleService("InitializeSecurityDescriptor");
    }

    /************************************************************************\
    *
    * Set DACL into SD - first the Admins-only then the Everyone SD
    *
    \************************************************************************/

    if (!SetSecurityDescriptorDacl(psdAbsoluteSD,
                                   TRUE,      // fDaclPresent flag
                                   pNewDACL,
                                   FALSE))    // not a default DACL
    { StopSimpleService("SetSecurityDescriptorDacl");
    }

    if (!SetSecurityDescriptorDacl(psdEveryoneSD,
                                   TRUE,      // fDaclPresent flag
                                   (PACL)NULL,
                                   FALSE))    // not a default DACL
    { StopSimpleService("SetSecurityDescriptorDacl");
    }

    /************************************************************************\
    *
    * Check to see that SD is valid before attempting to write it to the file
    *
    \************************************************************************/

    if (!IsValidSecurityDescriptor(psdAbsoluteSD))
    { StopSimpleService("IsValidSecurityDescriptor");
    }

    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = psdAbsoluteSD;
    sa.bInheritHandle = TRUE;  // why not... we spawn no processes

    // open our named pipe...
    //
    pipeHandle = CreateNamedPipe(
                    "\\\\.\\pipe\\sd_flppy",  // name of pipe
                    PIPE_ACCESS_DUPLEX,     // pipe open mode
                    PIPE_TYPE_MESSAGE |
                    PIPE_READMODE_MESSAGE |
                    PIPE_WAIT,              // pipe IO type
                    1,                      // number of instances
                    0,                      // size of outbuf (0 == allocate as necessary)
                    0,                      // size of inbuf
                    1000,                   // default time-out value
                    &sa);                   // security attributes

    if (!pipeHandle) {
        StopSimpleService("CreateNamedPipe");
        return;
    }

    // Set the same DACL onto the floppies
    //

    /************************************************************************\
    *
    * Write SD to file system - first for A: then B:
    *
    \************************************************************************/

    if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\A:"))
    { StopSimpleService("Write of DACL to A: failed");
    }

    if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\B:"))
    { StopSimpleService("Write of DACL to B: failed");
    }

    bFloppiesAreLocked = TRUE;

    /************************************************************************\
    *
    * Works for CDROM drives as well - commented out as this samples is floppy
    *   only
    *
    \************************************************************************/
  /*
    if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\E:"))
    { StopSimpleService("Write of DACL to E: failed");
    }
  */
    /************************************************************************\
    *
    * Works for COM ports as well - commented out as this samples is floppy only
    *
    \************************************************************************/
  /*
    if (!WriteSD_ToA_File(psdAbsoluteSD,"COM1:"))
    { StopSimpleService("Write of DACL to COM1: failed");
    }
  */

    // start the thread that performs the work of the service.
    //
    threadHandle = CreateThread(
                    NULL,       // security attributes
                    0,          // stack size (0 means inherit parent's stack size)
                    (LPTHREAD_START_ROUTINE)worker_thread,
                    NULL,       // argument to thread
                    0,          // thread creation flags
                    &TID);      // pointer to thread ID

    if (!threadHandle)
        goto cleanup;

    // report the status to the service control manager.
    //
    if (!ReportStatusToSCMgr(
        SERVICE_RUNNING, // service state
        NO_ERROR,        // exit code
        0,               // checkpoint
        0))              // wait hint
        goto cleanup;

    // wait indefinitely until hServDoneEvent is signaled.
    //
    dwWait = WaitForSingleObject(
        hServDoneEvent,  // event object
        INFINITE);       // wait indefinitely

cleanup:

    if (hServDoneEvent != NULL)
        CloseHandle(hServDoneEvent);


    // try to report the stopped status to the service control manager.
    //
    if (sshStatusHandle != 0)
        (VOID)ReportStatusToSCMgr(
                            SERVICE_STOPPED,
                            dwGlobalErr,
                            0,
                            0);

    // When SERVICE MAIN FUNCTION returns in a single service
    // process, the StartServiceCtrlDispatcher function in
    // the main thread returns, terminating the process.
    //
    return;
}



//  service_ctrl() --
//      this function is called by the Service Controller whenever
//      someone calls ControlService in reference to our service.
//
VOID WINAPI
service_ctrl(DWORD dwCtrlCode)
{
    DWORD  dwState = SERVICE_RUNNING;

    // Handle the requested control code.
    //
    switch(dwCtrlCode) {

        // Pause the service if it is running.
        //
        case SERVICE_CONTROL_PAUSE:

            if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
                SuspendThread(threadHandle);
                dwState = SERVICE_PAUSED;
            }
            break;

        // Resume the paused service.
        //
        case SERVICE_CONTROL_CONTINUE:

            if (ssStatus.dwCurrentState == SERVICE_PAUSED) {
                ResumeThread(threadHandle);
                dwState = SERVICE_RUNNING;
            }
            break;

        // Stop the service.
        //
        case SERVICE_CONTROL_STOP:

            dwState = SERVICE_STOP_PENDING;

            // Report the status, specifying the checkpoint and waithint,
            //  before setting the termination event.
            //
            ReportStatusToSCMgr(
                    SERVICE_STOP_PENDING, // current state
                    NO_ERROR,             // exit code
                    1,                    // checkpoint
                    3000);                // waithint

            if     (UNLOCK_AT_SERVICE_STOP)
            { if (!WriteSD_ToA_File(psdEveryoneSD,"\\\\.\\A:"))
              { StopSimpleService("Unlock of A: failed, see log file");
              }
              if (!WriteSD_ToA_File(psdEveryoneSD,"\\\\.\\B:"))
              { StopSimpleService("Unlock of B: failed, see log file");
              }

              bFloppiesAreLocked = FALSE;
            }

            SetEvent(hServDoneEvent);
            return;

        // Update the service status.
        //
        case SERVICE_CONTROL_INTERROGATE:
            break;

        // invalid control code
        //
        default:
            break;

    }

    // send a status response.
    //
    ReportStatusToSCMgr(dwState, NO_ERROR, 0, 0);
}



//  worker_thread() --
//      this function does the actual nuts and bolts work that
//      the service requires.  It will also Pause or Stop when
//      asked by the service_ctrl function.
//
VOID
worker_thread(VOID *notUsed)
{
    char                 inbuf[180];
    char                 outbuf[180];
    BOOL                 ret;
    DWORD                bytesRead;
    DWORD                bytesWritten;
    DWORD                dwLen;

    // okay, our pipe has been created, let's enter the simple
    //  processing loop...
    //
    while (1) {

        // wait for a connection...
        //
        ConnectNamedPipe(pipeHandle, NULL);

        // grab whatever's coming through the pipe...
        //
        ret = ReadFile(
                    pipeHandle,     // file to read from
                    inbuf,          // address of input buffer
                    sizeof(inbuf),  // number of bytes to read
                    &bytesRead,     // number of bytes read
                    NULL);          // overlapped stuff, not needed

        if (!ret)
            // pipe's broken... go back and reconnect
            //
            continue;

        switch (inbuf[0])
        { case 'U':
            dwLen = sprintf(outbuf,"Floppies were unlocked");

            if (!WriteSD_ToA_File(psdEveryoneSD,"\\\\.\\A:"))
            { dwLen += sprintf(outbuf+dwLen,", unlock of A: failed, see log file");
            }
            if (!WriteSD_ToA_File(psdEveryoneSD,"\\\\.\\B:"))
            { dwLen += sprintf(outbuf+dwLen,", unlock of B: failed, see log file");
            }

            bFloppiesAreLocked = FALSE;
            break;

          case 'L':
            dwLen = sprintf(outbuf,"Floppies were locked");

            if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\A:"))
            { dwLen += sprintf(outbuf+dwLen,", lock of A: failed, see log file");
            }
            if (!WriteSD_ToA_File(psdAbsoluteSD,"\\\\.\\B:"))
            { dwLen += sprintf(outbuf+dwLen,", lock of B: failed, see log file");
            }

            bFloppiesAreLocked = TRUE;
            break;

          case 'Q':
            if (bFloppiesAreLocked)
            { sprintf(outbuf,"Floppy status is: Locked");
            }
            else
            { sprintf(outbuf,"Floppy status is: Unlocked");
            }
            break;

          default :
            sprintf(outbuf,"Bad operation passed in");
        }

        // send it back out...
        //
        ret = WriteFile(
                    pipeHandle,     // file to write to
                    outbuf,         // address of output buffer
                    sizeof(outbuf), // number of bytes to write
                    &bytesWritten,  // number of bytes written
                    NULL);          // overlapped stuff, not needed

        if (!ret)
            // pipe's broken... go back and reconnect
            //
            continue;

        // drop the connection...
        //
        DisconnectNamedPipe(pipeHandle);
    }
}



// utility functions...



// ReportStatusToSCMgr() --
//      This function is called by the ServMainFunc() and
//      ServCtrlHandler() functions to update the service's status
//      to the service control manager.
//
BOOL
ReportStatusToSCMgr(DWORD dwCurrentState,
                    DWORD dwWin32ExitCode,
                    DWORD dwCheckPoint,
                    DWORD dwWaitHint)
{
    BOOL fResult;

    // Disable control requests until the service is started.
    //
    if (dwCurrentState == SERVICE_START_PENDING)
        ssStatus.dwControlsAccepted = 0;
    else
        ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
            SERVICE_ACCEPT_PAUSE_CONTINUE;

    // These SERVICE_STATUS members are set from parameters.
    //
    ssStatus.dwCurrentState = dwCurrentState;
    ssStatus.dwWin32ExitCode = dwWin32ExitCode;
    ssStatus.dwCheckPoint = dwCheckPoint;

    ssStatus.dwWaitHint = dwWaitHint;

    // Report the status of the service to the service control manager.
    //
    if (!(fResult = SetServiceStatus(
                sshStatusHandle,    // service reference handle
                &ssStatus))) {      // SERVICE_STATUS structure

        // If an error occurs, stop the service.
        //
        StopSimpleService("SetServiceStatus");
    }
    return fResult;
}



// The StopSimpleService function can be used by any thread to report an
//  error, or stop the service.
//
VOID
StopSimpleService(LPTSTR lpszMsg)
{
    CHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[2];

    dwGlobalErr = GetLastError();

    // Use event logging to log the error.
    //
    hEventSource = RegisterEventSource(NULL,
                            TEXT("SimpleService"));

    sprintf(chMsg, "SimpleService error: %d", dwGlobalErr);
    lpszStrings[0] = chMsg;
    lpszStrings[1] = lpszMsg;

    if (hEventSource != NULL) {
        ReportEvent(hEventSource, // handle of event source
            EVENTLOG_ERROR_TYPE,  // event type
            0,                    // event category
            0,                    // event ID
            NULL,                 // current user's SID
            2,                    // strings in lpszStrings
            0,                    // no bytes of raw data
            lpszStrings,          // array of error strings
            NULL);                // no raw data

        (VOID) DeregisterEventSource(hEventSource);
    }

    // Set a termination event to stop SERVICE MAIN FUNCTION.
    //
    SetEvent(hServDoneEvent);
}

/****************************************************************************\
*
* FUNCTION: WriteSD_ToA_File
*
\****************************************************************************/

BOOL WriteSD_ToA_File(PSECURITY_DESCRIPTOR psdAbsoluteSD, LPTSTR lpszFileName)
{
  DWORD dwErrorMode;
  BOOL  bStatus;

  /**************************************************************************\
  *
  * SetErrorMode so we don't get the error due to no floppy disk in the floppy
  *   drive
  *
  \**************************************************************************/

  dwErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

  /**************************************************************************\
  *
  * Write SD to file system
  *
  \**************************************************************************/

  bStatus = SetFileSecurity(lpszFileName,
                            (SECURITY_INFORMATION)(DACL_SECURITY_INFORMATION),
                            psdAbsoluteSD);

  /**************************************************************************\
  *
  * SetErrorMode back to its previous value
  *
  \**************************************************************************/

  SetErrorMode(dwErrorMode);

  if (!bStatus)
  { if (ERROR_FILE_NOT_FOUND == GetLastError())
    { printf("\nAttempted to lock %s, but it was not found",lpszFileName);
    }
    else
    { PERR("SetFileSecurity");
      return(FALSE);
    }
  }

  return(TRUE);
}
