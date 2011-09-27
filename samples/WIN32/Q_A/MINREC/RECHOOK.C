// ************************************************************************
//
//                      Microsoft Developer Support
//               Copyright (c) 1992-1995 Microsoft Corporation
//
// ************************************************************************
// MODULE    : RecHook.C
// PURPOSE   : A Small Win32 Recorder-like Sample Application Hook DLL
// FUNCTIONS :
//   DllMain()            - Dll entry point (via _DllMainCRTStartup)
//   StartJournalRecord   - starts the journal record hook
//   StopJournalRecord    - stops the journal record hook
//   StartJournalPlayback - starts the journal playback hook
//   StopJournalPlayback  - stops the journal playback hook
//   StopAllJournalling   - stops any current journal hook
// COMMENTS  : The Journal Record File contains an EVENTHEADER structure
//             followed by a variable number of EVENTMSG structures.
// ************************************************************************
#define   STRICT               // strict type checking enabled

// If this program is going to be run exclusively on Windows NT enable
// the UNICODE define below.  However, Windows 95 doesn't support UNICODE
// so for Windows 95 leave the define commented out.

// #define   UNICODE              // make the application unicode compliant
#include <Windows.H>           // required for all Windows applications

#include "RecHook.H"
#include "MinRec.H"            // we need IDM_MACRO_STOP

// Internal Defines
// -----------------------------------------------------------------------
#define MINREC_FILENAME  TEXT("MinRec.Rec") // File for record/play events
#define MINREC_SIGNATURE "MinRec\0"         // Journal Record File signature

//-- header for the MinRec record/playback file
typedef struct EVENTHEADER_STRUCT {
  CHAR   Signature[7];
  UINT   RecordedMsgCount;
  DWORD  BaseMsgTime;
} EVENTHEADER, *PEVENTHEADER;

//-- various "file global" data
typedef struct GLOBAL_STRUCT {
  BOOL      fRecording;
  BOOL      fPlaying;
  BOOL      fStopRecording;
  BOOL      fStopPlaying;
  UINT      PlayedMsgCount;
  UINT      RecordedMsgCount;
  DWORD     BaseMsgTime;
  DWORD     LastMsgTime;
  HANDLE    hFile;
  EVENTMSG  EventMsg;
  HHOOK     hHookRecord;
  HHOOK     hHookPlayback;
  HHOOK     hHookGetMsg;
  HINSTANCE hInstance;
} GLOBAL, *PGLOBAL;

// Global Data (file local)
// -----------------------------------------------------------------------
static GLOBAL  Global;                // various global data

// Internal Function Prototypes
// -----------------------------------------------------------------------
LRESULT CALLBACK JournalRecordProc  ( int, WPARAM, LPARAM );
LRESULT CALLBACK JournalPlaybackProc( int, WPARAM, LPARAM );
LRESULT CALLBACK GetMsgProc         ( int, WPARAM, LPARAM );


// ************************************************************************
// FUNCTION : DllMain( HINSTANCE, DWORD, LPVOID )
// PURPOSE  : DllMain is called by the C Run-Time library from the
//            _DllMainCRTStartup entry point.  The DLL entry point gets
//            called (entered) on the following events: "Process Attach",
//            "Thread Attach", "Thread Detach" or "Process Detach".
// ************************************************************************
BOOL WINAPI
DllMain( HINSTANCE hInstanceDll, DWORD dwReason, LPVOID lpvReserved )
{
  switch( dwReason ) {

    case DLL_PROCESS_ATTACH:
      Global.fRecording    = FALSE;
      Global.fPlaying      = FALSE;
      Global.hInstance     = hInstanceDll;
      break;

  }

  return( TRUE );
  UNREFERENCED_PARAMETER( lpvReserved );
}


