// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   ini.c
//
//  PURPOSE:  Fills a TreeView control with a listing of the INI
//            files. The root items are the INI files, then children
//            off the roots are the sections, and the children off
//            the sections are the entries:
//
//            ROOT WIN.INI
//                   |
//                   --[Fonts]
//                     |
//                     --AlbertusBold
//            ROOT SYSTEM.INI
//    
//
//  FUNCTIONS:
//
//    INI_FillTreeView - Entry point function to fill TreeView Control
//    FillIniKeys      - Build a list of sections for each INI file
//    FillKeyItems     - Build a list of entries for each section
//    
//  COMMENTS:
//
//    The INI_FillTreView function is called. It adds the INI file
//    names to the treeview. It calls FillIniKeys for each INI file. 
//    The FillIniKeys function adds the sections for the passed in
//    INI filename. The FillIniKeys calls FillKeyItems for each section.
//    The FillKeyItems function adds the entries for the passed in
//    section.

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // TreeView declarations live here
#include "globals.h"            // prototypes specific to this application

#define MAX_SECTION_SIZE      32764
#define MAX_SECTION_KEY_SIZE  256
#define MAX_ENTRY_SIZE        256
#define MAX_STRING_SIZE       256
#define MAX_INIFILE_SIZE      65536

//
//  FUNCTION PROTOTYPES
//

void INI_FillTreeView(HWND        hwndTreeView, 
                      int         iINIFileImage,
                      int         iSectionKeyImage,
                      int         iSelectedEntryImage,
                      int         iNonSelectedEntryImage);

void FillIniKeys(HWND hwndTreeView, 
                 HTREEITEM hParent, 
                 LPSTR szIniFileName,
                 int         iSectionKeyImage,
                 int         iSelectedEntryImage,
                 int         iNonSelectedEntryImage);

void FillKeyItems(HWND        hwndTreeView, 
                  HTREEITEM   hParent, 
                  LPSTR       szIniFileName, 
                  LPSTR       szSection,
                  int         iSelectedEntryImage,
                  int         iNonSelectedEntryImage);

//
//  FUNCTION: INI_FillTreeView(HWND        hwndTreeView, 
//                             int         iINIFileImage,
//                             int         iSectionKeyImage,
//                             int         iSelectedEntryImage,
//                             int         iNonSelectedEntryImage)
//
//
//  PURPOSE: Fills the TreeView control with the INI files,
//           call helper functions to fill in more detailed
//           information.
//
//  PARAMETERS:
//    hwndTreeView           - Handle fo TreeView Control
//    iINIFileImage          - ImageList ID for "INI" item.
//    iSectionKeyImage       - ImageList ID for "Folder" item.
//    iSelectedEntryImage    - ImageList ID for selected entry.
//    iNonSelectedEntryImage - ImageList ID for non-selected entry.
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
//    This function walks the Windows directory for INI files.
//    For each file found, a root entry is added to the TreeView,
//    and then the FillIniKeys helper function is called to
//    fill in the sections for the INI file.
//

