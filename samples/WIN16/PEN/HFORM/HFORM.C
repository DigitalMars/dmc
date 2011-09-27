/* PROGRAM: HFORM.C version 2.0

   PURPOSE:

      A program demonstrating the handwriting (HEdit and BEdit)
      and ink edit (IEdit) controls.

   COMMENTS:

      HFORM has several edit fields, typical of a generic
      form application (name, address, city, etc.).  The
      application registers itself as a pen aware application.
      (The Zip code field is too small for a Lens button to
      appear. This is intentional, to illustrate that constraint.)

      The lowermost field illustrates the ink edit control.
      In an earlier version of this sample, an HEDIT variant
      occupied that space.

      THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
      KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
      PURPOSE.

      Copyright (c) 1992-1995  Microsoft Corporation.  All Rights Reserved.

      Tabs are set at 1, 4, 7, ...
*/

/******************* Includes ***********************************************/
#include <windows.h>
#include <penwin.h>
#include "hfres.h"

/******************* Defines ************************************************/
#define NAME            0
#define ADDRESS         1
#define CITY            2
#define ZIP             3
#define NOTES           4
#define TEL             5
#define MAP             6
#define cFieldsMax      7

#define cFormWidth      180   /* In dialog units */
#define cFormHeight     220   /* In dialog units */
#define cbBufMax        1024  /* for file transfer  */

#define cchTitleMax     32
#define cbSzDlgMax      80
#define cbSzTMax        1024

#define chNextField     1
#define chPrecField     2

#define FIELDHEDIT      0
#define FIELDBEDIT      1
#define FIELDIEDIT      2

/******************* Macros *************************************************/
#define Unused(x)    x     /* To prevent warning message */

/* typedef stubs */
#define TypedefInst(RET, Name, args) \
   typedef RET (WINAPI *f##Name)##args

// invoker:
// (*(TYPE)(vrgFn[uId]))
#define FnPW(Name)   (*(f##Name)(vrgFn[u##Name]))

/******************* Typedefs ***********************************************/
typedef struct
   {
   char  szTitle[cchTitleMax];
   int   xStatic;
   int   yStatic;
   int   xEdit;
   int   yEdit;
   int   cxEdit;
   int   cyEdit;
   DWORD dwStyle;
   DWORD dwExStyle;
   ALC   alc;
   BOOL  fEnableGestures;
   UINT  wFieldType;
   UINT  wLimit;
   HWND  hwnd;
   }
   FIELD, *PFIELD, FAR *LPFIELD;    /* field */

TypedefInst(int,  DestroyHRC,(HRC));
TypedefInst(int,  DoDefaultPenInput,(HWND, UINT));
TypedefInst(int,  EnableGestureSetHRC,(HRC, SYV, BOOL));
TypedefInst(BOOL, IsPenEvent,(UINT, LONG));
TypedefInst(int,  SetAlphabetHRC,(HRC, ALC, LPBYTE));
TypedefInst(int,  SetWordlistCoercionHRC,(HRC, UINT));

/******************* Public Variables ***************************************/

BOOL fAppwideHookSet;
char *szHformClass         = "HformSampleClass";
char *szIeditROClass       = "ieditROClass";

#ifdef WIN32
char *szHformWnd           = "Hform Sample (32-bit)";
char *szPenWinDll          = "penwin32.dll";
char *szAppName            = "hform32";
#else
char *szHformWnd           = "Hform Sample";
char *szAppName            = "hform";
#endif //WIN32

char *szHookErr            = "Could not set hook";
char *szTarget             = "Target";
char *szFile               = "HFORM.DAT";

HANDLE hAccel;             /* Menu Accelerator Table */

HWND vhwndMain;            /* Parent window to all fields */
HWND vhwndFocus = NULL;    /* window with focus */

HINSTANCE vhInstanceApp;
HINSTANCE vhinstPW = NULL; /* Pen Windows  */

FIELD vrgfield[cFieldsMax] =
   {
   {"Name:",     6,   8,  44,   8, 128, 10,
      ES_AUTOHSCROLL, 0,
      ALC_DEFAULT, 1, FIELDHEDIT, 0, NULL},
   {"Address:",  6,  24,  44,  24, 128, 10,
      ES_AUTOHSCROLL, 0,
      ALC_DEFAULT, 1, FIELDHEDIT, 0, NULL},
   {"City:",     6,  40,  44,  40,  84, 10,
      ES_AUTOHSCROLL, 0,
      ALC_DEFAULT, 1, FIELDHEDIT, 0, NULL},
   {"Zip:",    132,  40, 147,  40,  25, 10,
      0, 0,
      ALC_NUMERIC, 1, FIELDHEDIT, 5, NULL},
   {"Notes:",    6,  56,  44,  54, 130, 40,
      WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_BORDER, 0,
      ALC_DEFAULT, 1, FIELDHEDIT, 0, NULL},
   {"Tel #:",    6, 102,  44,  98, BXD_CELLWIDTH*10+5, BXD_CELLHEIGHT+1,
      WS_BORDER, 0,
      ALC_NUMERIC, 1, FIELDBEDIT, 0, NULL},
   {"Map:",      6, 120,  44, 118, 130, 70,
      WS_BORDER, 0,
      0, 1, FIELDIEDIT, 0, NULL},
   };


NPSTR vrgszFn[] =
   {
   #define  uDestroyHRC                0
            "DestroyHRC",

   #define  uDoDefaultPenInput         1
            "DoDefaultPenInput",

   #define  uEnableGestureSetHRC       2
            "EnableGestureSetHRC",

   #define  uIsPenEvent                3
            "IsPenEvent",

   #define  uSetAlphabetHRC            4
            "SetAlphabetHRC",

   #define  uSetWordlistCoercionHRC    5
            "SetWordlistCoercionHRC",

   };
#define cFnNames (sizeof(vrgszFn) / sizeof(NPSTR))

// function table ptr:
FARPROC FAR* vrgFn;

/******************* Local prototypes ***************************************/
int PASCAL  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCommandLine, int cmdShow);

