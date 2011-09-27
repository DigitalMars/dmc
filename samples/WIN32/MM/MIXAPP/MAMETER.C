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
//  mameter.c
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
#include "muldiv32.h"

#include "mixapp.h"

#include "debug.h"


//
//
//
typedef struct tMACONTROLINSTANCE_METER
{
    LPMACONTROLINSTANCE             pmaci;
    BOOL                            fSigned;
    DWORD                           dwRange;
    MIXERCONTROLDETAILS_SIGNED      pmxcd_s[];

} MACONTROLINSTANCE_METER, *PMACONTROLINSTANCE_METER;


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppControlChangeMeter
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

BOOL FNLOCAL MixAppControlChangeMeter
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwControlID
)
{
    MMRESULT                        mmr;
    HWND                            htxt;
    PMACONTROLINSTANCE_METER        pmaci_meter;
    LPMACONTROLINSTANCE             pmaci;
    LPMIXERLINE                     pmxl;
    LPMIXERCONTROL                  pmxctrl;
    PMIXERCONTROLDETAILS_SIGNED     pmxcd_s;
    UINT                            cChannels;
    UINT                            cMultipleItems;
    UINT                            u;
    UINT                            uIndex;
    UINT                            v;
    MIXERCONTROLDETAILS             mxcd;
    HWND                            hsbFocus;
    HWND                            hsb;
    BOOL                            fSigned;
    DWORD                           dwRange;
    DWORD                           dwValue;
    int                             nValue;


    hsbFocus = GetFocus();
    if (NULL == hsbFocus)
    {
        hsbFocus = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE);
    }
    else
    {
        uIndex = GetDlgCtrlID(hsbFocus);
        if (uIndex < IDD_MACONTROL_MULTICHANNEL_BASE)
        {
            hsbFocus = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE);
        }
    }


    //
    //
    //
    pmaci_meter = (PMACONTROLINSTANCE_METER)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_meter->pmaci;
    pmxl        = pmaci->pmxl;
    pmxctrl     = pmaci->pmxctrl;
    pmxcd_s     = &pmaci_meter->pmxcd_s[0];
    fSigned     = pmaci_meter->fSigned;
    dwRange     = pmaci_meter->dwRange;

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
    mxcd.cbDetails      = sizeof(*pmxcd_s);
    mxcd.paDetails      = pmxcd_s;

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

            if (fSigned)
            {
                dwValue = (DWORD)(pmxcd_s[uIndex].lValue -
                                  pmxctrl->Bounds.lMinimum);
            }
            else
            {
                dwValue  = (DWORD)pmxcd_s[uIndex].lValue;
                dwValue -= pmxctrl->Bounds.dwMinimum;
            }

            nValue = (int)MulDivRN(dwValue, 32767, dwRange);

            //
            //  Windows is stupid and forces an update to scrollbars
            //  (a flickering paint) even when the position does not
            //  change... so don't update if it is already in the
            //  right position.
            //
            hsb = GetDlgItem(hwnd, IDD_MACONTROL_MULTICHANNEL_BASE + uIndex);

            if (hsbFocus == hsb)
            {
                htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
                if (fSigned)
                {
                    AppSetWindowText(htxt, "mapped=%d, lValue=%ld",
                                     nValue,
                                     pmxcd_s[uIndex].lValue);
                }
                else
                {
                    AppSetWindowText(htxt, "mapped=%d, dwValue=%lu",
                                     nValue,
                                     pmxcd_s[uIndex].lValue);
                }
            }

            if (32767 - nValue == GetScrollPos(hsb, SB_CTL))
                continue;

            //
            //  note that we _invert_ the position since a meter will
            //  increase approaching the top (opposite of scrollbar
            //  logic)
            //
            SetScrollPos(hsb, SB_CTL, 32767 - nValue, TRUE);
        }
    }




    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = 1;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_s);
    mxcd.paDetails      = pmxcd_s;

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

        if (fSigned)
        {
            dwValue = (DWORD)(pmxcd_s[uIndex].lValue -
                              pmxctrl->Bounds.lMinimum);
        }
        else
        {
            dwValue  = (DWORD)pmxcd_s[uIndex].lValue;
            dwValue -= pmxctrl->Bounds.dwMinimum;
        }

        nValue = (int)MulDivRN(dwValue, 32767, dwRange);

        hsb = GetDlgItem(hwnd, IDD_MACONTROL_UNIFORM_BASE + uIndex);
        if (hsbFocus == hsb)
        {
            htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
            if (fSigned)
            {
                AppSetWindowText(htxt, "mapped=%d, lValue=%ld",
                                 nValue,
                                 pmxcd_s[uIndex].lValue);
            }
            else
            {
                AppSetWindowText(htxt, "mapped=%d, dwValue=%lu",
                                 nValue,
                                 pmxcd_s[uIndex].lValue);
            }
        }

        if (32767 - nValue == GetScrollPos(hsb, SB_CTL))
            continue;

        //
        //  note that we _invert_ the position since a meter will
        //  increase approaching the top (opposite of scrollbar
        //  logic)
        //
        SetScrollPos(hsb, SB_CTL, 32767 - nValue, TRUE);
    }


    return (TRUE);
} // MixAppControlChangeMeter()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppLineChangeMeter
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

