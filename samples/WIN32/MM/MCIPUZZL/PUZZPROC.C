/****************************************************************************
 *
 *  PUZZPROC.C
 *
 *  Modification of standard AVI drawing handler.
 *
 ***************************************************************************/
/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define  STRICT
#define  INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <vfw.h>

#include "puzzproc.h"
#include "puzzle.h"

#define LOCAL  __stdcall
#define SZCODE char

static SZCODE szDescription[] = "Microsoft Puzzle Draw handler";
static SZCODE szName[]        = "MS Puzzle";

#define FOURCC_AVIDraw          mmioFOURCC('P','U','Z','Z')
#define VERSION_AVIDraw         0x00010000      // 1.00

extern PUZZLE   gPuzzle;

/***************************************************************************
 ***************************************************************************/

typedef struct {
    HDRAWDIB            hdd;
    HDC                 hdc;            // HDC to draw to
    int                 xDst;           // destination rectangle
    int                 yDst;
    int                 dxDst;
    int                 dyDst;
    int                 xSrc;           // source rectangle
    int                 ySrc;
    int                 dxSrc;
    int                 dySrc;
    LPBYTE              lpBuffer;
} INSTINFO;


/***************************************************************************
 ***************************************************************************/

typedef ICOPEN *                LPICOPEN;
typedef ICINFO *                LPICINFO;
typedef ICDRAW *                LPICDRAW;
typedef ICDRAWSUGGEST *         LPICDRAWSUGGEST;
typedef ICDRAWBEGIN *           LPICDRAWBEGIN;
typedef INSTINFO *              LPINSTINFO;

/***************************************************************************
 ***************************************************************************/

// static stuff in this file.
static LONG LOCAL AVIDrawOpen(LPICOPEN);
static LONG LOCAL AVIDrawClose(LPINSTINFO);
static LONG LOCAL AVIDrawGetInfo(LPICINFO, LONG);
static LONG LOCAL AVIDrawQuery(LPINSTINFO, LPBITMAPINFOHEADER);
static LONG LOCAL AVIDrawSuggestFormat(LPINSTINFO, LPICDRAWSUGGEST, LONG);
static LONG LOCAL AVIDrawBegin(LPINSTINFO, LPICDRAWBEGIN, LONG);
static LONG LOCAL AVIDraw(LPINSTINFO, LPICDRAW, LONG);
static LONG LOCAL AVIDrawEnd(LPINSTINFO);
static LONG LOCAL AVIDrawChangePalette(LPINSTINFO, LPBITMAPINFOHEADER);

/***************************************************************************
 ***************************************************************************/

LONG CALLBACK ICAVIDrawProc(
DWORD id,
HDRVR hDriver,
UINT uiMessage,
LPARAM lParam1,
LPARAM lParam2)
{
        LPINSTINFO pi = (LPINSTINFO)id;

        switch (uiMessage) {

                case DRV_LOAD:
                case DRV_FREE:
                        return 1L;

                //open
                case DRV_OPEN:
                        if (lParam2 == 0L)
                                return 1L;
                        else
                                return AVIDrawOpen((LPICOPEN)lParam2);

                //close
                case DRV_CLOSE:
                        return AVIDrawClose(pi);

                //Configure and Info messages
                case DRV_QUERYCONFIGURE:    // configuration from drivers applet
                        return 0;

                case DRV_CONFIGURE:
                        return 1;

                case ICM_CONFIGURE:
                case ICM_ABOUT:
                        return ICERR_UNSUPPORTED;

                case ICM_GETINFO:
                        return AVIDrawGetInfo((LPICINFO)lParam1, lParam2);

                //state messages
                case ICM_GETSTATE:
                case ICM_SETSTATE:
                        return 0L;

                //draw messages
                case ICM_DRAW_QUERY:
                        return AVIDrawQuery(pi, (LPBITMAPINFOHEADER)lParam1);

                case ICM_DRAW_SUGGESTFORMAT:
                        return AVIDrawSuggestFormat(pi, (LPICDRAWSUGGEST) lParam1, lParam2);

                case ICM_DRAW_BEGIN:
                        return AVIDrawBegin(pi, (LPICDRAWBEGIN) lParam1, lParam2);

                case ICM_DRAW_REALIZE:
                        pi->hdc = (HDC) lParam1;
                        if (!pi->hdc || !pi->hdd)
                                break;
                        return DrawDibRealize(pi->hdd, pi->hdc, (BOOL) lParam2);

                case ICM_DRAW_GET_PALETTE:
                        if (!pi->hdd)
                                break;
                        return (LONG) (UINT) DrawDibGetPalette(pi->hdd);

                case ICM_DRAW:
                        return AVIDraw(pi, (LPICDRAW)lParam1, lParam2);

                case ICM_DRAW_CHANGEPALETTE:
                        return AVIDrawChangePalette(pi, (LPBITMAPINFOHEADER) lParam1);

                case ICM_DRAW_END:
                        return AVIDrawEnd(pi);

                //standard driver messages
                case DRV_DISABLE:
                case DRV_ENABLE:
                        return 1L;

                case DRV_INSTALL:
                case DRV_REMOVE:
                        return 1L;
        }
        if (uiMessage < DRV_USER)
                return DefDriverProc(id,hDriver,uiMessage,lParam1,lParam2);
        else
                return ICERR_UNSUPPORTED;
}
/*****************************************************************************
 *
 * AVIDrawOpen() is called from the DRV_OPEN message
 *
 ****************************************************************************/

