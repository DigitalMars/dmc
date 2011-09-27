/* IP.c InkPut sample

   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
   PURPOSE.

   Copyright (c) 1993, 1995  Microsoft Corporation.  All Rights Reserved.


   This file demonstrates how to use Pen Windows 2.0 input and
   inking ("inkput") APIs, and Handwriting Recognizer (HRC) APIs.

   For new APIs, a pendata is created for the Polling example, but
   recognition is started for the event example.

   Tabs are set at 1, 4, 7, ...
*/

;
/******************* Includes and Controlling Defines ***********************/
#include <windows.h>
#include <penwin.h>
#include "ip.h"
#include <string.h>

/******************* Defines ************************************************/

#define chNull                   '\0'  // terminator
#define DEFTIMEOUT               1000  // timeout in ms
#define CBCHBUF                  1024
#define CSYVMAX                  64    // symbol chunk

#define INDX_MINEWAPI            0
#define INDX_MIDODEFAULT         1
#define INDX_MIOLDAPI            2

#define INDX_NEW_BEGIN           3
#define INDX_MIAPPRELEASES       3
#define INDX_MIAPPINKS           4
#define INDX_MIDOPOLLING         5
#define INDX_NEW_END             5

#define INDX_OLD_BEGIN           6
#define INDX_MIRECOGNIZE         6
#define INDX_MIRECOGNIZEDATA     7
#define INDX_MIPROCESSWRITING    8
#define INDX_MINULLRECOG         9
#define INDX_OLD_END             9

#define INDX_DEPENDENT_BEGIN     3
#define INDX_DEPENDENT_END       9

#define BUFF_SIZE                64
#define RGBAPP                   RGB(255,0,0)   // app inks in blue
#define RGBDEF                   RGB(0,0,255)   // default ink is red

/******************* Macros *************************************************/

#define Unused(x)                x

// check if a menu option is checked:
#define FMenu(i)        (vrgfMenu[i].fFlag)

// check a menu item, if both its flag and condition f are TRUE:
#define MenuCheck(hmenu, indx, f) \
   CheckMenuItem(hmenu, vrgfMenu[indx].miMenuId, \
   MF_BYCOMMAND | (((f) && FMenu(indx))?\
      MF_CHECKED: MF_UNCHECKED))

// enable a menu item, if condition f is TRUE:
#define MenuEnable(hmenu, indx, f) \
   EnableMenuItem(hmenu, vrgfMenu[indx].miMenuId, \
      MF_BYCOMMAND | ((f)? MF_ENABLED: (MF_DISABLED|MF_GRAYED)))

// invalidate & update:
#define RefreshWindow(hwnd)   do {\
   InvalidateRect(hwnd, NULL, TRUE);\
   UpdateWindow(hwnd);\
} while (0)

// clear pendata:
#define ClearVhpndt() do {\
   if (vhpndt)\
      DestroyPenData(vhpndt);\
   vhpndt = NULL;\
} while (0)

// clear text:
#define ClearText()  vrgcBuff[0] = chNull

/******************* Typedefs ***********************************************/

// menu management struct:
typedef struct tagMENUFLAGS
   {
   UINT miMenuId;
   BOOL fFlag;
   }
   MENUFLAGS;

/******************* Variables **********************************************/
// global variables have a 'v' prefix

// menu options:
MENUFLAGS vrgfMenu[] =
   {
   miNewApi,            TRUE,    // default to use PenWin 2.0 APIs
   miDoDefault,         FALSE,   // use DoDefault
   miOldApi,            FALSE,   // use PenWin 1.0 APIs
   miAppReleases,       FALSE,   // app controls when input ends
   miAppInks,           FALSE,   // app does inking
   miDoPolling,         FALSE,   // input by polling, not events
   miRecognize,         FALSE,   // 1.0 Recognize API
   miRecognizeData,     FALSE,   // 1.0 RecognizeData API
   miProcessWriting,    FALSE,   // 1.0 ProcessWriting API
   miNullRecog,         FALSE    // 1.0 ink collection (null recognizer)
   };

// misc globals, autoinit as NULL:
HWND vhwndMain;                  // main window
HINSTANCE vhinstCur;             // current instance
HPENDATA vhpndt;                 // the pendata
POINT vrgPt[BUFF_SIZE];          // place to buffer points
POINT vptLast;                   // the last point
HDC vhdcScreen;                  // screen DC
HPEN vhpenDraw;                  // drawing GDI pen
HPEN vhpenScreen;                // saved old pen when select vhpenDraw
HPCM vhpcm;                      // handle to pen collection mode
int vnWid2Pen;                   // halfwidth of pen nib
HRC vhrc;                        // handle to a handwriting context
SYV vrgsyv[CSYVMAX];             // symbol result buffer

