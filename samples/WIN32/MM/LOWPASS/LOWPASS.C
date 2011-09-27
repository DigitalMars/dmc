/***********************************************************************
 * lowpass.c - WinMain() and dialog procedures for LOWPASS, along with
 * initialization and support code.
 *
 * LOWPASS is a sample application illustrating how to use the multimedia
 * file I/O services to read and write RIFF files.
 *
 * LOWPASS runs a simple low-pass filter over an 8-bit-per-sample
 * mono WAVE file.  Note that this program does not copy unknown chunks
 * to the output file.
 ************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
 *  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
 *  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
 *  A PARTICULAR PURPOSE.
 *
 *  Copyright (C) 1993 - 1995 Microsoft Corporation. All Rights Reserved.
 *********************************************************************** */

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "lowpass.h"
#include "strings.h"


/* Globals
 */
char     gszAppName[] = "LowPass";  // for title bar, etc.
HINSTANCE   ghInst;           // app's instance handle
LPSTR    lpstrLoadStrBuf   = NULL;


/* DoLowPass - Gets the name of the input and output WAVE files from
 *  the dialog box; reads waveform data from the input file, performs
 *  a simple low-pass filter by averaging adjacent samples, and writes
 *  the filtered waveform data to the output WAVE file.
 *
 * Params:  hWnd - Window handle for our dialog box.
 *
 * Returns:
 */
