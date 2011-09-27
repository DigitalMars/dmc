/*
 * OLE2UI.C
 *
 * Contains initialization routines and miscellaneous API implementations for
 * the OLE 2.0 User Interface Support Library.
 *
 * Copyright (c)1992 Microsoft Corporation, All Right Reserved
 */

#define STRICT  1

#include "ole2ui.h"
#include "common.h"
#include "utility.h"
#include "resimage.h"
#include "iconbox.h"
#include <commdlg.h>

#define WINDLL  1           // make far pointer version of stdargs.h
#include <stdarg.h>

// NOTE: If this code is being compiled for a DLL, then we need to define
// our OLE2UI debug symbols here (with the OLEDBGDATA_MAIN macro).  If we're
// compiling for a static LIB, then the application we link to must
// define these symbols -- we just need to make an external reference here
// (with the macro OLEDBGDATA).

#ifdef DLL_VER
OLEDBGDATA_MAIN("OLE2UI")
#else
OLEDBGDATA
#endif

#if defined( _DEBUG ) && !defined( DLL_VER )
// This dummy variable is needed in order for the static link version
// of this library to work correctly.  When we include PRECOMP.OBJ
// in our library (.LIB file), it will only get linked into our
// application IF at least one symbol in precomp.c is referenced from
// either our EXE or LIB.  Therefore, we will define a variable in
// PRECOMP.C and reference it in OLE2UI.C which includes the function
// OleUIInitialize which MUST be called by the static lib user.

extern int near g_nOleUIStaticLibDummy;
int near* g_pnOleUIStaticLibDummy = &g_nOleUIStaticLibDummy;
#endif

//The DLL instance handle shared amongst all dialogs.
HINSTANCE     ghInst;

// object count, used to support DllCanUnloadNow and OleUICanUnloadNow
DWORD g_dwObjectCount=0;
DWORD g_dwLockCount=0;

//Registered messages for use with all the dialogs, registered in LibMain
UINT        uMsgHelp=0;
UINT        uMsgEndDialog=0;
UINT        uMsgBrowse=0;
UINT        uMsgChangeIcon=0;
UINT        uMsgFileOKString=0;
UINT        uMsgCloseBusyDlg=0;

//Clipboard formats used by PasteSpecial
UINT  cfObjectDescriptor;
UINT  cfLinkSrcDescriptor;
UINT  cfEmbedSource;
UINT  cfEmbeddedObject;
UINT  cfLinkSource;
UINT  cfOwnerLink;
UINT  cfFileName;

// local function prototypes
BOOL CALLBACK EXPORT PromptUserDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EXPORT UpdateLinksDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


// local definition
#define WM_U_UPDATELINK WM_USER


// local structure definition
typedef struct tagUPDATELINKS
{
	LPOLEUILINKCONTAINER    lpOleUILinkCntr;    // pointer to Link Container
	UINT                    cLinks;             // total number of links
	UINT                    cUpdated;           // number of links updated
	DWORD                   dwLink;             // pointer to link
	BOOL                    fError;             // error flag
	LPSTR                   lpszTitle;          // caption for dialog box
} UPDATELINKS, *PUPDATELINKS, FAR *LPUPDATELINKS;


/*
 * OleUIInitialize
 *
 * NOTE: This function should only be called by your application IF it is
 * using the static-link version of this library.  If the DLL version is
 * being used, this function is automatically called from the OLE2UI DLL's
 * LibMain.
 *
 * Purpose:
 *   Initializes the OLE UI Library.
 *
 * Parameters:
 *
 *  hInstance       HINSTANCE of the module where the UI library resources
 *                  and Dialog Procedures are contained.  If you are calling
 *                  this function yourself, this should be the instance handle
 *                  of your application.
 *
 *  hPrevInst       HINSTANCE of the previous application instance.
 *                  This is the parameter passed in to your WinMain.  For
 *                  the DLL version, this should always be set to zero (for
 *                  WIN16 DLLs).
 *
 * Return Value:
 *  BOOL            TRUE if initialization was successful.
 *                  FALSE if either the "Magic Number" did not verify, or one of
 *                  the window classes could not be registered.  If the
 *                  "Magic Number" did not verify, then the resources
 *                  in your module are of a different version than the
 *                  ones you compiled with.
 */

