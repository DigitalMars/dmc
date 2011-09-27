/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/* reverse.c - WinMain() and WndProc() for REVERSE, along with
 * initialization and support code.
 *
 * REVERSE is a Windows sample application that illustrates how to use
 * the low-level waveform playback services. It also shows how to use
 * the multimedia file I/O services to read data from a WAVE file.
 *
 * REVERSE plays a WAVE waveform audio file backwards.
 */

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "reverse.h"
#include "strings.h"

#define MAX_FILENAME_SIZE 128

/* Global variables.
 */
char     szAppName[]    = "Reverse";   // application name
HANDLE	hInstApp = NULL;     // instance handle
HWND     hwndApp     = NULL;     // main window handle
HWND     hwndName = NULL;     // filename window handle
HWND     hwndPlay = NULL;     // "Play" button window handle
HWND     hwndQuit = NULL;     // "Exit" button window handle
HWAVEOUT hWaveOut = NULL;
LPWAVEHDR   lpWaveHdr   = NULL;
HPSTR    lpData      = NULL;     // waveform data block
LPSTR    lpstrLoadStrBuf   = NULL;

/* WinMain - Entry point for Reverse.
 */
int PASCAL WinMain(
HINSTANCE hInst,
HINSTANCE hPrev,
LPSTR szCmdLine,
int cmdShow)
{
   MSG      msg;
   WNDCLASS wc;

   hInstApp =  hInst;

   /* Define and register a window class for the main window.
    */
   if (!hPrev)
   {
      wc.hCursor     = LoadCursor(NULL, IDC_ARROW);
      wc.hIcon    = LoadIcon(hInst, szAppName);
      wc.lpszMenuName      = szAppName;
      wc.lpszClassName  = szAppName;
      wc.hbrBackground  = GetStockBrush(LTGRAY_BRUSH);
      wc.hInstance      = hInst;
      wc.style    = 0;
      wc.lpfnWndProc    = WndProc;
      wc.cbWndExtra     = 0;
      wc.cbClsExtra     = 0;

      if (!RegisterClass(&wc))
         return FALSE;
   }
   // Keep string memory out of WIN16 DS.
   lpstrLoadStrBuf = GlobalAllocPtr(GMEM_MOVEABLE, LOADSTRBUFSIZE);
   if(lpstrLoadStrBuf == NULL)
   {
      // don't use LoadString here; it fails when memory is low
      MessageBox(NULL, "No memory left.",
            NULL, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
      return FALSE;
   }
   /* Create and show the main window.
    */
   LoadString(hInstApp, IDS_REVERSEWNDTITLE, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   hwndApp = CreateWindow (szAppName,  // class name
         lpstrLoadStrBuf,  // title from string resource
         WS_OVERLAPPEDWINDOW, // style bits
         CW_USEDEFAULT,    // x position
         CW_USEDEFAULT,    // y position
         WMAIN_DX,      // x size
         WMAIN_DY,      // y size
         (HWND)NULL,    // parent window
         (HMENU)NULL,      // use class menu
         (HANDLE)hInst,    // instance handle
         (LPSTR)NULL    // no params to pass on
         );
   /* Create child windows for the "Play" and "Exit" buttons
    * and for an edit field to enter filenames.
    */
   LoadString(hInstApp, IDS_PLAYBUTTONTEXT, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   hwndPlay = CreateWindow( "BUTTON", lpstrLoadStrBuf,
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         PLAY_X, PLAY_Y,
         PLAY_DX, PLAY_DY,
         hwndApp, (HMENU)IDB_PLAY, hInstApp, NULL );
   if( !hwndPlay )
      return( FALSE );

   LoadString(hInstApp, IDS_EXITBUTTONTEXT, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   hwndQuit = CreateWindow( "BUTTON", lpstrLoadStrBuf,
         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
         QUIT_X, QUIT_Y,
         QUIT_DX, QUIT_DY,
         hwndApp, (HMENU)IDB_QUIT, hInstApp, NULL );
   if( !hwndQuit )
      return( FALSE );

   hwndName = CreateWindow("EDIT","",
         WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL,
         NAME_X, NAME_Y,
         NAME_DX, NAME_DY,
         hwndApp, (HMENU)IDE_NAME, hInstApp, NULL);
   if( !hwndName )
      return( FALSE );
   Edit_LimitText(hwndName, MAX_FILENAME_SIZE - 1);

   ShowWindow(hwndApp,cmdShow);

   /* Add about dialog to system menu.
    */
   LoadString(hInstApp, IDS_ABOUTMENUTEXT, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   AppendMenu(GetSystemMenu(hwndApp, FALSE),
      MF_STRING | MF_ENABLED, IDM_ABOUT, lpstrLoadStrBuf);


   /* The main message processing loop. Nothing special here.
   */
   while (GetMessage(&msg,NULL,0,0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
}


/* WndProc - Main window procedure function.
 */
LONG FAR PASCAL WndProc(
HWND hWnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
   switch (msg)
   {
      HANDLE_MSG(hWnd, WM_DESTROY, ReverseOnDestroy);

      HANDLE_MSG(hWnd, WM_SYSCOMMAND, ReverseOnSysCommand);

      case WM_SETFOCUS:
         SetFocus(hwndName);
         return 0;

      HANDLE_MSG(hWnd, WM_COMMAND, ReverseOnCommand);
      case MM_WOM_DONE:
         /* This message indicates a waveform data block has
          * been played and can be freed. Clean up the
          * preparation done previously on the header.
          */
         waveOutUnprepareHeader( (HWAVEOUT) wParam,
            (LPWAVEHDR) lParam, sizeof(WAVEHDR) );

         /* free all memory associated with the data block
          */
         cleanup();
         /* Close the waveform output device.
          */
         waveOutClose( (HWAVEOUT) wParam );
         hWaveOut = NULL;

         /* Reenable both button controls.
          */
         EnableWindow( hwndPlay, TRUE );
         EnableWindow( hwndQuit, TRUE );
         SetFocus(hwndName);

         break;
   }
   return MyDefProc(hWnd,msg,wParam,lParam);
}

void ReverseOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
   /* Process messages sent by the child window controls.
    */
   switch (id)
   {
      case IDE_NAME:    // filename edit control
         return;

      case IDB_PLAY: // "Play" button
         if (codeNotify == BN_CLICKED)
            ReversePlay();
         break;

      case IDB_QUIT: // "Exit" button
         if (codeNotify == BN_CLICKED)
            PostQuitMessage(0);
         break;
   }
   FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, MyDefProc);
}

void ReverseOnDestroy(
HWND hwnd)
{
   if(lpstrLoadStrBuf)
      GlobalFreePtr(lpstrLoadStrBuf);
   if (hWaveOut)
   {
      waveOutReset(hWaveOut);
      waveOutUnprepareHeader(hWaveOut, lpWaveHdr,
         sizeof(WAVEHDR) );
      cleanup();
      waveOutClose(hWaveOut);
   }
   PostQuitMessage(0);
   FORWARD_WM_DESTROY(hwnd, MyDefProc);
}

void ReverseOnSysCommand(
HWND hwnd,
UINT cmd,
int x,
int y)
{
   switch (cmd)
   {
      case IDM_ABOUT:
         /* Show ABOUTBOX dialog box.
          */
         DialogBox(hInstApp, "ABOUTBOX", hwnd, AppAbout);
         break;
   }
   FORWARD_WM_SYSCOMMAND(hwnd, cmd, x, y, MyDefProc);
}



/* AppAbout -- Dialog procedure for ABOUTBOX dialog box.
 */
BOOL FAR PASCAL AppAbout(
HWND hDlg,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
   switch (msg)
   {
      case WM_COMMAND:
         if (GET_WM_COMMAND_ID(wParam, lParam) == IDOK
            || GET_WM_COMMAND_ID(wParam, lParam)
            == IDCANCEL)
            EndDialog(hDlg,TRUE);
         break;

      case WM_INITDIALOG:
         return TRUE;
   }
   return FALSE;
}



/* ReversePlay - Gets a filename from the edit control, then uses
 * the multimedia file I/O services to read data from the requested
 * WAVE file. If the file is a proper WAVE file, ReversePlay() calls
 * the Interchange() function to reverse the order of the waveform
 * samples in the file. It then plays the reversed waveform data.
 *
 * Note that ReversePlay() only handles a single waveform data block.
 * If the requested WAVE file will not fit in a single data block, it
 * will not be played. The size of a single data block depends on the
 * amount of available system memory.
 *
 * Params: void
 *
 * Return: void
 */
void ReversePlay()
{
   HMMIO          hmmio;
   MMCKINFO       mmckinfoParent;
   MMCKINFO       mmckinfoSubchunk;
   DWORD          dwFmtSize;
   char           szFileName[ MAX_FILENAME_SIZE ];
   DWORD          dwResult;
   HANDLE         hFormat;
   WAVEFORMATEX   *pFormat;
   DWORD          dwDataSize;
   HPSTR          hpch1, hpch2;
   WORD           wBlockSize;
   HANDLE         hData    = NULL;

   /* Get the filename from the edit control.
    */
   if (!GetWindowText( hwndName, (LPSTR)szFileName, MAX_FILENAME_SIZE))
   {
      LoadString(hInstApp, IDS_FAILEDTOGETFNAME, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* Open the given file for reading using buffered I/O.
    */
   if(!(hmmio = mmioOpen(szFileName, NULL, MMIO_READ | MMIO_ALLOCBUF)))
   {
      LoadString(hInstApp, IDS_FAILEDTOOPENFILE, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* Locate a 'RIFF' chunk with a 'WAVE' form type
    * to make sure it's a WAVE file.
    */
   mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   if (mmioDescend(hmmio, &mmckinfoParent, NULL, MMIO_FINDRIFF))
   {
      LoadString(hInstApp, IDS_NOTAWAVEFILE, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      mmioClose(hmmio, 0);
      return;
   }

   /* Now, find the format chunk (form type 'fmt '). It should be
    * a subchunk of the 'RIFF' parent chunk.
    */
   mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent,
      MMIO_FINDCHUNK))
   {
      LoadString(hInstApp, IDS_WAVEFILECORRUPT, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      mmioClose(hmmio, 0);
      return;
   }

   /* Get the size of the format chunk, allocate and lock memory for it.
    */
   dwFmtSize = mmckinfoSubchunk.cksize;
   hFormat = LocalAlloc(LMEM_MOVEABLE, LOWORD(dwFmtSize));
   if (!hFormat)
   {
      MessageBox(hwndApp, "Out of memory.",
            NULL, MB_OK | MB_ICONEXCLAMATION);
      mmioClose(hmmio, 0);
      return;
   }
   pFormat = (WAVEFORMATEX *) LocalLock(hFormat);
   if (!pFormat)
   {
      MessageBox(hwndApp, "Failed to lock memory for format chunk.",
            NULL, MB_OK | MB_ICONEXCLAMATION);
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      return;
   }

   /* Read the format chunk.
    */
   if (mmioRead(hmmio, (HPSTR) pFormat, dwFmtSize) != (LONG) dwFmtSize)
   {
      LoadString(hInstApp, IDS_FAILEDREADFMTCHNK, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      return;
   }

   /* Make sure it's a PCM file.
    */
   if (pFormat->wFormatTag != WAVE_FORMAT_PCM)
   {
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      LoadString(hInstApp, IDS_NOTAPCMFILE, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* Make sure a waveform output device supports this format.
    */
   #if (WINVER >= 0x0400)
   if (waveOutOpen(&hWaveOut, WAVE_MAPPER, pFormat, 0, 0L,
               WAVE_FORMAT_QUERY))
        #else
   if (waveOutOpen(&hWaveOut, WAVE_MAPPER, (LPWAVEFORMAT)pFormat, 0, 0L,
               WAVE_FORMAT_QUERY))
        #endif
   {
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      LoadString(hInstApp, IDS_CANTPLAYFORMAT, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* Ascend out of the format subchunk.
    */
   mmioAscend(hmmio, &mmckinfoSubchunk, 0);

   /* Find the data subchunk.
    */
   mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
   if (mmioDescend(hmmio, &mmckinfoSubchunk, &mmckinfoParent,
      MMIO_FINDCHUNK))
   {
      LoadString(hInstApp, IDS_NODATACHUNK, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      return;
   }

   /* Get the size of the data subchunk.
    */
   dwDataSize = mmckinfoSubchunk.cksize;
   if (dwDataSize == 0L)
   {
      LoadString(hInstApp, IDS_CHUNKHASNODATA, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      return;
   }

   /* Open a waveform output device.
    */
   #if (WINVER >= 0x0400)
   if (waveOutOpen(&hWaveOut, WAVE_MAPPER,
      pFormat, (UINT)hwndApp, 0L, CALLBACK_WINDOW))
        #else
   if (waveOutOpen(&hWaveOut, WAVE_MAPPER,
      (LPWAVEFORMAT)pFormat, (UINT)hwndApp, 0L, CALLBACK_WINDOW))
        #endif
   {
      LoadString(hInstApp, IDS_FAILEDOPENDEVICE, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      LocalUnlock( hFormat );
      LocalFree( hFormat );
      mmioClose(hmmio, 0);
      return;
   }

   /* Save block alignment info for later use.
    */
   wBlockSize = pFormat->nBlockAlign;

   /* We're done with the format header, free it.
    */
   LocalUnlock( hFormat );
   LocalFree( hFormat );

   /* Allocate and lock memory for the waveform data.
    */
   lpData = GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, dwDataSize );
   if (!lpData)
   {
      MessageBox(hwndApp, "Not enough memory for data chunk.",
            NULL, MB_OK | MB_ICONEXCLAMATION);
      mmioClose(hmmio, 0);
      return;
   }

   /* Read the waveform data subchunk.
    */
   if(mmioRead(hmmio, lpData, dwDataSize) != (LONG) dwDataSize)
   {
      LoadString(hInstApp, IDS_FAILEDREADCHUNK, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      GlobalFreePtr( lpData );
      mmioClose(hmmio, 0);
      return;
   }

   /* We're done with the file, close it.
    */
   mmioClose(hmmio, 0);

   /* Reverse the sound for playing.
    */
   hpch1 = lpData;
   hpch2 = lpData + dwDataSize - 1;
   while (hpch1 < hpch2)
   {
      Interchange( hpch1, hpch2, wBlockSize );
      hpch1 += wBlockSize;
      hpch2 -= wBlockSize;
   }

   /* Allocate a waveform data header. The WAVEHDR must be
    * globally allocated and locked.
    */
   lpWaveHdr = (LPWAVEHDR)GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE,
                  (DWORD) sizeof(WAVEHDR));
   if (!lpWaveHdr)
   {
      GlobalFreePtr( lpData );
      MessageBox(hwndApp, "Not enough memory for header.",
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* If you need instance data for a waveform data block, allocate some
    * memory and store the pointer in lpWaveHdr->dwUser, before the call
    * to waveOutPrepareHeader(). The code inside the #if 0 / #endif, and
    * the commented-out lpWaveHdr->dwUser = ... illustrate this.
    * Don't forget to free the instance memory when you're done with it,
    * or on error bailout.
    */
   #if 0
   lpYourData = GlobalAllocPtr(GMEM_MOVEABLE | GMEM_SHARE, sizeof(YOURDATA));
   if (!lpYourData)
   {
      GlobalFreePtr( lpData );
      GlobalFreePtr( lpWaveHdr );
      MessageBox(hwndApp, "Not enough memory for instance data.",
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }
   #endif
   /* Set up WAVEHDR structure and prepare it to be written to wave device.
    */
   lpWaveHdr->lpData = lpData;
   lpWaveHdr->dwBufferLength = dwDataSize;
   lpWaveHdr->dwFlags = 0L;
   lpWaveHdr->dwLoops = 0L;
   // lpWaveHdr->dwUser = (DWORD) lpYourData; // save instance data ptr
   if(waveOutPrepareHeader(hWaveOut, lpWaveHdr, sizeof(WAVEHDR)))
   {
      cleanup();
      LoadString(hInstApp, IDS_UNABLEPREPAREHDR, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);

      return;
   }

   /* Then the data block can be sent to the output device.
    */
   dwResult = waveOutWrite(hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
   if (dwResult != 0)
   {
      waveOutUnprepareHeader( hWaveOut, lpWaveHdr, sizeof(WAVEHDR));
      cleanup();
      LoadString(hInstApp, IDS_FAILEDWRITEDEVICE, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      MessageBox(hwndApp, lpstrLoadStrBuf,
            NULL, MB_OK | MB_ICONEXCLAMATION);
      return;
   }

   /* Disable input to the button controls.
    */
   EnableWindow(hwndPlay, FALSE);
   EnableWindow(hwndQuit, FALSE);
}

/* Interchange - Interchanges two samples at the given positions.
 *
 * Params:  hpchPos1 - Points to one sample.
 *       hpchPos2 - Points to the other sample.
 *       wLength  - The length of a sample in bytes.
 *
 * Return: void
 */
void Interchange(
HPSTR hpchPos1,
HPSTR hpchPos2,
WORD wLength)
{
   WORD  wPlace;
   BYTE  bTemp;

   for (wPlace = 0; wPlace < wLength; wPlace++)
   {
      bTemp = hpchPos1[wPlace];
      hpchPos1[wPlace] = hpchPos2[wPlace];
      hpchPos2[wPlace] = bTemp;
   }
}

VOID cleanup(void)
{
   // if you add wave instance data, this is a good place to free it.
   GlobalFreePtr( lpWaveHdr );
   GlobalFreePtr( lpData );
}

