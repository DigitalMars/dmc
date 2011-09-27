/***
*tibrowse.cpp
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  Type Information Browser
*
*
*Implementation Notes:
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tibrowse.h"

#ifdef _MAC
DialogPtr g_pdlg = NULL;
ListHandle g_rghlist[IDC_PARAMLIST+1] = {NULL};
#else
HWND g_hwnd;
char g_szAppName[] = "TiBrowse";
#endif

ITypeLib  FAR *g_ptlib = NULL;
ITypeInfo FAR *g_ptinfoCur = NULL;
TYPEATTR  FAR *g_ptattrCur = NULL;

char * g_rgszTKind[] = {
	"Enum",             /* TKIND_ENUM */
	"Struct",           /* TKIND_RECORD */
	"Module",           /* TKIND_MODULE */
	"Interface",        /* TKIND_INTERFACE */
	"Dispinterface",    /* TKIND_DISPATCH */
	"Coclass",          /* TKIND_COCLASS */
	"Typedef",          /* TKIND_ALIAS */
	"Union",            /* TKIND_UNION */
};


// Set the text of the control identified by the given control id
void
XSetDlgItemText(int ctlid, char FAR* psz)
{
#ifdef _MAC
	Rect rc;
	Handle h;
	short kind;
	char buf[255];

	strcpy(buf, psz);
	GetDItem(g_pdlg, ctlid, &kind, &h, &rc);
	SetIText(h, c2pstr(buf));
#else
	SendDlgItemMessage(g_hwnd, ctlid, WM_SETTEXT, 0, (LPARAM)psz);
#endif
}

// Clear the listbox identified by the given control id
void
XClrDlgItemList(int ctlid)
{
#ifdef _MAC
	LDelRow(0, 0, g_rghlist[ctlid]); // delete everything
#else
	SendMessage(GetDlgItem(g_hwnd, ctlid), LB_RESETCONTENT, 0, 0L);
#endif
}

// Add the given string to the listbox identified by the given control id
void
XAddDlgItemList(int ctlid, char FAR* psz)
{
#ifdef _MAC
	int row;
	Point pt;
	ListHandle hlist;

	hlist = g_rghlist[ctlid];
	row = LAddRow(1, -1, hlist);
	pt.v = row, pt.h = 0;
	LSetCell(psz, strlen(psz), pt, hlist);
#else
	SendDlgItemMessage(g_hwnd, ctlid, LB_ADDSTRING, 0, (LPARAM)psz);
#endif
}

void Cleanup()
{
	if(g_ptinfoCur != NULL){
	  if(g_ptattrCur != NULL)
		g_ptinfoCur->ReleaseTypeAttr(g_ptattrCur);
	  g_ptinfoCur->Release();
	  g_ptinfoCur = NULL;
	}
	if(g_ptlib != NULL){
	  g_ptlib->Release();
	  g_ptlib = NULL;
	}
}

void Uninit()
{
	OleUninitialize();
#ifdef _MAC
	CleanupLibraryManager();
#endif
}

void
OpenTypeLib(char FAR *sztlib)
{
	unsigned int utypeinfoCount, i;
	BSTR bstrName;
	TLIBATTR FAR* ptlibattr;

	/* clear out globals */
	Cleanup();

	/* clear out listboxes */
	XClrDlgItemList(IDC_TYPELIST);
	XClrDlgItemList(IDC_MEMBERLIST);
	XClrDlgItemList(IDC_PARAMLIST);

	/* load the type library */
	CHECKRESULT(LoadTypeLib(sztlib, &g_ptlib));

	/* get library attributes for the fun of it */
	CHECKRESULT(g_ptlib->GetLibAttr(&ptlibattr));

	/* release library attributes */
	g_ptlib->ReleaseTLibAttr(ptlibattr);

	/* Now add each of the names to the type list */
	utypeinfoCount = g_ptlib->GetTypeInfoCount();
	for(i = 0; i < utypeinfoCount; i++){
	  CHECKRESULT(g_ptlib->GetDocumentation(i, &bstrName, NULL, NULL, NULL));
	  ASSERT(bstrName);
	  XAddDlgItemList(IDC_TYPELIST, bstrName);
	  SysFreeString(bstrName);
	}
}

