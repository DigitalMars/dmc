// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    fileio.c
//
//  PURPOSE:   Performs application and instance specific initialization.
//
//  FUNCTIONS:
//    LoadDIBSection    - Loads a DIB from file into buffer created by
//                        CreateDIBSection
//    SaveDIB           - Saves the specified dib in a file
//
//  COMMENTS:
//

#include <windows.h>       // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>      // prototypes and structure for the common controls.

#include "globals.h"       // prototypes specific to this application
#include "resource.h"
#include "toolbar.h"
#include "palette.h"
#include "dibutil.h"

// Dib Header Marker - used in writing DIBs to files
#define DIB_HEADER_MARKER   ((WORD)('M' << 8) | 'B')

//
//  FUNCTION: LoadDIBSection(HWND, LPSTR)
//
//  PURPOSE: Loads the specified DIB from a file, and reads it into the buffer
//    created by CreateDIBSection.
//
//  PARAMETERS:
//    hwnd          - specifies window for display context
//    lpFileName    - specifies the file to load a DIB from
//
//  RETURN VALUE:
//    A handle to a bitmap, or NULL if unsuccessful.
//
//  COMMENTS:
//    The routines in this sample were not all written to handle OS/2 DIBs.
//    This function will reject any file that is not a Windows DIB.
//
//
HBITMAP LoadDIBSection(HWND hwnd, LPSTR lpFileName)
{
    HDC                 hdc;
    HANDLE              hFile;
    BITMAPFILEHEADER    bmfHeader;
    UINT                nNumColors;
    HDIB                hDIBNew;
    LPBITMAPINFOHEADER  lpbih;
    DWORD               dwBytesRead;

    // Set the cursor to a hourglass, in case the loading operation
    // takes more than a sec, the user will know what's going on.
    SetCursor(LoadCursor(NULL, IDC_WAIT));

    // open the DIB file
    hFile = CreateFile(lpFileName,
                       GENERIC_READ,
                       FILE_SHARE_READ,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        DIBError(ERR_FILENOTFOUND);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return NULL;
    }

    // read the BITMAPFILEHEADER from our file

    if (!ReadFile(hFile,
                  &bmfHeader,
                  sizeof(BITMAPFILEHEADER),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != sizeof(BITMAPFILEHEADER))
    {
        DIBError(ERR_READ);
        goto ErrExit;
    }

    if (bmfHeader.bfType != 0x4d42)  // 'BM'
    {
        DIBError(ERR_NOT_DIB);
        goto ErrExit;
    }

    // Allocate memory for header & color table.
    hDIBNew = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

    if (!hDIBNew)
    {
        DIBError(ERR_MEMORY);
        goto ErrExit;
    }

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBNew);
    if (!lpbih)
    {
        DIBError(ERR_LOCK);
        goto ErrExitFree;
    }

    // read the BITMAPINFOHEADER
    if (!ReadFile(hFile,
                  lpbih,
                  sizeof(BITMAPINFOHEADER),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != sizeof(BITMAPINFOHEADER))
    {
        DIBError(ERR_READ);
        goto ErrExitUnlock;
    }

    // Check to see that it's a Windows DIB -- an OS/2 DIB would cause 
    // problems with some of the functions in this sample since the fields
    // in the header are different and color table entries are smaller.
    // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.
    if (lpbih->biSize != sizeof(BITMAPINFOHEADER))
    {
        DIBError(ERR_NOT_DIB);
        goto ErrExitUnlock;
    }

    // check to see if it's a format we support
    switch (lpbih->biBitCount)
    {
        case 1:
        case 4:
        case 8:
        case 16:
        case 24:
        case 32:

            // the format is supported so continue
            break;

        default:
            // the format isn't supported so fail
            DIBError(ERR_UNSUPPORTEDFORMAT);
            goto ErrExitUnlock;
    }

    // Now determine the size of the color table and read it.  Since the
    // bitmap bits are offset in the file by bfOffBits, do some special 
    // processing to make sure the bits directly follow the color table  
    if (!(nNumColors = (UINT)lpbih->biClrUsed))
    {
        // color table only for 8 bits per pixel or less

        if (lpbih->biBitCount <= 8)
            nNumColors = 1 << lpbih->biBitCount; // standard size table
    }

    // fill in some default values if they are zero
    if (lpbih->biClrUsed == 0)
        lpbih->biClrUsed = nNumColors;

    if (lpbih->biSizeImage == 0)
        lpbih->biSizeImage = WIDTHBYTES(lpbih->biWidth * lpbih->biBitCount)
                                * lpbih->biHeight;

    if (nNumColors > 256)   // very rare, but possible
    {
        HDIB hDIBInfoTemp;

        // get a proper-sized buffer for header and color table
        GlobalUnlock(hDIBNew);
        hDIBInfoTemp = GlobalReAlloc(hDIBNew,
                                     lpbih->biSize + nNumColors * sizeof(RGBQUAD),
                                     0);

        if (!hDIBInfoTemp) // can't resize buffer for loading
        {
            DIBError(ERR_MEMORY);
            goto ErrExitFree;
        }
        else
            hDIBNew = hDIBInfoTemp;

        lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBNew);
        if (!lpbih)
        {
            DIBError(ERR_LOCK);
            goto ErrExitFree;
        }
    }

    // read the color table
    if (!ReadFile(hFile,
                  (LPSTR)(lpbih) + lpbih->biSize,
                  nNumColors * sizeof(RGBQUAD),
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != nNumColors * sizeof(RGBQUAD))
    {
        DIBError(ERR_READ);
        goto ErrExitUnlock;
    }

    // we've reached the point of no return (no undo)
    // remove existing DIB section
    RemoveDIBSection();

    // set global variable to new DIB header
    hDIBInfo = hDIBNew;

    // create new DIB section
    hdc = GetDC(hwnd);
    hBitmap = CreateDIBSection(hdc,
                               (LPBITMAPINFO)lpbih,
                               DIB_RGB_COLORS,
                               &lpvBits,
                               0,
                               0L);
    ReleaseDC(hwnd, hdc);

    if (!hBitmap || !lpvBits)
    {
        DIBError(ERR_CREATEDIBSECTION);

        if (hBitmap)
            DeleteObject(hBitmap);

        // create default wash for palette
        if (bPalDevice)
        {
            HDC hdc;

            hdc = GetDC(hwnd);
            hPalette = CreateHalftonePalette(hdc);
            ReleaseDC(hwnd, hdc);
        }

        goto ErrExitUnlock;
    }

    // fill the buffer allocated by CreateDIBSection with bitmap bits        
    SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN);
    if (!ReadFile(hFile,
                  lpvBits,
                  lpbih->biSizeImage,
                  &dwBytesRead,
                  NULL)
        || dwBytesRead != lpbih->biSizeImage)
    {
        // failed to read the DIB bits into the DIB section so get rid of it
        DIBError(ERR_READ);
        DeleteObject(hBitmap);
        goto ErrExitUnlock;
    }

    // create a palette for the DIB section from the DIB color table
    hPalette = CreateDIBPalette(hDIBInfo);
    // if hPalette is NULL it won't lead to failure condition, so don't check it

    // unlock the DIB header but don't free it!
    GlobalUnlock(hDIBInfo);

    CloseHandle(hFile);
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    // update status of menu and toolbar
    EnableMenuItem(hMenu, IDM_FILESAVEAS, MF_ENABLED);
    EnableMenuItem(hMenu, IDM_FILECLOSE, MF_ENABLED);
    EnableMenuItem(hMenu, IDM_FILEPRINT, MF_ENABLED);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILEPRINT, MAKELONG(TRUE, 0));
    EnableMenuItem(hMenu, IDM_CLEAR, MF_ENABLED);

    return hBitmap;

