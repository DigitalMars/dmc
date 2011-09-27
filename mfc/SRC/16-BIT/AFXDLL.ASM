; This is a part of the Microsoft Foundation Classes C++ library.
; Copyright (C) 1992-1993 Microsoft Corporation
; All rights reserved.
;
; This source code is only intended as a supplement to the
; Microsoft Foundation Classes Reference and Microsoft
; QuickHelp and/or WinHelp documentation provided with the library.
; See these sources for detailed information regarding the
; Microsoft Foundation Classes product.

;**************************************************************************
;* AFXDLL.ASM
;*
;* Allocates space on the stack for AFX per-app globals
;**************************************************************************

NULL        SEGMENT WORD PUBLIC 'BEGDATA'
NULL        ENDS
AFX_NULL    SEGMENT WORD PUBLIC 'BEGDATA'
AFX_NULL    ENDS
_DATA       SEGMENT WORD PUBLIC 'DATA'
_DATA       ENDS
_BSS        SEGMENT WORD PUBLIC 'BSS'
_BSS        ENDS

DGROUP      GROUP NULL, AFX_NULL, _DATA, _BSS

;*****************************************************************************
; DLL Library provided WinMain and alloc stub routines

	extrn __acrtused:ABS        ; bring in C runtime
	extrn WINMAIN:FAR           ; force linking of app's WinMain entry point

	extrn __FPMATH:FAR	    ; bring in floating-point math

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
	ASSUME CS:_TEXT

;********************************************

;; Export linkage for CWinApp::_ForceDLLinkage()
public ?_ForceLinkage@CWinApp@@VECXXZ
?_ForceLinkage@CWinApp@@VECXXZ PROC FAR
	int 3       ; // should never be called
?_ForceLinkage@CWinApp@@VECXXZ ENDP

	DD __FPMATH		; Note: Forces linkage of WIN87EM.DLL

_TEXT ENDS

;*****************************************************************************
; This module contains data which must be at a fixed offset in the _DATA
; segment and allocate 160 bytes for AFX specific data

;; NOTE: the following structure must mirror struct AFX_APPDATA defined
;;   in MFC\INCLUDE\AFXDLL_.H

IFDEF _DEBUG
	extrn AFXTRACEV:FAR     ;; from app linked 'dumpout.obj' module
	extrn AFXASSERTFAILEDLINE:FAR ;; from app linked 'afxasert.obj' module
ENDIF
	extrn AFXAPPABORT:FAR   ;; from app linked 'afxabort.obj' module

	extrn AFXAPPSETNEWHANDLER:FAR   ;; from app linked 'afxmem.obj' module
	extrn AFXAPPALLOC:FAR   ;; from app linked 'afxmem.obj' module
	extrn AFXAPPFREE:FAR    ;; from app linked 'afxmem.obj' module
	extrn AFXAPPREALLOC:FAR ;; from app linked 'afxmem.obj' module

	extrn _AFXVBAPIENTRY:FAR    ;; VBX API vector from DLL

AFX_NULL    SEGMENT
	ASSUME CS:DGROUP
	ASSUME DS:DGROUP

	;* struct AFX_APPDATA (160 bytes max)
	DW      160         ; number of bytes reserved
	DW      0250H       ; MFC 2.5 (MFC250)

IFDEF _DEBUG
	DW      OFFSET AppDebug ; stack based pointer to debug stuff
	DW      0           ; reserved
else
	DD      0           ; reserved
ENDIF

	DD      0
	DD      0
	;* VBX HOOK (must be at SS:20)
	DD      _AFXVBAPIENTRY

	;* memory allocation routines
	DD      AFXAPPABORT
	DD      AFXAPPSETNEWHANDLER
	DD      AFXAPPALLOC
	DD      AFXAPPFREE
	DD      AFXAPPREALLOC
	DD      0
	DD      0

	DB      160-48 DUP (0)

IFDEF _DEBUG
;; NOTE: the following structure must mirror struct AFX_APPDEBUG defined
;;   in MFC\INCLUDE\AFXDLL_.H

	;* struct AFX_APPDEBUG (32 bytes max)
AppDebug:
	DD      AFXTRACEV
	DD      AFXASSERTFAILEDLINE
	DB      32-8 DUP (0)

ENDIF

AFX_NULL    ENDS

;*****************************************************************************

	END