char *vszInkputClass = "InkputSampleClass";  // main window class name
char *vszInkputWnd = "Inkput Sample";        // main window name
char vrgcBuff[CBCHBUF];                      // char buffer for recog results


/******************* Local prototypes ***************************************/

LPARAM   FAR  PASCAL InkputWndProc(HWND, UINT, WPARAM, LPARAM);
int           PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

VOID     NEAR PASCAL DoPolling(HWND);
BOOL     NEAR PASCAL FInitApp(HINSTANCE);
BOOL     NEAR PASCAL FInitInstance(HINSTANCE, HINSTANCE, int);
BOOL     NEAR PASCAL FTermInstance(VOID);
BOOL     NEAR PASCAL FWMCommandHandler(HWND, WORD);
LRESULT  NEAR PASCAL LDoNewApis(HWND, UINT);
LRESULT  NEAR PASCAL LDoOldApis(HWND);
LRESULT  NEAR PASCAL LPeneventHandler(HWND, WPARAM, LPARAM);
VOID     NEAR PASCAL ManageMenuItems(UINT);
VOID     NEAR PASCAL TPtoCP(HWND, LPPOINT, int, LPRECT);


/******************* PUBLIC FUNCTIONS ***************************************/

/*--------------------------------------------------------------------------*/
LPARAM FAR PASCAL // wndproc ret; EXPORTed
InkputWndProc(    // Window Procedure for app's main window
   HWND hwnd,     // standard wndproc parameters
   UINT message,
   WPARAM wParam,
   LPARAM lParam)
   {
   LPARAM lRet = 0L;
   DWORD dwExtraInfo;

   switch (message)
      {
   case WM_LBUTTONDOWN:
      // get extra info assoc with click/tap event:
      dwExtraInfo = GetMessageExtraInfo();

      if (IsPenEvent(message, dwExtraInfo))  // checks PDK bits
         {
         // use 1.0 APIs:
         if (FMenu(INDX_MIOLDAPI))
            return LDoOldApis(hwnd);

         // use 2.0 inkput APIs:
         if (FMenu(INDX_MINEWAPI))
            return LDoNewApis(hwnd, LOWORD(dwExtraInfo));

         // do the default thing, including recognition:
         DoDefaultPenInput(hwnd, LOWORD(dwExtraInfo));
         }
      break;


   case WM_PENEVENT: // generated by inkput process
      if (!FMenu(INDX_MIOLDAPI) &&
          (lRet = LPeneventHandler(hwnd, wParam, lParam)))
            return lRet;
      break;         // do DefWindowProc


#ifndef WIN32
   case WM_RCRESULT: // generated only by 1.0 APIs
      {
      LPRCRESULT lpr = (LPRCRESULT)lParam; // ptr to RCRESULT struct
      int iCh;

      ClearText();                  // preterminate
      if (lpr->cSyv
         && (lpr->wResultsType == RCRT_DEFAULT || lpr->wResultsType==RCRT_UNIDENTIFIED))
         {
         SymbolToCharacter(lpr->lpsyv, lpr->cSyv, vrgcBuff, &iCh);
         vrgcBuff[iCh] = chNull;    // terminate
         }
      else if (lpr->hpendata)       // Null Recognizer
         {
         POINT pt;
         HPENDATA hpndt = DuplicatePenData(lpr->hpendata, 0);

         // get upper left corner of client in tablet coords:
         pt.x = pt.y = 0;
         ClientToScreen(hwnd, &pt);
         DPtoTP(&pt, 1);

         // move the pendata to effective client origin:
         OffsetPenData(hpndt, -pt.x, -pt.y);

         if (vhpndt) // already exists? append
            {
            InsertPenData(vhpndt, hpndt, IX_END);
            DestroyPenData(hpndt);  // don't need our local copy now
            }
         else
            vhpndt = hpndt;         // assume title
         }
      RefreshWindow(hwnd);
      }
      return 1L;  // continue
#endif // !WIN32


   case WM_PAINT:
      {
      HDC hdc;
      PAINTSTRUCT ps;

      InvalidateRect(hwnd, NULL, TRUE);   // ensure whole window
      if (hdc = BeginPaint(hwnd, &ps))
         {
         if (vhpndt)
            {
            if (FMenu(INDX_MIAPPINKS))
               {
               HPEN hpenOld = SelectObject(hdc, vhpenDraw);
               PENDATAHEADER ph;

               GetPenDataInfo(vhpndt, &ph, NULL, 0);
               TPtoDP((LPPOINT)&ph.rectBound, 2);        // to screen coords
               DrawPenDataEx(hdc, &ph.rectBound, vhpndt, 0, IX_END, 0, IX_END,
                              NULL, NULL, DPD_HDCPEN);   // redraw it
               SelectObject(hdc, hpenOld);               // all done
               }
            else
               RedisplayPenData(hdc, vhpndt, NULL, NULL, 2, RGBAPP);
            }

         if (vrgcBuff[0])
            TextOut(hdc, 0, 0, vrgcBuff, lstrlen(vrgcBuff));

         }
      EndPaint(hwnd, &ps);
      }
      break;


   case WM_COMMAND:
      if (FWMCommandHandler(hwnd, LOWORD(wParam)))
         return 0L;  // we handled it
      break;


   case WM_DESTROY:
      PostQuitMessage(wParam);
      return 1L;


   default:
      break;
      }

   return DefWindowProc(hwnd, message, wParam, lParam);
   }