STDAPI_(BOOL) OleUIInitialize(HINSTANCE hInstance, HINSTANCE hPrevInst)
{
	HRSRC   hr;
	HGLOBAL hg;
	LPWORD lpdata;

	OleDbgOut1("OleUIInitialize called.\r\n");
	ghInst=hInstance;

	// Verify that we have the correct resources added to our application
	// by checking the "VERIFICATION" resource with the magic number we've
	// compiled into our app.

	if ((hr = FindResource(hInstance, "VERIFICATION", RT_RCDATA)) == NULL)
		goto ResourceLoadError;

	if ((hg = LoadResource(hInstance, hr)) == NULL)
		goto ResourceLoadError;

	if ((lpdata = (LPWORD)LockResource(hg)) == NULL)
		goto ResourceLockError;

	if ((WORD)*lpdata != (WORD)OLEUI_VERSION_MAGIC)
		goto ResourceReadError;

	// OK, resource versions match.  Contine on.
	UnlockResource(hg);
	FreeResource(hg);
	OleDbgOut1("OleUIInitialize: Resource magic number verified.\r\n");

	// Register messages we need for the dialogs.  If
	uMsgHelp      =RegisterWindowMessage(SZOLEUI_MSG_HELP);
	uMsgEndDialog =RegisterWindowMessage(SZOLEUI_MSG_ENDDIALOG);
	uMsgBrowse    =RegisterWindowMessage(SZOLEUI_MSG_BROWSE);
	uMsgChangeIcon=RegisterWindowMessage(SZOLEUI_MSG_CHANGEICON);
	uMsgFileOKString = RegisterWindowMessage(FILEOKSTRING);
	uMsgCloseBusyDlg = RegisterWindowMessage(SZOLEUI_MSG_CLOSEBUSYDIALOG);

	// Register Clipboard Formats used by PasteSpecial dialog.
	cfObjectDescriptor = RegisterClipboardFormat(CF_OBJECTDESCRIPTOR);
	cfLinkSrcDescriptor= RegisterClipboardFormat(CF_LINKSRCDESCRIPTOR);
	cfEmbedSource      = RegisterClipboardFormat(CF_EMBEDSOURCE);
	cfEmbeddedObject   = RegisterClipboardFormat(CF_EMBEDDEDOBJECT);
	cfLinkSource       = RegisterClipboardFormat(CF_LINKSOURCE);
	cfOwnerLink        = RegisterClipboardFormat(CF_OWNERLINK);
	cfFileName         = RegisterClipboardFormat(CF_FILENAME);

	if (!FResultImageInitialize(hInstance, hPrevInst, SZCLASSRESULTIMAGE))
		{
		OleDbgOut1("OleUIInitialize: FResultImageInitialize failed. Terminating.\r\n");
		return 0;
		}

	if (!FIconBoxInitialize(hInstance, hPrevInst, SZCLASSICONBOX))
		{
		OleDbgOut1("OleUIInitialize: FIconBoxInitialize failed. Terminating.\r\n");
		return 0;
		}

	return TRUE;

ResourceLoadError:
	OleDbgOut1("OleUIInitialize: ERROR - Unable to find version verification resource.\r\n");
	return FALSE;

ResourceLockError:
	OleDbgOut1("OleUIInitialize: ERROR - Unable to lock version verification resource.\r\n");
	FreeResource(hg);
	return FALSE;

ResourceReadError:
	OleDbgOut1("OleUIInitialize: ERROR - Version verification values did not compare.\r\n");

	{char buf[255];
	wsprintf(buf, "resource read 0x%X, my value is 0x%X\n", (WORD)*lpdata, (WORD)OLEUI_VERSION_MAGIC);
	OutputDebugString(buf);
	}

	UnlockResource(hg);
	FreeResource(hg);
	return FALSE;
}


/*
 * OleUIUnInitialize
 *
 * NOTE: This function should only be called by your application IF it is using
 * the static-link version of this library.  If the DLL version is being used,
 * this function is automatically called from the DLL's LibMain.
 *
 * Purpose:
 *   Uninitializes OLE UI libraries.  Deletes any resources allocated by the
 *   library.
 *
 * Return Value:
 *   BOOL       TRUE if successful, FALSE if not.  Current implementation always
 *              returns TRUE.
 */


STDAPI_(BOOL) OleUIUnInitialize()
{
	IconBoxUninitialize();
	ResultImageUninitialize();

	return TRUE;
}