void INI_FillTreeView(HWND        hwndTreeView, 
                      int         iINIFileImage,
                      int         iSectionKeyImage,
                      int         iSelectedEntryImage,
                      int         iNonSelectedEntryImage)
{
    char             szWindowsDirectory[MAX_PATH]; // Windows Directory.
    WIN32_FIND_DATA  FindFile;                     // For file searches.
    HANDLE           hFindFile;                    // For file searches.
    TV_ITEM          tvi;                          // TreeView Item.
    TV_INSERTSTRUCT  tvins;                        // TreeView Insert Struct.
    HTREEITEM        hPrev = NULL;                 // Previous Item Added.

    // Hourglass on!

    SetCapture(GetParent(hwndTreeView));
    SetCursor(LoadCursor(NULL, IDC_WAIT));

    // Clear TreeView Control

    TreeView_DeleteAllItems(hwndTreeView);

    // Find Windows Directory

    GetWindowsDirectory(szWindowsDirectory, sizeof(szWindowsDirectory));

    // Make a *.INI mask

    lstrcat(szWindowsDirectory, "\\*.ini");

    // Find the first INI file 

    hFindFile = FindFirstFile(szWindowsDirectory, &FindFile);

    // Loop through each INI file and add to the tree

    if (INVALID_HANDLE_VALUE != hFindFile)
    {
        do
        {
            // Add this file to the TreeView
            // Set up the structure for a text label, and use
            // the ImageList ID iINIFileImage.
            
            tvi.mask            = TVIF_TEXT | TVIF_IMAGE;
            tvi.iImage          =
            tvi.iSelectedImage  = iINIFileImage;

            tvi.pszText         = FindFile.cFileName;
            tvi.cchTextMax      = MAX_PATH;

            // Populate the TreeVeiw Insert Struct
            // The item is the one filled above.
            // Insert it after the last item inserted at this level.
            // And indicate this is a root entry.

            tvins.item         = tvi;
            tvins.hInsertAfter = hPrev;
            tvins.hParent      = TVI_ROOT;

            // Add the item to the tree

            hPrev = TreeView_InsertItem(hwndTreeView, &tvins);

            // Add the keys for this ini to the TreeView

            FillIniKeys(hwndTreeView, 
                        hPrev, 
                        FindFile.cFileName,
                        iSectionKeyImage,
                        iSelectedEntryImage,
                        iNonSelectedEntryImage);
        }
        while (FindNextFile(hFindFile, &FindFile));

    } /* end if */

    // Sort TreeView Control
    //
    // Sort at the root, and recurse down the children levels.

//    TreeView_SortChildren(hwndTreeView, TVI_ROOT, TRUE);

    // Hourglass off!

    ReleaseCapture();
    SetCursor(LoadCursor(NULL, IDC_ARROW));
}

//
//  FUNCTION: bThisLineIsSection(LPSTR *pszFile, LPSTR szSectionName)
//
//  PURPOSE: Checks the next line of text in a file memory image
//           and it if is a [section], copies the section into
//           the szSectionName string and returns TRUE.
//
//  PARAMETERS:
//    pszFile      - Points to a LPSTR, which points to the start of
//                   a line of text in the file memory image.
//    szSectionNmae- String to copy section name to on success.
//
//  RETURN VALUE:
//    On success - TRUE
//    On Failure - FALSE
//
//  COMMENTS:
//
//    The pszFile pointer MUST point to the start of a line of
//    text in a file memory image. This function will move the
//    pointer to the start of the next line in the file memory
//    image.
//

BOOL bThisLineIsSection(LPSTR *pszFile, LPSTR szSectionName)
{
    int    iFilePtrPos;       // Character position in file mem image
    int    iSectionStringPtr; // Character position in szSectionName
    BOOL   bRetval = FALSE;   // Return value for function

    // Initialize position counters

    iFilePtrPos = iSectionStringPtr = 0;

    // Check to see if line starts with a '[', that would make
    // it a [section].

    if ('[' == (*pszFile)[iFilePtrPos])
    {
        // Bump char position in file by one to skip the
        // open square bracket.

        iFilePtrPos++;

        // Keep copying characters into the szSectionName string
        // until the closing square bracket is found.

        while ((*pszFile)[iFilePtrPos] && (']' != (*pszFile)[iFilePtrPos]))
        {
            szSectionName[iSectionStringPtr] = (*pszFile)[iFilePtrPos];
            iSectionStringPtr++;
            iFilePtrPos++;
        }
      
        // Add the null terminator to the end of szSectionName

        szSectionName[iSectionStringPtr] = 0;

        // Indicate sucess for the return value.

        bRetval = TRUE;
    }

    // Now, we must skip over the CRLF at the and of each text line
    // so that the next time this function is called, the pointer
    // will be at the end of the line

    while ( 
            ((*pszFile)[iFilePtrPos])          // While not EOF
            && 
            ('\n' != (*pszFile)[iFilePtrPos])  // While not LF char
          ) 
        iFilePtrPos++;

    // After the above while loop, the iFilePtrPos is still sitting
    // on the LF. Bump it to the next character, which will be the
    // first character of the next line.

    if ((*pszFile)[iFilePtrPos]) iFilePtrPos++;

    // Change the value of the pointer to the file memory image
    // to point to the next line.

    *pszFile += iFilePtrPos;

    // Return the results.

    return bRetval;
}

