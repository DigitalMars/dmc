
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/***************************************************************************\
*
*    PROGRAM: EXPDIR.C
*
*    PURPOSE: Directory Listbox expansion/collapse functions
*
\***************************************************************************/
#define  STRICT
#include <windows.h>
#include <limits.h>
#include "globals.h"
#include "filer.h"
#include "drvproc.h"
#include "expdir.h"

extern HANDLE ghModule;

/***************************************************************************\
*
* ExpDir()
*
* Thread Function
* Called when directory is selected (Enter or Double-click).  Expands subdirs
*  of selected directory if not expanded, indenting subdirs with a '|'.  If
*  already expaneded, deleteds all subdirs from listbox
*
* Returns:  TRUE if successful, FALSE if error.
*
* History:
* 5/11/93
*   Created.
*
\***************************************************************************/
BOOL ExpDir(LPCINFO lpCInfo)
{
    HANDLE          hFile = NULL;   //  Find file handle
    WIN32_FIND_DATA FileData;       //  Find file info structure

    TCHAR           szFileName[DIRECTORY_STRING_SIZE << 1];   // file name buffer
    PTCHAR          lpszHold;      //  Points to actual name in LB string

    BOOL            fDone = FALSE;  //  Loop flag for finding files in dir

    DWORD           dwAttrib;       //  Holds file attribute flags

    LONG            lIndex,         //  Index of selected listbox member
                    lDirDepth;


    if( WaitForSingleObject( lpCInfo->hDirMutex, MUTEX_TIMEOUT)
            == WAIT_TIMEOUT ){
        ErrorMsg(TEXT("ExpDir: Dir LB Mutex Timeout."));
        return(0);
    }

    //
    // If the LB is empty, we insert the root, unexpanded
    //
    if( !SendMessage( lpCInfo->hDirLB, LB_GETCOUNT, 0, 0) ){

        TCHAR   szHold[DIRECTORY_STRING_SIZE << 1];

        szHold[0] = TEXT('+');
        lstrcpy( &(szHold[1]), lpCInfo->lpDriveInfo->DriveName );

        //
        // Also copy the DriveName to the Caption Bar.
        //
        lstrcpy(lpCInfo->CaptionBarText, lpCInfo->lpDriveInfo->DriveName );

        SendMessage(lpCInfo->hDirLB, LB_ADDSTRING, 0,
                    (LPARAM)szHold );

        ReleaseMutex( lpCInfo->hDirMutex );
        return(1);
    }

    //
    // Retrieve index of selected (careted) directory.
    //
    lIndex = SendMessage( lpCInfo->hDirLB, LB_GETCARETINDEX,
                          (WPARAM)NULL, (LPARAM)NULL );

    if( SendMessage( lpCInfo->hDirLB, LB_GETTEXT, (WPARAM)lIndex,
                        (LPARAM)szFileName ) < 0 ){
        ErrorMsg(TEXT("Expand Directory:  Get listbox text failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    lDirDepth = GetDirDepth( szFileName, &lpszHold );

    if( lDirDepth == -1 ){
        ErrorMsg(TEXT(" Expand Directory:  GetDirDepth failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    //
    // If Directory is already expanded, collapse it, and vice versa.
    //  First, change the symbol.
    //
    if( *lpszHold == TEXT('-'))
        *lpszHold = TEXT('+');
    else
        *lpszHold = TEXT('-');

    //
    // Clear WM_SETREDRAW flag, so changes will not be seen until entire
    //  expansion/collapse is complete.  Reset flag at end of function
    //
    if( SendMessage(lpCInfo->hDirLB, WM_SETREDRAW, (WPARAM)FALSE,
                (LPARAM)0 ) < 0){
        ErrorMsg(TEXT("Expand Directory:  Clear redraw flag failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    //
    // Delete old dir string, insert new, and reset the selection
    //
    if( SendMessage(lpCInfo->hDirLB, LB_DELETESTRING, (WPARAM)lIndex,
                (LPARAM)0 ) < 0){
        ErrorMsg(TEXT("Expand Directory:  Delete dir string failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }
    if( SendMessage(lpCInfo->hDirLB, LB_INSERTSTRING, (WPARAM)lIndex,
               (LPARAM)szFileName ) < 0){
        ErrorMsg(TEXT("Expand Directory:  Insert dir string failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }
    if( SendMessage(lpCInfo->hDirLB, LB_SETCURSEL, (WPARAM)lIndex,
               (LPARAM)0 ) < 0){
        ErrorMsg(TEXT("Expand Directory:  Insert dir string failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    //
    // The symbol has been changed, now collapse if needed, then reset
    //  redraw flag, leave critical section, and exit.
    //
    if( *lpszHold == TEXT('+') ){
        CollapseDir( lpCInfo, lIndex, lDirDepth);
        ExpDirExit(lpCInfo, hFile);
        return(1);
    }

    //
    // If we're here, directory needs to be expanded.
    //  Enumerate subdirectories beneath the dir entry in the listbox.
    //
    // First, Get the full path of the directory (in szFileName)
    //
    if( !ConstructDirName(lpCInfo, lIndex, szFileName) ){
        ErrorMsg(TEXT("Expand Directory:  ConstructDirName failure"));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    //
    // Check to see if there is a terminating backslash
    //  Then append a '*' as a wildcard for FindFirstFile.
    //
    lpszHold = TStrChr(szFileName, TEXT('\0'));

    lpszHold--;
    if( *lpszHold != TEXT('\\') ){
        lpszHold++;
        *lpszHold = TEXT('\\');
    }

    lpszHold++;
    lstrcpy( lpszHold, TEXT("*"));

    //
    // Start a search on all the files within the directory
    //
    hFile = FindFirstFile( szFileName, &FileData );
    if( hFile == INVALID_HANDLE_VALUE ){
        ErrorMsg(TEXT("Expand Directory: FindFirstFile failure."));
        ExpDirExit(lpCInfo, hFile);
        return(0);
    }

    //
    // Walk all the files in the directory.
    //
    while( !fDone ){

        //
        // Check to see if the thread has been requested to kill itself.
        //  This code does not clear the suicide flag for synchronization
        //  reasons - it is left to the calling code.
        //
        if( lpCInfo->fSuicide ){
            ErrorMsg(TEXT("Expand Directory: killing thread per request."));
            ExpDirExit(lpCInfo, hFile);

            //
            // Post an MM_REFRESH Message if the user has re-collapsed the
            //  Dir LB.
            //
            if( !lpCInfo->fDirExpand )
                if( !PostMessage(lpCInfo->hwnd, WM_COMMAND, MM_REFRESH,
                                 (LPARAM)0) )
                    ErrorMsg(TEXT("ExpDir: MM_REFRESH call failure."));

            //
            // We must return failure (0) here, so if FullExpand is calling,
            //  it will terminate.
            //
            return(0);
        }

        //
        // Append filename to path, and get attributes
        //
        lstrcpy(lpszHold, FileData.cFileName);
        dwAttrib = GetFileAttributes( szFileName );

        //
        // Check if file is a directory.  If not, or if '.' or '..', fall
        //  through.  If so, add it to the directory listbox.
        //
        if( dwAttrib & FILE_ATTRIBUTE_DIRECTORY )
            if( lstrcmp( FileData.cFileName,TEXT(".") ) )
                if( lstrcmp( FileData.cFileName,TEXT("..") ) ){
                    TCHAR   szLBEntry[DIRECTORY_STRING_SIZE << 1];

                    ConstructLBEntry(lDirDepth, FileData.cFileName, szLBEntry);
                    //
                    //  Increment index in order to add subdir after
                    //  dirs just inserted.
                    //
                    if( SendMessage(lpCInfo->hDirLB, LB_INSERTSTRING,
                                       (WPARAM)++lIndex,
                                       (LPARAM)szLBEntry) < 0){
                        ErrorMsg(TEXT("Expand Directory: error inserting string."));
                        ExpDirExit(lpCInfo, hFile);
                        return(0);
                    }
                }

        fDone = !FindNextFile( hFile, &FileData );
    }

    ExpDirExit(lpCInfo, hFile);
    return(1);
}


/***************************************************************************\
*
* ExpDirExit()
*
* Performs clean-up operations for ExpDir, closing handles, etc.
*
* History:
* 5/28/93
*   Created.
*
\***************************************************************************/
void ExpDirExit(LPCINFO lpCInfo, HANDLE hFile)
{

    //
    // Reset redraw flag. Post this message, to avoid synchro problems
    //
    if( PostMessage(lpCInfo->hDirLB, WM_SETREDRAW, (WPARAM)TRUE,
                (LPARAM)0 ) < 0)
        ErrorMsg(TEXT("Expand Directory:  Clear redraw flag failure"));

    //
    // Close FindFirstFile session
    //
    if( hFile != NULL )
        FindClose(hFile);

    //
    // Release Dir LB Mutex
    //
    ReleaseMutex( lpCInfo->hDirMutex);
}


/***************************************************************************\
*
* ConstructDirName()
*
* Builds the fully qualified path of the current directory, by walking back
*  through the Dir LB tree.
*
* Returns:  TRUE if successful, FALSE if error.
*           Returns the full directory path of given directory name in
*             lpszDirName.
*
* History:
* 5/11/93
*   Created.
*
\***************************************************************************/
BOOL ConstructDirName(LPCINFO lpCInfo, LONG lIndex, LPTSTR lpszDirName)
{
    LONG    lDirDepth,      //  Depth of selected directory
            lSeekDepth = LONG_MAX;

    TCHAR   szFileName[DIRECTORY_STRING_SIZE << 1];   // file buffer
    LPTSTR  lpszInfoPtr,
            lpszHold;

    //
    // Clear the directory name buffer
    //
    *lpszDirName = TEXT('\0');

    //
    // Walk up the entries in the listbox, constructing the full path from
    //   the bottom up.
    //
    while( lIndex >= 0 ){

        //
        // Get listbox text, and compute the depth of the directory
        //
        if( SendMessage( lpCInfo->hDirLB, LB_GETTEXT, (WPARAM)lIndex,
                            (LPARAM)szFileName ) < 0 ){
            ErrorMsg(TEXT(" Expand Directory:  Get listbox text failure"));
            return(0);
        }

        lDirDepth = GetDirDepth(szFileName, &lpszInfoPtr);
        if( lDirDepth == -1){
            ErrorMsg(TEXT("ConstructDirName: GetDirDepth failed"));
            return(0);
        }

        //
        // If we've reached the next level up, add to the directory name
        //
        if( lDirDepth < lSeekDepth ){
            lSeekDepth = lDirDepth;

            // check if we will exceed the size of our buffer
            if( lstrlen(lpszInfoPtr) + lstrlen(lpszDirName) >
                    (DIRECTORY_STRING_SIZE << 1) ){
                ErrorMsg(TEXT("ConstructDirName:  Exceeded Directory Size limit"));
                return(0);
            }

            // Find the end of the directory name
            lpszHold = TStrChr(lpszInfoPtr, TEXT('\0'));

            // If we're not at the root, add a '\'
            if( lIndex && (*lpszDirName != TEXT('\0')) )
                *lpszHold++ = TEXT('\\');

            // Append the heretofore computed path to the end of the dir name
            lstrcpy( lpszHold, lpszDirName);
            // Copy the whole path so far back into the final buffer
            lstrcpy( lpszDirName, ++lpszInfoPtr);
        }

        // If the first level dir has been added, jump to the root,
        //   else go up to the previous entry in the listbox.
        if( lDirDepth == 1 )
            lIndex = 0;
        else
            lIndex--;
    }

    return(TRUE);
}


/***************************************************************************\
*
* GetDirDepth()
*
* Returns:  -1 if error
*           otherwise, the depth of the directory (i.e. root is depth 0,
*           c:\foo is depth 1, etc.  This is computed in perhaps not the
*           most efficient way, by counting the '|'s proceeding the name.
*
*           This function also returns in lpszDirName a pointer to the
*           end of the preceeding '|' characters within the given listbox str.
*           If the function fails, this pointer value is undefined.
*
* The left shifted index is to skip the tab characters. See ConstructLBEntry().
*
* History:
* 5/7/93
*   Created.
*
\***************************************************************************/
LONG GetDirDepth(LPTSTR lpszLBString, LPTSTR *lpszDirName)
{
    TCHAR cBar;
    LONG  lCount = 0;

    do{

        cBar = lpszLBString[lCount << 1];

        if( cBar == TEXT('|') )
            lCount++;
        else
            if( cBar != TEXT('+') && cBar != TEXT('-') ){
                ErrorMsg(TEXT("GetDirDepth: string parse error"));
                return(-1);
            }

    }while( cBar != TEXT('+') && cBar != TEXT('-') );

    *lpszDirName = &(lpszLBString[lCount << 1]);

    return( lCount );
}


/***************************************************************************\
*
* CollapseDir()
*
* If directory is expanded, collapses it, by deleteing any subdirectory
*  entries below it.
*
* Returns:  TRUE if successful, FALSE if error
*
* History:
* 5/11/93
*   Created.
*
\***************************************************************************/
BOOL CollapseDir(LPCINFO lpCInfo, LONG lIndex, LONG lDirDepth)
{
    TCHAR   szFileName[DIRECTORY_STRING_SIZE << 1];   // file name buffer
    LPTSTR  lpszNamePtr;

    LONG    lDepthHold;


    //
    // Remove any following LB entries until we return to same depth
    //
    do{

        if( SendMessage( lpCInfo->hDirLB, LB_GETTEXT, (WPARAM)lIndex + 1,
                            (LPARAM)szFileName ) < 0 ){
            ErrorMsg(TEXT(" Expand Directory:  Get listbox text failure"));
            return(0);
        }

        lDepthHold = GetDirDepth( szFileName, &lpszNamePtr);
        if( lDirDepth == -1 ){
            ErrorMsg(TEXT(" Expand Directory:  GetDirDepth failure"));
            return(0);
        }

        if( lDirDepth < lDepthHold )
            if( SendMessage( lpCInfo->hDirLB, LB_DELETESTRING, (WPARAM)lIndex + 1,
                                (LPARAM)0 ) < 0 ){
                ErrorMsg(TEXT(" Expand Directory:  Delete String failure"));
                return(0);
            }

    }while( lDirDepth < lDepthHold );

    return(1);
}


/***************************************************************************\
*
* ConstructLBEntry()
*
* Given the parent's directory depth, and the subdirectory name, inserts
* '|'s equal to the depth+1 of the parent (plus tab character), and an
* unexpanded '+' directory marker, then the name of the subdirectory.
*
* The left shifted index is to skip the tab characters.
*
* Returns:  void.  the completed listbox entry is returned in szLBEntry.
*
* History:
* 5/14/93
*   Created.
*
\***************************************************************************/
void ConstructLBEntry(LONG lDirDepth, LPTSTR szFileName, LPTSTR szLBEntry)
{
    int i;

    for(i = 0; i <= lDirDepth; i++){
        szLBEntry[i << 1] = TEXT('|');
        szLBEntry[(i << 1) + 1] = 9;
    }

    szLBEntry[i << 1] = TEXT('+');
    szLBEntry[(i << 1) + 1] = TEXT('\0');

    lstrcat(szLBEntry, szFileName);
}


/***************************************************************************\
*
* FullExpand()
*
* From the unexpanded root, walks down directory LB, expanding each directory
*  until it reaches the end of the tree.
*
* Returns:  void
*
* History:
* 5/20/93
*   Created.
*
\***************************************************************************/
BOOL FullExpand(LPCINFO lpCInfo)
{
    LONG lIndex = 0;

    if( WaitForSingleObject( lpCInfo->hDirMutex, MUTEX_TIMEOUT)
            == WAIT_TIMEOUT ){
        ErrorMsg(TEXT("FullExpand: Dir LB Mutex Timeout."));
        return(0);
    }

    while( SendMessage(lpCInfo->hDirLB, LB_SETCURSEL,
                        (WPARAM)lIndex,
                        (LPARAM)0) != LB_ERR ){
        if( !ExpDir( lpCInfo ) ){
            ErrorMsg(TEXT("Full Expand: ExpDir failure."));
            ReleaseMutex( lpCInfo->hDirMutex );

            // This is in case the ExpDir failed because a change drive
            //  command caused a kill.
            if( !PostMessage(lpCInfo->hwnd, WM_COMMAND, MM_REFRESH,
                             (LPARAM)0) )
                ErrorMsg(TEXT("ExpDir: MM_REFRESH call failure."));
            return(0);
        }

        lIndex++;
    }

    //  Set selection in listboxes to first item.
    if( SendMessage(lpCInfo->hDirLB, LB_SETCURSEL,
                        (WPARAM)0,
                        (LPARAM)0) == LB_ERR ){
        ErrorMsg(TEXT("Full Expand: Dir LB Set Selection Error"));
        ReleaseMutex( lpCInfo->hDirMutex );
        return(0);
    }

    ReleaseMutex( lpCInfo->hDirMutex );
    return(1);
}
