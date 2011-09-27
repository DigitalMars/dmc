/*
 * BCDEMO.C
 *
 * Main entry code for application.
 *
 * Copyright (c)1992-1993 Microsoft Corporation, All Right Reserved,
 * as applied to redistribution of this source code in source form
 * License is granted to use of compiled code in shipped binaries.
 */

#define STRICT
#include <windows.h>
#include "..\bttncur.h"
#include "bcdemo.h"


//Array of the possible button styles.
UINT        rguState[6]={COMMANDBUTTON_UP
						 , COMMANDBUTTON_MOUSEDOWN
						 , COMMANDBUTTON_DISABLED
						 , ATTRIBUTEBUTTON_DOWN
						 , ATTRIBUTEBUTTON_DOWNDISABLED
						 , ATTRIBUTEBUTTON_INDETERMINATE
						};


//Strings describing the possible states for labelling button rows.
char *      rgpsz[6]={"Up", "Mouse Down", "Disabled", "Down"
					  , "Down Disabled", "Indeterminate"
					 };


//More strings we'll use as labels.
char szStandard[]="Standard Buttons";
char szApp[]="App-defined Buttons";
char szMsg[]="Down, Down Disabled, and  Indeterminate states are not valid  on standard command buttons.";





/*
 * WinMain
 *
 * Purpose:
 *  Main entry point of application.
 *
 * Parameters:
 *  Standard
 *
 * Return Value:
 *  Value to return to Windows--termination code.
 */

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hInstPrev
				   , LPSTR pszCmdLine, int nCmdShow)
	{
	MSG         msg;
	BOOL        fSuccess=TRUE;
	PAPPVARS    pAV;

	//Attempt to allocate and initialize the application
	pAV=AppPAllocate(&fSuccess, hInst, hInstPrev, pszCmdLine, nCmdShow);

	if (NULL==pAV || !fSuccess)
		return (int)AppPFree(pAV);

	while (GetMessage(&msg, NULL, 0,0 ))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	AppPFree(pAV);
	return msg.wParam;
	}





/*
 * BCDemoWndProc
 *
 * Purpose:
 *  Window class procedure.  Standard callback.
 *
 * Parameters:
 *  Standard
 *
 * Return Value:
 *  Standard
 */