/***
*void SetSelectedType
*Purpose:
*  When the user changes the selection of a type, this function updates the
*  dialog by changing the member list and the help for the type. It also sets
*  g_ptinfoCur to refer to the typeinfo.
*
*Entry:
*  dwIndex =
*
*Exit:
*  return value = None
*
***********************************************************************/
void
SetSelectedType(unsigned long dwIndex)
{
	HRESULT hr;
	BSTR bstrDoc;
	char FAR* psz;
	char szBuf[40];
	unsigned long dwHelpContext;
	TYPEKIND tkind;

	if(g_ptinfoCur != NULL){
	  g_ptinfoCur->ReleaseTypeAttr(g_ptattrCur);
	  g_ptinfoCur->Release();
	}

	/* Clear out the member list */
	XClrDlgItemList(IDC_MEMBERLIST);

	if(g_ptlib == NULL)
	  return;

	// Note: the index in the list box is conveniently the same as the
	// one to pass to GetTypeInfo/GetTypeInfoType

	// typeinfo type can be obtained without actually loading the typeinfo
	CHECKRESULT(g_ptlib->GetTypeInfoType((unsigned int)dwIndex, &tkind));
	XSetDlgItemText(IDC_TYPEKIND, g_rgszTKind[tkind]);

	CHECKRESULT(g_ptlib->GetTypeInfo((unsigned int)dwIndex, &g_ptinfoCur));
	CHECKRESULT(g_ptinfoCur->GetTypeAttr(&g_ptattrCur));

	// GUID
	hr = StringFromCLSID(g_ptattrCur->guid, &psz);
	ASSERT(hr == NOERROR);
	XSetDlgItemText(IDC_GUID, psz);
	MemFree(psz);

	// Version
	sprintf(szBuf, "%u.%02u",
	  g_ptattrCur->wMajorVerNum, g_ptattrCur->wMinorVerNum);
	XSetDlgItemText(IDC_VERSION, szBuf);

	CHECKRESULT(
	  g_ptlib->GetDocumentation(
		(unsigned int)dwIndex, NULL, &bstrDoc, &dwHelpContext, NULL));

	// Help Context
	sprintf(szBuf, "%ld", dwHelpContext);
	XSetDlgItemText(IDC_HELPCONTEXT, szBuf);

	// Documentation string
	psz = (bstrDoc != NULL) ? bstrDoc : "<none>";
	XSetDlgItemText(IDC_HELPSTRING, psz);
	SysFreeString(bstrDoc);

	FillMemberList(g_ptinfoCur, g_ptattrCur, IDC_MEMBERLIST);

	XClrDlgItemList(IDC_PARAMLIST);
}

/***
*void FillMemberList
*Purpose:
* Sets the current typeinfo to the typeinfo indexed by dwIndex, and
* then fills in the list box with the members of the type.
*
*Entry:
*  ptinfo =
*  ptypeattr =
*
*Exit:
*  return value = None
*
***********************************************************************/
void
FillMemberList(
	ITypeInfo FAR *ptinfo,
	TYPEATTR FAR *ptypeattr,
	int ctlid)
{
	MEMBERID memid;
	BSTR bstrName;
	unsigned int i;
	FUNCDESC FAR *pfuncdesc;
	VARDESC  FAR *pvardesc;

	/* Now add all of the functions and all of the vars.
	 * This is somewhat roundabout.
	 * For each one, we need to get the funcdesc, or the vardesc.
	 * From that we get the MEMBERID, and finally can get to the name.
	 */
	for(i = 0; i < ptypeattr->cFuncs; i++){
	  CHECKRESULT(ptinfo->GetFuncDesc(i, &pfuncdesc));
	  memid = pfuncdesc->memid;
	  CHECKRESULT(ptinfo->GetDocumentation(memid, &bstrName, NULL, NULL, NULL));
	  ptinfo->ReleaseFuncDesc(pfuncdesc);
	  pfuncdesc = NULL;

	  ASSERT(bstrName);
	  XAddDlgItemList(ctlid, bstrName);
	  SysFreeString(bstrName);
	}

	for(i = 0; i < ptypeattr->cVars; i++)
	{
	  CHECKRESULT(ptinfo->GetVarDesc(i, &pvardesc));
	  memid = pvardesc->memid;
	  CHECKRESULT(ptinfo->GetDocumentation(memid, &bstrName, NULL, NULL, NULL));
	  ptinfo->ReleaseVarDesc(pvardesc);
	  pvardesc = NULL;

	  ASSERT(bstrName);
	  XAddDlgItemList(ctlid, bstrName);
	  SysFreeString(bstrName);
	}
}

