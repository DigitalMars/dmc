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
//  maswitch.c
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




//
//
//
typedef struct tMACONTROLINSTANCE_SWITCH
{
    LPMACONTROLINSTANCE             pmaci;
    MIXERCONTROLDETAILS_BOOLEAN     pmxcd_f[];

} MACONTROLINSTANCE_SWITCH, *PMACONTROLINSTANCE_SWITCH;


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppControlChangeSwitch
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
//  Return (BOOL):
//
//  History:
//      09/22/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppControlChangeSwitch
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwControlID
)
{
    MMRESULT                        mmr;
    HWND                            htxt;
    PMACONTROLINSTANCE_SWITCH       pmaci_switch;
    LPMACONTROLINSTANCE             pmaci;
    LPMIXERLINE                     pmxl;
    LPMIXERCONTROL                  pmxctrl;
    PMIXERCONTROLDETAILS_BOOLEAN    pmxcd_f;
    UINT                            cChannels;
    UINT                            cMultipleItems;
    UINT                            u;
    UINT                            uIndex;
    UINT                            v;
    BOOL                            fValue;
    MIXERCONTROLDETAILS             mxcd;
    HWND                            hcheckFocus;
    HWND                            hcheck;


    hcheckFocus = GetFocus();
    if (NULL == hcheckFocus)
    {
        hcheckFocus = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE);
    }
    else
    {
        uIndex = GetDlgCtrlID(hcheckFocus);
        if (uIndex < IDD_MACONTROL_MULTICHANNEL_BASE)
        {
            hcheckFocus = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE);
        }
    }


    //
    //
    //
    pmaci_switch = (PMACONTROLINSTANCE_SWITCH)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci        = pmaci_switch->pmaci;
    pmxl         = pmaci->pmxl;
    pmxctrl      = pmaci->pmxctrl;
    pmxcd_f      = &pmaci_switch->pmxcd_f[0];

    cChannels = (UINT)pmxl->cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
        cChannels = 1;


    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;

    mmr = mixerGetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }


    cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl->fdwControl)
        cMultipleItems = (UINT)pmxctrl->cMultipleItems;


    for (u = 0; u < cChannels; u++)
    {
        for (v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;

            fValue = (BOOL)pmxcd_f[uIndex].fValue;

            hcheck = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE + uIndex);
            if (hcheckFocus == hcheck)
            {
                htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
                AppSetWindowText(htxt, "mapped=%d, fValue=%ld",
                                fValue,
                                pmxcd_f[uIndex].fValue);
            }

            if (fValue == (BOOL)IsDlgButtonChecked(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE + uIndex))
                continue;

            CheckDlgButton(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE + uIndex, fValue);
        }
    }




    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = 1;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;

    mmr = mixerGetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, MIXER_GETCONTROLDETAILSF_VALUE);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }


    for (v = 0; v < cMultipleItems; v++)
    {
        uIndex = v;

        fValue = (BOOL)pmxcd_f[uIndex].fValue;

        hcheck = GetDlgItem(hwnd, IDD_MACONTROL_UNIFORM_BASE + uIndex);
        if (hcheckFocus == hcheck)
        {
            htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
            AppSetWindowText(htxt, "mapped=%d, fValue=%ld",
                             fValue,
                             pmxcd_f[uIndex].fValue);
        }

        if (fValue == (BOOL)IsDlgButtonChecked(hwnd, IDD_MACONTROL_UNIFORM_BASE + uIndex))
            continue;

        CheckDlgButton(hwnd, IDD_MACONTROL_UNIFORM_BASE + uIndex, fValue);
    }


    return (TRUE);
} // MixAppControlChangeSwitch()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppControlSetSwitch
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HWND hcheck:
//
//  Return (BOOL):
//
//  History:
//      09/22/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppControlSetSwitch
(
    HWND                    hwnd,
    HWND                    hcheck
)
{
    PMACONTROLINSTANCE_SWITCH       pmaci_switch;
    LPMACONTROLINSTANCE             pmaci;
    LPMIXERLINE                     pmxl;
    LPMIXERCONTROL                  pmxctrl;
    PMIXERCONTROLDETAILS_BOOLEAN    pmxcd_f;
    MMRESULT                        mmr;
    BOOL                            fValue;
    UINT                            cChannels;
    UINT                            uIndex;
    MIXERCONTROLDETAILS             mxcd;

    //
    //
    //
    pmaci_switch = (PMACONTROLINSTANCE_SWITCH)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci        = pmaci_switch->pmaci;
    pmxl         = pmaci->pmxl;
    pmxctrl      = pmaci->pmxctrl;
    pmxcd_f      = &pmaci_switch->pmxcd_f[0];

    uIndex = GetDlgCtrlID(hcheck);

    //
    //  get value and toggle..
    //
    fValue = (0 == IsDlgButtonChecked(hwnd, uIndex));

    if (uIndex < IDD_MACONTROL_UNIFORM_BASE)
    {
        cChannels = (UINT)pmxl->cChannels;
        if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
            cChannels = 1;

        uIndex -= IDD_MACONTROL_MULTICHANNEL_BASE;
    }
    else
    {
        cChannels = 1;

        uIndex -= IDD_MACONTROL_UNIFORM_BASE;
    }


    //
    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_f);
    mxcd.paDetails      = pmxcd_f;

    mmr = mixerGetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }

    pmxcd_f[uIndex].fValue = fValue;

    mmr = mixerSetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerSetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }

    CheckDlgButton(hwnd, GetDlgCtrlID(hcheck), (BOOL)pmxcd_f[uIndex].fValue);
