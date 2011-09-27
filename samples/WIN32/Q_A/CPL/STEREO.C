/****************************************************************************
*
*
*    PROGRAM: Stereo.c
*
*    PURPOSE: Example of a Control Panel Application
*
*    FUNCTIONS:
*
*        DllMain()
*        InitStereoApplet()
*        TermStereoApplet()
*        CPIApplet()
*        AmpDlgProc()
*        TermDlgProc()
*        TunerDlgProc()
*
*    COMMENTS:
*
*    The following code sample shows the CPlApplet function for a DLL containing
*    three Control Panel applications that set preferences for a component stereo
*    system attached to the computer. The sample uses an application-defined
*    StereoApplets array that contains three structures, each corresponding to
*    one of the Control Panel applications. Each structure contains all the
*    information required by the CPL_NEWINQUIRE message, as well as the dialog
*    box template and dialog box procedure required by the CPL_DBLCLK message.
*    The code demonstrates how to fill the structures in the StereoApplets array.
*
****************************************************************************/

#include <windows.h>
#include <cpl.h>
#include "stereo.h"

typedef struct tagApplets
{
    int icon;           // icon resource identifier
    int namestring;     // name-string resource identifier
    int descstring;     // description-string resource identifier
    int dlgtemplate;    // dialog box template resource identifier
    DLGPROC dlgfn;      // dialog box procedure
} APPLETS;

APPLETS StereoApplets[] =
{
    AMP_ICON, AMP_NAME, AMP_DESC, AMP_DLG, AmpDlgProc,
    TUNER_ICON, TUNER_NAME, TUNER_DESC, TUNER_DLG, TunerDlgProc,
    TAPE_ICON, TAPE_NAME, TAPE_DESC, TAPE_DLG, TapeDlgProc,

};

#define NUM_APPLETS (sizeof(StereoApplets)/sizeof(StereoApplets[0]))


HANDLE hModule = NULL;

char szCtlPanel[30];

/****************************************************************************
*
*    FUNCTION: DllMain(PVOID, ULONG, PCONTEXT)
*
*    PURPOSE: Win 32 Initialization DLL
*
*    COMMENTS:
*
*
****************************************************************************/

BOOL WINAPI DllMain(
IN PVOID hmod,
IN ULONG ulReason,
IN PCONTEXT pctx OPTIONAL)
{
    if (ulReason != DLL_PROCESS_ATTACH)
    {
        return TRUE;
    }
    else
    {
        hModule = hmod;
    }

    return TRUE;

    UNREFERENCED_PARAMETER(pctx);
}


/****************************************************************************
*
*    FUNCTION: InitStereoApplet(HWND)
*
*    PURPOSE: loads the caption string for the Control Panel
*
*    COMMENTS:
*
*
****************************************************************************/

BOOL InitStereoApplet (HWND hwndParent)
{
    LoadString (hModule, CPCAPTION, szCtlPanel, sizeof(szCtlPanel));

    return TRUE;

    UNREFERENCED_PARAMETER(hwndParent);
}


/****************************************************************************
*
*    FUNCTION: TermStereoApplet()
*
*    PURPOSE: termination procedure for the stereo applets
*
*    COMMENTS:
*
*
****************************************************************************/

void TermStereoApplet()
{
    return;
}



