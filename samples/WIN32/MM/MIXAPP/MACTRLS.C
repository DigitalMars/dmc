//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;
//
//  mactrls.c
//
//  Description:
//
//
//  History:
//       9/21/93
//
//==========================================================================;

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include "mixapp.h"

#include "debug.h"


TCHAR       gszLineControlsFormatTitle[]= TEXT("Index\t2!Ctrl Type\t8!Ctrl ID\t6!Flags\t6!Items\t3!Name");
TCHAR       gszLineControlsFormatList[] = TEXT("%u\t%-s\t%.08lXh\t%.08lXh\t%lu\t%-s");



//--------------------------------------------------------------------------;
//
//  BOOL MixAppGetControlTypeName
//
//  Description:
//
//
//  Arguments:
//      LPMIXERCONTROl pmxctrl:
//
//      LPTSTR szControlType:
//
//  Return (BOOL):
//
//  History:
//      09/24/93
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MixAppGetControlTypeName
(
    LPMIXERCONTROL          pmxctrl,
    LPTSTR                  szControlType
)
{
    switch (pmxctrl->dwControlType)
    {
        case MIXERCONTROL_CONTROLTYPE_CUSTOM:
            lstrcpy(szControlType, "Custom!");
            break;

        case MIXERCONTROL_CONTROLTYPE_BOOLEANMETER:
            lstrcpy(szControlType, "Boolean Meter");
            break;

        case MIXERCONTROL_CONTROLTYPE_SIGNEDMETER:
            lstrcpy(szControlType, "Signed Meter");
            break;

        case MIXERCONTROL_CONTROLTYPE_PEAKMETER:
            lstrcpy(szControlType, "Peak Meter");
            break;

        case MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER:
            lstrcpy(szControlType, "Unsigned Meter");
            break;

        case MIXERCONTROL_CONTROLTYPE_BOOLEAN:
            lstrcpy(szControlType, "Boolean Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_ONOFF:
            lstrcpy(szControlType, "On/Off Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_MUTE:
            lstrcpy(szControlType, "Mute Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_MONO:
            lstrcpy(szControlType, "Mono Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_LOUDNESS:
            lstrcpy(szControlType, "Loudness Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_STEREOENH:
            lstrcpy(szControlType, "Stereo Enh Switch");
            break;

        case MIXERCONTROL_CONTROLTYPE_BUTTON:
            lstrcpy(szControlType, "Button");
            break;

        case MIXERCONTROL_CONTROLTYPE_DECIBELS:
            lstrcpy(szControlType, "Decibels Number");
            break;

        case MIXERCONTROL_CONTROLTYPE_SIGNED:
            lstrcpy(szControlType, "Signed Number");
            break;

        case MIXERCONTROL_CONTROLTYPE_PERCENT:
            lstrcpy(szControlType, "Percent Number");
            break;

        case MIXERCONTROL_CONTROLTYPE_UNSIGNED:
            lstrcpy(szControlType, "Unsigned Number");
            break;

        case MIXERCONTROL_CONTROLTYPE_SLIDER:
            lstrcpy(szControlType, "Slider");
            break;

        case MIXERCONTROL_CONTROLTYPE_PAN:
            lstrcpy(szControlType, "Pan Slider");
            break;

        case MIXERCONTROL_CONTROLTYPE_QSOUNDPAN:
            lstrcpy(szControlType, "Q-Sound Pan Slider");
            break;

        case MIXERCONTROL_CONTROLTYPE_FADER:
            lstrcpy(szControlType, "Fader");
            break;

        case MIXERCONTROL_CONTROLTYPE_VOLUME:
            lstrcpy(szControlType, "Volume Fader");
            break;

        case MIXERCONTROL_CONTROLTYPE_BASS:
            lstrcpy(szControlType, "Bass Fader");
            break;

        case MIXERCONTROL_CONTROLTYPE_TREBLE:
            lstrcpy(szControlType, "Treble Fader");
            break;

        case MIXERCONTROL_CONTROLTYPE_EQUALIZER:
            lstrcpy(szControlType, "Equalizer Fader");
            break;

        case MIXERCONTROL_CONTROLTYPE_SINGLESELECT:
            lstrcpy(szControlType, "Single-Sel List");
            break;

        case MIXERCONTROL_CONTROLTYPE_MUX:
            lstrcpy(szControlType, "MUX List");
            break;

        case MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT:
            lstrcpy(szControlType, "Multi-Sel List");
            break;

        case MIXERCONTROL_CONTROLTYPE_MIXER:
            lstrcpy(szControlType, "Mixer List");
            break;

        case MIXERCONTROL_CONTROLTYPE_MICROTIME:
            lstrcpy(szControlType, "Microsecond Time");
            break;

        case MIXERCONTROL_CONTROLTYPE_MILLITIME:
            lstrcpy(szControlType, "Millisecond Time");
            break;

        default:
            DPF(0, "!MixAppGetControlTypeName: invalid control type %.08lXh!", pmxctrl->dwControlType);
            lstrcpy(szControlType, "** NOT VALID **");
            return (FALSE);
    }

    return (TRUE);
} // MixAppGetControlTypeName()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppDisplayControl
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      DWORD dwControlID:
//
//  Return (BOOL):
//
//  History:
//      07/21/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppDisplayControl
(
    HWND                    hwnd,
    DWORD                   dwControlID
)
{
    MIXERLINECONTROLS   mxlc;
    MMRESULT            mmr;
    BOOL                f;
    MACONTROLINSTANCE   maci;


    //
    //
    //
    //
    mxlc.cbStruct    = sizeof(mxlc);
////mxlc.dwLineID    = 0;
    mxlc.dwControlID = dwControlID;
////mxlc.cControls   = 1;
    mxlc.cbmxctrl    = sizeof(gmxctrl);
    mxlc.pamxctrl    = &gmxctrl;

    mmr = mixerGetLineControls((HMIXEROBJ)ghmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYID);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetLineControls(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  dwControlID, ghmx, mmr);
        return (FALSE);
    }


    //
    //
    //
    if (MIXERCONTROL_CONTROLF_DISABLED & gmxctrl.fdwControl)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "This control (ctrlid=%.08lXh) is disabled.",
                  dwControlID);
        return (FALSE);
    }


    //
    //
    //
    gfDisplayingControl = TRUE;
    gdwControlID        = dwControlID;


    //
    //
    //
    maci.hmx        = ghmx;
    maci.pmxl       = &gmxl;
    maci.pmxctrl    = &gmxctrl;



    //
    //
    //
    //
    f = FALSE;
    switch (MIXERCONTROL_CT_CLASS_MASK & gmxctrl.dwControlType)
    {
        case MIXERCONTROL_CT_CLASS_CUSTOM:
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "Control type class CUSTOM not done yet!");
            break;

        case MIXERCONTROL_CT_CLASS_METER:
            f = DialogBoxParam(ghinst,
                               DLG_MIXAPP_CONTROL,
                               hwnd,
                               MixAppDlgProcControlMeter,
                               (LPARAM)(LPVOID)&maci);
            break;

        case MIXERCONTROL_CT_CLASS_SWITCH:
            f = DialogBoxParam(ghinst,
                               DLG_MIXAPP_CONTROL,
                               hwnd,
                               MixAppDlgProcControlSwitch,
                               (LPARAM)(LPVOID)&maci);
            break;

        case MIXERCONTROL_CT_CLASS_NUMBER:
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "Control type class NUMBER not done yet!");
            break;

        case MIXERCONTROL_CT_CLASS_SLIDER:
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "Control type class SLIDER not done yet!");
            break;

        case MIXERCONTROL_CT_CLASS_FADER:
            f = DialogBoxParam(ghinst,
                               DLG_MIXAPP_CONTROL,
                               hwnd,
                               MixAppDlgProcControlFader,
                               (LPARAM)(LPVOID)&maci);
            break;

        case MIXERCONTROL_CT_CLASS_LIST:
            f = DialogBoxParam(ghinst,
                               DLG_MIXAPP_CONTROL,
                               hwnd,
                               MixAppDlgProcControlList,
                               (LPARAM)(LPVOID)&maci);
            break;

        default:
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "Unknown control type class=%.08lXh!",
                      gmxctrl.dwControlType);
            break;
    }

    gfDisplayingControl = FALSE;

    //
    //
    //
    if (!f)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "Do not know how to deal with control type=%.08lXh!",
                  gmxctrl.dwControlType);
    }

    return (f);
} // MixAppDisplayControl()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcControlInfo
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      UINT uMsg:
//
//      WPARAM wParam:
//
//      LPARAM lParam:
//
//  Return (BOOL):
//
//  History:
//      07/12/93
//
//--------------------------------------------------------------------------;