//
//  FUNCTION: FillIniKeys(HWND        hwndTreeView, 
//                        HTREEITEM   hParent, 
//                        LPSTR       szIniFileName,
//                        int         iSectionKeyImage,
//                        int         iSelectedEntryImage,
//                        int         iNonSelectedEntryImage)
//
//  PURPOSE: Adds all the [section] names of the specified
//           INI file to the TreeView control.
//
//  PARAMETERS:
//    hwndTreeView           - Handle fo TreeView Control.
//    hParent                - Handle to root TreeView Item which
//                             inidcates the name of the INI file.
//    szIniFileName          - Filename of the INI file.
//    iSectionKeyImage       - ImageList ID for "Folder" item.
//    iSelectedEntryImage    - ImageList ID for selected entry.
//    iNonSelectedEntryImage - ImageList ID for non-selected entry.
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
//    This function makes a memory image of the file by loading it
//    into memory. Then, a pointer to this image is repeatedly
//    passed to the bThisLineIsSection function, which then
//    determines if the line is a [section]. If so, the section
//    is added to the TreeView control, and the FillKeyItems
//    function is called to fill in the individual entries.
//

void FillIniKeys(HWND        hwndTreeView, 
                 HTREEITEM   hParent, 
                 LPSTR       szIniFileName,
                 int         iSectionKeyImage,
                 int         iSelectedEntryImage,
                 int         iNonSelectedEntryImage)
{
    static char      szIniFile[MAX_INIFILE_SIZE];  // Memory image of file

    char             szRawSection[MAX_SECTION_KEY_SIZE]; // Holds "Section".
    char             szSection[MAX_SECTION_KEY_SIZE+2];  // Holds "[Section]".
    int              iIniFileSize;                       // Size of INI file.
    HFILE            hFile;                              // File handle.
    OFSTRUCT         of;                                 // For OpenFile. 
    LPSTR            ptr;                                // Pointer to file
                                                         // image.
    TV_ITEM          tvi;                                // TreeView Item.
    TV_INSERTSTRUCT  tvins;                              // TreeView Insert
                                                         // struct.
    HTREEITEM        hPrev = NULL;                       // Previous Item
                                                         // added.

    // Load the INI file into the memory image, and count the
    // bytes.

    hFile        = OpenFile(szIniFileName, &of, OF_READ);
    iIniFileSize = _lread(hFile, szIniFile, MAX_INIFILE_SIZE-1);
    _lclose(hFile);

    // Terminate the EOF with a zero, this lets the bThisLineIsSection
    // function and the loop below know when to stop.

    szIniFile[iIniFileSize] = 0;

    // Set the pointer to the start of the memory image.

    ptr = szIniFile;

    // Loop through each line in the memory image

    while (*ptr)
    {
        // Check to see if this line is a [section]

        if (bThisLineIsSection(&ptr, szRawSection))
        {
            // Format the string to look like a [Section]

            wsprintf (szSection, "[%s]", szRawSection );

            // Add the string to the TreeView
            // Set up the structure for a text label, and use
            // the ImageList ID iSectionKeyImage for both images
            // and selected images. If we did not use the
            // TVIF_SELECTEDIMAGE mask, then the selected
            // image of the parent of this item would be used
            // for this image, which would look goofy.
 
            tvi.mask            = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvi.iImage          =
            tvi.iSelectedImage  = iSectionKeyImage;

            tvi.pszText    = szSection;
            tvi.cchTextMax = sizeof(szSection);

            // Populate the TreeVeiw Insert Struct
            // The item is the one filled above.
            // Insert it after the last item inserted at this level.
            // And indicate this is a child entry from the hParent
            // item passed into this function.

            tvins.item         = tvi;
            tvins.hInsertAfter = hPrev;
            tvins.hParent      = hParent;

            // Add the item to the tree

            hPrev = TreeView_InsertItem(hwndTreeView, &tvins);

            // Add the entries for this section to the TreeView.

            FillKeyItems(hwndTreeView, 
                         hPrev, 
                         szIniFileName, 
                         szRawSection, 
                         iSelectedEntryImage,
                         iNonSelectedEntryImage);

        } /* end if */

    } /* end while */

}