BOOL FNLOCAL MixAppLineChangeMeter
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwLineID
)
{
    MMRESULT                    mmr;
    PMACONTROLINSTANCE_METER    pmaci_meter;
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
    pmaci_meter = (PMACONTROLINSTANCE_METER)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_meter->pmaci;
    pmxl        = pmaci->pmxl;

    if (pmxl->dwLineID != dwLineID)
    {
        DPF(0, "!MixAppLineChangeMeter: why am i getting notifications for some other line??");

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
} // MixAppLineChangeMeter()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppInitDialogMeter
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

BOOL FNLOCAL MixAppInitDialogMeter
(
    HWND                    hwnd,
    HWND                    hwndFocus,
    LPARAM                  lParam
)
{
    #define FSB_DEF_STYLE   (WS_VISIBLE | WS_CHILD | SBS_VERT | WS_TABSTOP)

    static TCHAR        szScrollBar[] = TEXT("scrollbar");
    static TCHAR        szTitle[]     = TEXT("Meter Class: '%s'");

    TCHAR                       szControlType[64];
    LPMACONTROLINSTANCE         pmaci;
    PMACONTROLINSTANCE_METER    pmaci_meter;

    LPMIXERLINE                 pmxl;
    LPMIXERCONTROL              pmxctrl;

    HWND                        htxt;
    UINT                        u;
    UINT                        v;
    HWND                        hsb;
    RECT                        rcM;
    RECT                        rcU;
    int                         cxvsb;
    UINT                        cb;
    UINT                        cChannels;
    UINT                        cMultipleItems;
    UINT                        uIndex;
    DWORD                       dwRange;
    BOOL                        fSigned;


    //
    //
    //
    pmaci = (LPMACONTROLINSTANCE)lParam;
    if (NULL == pmaci)
    {
        DPF(0, "!MixAppInitDialogMeter: pmaci passed in lParam is NULL!?!");
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
        case MIXERCONTROL_CONTROLTYPE_SIGNEDMETER:
        case MIXERCONTROL_CONTROLTYPE_PEAKMETER:
        case MIXERCONTROL_CONTROLTYPE_UNSIGNEDMETER:
            break;

        default:
            DPF(0, "!MixAppInitDialogMeter: pmxctrl->dwControlType=%.08lXh not valid meter!", pmxctrl->dwControlType);
            return (FALSE);
    }

    fSigned = (MIXERCONTROL_CT_UNITS_SIGNED == (MIXERCONTROL_CT_UNITS_MASK & pmxctrl->dwControlType));


    //
    //
    //
    AppSetWindowText(hwnd, szTitle, (LPTSTR)szControlType);
    SetDlgItemText(hwnd, IDD_MACONTROL_TXT_SHORT_NAME, pmxctrl->szShortName);
    SetDlgItemText(hwnd, IDD_MACONTROL_TXT_LONG_NAME,  pmxctrl->szName);

    htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_BOUNDS);
    if (fSigned)
    {
        AppSetWindowText(htxt, "lMinimum=%ld, lMaximum=%ld",
                         pmxctrl->Bounds.lMinimum,
                         pmxctrl->Bounds.lMaximum);

        dwRange = (pmxctrl->Bounds.lMaximum - pmxctrl->Bounds.lMinimum);
    }
    else
    {
        AppSetWindowText(htxt, "dwMinimum=%lu, dwMaximum=%lu",
                         pmxctrl->Bounds.dwMinimum,
                         pmxctrl->Bounds.dwMaximum);

        dwRange = (pmxctrl->Bounds.dwMaximum - pmxctrl->Bounds.dwMinimum);
    }

    htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_METRICS);
    AppSetWindowText(htxt, "dwRange=%lu",
                     dwRange);

    //
    //
    //
    cChannels = (UINT)pmxl->cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
        cChannels = 1;

    cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl->fdwControl)
        cMultipleItems = (UINT)pmxctrl->cMultipleItems;

    cb  = sizeof(*pmaci_meter);
    cb += cChannels * cMultipleItems * sizeof(pmaci_meter->pmxcd_s[0]);
    pmaci_meter = (PMACONTROLINSTANCE_METER)LocalAlloc(LPTR, cb);
    if (NULL == pmaci_meter)
    {
        DPF(0, "!MixAppInitDialogMeter: failed trying to alloc %u bytes for control instance!", cb);
        return (FALSE);
    }


    //
    //
    //
    SetWindowLong(hwnd, DWL_USER, (LPARAM)(UINT)pmaci_meter);

    pmaci_meter->pmaci    = pmaci;
    pmaci_meter->fSigned  = fSigned;
    pmaci_meter->dwRange  = dwRange;


    //
    //
    //
    //
    cxvsb = GetSystemMetrics(SM_CXVSCROLL);

    hsb = GetDlgItem(hwnd, IDD_MACONTROL_GRP_MULTICHANNEL);
    GetWindowRect(hsb, &rcM);

    InflateRect(&rcM, -10, -20);
    ScreenToClient(hwnd, (LPPOINT)&rcM.left);
    ScreenToClient(hwnd, (LPPOINT)&rcM.right);

    rcM.right = rcM.left + cxvsb;

    for (u = 0; u < cChannels; u++)
    {
        for (v = 0; v < cMultipleItems; v++)
        {
            uIndex = (u * cMultipleItems) + v;

            hsb = CreateWindow(szScrollBar, gszNull, FSB_DEF_STYLE,
                               rcM.left, rcM.top,
                               rcM.right - rcM.left,
                               rcM.bottom - rcM.top,
                               hwnd, (HMENU)(IDD_MACONTROL_MULTICHANNEL_BASE + uIndex),
                               ghinst, NULL);

            SetScrollRange(hsb, SB_CTL, 0, 32767, FALSE);

            rcM.left  += cxvsb + 4;
            rcM.right += cxvsb + 4;
        }

        //
        //  add more separation between channels
        //
        rcM.left  += cxvsb;
        rcM.right += cxvsb;
    }


    //
    //
    //
    //
    hsb = GetDlgItem(hwnd, IDD_MACONTROL_GRP_UNIFORM);
    GetWindowRect(hsb, &rcU);

    InflateRect(&rcU, -10, -20);
    ScreenToClient(hwnd, (LPPOINT)&rcU.left);
    ScreenToClient(hwnd, (LPPOINT)&rcU.right);

    rcU.right = rcU.left + cxvsb;

    for (v = 0; v < cMultipleItems; v++)
    {
        hsb = CreateWindow(szScrollBar, gszNull, FSB_DEF_STYLE,
                            rcU.left, rcU.top,
                            rcU.right - rcU.left,
                            rcU.bottom - rcU.top,
                            hwnd, (HMENU)(IDD_MACONTROL_UNIFORM_BASE + v),
                            ghinst, NULL);

        SetScrollRange(hsb, SB_CTL, 0, 32767, FALSE);

        rcU.left  += cxvsb + 4;
        rcU.right += cxvsb + 4;
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
} // MixAppInitDialogMeter()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcControlMeter
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