/*
 * OleUICanUnloadNow
 *
 * NOTE: if you statically link to this library and you implement
 *       DllCanUnloadNow, then you must call this routine in your
 *       implementation of DllCanUnloadNow to determine if you can be
 *       unloaded or not.
 *       this function should NOT be used if the OLE2UI library is used
 *       in DLL form.
 *
 * Purpose:
 *   Determines when it is safe to go away
 *   (ie. there are no existing object instances).
 *
 * Return Value:
 *   HRESULT    NOERROR it is safe to go away, S_FALSE this code must stay
 *              loaded.
 *
 * Comments:
 *
 *   If an INPROC server DLL links to the OLE2UI library as a static
 *   library, then the OleUILockLibrary function should NOT be used.
 *   instead the INPROC server DLL should call OleUICanUnloadNow API from
 *   within its own DllCanUnloadNow function. The idea here is, that if there
 *   are any existing instance of objects created by the OLE2UI library
 *   functions (eg. EnumFORMATETC objects created by OleStdEnumFmtEtc_Create)
 *   then, the INPROC server DLL must NOT let itself be unloaded.
 *
 *   An EXE based object using the OLE2UI libray need NOT use either the
 *   OleUILockLibrary or OleUICanUnloadNow functions. All objects created
 *   by the OLE2UI library will have LRPC proxies and stubs created to
 *   manage remoting method calls. the worst that can happen when the EXE
 *   exits is that any outstanding proxies for unreleased objects will get
 *   RPC_E_SERVERDIED errors; they will not GPFault.
 */

STDAPI OleUICanUnloadNow()
{
  if (g_dwObjectCount == 0) {
	return NOERROR;
  }

  return ResultFromScode(S_FALSE);
}


/*
 * OleUIAddVerbMenu
 *
 * Purpose:
 *  Add the Verb menu for the specified object to the given menu.  If the
 *  object has one verb, we directly add the verb to the given menu.  If
 *  the object has multiple verbs we create a cascading sub-menu.
 *
 * Parameters:
 *  lpObj           LPOLEOBJECT pointing to the selected object.  If this
 *                  is NULL, then we create a default disabled menu item.
 *
 *  lpszShortType   LPSTR with short type name (AuxName==2) corresponding
 *                  to the lpOleObj. if the string is NOT known, then NULL
 *                  may be passed. if NULL is passed, then
 *                  IOleObject::GetUserType will be called to retrieve it.
 *                  if the caller has the string handy, then it is faster
 *                  to pass it in.
 *
 *  hMenu           HMENU in which to make modifications.
 *
 *  uPos            Position of the menu item
 *
 *  uIDVerbMin      UINT ID value at which to start the verbs.
 *                      verb_0 = wIDMVerbMin + verb_0
 *                      verb_1 = wIDMVerbMin + verb_1
 *                      verb_2 = wIDMVerbMin + verb_2
 *                      etc.
 *  uIDVerbMax      UINT maximum ID value allowed for object verbs.
 *                     if uIDVerbMax==0 then any ID value is allowed
 *
 *  bAddConvert     BOOL specifying whether or not to add a "Convert" item
 *                  to the bottom of the menu (with a separator).
 *
 *  idConvert       UINT ID value to use for the Convert menu item, if
 *                  bAddConvert is TRUE.
 *
 *  lphMenu         HMENU FAR * of the cascading verb menu if it's created.
 *                  If there is only one verb, this will be filled with NULL.
 *
 *
 * Return Value:
 *  BOOL            TRUE if lpObj was valid and we added at least one verb
 *                  to the menu.  FALSE if lpObj was NULL and we created
 *                  a disabled default menu item
 */

