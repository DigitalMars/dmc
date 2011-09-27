/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

#include "glos.h"

#include <GL/gl.h>
#include "stonehen.h"
#include "setpixel.h"

#define BLACK   PALETTERGB(0,0,0)
#define WHITE   PALETTERGB(255,255,255)
#define NUM_STATIC_COLORS   (COLOR_BTNHIGHLIGHT - COLOR_SCROLLBAR + 1)

static COLORREF gacrSave[NUM_STATIC_COLORS];

static COLORREF gacrBlackAndWhite[NUM_STATIC_COLORS] = {
    WHITE,  // COLOR_SCROLLBAR
    BLACK,  // COLOR_BACKGROUND
    BLACK,  // COLOR_ACTIVECAPTION
    WHITE,  // COLOR_INACTIVECAPTION
    WHITE,  // COLOR_MENU
    WHITE,  // COLOR_WINDOW
    BLACK,  // COLOR_WINDOWFRAME
    BLACK,  // COLOR_MENUTEXT
    BLACK,  // COLOR_WINDOWTEXT
    WHITE,  // COLOR_CAPTIONTEXT
    WHITE,  // COLOR_ACTIVEBORDER
    WHITE,  // COLOR_INACTIVEBORDER
    WHITE,  // COLOR_APPWORKSPACE
    BLACK,  // COLOR_HIGHLIGHT
    WHITE,  // COLOR_HIGHLIGHTTEXT
    WHITE,  // COLOR_BTNFACE
    BLACK,  // COLOR_BTNSHADOW
    BLACK,  // COLOR_GRAYTEXT
    BLACK,  // COLOR_BTNTEXT
    BLACK,  // COLOR_INACTIVECAPTIONTEXT
    BLACK   // COLOR_BTNHIGHLIGHT
    };
static INT gaiStaticIndex[NUM_STATIC_COLORS] = {
    COLOR_SCROLLBAR          ,
    COLOR_BACKGROUND         ,
    COLOR_ACTIVECAPTION      ,
    COLOR_INACTIVECAPTION    ,
    COLOR_MENU               ,
    COLOR_WINDOW             ,
    COLOR_WINDOWFRAME        ,
    COLOR_MENUTEXT           ,
    COLOR_WINDOWTEXT         ,
    COLOR_CAPTIONTEXT        ,
    COLOR_ACTIVEBORDER       ,
    COLOR_INACTIVEBORDER     ,
    COLOR_APPWORKSPACE       ,
    COLOR_HIGHLIGHT          ,
    COLOR_HIGHLIGHTTEXT      ,
    COLOR_BTNFACE            ,
    COLOR_BTNSHADOW          ,
    COLOR_GRAYTEXT           ,
    COLOR_BTNTEXT            ,
    COLOR_INACTIVECAPTIONTEXT,
    COLOR_BTNHIGHLIGHT
    };


HPALETTE    ghpalOld, ghPalette = (HPALETTE) 0;
BOOL        bUseStaticColors = FALSE;
BOOL        bSystemColorsInUse = FALSE;
BOOL        bStaticSaved = FALSE;



unsigned char threeto8[8] = {
    0, 0111>>1, 0222>>1, 0333>>1, 0444>>1, 0555>>1, 0666>>1, 0377
};

unsigned char twoto8[4] = {
    0, 0x55, 0xaa, 0xff
};

unsigned char oneto8[2] = {
    0, 255
};

unsigned char
ComponentFromIndex(UCHAR i, UINT nbits, UINT shift)
{
    unsigned char val;

    val = i >> shift;
    switch (nbits) {

    case 1:
        val &= 0x1;
        return oneto8[val];

    case 2:
        val &= 0x3;
        return twoto8[val];

    case 3:
        val &= 0x7;
        return threeto8[val];

    default:
        return 0;
    }
}

void
CreateRGBPalette(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    LOGPALETTE *pPal;
    int n, i;

    ppfd = &pfd;
    n = GetPixelFormat(hdc);
    DescribePixelFormat(hdc, n, sizeof(PIXELFORMATDESCRIPTOR), ppfd);

    if (ppfd->dwFlags & PFD_NEED_PALETTE) {
        n = 1 << ppfd->cColorBits;
        pPal = (PLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) +
                n * sizeof(PALETTEENTRY));
        pPal->palVersion = 0x300;
        pPal->palNumEntries = n;
        for (i=0; i<n; i++) {
            pPal->palPalEntry[i].peRed =
                    ComponentFromIndex(i, ppfd->cRedBits, ppfd->cRedShift);
            pPal->palPalEntry[i].peGreen =
                    ComponentFromIndex(i, ppfd->cGreenBits, ppfd->cGreenShift);
            pPal->palPalEntry[i].peBlue =
                    ComponentFromIndex(i, ppfd->cBlueBits, ppfd->cBlueShift);
            pPal->palPalEntry[i].peFlags = 0;
        }

        if (256 == n)
        {
            if (bUseStaticColors = ppfd->dwFlags & PFD_NEED_SYSTEM_PALETTE)
            {
                // Prepare to take over the system's static colors

                for (i = 0; i < n; i++)
                    pPal->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
            }
        }

        ghPalette = CreatePalette(pPal);
        LocalFree(pPal);

        //ghpalOld = SelectPalette(hdc, ghPalette, FALSE);
        //n = RealizePalette(hdc);
        n = RealizePaletteNow(hdc, ghPalette, FALSE);
    }
}