BOOL CALLBACK MixAppDlgProcControlMeter
(
    HWND                    hwnd,
    UINT                    uMsg,
    WPARAM                  wParam,
    LPARAM                  lParam
)
{
    static  BOOL        fTimerActive;
    LRESULT             lr;
    HLOCAL              hl;
    UINT                uId;
    BOOL                fLineActive;


    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            fTimerActive = FALSE;

            lr = HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, MixAppInitDialogMeter);
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
            fLineActive = MixAppLineChangeMeter(hwnd, (HMIXER)wParam, lParam);
            if (fLineActive)
            {
                if (!fTimerActive)
                {
                    fTimerActive = (0 != SetTimer(hwnd, 1, 100, NULL));
                }
            }
            else
            {
                if (fTimerActive)
                {
                    KillTimer(hwnd, 1);
                    fTimerActive = FALSE;
                }
            }
            return (TRUE);


        case WM_TIMER:
        case MM_MIXM_CONTROL_CHANGE:
            MixAppControlChangeMeter(hwnd, (HMIXER)wParam, lParam);
            return (TRUE);


        case WM_COMMAND:
            uId = GET_WM_COMMAND_ID(wParam, lParam);
            switch (uId)
            {
                case IDCANCEL:
                case IDOK:
                    if (fTimerActive)
                    {
                        KillTimer(hwnd, 1);
                        fTimerActive = FALSE;
                    }

                    hl = (HLOCAL)(UINT)GetWindowLong(hwnd, DWL_USER);
                    LocalFree(hl);

                    EndDialog(hwnd, TRUE);
                    ghdlgControl = NULL;
                    break;
            }
            break;
    }

    return (FALSE);
} // MixAppDlgProcControlMeter()
