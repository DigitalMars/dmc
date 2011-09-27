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
//  mafader.c
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
typedef struct tMACONTROLINSTANCE_FADER
{
    LPMACONTROLINSTANCE             pmaci;
    int                             nRange;
    int                             nPageInc;
    MIXERCONTROLDETAILS_UNSIGNED    pmxcd_u[];

} MACONTROLINSTANCE_FADER, *PMACONTROLINSTANCE_FADER;


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppControlChangeFader
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

BOOL FNLOCAL MixAppControlChangeFader
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwControlID
)
{
    MMRESULT                        mmr;
    HWND                            htxt;
    PMACONTROLINSTANCE_FADER        pmaci_fader;
    LPMACONTROLINSTANCE             pmaci;
    LPMIXERLINE                     pmxl;
    LPMIXERCONTROL                  pmxctrl;
    PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u;
    int                             nRange;
    int                             nValue;
    UINT                            cChannels;
    UINT                            cMultipleItems;
    UINT                            u;
    UINT                            uIndex;
    UINT                            v;
    MIXERCONTROLDETAILS             mxcd;
    HWND                            hsbFocus;
    HWND                            hsb;


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
    pmaci_fader = (PMACONTROLINSTANCE_FADER)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_fader->pmaci;
    pmxl        = pmaci->pmxl;
    pmxctrl     = pmaci->pmxctrl;
    pmxcd_u     = &pmaci_fader->pmxcd_u[0];
    nRange      = pmaci_fader->nRange;

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
    mxcd.cbDetails      = sizeof(*pmxcd_u);
    mxcd.paDetails      = pmxcd_u;

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

            nValue = (int)MulDivRN(pmxcd_u[uIndex].dwValue, nRange, 0xFFFF);


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
                AppSetWindowText(htxt, "mapped=%d, dwValue=%lu",
                                 nValue,
                                 pmxcd_u[uIndex].dwValue);
            }

            if (nRange - nValue == GetScrollPos(hsb, SB_CTL))
                continue;

            //
            //  note that we _invert_ the position since a fader will
            //  increase approaching the top (opposite of scrollbar
            //  logic)
            //
            SetScrollPos(hsb, SB_CTL, nRange - nValue, TRUE);
        }
    }

    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = 1;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_u);
    mxcd.paDetails      = pmxcd_u;

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

        nValue = (int)MulDivRN(pmxcd_u[uIndex].dwValue, nRange, 0xFFFF);

        hsb = GetDlgItem(hwnd, IDD_MACONTROL_UNIFORM_BASE + uIndex);
        if (hsbFocus == hsb)
        {
            htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
            AppSetWindowText(htxt, "mapped=%d, dwValue=%lu",
                             nValue,
                             pmxcd_u[0].dwValue);
        }

        if (nRange - nValue == GetScrollPos(hsb, SB_CTL))
            continue;

        //
        //  note that we _invert_ the position since a fader will
        //  increase approaching the top (opposite of scrollbar
        //  logic)
        //
        SetScrollPos(hsb, SB_CTL, nRange - nValue, TRUE);
    }


    return (TRUE);
} // MixAppControlChangeFader()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppControlSetFader
//
//  Description:
//
//
//  Arguments:
//      HWND hwnd:
//
//      HWND hsb:
//
//      UINT uCode:
//
//      int nPos:
//
//  Return (BOOL):
//
//  History:
//      09/22/93
//
//--------------------------------------------------------------------------;

