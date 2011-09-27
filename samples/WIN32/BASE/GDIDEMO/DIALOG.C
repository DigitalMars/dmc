
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*---------------------------------------------------------------------------*\
| DIALOG MODULE
|   This module contains the dialogbox routines for this application.
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"

/*---------------------------------------------------------------------------*\
| DISPLAY DIALOG BOX
|   This is a routine to display a generic modal-dialog box.
|
\*---------------------------------------------------------------------------*/
int FAR DisplayDialogBox(HWND hWnd, LPSTR lpszTemplate, WNDPROC lpfFunction, LONG lExtra)
{
    HANDLE  hInstance;
    WNDPROC lpfDlg;
    int     nRet;


    nRet = -1;
    if(hInstance = GETINSTANCE(hWnd))
    {
        if(lpfDlg = MakeProcInstance(lpfFunction,hInstance))
        {
            nRet = DialogBoxParam(hInstance,lpszTemplate,hWnd,(DLGPROC)lpfDlg,lExtra);
            FreeProcInstance(lpfDlg);
        }
    }
    return(nRet);
}


/*---------------------------------------------------------------------------*\
| ABOUT DIALOG PROCEDURE
|   This is the main dialog box routine for the HELPABOUT template.
|
\*---------------------------------------------------------------------------*/
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LONG lParam)
{
    lParam = lParam;

    switch(wMsg)
    {
        /*
        ** Set the focus to the OK button.
        */
        case WM_INITDIALOG:
            SetFocus(GetDlgItem(hDlg,IDOK));
            break;


        /*
        ** Look for an ESC or RETURN event.
        */
        case WM_COMMAND:
            switch(wParam)
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg,TRUE);
                    break;

                default:
                    return(FALSE);
            }
            break;


        /*
        ** Wash the background of the aboutbox to give it a nice blue-scaling
        ** effect.  Invalidate the OK button to force it to the top.  This
        ** seems to be necessary since the OK button gets overwritten during
        ** the washing.
        */
        case WM_PAINT:
            PaintWindow(hDlg,COLOR_SCALE_BLUE);
            InvalidateRect(GetDlgItem(hDlg,IDOK),NULL,TRUE);


        /*
        ** Default handler.
        */
        default:
            return(FALSE);
    }
    return(TRUE);
}


/*---------------------------------------------------------------------------*\
| PAINT WND BACKGROUND
|   This routine is used to wash the background of a window.
|
\*---------------------------------------------------------------------------*/
VOID PaintWindow(HWND hWnd, int nColor)
{
    HDC         hDC;
    int         nMapMode,idx,nSize,nReserved,nLoop;
    RECT        rect;
    HBRUSH      hBrush;
    PAINTSTRUCT ps;
    HPALETTE    hPal;


    if(hDC = BeginPaint(hWnd,&ps))
    {
        GetClientRect(hWnd,&rect);
        nMapMode = SetMapMode(hDC,MM_ANISOTROPIC);


        if(GetDeviceCaps(hDC,RASTERCAPS) & RC_PALETTE)
        {
            nReserved = GetDeviceCaps(hDC,NUMRESERVED);
            nSize     = GetDeviceCaps(hDC,SIZEPALETTE) - nReserved;

            if(hPal = CreateColorScalePalette(hDC,nColor))
            {
                hPal = SelectPalette(hDC,hPal,FALSE);
                RealizePalette(hDC);


#ifdef WIN16
                SetWindowExtEx(hDC,nSize,nSize);
                SetViewportExtEx(hDC,rect.right,-rect.bottom);
                SetViewportOrgEx(hDC,0,rect.bottom);
#else

                SetWindowExtEx(hDC,nSize,nSize,NULL);
                SetViewportExtEx(hDC,rect.right,-rect.bottom,NULL);
                SetViewportOrgEx(hDC,0,rect.bottom,NULL);

#endif

                nLoop = nSize >> 1;
                for(idx=0; idx < nLoop; idx++)
                {
                    hBrush = CreateSolidBrush(PALETTEINDEX(idx+nLoop));
                    SetRect(&rect,idx,idx,nSize-idx,nSize-idx);
                    FillRect(hDC,&rect,hBrush);
                    DeleteObject(hBrush);
                }
                DeleteObject(SelectPalette(hDC,hPal,FALSE));
                RealizePalette(hDC);
            }
        }
        else
        {

#ifdef WIN16

            SetWindowExtEx(hDC,512,512);
            SetViewportExtEx(hDC,rect.right,-rect.bottom);
            SetViewportOrgEx(hDC,0,rect.bottom);

#else

            SetWindowExtEx(hDC,512,512,NULL);
            SetViewportExtEx(hDC,rect.right,-rect.bottom,NULL);
            SetViewportOrgEx(hDC,0,rect.bottom,NULL);

#endif

            for(idx=0; idx < 256; idx++)
            {
                hBrush = CreateSolidBrush(RGB(0,0,idx));
                SetRect(&rect,idx,idx,512-idx,512-idx);
                FillRect(hDC,&rect,hBrush);
                DeleteObject(hBrush);
            }
        }

        SetMapMode(hDC,nMapMode);

        EndPaint(hWnd,&ps);
    }
    return;
}






