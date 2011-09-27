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

#define ID_APP      1000

/* Menu Items */
#define MENU_ABOUT          20

#define MENU_EXIT           4
#define MENU_BALL	    99
#define MENU_OPEN           11
#define	MENU_NEW	    10
#define MENU_ADD	    17
#define MENU_NEWPALETTE     12
#define MENU_SAVEAS         13
#define MENU_OPTIONS	    15

#define MENU_ZOOMQUARTER    54
#define MENU_ZOOMHALF	    50
#define MENU_ZOOM1	    51
#define MENU_ZOOM2	    52
#define MENU_ZOOM4	    53

#define MENU_PLAY	    100
#define MENU_PAUSE	    101
#define MENU_STOP           102

// Dialogs
#define IDD_ABOUT           301
#define IDD_NCOLORS         302
#define IDS_PALETTE         303
#define IDC_NCOLORS         400

#ifdef DEBUG
    extern void CDECL dprintf(LPSTR, ...);
    #define DPF dprintf
#else
    #define DPF / ## /
#endif