BOOL CALLBACK MixAppDlgProcControlInfo
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    HWND                hedit;
    UINT                u;
    MIXERLINECONTROLS   mxlc;
    MIXERCONTROL        mxctrl;
    MMRESULT            mmr;

    //
    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            hedit = GetDlgItem(hwnd, IDD_CONTROLINFO_EDIT_INFO);
            SetWindowFont(hedit, GetStockFont(ANSI_FIXED_FONT), FALSE);

            //
            //
            //
            //
            mxlc.cbStruct       = sizeof(mxlc);
////////////mxlc.dwLineID       = 0;
            mxlc.dwControlID    = lParam;
////////////mxlc.cControls      = 1;
            mxlc.cbmxctrl       = sizeof(mxctrl);
            mxlc.pamxctrl       = &mxctrl;

            mmr = mixerGetLineControls((HMIXEROBJ)ghmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYID);
            if (MMSYSERR_NOERROR != mmr)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                          "mixerGetLineControls(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                          lParam, ghmx, mmr);
            }
            else
            {
                static TCHAR    szDisplayTitle[]  = TEXT("[Control Info]\r\n");

                //
                //
                //
                MEditPrintF(hedit, NULL);
                MEditPrintF(hedit, szDisplayTitle);

                MEditPrintF(hedit, "%25s: %.04Xh", (LPTSTR)"Mixer Handle",
                            ghmx);

                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"Line ID",
                            mxlc.dwLineID);


                //
                //
                //
                MEditPrintF(hedit, "%25s: %lu bytes (requested %lu)", (LPTSTR)"Size of Control Info",
                            mxctrl.cbStruct, (DWORD)sizeof(mxctrl));


                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"Control ID",
                            mxctrl.dwControlID);

                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"Control Type",
                            mxctrl.dwControlType);

                MEditPrintF(hedit, "~%25s: %.08lXh", (LPTSTR)"Control Flags",
                            mxctrl.fdwControl);

                if (MIXERCONTROL_CONTROLF_DISABLED & mxctrl.fdwControl)
                    MEditPrintF(hedit, "~, disabled");

                if (MIXERCONTROL_CONTROLF_MULTIPLE & mxctrl.fdwControl)
                    MEditPrintF(hedit, "~, multiple");

                if (MIXERCONTROL_CONTROLF_UNIFORM & mxctrl.fdwControl)
                    MEditPrintF(hedit, "~, uniform");

                if (~(MIXERCONTROL_CONTROLF_UNIFORM | MIXERCONTROL_CONTROLF_MULTIPLE | MIXERCONTROL_CONTROLF_DISABLED) & mxctrl.fdwControl)
                    MEditPrintF(hedit, "~, *INVALID FLAGS*");

                MEditPrintF(hedit, "");

                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Multiple Items",
                            mxctrl.cMultipleItems);

                MEditPrintF(hedit, "%25s: '%s'", (LPTSTR)"Short Name",
                            (LPTSTR)mxctrl.szShortName);

                MEditPrintF(hedit, "%25s: '%s'", (LPTSTR)"Name",
                            (LPTSTR)mxctrl.szName);


                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"dwMinimum",
                            mxctrl.Bounds.dwMinimum);

                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"dwMaximum",
                            mxctrl.Bounds.dwMaximum);

                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"cSteps",
                            mxctrl.Metrics.cSteps);
            }

            return (TRUE);

        case WM_COMMAND:
            u = GET_WM_COMMAND_ID(wParam, lParam);
            if ((IDOK == u) || (IDCANCEL == u))
            {
                EndDialog(hwnd, (IDOK == u));
            }
            break;
    }

    return (FALSE);
} // MixAppDlgProcControlInfo()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcLineControls
//
//  Description:
//      This dialog procedure is used to display driver capabilities.
//
//  Arguments:
//      HWND hwnd: Handle to window.
//
//      UINT uMsg: Message being sent to the window.
//
//      WPARAM wParam: Specific argument to message.
//
//      LPARAM lParam: Specific argument to message.
//
//  Return (BOOL):
//      The return value is specific to the message that was received. For
//      the most part, it is FALSE if this dialog procedure does not handle
//      a message.
//
//  History:
//       1/ 2/93
//
//--------------------------------------------------------------------------;

