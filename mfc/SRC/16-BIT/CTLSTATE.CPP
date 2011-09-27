
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifndef _AFXCTL
#error This file is for the _AFXCTL build only
#endif

#include "toolhelp.h"
#include "winhand_.h"       // for creation of CHandleMap

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

extern "C" void AFX_EXPORT CALLBACK AfxAppAbort();
extern "C" FARPROC AFX_EXPORT CALLBACK AfxAppSetNewHandler(FARPROC pnh);
extern "C" void* AFX_EXPORT CALLBACK AfxAppAlloc(size_t nBytes);
extern "C" void AFX_EXPORT CALLBACK AfxAppFree(void* p);
extern "C" void* AFX_EXPORT CALLBACK AfxAppReAlloc(void* pOld, size_t nSize);
extern "C" void AFX_EXPORT CALLBACK AfxTraceV(LPCSTR lpszFormat, const void FAR* lpArgs);
extern "C" void AFXAPI AfxAssertFailedLine(LPCSTR lpszFileName, int nLine);

/////////////////////////////////////////////////////////////////////////////
//  Module data handling

static BYTE g_sExModuleData[sizeof(AFX_APPDATA_MODULE)];

AFX_APPDATA_MODULE* AFXAPI AfxGetBaseModuleContext()
{
	return (AFX_APPDATA_MODULE*) &g_sExModuleData;
}

AFX_APPDATA_MODULE* AFXAPI AfxGetCurrentModuleContext()
{
	return _AfxGetAppData()->m_pID;
}

AFX_APPDATA_MODULE* AFXAPI AfxPushModuleContext(AFX_APPDATA_MODULE* psIn)
{
	ASSERT(psIn != NULL);

	AFX_APPDATA_MODULE* psCurrentSource = AfxGetCurrentModuleContext();
	if (psCurrentSource == psIn)
		return NULL;

	AfxPopModuleContext(psIn);

	return psCurrentSource;
}

void AFXAPI AfxPopModuleContext(AFX_APPDATA_MODULE* psIn,
	BOOL bCopy /*=FALSE*/)
{
	AFX_APPDATA* psCurrent = _AfxGetAppData();
	AFX_APPDATA_MODULE* psOut = psCurrent->m_pID;

	ASSERT(psIn != psCurrent->m_pID);   // Same as current
	ASSERT(psIn == NULL || psIn == psIn->m_pID);    // Reference to original.
	ASSERT(psOut != NULL);  // App state is corrupted

	if (bCopy)
		_fmemcpy(psOut, psCurrent, sizeof(AFX_APPDATA_MODULE));
	else
	{
		// Copy only variables that may have changed.
		psOut->appDlgBkBrush = psCurrent->appDlgBkBrush;
		psOut->appDlgTextClr = psCurrent->appDlgTextClr;
	}

	// If psIn is NULL, then we just copy out.  Used for
	// initialization of the runtime DLL.

	if (psIn != NULL)
		_fmemcpy(psCurrent, psIn, sizeof(AFX_APPDATA_MODULE));
}

/////////////////////////////////////////////////////////////////////////////
//  Task data handling

BOOL bAppDataInit = FALSE;
static BOOL bTaskInit = FALSE;
static BYTE gcTaskMem[sizeof(CMapPtrToPtr)];

#define gcTaskList ((CMapPtrToPtr&)(*&gcTaskMem[0]))

static AFX_APPDATA* _AfxRegisterTask(HTASK hTask);
static void _AfxFreeTaskData(AFX_APPDATA* pAppData);
static void _AfxFlushUnusedTaskData();

#ifdef _DEBUG
static AFX_APPDATA* _AfxDebugGetAppData();

AFX_APPDATA* _AfxGetAppData()
{
	static int nInHere = 0;

	ASSERT(nInHere < 2);    // No recursing more than once.
	nInHere++;

	AFX_APPDATA* pAppData = _AfxDebugGetAppData();

	nInHere--;

	return pAppData;
}

AFX_APPDATA* _AfxDebugGetAppData()
#else
AFX_APPDATA* _AfxGetAppData()
#endif
{
	static HTASK hLastTask = NULL;
	static AFX_APPDATA *psLastRet = NULL;

	// Make sure the task map is initialized.
	if (!bTaskInit)
	{
		new ((void*)&gcTaskMem) CMapPtrToPtr;
		bTaskInit = TRUE;
	}

	AFX_APPDATA* pAppData = NULL;
	HTASK hTask = GetCurrentTask();

	if (hTask == hLastTask)
		return psLastRet;

	if (!gcTaskList.Lookup((void*) hTask, (void*&) pAppData))
		pAppData = _AfxRegisterTask(hTask);

	ASSERT(pAppData != NULL);

	psLastRet = pAppData;
	hLastTask = hTask;

	return pAppData;
}

