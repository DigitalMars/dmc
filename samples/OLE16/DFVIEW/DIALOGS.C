/*************************************************************************
**
**     OLE 2 DFVIEW Sample Code
**
**     dialogs.c
**
**     This file contains the dialog procedures and supporting functions
**     for DFVIEW.
**
**    (c) Copyright Microsoft Corp. 1993 All Rights Reserved
**
*************************************************************************/

#include "precomp.h"
#include <dlgs.h>
#include "dfview.h"
#include "resource.h"

extern char szVersion[]; // Defined in DFVIEW.C, used for About box.

extern HBITMAP           ghbmpFolders;    // Folder's bitmap
extern HBITMAP           ghbmpBullets;    // 3-D Bullet bitmap
extern HBITMAP           ghbmpWindows;    // Window guy bitmap
extern HBITMAP           ghbmpBlanks;
extern HBITMAP           ghbmpDocuments;

/***************************************************************************
**
**    About(HWND, unsigned, WORD, LONG)
**
****************************************************************************/

BOOL _far _pascal __export About(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, IDC_VERSION, (LPSTR)szVersion);
			return (TRUE);

		case WM_COMMAND:
			if (wParam == IDOK || wParam == IDCANCEL)
				{
				EndDialog(hDlg, TRUE);
				return (TRUE);
				}
			break;
		}
	return (FALSE);
	}

/***************************************************************************
**
** PlotBitmap()
**
** Plops a bitmap centered in the specified control in the dialog on the
** specified DC.
**
** There's a bunch of calculations going on to find out where in the bitmap
** we should display -- the reason is that our bitmaps actually have 4 copies
** of the bitmap in it -- the normal one, the open one, the highlighted normal
** one, and the highlighted open one.
**
****************************************************************************/

PlopBitmap(HWND hDlg, int control, HDC hDC, HBITMAP hBitmap)
{
  BITMAP bm;
  HDC hdcmem;
  HBITMAP hbmOld;
  RECT r1;
  POINT pt, pt1;

  GetObject(hBitmap, sizeof(BITMAP), &bm);

  hdcmem= CreateCompatibleDC(hDC);
  hbmOld = SelectObject(hdcmem, hBitmap);

  GetWindowRect(GetDlgItem(hDlg, control), &r1);
  pt.x = r1.left; pt.y = r1.top;
  ScreenToClient(hDlg, &pt);
  pt1.x= r1.right; pt1.y = r1.bottom;
  ScreenToClient(hDlg, &pt1);

  BitBlt(hDC, pt.x + (pt1.x - pt.x - bm.bmWidth/4) / 2,
				   pt.y + (pt1.y - pt.y - bm.bmHeight) /2,
				   bm.bmWidth /4, bm.bmHeight,
				   hdcmem, bm.bmWidth*3/4, 0, SRCCOPY);

  SelectObject(hdcmem, hbmOld);
  DeleteDC(hdcmem);
  return 1;
}

/****************************************************************************
**
**    HelpProc -- for "HELP" dialog
**
****************************************************************************/

BOOL _far _pascal __export HelpProc(HWND hDlg, unsigned message, WORD wParam, LONG lParam)
	{
	switch (message)
		{
		case WM_INITDIALOG:
			ShowWindow(GetDlgItem(hDlg, IDC_BM1), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BM2), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BM3), SW_HIDE);
			ShowWindow(GetDlgItem(hDlg, IDC_BM4), SW_HIDE);
			return (TRUE);

  case WM_PAINT:
	{
	PAINTSTRUCT ps;

	BeginPaint(hDlg, &ps);

	PlopBitmap(hDlg, IDC_BM1, ps.hdc, ghbmpWindows);
	PlopBitmap(hDlg, IDC_BM2, ps.hdc, ghbmpFolders);
	PlopBitmap(hDlg, IDC_BM3, ps.hdc, ghbmpDocuments);
	PlopBitmap(hDlg, IDC_BM4, ps.hdc, ghbmpBullets);

	EndPaint(hDlg, &ps);
	}

	return TRUE;

		case WM_COMMAND:
			if (wParam == IDOK || wParam == IDCANCEL)
				{
				EndDialog(hDlg, TRUE);
				return (TRUE);
				}
			break;
		}
	return (FALSE);
	}