LONG     WINAPI   HformWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LONG     WINAPI   IeditROWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL     WINAPI   SampleDlgProc(HWND hdlg, UINT message, UINT wParam, LONG lParam);

void     NEAR  PASCAL   ErrBox(HINSTANCE hinst, int rs, BOOL fError);
BOOL     NEAR  PASCAL   FCreateForm(HWND hwndParent);
BOOL     NEAR  PASCAL   FGetHpndt(HWND hwnd, HPENDATA NEAR *phpndt);
BOOL     NEAR  PASCAL   FInitApp(HANDLE hInstance, BOOL fFirstInstance);
BOOL     NEAR  PASCAL   FInitInstance(HANDLE hInstance, HANDLE hPrevInstance, int cmdShow);
BOOL     NEAR  PASCAL   FInitPWStubs(HINSTANCE hinst, BOOL fInit);
BOOL     NEAR  PASCAL   FLoadSave(BOOL fLoad);
BOOL     NEAR  PASCAL   FLoadSaveText(HFILE hfile, BOOL fLoad, HWND hwnd);
BOOL     NEAR  PASCAL   FPenOrStubPresent(VOID);
BOOL     NEAR  PASCAL   FSetHpndt(HWND hwnd, HPENDATA hpndt);
VOID     NEAR  PASCAL   FTermApp(VOID);
int      NEAR  PASCAL   IFromHwnd(HWND hwnd);
HPENDATA NEAR  PASCAL   ReadPenData(HFILE hfile);
BOOL     NEAR  PASCAL   ProcessFieldChange(UINT wParam);
VOID     NEAR  PASCAL   SampleDialog(HWND hinstance);
BOOL     NEAR  PASCAL   WritePenData(HFILE hfile, HPENDATA hpndt);

/******************* End of Header Section **********************************/



int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                  LPSTR lpszCommandLine, int cmdShow)
/***************************************************************

FUNCTION:   WinMain(hInstance, hPrevInstance, lpszCommandLine, cmdShow)

PURPOSE:

   Main Windows application function.

***************************************************************/
   {
   MSG   msg;

   Unused(lpszCommandLine);

   if (!FInitApp(hInstance, hPrevInstance == NULL))
      return 1;

   if (FInitInstance(hInstance, hPrevInstance, cmdShow))
      {
      while (GetMessage((LPMSG)&msg,NULL,0,0) )
         {
         /* Check for menu accelerator message */
         if (!TranslateAccelerator(vhwndMain, hAccel, &msg))
            {
            TranslateMessage((LPMSG)&msg);
            DispatchMessage((LPMSG)&msg);
            }
         }
      }
   else
      msg.wParam = 0;

   FTermApp();

   return msg.wParam;
   }



