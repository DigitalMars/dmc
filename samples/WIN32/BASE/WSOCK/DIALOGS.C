
//-----------------------------------------------------------------------------
// This is a part of the Microsoft Source Code Samples. 
// Copyright (C) 1993 Microsoft Corporation.
// All rights reserved. 
//  
// This source code is only intended as a supplement to 
// Microsoft Development Tools and/or WinHelp documentation.
// See these sources for detailed information regarding the 
// Microsoft samples programs.
//-----------------------------------------------------------------------------

/****************************************************************************\
*  dialogs.c -- sample program demonstrating Windows Sockets APIs.
*
*  Handles the dialog boxes for the WSOCK sample.
*
****************************************************************************/

#include <stdio.h>
#include <stdlib.h> /* atoi */
#include <windows.h>
#include <winsock.h>
#include "wsock.h"

extern u_short portno; /* Which tcp port are we going to use? */
extern char szBuff[80];

/****************************************************************************
*
*    FUNCTION: GetTcpPort(HWND, UINT, UINT, LONG)
*
*    PURPOSE: dialog callback procedure.  Allows the user to enter the
*             tcp port number, or a service name.
*
*\***************************************************************************/

LRESULT APIENTRY GetTcpPort(
        HWND hDlg,                /* window handle of the dialog box */
        UINT message,             /* type of message                 */
        UINT wParam,              /* message-specific information    */
        LONG lParam)
{

   switch (message) {
      case WM_INITDIALOG:                     /* message: initialize dialog box */
        SetFocus( GetDlgItem( hDlg, IDD_EDIT));
        return (TRUE);

      case WM_COMMAND: {                    /* message: received a command */
        switch(LOWORD(wParam)) {
            case IDOK:                      /* "OK" box selected?          */
            {
               GetDlgItemText( hDlg, IDD_EDIT, szBuff, 80);
               if ((portno = atoi(szBuff)) == 0)
                  EndDialog( hDlg, 2 );
               else
                  EndDialog( hDlg, 1 );
            }
            break;

         case IDCANCEL: /* System menu close command? */
            EndDialog(hDlg, 0);           /* Exits the dialog box        */
            break;

         default:
           return FALSE;
         }
                return (TRUE);
      }
    }   /* switch message */

    return (FALSE);                           /* Didn't process a message    */
        UNREFERENCED_PARAMETER(lParam);
}

/****************************************************************************
*
*    FUNCTION: GetHostName(HWND, UINT, UINT, LONG)
*
*    PURPOSE: dialog callback procedure.  Allows the user to enter the
*             the host name.  Used for menu item "Check Host Name" and
*             also for menu item "Connect".
*
*\***************************************************************************/

LRESULT APIENTRY GetHostName(
        HWND hDlg,                /* window handle of the dialog box */
        UINT message,             /* type of message                 */
        UINT wParam,              /* message-specific information    */
        LONG lParam)
{

    switch (message) {
       case WM_INITDIALOG:                     /* message: initialize dialog box */
         SetFocus( GetDlgItem( hDlg, IDD_EDIT));
         return (TRUE);
       case WM_COMMAND: {                     /* message: received a command */
          switch(LOWORD(wParam)) {
            case IDOK:          /* "OK" box selected?        */
             {
              GetDlgItemText( hDlg, IDD_EDIT, szBuff, 80);
              EndDialog(hDlg, TRUE);          /* Exits the dialog box        */
             }
             break;
             case IDCANCEL:     /* System menu close command? */
              EndDialog(hDlg, FALSE);         /* Exits the dialog box        */
              break;
             default:
              return FALSE;
           }
        return (TRUE);
      }
    }   /* switch message */

    return (FALSE);                           /* Didn't process a message    */
        UNREFERENCED_PARAMETER(lParam);
}
/****************************************************************************
*
*    FUNCTION: About(HWND, UINT, UINT, LONG)
*
*    PURPOSE: dialog callback procedure for "about" box.
*
*\***************************************************************************/