STDAPI_(BOOL) OleUIAddVerbMenu(LPOLEOBJECT lpOleObj,
							 LPSTR lpszShortType,
							 HMENU hMenu,
							 UINT uPos,
							 UINT uIDVerbMin,
							 UINT uIDVerbMax,
							 BOOL bAddConvert,
							 UINT idConvert,
							 HMENU FAR *lphMenu)
{
	LPPERSISTSTORAGE    lpPS=NULL;
	LPENUMOLEVERB       lpEnumOleVerb = NULL;
	OLEVERB             oleverb;
	LPUNKNOWN           lpUnk;
	LPSTR               lpszShortTypeName = lpszShortType;
	LPSTR               lpszVerbName = NULL;
	HRESULT             hrErr;
	BOOL                fStatus;
	BOOL                fIsLink = FALSE;
	BOOL                fResult = TRUE;
	BOOL                fAddConvertItem = FALSE;
	int                 cVerbs = 0;
	UINT                uFlags = MF_BYPOSITION;
	static BOOL         fFirstTime = TRUE;
	static char         szBuffer[OLEUI_OBJECTMENUMAX];
	static char         szNoObjectCmd[OLEUI_OBJECTMENUMAX];
	static char         szObjectCmd1Verb[OLEUI_OBJECTMENUMAX];
	static char         szLinkCmd1Verb[OLEUI_OBJECTMENUMAX];
	static char         szObjectCmdNVerb[OLEUI_OBJECTMENUMAX];
	static char         szLinkCmdNVerb[OLEUI_OBJECTMENUMAX];
	static char         szUnknown[OLEUI_OBJECTMENUMAX];
	static char         szEdit[OLEUI_OBJECTMENUMAX];
	static char         szConvert[OLEUI_OBJECTMENUMAX];

	*lphMenu=NULL;

	// Set fAddConvertItem flag
	if (bAddConvert & (idConvert != 0))
	   fAddConvertItem = TRUE;

	// only need to load the strings the 1st time
	if (fFirstTime) {
		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDITNOOBJCMD,
				 (LPSTR)szNoObjectCmd, OLEUI_OBJECTMENUMAX))
			return FALSE;
		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDITLINKCMD_1VERB,
				 (LPSTR)szLinkCmd1Verb, OLEUI_OBJECTMENUMAX))
			return FALSE;
		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDITOBJECTCMD_1VERB,
				 (LPSTR)szObjectCmd1Verb, OLEUI_OBJECTMENUMAX))
			return FALSE;

		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDITLINKCMD_NVERB,
				 (LPSTR)szLinkCmdNVerb, OLEUI_OBJECTMENUMAX))
			return FALSE;
		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDITOBJECTCMD_NVERB,
				 (LPSTR)szObjectCmdNVerb, OLEUI_OBJECTMENUMAX))
			return FALSE;

		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIUNKNOWN,
				 (LPSTR)szUnknown, OLEUI_OBJECTMENUMAX))
			return FALSE;

		if (0 == LoadString(ghInst, (UINT)IDS_OLE2UIEDIT,
				 (LPSTR)szEdit, OLEUI_OBJECTMENUMAX))
			return FALSE;

		if ( (0 == LoadString(ghInst, (UINT)IDS_OLE2UICONVERT,
				   (LPSTR)szConvert, OLEUI_OBJECTMENUMAX)) && fAddConvertItem)
			return FALSE;

		fFirstTime = FALSE;
	}

	// Delete whatever menu may happen to be here already.
	DeleteMenu(hMenu, uPos, uFlags);

	if (!lpOleObj)
		goto AVMError;

	if (! lpszShortTypeName) {
		// get the Short form of the user type name for the menu
		OLEDBG_BEGIN2("IOleObject::GetUserType called\r\n")
		hrErr = lpOleObj->lpVtbl->GetUserType(
				lpOleObj,
				USERCLASSTYPE_SHORT,
				(LPSTR FAR*)&lpszShortTypeName
		);
		OLEDBG_END2

		if (NOERROR != hrErr) {
			OleDbgOutHResult("IOleObject::GetUserType returned", hrErr);
		}
	}

	// check if the object is a link (it is a link if it support IOleLink)
	hrErr = lpOleObj->lpVtbl->QueryInterface(
			lpOleObj,
			&IID_IOleLink,
			(LPVOID FAR*)&lpUnk
	);
	if (NOERROR == hrErr) {
		fIsLink = TRUE;
		OleStdRelease(lpUnk);
	}

	// Get the verb enumerator from the OLE object
	OLEDBG_BEGIN2("IOleObject::EnumVerbs called\r\n")
	hrErr = lpOleObj->lpVtbl->EnumVerbs(
			lpOleObj,
			(LPENUMOLEVERB FAR*)&lpEnumOleVerb
	);
	OLEDBG_END2

	if (NOERROR != hrErr) {
		OleDbgOutHResult("IOleObject::EnumVerbs returned", hrErr);
	}

	if (!(*lphMenu = CreatePopupMenu()))
		goto AVMError;

	// loop through all verbs
	while (lpEnumOleVerb != NULL) {         // forever
		hrErr = lpEnumOleVerb->lpVtbl->Next(
				lpEnumOleVerb,
				1,
				(LPOLEVERB)&oleverb,
				NULL
		);
		if (NOERROR != hrErr)
			break;              // DONE! no more verbs

		/* OLE2NOTE: negative verb numbers and verbs that do not
		**    indicate ONCONTAINERMENU should NOT be put on the verb menu
		*/
		if (oleverb.lVerb < 0 ||
				! (oleverb.grfAttribs & OLEVERBATTRIB_ONCONTAINERMENU)) {

			/* OLE2NOTE: we must still free the verb name string */
			if (oleverb.lpszVerbName)
				OleStdFreeString(oleverb.lpszVerbName, NULL);
			continue;
		}

		// we must free the previous verb name string
		if (lpszVerbName)
			OleStdFreeString(lpszVerbName, NULL);

		lpszVerbName = oleverb.lpszVerbName;

		if ( 0 == uIDVerbMax ||
			(uIDVerbMax >= uIDVerbMin+(UINT)oleverb.lVerb) ) {
			fStatus = InsertMenu(
					*lphMenu,
					(UINT)-1,
					MF_BYPOSITION | (UINT)oleverb.fuFlags,
					uIDVerbMin+(UINT)oleverb.lVerb,
					(LPSTR)lpszVerbName
			);
			if (! fStatus)
				goto AVMError;

			cVerbs++;
		}
	}

	// Add the separator and "Convert" menu item.
	if (fAddConvertItem) {

		if (0 == cVerbs) {
			LPSTR lpsz;
			LPSTR lpszSrc = (LPSTR)szConvert;

			// if object has no verbs, then use "Convert" as the obj's verb
			lpsz = lpszVerbName = OleStdMalloc(sizeof(szConvert));
			uIDVerbMin = idConvert;

			// remove "&" and "..." from "&Convert..." string
			if (lpsz) {
				while (*lpszSrc && *lpszSrc != '.') {
					if (*lpszSrc != '&') {
						*lpsz = *lpszSrc;
						if (IsDBCSLeadByte(*lpszSrc)) {
							// copy second byte of DBCS character
							lpsz++;
							*lpsz = *(lpszSrc+1);
						}
						lpsz++;
					}
					lpszSrc = AnsiNext(lpszSrc);
				}
				*lpsz = '\0';
			}
		}

		if (cVerbs > 0) {
			fStatus = InsertMenu(*lphMenu,
						(UINT)-1,
						MF_BYPOSITION | MF_SEPARATOR,
						(UINT)0,
						(LPCSTR)NULL);
			if (! fStatus)
				goto AVMError;
		}

		/* add convert menu */
		fStatus = InsertMenu(*lphMenu,
					(UINT)-1,
					MF_BYPOSITION,
					idConvert,
					(LPCSTR)szConvert);
		if (! fStatus)
			goto AVMError;

		cVerbs++;
	}


	/*
	 * Build the appropriate menu based on the number of verbs found
	 *
	 * NOTE:  Localized verb menus may require a different format.
	 *        to assist in localization of the single verb case, the
	 *        szLinkCmd1Verb and szObjectCmd1Verb format strings start
	 *        with either a '0' (note: NOT '\0'!) or a '1':
	 *           leading '0' -- verb type
	 *           leading '1' -- type verb
	 */

	if (cVerbs == 0) {

		// there are NO verbs (not even Convert...). set the menu to be
		// "<short type> &Object/Link" and gray it out.
		wsprintf(
			szBuffer,
			(fIsLink ? (LPSTR)szLinkCmdNVerb:(LPSTR)szObjectCmdNVerb),
			(lpszShortTypeName ? lpszShortTypeName : (LPSTR)"")
		);
		uFlags |= MF_GRAYED;

#if defined( OBSOLETE )
		//No verbs.  Create a default using Edit as the verb.
		LPSTR       lpsz = (fIsLink ? szLinkCmd1Verb : szObjectCmd1Verb);

		if (*lpsz == '0') {
			wsprintf(szBuffer, lpsz+1, (LPSTR)szEdit,
				(lpszShortTypeName ? lpszShortTypeName : (LPSTR)"")
			);
		}
		else {
			wsprintf(szBuffer, lpsz+1,
				(lpszShortTypeName ? lpszShortTypeName : (LPSTR)""),
				(LPSTR)szEdit
			);
		}
#endif

		fResult = FALSE;
		DestroyMenu(*lphMenu);
		*lphMenu = NULL;

	}
	else if (cVerbs == 1) {
		//One verb without Convert, one item.
		LPSTR       lpsz = (fIsLink ? szLinkCmd1Verb : szObjectCmd1Verb);
		LPSTR       lpsz1 = lpszVerbName;

		// remove "&" from the verb name string. the &Object or &Link
		// should be the menu accelerator.
		// if only "verb" is "Convert" then the "&" has already been stripped
		if (lpsz1 && !fAddConvertItem) {
			LPSTR lpszVerbName2 = OleStdMalloc(lstrlen(lpszVerbName)+1);
			LPSTR lpsz2 = lpszVerbName2;
			if (lpsz2) {
				while (*lpsz1) {
					if (*lpsz1 != '&') {
						*lpsz2 = *lpsz1;
						if (IsDBCSLeadByte(*lpsz1)) {
						// copy second byte of DBCS character
							lpsz2++;
							*lpsz2 = *(lpsz1+1);
						}
						lpsz2++;
					}
					lpsz1 = AnsiNext(lpsz1);
				}
				*lpsz2 = '\0';
				OleStdFreeString(lpszVerbName, NULL);
				lpszVerbName = lpszVerbName2;
			}
		}

		if (*lpsz == '0') {
			wsprintf(szBuffer, lpsz+1, lpszVerbName,
				(lpszShortTypeName ? lpszShortTypeName : (LPSTR)"")
			);
		}
		else {
			wsprintf(szBuffer, lpsz+1,
				(lpszShortTypeName ? lpszShortTypeName : (LPSTR)""),
				lpszVerbName
			);
		}

		// if only "verb" is "Convert..." then append the ellipses
		if (fAddConvertItem)
			lstrcat(szBuffer, "...");

		DestroyMenu(*lphMenu);
		*lphMenu=NULL;
	}
	else {

		//Multiple verbs or one verb with Convert, add the cascading menu
		wsprintf(
			szBuffer,
			(fIsLink ? (LPSTR)szLinkCmdNVerb:(LPSTR)szObjectCmdNVerb),
			(lpszShortTypeName ? lpszShortTypeName : (LPSTR)"")
		);
		uFlags |= MF_ENABLED | MF_POPUP;
		uIDVerbMin=(UINT)*lphMenu;
	}

	if (!InsertMenu(hMenu, uPos, uFlags, uIDVerbMin, (LPSTR)szBuffer))