BOOL CALLBACK MixAppDlgProcLineControls
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    static PTABBEDLISTBOX   ptlb;
    HWND                hwndStatic;
    MMRESULT            mmr;
    RECT                rc;
    PAINTSTRUCT         ps;
    UINT                uId;
    UINT                uCode;
    int                 n;
    LRESULT             lr;

    UINT                cbmxctrls;
    MIXERLINECONTROLS   mxlc;
    PMIXERCONTROL       pamxctrl;


    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            hwndStatic = GetDlgItem(hwnd, IDD_LINECONTROLS_STATIC_POSITION);

            GetWindowRect(hwndStatic, &rc);
            ScreenToClient(hwnd, (LPPOINT)&rc.left);
            ScreenToClient(hwnd, (LPPOINT)&rc.right);

            ShowWindow(hwndStatic, SW_HIDE);

            EnableWindow(GetDlgItem(hwnd, IDD_LINECONTROLS_BTN_INFO), FALSE);
            EnableWindow(GetDlgItem(hwnd, IDD_LINECONTROLS_BTN_SETTINGS), FALSE);

            ptlb = TlbCreate(hwnd, IDD_LINECONTROLS_LIST_CONTROLS, &rc);
            if (NULL == ptlb)
                return (TRUE);

            TlbSetFont(ptlb, GetStockFont(ANSI_VAR_FONT), FALSE);
            TlbSetTitleAndTabs(ptlb, gszLineControlsFormatTitle, FALSE);

            SetWindowPos(ptlb->hlb, GetDlgItem(hwnd, IDOK),
                            0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

            ListBox_ResetContent(ptlb->hlb);

            //
            //
            //
            //
            gmxl.cbStruct     = sizeof(gmxl);
            gmxl.dwLineID     = lParam;

            mmr = mixerGetLineInfo((HMIXEROBJ)ghmx, &gmxl, MIXER_GETLINEINFOF_LINEID);
            if (MMSYSERR_NOERROR != mmr)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                        "mixerGetLineInfo(lineid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                        lParam, ghmx, mmr);
                EndDialog(hwnd, FALSE);
                return (TRUE);
            }

            if (0 == gmxl.cControls)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                            "There are no controls associated with the selected line.");
                EndDialog(hwnd, FALSE);
                return (TRUE);
            }


            cbmxctrls = sizeof(*pamxctrl) * (UINT)gmxl.cControls;

            pamxctrl = (PMIXERCONTROL)LocalAlloc(LPTR, cbmxctrls);
            if (NULL == pamxctrl)
            {
                EndDialog(hwnd, FALSE);
                return (TRUE);
            }

            SetWindowRedraw(ptlb->hlb, FALSE);

            //
            //
            //
            //
            mxlc.cbStruct       = sizeof(mxlc);
            mxlc.dwLineID       = lParam;