/*--------------------------------------------------------------------------*/
int PASCAL                 // ret errorlevel 0 if successful, else 1
WinMain(                   // Windows entry point
   HINSTANCE hInstance,    // instance handle
   HINSTANCE hPrevInstance,// previous instance if any
   LPSTR lpszCommandLine,  // pointer to command line string
   int cmdShow)            // ShowWindow parameter
   {
   MSG msg;

   Unused(lpszCommandLine);
   msg.wParam = 1;   // assume error

   if (GetSystemMetrics(SM_PENWINDOWS))   // assumes pen Windows already loaded
      {
      if (hPrevInstance || FInitApp(hInstance))
         {
         if (FInitInstance(hInstance, hPrevInstance, cmdShow))
            {
            while (GetMessage((LPMSG)&msg, NULL, 0, 0))
               {
               if (msg.message == WM_QUIT)
                  {
                  msg.wParam = 0;   // ok
                  break;
                  }
               TranslateMessage((LPMSG)&msg);
               DispatchMessage((LPMSG)&msg);
               }
            FTermInstance();
            }
         }
      }
   else
      MessageBox(NULL,
         (LPSTR)"This application requires Pen Windows to be loaded as an installable driver.",
         NULL, MB_ICONSTOP | MB_OK);

   return msg.wParam;
   }


/******************* PRIVATE FUNCTIONS **************************************/


