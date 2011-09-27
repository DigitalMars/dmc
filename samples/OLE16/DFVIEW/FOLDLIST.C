/*************************************************************************
**
**    OLE 2 DFVIEW Sample Code
**
**    foldlist.c
**
**    This file contains the routines which implement a generic expandable
**    owner-draw listbox similar to the Windows File Manager.
**
**    NOTE: This code is intended to supplement the DFVIEW sample
**    to provide a usable owner-draw listbox implementation, and is not
**    intended to demonstrate the techniques of writing an owner-draw
**    listbox.
**
**    (c) Copyright Microsoft Corp. 1990 - 1992 All Rights Reserved
**
*************************************************************************/

#include "precomp.h"
#include <string.h>
#include <assert.h>
#include <malloc.h>

#include "foldlist.h"

// Makes us build w/out warnings.
#pragma warning ( disable: 4049 )

/********************** Locally Used "Globals" *************************/
static unsigned gnFLControls = 0;      // # of currently created FL controls
static HFONT hLBFont;                  // Font to use for listbox
int iBmpX, iBmpY;                      // X and Y requested size of bitmap
#define IDC_LISTBOX     1000           // ID for creating listbox

/********************** Locally used Functions *************************/
long FAR PASCAL _export FLWndProc (HWND, UINT, WPARAM, LPARAM);
static long FLOnCreate        (HWND);
static long FLOnDestroy       (LPFL_LISTINFOSTRUCT);
static long FLOnDrawItem      (const LPFL_LISTINFOSTRUCT, const LPDRAWITEMSTRUCT);
static long FLOnAddItem       (LPFL_LISTINFOSTRUCT, LPFL_ITEMINFOSTRUCT);
static long FLOnAddItemChild  (LPFL_LISTINFOSTRUCT, UINT, LPFL_ITEMINFOSTRUCT);
static void FLFreeTree        (LPFL_ITEMINFOSTRUCT far *);
static long FLOnLBDblClick    (LPFL_LISTINFOSTRUCT);
static int  FLMakeItemVisible (HWND, LPFL_ITEMINFOSTRUCT, int, BOOL);
static void FLExpandAll       (HWND, LPFL_ITEMINFOSTRUCT, BOOL);
static void FLOpenFolder      (HWND, int, LPFL_ITEMINFOSTRUCT, BOOL);


/************************************************************************
**  Function: FLInit
**
**  Purpose : Initialization routine for folder listbox.  Registers
**            necessary classes, etc.
**
**  Parms   : hInst -- This applications instance handle (used for
**                      registering our class.
**
**            xBmp  -- Requested size of bitmaps for owner-draw listbox
**                     can be 0 for default size
**            yBmp  -- same
**
**  Returns : TRUE on success.
**            FALSE on failure.
**
************************************************************************/

BOOL FLInit (HINSTANCE hInst, int xBmp, int yBmp, BOOL bRegisterClass)
{
   WNDCLASS wc;
   iBmpX = xBmp;
   iBmpY = yBmp;

   if (bRegisterClass)
	 {
	   wc.style          = CS_HREDRAW |
						   CS_VREDRAW;
	   wc.lpfnWndProc    = FLWndProc;
	   wc.cbClsExtra     = 0;
	   wc.cbWndExtra     = sizeof (LPFL_LISTINFOSTRUCT);
	   wc.hInstance      = hInst;
	   wc.hIcon          = NULL;
	   wc.hCursor        = LoadCursor (NULL, IDC_ARROW);
	   wc.hbrBackground  = COLOR_WINDOW + 1;
	   wc.lpszMenuName   = NULL;
	   wc.lpszClassName  = FLClassName;

	   return RegisterClass (&wc);
	}
	return TRUE;
}



/************************************************************************
**  Function: FLFree
**
**  Purpose : De-initialization routine.  Call when done using
**            Folder-Listbox controls.
**
**  Parms   : hInst -- This applications instance handle (used for
**                      un-registering our class.
**
**  Returns : TRUE on success.
**            FALSE on failure.
**
************************************************************************/