/***
*void SetSelectedMember
*Purpose:
*  When a member of a type is selected, update the help to be the help
*  of the member, and if the member is a function update the parameter
*  list to reflect that it is a function.
*
*Entry:
*  dwIndex =
*
*Exit:
*  return value = None
*
***********************************************************************/
void
SetSelectedMember(unsigned long dwIndex)
{
	MEMBERID memid;

	/* In any case, we'll need to clear out the parameter list. */
	XClrDlgItemList(IDC_PARAMLIST);

	if(g_ptattrCur == NULL)
	  return;

	/* if this is a function, fill the param list, otherwise just fill
	 * in the item info.
	 */
	if(dwIndex < g_ptattrCur->cFuncs){
	  unsigned short i;
	  unsigned int cNames;
	  FUNCDESC FAR *pfuncdesc;
	  const unsigned int MAX_NAMES = 40;
	  BSTR rgNames[MAX_NAMES];

	  CHECKRESULT(g_ptinfoCur->GetFuncDesc((unsigned int) dwIndex, &pfuncdesc));
	  memid = pfuncdesc->memid;
	  UpdateMemberInfo(memid);

	  CHECKRESULT(g_ptinfoCur->GetNames(memid, rgNames, MAX_NAMES,&cNames));
	  for(i = 1; i < cNames; i++){
		ASSERT(rgNames[i])
		XAddDlgItemList(IDC_PARAMLIST, rgNames[i]);
		SysFreeString(rgNames[i]);
	  }
	  ASSERT(rgNames[0]);
	  SysFreeString(rgNames[0]);
	  g_ptinfoCur->ReleaseFuncDesc(pfuncdesc);
	}
	else
	{
	  VARDESC FAR *pvardesc;

	  CHECKRESULT(
		g_ptinfoCur->GetVarDesc(
		  (unsigned int)(dwIndex - g_ptattrCur->cFuncs), &pvardesc));
	  memid = pvardesc->memid;
	  UpdateMemberInfo(memid);
	  g_ptinfoCur->ReleaseVarDesc(pvardesc);
	}
}

/***
*void UpdateMemberInfo
*Purpose:
*  sets fields on the dialog (such as help string and help context) from
*  the type information.
*
*Entry:
*  memid =
*
*Exit:
*  return value = None
*
***********************************************************************/
void
UpdateMemberInfo(MEMBERID memid)
{
	BSTR bstrDoc;
	char buf[40];
	unsigned long dwHelpContext;

	/* get the member information */
	CHECKRESULT(g_ptinfoCur->GetDocumentation(
	  memid, NULL, &bstrDoc, &dwHelpContext, NULL));

	/* update the help string displayed in the dialog */
	XSetDlgItemText(IDC_HELPSTRING, (bstrDoc != NULL) ? bstrDoc : "<none>");
	SysFreeString(bstrDoc);

	/* update the help context displayed in the dialog */
	sprintf(buf, "%ld", dwHelpContext);
	XSetDlgItemText(IDC_HELPCONTEXT, buf);
}

/***
*void SetSelectedParam
*Purpose:
*  CONSIDER: Enhance to show parameter type information here.
*
*Entry:
*  dwIndex =
*
*Exit:
*  return value = None
*
***********************************************************************/
void
SetSelectedParam(unsigned long dwIndex)
{
}


