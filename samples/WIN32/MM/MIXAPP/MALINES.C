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
//  malines.c
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


TCHAR       gszLineFormatTitle[]        = TEXT("Type\t3!Component\t10!Line ID\t6!Flags\t6!Ctrls\t3!Conns\t3!Name");
TCHAR       gszLineFormatList[]         = TEXT("%-s%c\t%-s\t%.08lXh\t%.08lXh\t%lu\t%lu\t%-s");


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppGetComponentName
//
//  Description:
//
//
//  Arguments:
//      LPMIXERLINE pmxl:
//
//      LPTSTR szComponent:
//
//  Return (BOOL):
//
//  History:
//      08/18/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppGetComponentName
(
    LPMIXERLINE             pmxl,
    LPTSTR                  szComponent
)
{
    //
    //
    //
    if (0 == (MIXERLINE_LINEF_SOURCE & pmxl->fdwLine))
    {
        switch (pmxl->dwComponentType)
        {
            case MIXERLINE_COMPONENTTYPE_DST_UNDEFINED:
                lstrcpy(szComponent, "Undefined");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_DIGITAL:
                lstrcpy(szComponent, "Digital");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_LINE:
                lstrcpy(szComponent, "Line Level");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_MONITOR:
                lstrcpy(szComponent, "Monitor");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_SPEAKERS:
                lstrcpy(szComponent, "Speakers");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_HEADPHONES:
                lstrcpy(szComponent, "Headphones");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_TELEPHONE:
                lstrcpy(szComponent, "Telephone");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_WAVEIN:
                lstrcpy(szComponent, "Wave Input");
                break;

            case MIXERLINE_COMPONENTTYPE_DST_VOICEIN:
                lstrcpy(szComponent, "Voice Recognition");
                break;

            default:
                lstrcpy(szComponent, "NOT VALID");
                break;
        }
    }
    else
    {
        switch (pmxl->dwComponentType)
        {
            case MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED:
                lstrcpy(szComponent, "Undefined");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_DIGITAL:
                lstrcpy(szComponent, "Digital");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_LINE:
                lstrcpy(szComponent, "Line Level");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE:
                lstrcpy(szComponent, "Microphone");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER:
                lstrcpy(szComponent, "Synthesizer");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC:
                lstrcpy(szComponent, "Compact Disc");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE:
                lstrcpy(szComponent, "Telephone");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER:
                lstrcpy(szComponent, "PC Speaker");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT:
                lstrcpy(szComponent, "Wave Out");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY:
                lstrcpy(szComponent, "Auxiliary");
                break;

            case MIXERLINE_COMPONENTTYPE_SRC_ANALOG:
                lstrcpy(szComponent, "Analog");
                break;

            default:
                lstrcpy(szComponent, "NOT VALID");
                break;
        }
    }

    return (TRUE);
} // MixAppGetComponentName()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcLineInfo
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

