// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   ddecback.h
//
//  PURPOSE:  contains ddecback declarations
//

#ifndef _DDECBACK_H
#define _DDECBACK_H

#define IDE_SERVERNAME 100

#ifdef WIN16
HDDEDATA CALLBACK __export DdeCallback (UINT iType, UINT iFmt, HCONV hConv,
                               HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                               DWORD dwData1, DWORD dwData2);

#else
HDDEDATA CALLBACK DdeCallback (UINT iType, UINT iFmt, HCONV hConv,
                               HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                               DWORD dwData1, DWORD dwData2);

#endif

#endif