HRESULT
GetFileName(BSTR FAR* pbstrFileName)
{
	char buf[1024];
	BSTR bstrFileName;

#ifdef _MAC

	int i;
	Point pt;
	long dirID;
	char *p, *q;
	SFReply sfr;
	SFTypeList sftl;
	short sRefNumReal;

	SetPt(&pt, 100, 100);
	//sftl[0] = 'OTLB';
	sftl[0] = 0; // currently allow all file types
	SFGetFile(pt, NULL, NULL, -1, sftl, NULL, &sfr);

	if(!sfr.good)
	  return ResultFromScode(E_FAIL);

	// translate sfr.vRefNum into a dirID
	{
		WDPBRec wdpbr;

		wdpbr.ioWDIndex   = 0;
		wdpbr.ioVRefNum   = sfr.vRefNum;
		wdpbr.ioWDVRefNum = 0;
		wdpbr.ioWDIndex   = 0;
		wdpbr.ioWDProcID  = 0;
		wdpbr.ioNamePtr   = (StringPtr)buf;

		if(PBGetWDInfoSync (&wdpbr) != noErr)
		  return ResultFromScode(E_FAIL);

		sRefNumReal = wdpbr.ioWDVRefNum;
		dirID = wdpbr.ioWDDirID;
	}

	{
		Str255 stDir;
		CInfoPBRec pb;
		DirInfo *pdirinfo = (DirInfo*)&pb;
		char rgchTemp[256]; // REVIEW: what should the size be?

		pdirinfo->ioNamePtr = stDir;
		pdirinfo->ioVRefNum = sRefNumReal;
		pdirinfo->ioDrParID = dirID;
		pdirinfo->ioFDirIndex = -1;  // get info on a dir

		buf[0] = '\0';

		// loop until we hit the root dir
		do{
		  pdirinfo->ioDrDirID = pdirinfo->ioDrParID;
		  if(PBGetCatInfoSync (&pb) != noErr)
			return ResultFromScode(E_FAIL);
		  strncpy(rgchTemp, (char*)stDir, 64);
		  p2cstr((StringPtr)rgchTemp);
		  strcat(rgchTemp, ":");
		  strcat(rgchTemp, buf);
		  strcpy(buf, rgchTemp);
		}while(pdirinfo->ioDrDirID != fsRtDirID);

	}

	q = (char*)sfr.fName;
	i = (int)*q++;
	p = &buf[strlen(buf)];
	while(i-->0)
	  *p++ = *q++;
	*p = '\0';

	if((bstrFileName = SysAllocString(buf)) == NULL)
	  return ResultFromScode(E_OUTOFMEMORY);

#else

	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize  = sizeof(OPENFILENAME);
	ofn.hwndOwner    = g_hwnd;
	ofn.lpstrFile    = (LPSTR)&buf;
	ofn.nMaxFile     = sizeof(buf);
	*buf = '\0';
	ofn.lpstrFilter  = "Type Libraries\0*.tlb\0\0";
	ofn.nFilterIndex = 1;
	ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(GetOpenFileName(&ofn) == 0){
	  // REVIEW: we can probably do something smarter with this error
	  DWORD dwerr = CommDlgExtendedError();
	  return ResultFromScode(E_FAIL);
	}
	if((bstrFileName = SysAllocString(ofn.lpstrFile)) == NULL)
	  return ResultFromScode(E_OUTOFMEMORY);

#endif

	*pbstrFileName = bstrFileName;
	return NOERROR;
}

void
AssertFail(char FAR* file, int line)
{
	char buf[128];

	sprintf(buf, "Assertion failed: %s(%d)", file, line);

#ifdef _MAC
	DebugStr(c2pstr(buf));
#else
	OutputDebugString(buf);
	if(MessageBox(NULL, buf, "TiBrowse Assertion.  OK to continue, CANCEL to quit.", MB_OKCANCEL | MB_TASKMODAL) == IDCANCEL)
	  exit(1);
	DebugBreak();
#endif
}

#ifdef _MAC /* { */

void Init(void);
void EventLoop(void);

void AdjustMenus(void);
void DoEvent(EventRecord *pevent);
void DoMenuCommand(long menuResult);
void Quit(void);
void Fatal(Str255);

Boolean
IsAppWindow(WindowPtr window)
{
	return (window == NULL)
	  ? false : ((WindowPeek)window)->windowKind == userKind;
}

Boolean
IsDAWindow(WindowPtr window)
{
	return (window == NULL)
	  ? false : (((WindowPeek)window)->windowKind < 0);
}

void
main()
{
	Init();
//    ShowWindow(g_pdlg); // REVIEW: necessarry?
	EventLoop();
}

