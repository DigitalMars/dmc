
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* getdev.h - header file for the getdev sample. */


/* function prototypes.  Window procedures first. */
long FAR PASCAL MainDlgProc(HWND, UINT, UINT, LONG);

VOID ExpandInfo (HWND, int, int);



/* declare global variable to be used to hold string sent to list box. */
char buffer[100];
char space[] = "   %s";



/* the control ID from the dialog box. */
#define DID_LISTBOX     0x006B


/* structure for the lookup tables.  */
typedef struct tagLookupEntry{
    int     Value;
    char    String[100];
} LookupEntry;



/* GetDeviceCaps() codes from WINGDI.H */
#define NINDEX  28
LookupEntry  DevCaps[NINDEX] =
    {{  0 , "DRIVERVERSION "},
     {  2 , "TECHNOLOGY    "},
     {  4 , "HORZSIZE      "},
     {  6 , "VERTSIZE      "},
     {  8 , "HORZRES       "},
     {  10, "VERTRES       "},
     {  12, "BITSPIXEL     "},
     {  14, "PLANES        "},
     {  16, "NUMBRUSHES    "},
     {  18, "NUMPENS       "},
     {  20, "NUMMARKERS    "},
     {  22, "NUMFONTS      "},
     {  24, "NUMCOLORS     "},
     {  26, "PDEVICESIZE   "},
     {  28, "CURVECAPS     "},
     {  30, "LINECAPS      "},
     {  32, "POLYGONALCAPS "},
     {  34, "TEXTCAPS      "},
     {  36, "CLIPCAPS      "},
     {  38, "RASTERCAPS    "},
     {  40, "ASPECTX       "},
     {  42, "ASPECTY       "},
     {  44, "ASPECTXY      "},
     {  88, "LOGPIXELSX    "},
     {  90, "LOGPIXELSY    "},
     { 104, "SIZEPALETTE   "},
     { 106, "NUMRESERVED   "},
     { 108, "COLORRES      "}};




/* Device Technologies */
#define NDevice 7
LookupEntry  Device[NDevice] =
    {{ 0 , "DT_PLOTTER   "},
     { 1 , "DT_RASDISPLAY"},
     { 2 , "DT_RASPRINTER"},
     { 3 , "DT_RASCAMERA "},
     { 4 , "DT_CHARSTREAM"},
     { 5 , "DT_METAFILE  "},
     { 6 , "DT_DISPFILE  "}};


/* Curve Capabilities */
#define NCurveCaps 9
LookupEntry  CurveCaps[NCurveCaps] =
    {{ 0   , "CC_NONE      "},
     { 1   , "CC_CIRCLES   "},
     { 2   , "CC_PIE       "},
     { 4   , "CC_CHORD     "},
     { 8   , "CC_ELLIPSES  "},
     { 16  , "CC_WIDE      "},
     { 32  , "CC_STYLED    "},
     { 64  , "CC_WIDESTYLED"},
     { 128 , "CC_INTERIORS "}};


/* Line Capabilities */
#define NLineCaps 8
LookupEntry  LineCaps[NLineCaps] =
    {{ 0   , "LC_NONE       "},
     { 2   , "LC_POLYLINE   "},
     { 4   , "LC_MARKER     "},
     { 8   , "LC_POLYMARKER "},
     { 16  , "LC_WIDE       "},
     { 32  , "LC_STYLED     "},
     { 64  , "LC_WIDESTYLED "},
     { 128 , "LC_INTERIORS  "}};


/* Polygonal Capabilities */
#define NPolygonalCaps 10
LookupEntry  PolygonalCaps[NPolygonalCaps] =
    {{ 0   , "PC_NONE       "},
     { 1   , "PC_POLYGON    "},
     { 2   , "PC_RECTANGLE  "},
     { 4   , "PC_WINDPOLYGON"},
     { 4   , "PC_TRAPEZOID  "},   // HACK   two 4's ???
     { 8   , "PC_SCANLINE   "},
     { 16  , "PC_WIDE       "},
     { 32  , "PC_STYLED     "},
     { 64  , "PC_WIDESTYLED "},
     { 128 , "PC_INTERIORS  "}};


/* Clip Capabilities */
#define NClipCaps 2
LookupEntry  ClipCaps[NClipCaps] =
    {{ 0   , "CP_NONE     "},
     { 1   , "CP_RECTANGLE"}};


/* Text Capabilities */
#define NTextCaps 16
LookupEntry  TextCaps[NTextCaps] =
    {{ 0x0001 , "TC_OP_CHARACTER"},
     { 0x0002 , "TC_OP_STROKE   "},
     { 0x0004 , "TC_CP_STROKE   "},
     { 0x0008 , "TC_CR_90       "},
     { 0x0010 , "TC_CR_ANY      "},
     { 0x0020 , "TC_SF_X_YINDEP "},
     { 0x0040 , "TC_SA_DOUBLE   "},
     { 0x0080 , "TC_SA_INTEGER  "},
     { 0x0100 , "TC_SA_CONTIN   "},
     { 0x0200 , "TC_EA_DOUBLE   "},
     { 0x0400 , "TC_IA_ABLE     "},
     { 0x0800 , "TC_UA_ABLE     "},
     { 0x1000 , "TC_SO_ABLE     "},
     { 0x2000 , "TC_RA_ABLE     "},
     { 0x4000 , "TC_VA_ABLE     "},
     { 0x8000 , "TC_RESERVED    "}};


/* Raster Capabilities */
#define NRasterCaps 12
LookupEntry  RasterCaps[NRasterCaps] =
    {{  1     , "RC_BITBLT       "},
     {  2     , "RC_BANDING      "},
     {  4     , "RC_SCALING      "},
     {  8     , "RC_BITMAP64     "},
     {  0x0010, "RC_GDI20_OUTPUT "},
     {  0x0080, "RC_DI_BITMAP    "},
     {  0x0100, "RC_PALETTE      "},
     {  0x0200, "RC_DIBTODEV     "},
     {  0x0400, "RC_BIGFONT      "},
     {  0x0800, "RC_STRETCHBLT   "},
     {  0x1000, "RC_FLOODFILL    "},
     {  0x2000, "RC_STRETCHDIB   "}};