BOOL FLFree (HINSTANCE hInst)
{
   assert (gnFLControls == 0);

   return UnregisterClass (FLClassName, hInst);
}





/************************************************************************
**  Function: FLWndProc
**
**  Purpose : Main window procedure for our folder listbox control.
**            Handles all messages we need to.
**
**  Parms   : hWnd    == Handle to _this_ window.
**            message == Message to process.
**            wParam  == WORD parameter -- depends on message
**            lParam  == LONG parameter -- depends on message
**
**  Returns : Depends on message.
**
************************************************************************/

long FAR PASCAL _export FLWndProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   LPFL_LISTINFOSTRUCT lpListInfo;

   lpListInfo = (LPFL_LISTINFOSTRUCT) GetWindowLong (hWnd, 0);


   switch (msg)
	  {
	  case WM_CREATE:
		 return FLOnCreate (hWnd);


	  case WM_DRAWITEM:
		 return FLOnDrawItem (lpListInfo, (const LPDRAWITEMSTRUCT) lParam);


	  case WM_DESTROY:
		 return FLOnDestroy (lpListInfo);


	  case WM_FLADDITEM:
		 return FLOnAddItem (lpListInfo, (LPFL_ITEMINFOSTRUCT) lParam);


	  case WM_FLADDITEMCHILD:
		 return FLOnAddItemChild (lpListInfo, wParam, (LPFL_ITEMINFOSTRUCT) lParam);


	  case WM_FLGETSIZEFOLDERS:
		 return MAKELONG (lpListInfo->ptSizeFolders.x,
						  lpListInfo->ptSizeFolders.y);

	  case WM_FLGETCOLORHIGH:
		 return (LONG) lpListInfo->crBkHigh;

	  case WM_FLGETCOLORNORM:
		 return (LONG) lpListInfo->crBkNorm;

	  case WM_FLEXPANDALL:
	  case WM_FLCOLLAPSEALL:
		 {
		 HCURSOR hCursor;

		 hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT));

		 SendMessage (lpListInfo->hListBox, WM_SETREDRAW, FALSE, 0L);
		 FLExpandAll (lpListInfo->hListBox, lpListInfo->lpHead, msg == WM_FLEXPANDALL);
		 SendMessage (lpListInfo->hListBox, LB_SETCURSEL,  0xFFFF, 0L);
		 SendMessage (lpListInfo->hListBox, LB_SETTOPINDEX, 0, 0L);
		 SendMessage (lpListInfo->hListBox, WM_SETREDRAW,   TRUE, 0L);

		 SetCursor (hCursor);
		 return 0L;
		 }

	  case WM_COMMAND:
		 if ((wParam == IDC_LISTBOX) &&
			 (LOWORD (lParam) == lpListInfo->hListBox))
			switch (HIWORD (lParam))
			   {
			   case LBN_DBLCLK:
				  return FLOnLBDblClick (lpListInfo);

			   default:
				  /* do nothing */ ;
			   }
		 break;

	  /*
	  ** On a measure item, return the size of one item in our listbox.
	  **
	  ** NOTE:  This routine can be called when not all fields have
	  **        been set in lpListInfo.  Check out FLOnCreate() for
	  **        more details.
	  */
	  case WM_MEASUREITEM:
		 {
		 LPMEASUREITEMSTRUCT lpmis;

		 lpmis = (LPMEASUREITEMSTRUCT) lParam;

		 lpmis->itemWidth  = lpListInfo->ptSizeItem.x;
		 lpmis->itemHeight = lpListInfo->ptSizeItem.y;
		 break;
		 }


	  /*
	  **  On a size, resize our listbox, too.  Note that the width of
	  **  an item changes here!
	  */
	  case WM_SIZE:
		 lpListInfo->ptSizeItem.x = LOWORD (lParam);

		 SetWindowPos (lpListInfo->hListBox, NULL, 0, 0, LOWORD (lParam),
					   HIWORD (lParam), SWP_NOMOVE | SWP_NOZORDER);
		 break;


	  case WM_CTLCOLOR:
		 if (lpListInfo && lpListInfo->hbrBk)
			return lpListInfo->hbrBk;
		 break;


	  /*
	  **  Pass WM_SETREDRAW, LB_RESETCONTENT messages on to the listbox.
	  */
	  case LB_RESETCONTENT:
	  case WM_SETREDRAW:
		 return SendMessage (lpListInfo->hListBox, msg, wParam, lParam);


	  default:
		 return DefWindowProc (hWnd, msg, wParam, lParam);
	  }

   return 0L;
}