/*--------------------------------------------------------------------------*/
VOID NEAR PASCAL  // no ret
DoPolling(        // shows how to use polling model to gather pen input
   HWND hwnd)     // handle to window
   {
   int iRet = 0;
   STROKEINFO si;
   POINT pt;
   RECT rectCl;
   MSG msg;
   HRCRESULT hrcresult;          // handle to a recognition result

   for (;;)
      {
      // Be a good Windows app, and yield to other apps:
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
         {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
         }

      // Gather input:
      if ((iRet = GetPenInput(vhpcm, vrgPt, NULL, 0, BUFF_SIZE, &si)) > 0)
         {
         RECT rect;

         // If app wants to release do the required processing:
         if (FMenu(INDX_MIAPPRELEASES) && (si.wPdk & PDK_DOWN))
            {
            pt = vrgPt[0];
            GetClientRect(hwnd, &rectCl);
            TPtoDP(&pt, 1);
            ScreenToClient(hwnd, &pt);
            if (!PtInRect(&rectCl, pt))
               {
               StopPenInput(vhpcm, si.cbPnts, PCMR_APPTERMINATED);
               break;
               }
            }

         /* This is where points could be given to:
         ** a) A recognizer
         ** b) Multiple recognizers
         ** c) Target
         ** d) Control
         ** e) ... or merely create hpendata:
         */

         if (vhrc)
            {
            AddPenInputHRC(vhrc, vrgPt, NULL, 0, &si);
            ProcessHRC(vhrc, PH_DEFAULT);
            }

         TPtoCP(hwnd, vrgPt, iRet, &rect);
         AddPointsPenData(vhpndt, vrgPt, NULL, &si);

         // if app wants to ink:
         if (FMenu(INDX_MIAPPINKS) && vhdcScreen)
            {
            if (si.wPdk & PDK_DOWN)
               {
               if (!(si.wPdk & PDK_TRANSITION))
                  {
                  MoveToEx(vhdcScreen, vptLast.x, vptLast.y, NULL);
                  LineTo(vhdcScreen, vrgPt[0].x, vrgPt[0].y);
                  }

               if (iRet > 1)
                  Polyline(vhdcScreen, vrgPt, iRet);
               else
                  {
                  MoveToEx(vhdcScreen, vrgPt[0].x, vrgPt[0].y, NULL);
                  LineTo(vhdcScreen, vrgPt[0].x+1, vrgPt[0].y);
                  }
               vptLast = vrgPt[iRet-1];
               }
            }
         }

      else if (iRet < 0)
         {
         /* Input terminated.
         ** App would get the results here if recognition was
         ** in progress, and display them if it wanted to.
         */

         if (FMenu(INDX_MIAPPINKS) && vhdcScreen)
            {
            if (vhpenScreen)
               {
               SelectObject(vhdcScreen, vhpenScreen);
               vhpenScreen = NULL;
               }
            ReleaseDC(vhwndMain, vhdcScreen);
            vhdcScreen = NULL;
            }

         break;
         }
      }

   // recognize and display results:
   if (vhrc)
      {
      EndPenInputHRC(vhrc);                  // tell HRC end of ink
      ProcessHRC(vhrc, PH_MAX);              // finish recog

      // get a handle to the results:
      if (GetResultsHRC(vhrc, GRH_ALL, &hrcresult, 1) > 0)
         {
         int i = 0, cSyv = 0;

         // get a chunk of symbols:
         while ((cSyv = GetSymbolsHRCRESULT(hrcresult, i, vrgsyv, CSYVMAX))
            > 0)
            {
            if (i + cSyv + 1 > CBCHBUF)      // don't overflow char buff
               cSyv = CBCHBUF - i - 1;
            if (cSyv > 0)                    // still have something?
               {
               SymbolToCharacter(vrgsyv, cSyv, vrgcBuff + i, NULL);
               i += cSyv;
               }
            if (i + 1 >= CBCHBUF)
               break;
            }
         vrgcBuff[i] = chNull;               // terminate string

         DestroyHRCRESULT(hrcresult);        // we're finished with result
         }

      DestroyHRC(vhrc);                   // destroy HRC
      RefreshWindow(hwnd);                // view the text
      }
   }



/*--------------------------------------------------------------------------*/
BOOL NEAR PASCAL        // ret T if successful
FInitApp(               // register class - called only once
   HINSTANCE hInstance)
   {
   WNDCLASS wc;

   wc.hCursor        = LoadCursor(NULL, IDC_PEN);
   wc.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(iconInkput));
   wc.lpszMenuName   = MAKEINTRESOURCE(menuMain);
   wc.lpszClassName  = (LPSTR)vszInkputClass;
   wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
   wc.hInstance      = hInstance;
   wc.style          = CS_VREDRAW | CS_HREDRAW;
   wc.lpfnWndProc    = InkputWndProc;
   wc.cbClsExtra     = 0;
   wc.cbWndExtra     = 0;

   return RegisterClass((LPWNDCLASS) &wc);
   }



/*--------------------------------------------------------------------------*/
BOOL NEAR PASCAL           // ret T if successful
FInitInstance(             // init data for instance and create windows
   HINSTANCE hInstance,    // handle to instance
   HINSTANCE hPrevInstance,// handle to previous instance or NULL
   int cmdShow)            // ShowWindow param
   {
   Unused(hPrevInstance);
   vhinstCur = hInstance;  // set global

   // main window:
   if (vhwndMain = CreateWindow(
      (LPSTR)vszInkputClass,
      (LPSTR)vszInkputWnd,
      WS_SYSMENU | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL))
      {
      POINT pt;

      pt.x = 2;
      pt.y = 0;
      DPtoTP(&pt,1);

      vhpenDraw = CreatePen(PS_SOLID, pt.x, RGBDEF);
      vnWid2Pen = pt.x / 2 + pt.x % 2;
      ManageMenuItems(INDX_MINEWAPI);

      ShowWindow(vhwndMain, cmdShow);
      UpdateWindow(vhwndMain);
      }

   return vhwndMain != NULL;
   }



