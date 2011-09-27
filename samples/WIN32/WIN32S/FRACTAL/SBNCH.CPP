///////////////////////////////////////
//  File:           SThrash.cpp
//  Author:         tleonard
//  Description:    SegThrash benchmark program, main module
//
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include "sthrash.h"
#include "bench.h"
#include "butterf.h"
#include "fractal.h"
 
void Error(const char * s)
{
    MessageBox(NULL, s, NULL, MB_OK);
    exit(EXIT_FAILURE);
}

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

HANDLE hInst;

extern "C"
int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
            LPSTR lpszCmdLine, int nCmdShow)

{
    static char szAppName[] = "WinBmkr1";
    HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    
    if (!hPrevInstance)
    {
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = WndProc;
        wndclass.cbClsExtra = 0;
        wndclass.cbWndExtra = 0;
        wndclass.hInstance = hInstance;
        wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
        wndclass.lpszMenuName = NULL;
        wndclass.lpszClassName = szAppName;

        RegisterClass(&wndclass);
    }
    
    hInst = hInstance;

    hwnd = CreateWindow(szAppName,
                        "Windows Benchmarker v0.5",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam,
                            LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static RECT winrect;
    static HMENU hMenu;

	static BOOL bRunningBenchmark;
	static cBenchmark *pcbmk;

    switch (message)
    {
        case WM_CREATE:
            hMenu = LoadMenu(hInst, "STHRASH_MENU");
            assert(hMenu != NULL);
            SetMenu(hwnd, hMenu);
            return 0;
            break;
        
        case WM_COMMAND:
        {
            switch (wParam)
            {
                case IDM_BUTTERFLY:
                {
                    if (pcbmk)
                        delete pcbmk;
                    pcbmk = new cButterflyBench(hwnd, wButterflyIters);
                    if (!pcbmk)
                    {
                        Error("Butterfly Allocation Failed");
                        return 0;
                    }
                    if (!(*pcbmk))
                    {
                        Error("Butterfly Constructor Failed");
                    }
        		    if(pcbmk->explain())
        		    {
            			pcbmk->run();
            			pcbmk->displayResult();
            		}
            		else
            		{
            		    delete pcbmk;
            		    pcbmk = NULL;
            		}
            		break;
                    
                }
                case IDM_FRACTAL:
                {
                    if (pcbmk)
                        delete pcbmk;
                    pcbmk = new cFractalBench(hwnd, wFractalPanDistance);
                    if (!pcbmk)
                    {
                        Error("Fractal Allocation Failed");
                        return 0;
                    }
                    if (!(*pcbmk))
                    {
                        Error("Fractal Constructor Failed");
                    }
        		    if(pcbmk->explain())
        		    {
            			pcbmk->run();
            			pcbmk->displayResult();
            		}
            		else
            		{
            		    delete pcbmk;
            		    pcbmk = NULL;
            		}
            		break;
                }
                return 0;
            }
        }

        case WM_PAINT:
        {
        	if (pcbmk)
        	{
			    if (pcbmk->repaint() == 0)
			    {
                    GetClientRect(hwnd,&winrect);
                    hdc = BeginPaint(hwnd, &ps);
                    FillRect(hdc, &winrect, GetStockObject(GRAY_BRUSH));
                    EndPaint(hwnd, &ps);
			    }
        	}
        	else
            {
                GetClientRect(hwnd,&winrect);
                hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &winrect, GetStockObject(GRAY_BRUSH));
                EndPaint(hwnd, &ps);
            }
            return 0;
            break;
        }
        
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
            
        case WM_DESTROY:
        	if (pcbmk)
        		delete pcbmk;
            DestroyMenu(hMenu);
            PostQuitMessage (0);
            return 0;
            break;
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