void NEAR PASCAL ErrBox(HINSTANCE hinst, int rs, BOOL fError)
/***************************************************************

FUNCTION:   ErrBox(hinst, rs, fError)

PURPOSE:

   Put user error message based on resource id. If fError
   is TRUE, "Error" will appear as the caption.

***************************************************************/
   {
   char  sz[128];

   if (LoadString(hinst, rs, (LPSTR)sz, sizeof(sz)))
      MessageBox(NULL, (LPSTR)sz, fError ? NULL : szAppName, MB_OK);
   }


BOOL NEAR PASCAL FInitApp(HINSTANCE hInstance, BOOL fFirstInstance)
/***************************************************************

FUNCTION:   FInitApp(hInstance, fFirstInstance)

PURPOSE:

   Initialize application data and register window classes.

   Returns FALSE if function could not register window classes.
   TRUE if successful.

***************************************************************/
   {
   WNDCLASS wc;
   HCURSOR  hcursorArrow;

   hcursorArrow = LoadCursor(NULL, IDC_ARROW);

   /* check for Pen Windows: */
   if (vhinstPW = (HINSTANCE)GetSystemMetrics(SM_PENWINDOWS))
      {
      if (!FInitPWStubs(vhinstPW, TRUE))
         return FALSE;
      }

   else if (fFirstInstance)
      /* no Pen Windows: use read-only (RO) mode */
      {
      ErrBox(hInstance, rsNoPW, FALSE);

      /* register readonly IEDIT class if no penwindows */

      wc.hCursor = hcursorArrow;
      wc.hIcon = NULL;
      wc.lpszMenuName = NULL;
      wc.lpszClassName = (LPSTR)szIeditROClass;
      wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
      wc.hInstance = hInstance;
      wc.style = 0;
      wc.lpfnWndProc = IeditROWndProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = sizeof(LONG);

      if (!RegisterClass((LPWNDCLASS) &wc))
         return FALSE;
      }

   /* register main window class */
   if (fFirstInstance)
      {
      wc.hCursor = vhinstPW ? LoadCursor(vhinstPW, IDC_PEN) : hcursorArrow;
      wc.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(iconHform));
      wc.lpszMenuName = MAKEINTRESOURCE(menuMain);
      wc.lpszClassName = (LPSTR)szHformClass;
      wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
      wc.hInstance = hInstance;
      wc.style = 0;
      wc.lpfnWndProc = HformWndProc;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      if (!RegisterClass((LPWNDCLASS) &wc))
         return FALSE;
      }

   return TRUE;
   }


BOOL NEAR PASCAL FInitInstance(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                              int cmdShow)
/***************************************************************

FUNCTION:   FInitInstance(hInstance, hPrevInstance, cmdShow)

PURPOSE:

   Initialize all data structures for program instance and create
   the necessary windows.

   Returns TRUE if successsful, FALSE if failed.

***************************************************************/
   {
   int   i;
   LONG  lT          = GetDialogBaseUnits();
   int   cxDlgBase   = LOWORD(lT);
   int   cyDlgBase   = HIWORD(lT);

   Unused(hPrevInstance);

   vhInstanceApp = hInstance;

   /* Convert field coordinates to window coordinates */
   for (i = 0; i < cFieldsMax; i++)
      {
      vrgfield[i].xStatic = (vrgfield[i].xStatic * cxDlgBase)/4;
      vrgfield[i].yStatic = (vrgfield[i].yStatic * cyDlgBase)/8;
      vrgfield[i].xEdit = (vrgfield[i].xEdit * cxDlgBase)/4;
      vrgfield[i].yEdit = (vrgfield[i].yEdit * cyDlgBase)/8;
      vrgfield[i].cxEdit = (vrgfield[i].cxEdit * cxDlgBase)/4;
      vrgfield[i].cyEdit = (vrgfield[i].cyEdit * cyDlgBase)/8;
      }

   /* Create Main window */

   if (vhwndMain = CreateWindow((LPSTR)szHformClass,
      (LPSTR)szHformWnd,
      WS_CLIPCHILDREN | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, (cFormWidth*cxDlgBase)/4, (cFormHeight*cyDlgBase)/8,
      NULL,
      NULL,
      hInstance,
      NULL
      ))
      {

      hAccel = LoadAccelerators(hInstance, IDHFORM);
      ShowWindow(vhwndMain, cmdShow);
      UpdateWindow(vhwndMain);
      }

   SetFocus(vhwndFocus = vrgfield[NAME].hwnd);
   return vhwndMain != NULL;
   }