BOOL CALLBACK MixAppDlgProcLineInfo
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    HWND                hedit;
    UINT                u;
    MIXERLINE           mxl;
    MMRESULT            mmr;
    TCHAR               ach[64];

    switch (uMsg)
    {
        case WM_INITDIALOG:
            hedit = GetDlgItem(hwnd, IDD_LINEINFO_EDIT_INFO);
            SetWindowFont(hedit, GetStockFont(ANSI_FIXED_FONT), FALSE);

            //
            //
            //
            //
            mxl.cbStruct      = sizeof(mxl);
            mxl.dwLineID      = lParam;

            mmr = mixerGetLineInfo((HMIXEROBJ)ghmx, &mxl, MIXER_GETLINEINFOF_LINEID);
            if (MMSYSERR_NOERROR != mmr)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                          "mixerGetLineInfo(lineid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                          lParam, ghmx, mmr);
            }
            else
            {
                static TCHAR    szDisplayTitle[]  = TEXT("[Line Info]\r\n");

                //
                //
                //
                MEditPrintF(hedit, NULL);
                MEditPrintF(hedit, szDisplayTitle);

                MEditPrintF(hedit, "%25s: %.04Xh", (LPTSTR)"Mixer Handle", ghmx);

                //
                //
                //
                MEditPrintF(hedit, "%25s: %lu bytes (requested %lu)", (LPTSTR)"Size of Line Info", mxl.cbStruct, (DWORD)sizeof(mxl));
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Destination", mxl.dwDestination);
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Source", mxl.dwSource);
                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"Line ID", mxl.dwLineID);
                MEditPrintF(hedit, "~%25s: %.08lXh", (LPTSTR)"Line Flags", mxl.fdwLine);

                if (MIXERLINE_LINEF_SOURCE & mxl.fdwLine)
                    MEditPrintF(hedit, "~, source");
                else
                    MEditPrintF(hedit, "~, destination");

                if (MIXERLINE_LINEF_DISCONNECTED & mxl.fdwLine)
                    MEditPrintF(hedit, "~, disconnected");

                if (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine)
                    MEditPrintF(hedit, "~, active");

                if (~(MIXERLINE_LINEF_SOURCE | MIXERLINE_LINEF_DISCONNECTED | MIXERLINE_LINEF_ACTIVE) & mxl.fdwLine)
                    MEditPrintF(hedit, "~, *INVALID FLAGS*");

                MEditPrintF(hedit, "");
                MEditPrintF(hedit, "%25s: %.08lXh", (LPTSTR)"Driver User DWord", mxl.dwUser);

                MixAppGetComponentName(&mxl, ach);
                MEditPrintF(hedit, "%25s: %lu, (%s)", (LPTSTR)"Component Type", mxl.dwComponentType, (LPTSTR)ach);
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Num Channels", mxl.cChannels);
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Num Connections", mxl.cConnections);
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Num Controls", mxl.cControls);
                MEditPrintF(hedit, "%25s: '%s'", (LPTSTR)"Short Name", (LPTSTR)mxl.szShortName);
                MEditPrintF(hedit, "%25s: '%s'", (LPTSTR)"Name", (LPTSTR)mxl.szName);
                MEditPrintF(hedit, "\r\n%25s: %lu", (LPTSTR)"Target Type", mxl.Target.dwType);
                MEditPrintF(hedit, "%25s: %lu", (LPTSTR)"Device ID", mxl.Target.dwDeviceID);
                MEditPrintF(hedit, "%25s: %u", (LPTSTR)"Manufacturer ID", mxl.Target.wMid);
                MEditPrintF(hedit, "%25s: %u", (LPTSTR)"Product ID", mxl.Target.wPid);
                MEditPrintF(hedit, "%25s: %u.%.02u", (LPTSTR)"Version",
                            mxl.Target.vDriverVersion >> 8,
                            mxl.Target.vDriverVersion & 0x00FF);
                MEditPrintF(hedit, "%25s: '%s'", (LPTSTR)"Product Name", (LPTSTR)mxl.Target.szPname);
            }

            //
            //  return nonzero to set the input focus to the control
            //  identified by the (hwndFocus = (HWND)wParam) argument.
            //  a zero return tells the dialog manager that this function
            //  has set the focus using SetFocus.
            //
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
} // MixAppDlgProcLineInfo()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppRefreshLineList
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd: Handle of main window.
//
//  Return (BOOL):
//
//  History:
//      05/16/93
//
//--------------------------------------------------------------------------;