LONG FAR PASCAL BCDemoWndProc(HWND hWnd, UINT iMsg
							  , WPARAM wParam, LPARAM lParam)
	{
	PAPPVARS    pAV;
	WORD        wID;
	WORD        wCode;
	HWND        hWndMsg;
	PAINTSTRUCT ps;
	RECT        rc;
	HBITMAP     hBmp;
	HPEN        hPen, hPenT;
	HCURSOR     hCur;
	int         i, j;
	int         x, y;


	hWndMsg=(HWND)(UINT)lParam;
	wID=LOWORD(wParam);
	#ifdef WIN32
		wCode=HIWORD(wParam);
	#else
		wCode=HIWORD(lParam);
	#endif


	//This will be valid for all messages except WM_NCCREATE
	pAV=(PAPPVARS)GetWindowWord(hWnd, 0);


	switch (iMsg)
		{
		case WM_NCCREATE:
			//CreateWindow passed pAV to us.
			pAV=(PAPPVARS)((LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);

			//Store with the window--UINT converts to 32-bits.
			SetWindowWord(hWnd, 0, (UINT)pAV);
			return (DefWindowProc(hWnd, iMsg, wParam, lParam));

		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		case WM_PAINT:
			//Draw all the standard and app-defined buttons for show.
			BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rc);

			//Draw labels on the rows.  The pen is for drawing lines.
			SetTextColor(ps.hdc, GetSysColor(COLOR_WINDOWTEXT));
			SetBkColor(ps.hdc, GetSysColor(COLOR_WINDOW));
			SetBkMode(ps.hdc, OPAQUE);

			TextOut(ps.hdc, 8, 0, szStandard, lstrlen(szStandard));

			x=(TOOLIMAGE_MAX+1)*(5+pAV->tdd.cxButton);
			TextOut(ps.hdc, x, 0, szApp, lstrlen(szApp));


			//Pen is for drawing lines.
			hPen=CreatePen(PS_SOLID, 1, GetSysColor(COLOR_WINDOWTEXT));
			hPenT=SelectObject (ps.hdc, hPen);

			//Draw a horizontal separator line between text and buttons.
			MoveToEx(ps.hdc, 0, pAV->tdd.cyButton-4, NULL);
			LineTo(ps.hdc, rc.right-rc.left, pAV->tdd.cyButton-4);

			//Draw a vertical separator line between standard and app buttons.
			x-=2;
			y=pAV->tdd.cyButton+((5+pAV->tdd.cyButton)*6);

			MoveToEx(ps.hdc, x, 0, NULL);
			LineTo(ps.hdc, x, y);

			SelectObject(ps.hdc, hPenT);
			DeleteObject(hPen);


			/*
			 * Final label--show a message stating that the three attribute-
			 * only states are not available for standard command buttons.  We
			 * use the x & y we calculated for lines to position the text.
			 */
			SetRect(&rc, 0, y-((5+pAV->tdd.cyButton)*3), x, y);
			DrawText(ps.hdc, szMsg, lstrlen(szMsg), &rc, DT_CENTER | DT_WORDBREAK);


			//Get the application-defined images.
			hBmp=LoadBitmap(pAV->hInst, MAKEINTRESOURCE(pAV->idrBmp));

			//Now start drawing buttons.
			for (i=0; i<6; i++)
				{
				y=pAV->tdd.cyButton+((5+pAV->tdd.cyButton)*i);

				for (j=TOOLIMAGE_MIN; j<=TOOLIMAGE_MAX; j++)
					{
					/*
					 * Draw a standard button (NULL in hBmp).  If we fail, like
					 * we're attempting to draw an unsupported state on a
					 * command button, nothing appears.
					 */
					UIToolButtonDrawTDD(ps.hdc
							 , j*(5+pAV->tdd.cxButton), y
							 , pAV->tdd.cxButton, pAV->tdd.cyButton
							 , NULL, 0, 0, j, rguState[i], &pAV->tdd);
					}


				//Now draw two application-defined bitmaps in the same row.
				for (j=0; j<=1; j++)
					{
					/*
					 * Draw an application-defined button using indices 0 and
					 * 1 (a computer and a phone).  Adding TOOLIMAGE_MAX+1
					 * to j places these buttons at the ends of the existing
					 * rows of standard buttons.  These are 4 pixels larger
					 * in each direction over the standard size.
					 */
					UIToolButtonDrawTDD(ps.hdc
							 , (j+TOOLIMAGE_MAX+1)*(5+pAV->tdd.cxButton), y
							 , pAV->tdd.cxButton+4, pAV->tdd.cyButton+4, hBmp
							 , pAV->tdd.cxImage, pAV->tdd.cyImage, j
							 , rguState[i], &pAV->tdd);
					}

				/*
				 * Label the row with the state it shows.
				 *
				 * TOOLIMAGE_MAX+3 is the number of buttons.  +5 on width provides
				 * some spacing.  +4 on position spaces the text from the
				 * buttons a little.
				 */

				x=8+(TOOLIMAGE_MAX+3)*(5+pAV->tdd.cxButton);
				TextOut(ps.hdc, x, y, rgpsz[i], lstrlen(rgpsz[i]));
				}

			EndPaint(hWnd, &ps);
			DeleteObject(hBmp);
			break;


		case WM_COMMAND:
			//Restore the original cursor.
			if (IDM_NORMALCURSOR==wID)
				{
				hCur=LoadCursor(NULL, IDC_ARROW);
				SetClassWord(hWnd, GCW_HCURSOR, (UINT)hCur);
				}

			//Set a cursor from BTTNCUR.DLL.
			if (wID >= IDC_NEWUICURSORMIN && wID <= IDC_NEWUICURSORMAX)
				{
				/*
				 * Retrieve a new cursor and set it as the class cursor
				 * of our main window.
				 */
				hCur=UICursorLoad((UINT)wID);

				if (NULL!=hCur)
					SetClassWord(hWnd, GCW_HCURSOR, (UINT)hCur);
				}
			break;

		default:
			return (DefWindowProc(hWnd, iMsg, wParam, lParam));
		}

	return 0L;
	}