/*---------------------------------------------------------------------------*\
| CREATE COLOR SCALE PALETTE
|   This routine creates a palette representing the scale values of a
|   particular RGB color.  A gray-scale palette can also be created.
|
\*---------------------------------------------------------------------------*/
HPALETTE CreateColorScalePalette(HWND hDC, int nColor)
{
    HPALETTE     hPalette;
    GLOBALHANDLE hMem;
    LPLOGPALETTE lpMem;
    int          idx,nReserved,nSize;


    hPalette = NULL;
    if(GetDeviceCaps(hDC,RASTERCAPS) & RC_PALETTE)
    {
        nReserved = GetDeviceCaps(hDC,NUMRESERVED);
        nSize     = GetDeviceCaps(hDC,SIZEPALETTE) - nReserved;

        if(hMem = GlobalAlloc(GHND,(DWORD)sizeof(LOGPALETTE)+(sizeof(PALETTEENTRY)*nSize)))
        {
            if(lpMem = (LPLOGPALETTE)GlobalLock(hMem))
            {
                lpMem->palNumEntries = (WORD)nSize;
                lpMem->palVersion    = (WORD)0x0300;

                switch(nColor)
                {
                    case COLOR_SCALE_RED:
                        for(idx=0; idx < nSize; idx++)
                        {
                            lpMem->palPalEntry[idx].peRed   = (BYTE)idx;
                            lpMem->palPalEntry[idx].peGreen = 0;
                            lpMem->palPalEntry[idx].peBlue  = 0;
                            lpMem->palPalEntry[idx].peFlags = PC_RESERVED;
                        }
                        break;

                    case COLOR_SCALE_GREEN:
                        for(idx=0; idx < nSize; idx++)
                        {
                            lpMem->palPalEntry[idx].peRed   = 0;
                            lpMem->palPalEntry[idx].peGreen = (BYTE)idx;
                            lpMem->palPalEntry[idx].peBlue  = 0;
                            lpMem->palPalEntry[idx].peFlags = PC_RESERVED;
                        }
                        break;

                    case COLOR_SCALE_BLUE:
                        for(idx=0; idx < nSize; idx++)
                        {
                            lpMem->palPalEntry[idx].peRed   = 0;
                            lpMem->palPalEntry[idx].peGreen = 0;
                            lpMem->palPalEntry[idx].peBlue  = (BYTE)idx;
                            lpMem->palPalEntry[idx].peFlags = PC_RESERVED;
                        }
                        break;

                    default:
                    case COLOR_SCALE_GRAY:
                        for(idx=0; idx < nSize; idx++)
                        {
                            lpMem->palPalEntry[idx].peRed   = (BYTE)idx;
                            lpMem->palPalEntry[idx].peGreen = (BYTE)idx;
                            lpMem->palPalEntry[idx].peBlue  = (BYTE)idx;
                            lpMem->palPalEntry[idx].peFlags = PC_RESERVED;
                        }
                        break;
                }

                hPalette = CreatePalette(lpMem);

                GlobalUnlock(hMem);
            }
            GlobalFree(hMem);
        }
    }
    return(hPalette);
}