VOID NEAR PASCAL FTermApp(VOID)
/***************************************************************

FUNCTION:   FTermApp()

PURPOSE:

   Terminate application and free memory.

***************************************************************/
   {
   if (vrgFn)
      FInitPWStubs(vhinstPW, FALSE);
   }


LONG WINAPI IeditROWndProc(HWND hwnd, UINT message,
            WPARAM wParam, LPARAM lParam)
/***************************************************************

FUNCTION:   IeditROWndProc(hwnd, message, wParam, lParam)

PURPOSE:

   Windows procedure for readonly IEDIT window.

***************************************************************/
   {
   LONG     lRet  = 0L;

   switch (message)
      {
      case WM_PAINT:
         {
         PAINTSTRUCT ps;
         HDC         hdc = BeginPaint(hwnd, &ps);

         if (hdc)
            {
            HPENDATA hpndt = (HPENDATA)GetWindowLong(hwnd, 0);

            if (hpndt)
               DrawPenDataFmt(hdc, NULL, hpndt);
            EndPaint(hwnd, &ps);
            }
         break;
         }
      break;

   default:
      lRet = DefWindowProc(hwnd, message, wParam, lParam);
      break;
      }

   return lRet;
   }


LONG WINAPI HformWndProc(HWND hwnd, UINT message,
            WPARAM wParam, LPARAM lParam)
/***************************************************************

FUNCTION:   HformWndProc(hwnd, message, wParam, lParam)

PURPOSE:

   Windows procedure for application's parent window.

***************************************************************/
   {
   int      i;
   LONG     lRet  = 0L;

   static HBRUSH hbrBackgnd = NULL;

   switch (message)
      {
      case WM_CREATE:
         /* Create fields */

         if (!FCreateForm(hwnd))
            {
            lRet = 1L;  /* Failed */
            break;
            }

         hbrBackgnd = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
         break;

      case WM_LBUTTONDOWN:
         if (vhinstPW)
            {
            LONG lExtraInfo = GetMessageExtraInfo();

            if (FnPW(IsPenEvent)(message, lExtraInfo)
               && (FnPW(DoDefaultPenInput)(hwnd, LOWORD(lExtraInfo))
                  == PCMR_OK))
                  return 1L;
            }
         break;

      case WM_COMMAND:
         {
         /* Edit control commands */

         UINT wId = LOWORD(wParam);
#ifdef WIN32
         UINT wNotify = HIWORD(wParam);
         HWND hwndCtl = (HWND)lParam;
#else
         UINT wNotify = HIWORD(lParam);
         HWND hwndCtl = (HWND)LOWORD(lParam);
#endif //WIN32

         if (wNotify == EN_SETFOCUS || wNotify == IN_SETFOCUS)
            {
            /* Field focus is being set */
            vhwndFocus = hwndCtl;
            break;
            }

         /* break if non-menu */
         if (wNotify && hwndCtl)
            break;

         /* Menu commands */

         switch (wId)
            {
            case miExit:
               DestroyWindow(hwnd);
               break;

            case miClearAll:
               for (i = 0; i < cFieldsMax; i++)
                  {
                  if (vrgfield[i].hwnd != NULL)
                     {
                     /* Clear field */
                     if (vrgfield[i].wFieldType==FIELDIEDIT)
                        FSetHpndt(vrgfield[i].hwnd, NULL);
                     else
                        SendMessage(vrgfield[i].hwnd, WM_SETTEXT,
                           0, (LONG)(LPSTR)"");
                     }
                  }
               SetFocus(vrgfield[NAME].hwnd);
               break;

            case miSampleDlg:
               SampleDialog(hwnd);
               break;

            case miLoad:
            case miSave:
               FLoadSave(wId == miLoad);
               break;

            case miNextField:
               /* Focus on the next field */
               ProcessFieldChange((UINT)chNextField);
               break;

            case miPrecField:
               /* Set Focus on the preceeding field */
               ProcessFieldChange((UINT)chPrecField);
               break;

            default:
               break;
            }
         break;
         }

#ifdef WIN32
      case WM_CTLCOLOREDIT:
         {
         #define cbClass   8
         char szClass[cbClass];

         /* use grey background for single line edit controls if penwin installed */
         if (vhinstPW && hbrBackgnd &&
             !(GetWindowLong((HWND)LOWORD(lParam), GWL_STYLE) & ES_MULTILINE) &&
             GetClassName((HWND)LOWORD(lParam), szClass, cbClass) &&
             (lstrcmpi(szClass, "BEDIT") != 0))
            {
            SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNFACE));
            return (LRESULT)hbrBackgnd;
            }

         lRet = DefWindowProc(hwnd, message, wParam, lParam);
         break;
         }