AVMError:
		{
			InsertMenu(hMenu, uPos, MF_GRAYED | uFlags,
					uIDVerbMin, (LPSTR)szNoObjectCmd);
#if defined( OBSOLETE )
			HMENU hmenuDummy = CreatePopupMenu();

			InsertMenu(hMenu, uPos, MF_GRAYED | MF_POPUP | uFlags,
					(UINT)hmenuDummy, (LPSTR)szNoObjectCmd);
#endif
			fResult = FALSE;
		}

	if (lpszVerbName)
		OleStdFreeString(lpszVerbName, NULL);
	if (!lpszShortType && lpszShortTypeName)
		OleStdFreeString(lpszShortTypeName, NULL);
	if (lpEnumOleVerb)
		lpEnumOleVerb->lpVtbl->Release(lpEnumOleVerb);
	return fResult;
}


/* PromptUserDlgProc
 * -----------------
 *
 *  Purpose:
 *      Dialog procedure used by OleUIPromptUser(). Returns when a button is
 *      clicked in the dialog box and the button id is return.
 *
 *  Parameters:
 *      hDlg
 *      iMsg
 *      wParam
 *      lParam
 *
 *  Returns:
 *
 */
BOOL CALLBACK EXPORT PromptUserDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) {
		case WM_INITDIALOG:
		{
			LPSTR lpszTitle;
			char szBuf[256];
			char szFormat[256];
			va_list argptr;

			argptr = (va_list)lParam;
			if (!lParam) {
				EndDialog(hDlg, -1);
				return FALSE;
			}

			lpszTitle = va_arg(argptr, LPSTR);
			SetWindowText(hDlg, lpszTitle);

			GetDlgItemText(hDlg, ID_PU_TEXT,(LPSTR)szFormat,sizeof(szFormat));
			wvsprintf((LPSTR)szBuf, (LPSTR)szFormat, argptr);
			SetDlgItemText(hDlg, ID_PU_TEXT, (LPSTR)szBuf);
			return TRUE;
		}
		case WM_COMMAND:
			EndDialog(hDlg, wParam);
			return TRUE;

		default:
			return FALSE;
	}
}


