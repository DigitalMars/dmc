/****************************************************************************
 *
 *   TxtOut.c
 *
 *   TXTOUT is a sample installable compressor for AVI 1.0.
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

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <vfw.h>
#include <stdlib.h> // for mbstowcs
#include <memory.h> // for _fmemcpy
#include <stdarg.h> // for va_list

#include "TxtOut.h"

/*****************************************************************************
 *
 *
 ****************************************************************************/

#define MODNAME         "TXTOUT"
#define FOURCC_SAMP mmioFOURCC('D','R','A','W')

#define VERSION_SAMP    0x00010000 // 1.00

extern HANDLE ghModule;


/*****************************************************************************
 *
 * DefaultState holds the compression options that will be used if the user
 * compresses an image without configuring us at all first. In the case of
 * the sample compressor, it is the pixel keep ratio.
 *
 ****************************************************************************/

ICSTATE DefaultState = {2};


/*****************************************************************************
 *
 * Load() is called from the ICM_LOAD message.
 *
 * Tasks such as allocating global memory that is non-instance specific
 * or initializing coprocessor hardware may be performed here.
 *
 * Our simple case needs none of this.
 *
 ****************************************************************************/
BOOL NEAR PASCAL Load(void)
{
    DPF2("Load()");
    return TRUE;
}

/*****************************************************************************
 *
 * Free() is called from the ICM_FREE message.
 *
 * It should totally reverse the effects of Load() in preparation for
 * the DRV being removed from memory.
 *
 ****************************************************************************/
void NEAR PASCAL Free(void)
{
    DPF2("Free()");
}

/*****************************************************************************
 *
 * Open() is called from the ICM_OPEN message
 *
 * This message will be sent for a particular compress/decompress session.
 * Our code must verify that we are indeed being called as a video
 * compressor and create/initialize a state structure. The ICM will
 * give us back the pointer to that structure on every message dealing
 * with this session.
 *
 ****************************************************************************/
INSTINFO * NEAR PASCAL Open(
    ICOPEN FAR * icinfo)
{
    INSTINFO * pinst;

    DPF2("Open('%4.4hs', '%4.4hs')", &icinfo->fccType, &icinfo->fccHandler);

    //
    // refuse to open if we are not being opened as a Video compressor
    //
    if (icinfo->fccType != streamtypeTEXT) {
        DPF2("Open rejected: not TEXT");
        return NULL;
    }

    pinst = (INSTINFO *)LocalAlloc(LPTR, sizeof(INSTINFO));

    if (!pinst) {
        icinfo->dwError = ICERR_MEMORY;
        DPF2("Open rejected: no memory");
        return NULL;
    }

    //
    // init structure
    //
    pinst->dwFlags = icinfo->dwFlags;
    pinst->nDraw = 0;
    pinst->hfont = GetStockFont(ANSI_VAR_FONT);

    //
    // set the default state.
    //
    SetState(pinst, NULL, 0);

    //
    // return success.
    //
    icinfo->dwError = ICERR_OK;

    DPF2("Open succeeded!");
    return pinst;
}

/*****************************************************************************
 *
 * Close() is called on the ICM_CLOSE message.
 *
 * This message is the complement to ICM_OPEN and marks the end
 * of a compress/decompress session. We kill any in-progress operations
 * (although this shouldn't be needed) and free our instance structure.
 *
 ****************************************************************************/
LRESULT NEAR PASCAL Close(
    INSTINFO * pinst)
{
    DPF2("Close()");

    while (pinst->nDraw > 0)
        DrawEnd(pinst);
    LocalFree((HLOCAL)pinst);
    return 1;
}

/*****************************************************************************
 *
 * QueryAbout() and About() handle the ICM_ABOUT message.
 *
 * QueryAbout() returns TRUE to indicate we support an about box.
 * About() displays the box.
 *
 ****************************************************************************/
BOOL NEAR PASCAL QueryAbout(
    INSTINFO * pinst)
{
    DPF2("QueryAbout()");

    return TRUE;
}

LRESULT NEAR PASCAL About(
    INSTINFO * pinst,
    HWND hwnd)
{
    char sztempName[50];
    char sztempDesc[50];

    LoadString(ghModule, IDS_NAME, sztempName, sizeof sztempName);
    LoadString(ghModule, IDS_DESCRIPTION, sztempDesc, sizeof sztempDesc);
    DPF2("About()");
    MessageBox(hwnd, sztempDesc, sztempName, MB_OK|MB_ICONINFORMATION);
    return ICERR_OK;
}

/*****************************************************************************
 *
 * QueryConfigure() and Configure() implement the ICM_CONFIGURE message.
 *
 * These functions put up a dialog that allows the user, if he so
 * chooses, to modify the configuration portion of our state info.
 *
 ****************************************************************************/