void DoLowPass(
HWND hWnd)
{
   char         achInFile[200];  // name of input file
   char         achOutFile[200]; // name of output file
   HMMIO        hmmioIn = NULL;  // handle to open input WAVE file
   HMMIO        hmmioOut = NULL; // handle to open output WAVE file
   MMCKINFO       ckInRIFF;   // chunk info. for input RIFF chunk
   MMCKINFO       ckOutRIFF;  // chunk info. for output RIFF chunk
   MMCKINFO       ckIn;    // info. for a chunk in input file
   MMCKINFO       ckOut;      // info. for a chunk in output file
   PCMWAVEFORMAT  pcmWaveFormat; // contents of 'fmt' chunks
   MMIOINFO       mmioinfoIn; // current status of <hmmioIn>
   MMIOINFO       mmioinfoOut;   // current status of <hmmioOut>
   long         lSamples;  // number of samples to filter
   BYTE         abSamples[3]; // this, last, and before-last sample

   /* Read filenames from dialog box fields.
    */
   achInFile[0] = 0;
   GetDlgItemText(hWnd, ID_INPUTFILEEDIT, achInFile, sizeof(achInFile));
   achOutFile[0] = 0;
   GetDlgItemText(hWnd, ID_OUTPUTFILEEDIT, achOutFile, sizeof(achOutFile));

   /* Open the input file for reading using buffered I/O.
    */
   hmmioIn = mmioOpen(achInFile, NULL, MMIO_ALLOCBUF | MMIO_READ);
   if (hmmioIn == NULL)
      goto LOWPASS_ERROR_CANNOT_READ;  // cannot open WAVE file

   /* Open the output file for writing using buffered I/O. Note that
    * if the file exists, the MMIO_CREATE flag causes it to be truncated
    * to zero length.  This means of course that if the SAME FILE is given
    * for both input and output, the input file will be truncated right here
    * and the program will then (surprise!) be unable to read it!
    */
   hmmioOut = mmioOpen(achOutFile, NULL,
                  MMIO_ALLOCBUF | MMIO_WRITE | MMIO_CREATE);
   if (hmmioOut == NULL)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot open WAVE file

   /* Descend the input file into the 'RIFF' chunk.
    */
   if (mmioDescend(hmmioIn, &ckInRIFF, NULL, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_READ;  // end-of-file, probably

   /* Make sure the input file is a WAVE file.
    */
   if ((ckInRIFF.ckid != FOURCC_RIFF) ||
      (ckInRIFF.fccType != mmioFOURCC('W', 'A', 'V', 'E')))
      goto LOWPASS_ERROR_BAD_FORMAT;

   /* Search the input file for for the 'fmt ' chunk.
    */
   ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if (mmioDescend(hmmioIn, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
      goto LOWPASS_ERROR_BAD_FORMAT;   // no 'fmt ' chunk

   /* Expect the 'fmt' chunk to be at least as large as <pcmWaveFormat>;
    * if there are extra parameters at the end, we'll ignore them
    */
   if (ckIn.cksize < (long) sizeof(pcmWaveFormat))
      goto LOWPASS_ERROR_BAD_FORMAT;   // 'fmt ' chunk too small

   /* Read the 'fmt ' chunk into <pcmWaveFormat>.
    */
   if (mmioRead(hmmioIn, (HPSTR) &pcmWaveFormat,
      (long) sizeof(pcmWaveFormat)) != (long) sizeof(pcmWaveFormat))
      goto LOWPASS_ERROR_CANNOT_READ;  // truncated file, probably

   /* Ascend the input file out of the 'fmt ' chunk.
    */
   if (mmioAscend(hmmioIn, &ckIn, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_READ;  // truncated file, probably

   /* Make sure the input file is an 8-bit mono PCM WAVE file.
    */
   if ((pcmWaveFormat.wf.wFormatTag != WAVE_FORMAT_PCM) ||
      (pcmWaveFormat.wf.nChannels != 1) ||
      (pcmWaveFormat.wBitsPerSample != 8))
      goto LOWPASS_ERROR_BAD_FORMAT;   // bad input file format

   /* Search the input file for for the 'data' chunk.
    */
   ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a');
   if (mmioDescend(hmmioIn, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
      goto LOWPASS_ERROR_BAD_FORMAT;   // no 'data' chunk

   /* Create the output file RIFF chunk of form type 'WAVE'.
    */
   ckOutRIFF.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   if (mmioCreateChunk(hmmioOut, &ckOutRIFF, MMIO_CREATERIFF) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* We are now descended into the 'RIFF' chunk we just created.
    * Now create the 'fmt ' chunk. Since we know the size of this chunk,
    * specify it in the MMCKINFO structure so MMIO doesn't have to seek
    * back and set the chunk size after ascending from the chunk.
    */
   ckOut.ckid = mmioFOURCC('f', 'm', 't', ' ');
   ckOut.cksize = sizeof(pcmWaveFormat);  // we know the size of this ck.
   if (mmioCreateChunk(hmmioOut, &ckOut, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* Write the PCMWAVEFORMAT structure to the 'fmt ' chunk.
    */
   if (mmioWrite(hmmioOut, (HPSTR) &pcmWaveFormat, sizeof(pcmWaveFormat))
      != sizeof(pcmWaveFormat))
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
    */
   if (mmioAscend(hmmioOut, &ckOut, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* Create the 'data' chunk that holds the waveform samples.
    */
   ckOut.ckid = mmioFOURCC('d', 'a', 't', 'a');
   if (mmioCreateChunk(hmmioOut, &ckOut, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* Read samples from the 'data' chunk of the input file, and write
    * samples to the 'data' chunk of the output file.  Each sample in
    * the output file equals the average of the corresponding sample
    * in the input file and the previous two samples from the input file.
    * Access the I/O buffers of <hmmioIn> and <hmmioOut> directly,
    * since this is faster than calling mmioRead() and mmioWrite()
    * for each sample.
    */
   abSamples[0] = abSamples[1] = abSamples[2] = 128;

   /* Begin direct access of the I/O buffers.
    */
   if (mmioGetInfo(hmmioIn, &mmioinfoIn, 0) != 0)
      goto LOWPASS_ERROR_UNKNOWN;
   if (mmioGetInfo(hmmioOut, &mmioinfoOut, 0) != 0)
      goto LOWPASS_ERROR_UNKNOWN;

   /* For each input sample, compute and write the output sample.
    */
   for (lSamples = ckIn.cksize; lSamples > 0; lSamples--)
   {
      /* If we are at the end of the input file I/O buffer, fill it.
       * Test to see that we don't hit end of file while (lSamples > 0).
       */
      if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
      {
         if (mmioAdvance(hmmioIn, &mmioinfoIn, MMIO_READ) != 0)
            goto LOWPASS_ERROR_CANNOT_READ;

         if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead)
            goto LOWPASS_ERROR_CANNOT_READ;
      }

      /* If we are the end of the output file I/O buffer, flush it.
       */
      if (mmioinfoOut.pchNext == mmioinfoOut.pchEndWrite)
      {
         mmioinfoOut.dwFlags |= MMIO_DIRTY;
         if (mmioAdvance(hmmioOut, &mmioinfoOut, MMIO_WRITE) != 0)
            goto LOWPASS_ERROR_CANNOT_WRITE;
      }

      /* Keep track of the last 3 samples so we can average.
       */
      abSamples[2] = abSamples[1];  // next-to-last sample
      abSamples[1] = abSamples[0];  // last sample
      abSamples[0] = *(mmioinfoIn.pchNext)++; // current sample

      /* The output file sample is the average of the last
       * 3 input file samples.
       */
      *(mmioinfoOut.pchNext)++ = (BYTE) (((int) abSamples[0]
         + (int) abSamples[1] + (int) abSamples[2]) / 3);
   }

   /* We are through processing samples, end direct access of
    * the I/O buffers. Set the MMIO_DIRTY flag for the output buffer
    * to flush it.
    */
   if (mmioSetInfo(hmmioIn, &mmioinfoIn, 0) != 0)
      goto LOWPASS_ERROR_UNKNOWN;
   mmioinfoOut.dwFlags |= MMIO_DIRTY;
   if (mmioSetInfo(hmmioOut, &mmioinfoOut, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot flush, probably

   /* Ascend the output file out of the 'data' chunk -- this will cause
    * the chunk size of the 'data' chunk to be written.
    */
   if (mmioAscend(hmmioOut, &ckOut, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* Ascend the output file out of the 'RIFF' chunk -- this will cause
    * the chunk size of the 'RIFF' chunk to be written.
    */
   if (mmioAscend(hmmioOut, &ckOutRIFF, 0) != 0)
      goto LOWPASS_ERROR_CANNOT_WRITE; // cannot write file, probably

   /* We are done -- files are closed below.
    */
   goto EXIT_FUNCTION;

LOWPASS_ERROR_BAD_FORMAT:

   LoadString(ghInst, IDS_MUSTBEPCMWAVE, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   MessageBox(hWnd, lpstrLoadStrBuf,
         gszAppName, MB_OK | MB_ICONEXCLAMATION);
   goto EXIT_FUNCTION;

LOWPASS_ERROR_CANNOT_READ:

   LoadString(ghInst, IDS_CANTREADINPUTWAV, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   MessageBox(hWnd, lpstrLoadStrBuf,
         gszAppName, MB_OK | MB_ICONEXCLAMATION);
   goto EXIT_FUNCTION;

LOWPASS_ERROR_CANNOT_WRITE:

   LoadString(ghInst, IDS_CANTWRITEOUTPUT, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   MessageBox(hWnd, lpstrLoadStrBuf,
         gszAppName, MB_OK | MB_ICONEXCLAMATION);
   goto EXIT_FUNCTION;

LOWPASS_ERROR_UNKNOWN:

   LoadString(ghInst, IDS_UNKOWNERROR, lpstrLoadStrBuf,
      LOADSTRBUFSIZE);
   MessageBox(hWnd, lpstrLoadStrBuf,
         gszAppName, MB_OK | MB_ICONEXCLAMATION);
   goto EXIT_FUNCTION;

EXIT_FUNCTION:

   /* Close the files (unless they weren't opened successfully).
    */
   if (hmmioIn != NULL)
      mmioClose(hmmioIn, 0);
   if (hmmioOut != NULL)
      mmioClose(hmmioOut, 0);
}


/* WinMain - Entry point for LowPass.
 */
int PASCAL WinMain(
HINSTANCE hInst,
HINSTANCE hPrev,
LPSTR lpszCmdLine,
int iCmdShow)
{
   // Keep string memory out of WIN16 DS.
   lpstrLoadStrBuf = GlobalAllocPtr(GMEM_MOVEABLE, LOADSTRBUFSIZE);
   if(lpstrLoadStrBuf == NULL)
   {
      // don't use LoadString here; it fails when memory is low
      MessageBox(NULL, "No memory left.",
            NULL, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
      return FALSE;
   }
   /* Save instance handle for dialog boxes.
    */
   ghInst = hInst;

   /* Display our dialog box.
    */
   DialogBox(ghInst, "LOWPASSBOX", NULL, LowPassDlgProc);
   if(lpstrLoadStrBuf)
      GlobalFreePtr(lpstrLoadStrBuf);
   return TRUE;
}


/* AboutDlgProc - Dialog procedure function for ABOUTBOX dialog box.
 */
BOOL FAR PASCAL AboutDlgProc(
HWND hWnd,
UINT wMsg,
WPARAM wParam,
LPARAM lParam)
{
   switch (wMsg)
   {
   case WM_INITDIALOG:
      return TRUE;

   case WM_COMMAND:
      if (wParam == IDOK || wParam == IDCANCEL)
         EndDialog(hWnd, TRUE);
      break;
   }
   return FALSE;
}


/* LowPassDlgProc - Dialog procedure function for LOWPASSBOX dialog box.
 */
BOOL WINAPI LowPassDlgProc(
HWND hWnd,
UINT wMsg,
WPARAM wParam,
LPARAM lParam)
{
   HMENU hmenuSystem;   // system menu
   HCURSOR  ghcurSave;  // previous cursor

   switch (wMsg)
   {
   case WM_INITDIALOG:
      /* Append "About" menu item to system menu.
       */
      hmenuSystem = GetSystemMenu(hWnd, FALSE);
      AppendMenu(hmenuSystem, MF_SEPARATOR, 0, NULL);
      LoadString(ghInst, IDS_ABOUTMENUCAPTION, lpstrLoadStrBuf,
         LOADSTRBUFSIZE);
      AppendMenu(hmenuSystem, MF_STRING, IDM_ABOUT,
         lpstrLoadStrBuf);
      return TRUE;

   case WM_SYSCOMMAND:
      switch (wParam)   // WIN16 and 32 have same message packing
      {
         case IDM_ABOUT:
            /* Display "About" dialog box.
             */
            DialogBox(ghInst, "ABOUTBOX", hWnd, AboutDlgProc);
            break;
      }
      break;

   case WM_COMMAND:
      switch (wParam)
      {
         case IDOK:  // "Begin"
            /* Set "busy" cursor, filter input file, restore cursor.
             */
            ghcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));
            DoLowPass(hWnd);
            SetCursor(ghcurSave);
            break;

         case IDCANCEL: // "Done"
            EndDialog(hWnd, TRUE);
            break;
      }
      break;
   }
   return FALSE;
}