void
DoDialogEvent(EventRecord *pevent, short sItem)
{
	Point pt;
	GrafPtr pport;
	ListHandle hlist;

	switch(sItem){
	case IDC_TYPELIST:
	case IDC_MEMBERLIST:
	case IDC_PARAMLIST:
	  if(pevent->what == mouseDown){
		GetPort(&pport);
		SetPort(g_pdlg);
		GlobalToLocal(&pevent->where);
		hlist = g_rghlist[sItem];
		LClick(pevent->where, pevent->modifiers, hlist);
		SetPt(&pt, 0, 0);
		if(LGetSelect(true, &pt, hlist)){
		  switch(sItem){
		  case IDC_TYPELIST:
			SetSelectedType(pt.v);
			break;
		  case IDC_MEMBERLIST:
			SetSelectedMember(pt.v);
			break;
		  case IDC_PARAMLIST:
			SetSelectedParam(pt.v);
			break;
		  }
		}
		SetPort(pport);
	  }
	  break;
	}
}

void
EventLoop()
{
	short sItem;
	DialogPtr pdlg;
	EventRecord event;
	RgnHandle cursorRgn;

	cursorRgn = NewRgn();
	while(1){
	  if(WaitNextEvent(everyEvent, &event, MAXLONG, cursorRgn)){
		if (FrontWindow() != NULL
		 && event.what != diskEvt
		 && (event.what != keyDown || (event.modifiers & cmdKey) == 0)
		 && IsDialogEvent(&event))
		{
		  if(DialogSelect(&event, &pdlg, &sItem)){
			ASSERT(pdlg == g_pdlg);
			DoDialogEvent(&event, sItem);
		  }
		}else{
		  DoEvent(&event);
		}
	  }
	}
}

void
DoEvent(EventRecord *pevent)
{
	char key;
	short part;
	WindowPtr window;

	switch(pevent->what){
	case mouseDown:
	  part = FindWindow(pevent->where, &window);
	  switch(part){
	  case inMenuBar:
		AdjustMenus();
		DoMenuCommand(MenuSelect(pevent->where));
		break;

	  case inSysWindow: /* let the system handle the mouseDown */
		SystemClick(pevent, window);
		break;

	  case inContent:
		if(window != FrontWindow()){
		  SelectWindow(window);
		}
		break;

	  case inDrag:
		DragWindow(window, pevent->where, &qd.screenBits.bounds);
		break;
	  }
	  break;

	case keyDown:
	case autoKey:                       /* check for menukey equivalents */
	  key = (char)(pevent->message & charCodeMask);
	  if(pevent->modifiers & cmdKey){   /* Command key down */
		if(pevent->what == keyDown){
		  /* enable/disable/check menu items properly */
		  AdjustMenus();
		  DoMenuCommand(MenuKey(key));
		}
	  }
	  break;
	}
}

void
Enable(MenuHandle hmenu, short sItem, Boolean fEnable)
{
	if(fEnable)
	  EnableItem(hmenu, sItem);
	else
	  DisableItem(hmenu, sItem);
}

void
AdjustMenus()
{
	Boolean fIsDA;
	MenuHandle hmenu;

	fIsDA = IsDAWindow(FrontWindow());

	/* we can allow desk accessories to be closed from the menu */
	hmenu = GetMHandle(mFile);
	Enable(hmenu, iClose, fIsDA);

	hmenu = GetMHandle(mEdit);
	Enable(hmenu, iUndo,  fIsDA);
	Enable(hmenu, iCut,   fIsDA);
	Enable(hmenu, iCopy,  fIsDA);
	Enable(hmenu, iPaste, fIsDA);
	Enable(hmenu, iClear, fIsDA);
}

void
DoMenuCommand(long menuResult)
{
	short menuID;               /* the resource ID of the selected menu */
	short menuItem;             /* the item number of the selected menu */
	Str255 daName;

	menuID = HiWord(menuResult);
	menuItem = LoWord(menuResult);

	switch(menuID){
	case mApple:
	  switch(menuItem){
	  case iAbout:              /* bring up alert for About */
		Alert(rAboutAlert, NULL);
		break;
	  default:
		GetItem(GetMHandle(mApple), menuItem, daName);
		OpenDeskAcc(daName);
		break;
	  }
	  break;

	case mFile:
	  switch(menuItem){
	  case iOpen:
	{
		BSTR bstrFileName;
		if(GetFileName(&bstrFileName) != NOERROR)
		{
		  // REVIEW: shouldnt just bag out here
		  Fatal("\pUnable to open Type Library");
		}
		OpenTypeLib(bstrFileName);
	}
		break;

	  case iQuit:
		Quit();
		break;
	  }
	  break;

	case mEdit:
	  SystemEdit(menuItem-1);
	  break;
	}

	HiliteMenu(0);
}