#else
      case WM_CTLCOLOR:
         /* use grey background for single line edit controls if penwin installed */
         if (vhinstPW && hbrBackgnd)
            {
            #define cbClass   8
            char szClass[cbClass];

            switch (HIWORD(lParam))
               {
            case CTLCOLOR_EDIT:
               /* if multiline or bedit, break */
               if (GetWindowLong((HWND)LOWORD(lParam), GWL_STYLE) & ES_MULTILINE)
                  break;
               if (GetClassName((HWND)LOWORD(lParam), szClass, cbClass) &&
                   (lstrcmpi(szClass, "BEDIT") == 0))
                  break;
               SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNFACE));

               /* fall through */
            case CTLCOLOR_MSGBOX:
               return (LRESULT)hbrBackgnd;
               }
            }

         lRet = DefWindowProc(hwnd, message, wParam, lParam);
         break;
#endif // !WIN32

      case WM_PAINT:
         {
         PAINTSTRUCT ps;
         HDC         hdc;

         hdc = BeginPaint(hwnd, &ps);
         SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
         SetBkMode(hdc, TRANSPARENT);

         for (i = 0; i < cFieldsMax; i++)
            {
            PFIELD pfield = vrgfield + i;

            TextOut(hdc, pfield->xStatic, pfield->yStatic, pfield->szTitle,
               lstrlen(pfield->szTitle));
            }

         EndPaint(hwnd, &ps);
         break;
         }
      break;

      case WM_SETFOCUS:
         SetFocus(vhwndFocus);
         break;

      case WM_DESTROY:
         if (hbrBackgnd)
            DeleteObject(hbrBackgnd);
         PostQuitMessage(0);
         break;

      case WM_PENMISC:
         if (LOWORD(wParam) == PMSC_LOADPW && lParam == PMSCL_UNLOADING)
            {
            FTermApp();
            PostQuitMessage(0);
            }
         break;

      default:
         lRet = DefWindowProc(hwnd, message, wParam, lParam);
         break;
      }

   return lRet;
   }


BOOL NEAR PASCAL FCreateForm(HWND hwndParent)
/***************************************************************

FUNCTION:   FCreateForm(hwndParent)

PURPOSE:

   This function creates the fields.

   Returns TRUE is successful, FALSE if field hedit window could
   not be created.

***************************************************************/
   {
   int      i;
   HRC      hrc;
   LONG     lT    = GetDialogBaseUnits();
   int      cx    = LOWORD(lT)/2;
   int      cy    = HIWORD(lT)/2;
   char     *szClass[3];

   szClass[FIELDHEDIT] = vhinstPW? "hedit": "edit";
   szClass[FIELDBEDIT] = vhinstPW? "bedit": "edit";
   szClass[FIELDIEDIT] = vhinstPW? "iedit": szIeditROClass;

   for (i = 0; i < cFieldsMax; i++)
      {
      PFIELD pfield = vrgfield + i;
      DWORD dwStyle = WS_VISIBLE | WS_CHILD | pfield->dwStyle;

      if (!vhinstPW)
         dwStyle |= WS_BORDER;   // no underline available; subst border

      pfield->hwnd = CreateWindowEx(pfield->dwExStyle,
         (LPCSTR)szClass[pfield->wFieldType],
         (LPCSTR)NULL,
         dwStyle,
         pfield->xEdit,
         pfield->yEdit,
         pfield->cxEdit,
         pfield->cyEdit,
         hwndParent,
         (HMENU)(i+1), // unique non-zero child ID
         vhInstanceApp,
         NULL);

      if (!pfield->hwnd)
         {
         continue;
         }

      /* Set alphabet preferences for this edit control */
      if (vhinstPW)
         {
         if (hrc = (HRC)SendMessage(pfield->hwnd, WM_PENMISC, PMSC_GETHRC, 0L))
            {
            FnPW(SetAlphabetHRC)(hrc, pfield->alc, NULL);
            FnPW(SetWordlistCoercionHRC)(hrc, SCH_ADVISE);
            if (!pfield->fEnableGestures)    // default: all enabled
               FnPW(EnableGestureSetHRC)(hrc, GST_ALL, FALSE);
            SendMessage(pfield->hwnd, WM_PENMISC, PMSC_SETHRC, (LPARAM)hrc);
            FnPW(DestroyHRC)(hrc);
            }

         /* If no border, put underline in unless just kernel */
         if (!(pfield->dwStyle & ES_MULTILINE)
            && pfield->wFieldType == FIELDHEDIT)
            {
            SendMessage(pfield->hwnd, WM_PENCTL, HE_SETUNDERLINE, 1L);
            }

         /* specify approp. type of lens depending upon ALC */
         if (pfield->wFieldType == FIELDHEDIT)
            {
            if (pfield->alc == ALC_NUMERIC)
               // always use numeric keyboard
               SendMessage(pfield->hwnd, WM_PENCTL, HE_SETLENSTYPE,
                  MAKELONG(CWR_KEYBOARD, CWRK_NUMPAD));
            // use default lens type for other fields
            }
         }

      if (pfield->wLimit > 0)
         {
         SendMessage(pfield->hwnd, EM_LIMITTEXT, pfield->wLimit, 0L);
         }
      }

   return TRUE;
   }