LRESULT APIENTRY About(
        HWND hDlg,                /* window handle of the dialog box */
        UINT message,             /* type of message                 */
        UINT wParam,              /* message-specific information    */
        LONG lParam)
{
     switch (message) {
        case WM_COMMAND:                         /* message: received a command */
          if (LOWORD(wParam) == IDOK             /* "OK" box selected?          */
                || LOWORD(wParam) == IDCANCEL) { /* System menu close command?  */
                        EndDialog(hDlg, TRUE);   /* Exits the dialog box        */
          return (TRUE);        /* WM_COMMAND */
     }
     break;
   } /* End switch message */
   return (FALSE);                            /* Didn't process a message    */
        UNREFERENCED_PARAMETER(lParam);
}

/****************************************************************************
*
*    FUNCTION: DisplayHostEnt(HWND, UINT, UINT, LONG)
*
*    PURPOSE: dialog callback procedure.  Displays the information
*             returned by gethostbyname() (HOSTENT structure).
*
*\***************************************************************************/

LRESULT APIENTRY DisplayHostEnt(
        HWND hDlg,                /* window handle of the dialog box */
        UINT message,             /* type of message                 */
        UINT wParam,              /* message-specific information    */
        LONG lParam)
{

   DWORD ret;
   switch (message) {
        /*
        *   Initialize dialog box
        */
        case WM_INITDIALOG:
                {
                int count = 0;

                SetWindowText( GetDlgItem( hDlg, IDD_HOSTNAME),  (LPCTSTR)phe->h_name);

                while (phe->h_aliases[count] != NULL) {
                        SendDlgItemMessage(hDlg, IDD_LBALIAS, LB_ADDSTRING, 0, (LPARAM)(phe->h_aliases[count]));
                        count++;
                        }

                count = 0;

                /*
                *   Enumerate all the hosts IP addresses.
                */
                while (phe->h_addr_list[count] != NULL) {
                    sprintf( szBuff, "%u.%u.%u.%u",
                    (unsigned char) phe->h_addr_list[count][0],
                    (unsigned char) phe->h_addr_list[count][1],
                    (unsigned char) phe->h_addr_list[count][2],
                    (unsigned char) phe->h_addr_list[count][3]);

                    count++;

                    /*
                    *   Fill the dialog box..
                    */
                    if ((ret = SendDlgItemMessage(hDlg, IDD_LBADDR, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)szBuff)) == LB_ERR)
                       MessageBox(hDlg, szBuff, "Couldn't add address..", MB_OK);
                    }

                } /* while( more IP addresses ) */
           return (TRUE);       /* WM_INITDIALOG */

        /*
        *   Received a command message
        */
        case WM_COMMAND:
                if (LOWORD(wParam) == IDOK) {
                   EndDialog(hDlg, TRUE);             /* Exits the dialog box        */
                   return TRUE;
                }
                return FALSE;   /* WM_COMMAND */
    }   /* switch message */

    return (FALSE);                                   /* Didn't process a message    */
        UNREFERENCED_PARAMETER(lParam);
} /* DisplayHostEnt */

/****************************************************************************
*
*    FUNCTION: GetSendString(HWND, UINT, UINT, LONG)
*
*    PURPOSE: dialog callback procedure.  Allows the user to enter a
*             string to be sent to the connected remote host.
*
*\***************************************************************************/

LRESULT APIENTRY GetSendString(
        HWND hDlg,                /* window handle of the dialog box */
        UINT message,             /* type of message                 */
        UINT wParam,              /* message-specific information    */
        LONG lParam)
{

    switch (message) {
    case WM_INITDIALOG:                     /* message: initialize dialog box */
        SetFocus( GetDlgItem( hDlg, IDD_EDIT));
        return (TRUE);
    case WM_COMMAND: {                     /* message: received a command */
          switch(LOWORD(wParam)) {
            case IDOK:          /* "OK" box selected?        */
             {

              /*
              *   Store string in szBuff (global buffer)
              */
              GetDlgItemText( hDlg, IDD_EDIT, szBuff, 80);
              EndDialog(hDlg, TRUE);          /* Exits the dialog box        */
             }
             break;
             case IDCANCEL:     /* System menu close command? */
              EndDialog(hDlg, FALSE);         /* Exits the dialog box        */
              break;
             default:
              return FALSE;
           }
        return (TRUE);
      }
    }   /* switch message */

    return (FALSE);                           /* Didn't process a message    */
        UNREFERENCED_PARAMETER(lParam);
} /* GetString */