static LONG LOCAL AVIDrawOpen(
LPICOPEN icopen)
{
        LPINSTINFO pinst;

        // refuse to open if we are not being opened as a video draw device
        if (icopen->fccType != streamtypeVIDEO)
                return 0L;

        if (icopen->dwFlags == ICMODE_COMPRESS)
                return 0L;

        if (icopen->dwFlags == ICMODE_DECOMPRESS)
                return 0L;

        pinst = (LPINSTINFO)GlobalAllocPtr(GHND, sizeof(INSTINFO));

        if (!pinst) {
                icopen->dwError = ICERR_MEMORY;
                return 0L;
        }

        // init structure
        pinst->hdd = DrawDibOpen();

        // return success.
        icopen->dwError = ICERR_OK;

        return (LONG)pinst;
}
/*****************************************************************************
 *
 * AVIDrawClose() is called on the DRV_CLOSE message.
 *
 ****************************************************************************/
static LONG LOCAL AVIDrawClose(
LPINSTINFO pi)
{
        if (pi->hdd)
                DrawDibClose(pi->hdd);

        if (pi->lpBuffer)
                GlobalFreePtr(pi->lpBuffer);

        GlobalFreePtr(pi);
        return 1L;
}
/*****************************************************************************
 *
 * AVIDrawGetInfo() implements the ICM_GETINFO message
 *
 ****************************************************************************/
static LONG LOCAL AVIDrawGetInfo(
LPICINFO icinfo,
LONG lSize)
{
        if (icinfo == NULL)
                return sizeof(ICINFO);

        if (lSize < sizeof(ICINFO))
                return 0L;

        icinfo->dwSize          = sizeof(ICINFO);
        icinfo->fccType         = ICTYPE_VIDEO;
        icinfo->fccHandler      = FOURCC_AVIDraw;
        icinfo->dwFlags         = VIDCF_DRAW;
        icinfo->dwVersion       = VERSION_AVIDraw;
        icinfo->dwVersionICM    = ICVERSION;
        MultiByteToWideChar(CP_ACP, 0, szDescription, -1, icinfo->szDescription, sizeof(icinfo->szDescription));
        MultiByteToWideChar(CP_ACP, 0, szName, -1, icinfo->szDescription, sizeof(icinfo->szDescription));
        return sizeof(ICINFO);
}
/*****************************************************************************
 *
 * AVIDrawQuery() implements ICM_DRAW_QUERY
 *
 ****************************************************************************/
static LONG LOCAL AVIDrawQuery(
LPINSTINFO pi,
LPBITMAPINFOHEADER lpbiIn)
{
        // determine if the input DIB data is in a format we like.
        if (lpbiIn == NULL)
                return ICERR_BADFORMAT;

        // determine if the input DIB data is in a format we like.
        if (lpbiIn->biCompression != BI_RGB)
                return ICERR_BADFORMAT;

        return ICERR_OK;
}
/*****************************************************************************
 *
 * AVIDrawSuggestFormat() implements ICM_DRAW_SUGGESTFORMAT
 *
 ****************************************************************************/

static LONG LOCAL AVIDrawSuggestFormat(
LPINSTINFO pi,
LPICDRAWSUGGEST lpicd,
LONG cbicd)
{
        HIC hic;
        LONG l;

        if (lpicd->lpbiSuggest == NULL)
                return sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);

        // Call COMPMAN to get a good format to display data in....
        hic = ICGetDisplayFormat(NULL, lpicd->lpbiIn, lpicd->lpbiSuggest,
                                                0, lpicd->dxDst, lpicd->dyDst);

        if (hic)
                ICClose(hic);

        l = lpicd->lpbiSuggest->biClrUsed;

        if (lpicd->lpbiSuggest)
                if (lpicd->lpbiSuggest->biCompression == BI_RLE8)
                        lpicd->lpbiSuggest->biCompression = BI_RGB;

        // !!! Should check this format here to make sure it's RGB...
        // !!! If not, we could force it to 8-bit....

        l = l * sizeof(RGBQUAD);
        return l + sizeof(BITMAPINFOHEADER);
}

