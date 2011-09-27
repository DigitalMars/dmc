// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// MODULE: enumutil.h
//
// PURPOSE:
//    Contains declarations enumutil functions
//

#ifndef _ENUMUTIL_H
#define _ENUMUTIL_H

//-------------------------------------------------------------
// enumutil defines

#define BITMAP_START     500

#define BITMAP_HEIGHT    16
#define BITMAP_WIDTH     16

#define START_NDX        0
#define SERV_NDX         0
#define DOM_NDX          1
#define DOMEX_NDX        2
#define PROV_NDX         3
#define PROVEX_NDX       4
#define HIGHLIGHT_NDX    5

//-------------------------------------------------------------
// enumutil structs

// ENUMNODE is the basic tree data structure
typedef struct _ENUMNODE
{
    UINT ndxBmp;                 // ndx into bitmap array ( non highlight )
    LPNETRESOURCE lpNet;         // pointer to the corresponding NETRESOURCE entry
    DWORD cChild;                // number of children that have been enumerated
    struct _ENUMNODE * lpChild;  // dynamically allocated array of children
} ENUMNODE, *LPENUMNODE;

// ENUMSYNC is used for thread syncronization
typedef struct _ENUMSYNC
{
    HANDLE hwnd;                 // handle to dialog box
    DWORD cThread;               // number of executing threads
    DWORD cNetBuf;               // number of buffers containing NETRESOURCE info
    LPNETRESOURCE * lplpnetBuf;  // dynamically allocated array of buffers
    LPENUMNODE lpRoot;           // pointer to root of ENUMNODE tree
    CRITICAL_SECTION cs;         // critical section object for syncronization
} ENUMSYNC, *LPENUMSYNC;

// ENUMOBJECT bundles syncronization with a node
typedef struct _ENUMOBJECT
{
    LPENUMSYNC lpSync;
    LPENUMNODE lpNode;
} ENUMOBJECT, *LPENUMOBJECT;

// ENUMLISTOBJECT bundles ENUMOBJECT with an index into the listbox
typedef struct _ENUMLISTOBJECT
{
    UINT nItem;
    ENUMOBJECT Object;
} ENUMLISTOBJECT, *LPENUMLISTOBJECT;

// TLSOBJECT stores all thread local data
typedef struct _TLSOBJECT
{
    LPENUMSYNC lpSync;

    HBITMAP hbmpArray[HIGHLIGHT_NDX*2];
    HFONT   hfont;
    HBRUSH  hbrushHL;

    TEXTMETRIC tm;

    WNDPROC lpfnListProc;

    LPWSTR lpszServer;     
    LPDWORD lpcchServer;
    FARPROC lpfnHelpProc;

} TLSOBJECT, *LPTLSOBJECT;


//-------------------------------------------------------------
// enumutil prototypes

VOID FreeEnumSync( LPENUMSYNC * lplpSync );
VOID FreeEnumChild( LPENUMNODE lplpNode );
int __cdecl CompareEnumNode( const void * lpNode1, const void * lpNode2 );

UINT EnumListRecurse( HWND hwnd, LPENUMNODE lpNode, LPTSTR lpServer );
VOID EnumListExpand( HWND hwnd, LPENUMSYNC lpSync, LPENUMNODE lpNode, UINT ndx );
VOID EnumListCollapse( HWND hwnd, LPENUMNODE lpNode, UINT ndx );

VOID LoadBitmapArray( HBITMAP * lphbmpArray);

#endif