BOOL bSetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;

    ppfd->iPixelType = PFD_TYPE_RGBA;
    ppfd->cColorBits = 24;

    ppfd->cDepthBits = 16;			//GLX_DEPTH_SIZE


    ppfd->cRedBits = 1;				//GLX_RED_SIZE
    ppfd->cRedShift = 0;
    ppfd->cGreenBits = 1;			//GLX_GREEN_SIZE
    ppfd->cGreenShift = 0;
    ppfd->cBlueBits = 1;			//GLX_BLUE_SIZE
    ppfd->cBlueShift = 0;
    ppfd->cAlphaBits = 0;
    ppfd->cAlphaShift = 0;
    ppfd->cAccumBits = 0;			//ACCUM NOT SUPPORTED
    ppfd->cAccumRedBits = 0; 			//GLX_ACCUM_RED_SIZE
    ppfd->cAccumGreenBits = 0;			//GLX_ACCUM_GREEN_SIZE
    ppfd->cAccumBlueBits = 0; 			//GLX_ACCUM_BLUE_SIZE
    ppfd->cAccumAlphaBits = 0;			//GLX_ACCUM_ALPHA_SIZE

    ppfd->cStencilBits = 4;			//GLX_STENCIL_SIZE
    ppfd->cAuxBuffers = 0;
    ppfd->bReserved = 0;
    ppfd->dwVisibleMask =
    ppfd->dwDamageMask = 0;

    pixelformat = ChoosePixelFormat(hdc, ppfd);

    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 )
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    CreateRGBPalette(hdc);

    return TRUE;
}

long RealizePaletteNow( HDC hdc, HPALETTE hPal, BOOL bForceBackground )
{
    long Result = -1;

    // If static system color usage is set, prepare to take over the
    // system palette.

    if ( bUseStaticColors )
    {
    // If foreground, take over the static colors.  If background, release
    // the static colors.
    //
    // Note: even though SaveStaticEntries is always called, it only saves
    // the static colors if we don't currently possess them.  Once saved,
    // they cannot be resaved until RestoreStaticEntries is called.  This
    // prevents a quirk in the palette messaging that may cause us to try
    // to save a copy of the B&W settings as the original settings.

        SaveStaticEntries( hdc );
        SetSystemPaletteUse( hdc, bForceBackground ? SYSPAL_STATIC
                                                  : SYSPAL_NOSTATIC );

    // Rerealize the palette.
    //
    // If set to TRUE, bForceBackground will force the palette to be realized
    // as a background palette, regardless of focus.

        if ( UnrealizeObject( hPal ) &&
                NULL != (ghpalOld = SelectPalette( hdc, hPal, bForceBackground )) )
        {
            Result = RealizePalette( hdc );
        }

    // If foreground, setup the B&W static colors.  If background, restore
    // the original static colors.

        if ( bForceBackground )
            RestoreStaticEntries( hdc );
        else
            UseStaticEntries( hdc );
    }
    else
    {
        if ( NULL != (ghpalOld = SelectPalette( hdc, hPal, FALSE )) )
            Result = RealizePalette( hdc );
    }

    return( Result );
}

static VOID SaveStaticEntries(HDC hdc)
{
    int i;

    if ( !bSystemColorsInUse )
    {
        for (i = COLOR_SCROLLBAR; i <= COLOR_BTNHIGHLIGHT; i++)
            gacrSave[i - COLOR_SCROLLBAR] = GetSysColor(i);

        bStaticSaved = TRUE;
    }
}

static VOID UseStaticEntries(HDC hdc)
{
    SetSysColors(NUM_STATIC_COLORS, gaiStaticIndex, gacrBlackAndWhite);
    bSystemColorsInUse = TRUE;

    PostMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
}

static VOID RestoreStaticEntries(HDC hdc)
{
    if ( bStaticSaved )
    {
        SetSysColors(NUM_STATIC_COLORS, gaiStaticIndex, gacrSave);
        bSystemColorsInUse = FALSE;

        PostMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
    }
}
