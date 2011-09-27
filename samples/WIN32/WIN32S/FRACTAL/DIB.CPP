/****************************************************************************
 *                                                                          *
 *  MODULE  : DIB.C                                                         *
 *                                                                          *
 *  DESCRIPTION : Routines for dealing with Device Independent Bitmaps.     *
 *                                                                          *
 *  FUNCTIONS   : OpenDIB()  - Opens DIB file and creates a memory DIB      *
 *                                                                          *
 *                WriteDIB() - Writes a global handle in CF_DIB format      *
 *                             to a file.                                   *
 *                                                                          *
 *                DibInfo()  - Retrieves the info. block associated         *
 *                             with a CF_DIB format memory block.           *
 *                                                                          *
 *                CreateBIPalette() - Creates a GDI palette given a pointer *
 *                                    to a BITMAPINFO structure.            *
 *                                                                          *
 *                CreateDibPalette() - Creates a GDI palette given a HANDLE *
 *                  to a BITMAPINFO structure.                              *
 *                                                                          *
 *                ReadDibBitmapInfo() - Reads a file in DIB and returns     *
 *                                      a global handle to it's BITMAPINFO  *
 *                                                                          *
 *                PaletteSize() - Calculates the palette size in bytes      *
 *                                of given DIB                              *
 *                                                                          *
 *                DibNumColors() - Determines the number of colors in DIB   *
 *                                                                          *
 *                BitmapFromDib() - Creates a DDB given a global handle to  *
 *                                  a block in CF_DIB format.               *
 *                                                                          *
 *                DibFromBitmap() - Creates a DIB repr. the DDB passed in.  *
 *                                                                          *
 *                DrawBitmap() - Draws a bitmap at specified position       *
 *                               in the DC.                                 *
 *                                                                          *
 *                DibBlt() - Draws a bitmap in CIF_DIB format using         *
 *                           SetDIBitsToDevice()                            *
 *                                                                          *
 *                StretchDibBlt() - Draws a bitmap in CIF_DIB format using  *
 *                                  StretchDIBits()                         *
 *                                                                          *
 *                local_read() - Private routine to read more than 64k      *
 *                                                                          *
 *                local_write() - Private routine to write more than 64k    *
 *                                                                          *
 ***************************************************************************/
#define NOCOMM
#define NOKANJI
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "dib.h"

#ifdef __NT__
#define HUGE
#else
#define HUGE __huge
#endif

#if 0
static void mess(const char *s)
{
	MessageBox(NULL, s, "Message", MB_OK);
}
#else
#define mess(s)
#endif
VOID WriteMapFileHeaderandConvertFromDwordAlignToPacked(HFILE fh, LPBITMAPFILEHEADER pbf);
VOID ReadBitMapFileHeaderandConvertToDwordAlign(HFILE fh, LPBITMAPFILEHEADER pbf, LPDWORD lpdwoff);

static   HCURSOR hcurSave;
////////////////////////////////////////////////////////////////
/////////////////// Additions by tl

//////////
// Get and test bitmaps
BOOL GetAllBitmapInfo(const char * pszDib, HANDLE &hDib, 
                      HPALETTE &hpalDib, HBITMAP &hBm)
{
//	MessageBeep(MB_ICONASTERISK);
    hDib = OpenDIB((LPSTR)pszDib);
    if (hDib)
    {
        hpalDib = CreateDibPalette(hDib);
        if (hpalDib)
        {
            hBm = BitmapFromDib(hDib, hpalDib);
            if (hBm)
                return TRUE;
           	DeleteObject(hpalDib);
           	hpalDib = NULL;
        }
       	GlobalFree(hDib);
       	hDib = NULL;
    }
    return FALSE;
}

HPALETTE DuplicatePalette(HPALETTE hpal)
{
    HPALETTE hpalRet = NULL;
    if (hpal)
    {
        static BYTE byLogPalBuf[ sizeof(LOGPALETTE) + 
                                 (256*sizeof(PALETTEENTRY)) ];
        LOGPALETTE * plgpl = (LOGPALETTE *) byLogPalBuf;
        if (plgpl)
        {
            plgpl->palVersion = 0x300;
            plgpl->palNumEntries = GetPaletteEntries(hpal, 0, 256, 
                                                     &plgpl->palPalEntry[0]);
            if (plgpl->palNumEntries)
                hpalRet = CreatePalette(plgpl);
        }
    }
    return hpalRet;
}

static char buf[512];
#define db(s) OutputDebugString(s)

inline void GlobalClean(HANDLE h)
{
    while (GlobalUnlock(h))
        ;
    GlobalFree(h);
}