/*****************************************************************************
 *
 * AVIDrawBegin() implements ICM_DRAW_BEGIN
 *
 ****************************************************************************/

static LONG LOCAL AVIDrawBegin(
LPINSTINFO pi,
LPICDRAWBEGIN lpicd,
LONG cbicd)
{
        LONG l = AVIDrawQuery(pi, lpicd->lpbi);

        if ((l != 0) || (lpicd->dwFlags & ICDRAW_QUERY))
                return l;

        // Copy over whatever we want to remember
        pi->hdc = lpicd->hdc;
        pi->xDst = lpicd->xDst;
        pi->yDst = lpicd->yDst;
        pi->dxDst = lpicd->dxDst;
        pi->dyDst = lpicd->dyDst;
        pi->xSrc = lpicd->xSrc;
        pi->ySrc = lpicd->ySrc;
        pi->dxSrc = lpicd->dxSrc;
        pi->dySrc = lpicd->dySrc;

        SetStretchBltMode(pi->hdc, COLORONCOLOR);

        if (!DrawDibBegin(
                        pi->hdd,
                        pi->hdc,
                        pi->dxDst,
                        pi->dyDst,
                        lpicd->lpbi,
                        pi->dxSrc,
                        pi->dySrc,
                        0)) {  // !!! Flags?
                return ICERR_UNSUPPORTED;
        }

        // !!! error check

        // Allocate a buffer for the scrambled picture
        if (pi->lpBuffer)
                GlobalFreePtr(pi->lpBuffer);

        pi->lpBuffer = GlobalAllocPtr(GMEM_MOVEABLE, lpicd->lpbi->biSizeImage);

        if (!pi->lpBuffer)
                return ICERR_MEMORY;

        return ICERR_OK;
}
/*****************************************************************************
 *
 * AVIDraw() implements ICM_DRAW
 *
 ****************************************************************************/

static LONG LOCAL AVIDraw(
LPINSTINFO pi,
LPICDRAW lpicd,
LONG cbicd)
{
        UINT wFlags = DDF_SAME_HDC;

        if ((lpicd->dwFlags & ICDRAW_NULLFRAME) || lpicd->lpData == NULL) {
                if (lpicd->dwFlags & ICDRAW_UPDATE)
                        wFlags |= DDF_UPDATE;
                else
                        return ICERR_OK;            // nothing to draw
        }

        if (lpicd->dwFlags & ICDRAW_PREROLL)
                wFlags |= DDF_DONTDRAW;

        if (lpicd->dwFlags & ICDRAW_HURRYUP)
                wFlags |= DDF_HURRYUP;

        // This is the only part that actually has to do with the puzzle:
        // Mix up the picture into our extra buffer.
        if (lpicd->lpData)
                MixPicture(&gPuzzle, lpicd->lpFormat, lpicd->lpData, pi->lpBuffer);

        if (!DrawDibDraw(
                        pi->hdd,
                        pi->hdc,
                        pi->xDst,
                        pi->yDst,
                        pi->dxDst,
                        pi->dyDst,
                        lpicd->lpFormat,
                        pi->lpBuffer,
                        pi->xSrc,
                        pi->ySrc,
                        pi->dxSrc,
                        pi->dySrc,
                        wFlags)) {
                if (wFlags & DDF_UPDATE)
                        return ICERR_CANTUPDATE;
                else
                        return ICERR_UNSUPPORTED;
        }

        return ICERR_OK;
}
/*****************************************************************************
 *
 * AVIDrawChangePalette() implements ICM_DRAW_CHANGE_PALETTE
 *
 ****************************************************************************/

static LONG LOCAL AVIDrawChangePalette(
LPINSTINFO pi,
LPBITMAPINFOHEADER lpbi)
{
        PALETTEENTRY ape[256];
        LPRGBQUAD lprgb = (LPRGBQUAD) ((LPBYTE) lpbi + lpbi->biSize);
        int i;

        for (i = 0; i < (int) lpbi->biClrUsed; i++) {
                ape[i].peRed = lprgb[i].rgbRed;
                ape[i].peGreen = lprgb[i].rgbGreen;
                ape[i].peBlue = lprgb[i].rgbBlue;
                ape[i].peFlags = 0;
        }

        DrawDibChangePalette(pi->hdd, 0, (int) lpbi->biClrUsed, (LPPALETTEENTRY)ape);
        return ICERR_OK;
}
/*****************************************************************************
 *
 * AVIDrawEnd() implements ICM_DRAW_END
 *
 ****************************************************************************/

static LONG LOCAL AVIDrawEnd(
LPINSTINFO pi)
{
        // Note: do not call DrawDibEnd here, as we still may be asked to
        // update our current display, and calling DrawDibEnd would wipe
        // that out.

        return ICERR_OK;
}