/************************************************************************
**  Function: FLOnCreate
**
**  Purpose : Called by FLWndProc to handle WM_CREATE messages for
**            a folder list.  Does all internal initialization of
**            our LISTINFOSTRUCT, and creates our owner draw listbox.
**
**  Parms   : hWnd    == Handle to _this_ window.
**
**  Returns :  0 on success
**            -1 if window shouldn't be created.
**
************************************************************************/

long FLOnCreate (HWND hWnd)
{
   LPFL_LISTINFOSTRUCT lpListInfo;
   RECT                rcWnd;
   HDC                 hDC;
   TEXTMETRIC          tm;
   LOGFONT lf;
   HFONT hfold;

   lpListInfo = (LPFL_LISTINFOSTRUCT) GlobalAllocPtr (GHND, sizeof (FL_LISTINFOSTRUCT));

   if (!lpListInfo)
	  return -1;


   /*
   ** Initialize our map of ID's to FL_LISTINFOSTRUCT's.
   */

   lpListInfo->lpMapIDToItem = (LPFL_ITEMINFOSTRUCT far *) GlobalAllocPtr (GHND,
								 FL_MAXITEMS * sizeof (LPFL_ITEMINFOSTRUCT));
   if (!lpListInfo->lpMapIDToItem)
	  {
	  GlobalFreePtr (lpListInfo);
	  return -1;
	  }
   _fmemset (lpListInfo->lpMapIDToItem, 0,
			 FL_MAXITEMS * sizeof (LPFL_ITEMINFOSTRUCT));

   /*
   ** Create font for list box
   */


   hDC = GetDC (hWnd);
   memset(&lf, 0, sizeof(LOGFONT));
   // Do us an 8 point Courier font.
   lf.lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
   strcpy(lf.lfFaceName, "Courier");
   hLBFont = CreateFontIndirect(&lf);

   GetClientRect  (hWnd, &rcWnd);
   hfold = SelectObject(hDC, hLBFont);
   GetTextMetrics (hDC, &tm);

   /*
   ** If user passed in requested X and Y sizes for little bitmaps
   ** in our FLInit() routine, then use them here.  If not, then use
   ** default
   */

   if (iBmpX > 0 && iBmpY > 0)
	 {
	 lpListInfo->ptSizeFolders.x = iBmpX;
	 lpListInfo->ptSizeFolders.y = iBmpY;
	 }
   else
	 {
	 lpListInfo->ptSizeFolders.x = tm.tmMaxCharWidth;
	 lpListInfo->ptSizeFolders.y = tm.tmHeight + tm.tmExternalLeading;
	 }

   lpListInfo->ptSizeItem.x    = rcWnd.right;
   lpListInfo->ptSizeItem.y    = lpListInfo->ptSizeFolders.y;
   lpListInfo->cxIndent        = lpListInfo->ptSizeFolders.x;
   lpListInfo->crBkHigh        = GetSysColor (COLOR_HIGHLIGHT);
   lpListInfo->crBkNorm        = GetSysColor (COLOR_MENU);
   lpListInfo->crTextHigh      = GetSysColor (COLOR_HIGHLIGHTTEXT);
   lpListInfo->crTextNorm      = GetSysColor (COLOR_MENUTEXT);
   lpListInfo->hbrBk           = CreateSolidBrush (lpListInfo->crBkNorm);
   lpListInfo->nNextID         = 0;
   lpListInfo->lpHead          = NULL;

   gnFLControls++;

   SelectObject(hDC, hfold);

   ReleaseDC     (hWnd, hDC);
   SetWindowLong (hWnd, 0, (LONG) lpListInfo);


   /*
   **  Our listbox needs to be created *after* we've done our
   **  SetWindowLong, as it generates a WM_MEASUREITEM message,
   **  which requires our lpListInfo pointer to be set already...
   */

   lpListInfo->hListBox = CreateWindow ("listbox", "",
							 LBS_OWNERDRAWFIXED | WS_CHILD | WS_VISIBLE |
							 WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
							 0, 0, rcWnd.right, rcWnd.bottom,
							 hWnd, IDC_LISTBOX,
							 GetWindowWord (hWnd, GWW_HINSTANCE),
							 NULL);

   SendMessage(lpListInfo->hListBox, WM_SETFONT, hLBFont, MAKELONG(TRUE,0));

   if (!lpListInfo->hListBox)
	  {
	  GlobalFreePtr (lpListInfo);
	  GlobalFreePtr (lpListInfo->lpMapIDToItem);
	  SetWindowLong (hWnd, 0, 0L);
	  return -1;
	  }

   return 0;
}