#if 0
    if (0 == uIndex)
    {
        htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
        AppSetWindowText(htxt, "mapped=%d, fValue=%ld",
                         nValue,
                         pmxcd_f[uIndex].fValue);
    }
#endif

    return (TRUE);
} // MixAppControlSetSwitch()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppLineChangeSwitch
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
//  Return (BOOL):
//
//  History:
//      09/24/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppLineChangeSwitch
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwLineID
)
{
    MMRESULT                    mmr;
    PMACONTROLINSTANCE_SWITCH   pmaci_switch;
    LPMACONTROLINSTANCE         pmaci;
    LPMIXERLINE                 pmxl;
    MIXERLINE                   mxl;
    BOOL                        fSource;
    BOOL                        fActive;
    BOOL                        fDisconnected;
    HWND                        htxt;


    //
    //
    //
    pmaci_switch = (PMACONTROLINSTANCE_SWITCH)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_switch->pmaci;
    pmxl        = pmaci->pmxl;

    if (pmxl->dwLineID != dwLineID)
    {
        DPF(0, "!MixAppLineChangeSwitch: why am i getting notifications for some other line??");

        //
        //  this would be a bug in this app... keep from bombing though
        //  until i fix it.
        //
        dwLineID = pmxl->dwLineID;
    }

    //
    //  get the current state of the line that changed.. this is actually
    //  silly that you have to do this just to determine if the line went
    //  [in]active. sigh..
    //
    mxl.cbStruct      = sizeof(mxl);
    mxl.dwLineID      = dwLineID;

    mmr = mixerGetLineInfo((HMIXEROBJ)ghmx, &mxl, MIXER_GETLINEINFOF_LINEID);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetLineInfo(lineid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  dwLineID, ghmx, mmr);

        return (FALSE);
    }

    fSource       = (0 != (MIXERLINE_LINEF_SOURCE & mxl.fdwLine));
    fActive       = (0 != (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine));
    fDisconnected = (0 != (MIXERLINE_LINEF_DISCONNECTED & mxl.fdwLine));


    htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_LINEINFO);
    AppSetWindowText(htxt, "(%s), '%s', %s, %s",
                     fSource ? (LPSTR)"src" : (LPSTR)"DST",
                     (LPSTR)mxl.szShortName,
                     fActive ? (LPSTR)"ACTIVE" : (LPSTR)"inactive",
                     fDisconnected ? (LPSTR)"DISCONNECTED" : (LPSTR)"connected");

    return (fActive);
} // MixAppLineChangeSwitch()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppInitDialogSwitch
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HWND hwndFocus:
//
//      LPARAM lParam:
//
//  Return (BOOL):
//
//  History:
//      09/22/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppInitDialogSwitch
(
    HWND                    hwnd,
    HWND                    hwndFocus,
    LPARAM                  lParam
)
{
    #define FCB_DEF_STYLE   (WS_VISIBLE | WS_CHILD | BS_CHECKBOX | WS_TABSTOP)

    static TCHAR        szButton[]  = TEXT("button");
    static TCHAR        szTitle[]   = TEXT("Switch Class: '%s'");

    TCHAR                       szControlType[64];
    LPMACONTROLINSTANCE         pmaci;
    PMACONTROLINSTANCE_SWITCH   pmaci_switch;

    LPMIXERLINE                 pmxl;
    LPMIXERCONTROL              pmxctrl;

    HWND                        htxt;
    UINT                        u;
    UINT                        v;
    HWND                        hcheck;
    RECT                        rcM;
    RECT                        rcU;
    int                         cycap;
    UINT                        cb;
    UINT                        cChannels;
    UINT                        cMultipleItems;
    UINT                        uIndex;


    //
    //
    //
    pmaci = (LPMACONTROLINSTANCE)lParam;
    if (NULL == pmaci)
    {
        DPF(0, "!MixAppInitDialogSwitch: pmaci passed in lParam is NULL!?!");
        return (FALSE);
    }

    pmxl    = pmaci->pmxl;
    pmxctrl = pmaci->pmxctrl;

    if (!MixAppGetControlTypeName(pmxctrl, szControlType))
    {
        return (FALSE);
    }

    //
    //
    //
    switch (pmxctrl->dwControlType)
    {
        case MIXERCONTROL_CONTROLTYPE_BOOLEAN:
        case MIXERCONTROL_CONTROLTYPE_ONOFF:
        case MIXERCONTROL_CONTROLTYPE_MUTE:
        case MIXERCONTROL_CONTROLTYPE_MONO:
        case MIXERCONTROL_CONTROLTYPE_LOUDNESS:
        case MIXERCONTROL_CONTROLTYPE_STEREOENH:
            break;

        default:
            DPF(0, "!MixAppInitDialogSwitch: pmxctrl->dwControlType=%.08lXh not valid switch!", pmxctrl->dwControlType);
            return (FALSE);
    }



    //
    //
    //
    AppSetWindowText(hwnd, szTitle, (LPTSTR)szControlType);
    SetDlgItemText(hwnd, IDD_MACONTROL_TXT_SHORT_NAME, pmxctrl->szShortName);
    SetDlgItemText(hwnd, IDD_MACONTROL_TXT_LONG_NAME,  pmxctrl->szName);

    htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_BOUNDS);
    AppSetWindowText(htxt, "dwMinimum=%lu, dwMaximum=%lu",
                     pmxctrl->Bounds.dwMinimum,
                     pmxctrl->Bounds.dwMaximum);

    htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_METRICS);
    AppSetWindowText(htxt, "N/A");



    //
    //
    //
    cChannels = (UINT)pmxl->cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
        cChannels = 1;

    cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl->fdwControl)
        cMultipleItems = (UINT)pmxctrl->cMultipleItems;

    cb  = sizeof(*pmaci_switch);
    cb += cChannels * cMultipleItems * sizeof(pmaci_switch->pmxcd_f[0]);
    pmaci_switch = (PMACONTROLINSTANCE_SWITCH)LocalAlloc(LPTR, cb);
    if (NULL == pmaci_switch)
    {
        DPF(0, "!MixAppInitDialogSwitch: failed trying to alloc %u bytes for control instance!", cb);
        return (FALSE);
    }


    //
    //
    //
    SetWindowLong(hwnd, DWL_USER, (LPARAM)(UINT)pmaci_switch);

    pmaci_switch->pmaci    = pmaci;


    //
    //
    //
    //
    cycap = GetSystemMetrics(SM_CYCAPTION);

    hcheck = GetDlgItem(hwnd, IDD_MACONTROL_GRP_MULTICHANNEL);
    GetWindowRect(hcheck, &rcM);

    InflateRect(&rcM, -10, -20);
    ScreenToClient(hwnd, (LPPOINT)&rcM.left);
    ScreenToClient(hwnd, (LPPOINT)&rcM.right);

    rcM.bottom = rcM.top + cycap;

    for (u = 0; u < cChannels; u++)
    {
        for (v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;

            hcheck = CreateWindow(szButton, pmxctrl->szShortName, FCB_DEF_STYLE,
                                  rcM.left, rcM.top,
                                  rcM.right - rcM.left,
                                  rcM.bottom - rcM.top,
                                  hwnd, (HMENU)(IDD_MACONTROL_MULTICHANNEL_BASE + uIndex),
                                  ghinst, NULL);

            rcM.top    += cycap + 4;
            rcM.bottom += cycap + 4;
        }

        //
        //  add more separation between channels
        //
        rcM.top    += cycap;
        rcM.bottom += cycap;
    }


    //
    //
    //
    //
    hcheck = GetDlgItem(hwnd, IDD_MACONTROL_GRP_UNIFORM);
    GetWindowRect(hcheck, &rcU);

    InflateRect(&rcU, -10, -20);
    ScreenToClient(hwnd, (LPPOINT)&rcU.left);
    ScreenToClient(hwnd, (LPPOINT)&rcU.right);

    rcU.bottom = rcU.top + cycap;

    for (v = 0; v < cMultipleItems; v++)
    {
        hcheck = CreateWindow(szButton, pmxctrl->szShortName, FCB_DEF_STYLE,
                              rcU.left, rcU.top,
                              rcU.right - rcU.left,
                              rcU.bottom - rcU.top,
                              hwnd, (HMENU)(IDD_MACONTROL_UNIFORM_BASE + v),
                              ghinst, NULL);

        rcU.top    += cycap + 4;
        rcU.bottom += cycap + 4;
    }


    //
    //
    //
    SendMessage(hwnd,
                MM_MIXM_LINE_CHANGE,
                (WPARAM)pmaci->hmx,
                pmxl->dwLineID);

    SendMessage(hwnd,
                MM_MIXM_CONTROL_CHANGE,
                (WPARAM)pmaci->hmx,
                pmxctrl->dwControlID);

    return (TRUE);
} // MixAppInitDialogSwitch()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcControlSwitch
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
//      07/22/93
//
//--------------------------------------------------------------------------;