/*--------------------------------------------------------------------------*/
BOOL NEAR PASCAL        // ret T if successful
FTermInstance(          // clean up at end of instance
   VOID)                // no params
   {
   ClearVhpndt();       // goodbye pen data

   if (vhdcScreen)
      {
      if (vhpenScreen)
         {
         SelectObject(vhdcScreen, vhpenScreen);
         vhpenScreen = NULL;
         }
      ReleaseDC(vhwndMain, vhdcScreen);
      vhdcScreen = NULL;
      }

   if (vhpenDraw)
      {
      DeleteObject(vhpenDraw);
      vhpenDraw = NULL;
      }

   return TRUE;
   }

/*--------------------------------------------------------------------------*/
BOOL NEAR PASCAL     // ret TRUE if handled, else FALSE
FWMCommandHandler(   // handles WM_COMMAND message for menu choices
   HWND hwnd,        // handle to window
   WORD wId)
   {
   switch (wId)
      {
   case miClearAll:
      {
      ClearVhpndt();
      ClearText();
      RefreshWindow(hwnd);
      break;
      }

   case miExit:
      DestroyWindow(hwnd);
      break;

   case miNewApi:
      ManageMenuItems(INDX_MINEWAPI);
      ClearText();
      RefreshWindow(hwnd);
      break;

   case miDoPolling:
      ManageMenuItems(INDX_MIDOPOLLING);
      break;

   case miAppReleases:
      ManageMenuItems(INDX_MIAPPRELEASES);
      break;

   case miAppInks:
      ManageMenuItems(INDX_MIAPPINKS);
      break;

   case miOldApi:
      ManageMenuItems(INDX_MIOLDAPI);
      break;

   case miRecognize:
      ManageMenuItems(INDX_MIRECOGNIZE);
      ClearText();
      RefreshWindow(hwnd);
      break;

   case miRecognizeData:
      ManageMenuItems(INDX_MIRECOGNIZEDATA);
      if (FMenu(INDX_MIRECOGNIZEDATA))
         LDoOldApis(hwnd);                   // recognize now
      ManageMenuItems(INDX_MIRECOGNIZEDATA); // switch back to Recognize
      break;

   case miNullRecog:
      ManageMenuItems(INDX_MINULLRECOG);
      ClearText();
      RefreshWindow(hwnd);
      break;

   case miProcessWriting:
      ManageMenuItems(INDX_MIPROCESSWRITING);
      ClearText();
      RefreshWindow(hwnd);
      break;

   case miDoDefault:
      ManageMenuItems(INDX_MIDODEFAULT);
      ClearText();
      RefreshWindow(hwnd);
      break;

   default:
      return FALSE;
      }

   return TRUE;
   }



/*--------------------------------------------------------------------------*/
LRESULT NEAR PASCAL  // wndproc ret to WM_LBUTTONDOWN
LDoNewApis(          // demo inkput APIs
   HWND hwnd,        // handle to window
   UINT idEvent)  // pointer to data
   {
   PCMINFO pcminfo;  // pen collection mode structure
   int errCode;

   /* Options:
   ** a) Do event driven data collection or polling
   ** b) Have the pen driver release input automatically or
   **    keep it under application's control
   ** c) Have pen/display driver handle inking or leave it under
   **    application's control.
   */

   // if app wants to release peninput, don't set any automatic
   // input release conditions, otherwise init fields & bits:

   pcminfo.cbSize = sizeof(PCMINFO);
   if (FMenu(INDX_MIAPPRELEASES))
      pcminfo.dwPcm = 0L;  // no pen termination conditions set
   else
      {
      pcminfo.dwPcm = PCM_RECTBOUND | PCM_TIMEOUT; // default to timeout+rect
      pcminfo.dwTimeout = (DWORD)DEFTIMEOUT;       // 1 second

      // set inclusion rect to client area, in screen coords:
      GetClientRect(hwnd, &pcminfo.rectBound);
      ClientToScreen(hwnd, (LPPOINT)&pcminfo.rectBound);
      ClientToScreen(hwnd, (LPPOINT)&pcminfo.rectBound.right);
      }

   // if app wants to do polling, set the PCM_DOPOLLING bit:
   if (FMenu(INDX_MIDOPOLLING))
      pcminfo.dwPcm |= PCM_DOPOLLING;


   // Start gathering input:
   ///////////////////////////////////////////////////////////////////////
   if (!(vhpcm = StartPenInput(vhwndMain, idEvent, &pcminfo, &errCode)))
      return 0L;
   ///////////////////////////////////////////////////////////////////////

   if (FMenu(INDX_MIAPPINKS))
      {
      RECT rect;

      /* App wants to do inking by itself. Get all the corresponding GDI
      ** objects. DC could be obtained using GetDC or CreateDC.
      ** DC will be released in DoPolling() or LPeneventHandler():
      */

      if (vhdcScreen = GetDC(vhwndMain))
         {
         SetMapMode(vhdcScreen, MM_ANISOTROPIC);
         GetClientRect(vhwndMain, &rect);
         SetViewportExtEx(vhdcScreen, rect.right, rect.bottom, NULL);
         DPtoTP((LPPOINT)&rect.right, 1);
         SetWindowExtEx(vhdcScreen, rect.right, rect.bottom, NULL);

         if (vhpenDraw)
            vhpenScreen = SelectObject(vhdcScreen, vhpenDraw);
         }
      }
   else  // system inking:
   ///////////////////////////////////////////////////////////////////////
      StartInking(vhpcm, idEvent, NULL);
   ///////////////////////////////////////////////////////////////////////

   /* This is where an app would start:
   ** a) A new HRC for recognition
   ** b) Multiple HRCs
   ** c) Targeting
   ** d) ... or merely create an hpendata:
   */

   if (!vhpndt)
      vhpndt = CreatePenDataEx(NULL, PDTS_STANDARDSCALE, CPD_DEFAULT, GMEM_SHARE);

   // create global HRC handle here (may be NULL if no recognizer):
   vhrc = CreateCompatibleHRC(
      NULL,    // no default template
      NULL);   // use system default recognizer


   // If app wants to poll for points fall into a polling loop:
   if (FMenu(INDX_MIDOPOLLING))
      DoPolling(hwnd);

   return 1L;
   }