VOID NEAR PASCAL SampleDialog(HWND hwnd)
/***************************************************************

FUNCTION:   SampleDialog()

PURPOSE:

   Brings up a sample dialog containing an EDIT (not HEDIT) control.
   If we're running under Pen Windows system, EDIT will act like an HEDIT.

***************************************************************/
   {
   DLGPROC lpSampleDlg;

   lpSampleDlg = MakeProcInstance((DLGPROC) SampleDlgProc, vhInstanceApp);
   DialogBox(vhInstanceApp, "SampleH", hwnd, lpSampleDlg);
   FreeProcInstance(lpSampleDlg);
   }


BOOL WINAPI SampleDlgProc(HWND hdlg, UINT message, UINT wParam, LONG lParam)
/***************************************************************

FUNCTION:   SampleDlgProc(hdlg, message, wParam, lParam)

PURPOSE:

   Dialog window procedure. Does nothing much, just shows a dialog.

***************************************************************/
   {
   Unused(lParam);

   switch (message)
      {
      case WM_INITDIALOG:
         SetDlgItemText(hdlg, IDC_EDIT, "Sample Name");

         /* Set focus to edit field and select all text */
         SetFocus(GetDlgItem(hdlg, IDC_EDIT));
         SendDlgItemMessage(hdlg, IDC_EDIT, EM_SETSEL, 0, MAKELONG(0,-1));
         break;

      case WM_COMMAND:
         switch (LOWORD(wParam))
            {
            case IDOK:
            case IDCANCEL:
               EndDialog(hdlg, LOWORD(wParam) == IDOK);
               return TRUE;

            default:
               break;
            }
         break;

      default:
         break;
      }

   return FALSE;
   }


BOOL NEAR PASCAL ProcessFieldChange (UINT wParam)
/***************************************************************

FUNCTION:   ProcessFieldChange (wParam)

PURPOSE:

   Set the focus on either the next or previous field based on
   the value of wParam. wParam is can be set to chNextField or
   chPrecField.  The global window handle vhwndFocus is assigned
   the value of the newly focused field.

   Returns TRUE if successful, FALSE otherwise.

***************************************************************/
   {
   if ((wParam == chNextField) || (wParam == chPrecField))
      {
      int inc = wParam == chPrecField ? cFieldsMax-1 : 1;
      int i = (IFromHwnd(vhwndFocus)+inc) %(cFieldsMax);

      SetFocus(vhwndFocus = vrgfield[i].hwnd);
      return TRUE;
      }

   return FALSE;
   }



int NEAR PASCAL IFromHwnd(HWND hwnd)
/***************************************************************

FUNCTION:   IFromHwnd(hwnd)

PURPOSE:

   Returns the index into the vrgfield which corresponds to
   the entry containing parameter hwnd.

   Returns 0 if a match is not found.

***************************************************************/
   {
   register int   i;
   LPFIELD        lpfield;

   for (lpfield = vrgfield, i = 0; i < cFieldsMax; i++, lpfield++)
      if (lpfield->hwnd == hwnd)
         return i;

   return 0;    /* default on err */
   }