BOOL CALLBACK MixAppDlgProcControlSwitch
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    LRESULT             lr;
    HLOCAL              hl;
    UINT                uId;


    if (WM_COMMAND == uMsg)
    {
        uId = GET_WM_COMMAND_ID(wParam, lParam);
        if (IDD_MACONTROL_GRP_MULTICHANNEL <= uId)
        {
            MixAppControlSetSwitch(hwnd, GET_WM_COMMAND_HWND(wParam, lParam));
            return (TRUE);
        }
    }


    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            lr = HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, MixAppInitDialogSwitch);
            if (FALSE == lr)
            {
                EndDialog(hwnd, FALSE);
            }
            else
            {
                ghdlgControl = hwnd;
            }
            return (TRUE);


        case MM_MIXM_LINE_CHANGE:
            MixAppLineChangeSwitch(hwnd, (HMIXER)wParam, lParam);
            return (TRUE);

        case MM_MIXM_CONTROL_CHANGE:
            MixAppControlChangeSwitch(hwnd, (HMIXER)wParam, lParam);
            return (TRUE);


        case WM_COMMAND:
            uId = GET_WM_COMMAND_ID(wParam, lParam);
            switch (uId)
            {
                case IDCANCEL:
                case IDOK:
                    hl = (HLOCAL)(UINT)GetWindowLong(hwnd, DWL_USER);
                    LocalFree(hl);

                    EndDialog(hwnd, TRUE);
                    ghdlgControl = NULL;
                    break;
            }
            break;
    }

    return (FALSE);
} // MixAppDlgProcControlSwitch()