#define new DEBUG_NEW

AFX_APPDATA* _AfxRegisterTask(HTASK hTask)
{
	AFX_APPDATA* pAppData;

	bAppDataInit = FALSE;

	// This task should not already be in the map.
	ASSERT(!gcTaskList.Lookup((void*) hTask, (void*&) pAppData));

	pAppData = (AFX_APPDATA*) calloc(1, sizeof(AFX_APPDATA));
	ASSERT(pAppData != NULL);

	// Copy in base module state.
	AFX_APPDATA_MODULE* pModuleData = AfxGetBaseModuleContext();
	_fmemcpy(pAppData, pModuleData, sizeof(AFX_APPDATA_MODULE));

	pAppData->m_pID = pModuleData;  // Initially points to base module.

	// Initialize task specific state.
	pAppData->cbSize = sizeof(AFX_APPDATA);
	pAppData->wVersion = 0x251;

	pAppData->lpfnAppAbort = AfxAppAbort;
	pAppData->lpfnAppSetNewHandler = AfxAppSetNewHandler;
	pAppData->lpfnAppAlloc = AfxAppAlloc;
	pAppData->lpfnAppFree = AfxAppFree;
	pAppData->lpfnAppReAlloc = AfxAppReAlloc;

#ifdef _DEBUG
	pAppData->pAppDebug = (AFX_APPDEBUG *) calloc(1, sizeof(AFX_APPDEBUG));
	ASSERT(pAppData->pAppDebug != NULL);

	pAppData->pAppDebug->lpfnTraceV = AfxTraceV;
	pAppData->pAppDebug->lpfnAssertFailed = AfxAssertFailedLine;
#endif

	// other initialization
	pAppData->appDlgTextClr = (COLORREF)-1; // use normal text

	// put the data in the map, since calls below will try to access it.
	gcTaskList.SetAt((void *) hTask, pAppData);

	bAppDataInit = TRUE;

	// allocate handle maps
	pAppData->appMapHGDIOBJ = new CHandleMap(RUNTIME_CLASS(CGdiObject),
		offsetof(CGdiObject, m_hObject));
	pAppData->appMapHDC = new CHandleMap(RUNTIME_CLASS(CDC),
		offsetof(CDC, m_hDC), 2);
	pAppData->appMapHMENU = new CHandleMap(RUNTIME_CLASS(CMenu),
		offsetof(CMenu, m_hMenu));
	pAppData->appMapHWND = new CHandleMap(RUNTIME_CLASS(CWnd),
		offsetof(CWnd, m_hWnd));

	// allocate extra data map
	pAppData->appMapExtra = new CMapPtrToPtr;
	pAppData->appMapExtra->InitHashTable(67);

	_AfxFlushUnusedTaskData();

#ifdef _DEBUG
	// turn on memory tracking from now on
	pAppData->pAppDebug->bMemoryTracking = TRUE;
#endif
	return pAppData;
}

void _AfxFlushUnusedTaskData()
{
	void* pvTask;
	void* pvAppData;

	POSITION pos = gcTaskList.GetStartPosition();
	while (pos != NULL)
	{
		gcTaskList.GetNextAssoc(pos, pvTask, pvAppData);
		if (IsTask((HTASK)(DWORD)pvTask))
			continue;

		// Free up data for tasks that have disappeared.
		_AfxFreeTaskData((AFX_APPDATA*)pvAppData);
		gcTaskList.RemoveKey(pvTask);
	}
}

void _AfxDestroyAllTaskData()
{
	HTASK hTaskCurrent = GetCurrentTask();
	void* pvTask;
	void* pvAppData;

	POSITION pos = gcTaskList.GetStartPosition();
	while (pos != NULL)
	{
		gcTaskList.GetNextAssoc(pos, pvTask, pvAppData);

		// remove current task later
		// REVIEW_BRENDANX: ever?
		if (((HTASK)(DWORD)pvTask) == hTaskCurrent)
			continue;

		_AfxFreeTaskData((AFX_APPDATA*)pvAppData);
	}

	gcTaskList.RemoveAll();
}

void _AfxFreeTaskData(AFX_APPDATA* pAppData)
{
	pAppData->appMapHGDIOBJ->DeleteTemp();
	delete pAppData->appMapHGDIOBJ;
	pAppData->appMapHDC->DeleteTemp();
	delete pAppData->appMapHDC;
	pAppData->appMapHMENU->DeleteTemp();
	delete pAppData->appMapHMENU;
	pAppData->appMapHWND->DeleteTemp();
	delete pAppData->appMapHWND;

	delete pAppData->appMapExtra;

#ifdef _DEBUG
	free(pAppData->pAppDebug);
#endif
	free(pAppData);
}