/****************************************************************************
*
*    FUNCTION: CPIApplet(HWND, UINT, LONG, LONG)
*
*    PURPOSE: Processes messages for control panel applets
*
*    COMMENTS:
*
*
****************************************************************************/
LONG CALLBACK CPlApplet (hwndCPL, uMsg, lParam1, lParam2)
HWND hwndCPL;       // handle of Control Panel window
UINT uMsg;          // message
LONG lParam1;       // first message parameter
LONG lParam2;       // second message parameter
{
    int iApplet;
    LPNEWCPLINFO lpNewCPlInfo;
    static iInitCount = 0;
            
    switch (uMsg) {
        case CPL_INIT:              // first message, sent once
            if (!iInitCount)
            {
                if (!InitStereoApplet(hwndCPL))
                    return FALSE;
            }
            iInitCount++;
            return TRUE;

        case CPL_GETCOUNT:          // second message, sent once
            return (LONG)NUM_APPLETS;
            break;

        case CPL_NEWINQUIRE:        // third message, sent once per app
            lpNewCPlInfo = (LPNEWCPLINFO) lParam2;

            iApplet = (int)(LONG)lParam1;
            lpNewCPlInfo->dwSize = (DWORD) sizeof(NEWCPLINFO);
            lpNewCPlInfo->dwFlags = 0;
            lpNewCPlInfo->dwHelpContext = 0;
            lpNewCPlInfo->lData = 0;
            lpNewCPlInfo->hIcon = LoadIcon (hModule,
                (LPCTSTR) MAKEINTRESOURCE(StereoApplets[iApplet].icon));
            lpNewCPlInfo->szHelpFile[0] = '\0';

            LoadString (hModule, StereoApplets[iApplet].namestring,
                        lpNewCPlInfo->szName, 32);

            LoadString (hModule, StereoApplets[iApplet].descstring,
                        lpNewCPlInfo->szInfo, 64);
            break;

        case CPL_SELECT:            // application icon selected
            break;


        case CPL_DBLCLK:            // application icon double-clicked
            iApplet = (int)(LONG)lParam1;
            MessageBeep (MB_ICONEXCLAMATION);

            DialogBox (hModule,
                       MAKEINTRESOURCE(StereoApplets[iApplet].dlgtemplate),
                       hwndCPL,
                       StereoApplets[iApplet].dlgfn);
            break;

         case CPL_STOP:              // sent once per app. before CPL_EXIT
            break;

         case CPL_EXIT:              // sent once before FreeLibrary called
            iInitCount--;
            if (!iInitCount)
                TermStereoApplet();
            break;

         default:
            break;
    }
    return 0;
}

/****************************************************************************
*
*    FUNCTION: AmpDlgProc
*
*    PURPOSE: Processes messages sent to the Amp applet.
*
*    COMMENTS:
*
*    This dialog simply puts up a box and has an OK key.  It doesn't
*    do anything except display.
*
****************************************************************************/
BOOL APIENTRY AmpDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
    {
	    case WM_INITDIALOG:
	        return (TRUE);

	    case WM_COMMAND:      
            if (LOWORD(wParam))
            { 
		        EndDialog(hDlg, TRUE);
		        return (TRUE);
	        }
	        break;
    }
    return (FALSE);
}

/****************************************************************************
*
*    FUNCTION: TunerDlgProc
*
*    PURPOSE: Processes messages sent to the Tuner applet.
*
*    COMMENTS:
*
*    This dialog simply puts up a box and has an OK key.  It doesn't
*    do anything except display.
*
****************************************************************************/
BOOL APIENTRY TunerDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
    {
	    case WM_INITDIALOG:
	        return (TRUE);

	    case WM_COMMAND:      
            if (LOWORD(wParam))
            { 
		        EndDialog(hDlg, TRUE);
		        return (TRUE);
	        }
	        break;
    }
    return (FALSE);
}

/****************************************************************************
*
*    FUNCTION: TapeDlgProc
*
*    PURPOSE: Processes messages sent to the Tape applet.
*
*    COMMENTS:
*
*    This dialog simply puts up a box and has an OK key.  It doesn't
*    do anything except display.
*
****************************************************************************/
BOOL APIENTRY TapeDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
{
    switch (message)
    {
	    case WM_INITDIALOG:
	        return (TRUE);

	    case WM_COMMAND:      
            if (LOWORD(wParam))
            { 
		        EndDialog(hDlg, TRUE);
		        return (TRUE);
	        }
	        break;
    }
    return (FALSE);
}