extern "C" {

pascal void
DrawListbox(DialogPtr pdlg, short sItem)
{
	Rect rc;
	Handle h;
	short kind;
	GrafPtr port;
	ListHandle hlist;

	ASSERT(sItem == IDC_TYPELIST
		|| sItem == IDC_MEMBERLIST
		|| sItem == IDC_PARAMLIST);

	GetPort(&port);
	SetPort(pdlg);
	hlist = g_rghlist[sItem];
	LUpdate(pdlg->visRgn, hlist);
	GetDItem(pdlg, sItem, &kind, &h, &rc);
	InsetRect(&rc, -1, -1);
	FrameRect(&rc);
	SetPort(port);
}

}


/***
*ListHandle CreateListBox(DialogPtr, int)
*Purpose:
*  Create a listbox on the given dialog, associated with the given
*  control id.
*
*Entry:
*  pdlg =
*  ctlid =
*
*Exit:
*  return value = ListHandle
*
***********************************************************************/
ListHandle
CreateListbox(DialogPtr pdlg, int ctlid)
{
	Handle h;
	short kind;
	Point ptCell;
	ListHandle hlist;
	Rect rcView, rcBounds;

	GetDItem(pdlg, ctlid, &kind, &h, &rcView);
	SetDItem(pdlg, ctlid, kind, (Handle)DrawListbox, &rcView);

	// make room for scroll bars
	rcView.right -= 15;

	// list is 1 wide
	SetRect(&rcBounds, 0, 0, 1, 0);

	SetPt(&ptCell, 0, 0);

	hlist = LNew(
	  &rcView,
	  &rcBounds,
	  ptCell,
	  0, (WindowPtr)pdlg, true, false, false, true);

	ASSERT(hlist != NULL);

	(*hlist)->selFlags = lUseSense | lNoRect | lNoExtend | lOnlyOne;

	return hlist;
}

/***
*DialogPtr CreateDialog(void)
*Purpose:
*  Create the TiBrowse dialog
*
*Entry:
*  None
*
*Exit:
*  return value = DialogPtr
*
***********************************************************************/
DialogPtr
CreateDialog()
{
	int i;
	Point pt;
	DialogPtr pdlg;
	ListHandle hlist;
static short rgctl[] = {IDC_TYPELIST, IDC_MEMBERLIST, IDC_PARAMLIST};

	if((pdlg = GetNewDialog(rDlg, NULL, (WindowPtr)-1)) == NULL)
	  return NULL;

	for(i = 0; i < DIM(rgctl); ++i){
	  g_rghlist[rgctl[i]]  = CreateListbox(pdlg, rgctl[i]);
	}

	SetPt(&pt, 0, 0);
	for(i = 0; i < DIM(rgctl); ++i){
	  hlist = g_rghlist[rgctl[i]];
	  LSetSelect(true, pt, hlist);
	  LDoDraw(true, hlist);
	}

	return pdlg;
}