/************************************************************************
**  Function: FLOnDestroy
**
**  Purpose : Called by FLWndProc to handle WM_DESTROY.  Frees up anything
**            we've allocated (i.e. cleans up after ourselves).
**
**  Parms   : lpListInfo -- A FL_INFOSTRUCT that describes data on our
**                          control.
**
**  Returns : 0L
**
************************************************************************/

long FLOnDestroy (LPFL_LISTINFOSTRUCT lpListInfo)
{
   GlobalFreePtr (lpListInfo->lpMapIDToItem);

   if (lpListInfo->hbrBk)
	  DeleteObject (lpListInfo->hbrBk);

   if (hLBFont) DeleteObject(hLBFont);

   FLFreeTree (&lpListInfo->lpHead);
   gnFLControls--;

   return 0L;
}




/************************************************************************
**  Function: FLFreeTree
**
**  Purpose : Given a pointer to a pointer to a node, traverses our
**            tree of nodes, deleting each node (and setting the node
**            ptr to NULL).  Also deletes items the node contains
**            (such as the hBitmaps bitmap, if necessary).
**
**            !! IMPORTANT !! Routine is recursive in the "child"
**            direction.  I think this is OK, since I think most
**            "trees" will be quite "deep", but not very "wide".
**
**  Parms   : lplpNode -- ptr to a ptr to a node.
**
**  Returns : None.
**
************************************************************************/

void FLFreeTree (LPFL_ITEMINFOSTRUCT far *lplpNode)
{
   while (*lplpNode)
	  {
	  LPFL_ITEMINFOSTRUCT far *lplpNext;

	  if ((*lplpNode)->lpChild)
		 FLFreeTree (&(*lplpNode)->lpChild);

	  lplpNext = &(*lplpNode)->lpSibling;

	  if (((*lplpNode)->bFLOwnsBitmap) && ((*lplpNode)->hBitmaps))
		 DeleteObject ((*lplpNode)->hBitmaps);

	  _ffree (*lplpNode);

	  lplpNode = lplpNext;
	  }
}



/************************************************************************
**  Function: FLOnDrawItem
**
**  Purpose : Called by FLWndProc to handle WM_DRAWITEM messages for
**            a folder list.  Does all the painting of items in our
**            listbox.
**
**  Parms   : lpDIS -- A DRAWITEMSTRUCT describing what we need to draw.
**            lpLIS -- A FL_INFOSTRUCT that describes data on our control.
**
**  Returns : TRUE
**
************************************************************************/