/* OleUIPromptUser
 * ---------------
 *
 *  Purpose:
 *      Popup a dialog box with the specified template and returned the
 *      response (button id) from the user.
 *
 *  Parameters:
 *      nTemplate       resource number of the dialog
 *      hwndParent      parent of the dialog box
 *      ...             title of the dialog box followed by argument list
 *                      for the format string in the static control
 *                      (ID_PU_TEXT) of the dialog box.
 *                      The caller has to make sure that the correct number
 *                      and type of argument are passed in.
 *
 *  Returns:
 *      button id selected by the user (template dependent)
 *
 *  Comments:
 *      the following message dialog boxes are supported:
 *
 *      IDD_LINKSOURCEUNAVAILABLE -- Link source is unavailable
 *          VARARG Parameters:
 *              None.
 *          Used for the following error codes:
 *              OLE_E_CANT_BINDTOSOURCE
 *              STG_E_PATHNOTFOUND
 *              (sc >= MK_E_FIRST) && (sc <= MK_E_LAST) -- any Moniker error
 *              any unknown error if object is a link
 *
 *      IDD_SERVERNOTFOUND -- server registered but NOT found
 *          VARARG Parameters:
 *              LPSTR lpszUserType -- user type name of object
 *          Used for the following error codes:
 *              CO_E_APPNOTFOUND
 *              CO_E_APPDIDNTREG
 *              any unknown error if object is an embedded object
 *
 *      IDD_SERVERNOTREG -- server NOT registered
 *          VARARG Parameters:
 *              LPSTR lpszUserType -- user type name of object
 *          Used for the following error codes:
 *              REGDB_E_CLASSNOTREG
 *              OLE_E_STATIC -- static object with no server registered
 *
 *      IDD_LINKTYPECHANGED -- class of link source changed since last binding
 *          VARARG Parameters:
 *              LPSTR lpszUserType -- user type name of ole link source
 *          Used for the following error codes:
 *              OLE_E_CLASSDIFF
 *
 *      IDD_LINKTYPECHANGED -- class of link source changed since last binding
 *          VARARG Parameters:
 *              LPSTR lpszUserType -- user type name of ole link source
 *          Used for the following error codes:
 *              OLE_E_CLASSDIFF
 *
 *      IDD_OUTOFMEMORY -- out of memory
 *          VARARG Parameters:
 *              None.
 *          Used for the following error codes:
 *              E_OUTOFMEMORY
 *
 */