/*--------------------------------------------------------------------------*/
LRESULT NEAR PASCAL  // WndProc ret
LDoOldApis(          // use penwin 1.0 APIs
   HWND hwnd)        // handle to window
   {
#ifdef WIN32
   return 0;
#else // WIN32
   RC rc;   // Recog Context

   InitRC(hwnd, &rc);
   GetClientRect(hwnd, &rc.rectBound);
   ClientToScreen(hwnd, (LPPOINT)&rc.rectBound);
   ClientToScreen(hwnd, (LPPOINT)&rc.rectBound.right);

   rc.lRcOptions |= RCO_NOHIDECURSOR;

   if (FMenu(INDX_MIRECOGNIZE))
      {
      ClearVhpndt();
      Recognize(&rc);
      }

   else if (FMenu(INDX_MIRECOGNIZEDATA))
      {
      if (vhpndt)
         {
         RecognizeData(&rc, vhpndt);
         ClearVhpndt();
         RefreshWindow(hwnd);
         }
      }

   else if (FMenu(INDX_MINULLRECOG))
      {
      rc.hrec = NULL;
      Recognize(&rc);
      }

   else if (FMenu(INDX_MIPROCESSWRITING))
      {
      ClearVhpndt();
      ProcessWriting(hwnd, &rc);
      }

   return 1L;
#endif // WIN32
   }



