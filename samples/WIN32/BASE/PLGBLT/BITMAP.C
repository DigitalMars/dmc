
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/**************************************************************************\
*  bitmap.c -- support for reading in and drawing bitmaps.
\**************************************************************************/

#include <windows.h>
#include <commdlg.h>

/**************************************************************************\
*
*  function:  DrawBitmap()
*
*  input parameters:  HDC, HBITMAP
*
*  Draw the bitmap into the hdc.  Source rectangle computed to include the
*   whole bitmap.  Destination location is 0,0.
*
*  global variables: none.
*
\**************************************************************************/
VOID DrawBitmap (HDC hdc, HBITMAP hbm)
{
    BOOL f;
    HDC hdcBits;
    BITMAP bm;

    hdcBits = CreateCompatibleDC(hdc);
    GetObject (hbm, sizeof(BITMAP), &bm);
    SelectObject(hdcBits,hbm);
    f = BitBlt(hdc,0,0,bm.bmWidth, bm.bmHeight,hdcBits,0,0,SRCCOPY);
    DeleteDC(hdcBits);
}



/**************************************************************************\
*
*  function:  GetBitmap()
*
*  input parameters:
*   hdc - hdc to make the bitmap compatible with.
*   hInst - instance handle
*
*  Put up a common dialog box to open a new *.BMP file.
*   Once this is complete, open the file, read in the information,
*   and create a compatible bitmap.
*
*  returns:  handle to the bitmap iff successful.  NULL otherwise.
*
\**************************************************************************/
HBITMAP GetBitmap (HDC hdc, HANDLE hInst, BOOL monochrome)
{
    HBITMAP hbm;
    PBITMAPFILEHEADER pbmfh;
    PBITMAPINFOHEADER pbmih;
    PBYTE             pBits;
    int fh;
    int bfOffBits;
    int nbytes;
    OPENFILENAME  of;
    char buffer [MAX_PATH];

    buffer[0] = 0;

    /* set up the OPENFILE structure,
     *  then use the appropriate common dialog
     */
    of.lStructSize       = sizeof (OPENFILENAME);
    of.hwndOwner         = NULL;
    of.hInstance         = hInst;
    of.lpstrFilter       = "Bitmaps\000 *.BMP\000\000";
    of.lpstrCustomFilter = NULL;
    of.nMaxCustFilter    = 0;
    of.nFilterIndex      = 0;
    of.lpstrFile         = buffer;
    of.nMaxFile          = MAX_PATH;
    of.lpstrFileTitle    = NULL;
    of.nMaxFileTitle     = 0;
    of.lpstrInitialDir   = "c:\\nt\\windows";
    of.lpstrTitle        = NULL;
    of.Flags             = OFN_HIDEREADONLY;
    of.nFileOffset       = 0;
    of.nFileExtension    = 0;
    of.lpstrDefExt       = NULL;
    of.lCustData         = 0;
    of.lpfnHook          = NULL;
    of.lpTemplateName    = NULL;
    if (!GetOpenFileName (&of)) return NULL;

    /* Try to open the file.  If successful, then allocate space for it,
     *  and read in all of the bytes.
     */
    fh = _lopen (buffer, OF_READ);
    if (fh == -1) return NULL;
    nbytes = GetFileSize ((HANDLE) fh, NULL);

    /* The contents of the file are read in here in three parts.  First
     *  the bitmap file header is read, then the bitmap header along with
     *  the color table, then finally the actual bit data.  I.e. from
     *  a total of nbytes...
     *    1.  sizeof (BITMAPFILEHEADER)
     *    2.  bfOffBits- sizeof (BITMAPFILEHEADER)
     *    3.  (nbytes - bfOffBits)
     */

    /* read in the bitmap file header.  save the offset to bits. */
    if (!(pbmfh = (PBITMAPFILEHEADER)LocalAlloc (LPTR, sizeof (BITMAPFILEHEADER))))
        return NULL;
    _lread (fh, (LPSTR)pbmfh, sizeof (BITMAPFILEHEADER));
    bfOffBits=pbmfh->bfOffBits;

    /* read in the bitmap info header and the color table right after it.
     * both BITMAPINFOHEADER and BITMAPINFO needed for CreateDIBitmap()
     */
    if (!(pbmih = (PBITMAPINFOHEADER)LocalAlloc (LPTR, bfOffBits- sizeof (BITMAPFILEHEADER))))
        return NULL;
    _lread (fh, (LPSTR)pbmih, bfOffBits- sizeof (BITMAPFILEHEADER));

    /* finally read in the bit data. */
    if (!(pBits = (PBYTE)LocalAlloc (LPTR, (nbytes - bfOffBits)))) return NULL;
    _lread (fh, (LPSTR)pBits, (nbytes - bfOffBits));


    /* in the case of desiring a monochrome bitmap (input parameter),
     *  verify that is what we got.  If it is, then use CreateBitmap,
     *  else use a device independent bitmap.
     */
    if (monochrome) {
      if (pbmih->biBitCount != 1) {
        MessageBox (NULL, "Mask must be monochrome bitmap.",
                    "Error", MB_APPLMODAL | MB_ICONSTOP | MB_OK);
        hbm = NULL;
      } else {
      hbm = CreateBitmap (pbmih->biWidth, pbmih->biHeight,
                          pbmih->biPlanes, pbmih->biBitCount,
                          pBits);
      }
    } else  /* bitmap is NOT monochrome, use DIB. */
      hbm = CreateDIBitmap (hdc, pbmih, CBM_INIT,
                          pBits, (PBITMAPINFO) pbmih, DIB_RGB_COLORS);


    /* hbm is set... free up memory, and return */
    LocalFree (LocalHandle ((LPSTR)pBits));
    LocalFree (LocalHandle ((LPSTR)pbmih));
    LocalFree (LocalHandle ((LPSTR)pbmfh));
    _lclose (fh);

    return hbm;
}
