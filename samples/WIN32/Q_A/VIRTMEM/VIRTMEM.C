
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*************************************************************************\
*  PROGRAM: Virtmem.c
*
*  PURPOSE:
*
*     To demonstrate the use of various virtual memory API.
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - Process messages, launches server & client threads.
*    ResetMenu()    - Looks at the memory page and checks the menu items
*                     accordingly.
*    ShowDlgProc()  - Shows information regarding the virtual page.
*
*  GLOBAL VARIABLES:
*
*    - Buf, Buf2:
*             Character arrays used for error messages.
*
*    - base:  Pointer to CHAR, used as a pointer to the base of the page.
*    - hInst: Handle to the application's Instance.
*    - hWnd:  Handle to the parent window.
*    - MemInfo:
*             Structure used to hold the page's memory information.
*
*  COMMENTS:
*
*    To Use:
*      Start the application.  You are automatically given a Reserved page
*      of memory with no access (4096 bytes in size).  You can then use
*      the various menu selection to change the state and protection on
*      the page.  "State" allows you to change the page between Free,
*      Reserved, and Committed.  "Access" allows you to change the
*      protection on the page between Read/Write, Read Only, or No Access.
*      Lock allows you to Lock or Unlock the page in memory (note that
*      this was not yet implemented in PDK-2).  Test trys to write to
*      memory.  If the page is not committed, and is marked with read/
*      write access, this will cause an exception.  There is an example
*      of exception handling in the code.
*
*      The menu will keep track of the state and protection access
*      to the page, and the menu items will be checked accordingly.
*      For more information on the page, you can select "Show Page"
*      from the menu.  A dialog box will appear showing you the page
*      information.  You can dismiss the dialog box through its system
*      menu.
*
\*************************************************************************/


#include <windows.h>
#include <excpt.h>
#include <stdio.h>
#include <stdlib.h>
#include "virtmem.h"

#define WERR(who,where)    {sprintf(Buf,"ERROR: %s returned %u, line: %u", who, GetLastError(), __LINE__);\
                            sprintf(Buf2,"From within %s", where);\
                            MessageBox(hwnd, Buf, Buf2, MB_OK);}


#define IMPLEMENTED   // Used if VirtualLock and VirtualUnlock are implemented.

CHAR Buf[80];
CHAR Buf2[80];

CHAR *base;
CHAR *page;

HANDLE hInst;
HANDLE hWnd;
MEMORY_BASIC_INFORMATION MemInfo;

/*************************************************************************\
*
*  FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
*
*  PURPOSE: calls initialization function, processes message loop
*
*  COMMENTS:
*
\*************************************************************************/

