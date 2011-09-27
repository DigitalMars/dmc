
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/* structure for the character range 'name' lookup table.  */
typedef struct tagLookupEntry{
    USHORT   start;
    USHORT   end;
    TCHAR    String[32];
} LookupEntry;

/* The following data comes straight out of the Addison-Wesley Unicode book. */
#define NRANGE 24
LookupEntry  RangeName[NRANGE] =
    {{ 0x0000,0x007f, TEXT("ASCII")},
     { 0x0080,0x00ff, TEXT("Latin1 Characters")},
     { 0x0100,0x017f, TEXT("European Latin")},
     { 0x0180,0x01ff, TEXT("Extended Latin")},
     { 0x0200,0x024f, TEXT("<Bad unicode range.>")},
     { 0x0250,0x02af, TEXT("Standard Phonetic")},
     { 0x02b0,0x02ff, TEXT("Modifier Letters")},
     { 0x0300,0x036f, TEXT("Generic Diacritical Marks")},
     { 0x0370,0x03ff, TEXT("Greek")},
     { 0x0400,0x04ff, TEXT("Cyrillic")},
     { 0x0500,0x052f, TEXT("<Bad unicode range.>")},
     { 0x0530,0x058f, TEXT("Armenian")},
     { 0x0590,0x05ff, TEXT("Hebrew")},
     { 0x0600,0x06ff, TEXT("Arabic")},
     { 0x0900,0x1fff, TEXT("<not specified in table.>")},
     { 0x2000,0x206f, TEXT("General Punctutation")},
     { 0x2070,0x209f, TEXT("Superscipts & Subscripts")},
     { 0x20a0,0x20cf, TEXT("Currency Symbols")},
     { 0x20d0,0x20ff, TEXT("Diacritical Marks for Symbols")},
     { 0x2100,0x214f, TEXT("Letterlike Symbols")},
     { 0x2150,0x218f, TEXT("Number Forms")},
     { 0x2190,0x21ff, TEXT("Arrows")},
     { 0x2200,0x22ff, TEXT("Mathematical Operators")},
     { 0x2300,0xffff, TEXT("<not specified in table.>")}};