/*--------------------------------------------------------------------------*/
LRESULT NEAR PASCAL  // ret value used by WndProc
LPeneventHandler(    // handles WM_PENEVENT messages during pen input
   HWND hwnd,        // handle to a window
   WPARAM wParam,    // first wndproc param
   LPARAM lParam)    // second wndproc param
   {
   int iRet;
   STROKEINFO si;
   POINT pt;
   HPCM hpcm;
   HRCRESULT hrcresult;          // handle to a recognition result

   // If we are doing Default Input, deal only with PE_ENDDATA message
   if (FMenu(INDX_MIDODEFAULT) &&
       (SubPenMsgFromWpLp(wParam, lParam) != PE_ENDDATA))
      return 0L;

   hpcm = HpcmFromWpLp(wParam, lParam);
   switch (SubPenMsgFromWpLp(wParam, lParam))
      {
   case PE_PENDOWN:
      if (FMenu(INDX_MIAPPRELEASES))
         {
         RECT rectCl;

         /* If app wants to release input on its own - our example
         ** releases if pen down outside client rect; the user will see
         ** the pen cursor change to an arrow outside the client area:
         */

         // Examine this [pendown] penpacket without dequeueing it:
         iRet = PeekPenInput(hpcm, EventRefFromWpLp(wParam, lParam), &pt, NULL, 0);

         // Check if it falls within the client bounds:
         GetClientRect(hwnd, &rectCl);
         TPtoDP(&pt, 1);
         ScreenToClient(hwnd, &pt);
         if (!PtInRect(&rectCl, pt))   // end pen input at this packet
            {
            StopPenInput(hpcm, EventRefFromWpLp(wParam, lParam), PCMR_APPTERMINATED);
            break;
            }
         }
         // Fall through
   case PE_PENUP:
   case PE_PENMOVE:
   case PE_TERMINATING:
      // Get all the points so far:
      while ((iRet = GetPenInput(hpcm, vrgPt, NULL, 0, BUFF_SIZE, &si)) > 0)
         {
         RECT rect;

         /* Do whatever needs to be done with the points.
         ** They could be given to:
         ** a) A recognizer (this is what we do here)
         ** b) Multiple recognizers
         ** c) Target
         ** d) Control
         ** ... or merely create an hpendata
         */

         if (vhrc)
            {
            AddPenInputHRC(vhrc, vrgPt, NULL, 0, &si);
            ProcessHRC(vhrc, PH_DEFAULT);
            }

         TPtoCP(hwnd, vrgPt, iRet, &rect);
         AddPointsPenData(vhpndt, vrgPt, NULL, &si);

         if (FMenu(INDX_MIAPPINKS) && vhdcScreen)
            {
            // If app wants to do its own inking:
            if (si.wPdk & PDK_DOWN)
               {
               if (!(si.wPdk & PDK_TRANSITION))
                  {
                  MoveToEx(vhdcScreen, vptLast.x, vptLast.y, NULL);
                  LineTo(vhdcScreen, vrgPt[0].x, vrgPt[0].y);
                  }

               if (iRet > 1)
                  Polyline(vhdcScreen, vrgPt, iRet);
               else
                  {
                  MoveToEx(vhdcScreen, vrgPt[0].x, vrgPt[0].y, NULL);
                  LineTo(vhdcScreen, vrgPt[0].x+1, vrgPt[0].y);
                  }

               vptLast = vrgPt[iRet-1];
               }
            }
         }
      break;

   case PE_TERMINATED: // pen input has been terminated for this application
   case PE_ENDDATA:    // Default Pen Input terminated

      /* Input terminated.
      **
      ** Get the results here and display them:
      */
      if (SubPenMsgFromWpLp(wParam, lParam) == PE_TERMINATED)
         {
         // if app was inking, clean up the GDI objects:
         if (FMenu(INDX_MIAPPINKS) && vhdcScreen)
            {
            if (vhpenScreen)
               {
               SelectObject(vhdcScreen, vhpenScreen);
               vhpenScreen = NULL;
               }
            ReleaseDC(vhwndMain, vhdcScreen);
            vhdcScreen = NULL;
            }
         }
      else
         vhrc = (HRC)(((LPTARGET)lParam)->dwData);

      // terminate recognition and display results:
      if (vhrc)
         {
         EndPenInputHRC(vhrc);                  // tell HRC end of ink
         ProcessHRC(vhrc, PH_MAX);              // finish recog

         // get a handle to the results:
         if (GetResultsHRC(vhrc, GRH_ALL, &hrcresult, 1) > 0)
            {
            int i = 0, cSyv = 0;

            // get a chunk of symbols:
            while ((cSyv = GetSymbolsHRCRESULT(hrcresult, i, vrgsyv, CSYVMAX))
               > 0)
               {
               if (i + cSyv + 1 > CBCHBUF)      // don't overflow char buff
                  cSyv = CBCHBUF - i - 1;
               if (cSyv > 0)                    // still have something?
                  {
                  SymbolToCharacter(vrgsyv, cSyv, vrgcBuff + i, NULL);
                  i += cSyv;
                  }
               if (i + 1 >= CBCHBUF)
                  break;
               }
            vrgcBuff[i] = chNull;               // terminate string

            DestroyHRCRESULT(hrcresult);        // we're finished with result
            }

         if (SubPenMsgFromWpLp(wParam, lParam) == PE_TERMINATED)
            DestroyHRC(vhrc);                   // done with this HRC session
         }

      RefreshWindow(hwnd);                   // view the text
      break;

   default:
      return 0L;
      }

   return 1L;
   }



