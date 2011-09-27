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
/****************************************************************************
 *
 *  WAVEFILE.H
 *
 *  header file for routines for reading WAVE files
 *
 ***************************************************************************/

/*	-	-	-	-	-	-	-	-	*/
#include "extra.h"
#include "wavefile.rc"

extern HMODULE ghModule; // = NULL; // global HMODULE/HINSTANCE for resource access
/*	-	-	-	-	-	-	-	-	*/

/*
** This class is used to implement a handler for a type of file with only
** one stream.  In this case, we don't have to worry about allocating more
** than one stream object for each file object, so we can combine the
** two together in a single class.
**
*/

HRESULT WaveFileCreate(
	IUnknown FAR*	pUnknownOuter,
	REFIID		riid,
	void FAR* FAR*	ppv);

typedef struct {

	/*
	** This implementation of a file handler is done in C, not C++, so a few
	** things work differently than in C++.  Our structure contains Vtbls
	** (pointer to function tables) for three interfaces... Unknown, AVIStream,
	** and AVIFile, as well as our private data we need to implement the
	** handler.
	**
	*/

	IAVIStreamVtbl FAR	*AVIStream;
	IAVIFileVtbl FAR	*AVIFile;
	IUnknownVtbl FAR	*Unknown;
	IPersistFileVtbl FAR	*Persist;

	// This is our controlling object.
	IUnknown FAR*	pUnknownOuter;

	//
	// WaveFile instance data
	//
	HMMIO			hmmio;		// file I/O

	MMCKINFO		ckData;

	LONG			refs;		// for UNKNOWN
	AVISTREAMINFOW		avistream;	// for STREAM

	LPWAVEFORMAT		lpFormat;	// stream format
	LONG			cbFormat;
	BOOL			fDirty;
	UINT			mode;
	EXTRA			extra;
	AVIFILEINFOW		avihdr;
} WAVESTUFF, FAR *LPWAVESTUFF;

/*
** Whenever a function is called with a pointer to one of our Vtbls, we need
** to back up and get a pointer to the beginning of our structure.  Depending
** on which pointer we are passed, we need to back up a different number of
** bytes.  C++ would make this easier, by declaring backpointers.
*/

WAVESTUFF ws;
#define WAVESTUFF_FROM_UNKNOWN(pu)	(LPWAVESTUFF)((LPBYTE)(pu) - ((LPBYTE)&ws.Unknown - (LPBYTE)&ws))
#define WAVESTUFF_FROM_FILE(pf)		(LPWAVESTUFF)((LPBYTE)(pf) - ((LPBYTE)&ws.AVIFile - (LPBYTE)&ws))
#define WAVESTUFF_FROM_STREAM(ps)	(LPWAVESTUFF)((LPBYTE)(ps) - ((LPBYTE)&ws.AVIStream - (LPBYTE)&ws))
#define WAVESTUFF_FROM_PERSIST(ppf)	(LPWAVESTUFF)((LPBYTE)(ppf) - ((LPBYTE)&ws.Persist - (LPBYTE)&ws))

/*	-	-	-	-	-	-	-	-	*/

/*
** This class is our version of the IClassFactory interface.
**
** COMPOBJ.DLL expects our DllGetClassObject() function to return an
** IClassFactory object which it can then call to get the object that
** implements the IAVIFile interface to actually open the file.  Whew.
*/
typedef struct {
	IClassFactoryVtbl FAR *lpVtbl;

	/*
	** Data local to this object: reference counts for the object
	*/
	ULONG	ulRef;
	CLSID FAR *	clsid;
} WAVEFACTORY, FAR *LPWAVEFACTORY;

/*	-	-	-	-	-	-	-	-	*/

/*
** These variables help keep track of whether the DLL is still in use,
** so that when our DllCanUnloadNow() function is called, we know what
** to say.
*/

extern UINT	uUseCount;
extern UINT	uLockCount;

/*	-	-	-	-	-	-	-	-	*/

//
// This is our unique identifier
//
//  NOTE: If you modify this sample code to do something else, you MUST
//	    CHANGE THIS!
//
//  Run uuidgen.exe from the tools directory and get your own GUID.
//  DO NOT USE THIS ONE!
//
//
//
DEFINE_GUID(CLSID_AVIWaveFileReader, 0x00020003, 0, 0, 0xC0,0,0,0,0,0,0,0x46);