BOOL NEAR PASCAL QueryConfigure(
    INSTINFO * pinst)
{
    DPF2("QueryConfigure()");
    return FALSE;
}

LRESULT NEAR PASCAL Configure(
    INSTINFO * pinst,
    HWND hwnd)
{
    DPF2("Configure()");
    return DialogBoxParam(ghModule,"Configure",hwnd,ConfigureDlgProc, (LONG)(UINT)pinst);
}

/*****************************************************************************
 *
 * GetState() implements the ICM_GETSTATE message.
 *
 * We copy our configuration information and return how many bytes it took.
 *
 ****************************************************************************/
LRESULT NEAR PASCAL GetState(
    INSTINFO * pinst,
    LPVOID pv,
    DWORD dwSize)
{
    DPF2("GetState(%08lX, %ld)", pv, dwSize);

    if (pv == NULL || dwSize == 0)
        return sizeof(ICSTATE);

    if (dwSize < sizeof(ICSTATE))
        return 0;

    *((ICSTATE FAR *)pv) = pinst->CurrentState;

    // return number of bytes copied
    return sizeof(ICSTATE);
}

/*****************************************************************************
 *
 * SetState() implements the ICM_SETSTATE message.
 *
 * The ICM is giving us configuration information saved by GetState()
 * earlier.
 *
 ****************************************************************************/
LRESULT NEAR PASCAL SetState(
    INSTINFO * pinst,
    LPVOID pv,
    DWORD dwSize)
{
    DPF2("SetState(%08lX, %ld)", pv, dwSize);

    if (pv == NULL)
        pinst->CurrentState = DefaultState;
    else if (dwSize >= sizeof(ICSTATE))
        pinst->CurrentState = *((ICSTATE FAR *)pv);
    else
        return 0;

    // return number of bytes copied
    return sizeof(ICSTATE);
}

/*****************************************************************************
 *
 * GetInfo() implements the ICM_GETINFO message
 *
 * We just fill in the structure to tell the ICM what we can do. The flags
 * (none of which this sample supports) mean the following :
 *
 * VIDCF_QUALITY - we support the quality variable. This means we look at
 *                 dwQuality in the ICINFO structure when compressing and
 *                 make a concious decision to trade quality for space.
 *                 (higher values of dwQuality mean quality is more
 *                 important). dwQuality is set by the ICM.
 *
 * VIDCF_TEMPORAL - We do interframe compression. In this algorithm, not
 *                  every frame is a "key frame"; some frames depend on
 *                  other frames to be generated. An example of this might
 *                  be to store frame buffer differences until the
 *                  differences are big enough to no longer make this
 *                  worthwhile, then storing another complete frame and
 *                  starting over. In this case, the complete frames that
 *                  are stored are key frames and should be flagged as
 *                  such.
 *
 * VIDCF_DRAW -     We will draw the decompressed image on our own. This is
 *                  useful if the decompression is assisted by the video
 *                  hardware.
 *
 ****************************************************************************/
LRESULT NEAR PASCAL GetInfo(
    INSTINFO * pinst,
    ICINFO FAR *icinfo,
    DWORD dwSize)
{
    char sztempName[50];
    char sztempDesc[50];

    LoadString(ghModule, IDS_NAME, sztempName, sizeof sztempName);
    LoadString(ghModule, IDS_DESCRIPTION, sztempDesc, sizeof sztempDesc);
    DPF2("GetInfo()");

    if (icinfo == NULL)
        return sizeof(ICINFO);

    if (dwSize < sizeof(ICINFO))
        return 0;

    icinfo->dwSize      = sizeof(ICINFO);
    icinfo->fccType     = streamtypeTEXT;
    icinfo->fccHandler  = FOURCC_SAMP;
    icinfo->dwFlags     = VIDCF_DRAW;

                // VIDCF_QUALITY    // supports quality
                // VIDCF_TEMPORAL   // supports inter-frame
                // VIDCF_DRAW       // supports drawing

    icinfo->dwVersion   = VERSION_SAMP;
    icinfo->dwVersionICM    = ICVERSION;
    mbstowcs(icinfo->szDescription, sztempDesc, sizeof sztempDesc);
    mbstowcs(icinfo->szName, sztempName, sizeof sztempName);

    return sizeof(ICINFO);
}


LRESULT NEAR PASCAL DrawQuery(
    INSTINFO * pinst,
    LPVOID FAR * lpFormat)
{
    return ICERR_OK;
}