/*--------------------------------------------------------------------------*/
VOID NEAR PASCAL  // no ret
ManageMenuItems(  // Set up appropriate flags for doing pen APIs
   UINT miIndx)   // index of item
   {
   int i;
   HMENU hMenu = GetMenu(vhwndMain);
   BOOL fFlagSav = FMenu(miIndx);
   BOOL f;
   BOOL fTypeChange = ((miIndx==INDX_MIOLDAPI)
      || (miIndx==INDX_MINEWAPI)
      || (miIndx==INDX_MIDODEFAULT));
   HREC hrec = NULL;

   if (GetPenMiscInfo(PMI_SYSREC, (LPARAM)(HREC FAR *)&hrec) != PMIR_OK)
      hrec = NULL;

   for (i = INDX_OLD_BEGIN; i <= INDX_OLD_END; ++i)
      vrgfMenu[i].fFlag = FALSE;

   if (!fTypeChange)
      {
      vrgfMenu[miIndx].fFlag = fFlagSav ^ 1;

      // If it is one of the penwin1.0 APIs:
      if (miIndx >= INDX_OLD_BEGIN && miIndx <= INDX_OLD_END)
         vrgfMenu[FMenu(miIndx)? miIndx: INDX_MIRECOGNIZE].fFlag = TRUE;
      }
   else
      {
      for (i = 0; i < INDX_DEPENDENT_BEGIN; ++i)
         vrgfMenu[i].fFlag = FALSE;

      vrgfMenu[miIndx].fFlag = TRUE;

      if (vrgfMenu[INDX_MIOLDAPI].fFlag)
         vrgfMenu[INDX_MIRECOGNIZE].fFlag = TRUE;

      if (FMenu(INDX_MINEWAPI))
         for (i = INDX_NEW_BEGIN; i <= INDX_NEW_END; ++i)
            vrgfMenu[i].fFlag = FALSE;
      }

   // Set check marks in the "Type" menuitems:
#ifndef WIN32
   MenuCheck(hMenu, INDX_MIOLDAPI, TRUE);
#endif // !WIN32
   MenuCheck(hMenu, INDX_MINEWAPI, TRUE);
   MenuCheck(hMenu, INDX_MIDODEFAULT, TRUE);

   f = FMenu(INDX_MINEWAPI);
   for (i = INDX_NEW_BEGIN; i <= INDX_NEW_END; ++i)
      {
      MenuCheck(hMenu, i, f);
      MenuEnable(hMenu, i, f);
      }

   f = FMenu(INDX_MIOLDAPI);
   for (i = INDX_OLD_BEGIN; i <= INDX_OLD_END; ++i)
      {
      MenuCheck(hMenu, i, f);
      MenuEnable(hMenu, i, f);
      }

   // if no recognizer, disable some options:
   if (!hrec)
      {
      MenuCheck(hMenu, INDX_MIRECOGNIZE, FALSE);
      MenuCheck(hMenu, INDX_MIRECOGNIZEDATA, FALSE);
      MenuCheck(hMenu, INDX_MIPROCESSWRITING, FALSE);
      MenuCheck(hMenu, INDX_MIDODEFAULT, FALSE);

      MenuEnable(hMenu, INDX_MIRECOGNIZE, FALSE);
      MenuEnable(hMenu, INDX_MIRECOGNIZEDATA, FALSE);
      MenuEnable(hMenu, INDX_MIPROCESSWRITING, FALSE);
      MenuEnable(hMenu, INDX_MIDODEFAULT, FALSE);
      }
   }



/*--------------------------------------------------------------------------*/
VOID NEAR PASCAL        // no ret
TPtoCP(                 // Convert tablet to client coords
   HWND hwnd,           // Handle to client window
   LPPOINT lppt,        // Pointer to an array of points
   int cpt,             // Count of points
   LPRECT lprect)       // Bounding rectangle calculated, or NULL
   {
   POINT pt;
   BOOL fFirst = TRUE;

   // get upper left corner of client in tablet coords:
   pt.x = pt.y = 0;
   ClientToScreen(hwnd, &pt);
   DPtoTP(&pt, 1);

   for (lppt = lppt + cpt - 1; cpt > 0; --cpt, --lppt)
      {
      lppt->x -= pt.x;
      lppt->y -= pt.y;

      if (lprect)
         {
         if (fFirst)
            {
            lprect->left = lprect->right = lppt->x;
            lprect->top = lprect->bottom = lppt->y;
            fFirst = FALSE;
            }

         if (lppt->x > lprect->right)
            lprect->right = lppt->x;
         else if (lppt->x < lprect->left)
            lprect->left = lppt->x;

         if (lppt->y > lprect->bottom)
            lprect->bottom = lppt->y;
         else if (lppt->y < lprect->left)
            lprect->top = lppt->y;
         }
      }

   if (lprect)
      TPtoDP((LPPOINT)lprect, 2);
   }