BOOL FNLOCAL MixAppControlSetFader
(
    HWND                    hwnd,
    HWND                    hsb,
    UINT                    uCode,
    int                     nPos
)
{
    PMACONTROLINSTANCE_FADER        pmaci_fader;
    LPMACONTROLINSTANCE             pmaci;
    LPMIXERLINE                     pmxl;
    LPMIXERCONTROL                  pmxctrl;
    PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u;
    MMRESULT                        mmr;
    int                             nRange;
    int                             nPageInc;
    int                             nValue;
    UINT                            cChannels;
    UINT                            uIndex;
    MIXERCONTROLDETAILS             mxcd;

    //
    //
    //
    pmaci_fader = (PMACONTROLINSTANCE_FADER)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_fader->pmaci;
    pmxl        = pmaci->pmxl;
    pmxctrl     = pmaci->pmxctrl;
    pmxcd_u     = &pmaci_fader->pmxcd_u[0];
    nRange      = pmaci_fader->nRange;
    nPageInc    = pmaci_fader->nPageInc;


    uIndex = GetDlgCtrlID(hsb);

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


    nValue = GetScrollPos(hsb, SB_CTL);

    switch (uCode)
    {
        case SB_PAGEDOWN:
            nValue = (int)min(nRange, (LONG)nValue + nPageInc);
            break;

        case SB_LINEDOWN:
            nValue = (int)min(nRange, (LONG)nValue + 1);
            break;

        case SB_PAGEUP:
            nValue -= nPageInc;

            //-- fall through --//

        case SB_LINEUP:
            nValue = (nValue < 1) ? 0 : (nValue - 1);
            break;


        case SB_TOP:
            //
            //  !!! would be a VERY BAD IDEA to go full volume !!!
            //
            nValue = nRange / 2;
            break;

        case SB_BOTTOM:
            nValue = nRange;
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            nValue = nPos;
            break;

        default:
            return (FALSE);
    }


    //
    //
    //
    //
    mxcd.cbStruct       = sizeof(mxcd);
    mxcd.dwControlID    = pmxctrl->dwControlID;
    mxcd.cChannels      = cChannels;
    mxcd.cMultipleItems = pmxctrl->cMultipleItems;
    mxcd.cbDetails      = sizeof(*pmxcd_u);
    mxcd.paDetails      = pmxcd_u;

    mmr = mixerGetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }

    pmxcd_u[uIndex].dwValue  = (DWORD)MulDivRN((nRange - nValue), 0xFFFF, nRange);

    mmr = mixerSetControlDetails((HMIXEROBJ)pmaci->hmx, &mxcd, 0L);
    if (MMSYSERR_NOERROR != mmr)
    {
        AppMsgBox(hwnd, MB_OK | MB_ICONEXCLAMATION,
                  "mixerSetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                  pmxctrl->dwControlID, pmaci->hmx, mmr);
        return (FALSE);
    }

// * commented out to stop sliders getting out of synch    *
// * between different instances of the app. AGuy 22.12.93 *
//  SetScrollPos(hsb, SB_CTL, nValue, TRUE);

#if 0
    if (0 == uIndex)
    {
        htxt = GetDlgItem(hwnd, IDD_MACONTROL_TXT_VALUE);
        AppSetWindowText(htxt, "mapped=%d, dwValue=%lu",
                         nValue,
                         pmxcd_u[0].dwValue);
    }
#endif

    return (TRUE);
} // MixAppControlSetFader()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppLineChangeFader
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