BOOL FNGLOBAL MixAppRefreshLineList
(
    HWND                    hwnd,
    PTABBEDLISTBOX          ptlb
)
{
    static TCHAR        szLineTypeDst[] = TEXT("DST");
    static TCHAR        szLineTypeSrc[] = TEXT("  src");

    MMRESULT            mmr;
    UINT                u;
    UINT                v;
    UINT                cConnections;
    MIXERLINE           mxl;
    TCHAR               ach[128];
    TCHAR               szComponent[64];
    int                 nIndex;
    MIXERCAPS           mxcaps;


    //
    //
    //
    SetWindowRedraw(ptlb->hlb, FALSE);
    ListBox_ResetContent(ptlb->hlb);


    if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
        MixAppDebugLog(NULL);


    //
    //
    //
    //
    mmr = mixerGetDevCaps((UINT)ghmx, &mxcaps, sizeof(mxcaps));
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                    "mixerGetDevCaps() failed on hmx=%.04Xh, mmr=%u!",
                    ghmx, mmr);

        mxcaps.cDestinations = 0;
    }

    for (u = 0; u < mxcaps.cDestinations; u++)
    {
        mxl.cbStruct      = sizeof(mxl);
        mxl.dwDestination = u;

        mmr = mixerGetLineInfo((HMIXEROBJ)ghmx, &mxl, MIXER_GETLINEINFOF_DESTINATION);
        if (MMSYSERR_NOERROR != mmr)
        {
            AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                      "mixerGetLineInfo(dst=%u) failed on hmx=%.04Xh, mmr=%u!",
                      u, ghmx, mmr);
            continue;
        }

        MixAppGetComponentName(&mxl, szComponent);

        //
        //
        //
        wsprintf(ach, gszLineFormatList,
                 (LPSTR)szLineTypeDst,
                 (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine) ? '*' : ' ',
                 (LPTSTR)szComponent,
                 mxl.dwLineID,
                 mxl.fdwLine,
                 mxl.cControls,
                 mxl.cConnections,
                 (LPSTR)mxl.szName);

        if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
        {
            MixAppDebugLog(ach);
            MixAppDebugLog(gszCRLF);
        }

        nIndex = ListBox_AddString(ptlb->hlb, ach);
        ListBox_SetItemData(ptlb->hlb, nIndex, mxl.dwLineID);

        cConnections = (UINT)mxl.cConnections;
        for (v = 0; v < cConnections; v++)
        {
            mxl.cbStruct      = sizeof(mxl);
            mxl.dwDestination = u;
            mxl.dwSource      = v;

            mmr = mixerGetLineInfo((HMIXEROBJ)ghmx, &mxl, MIXER_GETLINEINFOF_SOURCE);
            if (MMSYSERR_NOERROR != mmr)
            {
                AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                          "mixerGetLineInfo(src=%u) failed on hmx=%.04Xh, mmr=%u!",
                          v, ghmx, mmr);
                continue;
            }


            MixAppGetComponentName(&mxl, szComponent);


            //
            //
            //
            wsprintf(ach, gszLineFormatList,
                     (LPSTR)szLineTypeSrc,
                     (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine) ? '*' : ' ',
                     (LPTSTR)szComponent,
                     mxl.dwLineID,
                     mxl.fdwLine,
                     mxl.cControls,
                     mxl.cConnections,
                     (LPSTR)mxl.szName);

            if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
            {
                MixAppDebugLog(ach);
                MixAppDebugLog(gszCRLF);
            }

            nIndex = ListBox_AddString(ptlb->hlb, ach);
            ListBox_SetItemData(ptlb->hlb, nIndex, mxl.dwLineID);
        }
    }


    //
    //
    //
    SetWindowRedraw(ptlb->hlb, TRUE);

    return (TRUE);
} // MixAppRefreshLineList()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  LRESULT MixAppLineChange
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HMIXER hmx:
//
//      DWORD dwLineID:
//
//  Return (LRESULT):
//
//  History:
//      07/21/93
//
//--------------------------------------------------------------------------;

LRESULT FNGLOBAL MixAppLineChange
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwLineID
)
{
    DPF(1, "MixAppLineChange(hwnd=%Xh, hmx=%Xh, dwLineID=%.08lXh)",
        hwnd, hmx, dwLineID);


    //
    //
    //
    if (gfDisplayingControl &&
        (dwLineID == gmxl.dwLineID) &&
        (NULL != ghdlgControl))
    {
        SendMessage(ghdlgControl, MM_MIXM_LINE_CHANGE, (WPARAM)hmx, dwLineID);
    }


    MixAppRefreshLineList(hwnd, gptlbLines);


    //
    //  return zero because we handled the message
    //
    return (0L);
} // MixAppLineChange()