int EXPORT FAR CDECL OleUIPromptUser(WORD nTemplate, HWND hwndParent, ...)
{
	int         nRet;

	va_list     argptr;

	va_start(argptr, hwndParent);
	nRet = DialogBoxParam(ghInst, MAKEINTRESOURCE(nTemplate), hwndParent,
			PromptUserDlgProc, (LPARAM)(LPVOID)argptr);

	va_end(argptr);
	return nRet;
}



/* UpdateLinksDlgProc
 * ------------------
 *
 *  Purpose:
 *      Dialog procedure used by OleUIUpdateLinks(). It will enumerate all
 *      all links in the container and updates all automatic links.
 *      Returns when the Stop Button is clicked in the dialog box or when all
 *      links are updated
 *
 *  Parameters:
 *      hDlg
 *      iMsg
 *      wParam
 *      lParam          pointer to the UPDATELINKS structure
 *
 *  Returns:
 *
 */
BOOL CALLBACK EXPORT UpdateLinksDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	LPUPDATELINKS FAR*      lplpUL = NULL;
	HANDLE                  gh;
	static BOOL             fAbort = FALSE;

	//Process the temination message
	if (iMsg==uMsgEndDialog)
		{
		gh = RemoveProp(hDlg, STRUCTUREPROP);
		if (NULL!=gh) {
			GlobalUnlock(gh);
			GlobalFree(gh);
		}
		EndDialog(hDlg, wParam);
		return TRUE;
		}

	switch (iMsg) {
		case WM_INITDIALOG:
		{
			gh=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,sizeof(LPUPDATELINKS));
			SetProp(hDlg, STRUCTUREPROP, gh);

			if (NULL==gh)
			{
				PostMessage(hDlg, uMsgEndDialog, OLEUI_ERR_GLOBALMEMALLOC,0L);
				return FALSE;
			}

			fAbort = FALSE;
			lplpUL = (LPUPDATELINKS FAR*)GlobalLock(gh);

			if (lplpUL) {
				*lplpUL = (LPUPDATELINKS)lParam;
				SetWindowText(hDlg, (*lplpUL)->lpszTitle);
				SetTimer(hDlg, 1, UPDATELINKS_STARTDELAY, NULL);
				return TRUE;
			} else {
				PostMessage(hDlg, uMsgEndDialog, OLEUI_ERR_GLOBALMEMALLOC,0L);
				return FALSE;
			}
		}

		case WM_TIMER:
			KillTimer(hDlg, 1);
			gh = GetProp(hDlg, STRUCTUREPROP);

			if (NULL!=gh) {
				// gh was locked previously, lock and unlock to get lplpUL
				lplpUL = GlobalLock(gh);
				GlobalUnlock(gh);
			}
			if (! fAbort && lplpUL)
				PostMessage(hDlg, WM_U_UPDATELINK, 0, (LPARAM)(*lplpUL));
			else
				PostMessage(hDlg,uMsgEndDialog,OLEUI_CANCEL,0L);

			return 0;

		case WM_COMMAND:    // Stop button
			fAbort = TRUE;
			SendMessage(hDlg, uMsgEndDialog, OLEUI_OK, 0L);
			return TRUE;

		case WM_U_UPDATELINK:
		{
			HRESULT         hErr;
			int             nPercent;
			RECT            rc;
			char            szPercent[5];       // 0% to 100%
			HBRUSH          hbr;
			HDC             hDC;
			HWND            hwndMeter;
			MSG             msg;
			DWORD           dwUpdateOpt;
			LPUPDATELINKS   lpUL = (LPUPDATELINKS)lParam;

			lpUL->dwLink=lpUL->lpOleUILinkCntr->lpVtbl->GetNextLink(
					lpUL->lpOleUILinkCntr,
					lpUL->dwLink
			);

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (! IsDialogMessage(hDlg, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			if (fAbort)
				return FALSE;

			if (!lpUL->dwLink) {        // all links processed
				SendMessage(hDlg, uMsgEndDialog, OLEUI_OK, 0L);
				return TRUE;
			}

			hErr = lpUL->lpOleUILinkCntr->lpVtbl->GetLinkUpdateOptions(
					lpUL->lpOleUILinkCntr,
					lpUL->dwLink,
					(LPDWORD)&dwUpdateOpt
			);

			if ((hErr == NOERROR) && (dwUpdateOpt == OLEUPDATE_ALWAYS)) {

				hErr = lpUL->lpOleUILinkCntr->lpVtbl->UpdateLink(
						lpUL->lpOleUILinkCntr,
						lpUL->dwLink,
						FALSE,      // fMessage
						FALSE       // ignored
				);
				lpUL->fError |= (hErr != NOERROR);
				lpUL->cUpdated++;

				nPercent = lpUL->cUpdated * 100 / lpUL->cLinks;
				if (nPercent <= 100) {  // do NOT advance % beyond 100%
					// update percentage
					wsprintf((LPSTR)szPercent, "%d%%", nPercent);
					SetDlgItemText(hDlg, ID_PU_PERCENT, (LPSTR)szPercent);

					// update indicator
					hwndMeter = GetDlgItem(hDlg, ID_PU_METER);
					GetClientRect(hwndMeter, (LPRECT)&rc);
					InflateRect((LPRECT)&rc, -1, -1);
					rc.right = (rc.right - rc.left) * nPercent / 100 + rc.left;
					hbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
					if (hbr) {
						hDC = GetDC(hwndMeter);
						if (hDC) {
							FillRect(hDC, (LPRECT)&rc, hbr);
							ReleaseDC(hwndMeter, hDC);
						}
						DeleteObject(hbr);
					}
				}
			}

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (! IsDialogMessage(hDlg, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			PostMessage(hDlg, WM_U_UPDATELINK, 0, lParam);

			return TRUE;
		}

		default:
			return FALSE;
	}
}


/* OleUIUpdateLink
 * ---------------
 *
 *  Purpose:
 *      Update all links in the Link Container and popup a dialog box which
 *      shows the progress of the updating.
 *      The process is stopped when the user press Stop button or when all
 *      links are processed.
 *
 *  Parameters:
 *      lpOleUILinkCntr         pointer to Link Container
 *      hwndParent              parent window of the dialog
 *      lpszTitle               title of the dialog box
 *      cLinks                  total number of links
 *
 *  Returns:
 *      TRUE                    all links updated successfully
 *      FALSE                   otherwise
 */
STDAPI_(BOOL) OleUIUpdateLinks(LPOLEUILINKCONTAINER lpOleUILinkCntr, HWND hwndParent, LPSTR lpszTitle, int cLinks)
{
	LPUPDATELINKS lpUL = (LPUPDATELINKS)OleStdMalloc(sizeof(UPDATELINKS));
	BOOL          fError;

	OleDbgAssert(lpOleUILinkCntr && hwndParent && lpszTitle && (cLinks>0));
	OleDbgAssert(lpUL);

	lpUL->lpOleUILinkCntr = lpOleUILinkCntr;
	lpUL->cLinks           = cLinks;
	lpUL->cUpdated         = 0;
	lpUL->dwLink           = 0;
	lpUL->fError           = FALSE;
	lpUL->lpszTitle    = lpszTitle;

	DialogBoxParam(ghInst, MAKEINTRESOURCE(IDD_UPDATELINKS),
			hwndParent, UpdateLinksDlgProc, (LPARAM)lpUL);

	fError = lpUL->fError;
	OleStdFree((LPVOID)lpUL);

	return !fError;
}