////////////mxlc.dwControlID    = 0;
////////////mxlc.dwControlType  = 0;
            mxlc.cControls      = gmxl.cControls;
            mxlc.cbmxctrl       = sizeof(*pamxctrl);
            mxlc.pamxctrl       = pamxctrl;

            mmr = mixerGetLineControls((HMIXEROBJ)ghmx, &mxlc, MIXER_GETLINECONTROLSF_ALL);
            if (MMSYSERR_NOERROR != mmr)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                        "mixerGetLineControls(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                        lParam, ghmx, mmr);
            }
            else
            {
                TCHAR       ach[128];
                TCHAR       szControlType[64];
                UINT        u;
                int         nIndex;

                if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
                    MixAppDebugLog(NULL);

                for (u = 0; u < mxlc.cControls; u++)
                {
                    MixAppGetControlTypeName(&pamxctrl[u], szControlType);

                    //
                    //
                    //
                    wsprintf(ach, gszLineControlsFormatList,
                            u,
                            (LPTSTR)szControlType,
                            pamxctrl[u].dwControlID,
                            pamxctrl[u].fdwControl,
                            pamxctrl[u].cMultipleItems,
                            (LPSTR)pamxctrl[u].szName);

                    if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
                    {
                        MixAppDebugLog(ach);
                        MixAppDebugLog(gszCRLF);
                    }

                    nIndex = ListBox_AddString(ptlb->hlb, ach);
                    ListBox_SetItemData(ptlb->hlb, nIndex, pamxctrl[u].dwControlID);
                }

                ListBox_SetCurSel(ptlb->hlb, 0);
                EnableWindow(GetDlgItem(hwnd, IDD_LINECONTROLS_BTN_INFO), TRUE);
                EnableWindow(GetDlgItem(hwnd, IDD_LINECONTROLS_BTN_SETTINGS), TRUE);
            }

            SetWindowRedraw(ptlb->hlb, TRUE);
            LocalFree((HLOCAL)pamxctrl);
            return (TRUE);


        case WM_PAINT:
            if (NULL != ptlb)
            {
                BeginPaint(hwnd, &ps);
                TlbPaint(ptlb, hwnd, ps.hdc);
                EndPaint(hwnd, &ps);
            }
            break;

        case WM_COMMAND:
            uId   = GET_WM_COMMAND_ID(wParam, lParam);
            uCode = GET_WM_COMMAND_CMD(wParam, lParam);
            switch (uId)
            {
                case IDOK:
                case IDCANCEL:
                    if (NULL != ptlb)
                    {
                        //
                        //  hadidk! don't destroy the listbox window, but
                        //  free all other memory for TLB. the listbox
                        //  window will be destroyed when the dialog is
                        //  destroyed.
                        //
                        ptlb->hlb = NULL;

                        TlbDestroy(ptlb);
                        ptlb = NULL;
                    }

                    EndDialog(hwnd, TRUE);
                    break;


                case IDD_LINECONTROLS_BTN_INFO:
                    n  = ListBox_GetCurSel(ptlb->hlb);
                    if (LB_ERR == n)
                        break;

                    lr = ListBox_GetItemData(ptlb->hlb, n);
                    DialogBoxParam(ghinst,
                                   DLG_CONTROLINFO,
                                   hwnd,
                                   MixAppDlgProcControlInfo,
                                   lr);
                    break;

                case IDD_LINECONTROLS_BTN_SETTINGS:
                    n  = ListBox_GetCurSel(ptlb->hlb);
                    if (LB_ERR == n)
                        break;

                    lr = ListBox_GetItemData(ptlb->hlb, n);
                    MixAppDisplayControl(hwnd, lr);
                    break;

                case IDD_LINECONTROLS_LIST_CONTROLS:
                    switch (uCode)
                    {
                        case LBN_SELCHANGE:
                            break;

                        case LBN_DBLCLK:
                            n  = ListBox_GetCurSel(ptlb->hlb);
                            if (LB_ERR == n)
                                break;

                            lr = ListBox_GetItemData(ptlb->hlb, n);
                            if (GetKeyState(VK_CONTROL) < 0)
                            {
                                DialogBoxParam(ghinst,
                                               DLG_CONTROLINFO,
                                               hwnd,
                                               MixAppDlgProcControlInfo,
                                               lr);
                            }
                            else
                            {
                                MixAppDisplayControl(hwnd, lr);
                            }
                            break;
                    }
                    break;
            }
            break;
    }

    return (FALSE);
} // MixAppDlgProcLineControls


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  LRESULT MixAppControlChange
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HMIXER hmx:
//
//      DWORD dwControlID:
//
//  Return (LRESULT):
//
//  History:
//      07/21/93
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL MixAppControlChange
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwControlID
)
{
    DPF(1, "MixAppControlChange(hwnd=%Xh, hmx=%Xh, dwControlID=%.08lXh)",
        hwnd, hmx, dwControlID);


    //
    //
    //
    if (gfDisplayingControl &&
        (dwControlID == gdwControlID) &&
        (NULL != ghdlgControl))
    {
        SendMessage(ghdlgControl, MM_MIXM_CONTROL_CHANGE, (WPARAM)hmx, dwControlID);
    }


    //
    //  return zero because we handled the message
    //
    return (0L);
} // MixAppControlChange()
