/*************************************************************************
**
**    OLE 2 DFVIEW Sample Code
**
**    foldlist.h
**
**    (c) Copyright Microsoft Corp. 1990 - 1992 All Rights Reserved
**
*************************************************************************/

/*
**
** The hBitmaps in a FL_ITEMINFOSTRUCT is comprised of 4 bitmaps layed out
** horizontally:
**    #1 == Selected/Closed Folder
**    #2 == Selected/Open Folder
**    #1 == UnSelected/Closed Folder
**    #2 == UnSelected/Open Folder
**
** The size of each of these 4 bitmaps is stored in ptSizeFolders of
** the FL_LISTINFOSTRUCT.
**
** Limit:  There's an internal limitation of 16384 items in our
**         folder list.  This limit is due to a table that maps
**         item IDs to FAR pointers to items.  Each far pointer
**         takes up 4 bytes.  We put this table in a single
**         segment.
**
*/

#define FLNoMessage        0xFFFF    // No msg if in nMsg* has this.
#define FL_MAX_STR_LEN     80
#define FLClassName        "FolderList"

#define WM_FLADDITEM          WM_USER+1500
#define WM_FLADDITEMCHILD     WM_USER+1501
#define WM_FLGETSIZEFOLDERS   WM_USER+1502
#define WM_FLGETCOLORHIGH     WM_USER+1503
#define WM_FLGETCOLORNORM     WM_USER+1504
#define WM_FLEXPANDALL        WM_USER+1515
#define WM_FLCOLLAPSEALL      WM_USER+1516

#define FL_MAXITEMS        16383U


typedef struct tagFL_ITEMINFOSTRUCT
{
   BOOL     bIsFolder;        // Is this entry a folder?
   BOOL     bIsOpen;          // Open (iff bIsFolder)?
   BOOL     bIsVisible;       // Is entry visible (if parent is visibile).
   UINT     nMsgClick;        // On a click, send this message to parent.
   UINT     nMsgDblClick;     // On a double click, send this message to parent.
   BOOL     bFLOwnsBitmap;    // True if FolerList needs to delete bitmap.
   HANDLE   hBitmaps;         // Bitmap containing all the folder/item bitmaps.
   char     szText[FL_MAX_STR_LEN];

/*** Set by our control but useable by the caller. ***/
   UINT     nID;              // Unique ID for this item.
   UINT     nIndent;

/*** Privately set by our control ***/
   struct tagFL_INFOSTRUCT far *lpSibling;
   struct tagFL_INFOSTRUCT far *lpChild;
   struct tagFL_INFOSTRUCT far *lpParent;
} FL_ITEMINFOSTRUCT, far * LPFL_ITEMINFOSTRUCT;

typedef LPFL_ITEMINFOSTRUCT far *LPLPFL_ITEMINFOSTRUCT;


typedef struct tagFL_LISTINFOSTRUCT
{
   HWND                hListBox;
   POINT               ptSizeFolders;
   POINT               ptSizeItem;
   UINT                cxIndent;
   COLORREF            crBkHigh;
   COLORREF            crBkNorm;
   COLORREF            crTextHigh;
   COLORREF            crTextNorm;
   HBRUSH              hbrBk;
   UINT                nNextID;
   LPFL_ITEMINFOSTRUCT lpHead;
   LPFL_ITEMINFOSTRUCT far *lpMapIDToItem;
} FL_LISTINFOSTRUCT, far *LPFL_LISTINFOSTRUCT;

BOOL FLInit (HINSTANCE, int, int, BOOL);
BOOL FLFree (HINSTANCE);
