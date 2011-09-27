/*
	stockdlg.h

	The StockDlg class declaration.  The StockDlg class implements a
	dialog box allowing users to modify various parameters

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#ifndef __STOCKDLG_H
#define __STOCKDLG_H

#include <windows.h>    //maybe we ought to put this elsewhere?
#include "dataview.h"


class StockDlg {
	private:
		HWND hDlg;
		HANDLE hInstance;
		/*static BOOL FAR PASCAL __export DlgProc (HWND hDlg, UINT message,
			UINT wParam, LONG lParam );  */
	public:
		StockDlg( HWND hWnd, CDataView *pdv );
};

#endif