//
//  FUNCTION: FillKeyItems(HWND        hwndTreeView, 
//                         HTREEITEM   hParent, 
//                         LPSTR       szIniFileName,
//                         LPSTR       szSection,
//                         int         iSelectedEntryImage,
//                         int         iNonSelectedEntryImage)
//
//  PURPOSE: Adds all the entries under the specified [section]
//           of the specified INI file to the TreeView control.
//
//  PARAMETERS:
//    hwndTreeView           - Handle fo TreeView Control.
//    hParent                - Handle to root TreeView Item which
//                             inidcates the name of the INI file.
//    szIniFileName          - Filename of the INI file.
//    szSection              - Name of the [section].
//    iSelectedEntryImage    - ImageList ID for selected entry.
//    iNonSelectedEntryImage - ImageList ID for non-selected entry.
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
//    This function uses the GetPrivateProfileSection API to
//    get a list of strings for the specified [section]. Each
//    string is then happily added to the TreeView.
//

void FillKeyItems(HWND        hwndTreeView, 
                  HTREEITEM   hParent, 
                  LPSTR       szIniFileName, 
                  LPSTR       szSection,
                  int         iSelectedEntryImage,
                  int         iNonSelectedEntryImage)
{
    static char  szSectionData[MAX_SECTION_SIZE];          // Place to
                                                           // put all the
                                                           // strings.

    LPSTR szEntryPtr;                               // Pointer to
                                                    // the current
                                                    // entry.

    TV_ITEM         tvi;                            // TreeView Item.
    TV_INSERTSTRUCT tvins;                          // TreeView Insert struct.
    HTREEITEM       hPrev = NULL;                   // Previous Item added.

    // Fill the szSectionData buffer with the entries. The API will
    // fill the buffer like this: "one=1\0two=2" for the following
    // entries:
    //
    // [Section]
    // one=1
    // two=2
    //

    GetPrivateProfileSection(szSection, 
                             szSectionData, 
                             MAX_SECTION_SIZE, 
                             szIniFileName);

    // Set the entry to point to the first item in the above
    // string.

    szEntryPtr = szSectionData;

    // Loop through each entry, and add them to the TreeView.
   
    while (*szEntryPtr)
    {
        // Add the string to the TreeView
        // Set up the structure for a text label, and use
        // the ImageList ID iNonSelectedEntryImage for
        // non-selected images, and iSelectedEntryImage for
        // selected images. If we did not use the
        // TVIF_SELECTEDIMAGE mask, then the selected
        // image of the parent of this item would be used
        // for this image, which would look goofy.

        tvi.mask            = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.iImage          = iNonSelectedEntryImage;
        tvi.iSelectedImage  = iSelectedEntryImage;

        tvi.pszText    = szEntryPtr;
        tvi.cchTextMax = lstrlen(szEntryPtr);

        // Populate the TreeVeiw Insert Struct
        // The item is the one filled above.
        // Insert it after the last item inserted at this level.
        // And indicate this is a child entry from the hParent
        // item passed into this function.

        tvins.item         = tvi;
        tvins.hInsertAfter = hPrev;
        tvins.hParent      = hParent;

        // Add the item to the tree

        hPrev = TreeView_InsertItem(hwndTreeView, &tvins);

        // Bump the pointer to the next string.

        szEntryPtr += lstrlen(szEntryPtr) + 1;

    } /* end while */

}