BOOL NEAR PASCAL FLoadSaveText(HFILE hfile, BOOL fLoad, HWND hwnd)
/***************************************************************

FUNCTION:   FLoadSaveText(hfile, fLoad, hwnd)

PURPOSE:

   Loads (fLoad TRUE) or saves (fLoad FALSE) the contents of
   a text field. The file format is 2 bytes for length followed
   by that many bytes of text (no null term)

   Returns TRUE if successful.

***************************************************************/
   {
   char  sz[cbSzTMax];
   int   cb;
   BOOL  fRet;

   if (fLoad)
      {
      if (fRet = _lread(hfile, &cb, sizeof(int)) != HFILE_ERROR
         && _lread(hfile, sz, cb) != HFILE_ERROR)
         {
         sz[cb] = '\0'; // terminate
         SetWindowText(hwnd, (LPSTR)sz);
         }
      }
   else  // save text to file
      {
      cb = GetWindowText(hwnd, (LPSTR)sz, sizeof(sz));
      fRet = _lwrite(hfile, (LPCSTR)&cb, sizeof(int)) != HFILE_ERROR
         && _lwrite(hfile, (LPCSTR)sz, cb) != HFILE_ERROR;
      }

   return fRet;
   }


BOOL NEAR PASCAL FLoadSave(BOOL fLoad)
/***************************************************************

FUNCTION:   FLoadSave(fLoad)

PURPOSE:

   Loads (fLoad TRUE) or saves (fLoad FALSE) the contents of
   the form, in file HFORM.DAT.

   Returns TRUE if successful.

***************************************************************/
   {
   HCURSOR  hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
   HFILE    hfile;
   OFSTRUCT of;
   int      i;
   BOOL     fRet = TRUE;

   if ((hfile = OpenFile((LPSTR)szFile, &of,
      fLoad? OF_READ: OF_CREATE)) != HFILE_ERROR)
      {
      // first do the text fields:
      for (i = 0; fRet && i < cFieldsMax-1; i++)
         fRet = FLoadSaveText(hfile, fLoad, vrgfield[i].hwnd);

      // now do the IEDIT (assumed to be last):
      if (fRet)
         {
         HPENDATA hpndt;

         if (fLoad)
            {
            fRet = ((hpndt = ReadPenData(hfile)) != NULL)
               && FSetHpndt(vrgfield[MAP].hwnd, hpndt);
            }
         else  // save
            {
            if (FGetHpndt(vrgfield[MAP].hwnd, &hpndt) // retrieve
               && hpndt)   // may be empty though
               {
               fRet = WritePenData(hfile, hpndt);
               DestroyPenData(hpndt);  // we took it so we drop it too
               }
            else
               fRet = FALSE;
            }
         }
      _lclose(hfile);
      }
   else
      fRet = FALSE;

   SetCursor(hCursor);
   return fRet;
   }


HPENDATA NEAR PASCAL ReadPenData(HFILE hfile)
/***************************************************************

FUNCTION:   ReadPenData(hfile)

PURPOSE:

   Reads pen data from a file. The file format at this point is
   a UINT value representing the size of the pendata, followed
   by that many bytes of pendata.

   Returns a handle to a pendata if successful.

   Before calling this function, the caller should have already
   opened the file specified by hfile and ensured that the
   file pointer is offset to the beginning of the pen data.
   When the function returns, the file pointer will be offset
   to the end of the pen data in the file.

***************************************************************/
   {
   HPENDATA hpndt = NULL;
   UINT     cb, cbRead, cbHpndt;
   BYTE     lpbBuf[cbBufMax];    // buffer
   DWORD    dwState = 0L;        // required init
   BOOL     fError = FALSE;

   if (!hfile
      || (cb = _lread(hfile, &cbHpndt, sizeof(UINT))) == HFILE_ERROR
      || cb != sizeof(UINT))
         return NULL;

   while (cbHpndt > 0)
      {
      if ((cbRead = _lread(hfile, lpbBuf, min(cbHpndt, cbBufMax)))
         == HFILE_ERROR
         || PenDataFromBuffer(&hpndt, 0, lpbBuf, cbBufMax, &dwState) < 0)
         {
         if (hpndt)
            DestroyPenData(hpndt);
         return NULL;
         }

      if (cbRead > 0)
         cbHpndt -= cbRead;
      else
         break;
      }

   return hpndt;
   }