/*****************************************************************************
 *
 * DrawBegin() implements ICM_DRAW_BEGIN
 *
 * This is just like DecompressBegin() except that we also must prepare to
 * actually draw the bitmap on the screen. ICDRAWBEGIN provides info specific
 * to this task.
 *
 ****************************************************************************/
LRESULT NEAR PASCAL DrawBegin(
    INSTINFO * pinst,
    ICDRAWBEGIN FAR *icinfo,
    DWORD dwSize)
{
    DPF2("DrawBegin()");

    if (pinst->nDraw++ > 0)
        return ICERR_OK;

    //
    // get ready to draw
    //
    pinst->db = *icinfo;
    SetRect(&pinst->rc, icinfo->xDst, icinfo->yDst,
        icinfo->xDst + icinfo->dxDst, icinfo->yDst + icinfo->dyDst);

    return ICERR_OK;
}

/*****************************************************************************
 *
 * Draw implements ICM_DRAW
 *
 * Decompress and draw
 *
 ****************************************************************************/
LRESULT NEAR PASCAL Draw(
    INSTINFO * pinst,
    ICDRAW FAR *icinfo,
    DWORD dwSize)
{
    HFONT hfont;

    DPF2("Draw()");

    if (icinfo->lpData && icinfo->cbData) {
        pinst->iLen = min((int) icinfo->cbData,
                (int) _fstrlen((LPSTR) icinfo->lpData));
        pinst->iLen = min(pinst->iLen, BUFFER_SIZE - 1);
        hmemcpy(pinst->achBuffer, icinfo->lpData, pinst->iLen);
        pinst->achBuffer[pinst->iLen] = '\0';
    } else if (!(icinfo->dwFlags & ICDRAW_UPDATE))
        return ICERR_OK;
    if (pinst->nDraw == 0)
        return ICERR_ERROR;

    hfont = SelectFont(pinst->db.hdc, pinst->hfont);

    ExtTextOut(pinst->db.hdc,
        pinst->db.xDst, pinst->db.yDst,
        ETO_OPAQUE,
        &pinst->rc,
        (LPSTR) pinst->achBuffer,
        pinst->iLen,
        NULL);

    SelectFont(pinst->db.hdc, hfont);

    return ICERR_OK;
}

/*****************************************************************************
 *
 * DrawEnd() implements ICM_DRAW_END
 *
 * See DecompressEnd()
 *
 ****************************************************************************/
LRESULT NEAR PASCAL DrawEnd(
    INSTINFO * pinst)
{
    DPF2("DrawEnd()");

    if (pinst->nDraw == 0)
        return ICERR_ERROR;

    if (--pinst->nDraw > 0)
        return ICERR_OK;

    //
    // but if we did we would clean up here
    //

    return ICERR_OK;
}

/*****************************************************************************
 *
 * ConfigureDlgProc() is called by Configure
 *
 * This is a standard dialog proc which allows the user to
 * pick config options for the driver.
 *
 ****************************************************************************/
BOOL CALLBACK ConfigureDlgProc(
    HWND hdlg,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch LOWORD((wParam))
        {
            case IDOK:
                EndDialog(hdlg,TRUE);
                break;

            case IDCANCEL:
                EndDialog(hdlg,FALSE);
                break;
        }
        break;
    }
    return FALSE;
}

/*****************************************************************************
 *
 * dprintf() is called by the DPF macro if DEBUG is defined at compile time.
 *
 * The messages will be send to COM1: like any debug message. To
 * enable debug output, add the following to WIN.INI :
 *
 * [debug]
 * TXTOUT=1
 *
 ****************************************************************************/

#ifdef DEBUG
static int iDebug = -1;

void FAR cdecl dprintf(
    LPSTR szFormat,
    ...)
{
    char ach[128];

    if (iDebug == -1)
        iDebug = GetProfileInt("Debug", MODNAME, FALSE);

    if (!iDebug)
        return;

    lstrcpy(ach, MODNAME ": ");

    {
        va_list va;

        va_start(va, szFormat);
        wvsprintf(ach+lstrlen(ach), szFormat, va);
        va_end(va);

    }

    lstrcat(ach, "\r\n");
    OutputDebugString(ach);
}

void FAR cdecl dprintf2(
    LPSTR szFormat,
    ...)
{
    char ach[128];

    if (iDebug == -1)
        iDebug = GetProfileInt("Debug", MODNAME, FALSE);

    if (iDebug < 2)
        return;

    lstrcpy(ach, MODNAME ": ");

    {
        va_list va;

        va_start(va, szFormat);
        wvsprintf(ach+lstrlen(ach), szFormat, va);
        va_end(va);

    }

    lstrcat(ach, "\r\n");

    OutputDebugString(ach);
}

#endif