HANDLE SimplePaletteConform(HANDLE hDib, HPALETTE hpalConformTo)
{
    HDC hdcDesktop;
    int iBitsPixel;
    
    hdcDesktop = GetDC(NULL);
    iBitsPixel = GetDeviceCaps(hdcDesktop, BITSPIXEL);
    ReleaseDC(NULL, hdcDesktop);
    if (iBitsPixel == 8)
    {
        db("Quick conform");
        // We can very quickly conform
        HANDLE hNewDib;
        HBITMAP hBm = BitmapFromDib(hDib, hpalConformTo);
        if (hBm == NULL)
        {
            return hDib;
        }
        GlobalClean(hDib);
        hNewDib = DibFromBitmap(hBm, BI_RGB, 8, hpalConformTo);
        if (!hNewDib)
        {
            MessageBox(NULL, "Out of memory", NULL, MB_OK);
            exit(EXIT_FAILURE);
        }
        DeleteObject(hBm);
        return hNewDib;
    }
    else
    {
        // We must do it by hand, very slow
        int iSzOldPal;
        static RGBQUAD pOldColors[256];
        LPBITMAPINFOHEADER pDibInfo = (LPBITMAPINFOHEADER) GlobalLock(hDib);

        assert(pDibInfo);
        
        if (pDibInfo->biBitCount <= 4 || pDibInfo->biBitCount > 8)
        {
            GlobalUnlock(hDib);
            return hDib;
        }
        
        if (pDibInfo->biClrUsed == 0)
            iSzOldPal = 256;
        else
            iSzOldPal = int(pDibInfo->biClrUsed);

        if (hpalConformTo)
        {
            LPBITMAPINFO pDib = (LPBITMAPINFO) pDibInfo;
            static PALETTEENTRY ppe[256];

            memcpy(pOldColors, &pDib->bmiColors, sizeof(pOldColors));
            GetPaletteEntries(hpalConformTo, 0, iSzOldPal, ppe);
            for (int i = 0; i < iSzOldPal; i++)
            {
                pDib->bmiColors[i].rgbRed = ppe[i].peRed;
                pDib->bmiColors[i].rgbGreen = ppe[i].peGreen;
                pDib->bmiColors[i].rgbBlue = ppe[i].peBlue;
            }
        }
        else
        {
            GlobalUnlock(hDib);
            return 0;
        }
        
        BYTE HUGE * pDibPixels = (BYTE *) pDibInfo + pDibInfo->biSize + 
                                 PaletteSize(pDibInfo);
        long i, j;
        int iWidthBytes = (pDibInfo->biWidth % long(sizeof(long))) ?
                            (pDibInfo->biWidth + (sizeof(long) - 
                                (pDibInfo->biWidth % long(sizeof(long))))) :
                            pDibInfo->biWidth;
        long lScanlineIndex;
        int iOldColorIndex;
        if (pDibInfo->biCompression == BI_RGB)
        {
            MSG msg;
            StartWait();
            for (i = 0; i < pDibInfo->biHeight; i++)
            {
                lScanlineIndex = i * long(iWidthBytes);
                for (j = 0; j < pDibInfo->biWidth; j++)
                {
                    iOldColorIndex = pDibPixels[lScanlineIndex + j];
                    pDibPixels[lScanlineIndex + j] = 
                        GetNearestPaletteIndex(hpalConformTo,
                            RGB(pOldColors[iOldColorIndex].rgbRed,
                                pOldColors[iOldColorIndex].rgbGreen,
                                pOldColors[iOldColorIndex].rgbBlue)
                            );
                }
                EndWait();
                PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
                StartWait();
            }
            EndWait();
            GlobalUnlock(hDib);
            return hDib;
        }
        else /* BI_RLE8 */
        {
            MessageBox(NULL, "No support for RLE bitmaps.", NULL, MB_OK);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

enum eDibColorDepth {
    depth2 = 1,
    depth16 = 4,
    depth256 = 8,
    depth16mil = 24
};

HANDLE IncreaseDibColors(HANDLE hDib, eDibColorDepth newDepth)
{
    if (!hDib)
        return hDib;
        
    LPBITMAPINFOHEADER pDibInfo = (LPBITMAPINFOHEADER) GlobalLock(hDib);
    
    if (pDibInfo == NULL)
    {
        MessageBox(NULL, "Out of memory.", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    
    if (pDibInfo->biBitCount >= int(newDepth))
    {
        GlobalUnlock(hDib);
        return hDib;
    }
    
    if (newDepth != depth16mil)
    {
        MessageBox(NULL, "Only color increase to 24-bit supported\n", NULL, MB_OK);
        GlobalUnlock(hDib);
        return hDib;
    }
    
    ////////
    BYTE HUGE * pDibPixels = (BYTE *) pDibInfo + pDibInfo->biSize + 
                             PaletteSize(pDibInfo);
    HGLOBAL hNewDib = GlobalAlloc(GPTR, sizeof(BITMAPINFOHEADER) + 
                                        PaletteSize(pDibInfo) +
                                        long(3L * long(pDibInfo->biWidth) * 
                                             long(pDibInfo->biHeight))
                                 );
    if (hNewDib == NULL)
    {
        MessageBox(NULL, "Out of memory.", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    
    LPBITMAPINFOHEADER pNewDibInfo = (LPBITMAPINFOHEADER) GlobalLock(hNewDib);

    if (pNewDibInfo == NULL)
    {
        MessageBox(NULL, "Out of memory.", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    
    // Copy header and re-use old color palette as optimized palette
    memcpy(pNewDibInfo, pDibInfo, pDibInfo->biSize + PaletteSize(pDibInfo));
    
    pNewDibInfo->biBitCount = 24;
    pNewDibInfo->biCompression = BI_RGB;
    pNewDibInfo->biSizeImage = 0;

    // Copy image
    BYTE HUGE * pNewDibPixels = (BYTE *) pNewDibInfo + pDibInfo->biSize + 
                           PaletteSize(pNewDibInfo);
    
    if (pDibInfo->biBitCount == 4)
    {
        MessageBox(NULL, "16 color bitmaps not supported.", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    else if (pDibInfo->biCompression == BI_RLE8)
    {
        MessageBox(NULL, "Run length encoded bitmaps not supported at this time", NULL, MB_OK);
        exit(EXIT_FAILURE);
    }
    else
    {
        RGBQUAD * pColors = (RGBQUAD *)((LPSTR) pDibInfo + pDibInfo->biSize);
        for (unsigned long i = 0; i <= (long(pDibInfo->biWidth) * 
                                        long(pDibInfo->biHeight)); i++)
        {
            pNewDibPixels[0] = pColors[pDibPixels[i]].rgbBlue;
            pNewDibPixels[1] = pColors[pDibPixels[i]].rgbGreen;
            pNewDibPixels[2] = pColors[pDibPixels[i]].rgbRed;
            pNewDibPixels += 3;
        }
    }
    
    GlobalUnlock(hNewDib);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    return hNewDib;
}

// Add system colors to start of palette, fill the rest with what's in
// hpal, use bitmap to identify important colors if there's not enough
// space.


#define LimitPaletteSize            256
#define SYSSTATICPALSZ     20
static int _cdecl PalEntCmp(const void * p1, const void * p2)
{
/*
    The RGB macro is defined in WINDOWS.H as follows: 
    #define RGB(r,g,b)   ((COLORREF)(((BYTE)(r)|((WORD)(g)<<8))| \
        (((DWORD)(BYTE)(b))<<16)))
*/

    PALETTEENTRY *ppe1 = (PALETTEENTRY *) p1, *ppe2 = (PALETTEENTRY *)p2;
    COLORREF colPe1 = RGB(ppe1->peRed, ppe1->peGreen, ppe1->peBlue);
    COLORREF colPe2 = RGB(ppe2->peRed, ppe2->peGreen, ppe2->peBlue);
    return (colPe1 == colPe2) ? 0 :
           (colPe1 > colPe2)  ? 1 :
                                -1;
}

// Eliminate entries in param 1 found in param 2, eliminate
// redundant entries
static void ReducePalette(LOGPALETTE *plgplReduce, 
                          LOGPALETTE *plgplReduceWith)
{
    mess("ReducePalette");
    PALETTEENTRY 
        *ppeEndPal = &plgplReduce->palPalEntry[plgplReduce->palNumEntries - 1],
        *ppeBasePal = &plgplReduce->palPalEntry[0];
        
    // Sort target
    qsort(&plgplReduce->palPalEntry[0], plgplReduce->palNumEntries, 
          sizeof(PALETTEENTRY), PalEntCmp);
    // Remove colors specified in param 2
    if (plgplReduceWith)
    {
        PALETTEENTRY *ppeSearchRes;
        for (int i = 0; i < plgplReduceWith->palNumEntries; i++)
        {
            while (1)
            {
                ppeSearchRes = (PALETTEENTRY *) 
                				bsearch(&plgplReduceWith->palPalEntry[i],
                                       ppeBasePal, 
                                       size_t(ppeEndPal - ppeBasePal) + 1,
                                       sizeof(PALETTEENTRY),
                                       PalEntCmp);
                if (ppeSearchRes != NULL)
                {
                    memmove(ppeSearchRes, ppeSearchRes + 1, 
                            ppeEndPal - ppeSearchRes);
                    ppeEndPal--;
                }
                else
                    break;
            }
        }
    }
    // remove reduntant entries in palette
    {
        PALETTEENTRY *ppe1, *ppe2;
        ppe1 = &plgplReduce->palPalEntry[0];
        ppe2 = &plgplReduce->palPalEntry[1];
        
        while (ppe2 <= ppeEndPal)
        {
            if (RGB(ppe1->peRed, ppe1->peGreen, ppe1->peBlue) ==
                RGB(ppe2->peRed, ppe2->peGreen, ppe2->peBlue))
            {
                memmove(ppe1, ppe2, ppeEndPal - ppe1);
                ppeEndPal--;
            }
            else
            {
                ppe1++;
                ppe2++;
            }
        }
        if (ppeEndPal < 
            &plgplReduce->palPalEntry[plgplReduce->palNumEntries - 1])
        {
            memset(ppeEndPal + 1, 0, 
                   (&plgplReduce->palPalEntry[plgplReduce->palNumEntries - 1] 
                    - ppeEndPal) * sizeof(PALETTEENTRY));
            plgplReduce->palNumEntries = (ppeEndPal - ppeBasePal) + 1;
        }
        else
            assert(0);
    }
}

HPALETTE CreateOptimized256Palette(HPALETTE hpal, HBITMAP hBmForColorQuant)
{
    mess("CreateOptimized256Palette");
    // Prepare our logical palette structures
    LOGPALETTE *pNewPal, *pOldPal;
    HPALETTE hpalNew = NULL, hpalSystem;
    
    pNewPal= (LOGPALETTE *) calloc(sizeof(LOGPALETTE) + 
                                   (LimitPaletteSize * sizeof(PALETTEENTRY)), 
                                   1);
    if (!pNewPal)
        return NULL;
        
    pOldPal= (LOGPALETTE *) calloc(sizeof(LOGPALETTE) + 
                                   (LimitPaletteSize * sizeof(PALETTEENTRY)),
                                   1);
    if (!pOldPal)
    {
        free(pNewPal);
        return NULL;
    }
    
    // Fill initial entries of new with system colors
    hpalSystem = GetStockObject(DEFAULT_PALETTE);
    if (GetPaletteEntries(hpalSystem, 0, SYSSTATICPALSZ, 
                          &pNewPal->palPalEntry[0]) != 
        SYSSTATICPALSZ)
        assert(0);
    pNewPal->palVersion    = 0x300;
    pNewPal->palNumEntries = SYSSTATICPALSZ;

    // Get old colors
    if (GetPaletteEntries(hpal, 0, LimitPaletteSize, &pOldPal->palPalEntry[0])
        < 2)
        assert(0);
    pOldPal->palVersion    = 0x300;
    pOldPal->palNumEntries = LimitPaletteSize;
    
    ReducePalette(pOldPal, pNewPal);
    
#if 0   /***** UNIMPLEMENTED PORTION *****/
    // If we don't have enough slots and we have a bitmap, count frequencies
    // and fill with most frequent unaccounted for
    if ((pOldPal->palNumEntries > (LimitPaletteSize - SYSSTATICPALSZ))
        && hBmForColorQuant)
    {
    }
    // else, just fill from bottom up.
    else
#endif
    {
        memcpy(&pNewPal->palPalEntry[SYSSTATICPALSZ], 
                &pOldPal->palPalEntry[0],
                (LimitPaletteSize - SYSSTATICPALSZ) * 
                 sizeof(PALETTEENTRY));
        pNewPal->palNumEntries += 
            (pOldPal->palNumEntries > (LimitPaletteSize - SYSSTATICPALSZ)) ?
                (LimitPaletteSize - SYSSTATICPALSZ) :
                pOldPal->palNumEntries;
    }
    
    free(pOldPal);
    hpalNew = CreatePalette(pNewPal);
    free(pNewPal);

    return hpalNew;
}

// Create a unified palette with the properies of an optimized palette, plus
// any available slots filled with colors from the submissive

HPALETTE CreateUnified256Palette(HPALETTE hpalDominant, 
                                 HPALETTE hpalSubmissive,
                                 HBITMAP  hBmForDominantColorQuant,
                                 HBITMAP  hBmForSubmissiveColorQuant)
{
    return DuplicatePalette(hpalDominant);
    mess("CreateUnified256Palette");
    // Get optimized palette for dominant
    HPALETTE hpalNew = NULL;
    HPALETTE hpalTemp1 =
        CreateOptimized256Palette(hpalDominant, hBmForDominantColorQuant);
    
    if (hpalTemp1)
    {
        HPALETTE hpalTemp2 = 
            CreateOptimized256Palette(hpalSubmissive, 
                                      hBmForSubmissiveColorQuant);
        if (hpalTemp2)
        {
            LOGPALETTE *pDomPal, *pSubPal;
            pDomPal = 
                (LOGPALETTE *)calloc(sizeof(LOGPALETTE) + 
                                     (LimitPaletteSize * sizeof(PALETTEENTRY)),
                                     1);
            if (pDomPal)
            {
        
                pSubPal= (LOGPALETTE *) calloc(sizeof(LOGPALETTE) + 
                                   (LimitPaletteSize * sizeof(PALETTEENTRY)),
                                   1);
                if (pSubPal)
                {
                    if (GetPaletteEntries(hpalTemp1, 0, LimitPaletteSize, 
                                          &pDomPal->palPalEntry[0]) < 22)
                        assert(0);
                    PALETTEENTRY *ppeInsertPoint;
                    ppeInsertPoint = &pDomPal->palPalEntry[20];
                    while ((ppeInsertPoint->peRed ||
                           ppeInsertPoint->peGreen ||
                           ppeInsertPoint->peBlue) &&
                           ppeInsertPoint < &pDomPal->palPalEntry[255])
                        ppeInsertPoint++;
                    
                    if (ppeInsertPoint >= &pDomPal->palPalEntry[255])
                    {
                        hpalNew = hpalTemp1;
                        hpalTemp2 = NULL;
                    }
                    else
                    {
                        /*** Optimized color choice unimplemented ***/
                        if (GetPaletteEntries(hpalTemp2, 0, LimitPaletteSize, 
                            &pSubPal->palPalEntry[0]) < 20)
                            assert(0);
                        PALETTEENTRY * ppeSubScan = &pSubPal->palPalEntry[20];
                        while (ppeInsertPoint <= &pDomPal->palPalEntry[255])
                        {
                            *ppeInsertPoint = *ppeSubScan;
                            ppeInsertPoint++;
                            ppeSubScan++;
                        }
                        pDomPal->palVersion = 0x300;
                        pDomPal->palNumEntries = 256;
                        hpalNew = CreatePalette(pDomPal);
                    }
                    free(pSubPal);
                }       
                free(pDomPal);
            }
            if (hpalTemp2)
                DeleteObject(hpalTemp2);
            
        }
        DeleteObject(hpalTemp1);
    }
    
    // Eliminate submissive colors accounted for in system palette
    return hpalNew;
}
////////////////////////////////////////////////////////////////

#define MAXREAD  32768                                   /* Number of bytes to be read during */
                                                                 /* each read operation.                                 */
 /************* PRIVATE ROUTINES TO READ/WRITE MORE THAN 64K ***************/
/****************************************************************************
 *                                      *
 *  FUNCTION   : local_read(int fh, VOID FAR *pv, DWORD ul)             *
 *                                      *
 *  PURPOSE    : Reads data in steps of 32k till all the data has been read.*
 *                                      *
 *  RETURNS    : 0 - If read did not proceed correctly.             *
 *       number of bytes read otherwise.                *
 *                                      *
 ****************************************************************************/

static DWORD PASCAL local_read( int       fh,
                VOID far      *pv,
                DWORD         ul)
{
    DWORD     ulT = ul;
    BYTE huge *hp = (BYTE huge *) pv;

    while (ul > (DWORD) MAXREAD) {
        if (_lread(fh, (LPSTR) hp, (WORD) MAXREAD) != MAXREAD)
            return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
        }
    if (_lread(fh, (LPSTR) hp, (WORD) ul) != (WORD) ul)
        return 0;
    return ulT;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : local_write(int fh, VOID FAR *pv, DWORD ul)                *
 *                                      *
 *  PURPOSE    : Writes data in steps of 32k till all the data is written.  *
 *                                      *
 *  RETURNS    : 0 - If write did not proceed correctly.            *
 *       number of bytes written otherwise.             *
 *                                      *
 ****************************************************************************/

static DWORD PASCAL local_write(int      fh,
                VOID FAR     *pv,
                DWORD        ul)
{
    DWORD     ulT = ul;
    BYTE huge *hp = (BYTE huge *) pv;

    while (ul > MAXREAD) {
        if (_lwrite(fh, (LPSTR) hp, (WORD) MAXREAD) != MAXREAD)
            return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
        }
    if (_lwrite(fh, (LPSTR) hp, (WORD) ul) != (WORD) ul)
        return 0;
    return ulT;
}

/****************************************************************************
 *                                                                          *
 *  FUNCTION   :OpenDIB(LPSTR szFile)                                       *
 *                                                                          *
 *  PURPOSE    :Open a DIB file and create a MEMORY DIB, a memory handle    *
 *      containing BITMAPINFO, palette data and the bits.                   *
 *                                                                          *
 *  RETURNS    :A handle to the DIB.                                        *
 *                                                                          *
 ****************************************************************************/

HANDLE OpenDIB(LPSTR szFile)
{
    unsigned        fh;
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER  lpbi;
    DWORD       dwLen = 0;
    DWORD       dwBits;
    HANDLE      hdib;
    HANDLE     h;
    OFSTRUCT   of;

    /* Open the file and read the DIB information */
    fh = OpenFile(szFile, &of, OF_READ);
    if (fh == -1)
        return NULL;

    hdib = ReadDibBitmapInfo(fh);
    if (!hdib)
        return NULL;
    DibInfo(hdib, &bi);

    /* Calculate the memory needed to hold the DIB */
    dwBits = bi.biSizeImage;
    dwLen = bi.biSize + (DWORD) PaletteSize(&bi) + dwBits;

    /* Try to increase the size of the bitmap info. buffer to hold the DIB */
    h = GlobalReAlloc(hdib, dwLen, GHND);
    if (!h) {
        GlobalFree(hdib);
        hdib = NULL;
        }
    else
        hdib = h;

    /* Read in the bits */
    if (hdib) {
        lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);
        local_read(fh, (LPSTR) lpbi + (WORD) lpbi->biSize + PaletteSize(lpbi), dwBits);
        GlobalUnlock(hdib);
        }
    _lclose(fh);

    return hdib;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : WriteDIB(LPSTR szFile,HANDLE hdib)             *
 *                                      *
 *  PURPOSE    : Write a global handle in CF_DIB format to a file.      *
 *                                      *
 *  RETURNS    : TRUE  - if successful.                     *
 *       FALSE - otherwise                      *
 *                                      *
 ****************************************************************************/

BOOL WriteDIB(LPSTR szFile, HANDLE hdib)
{
    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    int         fh;
    OFSTRUCT        of;

    if (!hdib)
        return FALSE;

    fh = OpenFile(szFile, &of, OF_CREATE | OF_READWRITE);
    if (fh == -1)
        return FALSE;

    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    /* Fill in the fields of the file header */
    hdr.bfType = BFT_BITMAP;
    hdr.bfSize = GlobalSize(hdib) + ((UINT)SIZEOF_BITMAPFILEHEADER_PACKED);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = (DWORD) ((UINT)SIZEOF_BITMAPFILEHEADER_PACKED) + lpbi->biSize +
         PaletteSize(lpbi);

    /* Write the file header */
#ifdef __NT__
	WriteMapFileHeaderandConvertFromDwordAlignToPacked(fh, &hdr);
#else
    _lwrite(fh, (LPSTR) &hdr, ((UINT)SIZEOF_BITMAPFILEHEADER_PACKED));
#endif

    /* Write the DIB header and the bits */
    local_write(fh, (LPSTR) lpbi, GlobalSize(hdib));

    GlobalUnlock(hdib);
    _lclose(fh);
    return TRUE;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : DibInfo(HANDLE hbi,LPBITMAPINFOHEADER lpbi)            *
 *                                      *
 *  PURPOSE    : Retrieves the DIB info associated with a CF_DIB        *
 *       format memory block.                       *
 *                                      *
 *  RETURNS    : TRUE  - if successful.                     *
 *       FALSE - otherwise                      *
 *                                      *
 ****************************************************************************/

BOOL DibInfo(HANDLE hbi, LPBITMAPINFOHEADER lpbi)
{
    if (hbi) {
        *lpbi = *(LPBITMAPINFOHEADER) GlobalLock(hbi);

    /* fill in the default fields */
        if (lpbi->biSize != sizeof(BITMAPCOREHEADER)) {
            if (lpbi->biSizeImage == 0L)
                lpbi->biSizeImage =
                     WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

            if (lpbi->biClrUsed == 0L)
                lpbi->biClrUsed = DibNumColors(lpbi);
            }
        GlobalUnlock(hbi);
        return TRUE;
        }
    return FALSE;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : CreateBIPalette(LPBITMAPINFOHEADER lpbi)           *
 *                                      *
 *  PURPOSE    : Given a Pointer to a BITMAPINFO struct will create a       *
 *       a GDI palette object from the color table.         *
 *                                      *
 *  RETURNS    : A handle to the palette.                   *
 *                                      *
 ****************************************************************************/

HPALETTE CreateBIPalette(LPBITMAPINFOHEADER lpbi)
{
    LOGPALETTE      _near *pPal;
    HPALETTE        hpal = NULL;
    WORD            nNumColors;
    BYTE            red;
    BYTE            green;
    BYTE            blue;
    unsigned         i;
    RGBQUAD        FAR *pRgb;

    if (!lpbi)
        return NULL;

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
        return NULL;

    /* Get a pointer to the color table and the number of colors in it */
    pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + (WORD) lpbi->biSize);
    nNumColors = DibNumColors(lpbi);

    if (nNumColors) {
    /* Allocate for the logical palette structure */
        pPal = (LOGPALETTE _near *) LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion = PALVERSION;

    /* Fill in the palette entries from the DIB color table and
     * create a logical color palette.
     */
        for (i = 0;  i < nNumColors;  i++) {
            pPal->palPalEntry[i].peRed = pRgb[i].rgbRed;
            pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
            pPal->palPalEntry[i].peBlue = pRgb[i].rgbBlue;
            pPal->palPalEntry[i].peFlags = (BYTE) 0;
            }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE) pPal);
        }
    else if (lpbi->biBitCount == 24) {
    /* A 24 bitcount DIB has no color table entries so, set the number of
     * to the maximum value (256).
     */
        nNumColors = MAXPALETTE;
        pPal = (LOGPALETTE _near *) LocalAlloc(LPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion = PALVERSION;

        red = green = blue = 0;

    /* Generate 256 (= 8*8*4) RGB combinations to fill the palette
     * entries.
     */
        for (i = 0;  i < pPal->palNumEntries;  i++) {
            pPal->palPalEntry[i].peRed = red;
            pPal->palPalEntry[i].peGreen = green;
            pPal->palPalEntry[i].peBlue = blue;
            pPal->palPalEntry[i].peFlags = (BYTE) 0;

            if (!(red += 32))
                if (!(green += 32))
                    blue += 64;
            }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE) pPal);
        }
    return hpal;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : CreateDibPalette(HANDLE hbi)                   *
 *                                      *
 *  PURPOSE    : Given a Global HANDLE to a BITMAPINFO Struct           *
 *       will create a GDI palette object from the color table.     *
 *       (BITMAPINFOHEADER format DIBs only)                     *
 *                                      *
 *  RETURNS    : A handle to the palette.                   *
 *                                      *
 ****************************************************************************/

HPALETTE CreateDibPalette(HANDLE hbi)
{
    HPALETTE hpal;

    if (!hbi)
        return NULL;
    hpal = CreateBIPalette((LPBITMAPINFOHEADER) GlobalLock(hbi));
    GlobalUnlock(hbi);
    return hpal;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : ReadDibBitmapInfo(int fh)                  *
 *                                      *
 *  PURPOSE    : Will read a file in DIB format and return a global HANDLE  *
 *       to it's BITMAPINFO.  This function will work with both     *
 *       "old" (BITMAPCOREHEADER) and "new" (BITMAPINFOHEADER)      *
 *       bitmap formats, but will always return a "new" BITMAPINFO  *
 *                                      *
 *  RETURNS    : A handle to the BITMAPINFO of the DIB in the file.     *
 *                                      *
 ****************************************************************************/

HANDLE ReadDibBitmapInfo(int fh)
{
    DWORD     off;
    HANDLE    hbi = NULL;
    int   size;
    int   i;
    WORD      nNumColors;

    RGBQUAD FAR   *pRgb;
    BITMAPINFOHEADER   bi;
    BITMAPCOREHEADER   bc;
    LPBITMAPINFOHEADER lpbi;
    BITMAPFILEHEADER   bf;
    DWORD          dwWidth = 0;
    DWORD          dwHeight = 0;
    WORD           wPlanes, wBitCount;

    if (fh == -1)
        return NULL;
#ifdef __NT__
	ReadBitMapFileHeaderandConvertToDwordAlign(fh, &bf, &off);
	/* at this point we have read the file into bf*/
#else
    /* Reset file pointer and read file header */
    off = _llseek(fh, 0L, SEEK_CUR);
    if (sizeof (bf) != _lread(fh, (LPSTR) &bf, sizeof (bf)))
        return FALSE;
#endif

    /* Do we have a RC HEADER? */
    if (! ISDIB (bf.bfType)) {
        bf.bfOffBits = 0L;
        _llseek(fh, off, SEEK_SET);
        }
    if (sizeof (bi) != _lread(fh, (LPSTR) &bi, sizeof (bi)))
        return FALSE;

    nNumColors = DibNumColors(&bi);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
     * and extract the field information accordingly. If a BITMAPCOREHEADER,
     * transfer it's field information to a BITMAPINFOHEADER-style block
     */
    switch (size = (int) bi.biSize) {
  case sizeof(BITMAPINFOHEADER):
        break;

  case sizeof(BITMAPCOREHEADER):

        bc = *(BITMAPCOREHEADER *) &bi;

        dwWidth = (DWORD) bc.bcWidth;
        dwHeight = (DWORD) bc.bcHeight;
        wPlanes = bc.bcPlanes;
        wBitCount = bc.bcBitCount;

        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = dwWidth;
        bi.biHeight = dwHeight;
        bi.biPlanes = wPlanes;
        bi.biBitCount = wBitCount;

        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = nNumColors;
        bi.biClrImportant = nNumColors;

        _llseek(fh, (LONG) sizeof(BITMAPCOREHEADER) -
             sizeof(BITMAPINFOHEADER), SEEK_CUR);
        break;

  default:
        /* Not a DIB! */
        return NULL;
        }

    /*  Fill in some default values if they are zero */
    if (bi.biSizeImage == 0) {
        bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount)
              * bi.biHeight;
        }
    if (bi.biClrUsed == 0)
        bi.biClrUsed = DibNumColors(&bi);

    /* Allocate for the BITMAPINFO structure and the color table. */
    hbi = GlobalAlloc(GHND, (LONG) bi.biSize + nNumColors * sizeof(RGBQUAD));
    if (!hbi)
        return NULL;
    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hbi);
    *lpbi = bi;

    /* Get a pointer to the color table */
    pRgb = (RGBQUAD FAR *) ((LPSTR) lpbi + bi.biSize);
    if (nNumColors) {
        if (size == sizeof(BITMAPCOREHEADER)) {
        /* Convert a old color table (3 byte RGBTRIPLEs) to a new
         * color table (4 byte RGBQUADs)
         */
            _lread(fh, (LPSTR) pRgb, nNumColors * sizeof(RGBTRIPLE));

            for (i = nNumColors - 1;  i >= 0;  i--) {
                RGBQUAD rgb;

                rgb.rgbRed = ((RGBTRIPLE FAR *) pRgb)[i].rgbtRed;
                rgb.rgbBlue = ((RGBTRIPLE FAR *) pRgb)[i].rgbtBlue;
                rgb.rgbGreen = ((RGBTRIPLE FAR *) pRgb)[i].rgbtGreen;
                rgb.rgbReserved = (BYTE) 0;

                pRgb[i] = rgb;
                }
            }
        else
            _lread(fh, (LPSTR) pRgb, nNumColors * sizeof(RGBQUAD));
        }

    if (bf.bfOffBits != 0L)
        _llseek(fh, off + bf.bfOffBits, SEEK_SET);

    GlobalUnlock(hbi);
    return hbi;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   :  PaletteSize(VOID FAR * pv)                    *
 *                                      *
 *  PURPOSE    :  Calculates the palette size in bytes. If the info. block  *
 *        is of the BITMAPCOREHEADER type, the number of colors is  *
 *        multiplied by 3 to give the palette size, otherwise the   *
 *        number of colors is multiplied by 4.                              *
 *                                      *
 *  RETURNS    :  Palette size in number of bytes.              *
 *                                      *
 ****************************************************************************/

WORD PaletteSize(VOID FAR * pv)
{
    LPBITMAPINFOHEADER lpbi;
    WORD           NumColors;

    lpbi = (LPBITMAPINFOHEADER) pv;
    NumColors = DibNumColors(lpbi);

    if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
        return NumColors * sizeof(RGBTRIPLE);
    else
        return NumColors * sizeof(RGBQUAD);
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : DibNumColors(VOID FAR * pv)                    *
 *                                      *
 *  PURPOSE    : Determines the number of colors in the DIB by looking at   *
 *       the BitCount filed in the info block.              *
 *                                      *
 *  RETURNS    : The number of colors in the DIB.               *
 *                                      *
 ****************************************************************************/

WORD DibNumColors(VOID FAR * pv)
{
    int         bits;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPCOREHEADER  lpbc;

    lpbi = ((LPBITMAPINFOHEADER) pv);
    lpbc = ((LPBITMAPCOREHEADER) pv);

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)) {
        if (lpbi->biClrUsed != 0)
            return (WORD) lpbi->biClrUsed;
        bits = lpbi->biBitCount;
        }
    else
        bits = lpbc->bcBitCount;

    switch (bits) {
  case 1:
        return 2;
  case 4:
        return 16;
  case 8:
        return 256;
  default:
        /* A 24 bitcount DIB has no color table */
        return 0;
        }
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : DibFromBitmap()                        *
 *                                      *
 *  PURPOSE    : Will create a global memory block in DIB format that       *
 *       represents the Device-dependent bitmap (DDB) passed in.    *
 *                                      *
 *  RETURNS    : A handle to the DIB                        *
 *                                      *
 ****************************************************************************/

HANDLE DibFromBitmap(   HBITMAP      hbm,
            DWORD        biStyle,
            WORD         biBits,
            HPALETTE     hpal)
{
    BITMAP           bm;
    BITMAPINFOHEADER     bi;
    BITMAPINFOHEADER FAR *lpbi;
    DWORD            dwLen;
    HANDLE           hdib;
    HANDLE           h;
    HDC          hdc;

    if (!hbm)
        return NULL;

    if (hpal == NULL)
        hpal = GetStockObject(DEFAULT_PALETTE);

    GetObject(hbm, sizeof (bm), (LPSTR) &bm);

    if (biBits == 0)
        biBits = bm.bmPlanes * bm.bmBitsPixel;

    //If we have a 3-bit or 16-bit driver, fix things up -- mh 6/91

    if(biBits==3)
        biBits=4;
    else if(biBits==16)
        biBits=24;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = biBits;
    bi.biCompression = biStyle;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwLen = bi.biSize + PaletteSize(&bi);

    hdc = GetDC(NULL);
    hpal = SelectPalette(hdc, hpal, FALSE);
    RealizePalette(hdc);

    hdib = GlobalAlloc(GHND, dwLen);

    if (!hdib) {
        SelectPalette(hdc, hpal, FALSE);
        ReleaseDC(NULL, hdc);
        return NULL;
        }

    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    *lpbi = bi;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
    GetDIBits(hdc, hbm, 0, (WORD) bi.biHeight,
         NULL, (LPBITMAPINFO) lpbi, DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bi.biSizeImage == 0) {
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

        if (biStyle != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
        }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bi.biSize + PaletteSize(&bi) + bi.biSizeImage;
    if ((h = GlobalReAlloc(hdib, dwLen, 0)) != NULL)
        hdib = h;
    else {
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc, hpal, FALSE);
        ReleaseDC(NULL, hdc);
        return hdib;
        }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    if (GetDIBits(hdc,
         hbm,
         0,
         (WORD) bi.biHeight,
         (LPSTR) lpbi + (WORD) lpbi->biSize + PaletteSize(lpbi),
         (LPBITMAPINFO) lpbi, DIB_RGB_COLORS) == 0) {
        GlobalUnlock(hdib);
        hdib = NULL;
        SelectPalette(hdc, hpal, FALSE);
        ReleaseDC(NULL, hdc);
        return NULL;
        }

    bi = *lpbi;
    GlobalUnlock(hdib);

    SelectPalette(hdc, hpal, FALSE);
    ReleaseDC(NULL, hdc);
    return hdib;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : BitmapFromDib(HANDLE hdib, HPALETTE hpal)          *
 *                                      *
 *  PURPOSE    : Will create a DDB (Device Dependent Bitmap) given a global *
 *       handle to a memory block in CF_DIB format          *
 *                                      *
 *  RETURNS    : A handle to the DDB.                       *
 *                                      *
 ****************************************************************************/

HBITMAP BitmapFromDib(HANDLE     hdib,
              HPALETTE   hpal)
{
    LPBITMAPINFOHEADER  lpbi;
    HPALETTE        hpalT;
    HDC         hdc;
    HBITMAP     hbm;

    StartWait();

    if (!hdib)
        return NULL;

    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    if (!lpbi)
        return NULL;

    hdc = GetDC(NULL);

    if (hpal) {
        //MessageBeep(MB_ICONASTERISK);
        hpalT = SelectPalette(hdc, hpal, FALSE);
        RealizePalette(hdc);    // GDI Bug...????
        }

    //MessageBeep(MB_ICONASTERISK);
    hbm = CreateDIBitmap(hdc,
         (LPBITMAPINFOHEADER) lpbi,
         (LONG) CBM_INIT,
         (BYTE *) lpbi + lpbi->biSize + PaletteSize(lpbi),
         (LPBITMAPINFO) lpbi,
         DIB_RGB_COLORS);

    if (hpal)
        SelectPalette(hdc, hpalT, FALSE);

    ReleaseDC(NULL, hdc);
    GlobalUnlock(hdib);

    EndWait();

    return hbm;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : DrawBitmap(HDC hdc, int x, int y, HBITMAP hbm, DWORD rop)  *
 *                                      *
 *  PURPOSE    : Draws bitmap <hbm> at the specifed position in DC <hdc>    *
 *                                      *
 *  RETURNS    : Return value of BitBlt()                   *
 *                                      *
 ****************************************************************************/

BOOL DrawBitmap(HDC    hdc,
        int    x,
        int    y,
        HBITMAP    hbm,
        DWORD      rop)
{
    HDC   hdcBits;
    BITMAP    bm;
    HPALETTE  hpalT;
    BOOL      f;

    if (!hdc || !hbm)
        return FALSE;

    hdcBits = CreateCompatibleDC(hdc);
    GetObject(hbm, sizeof(BITMAP), (LPSTR) &bm);
    SelectObject(hdcBits, hbm);
    f = BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcBits, 0, 0, rop);
    DeleteDC(hdcBits);

    return f;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : DibBlt( HDC hdc,                       *
 *           int x0, int y0,                    *
 *           int dx, int dy,                    *
 *           HANDLE hdib,                       *
 *           int x1, int y1,                    *
 *           LONG rop)                      *
 *                                      *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using SetDIBits to device.*
 *       taking the same parameters as BitBlt().            *
 *                                      *
 *  RETURNS    : TRUE  - if function succeeds.                  *
 *       FALSE - otherwise.                     *
 *                                      *
 ****************************************************************************/

BOOL DibBlt(    HDC    hdc,
        int    x0,
        int    y0,
        int    dx,
        int    dy,
        HANDLE     hdib,
        int    x1,
        int    y1,
        LONG       rop)
{
    LPBITMAPINFOHEADER   lpbi;
    HPALETTE         hpal, hpalT;
    LPSTR        pBuf;
    HDC          hdcMem;
    HBITMAP      hbm, hbmT;

    if (!hdib)
        return PatBlt(hdc, x0, y0, dx, dy, rop);

    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR) lpbi + (WORD) lpbi->biSize + PaletteSize(lpbi);
    SetDIBitsToDevice(hdc, x0, y0, dx, dy,
         x1, y1,
         x1,
         dy,
         pBuf, (LPBITMAPINFO) lpbi,
         DIB_RGB_COLORS);

    GlobalUnlock(hdib);
    return TRUE;
}

/****************************************************************************
 *                                      *
 *  FUNCTION   : StretchDibBlt( HDC hdc,                    *
 *              int x, int y,                   *
 *              int dx, int dy,                 *
 *              HANDLE hdib,                    *
 *              int x0, int y0,                 *
 *              int dx0, int dy0,               *
 *              LONG rop)                   *
 *                                      *
 *  PURPOSE    : Draws a bitmap in CF_DIB format, using StretchDIBits()     *
 *       taking the same parameters as StretchBlt().            *
 *                                      *
 *  RETURNS    : TRUE  - if function succeeds.                  *
 *       FALSE - otherwise.                     *
 *                                      *
 ****************************************************************************/

BOOL StretchDibBlt( HDC hdc,
            int x,
            int y,
            int dx,
            int dy,
            HANDLE  hdib,
            int x0,
            int y0,
            int dx0,
            int dy0,
            LONG    rop)
{
    LPBITMAPINFOHEADER lpbi;
    LPSTR        pBuf;
    BOOL         f;

    if (!hdib)
        return PatBlt(hdc, x, y, dx, dy, rop);

    lpbi = (BITMAPINFOHEADER FAR *) GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR) lpbi + (WORD) lpbi->biSize + PaletteSize(lpbi);

    f = StretchDIBits(hdc,
         x, y,
         dx, dy,
         x0, y0,
         dx0, dy0,
         pBuf, (LPBITMAPINFO) lpbi,
         DIB_RGB_COLORS,
         rop);

    GlobalUnlock(hdib);
    return f;
}

/****************************************************************************
 *									    *
 *  FUNCTION   : ReadBitMapFileHeaderandConvertToDwordAlign(HFILE fh, LPBITMAPFILEHEADER pbf)
 *									    *
 *  PURPOSE    : read file header (which is packed) and convert into unpacked BITMAPFILEHEADER strucutre
 *									    *
 *  RETURNS    : VOID
 *									    *
 ****************************************************************************/

VOID ReadBitMapFileHeaderandConvertToDwordAlign(HFILE fh, LPBITMAPFILEHEADER pbf, LPDWORD lpdwoff)
{
	DWORD off;

	off = _llseek(fh, 0L, (UINT) SEEK_CUR);
	*lpdwoff = off;

/* 		BITMAPFILEHEADER STRUCUTURE is as follows 
 *		BITMAPFILEHEADER
 *   		WORD  	bfType 
 >          ....		  <	add WORD if packed here!
 *   		DWORD	bfSize 
 *   		WORD	bfReserved1
 *   		WORD	bfReserved2
 *   		DWORD	bfOffBits 
 *			This is the packed format, unpacked adds a WORD after bfType
 */

	/* read in bfType*/
	_lread(fh, (LPSTR) &pbf->bfType, sizeof(WORD));
	/* read in last 3 dwords*/
	_lread(fh, (LPSTR) &pbf->bfSize, sizeof(DWORD) * 3);

}



/****************************************************************************
 *									    *
 *  FUNCTION   : WriteMapFileHeaderandConvertFromDwordAlignToPacked(HFILE fh, LPBITMAPFILEHEADER pbf)
 *									    *
 *  PURPOSE    : write header structure (which NOT packed) and write it PACKED
 *									    *
 *  RETURNS    : VOID
 *									    *
 ****************************************************************************/

VOID WriteMapFileHeaderandConvertFromDwordAlignToPacked(HFILE fh, LPBITMAPFILEHEADER pbf)
{

	/* write bfType*/
    _lwrite(fh, (LPSTR)&pbf->bfType, (UINT)sizeof (WORD));
	/* now pass over extra word, and only write next 3 DWORDS!*/
	_lwrite(fh, (LPSTR)&pbf->bfSize, sizeof(DWORD) * 3);
}