BOOL NEAR PASCAL WritePenData(HFILE hfile, HPENDATA hpndt)
/***************************************************************

FUNCTION:   WritePenData(hfile, hpndt)

PURPOSE:

   Writes pen data into a file, preceded by a UINT consisting of
   the size of the pen data in bytes.

   Returns TRUE if successful.

   Before calling this function, the caller should have
   already opened the file specified by hfile and ensured that
   the file pointer is correctly placed.  When the function
   returns, the file pointer will be offset to the end of the
   pen data in the file. The function fails if the pen data is
   larger than 64K.

***************************************************************/
   {
   BYTE lpbBuf[cbBufMax];
   DWORD dwState = 0L;  // required init
   LONG cb;
   DWORD dwSize;
   UINT cbSize;

   if (!hfile || !hpndt)
      return FALSE;

   if (GetPenDataAttributes(hpndt, (LPVOID)&dwSize, GPA_SIZE) < 0)
      return FALSE;

   cbSize = LOWORD(dwSize);

   if (_lwrite(hfile, (LPCSTR)&cbSize, sizeof(UINT)) == HFILE_ERROR)
      return FALSE;

   while (cb = (LONG)PenDataToBuffer(hpndt, lpbBuf, cbBufMax, &dwState))
      if (_lwrite(hfile, lpbBuf, (UINT)cb) == HFILE_ERROR)
         return FALSE;

   return cb >= 0L;
   }



BOOL NEAR PASCAL FInitPWStubs(HINSTANCE hinst, BOOL fInit)
/***************************************************************

FUNCTION:   FInitPWStubs(hinst, fInit)

PURPOSE:

   Initializes a table of functions required in penwin.dll.

   Returns TRUE if successful.

***************************************************************/
   {
   BOOL fRet = TRUE;
   static HGLOBAL hFnTable = NULL;

   if (fInit)  // alloc table on heap and fill it
      {
      if (hFnTable = GlobalAlloc(GHND | GMEM_DDESHARE,
         cFnNames * sizeof(FARPROC)))
         {
         int i;

         if (!((LPVOID)vrgFn = GlobalLock(hFnTable)))
            {
            fRet = FALSE;
            goto endFIPFT1;
            }

#ifdef WIN32
         // In 32 bits, you need to load the library before trying to
         // call functions from it. In 16 bits, it is OK to simply
         // get the module handle and call functions from it.
         if (LoadLibrary(szPenWinDll) != hinst) // make sure we get the same handle
            {
            hinst = NULL;
            fRet = FALSE;
            goto endFIPFT2;
            }
#endif // WIN32

         for (i = 0; i < cFnNames; i++)
            {
            if (!(vrgFn[i] = GetProcAddress(hinst, (LPCSTR)vrgszFn[i])))
               {
               fRet = FALSE;
               goto endFIPFT2;
               }
            }
         }
      }

   else if (hFnTable)   // free table
      {
endFIPFT2:

#ifdef WIN32
      if (hinst) // In 32 bits, we had to load the library. Now free it.
         FreeLibrary(hinst);
#endif // WIN32

      fRet = !GlobalUnlock(hFnTable) && fRet;
      vrgFn = NULL;
endFIPFT1:
      fRet = !GlobalFree(hFnTable) && fRet;
      hFnTable = NULL;
      }

   return fRet;
   }


BOOL NEAR PASCAL FGetHpndt(HWND hwnd, HPENDATA NEAR *phpndt)
/***************************************************************

FUNCTION:   FGetHpndt(hwnd, phpndt)

PURPOSE:

   Gets a copy of a pendata from a window into parameter.

   Returns TRUE if successful.

***************************************************************/
   {
   if (vhinstPW)
      {
      HPENDATA hpndt = (HPENDATA)SendMessage(hwnd, IE_GETINK, IEGI_ALL, 0);

      if ((int)hpndt < 0)
         return FALSE;
      *phpndt = hpndt;
      return TRUE;
      }

   // else it's our own readonly "control":
   *phpndt = DuplicatePenData((HPENDATA)GetWindowWord(hwnd, 0), 0);
   return TRUE;
   }


BOOL NEAR PASCAL FSetHpndt(HWND hwnd, HPENDATA hpndt)
/***************************************************************

FUNCTION:   FSetHpndt(hwnd, hpndt)

PURPOSE:

   Sets a pendata into a window and destoys any pendata that may
   already be in there.

   Returns TRUE if successful.

***************************************************************/
   {
   BOOL fRet = TRUE;

   if (vhinstPW)
      {
      fRet = SendMessage(hwnd, IE_SETINK, IESI_REPLACE, (LPARAM)hpndt) == IER_OK;
      }
   else
      {
      if (hpndt = (HPENDATA)SetWindowLong(hwnd, 0, (LONG)hpndt))  // old pendata?
         DestroyPenData(hpndt);                             // destroy it
      }
   InvalidateRect(hwnd, NULL, TRUE);

   return fRet;
   }