void
Init()
{
	Handle menuBar;

	MaxApplZone();

	InitGraf((Ptr)&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(NULL);
	InitCursor();
	FlushEvents(everyEvent, 0);

	if(InitLibraryManager(0, kCurrentZone, kNormalMemory) != noErr)
	  Fatal("\pCould not initialize LibraryManager");

	if(OleInitialize(NULL) != NOERROR)
	  Fatal("\pUnable to Initialize Ole");

	if((menuBar = GetNewMBar(rMenuBar)) == NULL)
	  Fatal("\pUnable to load menu bar");
	SetMenuBar(menuBar);
	DisposHandle(menuBar);
	AddResMenu(GetMHandle(mApple), 'DRVR');
	DrawMenuBar();

	if((g_pdlg = CreateDialog()) == NULL)
	  Fatal("\pUnable to create dialog");
}

void
Quit()
{
	Cleanup();
	Uninit();
	ExitToShell();
}

/* display fatal error alert, and exit */
void
Fatal(Str255 msg)
{
	SetCursor(&qd.arrow);
	ParamText(msg, "\p", "\p", "\p");
	Alert(rUserAlert, NULL);
	Quit();
}

#else /* }{ */

long FAR PASCAL EXPORT WndProc (HWND, UINT, WPARAM, LPARAM) ;

int PASCAL
WinMain(
	HANDLE hinst,
	HANDLE hinstPrev,
	LPSTR lpszCmdLine,
	int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass;

	if(!hinstPrev){
	  wndclass.style          = CS_HREDRAW | CS_VREDRAW;
	  wndclass.lpfnWndProc    = WndProc ;
	  wndclass.cbClsExtra     = 0 ;
	  wndclass.cbWndExtra     = DLGWINDOWEXTRA ;
	  wndclass.hInstance      = hinst ;
	  wndclass.hIcon          = LoadIcon (hinst, g_szAppName) ;
	  wndclass.hCursor        = LoadCursor (NULL, IDC_ARROW) ;
	  wndclass.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1) ;
	  wndclass.lpszMenuName   = NULL ;
	  wndclass.lpszClassName  = g_szAppName ;

	  RegisterClass (&wndclass) ;
	}

	if(OleInitialize(NULL) != NOERROR){
	  MessageBox(NULL, "unable to initialize Ole", g_szAppName, MB_OK);
	  return 0;
	}

	g_hwnd = CreateDialog(hinst, g_szAppName, 0, NULL);

	if(!lstrcmp(lpszCmdLine, "")){
	  BSTR bstrFileName;
	  if(GetFileName(&bstrFileName) != NOERROR){
		Cleanup();
		Uninit();
		exit(-1);
	  }
	  OpenTypeLib(bstrFileName);
	  SysFreeString(bstrFileName);
	}else{
	  OpenTypeLib(lpszCmdLine);
	}

	ShowWindow(g_hwnd, nCmdShow);

	while(GetMessage (&msg, NULL, 0, 0)){
	  TranslateMessage (&msg);
	  DispatchMessage (&msg);
	}
	Cleanup();
	Uninit();
	return msg.wParam;
}

long FAR PASCAL EXPORT
WndProc(
	HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	unsigned long dwIndex;

	switch(message){
	case WM_COMMAND:
	  /* We deal with the following events:
	   * The selection changes on the type list and we have
	   *  to update the member list & type info.
	   * The selection changes on the member list and we have
	   *  to update the param list & member info.
	   * Selection changes on a parameter and we have to
	   *  update the param info.
	   */
#ifdef WIN32
# define wParamX LOWORD(wParam)
# define notifyMsg HIWORD(wParam)
#else
# define wParamX wParam
# define notifyMsg HIWORD(lParam)
#endif
	  switch(wParamX){
	  case IDC_TYPELIST:
		if(notifyMsg == LBN_SELCHANGE){
		  dwIndex = SendDlgItemMessage(hwnd, IDC_TYPELIST, LB_GETCURSEL, 0, 0L);
		  if(dwIndex != LB_ERR)
			SetSelectedType(dwIndex);
		}
		break;
	  case IDC_MEMBERLIST:
		if(notifyMsg == LBN_SELCHANGE){
		  dwIndex = SendDlgItemMessage(hwnd, IDC_MEMBERLIST, LB_GETCURSEL,0,0L);
		  if(dwIndex != LB_ERR)
			SetSelectedMember(dwIndex);
		}
		break;
	  case IDC_PARAMLIST:
		if(notifyMsg == LBN_SELCHANGE){
		  dwIndex = SendDlgItemMessage(hwnd, IDC_PARAMLIST, LB_GETCURSEL, 0,0L);
		  if(dwIndex != LB_ERR)
			SetSelectedParam(dwIndex);
		}
		break;
	  }
	  return 0;

	case WM_DESTROY:
	  PostQuitMessage(0);
	  return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

#endif /* } */

void
MethodError(HRESULT hresult)
{
	/* CONSIDER: add code to figure out what specific error this is */
	UNUSED(hresult);

#ifdef _MAC
	Fatal("\pType Library");
#else
	MessageBox(NULL, "Error returned from TYPELIB.DLL", g_szAppName, MB_OK);
	exit(1);
#endif
}

/* free using the task allocator */
void
MemFree(void FAR* pv)
{
	HRESULT hresult;
	IMalloc FAR* pmalloc;

	hresult = CoGetMalloc(MEMCTX_TASK, &pmalloc);

	if(hresult != NOERROR){
#ifdef _MAC
	  Fatal("\pError accessing task allocator");
#else
	  MessageBox(NULL, "Error accessing task allocator", g_szAppName, MB_OK);
#endif
	  return;
	}

	pmalloc->Free(pv);
	pmalloc->Release();
}