// ************************************************************************
// FUNCTION : StartJournalRecord()
// PURPOSE  : Creates the record file and installs the record hook.
// ************************************************************************
BOOL
StartJournalRecord()
{
  // Do not allow recording while playing back
  if( Global.fPlaying ) {
    MessageBox( NULL,
      TEXT( "Sorry, recording while\n" )
      TEXT( "playing is not allowed."  ),
      TEXT( "MinRec - Notice!"         ),
      MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND );
    return( FALSE );
  }

  // Create and open the journal record file and set the file pointer to
  //  just past the journal record file header
  Global.hFile = CreateFile( (LPCTSTR) MINREC_FILENAME, GENERIC_WRITE,
                       0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
  SetFilePointer( Global.hFile, (LONG) sizeof(EVENTHEADER), NULL, FILE_BEGIN );

  // Initialize the recorded message count to zero and set recording flag
  Global.RecordedMsgCount = 0;
  Global.fRecording       = TRUE;

  // Set the GetMsg hook to trap Ctrl+Esc, Alt+Esc and Ctrl+Alt+Del.
  Global.hHookGetMsg = SetWindowsHookEx( WH_GETMESSAGE, GetMsgProc,
                         Global.hInstance, 0 );

  // Set the Journal Record hook.
  Global.hHookRecord = SetWindowsHookEx( WH_JOURNALRECORD, JournalRecordProc,
                         Global.hInstance, 0 );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : JournalRecordProc()
// PURPOSE  : Processes the journal record events by writing them to  the
//            record file one event at a time.
// ************************************************************************
LRESULT CALLBACK
JournalRecordProc( int nCode, WPARAM wParam, LPARAM lParam )
{
  // if nCode < 0 or is set Global.fStopRecording we must immediately return
  // the result of CallNextHookEx.
  if( nCode < 0 || Global.fStopRecording )
    return( CallNextHookEx( Global.hHookRecord, nCode, wParam, lParam ) );

  switch( nCode ) {

    // The lParam parameter points to an EVENTMSG structure
    // containing information about a message removed from the system
    // queue.  The hook procedure must record the contents of the structure
    // by copying them to the Recorded Journal file.
    case HC_ACTION: {
      LPEVENTMSG lpEventMsg;
      DWORD      dwBytesWritten;

      lpEventMsg = (LPEVENTMSG) lParam;

      // user pressed Ctrl+Break.  We use this as a request to cancel
      // journal recording.  We wont display a message to the user here
      // since this is an action the user choose to do.
      if( (lpEventMsg->message == WM_KEYDOWN)
           && (LOBYTE(lpEventMsg->paramL) == VK_CANCEL) ) {
        StopAllJournalling();
      }
      else {
        // write the event message to the Recorded Journal file
        WriteFile( Global.hFile, (LPCSTR) lParam, sizeof(EVENTMSG),
          &dwBytesWritten, NULL );
        if( ++Global.RecordedMsgCount == 1)
          Global.BaseMsgTime = lpEventMsg->time;
      }
      break;
    }

    // A system-modal dialog box is being displayed. Until the dialog box
    // is destroyed, the hook procedure must stop recording.
    case HC_SYSMODALON:
      Global.fStopRecording = TRUE;
      break;

    // A system-modal dialog box has been destroyed. The hook
    // procedure must resume recording.
    case HC_SYSMODALOFF:
      Global.fStopRecording = FALSE;
      break;

  }

  return( CallNextHookEx( Global.hHookRecord, nCode, wParam, lParam ) );
  UNREFERENCED_PARAMETER( wParam );
}


// ************************************************************************
// FUNCTION : StopJournalRecord()
// PURPOSE  : Unhooks the record hook, records the record file header and
//            closes the file.
// ************************************************************************
BOOL
StopJournalRecord()
{
  EVENTHEADER EventHeader;

  // Remove the Journal Record hook if recording
  if( Global.fRecording ) {
    DWORD dwBytesWritten;

    // remove the record hook
    UnhookWindowsHookEx( Global.hHookRecord );

    // remove the get message hook
    UnhookWindowsHookEx( Global.hHookGetMsg );

    // Copy Recorded Journal header data to temporary buffer
    lstrcpyA( EventHeader.Signature, MINREC_SIGNATURE );
    EventHeader.RecordedMsgCount = Global.RecordedMsgCount;
    EventHeader.BaseMsgTime      = Global.BaseMsgTime;

    // Open Recorded Journal file and update the file header and close
    //  the file
    SetFilePointer( Global.hFile, 0, NULL, FILE_BEGIN );
    WriteFile( Global.hFile, (LPCSTR) &EventHeader, sizeof(EVENTHEADER),
      &dwBytesWritten, NULL );
    CloseHandle( Global.hFile );

    // Clear recording flag
    Global.fRecording = FALSE;
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : StartJournalPlayback()
// PURPOSE  : Opens the playback file and reads the header and installs the
//            playback hook.
// ************************************************************************
BOOL
StartJournalPlayback()
{
  EVENTHEADER EventHeader;
  DWORD       dwBytesRead;

  // Allow infinite playback loop
  if( Global.fRecording ) {
    StopJournalRecord();
    MessageBox( NULL,
      TEXT( "Infinite playback\n" )
      TEXT( "loop recorded!"      ),
      TEXT( "MinRec - Notice!"    ),
      MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND );
    return( FALSE );
  }

  if( Global.fPlaying ) {
    StopJournalPlayback();
  }

  // Open Recorded Journal file
  Global.hFile = CreateFile( (LPCTSTR) MINREC_FILENAME, GENERIC_READ,
                       0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
  if( Global.hFile == INVALID_HANDLE_VALUE ) {
    MessageBox( NULL,
      TEXT( "The default Journal Record File\n" )
      TEXT( "MINREC.REC could not be found.\n"  ),
      TEXT( "MinRec - Error!"                   ),
      MB_ICONEXCLAMATION | MB_OK | MB_SETFOREGROUND );
    return( FALSE );
  }

  // Quick check, file size should be at least the length of the header
  if( GetFileSize( Global.hFile, NULL ) < sizeof(EVENTHEADER) ) {
    MessageBox( NULL,
      TEXT( "The file MINREC.REC is not a \n"     )
      TEXT( "valid MinRec Journal Record file.\n" ),
      TEXT( "MinRec - Error!"                     ),
      MB_ICONEXCLAMATION | MB_OK | MB_SETFOREGROUND );
    return( FALSE );
  }

  // Read Recorded Journal file header
  SetFilePointer( Global.hFile, 0L, NULL, FILE_BEGIN );
  ReadFile( Global.hFile, (LPVOID) &EventHeader, sizeof(EVENTHEADER),
        &dwBytesRead, NULL );

  // Quick check, the signature of the file must be "MinRec"
  if( lstrcmpA( EventHeader.Signature, MINREC_SIGNATURE ) != 0 ) {
    MessageBox( NULL,
      TEXT( "The file MINREC.REC does not\n"    )
      TEXT( "contain a valid MinRec signature." ),
      TEXT( "MinRec - Error!"                   ),
      MB_ICONEXCLAMATION | MB_OK | MB_SETFOREGROUND );
     return( FALSE );
  }

  // Store the header values
  Global.RecordedMsgCount = EventHeader.RecordedMsgCount;
  Global.BaseMsgTime      = EventHeader.BaseMsgTime;
  Global.LastMsgTime      = EventHeader.BaseMsgTime;

  // If no messages were recorded, none to play back.
  if( Global.RecordedMsgCount == 0 ) {
    MessageBox( NULL,
      TEXT( "Sorry, nothing was recorded, so\n")
      TEXT( "there is nothing to playback."    ),
      TEXT( "MinRec - Notice!"                 ),
      MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND );
     return( FALSE );
  }

  // Initialize the played message count to zero and set the play flag
  Global.PlayedMsgCount = 0;
  Global.fPlaying       = TRUE;

  // Get the first recorded message from the file and store it
  ReadFile( Global.hFile, (LPVOID) &(Global.EventMsg), sizeof(EVENTMSG),
        &dwBytesRead, NULL );

  // Set the GetMsg hook to trap Ctrl+Break, Ctrl+Esc and Ctrl+Alt+Del.
  Global.hHookGetMsg = SetWindowsHookEx( WH_GETMESSAGE, GetMsgProc,
                         Global.hInstance, 0 );

  // Set the Journal Playback hook
  Global.hHookPlayback = SetWindowsHookEx( WH_JOURNALPLAYBACK,
                           JournalPlaybackProc, Global.hInstance, 0 );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : JournalPlaybackProc()
// PURPOSE  : Processes the journal playback events by reading in the
//            playback file one event at a time.
// ************************************************************************
LRESULT CALLBACK
JournalPlaybackProc( int nCode, WPARAM wParam, LPARAM lParam )
{
  // if nCode < 0 or is set Global.fStopPlaying we must immediately return
  // the result of CallNextHookEx.
  if( nCode < 0 || Global.fStopPlaying )
    return( CallNextHookEx( Global.hHookPlayback, nCode, wParam, lParam ) );

  switch( nCode ) {

    // HC_GETNEXT occurs when Windows requests the next system message
    // queue message.  If we return 0 it will place the message found in
    // lParam into the system message queue otherwise it will sleep
    // for the specified number of milliseconds returned and then later
    // call JournalPlaybackProc with nCode set, again, to HC_GETNEXT.
    case HC_GETNEXT: {
      LRESULT SleepTime;

      // determine the amount of time Windows must sleep before
      // it should place this message into the system message queue.
      SleepTime = Global.EventMsg.time - Global.LastMsgTime;

      // copy the current ready system message to lParam
      CopyMemory( (PVOID) lParam, (CONST VOID*) &(Global.EventMsg),
        sizeof(EVENTMSG) );

      // store the value of the next system message post time so we can
      // use this value to set SleepTime to 0 on the next HC_GETNEXT
      Global.LastMsgTime = Global.EventMsg.time;

      return( SleepTime );
    }

    // HC_SKIP occurs only after Windows has processed a HC_GETNEXT
    // condition that returned 0.  We can use this to prepare the next
    // ready system message to be posted or deterimine of there are no
    // remaining system messages to post and thus stop the playback.
    case HC_SKIP: {
      if( ++Global.PlayedMsgCount > Global.RecordedMsgCount ) {
        StopJournalPlayback();
        break;
        // return( (LRESULT) 0L );
      }
      else {
        DWORD dwBytesRead;

        // Get the next ready message from the file and store it
        // in the global buffer for the next HC_GETNEXT
        ReadFile( Global.hFile, (LPVOID) &(Global.EventMsg), sizeof(EVENTMSG),
          &dwBytesRead, NULL );
        // fall through and call CallNextHookEx
        // return( (LRESULT) 0L );
        break;

      }
    }

    // A system-modal dialog box is being displayed. Until the dialog box
    // is destroyed, the hook procedure must stop playing back messages.
    case HC_SYSMODALON:
      Global.fStopPlaying = TRUE;
      break;

    // A system-modal dialog box has been destroyed. The hook
    // procedure must resume playing back the messages.
    case HC_SYSMODALOFF:
      Global.fStopPlaying = FALSE;
      break;

  }

  return( CallNextHookEx( Global.hHookPlayback, nCode, wParam, lParam ) );
  UNREFERENCED_PARAMETER( wParam );
}


// ************************************************************************
// FUNCTION : StopJournalPlayback()
// PURPOSE  : Unhooks the playback hook and closes the playback file.
// ************************************************************************
BOOL
StopJournalPlayback()
{
  // Remove the Playback Journal hook if playing
  if( Global.fPlaying ) {

    // remove the playback hook
    UnhookWindowsHookEx( Global.hHookPlayback );

    // remove the get message hook
    UnhookWindowsHookEx( Global.hHookGetMsg );

    // close the journal record file
    CloseHandle( Global.hFile );

    // Clear playback flag
    Global.fPlaying = FALSE;
  }

  return( TRUE );
}


// ************************************************************************
// FUNCTION : StopAllJournalling()
// PURPOSE  : Stops (unhooks) any current journal hook started (hooked) by
//            this application.
// ************************************************************************
BOOL
StopAllJournalling( )
{
  if( Global.fRecording )
    StopJournalRecord();
  if( Global.fPlaying )
    StopJournalPlayback();

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetMsgProc()
// PURPOSE  : Installs a get message hook to monitor messages sent to the
//            queue so we can trap the WM_CANCELJOURNAL message send by the
//            system when the user presses Ctrl+Esc, Alt+Esc or
//            Ctrl+Alt+Del which cancels the journal hook.
// COMMENTS : WM_CANCELJOURNAL gets sent to the thread which installs the
//            hook but the hWnd of this message is NULL and thus does not
//            get dispatched.  Thus this hook is used to retrieve this
//            message.
// ************************************************************************
LRESULT CALLBACK
GetMsgProc( int nCode, WPARAM wParam, LPARAM lParam )
{
  // if nCode < 0 we must immediately return the result of CallNextHookEx.
  if( nCode < 0  )
    return( CallNextHookEx( Global.hHookGetMsg, nCode, wParam, lParam ) );

  switch( ((LPMSG)lParam)->message ) {

    // user pressed Ctrl+Esc, Alt+Esc or Ctrl+Alt+Del, the system will
    // automatically stop all journalling.  Thus we display a message that
    // this has resulted in the termination of the Jorunalling since the
    // user may not expect this to occur with these key events.
    case WM_CANCELJOURNAL:
      StopAllJournalling();
      MessageBox( NULL,
        TEXT( "The user has pressed either\n"            )
        TEXT( "Ctrl+Esc, Alt+Esc or Ctrl+Alt+Del\n"      )
        TEXT( "and thus the system has automatically\n"  )
        TEXT( "canceled all journalling.\n"              ),
        TEXT( "MinRec - Notice!"                         ),
        MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND );
      break;

  }

  return( CallNextHookEx( Global.hHookGetMsg, nCode, wParam, lParam ) );
  UNREFERENCED_PARAMETER( wParam );
}
