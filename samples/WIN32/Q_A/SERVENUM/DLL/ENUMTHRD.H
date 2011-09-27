// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   enumthrd.h
//
//  PURPOSE:  contains enumthrd declarations
//

#ifndef _ENUMTHRD_H
#define _ENUMTHRD_H

//-----------------------------------------------------------------------
// enumthrd defines

#define ERROR_FAILURE 1

#define HANDLE_PROV  0
#define HANDLE_DOM   1
#define HANDLE_SERV  2
#define NUM_HANDLES  3


//-----------------------------------------------------------------------
// enumthrd prototypes

DWORD WINAPI InitialEnumThread( LPENUMSYNC lpSync );
DWORD WINAPI EnumExpandThread( LPENUMLISTOBJECT lpObject );
DWORD WINAPI EnumContainerThread( LPENUMOBJECT lpObject );

#endif