BOOL FNLOCAL MixAppLineChangeFader
(
    HWND                    hwnd,
    HMIXER                  hmx,
    DWORD                   dwLineID
)
{
    MMRESULT                    mmr;
    PMACONTROLINSTANCE_FADER    pmaci_fader;
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
    pmaci_fader = (PMACONTROLINSTANCE_FADER)(UINT)GetWindowLong(hwnd, DWL_USER);
    pmaci       = pmaci_fader->pmaci;
    pmxl        = pmaci->pmxl;

    if (pmxl->dwLineID != dwLineID)
    {
        DPF(0, "!MixAppLineChangeFader: why am i getting notifications for some other line??");

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
} // MixAppLineChangeFader()


//--------------------------------------------------------------------------;
//
//  BOOL MixAppInitDialogFader
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

BOOL FNLOCAL MixAppInitDialogFader
(
    HWND                    hwnd,
    HWND                    hwndFocus,
    LPARAM                  lParam
)
{
    #define FSB_DEF_STYLE   (WS_VISIBLE | WS_CHILD | SBS_VERT | WS_TABSTOP)

    static TCHAR        szScrollBar[] = TEXT("scrollbar");
    static TCHAR        szTitle[]     = TEXT("Fader Class: '%s'");

    TCHAR                       szControlType[64];
    LPMACONTROLINSTANCE         pmaci;
    PMACONTROLINSTANCE_FADER    pmaci_fader;

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
    int                         nRange;
    int                         nPageInc;


    //
    //
    //
    pmaci = (LPMACONTROLINSTANCE)lParam;
    if (NULL == pmaci)
    {
        DPF(0, "!MixAppInitDialogFader: pmaci passed in lParam is NULL!?!");
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
        case MIXERCONTROL_CONTROLTYPE_FADER:
        case MIXERCONTROL_CONTROLTYPE_VOLUME:
        case MIXERCONTROL_CONTROLTYPE_BASS:
        case MIXERCONTROL_CONTROLTYPE_TREBLE:
        case MIXERCONTROL_CONTROLTYPE_EQUALIZER:
            break;

        default:
            DPF(0, "!MixAppInitDialogFader: pmxctrl->dwControlType=%.08lXh not valid fader!", pmxctrl->dwControlType);
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
    AppSetWindowText(htxt, "cSteps=%lu",
                     pmxctrl->Metrics.cSteps);



    //
    //
    //
    cChannels = (UINT)pmxl->cChannels;
    if (MIXERCONTROL_CONTROLF_UNIFORM & pmxctrl->fdwControl)
        cChannels = 1;

    cMultipleItems = 1;
    if (MIXERCONTROL_CONTROLF_MULTIPLE & pmxctrl->fdwControl)
        cMultipleItems = (UINT)pmxctrl->cMultipleItems;

    cb  = sizeof(*pmaci_fader);
    cb += cChannels * cMultipleItems * sizeof(pmaci_fader->pmxcd_u[0]);
    pmaci_fader = (PMACONTROLINSTANCE_FADER)LocalAlloc(LPTR, cb);
    if (NULL == pmaci_fader)
    {
        DPF(0, "!MixAppInitDialogFader: failed trying to alloc %u bytes for control instance!", cb);
        return (FALSE);
    }


    //
    //
    //
    SetWindowLong(hwnd, DWL_USER, (LPARAM)(UINT)pmaci_fader);

    nRange   = (int)min(32767, pmxctrl->Metrics.cSteps - 1);
    nPageInc = nRange / 10;
    if (0 == nPageInc)
        nPageInc = 1;

    pmaci_fader->pmaci    = pmaci;
    pmaci_fader->nRange   = nRange;
    pmaci_fader->nPageInc = nPageInc;


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

            SetScrollRange(hsb, SB_CTL, 0, nRange, FALSE);

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

        SetScrollRange(hsb, SB_CTL, 0, nRange, FALSE);

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
} // MixAppInitDialogFader()


//==========================================================================;
//
//
//
//
//==========================================================================;

//--------------------------------------------------------------------------;
//
//  BOOL MixAppDlgProcControlFader
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

BOOL CALLBACK MixAppDlgProcControlFader
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


    //
    //
    //
    switch (uMsg)
    {
        case WM_INITDIALOG:
            DPF(0, "WM_INITDIALOG");
            lr = HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, MixAppInitDialogFader);
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
            DPF(0, "MM_MIXM_LINE_CHANGE");
            MixAppLineChangeFader(hwnd, (HMIXER)wParam, lParam);
            return (TRUE);

        case MM_MIXM_CONTROL_CHANGE:
            DPF(0, "MM_MIXM_CONTROL_CHANGE");
            MixAppControlChangeFader(hwnd, (HMIXER)wParam, lParam);
            DPF(0, "done...");
            return (TRUE);


        case WM_VSCROLL:
            DPF(0,"WM_SCROLL");
            HANDLE_WM_VSCROLL(hwnd, wParam, lParam, MixAppControlSetFader);
            return (TRUE);


        case WM_COMMAND:
            DPF(0, "WM_COMMAND");
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
} // MixAppDlgProcControlFader()
