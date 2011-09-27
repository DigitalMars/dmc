// defo2v.cpp
//
// Implementation file for DEFO2V.O2V (LibMain, WEP).
//
// Copyright (c) 1993-1995 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 7, 1993
//
// InterNet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 6, 1993  cek     First implementation.
//  August 7, 1993  cek     Rewrote without using MFC 2.0.
//

#include "precomp.h"

#include <initguid.h>

#include "defo2v.h"

HINSTANCE   g_hInst = NULL ;

extern "C"
HANDLE FAR PASCAL LibMain(HINSTANCE hInst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpCmdLine)
{

    if (0!=cbHeapSize)
        UnlockData(0);

    g_hInst=hInst;
    return hInst;
}

#pragma code_seg("WEP_SEG")
extern "C"
void FAR PASCAL WEP(int bSystemExit)
{
    return;
}