int PASCAL WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR  lpCmdLine,
                    int    nCmdShow)
{
  MSG  msg;
  WNDCLASS wc;

  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  hInst = hInstance;

  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "Virtmem_Menu";
  wc.lpszClassName = "VirtmemWClass";

  RegisterClass(&wc);


  hWnd = CreateWindow ("VirtmemWClass",
                       "Virtual Memory Sample",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);


  ShowWindow(hWnd, nCmdShow);
  while (GetMessage (&msg, NULL, 0, 0))
    DispatchMessage (&msg);      // Dispatch message to window.

  return (msg.wParam);           // Returns value from PostQuitMessage.

}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   To process the windows messages.
*
*  VARIABLES USED:
*
*    - Buf[]: Array of CHAR, error message buffer.
*    - retCode:
*             DWORD used to trap return codes.
*    - oldProtection:
*             DWORD used to hold the old protection value returned from
*             VirtualProtect() call.
*    - page:  Array of CHAR, used as a base pointer to the page.
*    - ExceptError[]:
*             Array of CHAR, used to be printed as an error message string.
*    - ExceptSuccess[]:
*             Array of CHAR, used to be printed an a success message string.
*
*  MESSAGES:
*
*    WM_DESTROY:     - Terminates the threads and post the quit message.
*    WM_CREATE:      - Allocates memory to hold some color values, and
*                      creates the two threads.
*    WM_USER:        - Used to delay the message to reset the menu until
*                      the window is created.
*
*    WM_COMMAND
*
*      IDM_LOCK:
*               Uses VirtualLock() to lock the page into memory (preventing
*               it from being paged out.
*
*      IDM_UNLOCK:
*               Unlocks the page from memory allowing it to be paged out
*               by the system.
*
*      IDM_WRITE:
*               Attempts to write to the page.  This message uses structured
*               exception handling and tries to write to offset 100 in the
*               page.  If there is an exception (usually due to the memory
*               not being committed, or not having the proper read/write
*               access), then the exception is handled by putting up a message
*               box.  If no exception is trapped, a success message is
*               put up.
*
*      IDM_SHOW:
*               Puts up a dialog box which gives information about the current
*               state of the page (See comments on ShowDlgProc);
*
*      IDM_NOACCESS:
*               Uses VirtualProtect() to change the protection on the page
*               to PAGE_NOACCESS.  Note a page must be committed before
*               changing its protection.
*
*      IDM_READONLY:
*               Uses VirtualProtect() to change the protection on the page to
*               PAGE_READONLY.  Note a page must be committed before changing
*               its protection.
*
*      IDM_READWRITE:
*               Uses VirtualProtect() to change the protection on the page to
*               PAGE_READWRITE.  Note a page must be committed before
*               changing its protection.
*
*      IDM_COMMIT:
*               Uses VirtualAlloc() to commit the page.  Note that if the
*               page has been previously RESERVED with VirtualAlloc, then
*               you can use a pointer to the base of the memory as the first
*               parameter of the call.  If the page is FREE rather than
*               RESERVED, then you use NULL as the first parameter.
*
*      IDM_FREE:
*               Uses VirtualFree() to commit the page.  Note that a committed
*               page must first be decommitted before it can be freed.
*
*      IDM_RESERVE:
*               If the page is FREE, this uses VirtualAlloc() reserve it.
*               If the page is committed, this uses VirtualFree() to
*               decommit the page.
*
*  CALLED BY:
*
*    WinMain();
*
*  CALLS TO:
*
*    ResetMenu();
*    ShowDlgProc();
*
*  COMMENTS:
*
*
\*************************************************************************/


LRESULT CALLBACK MainWndProc (HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
{
  CHAR  Buf[80];
  DWORD retCode;
  DWORD oldProtection;
  CHAR  *page;
  CHAR  ExceptError[200] = "An exception was trapped and handled with this message box.\
    Make sure the page is commited and that you have Read/Write access.";
  CHAR ExceptSuccess[] = "Writing to memory was successful: base[100] = 'a'";

  switch (message)
      {

        case WM_CREATE:

            base = VirtualAlloc (NULL,
                                 PAGESIZE,
                                 MEM_RESERVE,
                                 PAGE_NOACCESS);

            if (!base)
              WERR ("VirtualAlloc", "WM_CREATE");

            PostMessage (hwnd, WM_USER, 0, 0);

            return (0);


        case WM_USER:
            ResetMenu();
            return (0);

        case WM_COMMAND:
          switch (LOWORD(wParam))
            {

#ifdef IMPLEMENTED

            case IDM_LOCK:

              VirtualQuery (base, &MemInfo, sizeof(MemInfo));
              if (MemInfo.State != MEM_COMMIT)
                {
                MessageBox (hWnd,
                           "Stop!  You must first COMMIT a page before locking it.",
                            "From IDM_LOCK", MB_OK);
                return (0);
                }

              retCode = VirtualLock(base, PAGESIZE);
              if (!retCode)

                  MessageBox(hWnd, "Error in locking memory",
                             "From within IDM_LOCK", MB_OK);
                else
                  MessageBox(hWnd, "The Memory was Locked.",
                             "From within IDM_LOCK", MB_OK);

              return (0);

            case IDM_UNLOCK:

              retCode = VirtualUnlock(base, PAGESIZE);
              if (!retCode)

                  MessageBox(hWnd, "Error in unlocking memory",
                             "From within IDM_UNLOCK", MB_OK);
                else
                  MessageBox(hWnd, "The Memory was Unlocked.",
                             "From within IDM_UNLOCK", MB_OK);

              return (0);

#endif

            case IDM_WRITE:
              __try
                {
                 base[100] = 'a';
                 MessageBox (hWnd, ExceptSuccess, "try/except", MB_OK);
                }
              __except (EXCEPTION_EXECUTE_HANDLER)
                {
                 MessageBox (hWnd, ExceptError, "try/except", MB_OK);
                }
              return (0);


            case IDM_SHOW:
              DialogBox (hInst, "Page_Info", hwnd, (DLGPROC)ShowDlgProc);
              return (0);


            case IDM_NOACCESS:

              retCode = VirtualQuery (base, &MemInfo,
                                      sizeof(MEMORY_BASIC_INFORMATION));
              if (!retCode)
                {
                 WERR("VirtualQuery", "IDM_NOACCESS");
                 return (0);
                }

              if (MemInfo.State != MEM_COMMIT)
                {
                 MessageBox ( hWnd, "The page must be commited first.",
                             "From IDM_NOACCESS", MB_OK);
                 return (0);
                }

              retCode = VirtualProtect (base, PAGESIZE,
                                        PAGE_NOACCESS, &oldProtection);
              if (!retCode)
                 WERR("VirtualProtect","IDM_NOACCESS");

              ResetMenu();
              return (0);


            case IDM_READONLY:
              retCode = VirtualQuery (base, &MemInfo,
                                      sizeof(MEMORY_BASIC_INFORMATION));
              if (!retCode)
                {
                 WERR("VirtualQuery", "IDM_READONLY");
                 return (0);
                }

              if (MemInfo.State != MEM_COMMIT)
                {
                 MessageBox ( hWnd, "The page must be commited first.",
                             "From IDM_READONLY", MB_OK);
                 return (0);
                }

              retCode = VirtualProtect (base, PAGESIZE,
                                        PAGE_READONLY, &oldProtection);
              if (!retCode)
                WERR("VirtualProtect", "IDM_READONLY");

              ResetMenu();
              return (0);


            case IDM_READWRITE:
              retCode = VirtualQuery (base, &MemInfo,
                                      sizeof(MEMORY_BASIC_INFORMATION));
              if (!retCode)
                {
                 WERR("VirtualQuery", "IDM_READWRITE");
                 return (0);
                }

              if (MemInfo.State != MEM_COMMIT)
                {
                 MessageBox ( hWnd, "The page must be commited first.",
                             "From IDM_READWRITE", MB_OK);
                 return (0);
                }

              retCode = VirtualProtect (base, PAGESIZE,
                                        PAGE_READWRITE, &oldProtection);
              if (!retCode)
                 WERR("VirtualProtect", "IDM_READWRITE");

              ResetMenu();
              return (0);


            case IDM_COMMIT:

              VirtualQuery ((LPVOID)base,
                            &MemInfo,
                            sizeof(MEMORY_BASIC_INFORMATION));

              switch (MemInfo.State)
                {
                case MEM_COMMIT:
                  return (0);

                case MEM_RESERVE:
                  page = VirtualAlloc (base,PAGESIZE,MEM_COMMIT,PAGE_READWRITE);

                  if (page)
                    ResetMenu();
                  else
                    WERR("VirtualAlloc", "IDM_COMMIT");
                  break;

                case MEM_FREE:
                  base = VirtualAlloc(NULL,PAGESIZE,MEM_COMMIT,PAGE_READWRITE);
                  if (base)
                    ResetMenu();
                  else
                    WERR("VirtualAlloc", "IDM_COMMIT");
                  break;
                }

              ResetMenu();
              return (0);


            case IDM_FREE:

              VirtualQuery (base,
                            &MemInfo,
                            sizeof(MEMORY_BASIC_INFORMATION));

              if (MemInfo.State == MEM_COMMIT)
                {
                retCode = VirtualFree(base, PAGESIZE, MEM_DECOMMIT);
                if (!retCode)
                  WERR("VirtualFree","IDM_FREE");

                }

              VirtualQuery (base,
                            &MemInfo,
                            sizeof(MEMORY_BASIC_INFORMATION));

              if (MemInfo.State == MEM_RESERVE)
                {
                retCode = VirtualFree(base, 0, MEM_RELEASE);
                if (!retCode)
                  WERR("VirtualFree", "IDM_FREE");
                }

              ResetMenu();
              return (0);



            case IDM_RESERVE:

              VirtualQuery ((LPVOID)base,
                            &MemInfo,
                            sizeof(MEMORY_BASIC_INFORMATION));

              switch (MemInfo.State)
                {
                 case MEM_RESERVE:
                   return (0);

                 case MEM_FREE:
                   base = VirtualAlloc(NULL, PAGESIZE, MEM_RESERVE, PAGE_NOACCESS);
                   if (!base)
                     WERR("VirtualAlloc","IDM_RESERVE");

                   ResetMenu();
                   return (0);

                 case MEM_COMMIT:
                   retCode = VirtualFree(base, PAGESIZE, MEM_DECOMMIT);

                   if (!retCode)
                     WERR("VirtualFree","IDM_RESERVE");

                   ResetMenu();
                   return (0);

                 default:
                   MessageBox(hWnd, "Unknown MemInfo.State", "From IDM_RESERVE", MB_OK);

                }
              return (0);

            default:
              return (0);
            }

        case WM_DESTROY:
          PostQuitMessage(0);
          return (0);


       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}


/*************************************************************************\
*
*  FUNCTION:  VOID ResetMenu (VOID)
*
*  PURPOSE:   Updates the menu with check marks to indicate the pages state
*             and protection.
*
*  VARIABLES USED:
*
*    - i:     Local integer used for counting in a for loop.
*    - hMenu: Local menu handle to the menu.
*
*  CALLED BY:
*
*    MainWndProc();
*
*  COMMENTS:
*    This function uses a for loop to uncheck all the menu items.  It then
*    calls VirtualQuery() to  determine information about the page, and
*    updates the menu items with check marks as appropriate.
*
\*************************************************************************/

VOID ResetMenu (VOID)
{

  HMENU hMenu;
  INT   i;

  hMenu = GetMenu(hWnd);

  for (i = IDM_FREE; i <= IDM_READWRITE; i++)
    CheckMenuItem(hMenu, i, MF_BYCOMMAND | MF_UNCHECKED);


  VirtualQuery (base,
                &MemInfo,
                sizeof(MEMORY_BASIC_INFORMATION));

  switch (MemInfo.State)
    {
    case MEM_COMMIT:
       CheckMenuItem(hMenu,
                     IDM_COMMIT,
                     MF_BYCOMMAND | MF_CHECKED);
       break;

    case MEM_FREE:
       CheckMenuItem(hMenu,
                     IDM_FREE,
                     MF_BYCOMMAND | MF_CHECKED);
       break;

    case MEM_RESERVE:
       CheckMenuItem(hMenu,
                     IDM_RESERVE,
                     MF_BYCOMMAND | MF_CHECKED);
       break;
    }


  switch (MemInfo.Protect)
    {
    case 0:
      break;

    case PAGE_READWRITE:
      CheckMenuItem(hMenu,
                    IDM_READWRITE,
                    MF_BYCOMMAND | MF_CHECKED);

      break;

    case PAGE_READONLY:
      CheckMenuItem(hMenu,
                    IDM_READONLY,
                    MF_BYCOMMAND | MF_CHECKED);
      break;

    case PAGE_NOACCESS:
      CheckMenuItem(hMenu,
                    IDM_NOACCESS,
                    MF_BYCOMMAND | MF_CHECKED);
      break;
    }

  CloseHandle (hMenu);


}


/*************************************************************************\
*
*  FUNCTION:  BOOL CALLBACK ShowDlgProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   This is a dialog box function which queries page information
*             using VirtualQuery, and then displays the information in
*             the appropriate edit fields.
*
*  MESSAGES:
*
*    WM_SYSCOMMAND:
*             If this message is trapped, the dialog box is terminated.
*
*    WM_INITDIALOG:
*             Does a VirtualQuery, checks the values returned in the
*             fields of the MEMORY_BASIC_INFORMATION structure, and
*             fills the edit fields of the dialog box with the appropriate
*             information.
*
*  CALLED BY:
*
*    MainWndProc();
*
\*************************************************************************/

BOOL CALLBACK ShowDlgProc (HWND hdlg,
                           UINT message,
                           WPARAM wParam,
                           LPARAM lParam)
{
  UNREFERENCED_PARAMETER( lParam );

  switch (message)
    {
    case WM_SYSCOMMAND:
      if (wParam == SC_CLOSE)
        {
         EndDialog(hdlg, TRUE);
         return (TRUE);
        }
      return (0);

    case WM_INITDIALOG:

      VirtualQuery ((LPVOID)base,
                    &MemInfo,
                    sizeof(MEMORY_BASIC_INFORMATION));

      SetDlgItemInt (hdlg, IDE_BASEADDR,  (int)MemInfo.BaseAddress,    FALSE);
      SetDlgItemInt (hdlg, IDE_ALLOCBASE, (int)MemInfo.AllocationBase, FALSE);

      switch (MemInfo.AllocationProtect)
        {
        case PAGE_NOACCESS:
            SetDlgItemText (hdlg, IDE_INITPROT, "No Access");
            break;

        case PAGE_READONLY:
            SetDlgItemText (hdlg, IDE_INITPROT, "Read Only");
            break;

        case PAGE_READWRITE:
            SetDlgItemText (hdlg, IDE_INITPROT, "Read/Write");
            break;

        default:
            SetDlgItemText (hdlg, IDE_INITPROT, "Unknown");

        }

      SetDlgItemInt (hdlg, IDE_SIZE, MemInfo.RegionSize, FALSE);


      switch (MemInfo.State)
        {
        case MEM_FREE:
            SetDlgItemText (hdlg, IDE_STATE, "Free");
            break;

        case MEM_RESERVE:
            SetDlgItemText (hdlg, IDE_STATE, "Reserved");
            break;

        case MEM_COMMIT:
            SetDlgItemText (hdlg, IDE_STATE, "Commited");
            break;

        default:
            SetDlgItemText (hdlg, IDE_STATE, "Unknown");

        }


      switch (MemInfo.Protect)
        {
        case 0:
            SetDlgItemText (hdlg, IDE_PROTECT, "Undefined");
            break;

        case PAGE_NOACCESS:
            SetDlgItemText (hdlg, IDE_PROTECT, "No Access");
            break;

        case PAGE_READONLY:
            SetDlgItemText (hdlg, IDE_PROTECT, "Read Only");
            break;

        case PAGE_READWRITE:
            SetDlgItemText (hdlg, IDE_PROTECT, "Read/Write");
            break;

        default:
            SetDlgItemText (hdlg, IDE_PROTECT, "Unknown");

        }


      return (TRUE);

    default:
      return (0);

    }

  return (0);

}
