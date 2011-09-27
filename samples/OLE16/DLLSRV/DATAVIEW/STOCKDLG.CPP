/*
	stockdlg.cpp

	The file implements the StockDlg class, allows the user to modify
	object parameters via a dialog box.

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#include <windows.h>
#include <stdio.h>
#include "stockdlg.h"
#include "stockutl.h"
#include "resource.h"

#define STOCKDLGPROP "StockDialogProp"


#define XXERROR(x) MessageBox(hDlg, x, DV_APPNAME, MB_ICONEXCLAMATION | MB_OK );

BOOL FAR PASCAL __export StockDlgProc (HWND hDlg, UINT message,
	UINT wParam, LPARAM lParam )
{
	CDataView *pdv;
	int Jitter, UpdInt;
	long nLimit;
	static char szBuf[128];
	HGLOBAL gh;

	//retrieve a pointer to the dataview object

	if( message == WM_INITDIALOG )
		SetProp(hDlg, STOCKDLGPROP, (gh = (HGLOBAL)(lParam)));
	else
		gh = GetProp(hDlg, STOCKDLGPROP);

	if( gh ){
		pdv = *(CDataView **)GlobalLock(gh);
		GlobalUnlock(gh); //lock #1 is held by the stock dialog constructor
	}

	switch (message) {
		case WM_INITDIALOG:
			SetDlgItemText(hDlg, ID_SERVICE, pdv->m_stockInfo.GetServiceName());
			SetDlgItemText( hDlg, ID_STOCK, pdv->m_stockInfo.GetStockName());
			SetDlgItemText( hDlg, ID_LIMIT, Stock2Str(pdv->m_stockInfo.GetPriceLimit()));
			CheckDlgButton( hDlg, ID_BEEP, pdv->m_stockInfo.GetBeep() );
			if (pdv->m_stockInfo.GetMode()){
				CheckDlgButton( hDlg, ID_MANUAL, 1);
				EnableWindow(GetDlgItem(hDlg, ID_TIME), FALSE);
			}else
				CheckDlgButton( hDlg, ID_AUTO, 1);

			if( !pdv->m_stockInfo.GetRealTimeParam( (LPINT)&Jitter, (LPINT)&UpdInt) ){
				XXERROR("Unable to get realtime parameters");
				Jitter = 10;
				UpdInt = 5;
				return TRUE;
			}
			wsprintf((LPSTR)szBuf, (LPSTR)"%d", Jitter);
			SetDlgItemText( hDlg, ID_JITTER, (LPSTR)szBuf );
			wsprintf((LPSTR)szBuf, (LPSTR)"%d", UpdInt);
			SetDlgItemText( hDlg, ID_TIME, (LPSTR)szBuf );
			return TRUE ;
		case WM_COMMAND:
			switch (wParam) {
				BOOL fHaveJitter, fHaveUpdInt;
				case ID_MANUAL:
					CheckRadioButton(hDlg, ID_MANUAL, ID_AUTO, ID_MANUAL);
					EnableWindow(GetDlgItem(hDlg, ID_TIME), FALSE);
					return TRUE;
				case ID_AUTO:
					CheckRadioButton(hDlg, ID_MANUAL, ID_AUTO, ID_AUTO);
					EnableWindow(GetDlgItem(hDlg, ID_TIME), TRUE);
					return TRUE;
				case ID_UPDATE:
					pdv->UpdateData();
					return TRUE;
				case ID_HELP:
					XXERROR("No help implemented!  :(");
					return TRUE;
				case IDOK:

				BOOL fSetServiceName;

				//get price ceiling
				GetDlgItemText( hDlg, ID_LIMIT, szBuf, 127);
				if( (nLimit = Str2Stock((LPSTR)szBuf)) == -1 ){
					XXERROR("Price ceiling must in the form dollars [n/[248]]");
					return TRUE;
				}else //get the beep flag too
					pdv->m_stockInfo.SetPriceLimit(nLimit, IsDlgButtonChecked(hDlg, ID_BEEP));

				//get jitter
				Jitter = GetDlgItemInt( hDlg, ID_JITTER, (BOOL FAR *)&fHaveJitter, TRUE);
				if( fHaveJitter && Jitter < 0 || Jitter > 100 ){
					XXERROR("Jitter must be a value between 0 and 100");
					return TRUE;
				}

				//check for manual/automatic mode

				if( IsDlgButtonChecked(hDlg, ID_MANUAL) ) {
					UpdInt = 0;
					pdv->m_stockInfo.SetManualMode();
				}
				else {
					UpdInt = GetDlgItemInt( hDlg, ID_TIME, (BOOL FAR *)&fHaveUpdInt, TRUE);
					if( fHaveUpdInt && UpdInt <= 0 ){
						XXERROR("Update interval must be a postive number");
						return TRUE;
					}
					pdv->m_stockInfo.SetAutoMode();
				}

				//get name
				GetDlgItemText(hDlg, ID_SERVICE, szBuf, 127);
				fSetServiceName = pdv->m_stockInfo.SetServiceName(szBuf);
				GetDlgItemText( hDlg, ID_STOCK, szBuf, 127);
				if(!pdv->m_stockInfo.SetStockName(
						(LPSTR)szBuf,fSetServiceName)){
					//  Binds to the object, if possible
					XXERROR("Unable to set stock Name");
					EndDialog (hDlg, 0) ;
					return TRUE;
				}

				//set parameters for the realtime data server
				//  don't bother if either value is 0.
				if( fHaveJitter && fHaveUpdInt &&
					!pdv->m_stockInfo.SetRealTimeParam(Jitter, UpdInt) ){
					XXERROR("Unable to set realtime parameters");
				}
				EndDialog (hDlg, 0) ;
				return TRUE;

				case IDCANCEL:
					EndDialog (hDlg, 0) ;
					return TRUE ;
			}
	}
	return FALSE ;
}

extern HINSTANCE vDV_DLLhinst;

StockDlg::StockDlg( HWND hWnd,CDataView *pdv )
{
	HGLOBAL gh = GlobalAlloc(GMEM_MOVEABLE, sizeof(CDataView *) );

	if( gh == NULL ) {
		MessageBox(hWnd, "Not enough Memory", "Simulated Stock Watcher",
			MB_ICONEXCLAMATION | MB_OK );
		return;
	} else
		*(CDataView **)GlobalLock(gh) = pdv;

	if( DialogBoxParam(vDV_DLLhinst, "StockDialogBox",
			hWnd, MakeProcInstance((FARPROC)StockDlgProc, vDV_DLLhinst), (LPARAM)gh) == -1)    //no MakeProcInstance
		MessageBox(hWnd, "Cannot Create Options Dialog Box", "Simulated Stock Watcher",
			MB_ICONEXCLAMATION | MB_OK );

	GlobalUnlock(gh);
	GlobalFree(gh);
}
