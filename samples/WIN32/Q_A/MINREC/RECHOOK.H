// ************************************************************************
//
//                      Microsoft Developer Support
//               Copyright (c) 1992-1995 Microsoft Corporation
//
// ************************************************************************
// PROGRAM   : RecHook.H
// PURPOSE   : A Small Win32 Recorder-like Sample Application Hook DLL
//             Header File
// COMMENTS  :
// ************************************************************************
#ifndef RECHOOK_H

 #define RECHOOK_H

 // Global Defines
 // -----------------------------------------------------------------------

 // Public Function Prototypes
 // -----------------------------------------------------------------------
 BOOL StartJournalRecord( VOID );
 BOOL StopJournalRecord( VOID );
 BOOL StartJournalPlayback( VOID );
 BOOL StopJournalPlayback( VOID );
 BOOL StopAllJournalling( VOID );

#endif // RECHOOK_H