long FLOnDrawItem (const LPFL_LISTINFOSTRUCT lpLIS,
				   const    LPDRAWITEMSTRUCT lpDIS)
{
   LPFL_ITEMINFOSTRUCT lpItemInfo;
   int                 x, y;
   COLORREF            crOldText, crOldBack;
   RECT                rcItem;

   /*
   ** If this is just a focus change, then we don't need to do
   ** anything.
   */
   if (!(lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	  return TRUE;

   /*
   ** Retrieve our FL_ITEMINFOSTRUCT
   */
   lpItemInfo = (LPFL_ITEMINFOSTRUCT) lpDIS->itemData;
   assert (lpItemInfo);

   /*
   ** Do the drawing.
   */
   x = lpDIS->rcItem.left + lpItemInfo->nIndent * lpLIS->cxIndent;
   y = lpDIS->rcItem.top;

   if (lpItemInfo->hBitmaps)
	  {
	  HDC     hMemDC;
	  HBITMAP hOldBitmap;
	  int     nBitmap;

	  hMemDC     = CreateCompatibleDC (lpDIS->hDC);
	  hOldBitmap = SelectObject (hMemDC, lpItemInfo->hBitmaps);
	  nBitmap    = ((lpDIS->itemState & ODS_SELECTED) ? 0 : 2) +
				   lpItemInfo->bIsOpen;

	  BitBlt (lpDIS->hDC, x, y, lpLIS->ptSizeFolders.x, lpLIS->ptSizeFolders.y,
			  hMemDC, nBitmap * lpLIS->ptSizeFolders.x, 0, SRCCOPY);
	  SelectObject (hMemDC, hOldBitmap);
	  DeleteDC (hMemDC);

	  x += lpLIS->ptSizeFolders.x;
	  }

   rcItem       = lpDIS->rcItem;
   rcItem.left  = x;
   rcItem.right = x + LOWORD(GetTextExtent (lpDIS->hDC, lpItemInfo->szText,
				   lstrlen (lpItemInfo->szText)));
   crOldText    = SetTextColor (lpDIS->hDC, (lpDIS->itemState & ODS_SELECTED) ?
								lpLIS->crTextHigh : lpLIS->crTextNorm);
   crOldBack    = SetBkColor   (lpDIS->hDC, (lpDIS->itemState & ODS_SELECTED) ?
								lpLIS->crBkHigh : lpLIS->crBkNorm);

   ExtTextOut (lpDIS->hDC, x, y, ETO_OPAQUE | ETO_CLIPPED, &rcItem,
			   lpItemInfo->szText, lstrlen (lpItemInfo->szText), NULL);


   /*
   ** Clean up.
   */
   SetTextColor (lpDIS->hDC, crOldText);
   SetBkColor   (lpDIS->hDC, crOldBack);
}





/************************************************************************
**  Function: FLOnAddItem
**
**  Purpose : Called by FLWndProc to handle adding new items to our folder
**            listbox.  Does all the necessary housekeeping, like
**            traversing our tree structure to find where to put the guy,
**            setting private fields in the ItemInfo, etc.
**
**  Parms   : lpListInfo -- A FL_INFOSTRUCT that describes data on our
**                          control.
**            lpItemInfo -- Info on the new item.
**
**  Returns : Unique identifier for this node on success.
**            -1 on failure.
**
************************************************************************/

long FLOnAddItem (LPFL_LISTINFOSTRUCT lpListInfo,
				  LPFL_ITEMINFOSTRUCT lpItemInfo)
{
   LPFL_ITEMINFOSTRUCT far *lplpNode;
   LPFL_ITEMINFOSTRUCT lpNewItemInfo;
   UINT                nID;


   /*
   ** Make sure we have room for more.
   */
   if (lpListInfo->nNextID >= FL_MAXITEMS)
	  {
	  UINT i;

	  /*
	  ** Look for a free item in our map (in case we deleted one).
	  **/
	  for (i=0;  i < FL_MAXITEMS;  i++)
		 if (lpListInfo->lpMapIDToItem[i] == NULL)
			break;

	  if (i == FL_MAXITEMS)
		 return -1;

	  nID = i;
	  }
   else
	  nID = lpListInfo->nNextID++;     // Note, we could fail later, leaving a blank ID -- we'll live with it!


   /*
   ** Go find the last item in our list.
   */
   lplpNode = &lpListInfo->lpHead;
   while (*lplpNode != NULL)
	  lplpNode = &(*lplpNode)->lpSibling;

   /*
   ** Allocate storage for a new node.  Note that C7's _fmalloc()
   ** function will do sub-segment heap allocation for us (thus
   ** not eating up selectors unnecessarily).  Most excellent!
   */
   lpNewItemInfo = (LPFL_ITEMINFOSTRUCT) _fmalloc (sizeof (FL_ITEMINFOSTRUCT));
   if (!lpNewItemInfo)
	  return -1;

   *lplpNode = lpNewItemInfo;


   /*
   ** Setup our new node appropriately.  Note, the caller might be interested
   ** in this info, so we also change the structure it passed in.
   */
   *lpNewItemInfo = *lpItemInfo;

   lpItemInfo->nIndent   = lpNewItemInfo->nIndent   = 0;
   lpItemInfo->nID       = lpNewItemInfo->nID       = nID;
   lpItemInfo->lpSibling = lpNewItemInfo->lpSibling = NULL;
   lpItemInfo->lpChild   = lpNewItemInfo->lpChild   = NULL;
   lpItemInfo->lpParent  = lpNewItemInfo->lpParent  = NULL;


   /*
   ** Tell the owner-draw listbox we have a new item, and
   ** get outta here.  And add the item to our map!
   */
   if (SendMessage (lpListInfo->hListBox, LB_ADDSTRING, 0, (LPARAM) lpNewItemInfo) < 0)
	  {
	  assert (FALSE);
	  _ffree (lpNewItemInfo);
	  return -1;
	  }

   lpListInfo->lpMapIDToItem[lpItemInfo->nID] = lpNewItemInfo;
   return lpNewItemInfo->nID;
}



/************************************************************************
**  Function: FLOnAddItemChild
**
**  Purpose : Called by FLWndProc to handle adding new child items to our
**            folder listbox.  Does all the necessary housekeeping, like
**            traversing our tree structure to find where to put the guy,
**            setting private fields in the ItemInfo, etc.
**
**  Parms   : lpListInfo -- A FL_INFOSTRUCT that describes data on our
**                          control.
**            nParent    -- ID of this item's parent.
**            lpItemInfo -- Info on the new item.
**
**  Returns : Unique identifier for this node on success.
**            -1 on failure.
**
************************************************************************/

long FLOnAddItemChild (LPFL_LISTINFOSTRUCT lpListInfo,
									  UINT nParent,
					   LPFL_ITEMINFOSTRUCT lpItemInfo)
{
   LPFL_ITEMINFOSTRUCT      lpParent;
   LPFL_ITEMINFOSTRUCT      lpNewItemInfo;
   LPFL_ITEMINFOSTRUCT far *lplpNode;
   UINT                      nID;


   /*
   ** Make sure we have room for more.
   */
   if (lpListInfo->nNextID >= FL_MAXITEMS)
	  {
	  UINT i;

	  /*
	  ** Look for a free item in our map (in case we deleted one).
	  **/
	  for (i=0;  i < FL_MAXITEMS;  i++)
		 if (lpListInfo->lpMapIDToItem[i] == NULL)
			break;

	  if (i == FL_MAXITEMS)
		 return -1;

	  nID = i;
	  }
   else
	  nID = lpListInfo->nNextID++;     // Note, we could fail later, leaving a blank ID -- we'll live with it!


   /*
   ** Find where we're going to add the child.
   */
   lpParent = lpListInfo->lpMapIDToItem[nParent];

   if (!lpParent)
	  {
	  assert (FALSE);
	  return -1;
	  }

   lplpNode = &lpParent->lpChild;
   while (*lplpNode != NULL)
	  lplpNode = &(*lplpNode)->lpSibling;

   /*
   ** Allocate storage for a new node.  Note that C7's _fmalloc()
   ** function will do sub-segment heap allocation for us (thus
   ** not eating up selectors unnecessarily).  Most excellent!
   */
   lpNewItemInfo = (LPFL_ITEMINFOSTRUCT) _fmalloc (sizeof (FL_ITEMINFOSTRUCT));
   if (!lpNewItemInfo)
	  {
	  assert (FALSE);
	  return -1;
	  }

   *lplpNode = lpNewItemInfo;


   /*
   ** Setup our new node appropriately.  Note, the caller might be interested
   ** in this info, so we also change the structure it passed in.
   */
   *lpNewItemInfo = *lpItemInfo;

   lpItemInfo->nIndent    = lpNewItemInfo->nIndent    = lpParent->nIndent + 1;
   lpItemInfo->nID        = lpNewItemInfo->nID        = nID;
   lpItemInfo->lpSibling  = lpNewItemInfo->lpSibling  = NULL;
   lpItemInfo->lpChild    = lpNewItemInfo->lpChild    = NULL;
   lpItemInfo->lpParent   = lpNewItemInfo->lpParent   = lpParent;
   lpItemInfo->bIsVisible = lpNewItemInfo->bIsVisible = lpParent->bIsVisible &&
						  (lpParent->bIsFolder ? lpParent->bIsOpen : TRUE);


   /*
   ** Tell the owner-draw listbox we have a new item, and
   ** get outta here.  And add the item to our map!
   */
   if (lpItemInfo->bIsVisible)
	  {
	  int nPos;

	  nPos = (int) SendMessage (lpListInfo->hListBox, LB_FINDSTRINGEXACT,
								(WPARAM) -1, (LPARAM) lpParent);
	  assert (nPos != LB_ERR);

	  if (nPos == LB_ERR || (SendMessage (lpListInfo->hListBox,
		  LB_INSERTSTRING, nPos+1, (LPARAM) lpNewItemInfo) < 0))
		 {
		 assert (FALSE);
		 _ffree (lpNewItemInfo);
		 return -1;
		 }
	  }

   lpListInfo->lpMapIDToItem[lpItemInfo->nID] = lpNewItemInfo;

   return lpNewItemInfo->nID;
}




/************************************************************************
**  Function: FLOnLBDblClick
**
**  Purpose : Called by FLWndProc when an item is double clicked.  Handles
**            opening/closing folders, and informing our parent of double
**            clicks, if necessary.
**
**  Parms   : lpListInfo -- A FL_INFOSTRUCT that describes data on our
**                          control.
**
**  Returns : TRUE  on success.
**            FALSE on failure.
**
************************************************************************/

long FLOnLBDblClick (LPFL_LISTINFOSTRUCT lpListInfo)
{
   int                 nItem;
   LPFL_ITEMINFOSTRUCT lpItemInfo;

   nItem = (int) SendMessage (lpListInfo->hListBox, LB_GETCURSEL, 0, 0L);
   if (nItem < 0L)
	  {
	  assert (FALSE);
	  return FALSE;
	  }

   if (SendMessage (lpListInfo->hListBox, LB_GETTEXT, nItem,
					(LPARAM) (LPFL_ITEMINFOSTRUCT) &lpItemInfo) == LB_ERR)
	  {
	  assert (FALSE);
	  return FALSE;
	  }


   FLOpenFolder (lpListInfo->hListBox, nItem, lpItemInfo, !lpItemInfo->bIsOpen);

   return TRUE;
}




/************************************************************************
**  Function: FLOpenFolder
**
**  Purpose : Given a folder, open or close it.
**
**  Parms   : hListBox   -- Handle to our listbox.
**            nItem      -- Where to add/delete in list box.
**            lpItemInfo -- Ptr to info on this item.
**            bOpen      -- whether we're opening or closing it.
**
**  Returns : None.
**
************************************************************************/

void FLOpenFolder (HWND hListBox, int nItem, LPFL_ITEMINFOSTRUCT lpItemInfo,
				   BOOL bOpen)
{
   /*
   **  Only do this if this item is a folder, and it's state is changing.
   */
   if (lpItemInfo->bIsFolder && bOpen != lpItemInfo->bIsOpen)
	  {
	  LPFL_ITEMINFOSTRUCT lpChild;
	  int                 nCurItem;
	  RECT                rcItem;


	  /*
	  **  We can open items that aren't even in our listbox, so make
	  **  sure we remember that...
	  */
	  if (nItem != LB_ERR)
		 nCurItem = nItem + 1;
	  else
		 nCurItem = LB_ERR;

	  /*
	  **  Open or close this folder.
	  */
	  lpItemInfo->bIsOpen = bOpen;


	  /*
	  **  Go thru this folder's children, making them visible/invisible.
	  */
	  lpChild = lpItemInfo->lpChild;
	  while (lpChild)
		 {
		 nCurItem += FLMakeItemVisible (hListBox, lpChild, nCurItem, bOpen);
		 lpChild   = lpChild->lpSibling;
		 }


	  /*
	  **  Re-draw this item in its new state.
	  */
	  if (nItem != LB_ERR)
		 {
		 SendMessage    (hListBox, LB_GETITEMRECT, nItem, (LPARAM)(LPRECT) &rcItem);
		 InvalidateRect (hListBox, &rcItem, TRUE);
		 UpdateWindow   (hListBox);
		 }
	  }
}




/************************************************************************
**  Function: FLMakeItemVisible
**
**  Purpose : Called by FLOnDblClick to make children of a folder visible
**            or invisible.  Handles adding the appropiate items to the
**            listbox, or deleting them.
**
**  Parms   : hListBox -- Handle to our listbox.
**            lpItem   -- Ptr to the item in question.
**            nItem    -- Where to add/delete in list box.
**            bVisible -- whether we're making it visible or invisible.
**
**  Returns : # of items added to the listbox due to this change.
**
************************************************************************/

int FLMakeItemVisible (HWND hListBox, LPFL_ITEMINFOSTRUCT lpItem,
					   int nItem, BOOL bVisible)
{
   int                 nItems;
   BOOL                bChanged = (bVisible != lpItem->bIsVisible);
   LPFL_ITEMINFOSTRUCT lpChild;

   /*
   **  Change our item's state.  Note that we can be changing item
   **  states that aren't even in our listbox.  So, we do special
   **  hanlding if nItem == LB_ERR.
   */

   if (bVisible)
	  {
	  if (bChanged)
		 {
		 if (nItem != LB_ERR)
			SendMessage (hListBox, LB_INSERTSTRING, nItem, (LPARAM) lpItem);
		 lpItem->bIsVisible = TRUE;
		 }

	  nItems = (nItem == LB_ERR) ? 0 : 1;
	  }
   else
	  {
	  if (bChanged)
		 {
		 if (nItem != LB_ERR)
			SendMessage (hListBox, LB_DELETESTRING, nItem, 0L);
		 lpItem->bIsVisible = FALSE;
		 }

	  nItems = 0;
	  }


   /*
	*  Make any children of this item visible/invisible.  But, if this guy's
	*  a folder, only make its children visible if it's open and the
	*  folder is visible.
	*/

   lpChild = lpItem->lpChild;
   while (lpChild)
	  {
	  nItems += FLMakeItemVisible (hListBox, lpChild, nItem+nItems,
				  bVisible ? (lpItem->bIsFolder ? lpItem->bIsOpen : TRUE)
					 : FALSE);
	  lpChild = lpChild->lpSibling;
	  }

   return nItems;
}




/************************************************************************
**  Function: FLExpandAll
**
**  Purpose : Open all the folders in our listbox starting at a given
**            item.
**
**            !! IMPORTANT !!  Yet another recursive routine brought
**            to you by yours truly -- remember to have a big stack
**            unless you want a big headache.
**
**  Parms   : hListBox   -- Handle to our ownerdraw listbox.
**            lpItemInfo -- First guy to start our expansion from.
**            bOpen      -- Whether we're opening or closing each folder.
**
**  Returns : 0L
**
************************************************************************/

void FLExpandAll (HWND hListBox, LPFL_ITEMINFOSTRUCT lpItemInfo, BOOL bOpen)
{
   /*
   **  Go through all the top-level items in the list, opening each,
   **  and calling our routine recursively on each child.
   */
   while (lpItemInfo)
	  {
	  int nItem;

	  /*
	  **  The following *can* return LB_ERR, if the item isn't
	  **  currently in our listbox.  FLOpenFolder() does the
	  **  right this for us!
	  */
	  nItem = (int) SendMessage (hListBox, LB_FINDSTRINGEXACT,
								 (WPARAM) -1, (LPARAM) lpItemInfo);

	  FLOpenFolder (hListBox, nItem, lpItemInfo, bOpen);

	  FLExpandAll (hListBox, lpItemInfo->lpChild, bOpen);

	  lpItemInfo = lpItemInfo->lpSibling;
	  }
}