ErrExitUnlock:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call
    GlobalUnlock(hDIBNew);

ErrExitFree:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call
    GlobalFree(hDIBNew);

ErrExit:
    CloseHandle(hFile);
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    return NULL;
}


//
//  FUNCTION: SaveDIB(HDIB, LPVOID, LPSTR)
//
//  PURPOSE:  Saves a DIB to a file.
//
//  PARAMETERS:
//    hDib       - Handle to BITMAPFINFO and color table (if any) of dib
//    lpvDIBBits - Pointer to bitmap bits
//    lpFileName - Pointer to full pathname to save DIB under
//
//  RETURN VALUE:
//    0 if successful, or one of ERR_INVALIDHANDLE, ERR_OPEN, ERR_LOCK or ERR_SAVE.
//
//  COMMENTS:
//    The routines in this sample were not all written to handle OS/2 DIBs.
//    This function will reject any file that is not a Windows DIB.
//
//
WORD SaveDIB(HDIB hDib, LPVOID lpvDIBits, LPSTR lpFileName)
{
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file
    LPBITMAPINFOHEADER  lpbih;      // Pointer to DIB info structure
    HANDLE              hFile;      // file handle for opened file
    DWORD               dwDIBHeaderSize, dwDIBBitsSize, dwBytesWritten;
    BOOL                bError;

    if (!hDib)
        return ERR_INVALIDHANDLE;

    hFile = CreateFile(lpFileName,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return ERR_OPEN;

    // Get a pointer to the DIB memory, the first of which contains
    // a BITMAPINFO structure

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    if (!lpbih)
        return ERR_LOCK;

    // Check to see if we're dealing with an OS/2 DIB.  If so, don't
    // save it because our functions aren't written to deal with these
    // DIBs.

    if (lpbih->biSize != sizeof(BITMAPINFOHEADER))
    {
        GlobalUnlock(hDib);
        return ERR_NOT_DIB;
    }

    // Fill in the fields of the file header

    // Fill in file type (first 2 bytes must be "BM" for a bitmap)

    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

    // Calculating the size of the DIB is a bit tricky (if we want to
    // do it right).  The easiest way to do this is to call GlobalSize()
    // on our global handle, but since the size of our global memory may have
    // been padded a few bytes, we may end up writing out a few too
    // many bytes to the file (which may cause problems with some apps,
    // like HC 3.0).
    //
    // So, instead let's calculate the size manually.
    //
    // We most likely do not have a packed DIB to work with, in other words,
    // the bitmap bits do not immediately follow the DIB header and color table
    // (if any).  So calculate sizes individually.
 
    // Calculate size of header and color table (if any)
    dwDIBHeaderSize = lpbih->biSize + PaletteSize((LPSTR)lpbih);

    // Now calculate the size of the image

    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage
    // field

    if ((lpbih->biCompression == BI_RLE8) || (lpbih->biCompression == BI_RLE4))
        dwDIBBitsSize = lpbih->biSizeImage;
    else
    {
        // It's not RLE, so size is Width (DWORD aligned) * Height

        dwDIBBitsSize = WIDTHBYTES(lpbih->biWidth * lpbih->biBitCount)
                            * lpbih->biHeight;

        // Now, since we have calculated the correct size, why don't we
        // fill in the biSizeImage field (this will fix any .BMP files which
        // have this field incorrect).

        lpbih->biSizeImage = dwDIBBitsSize;
    }


    // Calculate file size by adding DIB size to sizeof(BITMAPFILEHEADER)

    bmfHdr.bfSize = dwDIBHeaderSize + dwDIBBitsSize + sizeof(BITMAPFILEHEADER);
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;

    // Now, calculate the offset the actual bitmap bits will be in
    // the file -- It's the Bitmap file header plus the DIB header,
    // plus the size of the color table.

    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + dwDIBHeaderSize;

    // Write the file header

    bError = (!WriteFile(hFile, &bmfHdr, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL)
                || dwBytesWritten != sizeof(BITMAPFILEHEADER));

    // Write the DIB header and the bits

    if (!bError)
        bError = (!WriteFile(hFile, lpbih, dwDIBHeaderSize, &dwBytesWritten, NULL)
                    || dwBytesWritten != dwDIBHeaderSize);

    if (!bError)
        bError = (!WriteFile(hFile, lpvBits, dwDIBBitsSize, &dwBytesWritten, NULL)
                    || dwBytesWritten != dwDIBBitsSize);

    GlobalUnlock(hDib);
    CloseHandle(hFile);

    return (bError ? ERR_SAVE : 0);
}
