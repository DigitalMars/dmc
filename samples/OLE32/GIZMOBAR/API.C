/*
 * API.C
 * GizmoBar Version 1.01
 *
 * API functions affecting a GizmoBar and a message processing
 * function to handle the equivalent called through messages.
 *
 * Copyright (c)1993-1995 Microsoft Corporation, All Rights Reserved
 *
 * Kraig Brockschmidt, Software Design Engineer
 * Microsoft Systems Developer Relations
 *
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  >INTERNET:kraigb@microsoft.com
 */


#include <windows.h>
#include <malloc.h>
#include "gizmoint.h"



/*
 * GBMessageHandler
 *
 * Purpose:
 *  Processes control messages that are equivalents of available
 *  control API.  The data passed with these messages is simply
 *  extracted from structures and passed as parameters to their
 *  equivalent function.
 *
 * Parameters:
 *  <Standard Message Parameters> plus
 *  pGB             PGIZMOBAR providing control-specific data.
 *
 * Return Value:
 *  LRESULT         Return value from equivalent API function.
 */

LRESULT GBMessageHandler(HWND hWnd, UINT iMsg, WPARAM wParam
    , LPARAM lParam, PGIZMOBAR pGB)
    {
    LRESULT         lRet=0L;
    LPCREATEGIZMOW   pCGW;
    LPCREATEGIZMOA   pCGA;
    LPGBMSG         pMsg;
    LPGBGETTEXTW    pGTW;
    LPGBGETTEXTA    pGTA;
    LPGBGETINT      pGI;
    LPGBSETINT      pSI;

    if (NULL==pGB)
        return 0L;

    switch (iMsg)
        {
        case GBM_HWNDASSOCIATESET:
            lRet=(LRESULT)(UINT)GBHwndAssociateSet(hWnd
                , (HWND)wParam);
            break;

        case GBM_HWNDASSOCIATEGET:
            lRet=(LRESULT)(UINT)GBHwndAssociateGet(hWnd);
            break;

        case GBM_GIZMOADDW:
            pCGW=(LPCREATEGIZMOW)lParam;
            lRet=(LRESULT)GBGizmoAddW(pCGW->hWndParent, pCGW->iType
                , pCGW->iGizmo, pCGW->uID, pCGW->dx, pCGW->dy
                , pCGW->pszText, pCGW->hBmp, pCGW->iImage, pCGW->uState);
            break;

        case GBM_GIZMOADDA:
            pCGA=(LPCREATEGIZMOA)lParam;
            lRet=(LRESULT)GBGizmoAddA(pCGA->hWndParent, pCGA->iType
                , pCGA->iGizmo, pCGA->uID, pCGA->dx, pCGA->dy
                , pCGA->pszText, pCGA->hBmp, pCGA->iImage, pCGA->uState);
            break;

        case GBM_GIZMOREMOVE:
            lRet=(LRESULT)GBGizmoRemove(hWnd, wParam);
            break;

        case GBM_GIZMOSENDMESSAGE:
            pMsg=(LPGBMSG)lParam;
            lRet=GBGizmoSendMessage(hWnd, wParam, pMsg->iMsg
                , pMsg->wParam, pMsg->lParam);
            break;

        case GBM_GIZMOSHOW:
            lRet=(LRESULT)GBGizmoShow(hWnd, wParam
                , (BOOL)LOWORD(lParam));
            break;

        case GBM_GIZMOENABLE:
            lRet=(LRESULT)GBGizmoEnable(hWnd, wParam
                , (BOOL)LOWORD(lParam));
            break;

        case GBM_GIZMOCHECK:
            lRet=(LRESULT)GBGizmoCheck(hWnd, wParam
                , (BOOL)LOWORD(lParam));
            break;

        case GBM_GIZMOFOCUSSET:
            lRet=(LRESULT)GBGizmoFocusSet(hWnd, wParam);
            break;

        case GBM_GIZMOEXIST:
            lRet=(LRESULT)GBGizmoExist(hWnd, wParam);
            break;

        case GBM_GIZMOTYPEGET:
            lRet=(LRESULT)GBGizmoTypeGet(hWnd, wParam);
            break;

        case GBM_GIZMODATASET:
            lRet=(LRESULT)GBGizmoDataSet(hWnd, wParam
                , (DWORD)lParam);
            break;

        case GBM_GIZMODATAGET:
            lRet=(LRESULT)GBGizmoDataGet(hWnd, wParam);
            break;

        case GBM_GIZMONOTIFYSET:
            lRet=(LRESULT)GBGizmoNotifySet(hWnd, wParam
                , (BOOL)LOWORD(lParam));
            break;

        case GBM_GIZMONOTIFYGET:
            lRet=(LRESULT)GBGizmoNotifyGet(hWnd, wParam);
            break;

        case GBM_GIZMOTEXTGETW:
            pGTW=(LPGBGETTEXTW)lParam;
            lRet=(LRESULT)GBGizmoTextGetW(hWnd, wParam, pGTW->psz
                , pGTW->cch);
            break;
        case GBM_GIZMOTEXTGETA:
            pGTA=(LPGBGETTEXTA)lParam;
            lRet=(LRESULT)GBGizmoTextGetA(hWnd, wParam, pGTA->psz
                , pGTA->cch);
            break;

        case GBM_GIZMOTEXTSETW:
            GBGizmoTextSetW(hWnd, wParam, (LPWSTR)lParam);
            break;

        case GBM_GIZMOTEXTSETA:
            GBGizmoTextSetA(hWnd, wParam, (LPSTR)lParam);
            break;

        case GBM_GIZMOINTGET:
            pGI=(LPGBGETINT)lParam;
            lRet=(LRESULT)GBGizmoIntGet(hWnd, wParam, &pGI->fSuccess
                , pGI->fSigned);
            break;


        case GBM_GIZMOINTSET:
            pSI=(LPGBSETINT)lParam;
            GBGizmoIntSet(hWnd, wParam, pSI->uValue, pSI->fSigned);
            break;

        default:
            break;
        }

    return lRet;
    }










/*
 * PGizmoFromHwndID
 *
 * Purpose:
 *  Retrieves the pGizmo for the given GizmoBar and the gizmo ID.
 *
 * Parameters:
 *  hWnd            HWND of a GizmoBar.
 *  uID             UINT gizmo identifier.
 *
 * Return Value:
 *  PGIZMO          NULL if the gizmo does not exist or hWnd is
 *                  invalid.  Non-NULL PGIZMO otherwise.
 */

PGIZMO PGizmoFromHwndID(HWND hWnd, UINT uID)
    {
    PGIZMOBAR     pGB;

    if (!IsWindow(hWnd))
        return FALSE;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL==pGB)
        return FALSE;

    return GizmoPFind(&pGB->pGizmos, uID);
    }






/*
 * GBHwndAssociateSet
 *
 * Purpose:
 *  Changes the associate window of a GizmoBar.
 *
 * Parameters:
 *  hWnd            HWND of the control window.
 *
 * Set Parameters:
 *  hWndAssociate   HWND of new associate.
 *
 * Return Value:
 *  HWND            Handle of previous associate.
 */

HWND WINAPI GBHwndAssociateSet(HWND hWnd, HWND hWndNew)
    {
    HWND        hWndOld=NULL;
    PGIZMOBAR   pGB;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL!=pGB)
        {
        hWndOld=pGB->hWndAssociate;
        pGB->hWndAssociate=hWndNew;

        if (NULL!=hWndOld)
            SendCommand(hWndOld, pGB->uID, GBN_ASSOCIATELOSS, hWnd);

        if (NULL!=hWndNew)
            SendCommand(hWndNew, pGB->uID, GBN_ASSOCIATEGAIN, hWnd);
        }

    return hWndOld;
    }





/*
 * GBHwndAssociateGet
 *
 * Purpose:
 *  Retrieves the associate window of a GizmoBar
 *
 * Parameters:
 *  hWnd            HWND of the control window.
 *
 * Set Parameters:
 *  hWndAssociate   HWND of new associate.
 *
 * Return Value:
 *  HWND            Handle of current associate.
 */

HWND WINAPI GBHwndAssociateGet(HWND hWnd)
    {
    HWND        hWndOld=NULL;
    PGIZMOBAR   pGB;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL!=pGB)
        hWndOld=pGB->hWndAssociate;

    return hWndOld;
    }





/*
 * GBGizmoAdd
 *
 * Purpose:
 *  Creates a new gizmo on the GizmoBar.  Subsequent operations
 *  should be done using the identifier, uID, for this gizmo.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  iType           UINT type of the gizmo to create.
 *  iGizmo          UINT position (zero-based) at which to place the
 *                  gizmo.
 *  uID             UINT identifier for WM_COMMAND from this gizmo.
 *  dx, dy          UINT dimensions of the gizmo.
 *  pszText         LPTSTR initial text for edit, list, combo, and
 *                  text gizmos.
 *  hBitmap         HBITMAP for gizmos of the button types (COMMAND
 *                  or ATTRIBUTE) specifies a source bitmap from
 *                  which the button image is taken.
 *  iImage          UINT index into hBitmap for the image for this
 *                  button.
 *  uState          UINT initial state of the gizmo.
 *
 * Return Value:
 *  BOOL            TRUE if creation succeeded, FALSE otherwise.
 */

BOOL WINAPI GBGizmoAddW(HWND hWnd, UINT iType, UINT iGizmo, UINT uID
    , UINT dx, UINT dy, LPWSTR pszText, HBITMAP hBmp, UINT iImage
    , UINT uState)
    {
    BOOL        fSuccess;
    PGIZMOBAR   pGB;
    PGIZMO      pGizmo;

    if (!IsWindow(hWnd))
        return FALSE;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL==pGB)
        return FALSE;

    /*
     * This automatically creates the windows, allocates structures,
     * includes the gizmo in pGB->pGizmos, and so forth.
     */
    pGizmo=GizmoPAllocate(&fSuccess, &pGB->pGizmos, hWnd, iType
        , iGizmo, uID, dx, dy, pszText, hBmp, iImage, uState);

    if (fSuccess)
        {
        if (NULL!=pGB->hWndAssociate)
            {
            SendCommand(pGB->hWndAssociate,GBN_GIZMOADDED, pGB->uID
                , hWnd);
            }

        InvalidateRect(hWnd, NULL, TRUE);
        UpdateWindow(hWnd);
        }
    else
        GizmoPFree(&pGB->pGizmos, pGizmo);

    return fSuccess;
    }

BOOL WINAPI GBGizmoAddA(HWND hWnd, UINT iType, UINT iGizmo, UINT uID
    , UINT dx, UINT dy, LPSTR pszText, HBITMAP hBmp, UINT iImage
    , UINT uState)
{
    LPWSTR  pszTemp;
    BOOL    retVal;

    if(pszText != NULL)
    {
        pszTemp = malloc((1+strlen(pszText))*sizeof(WCHAR));

        if (pszTemp)
        {
            MultiByteToWideChar(CP_ACP, 0, pszText, -1, pszTemp, strlen(pszText)+1);
            retVal=GBGizmoAddW(hWnd, iType, iGizmo, uID, dx, dy, pszTemp, hBmp, iImage, uState);

            free(pszTemp);
            return retVal;
        }

    }
    else
        return GBGizmoAddW(hWnd, iType, iGizmo, uID, dx, dy, NULL, hBmp, iImage, uState);
}


/*
 * GBGizmoRemove
 *
 * Purpose:
 *  Removes an existing gizmo from the GizmoBar.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier for this gizmo.
 *
 * Return Value:
 *  BOOL            TRUE if deletion succeeded, FALSE otherwise.
 */

BOOL WINAPI GBGizmoRemove(HWND hWnd, UINT uID)
    {
    PGIZMOBAR   pGB;
    PGIZMO      pGizmo;

    if (!IsWindow(hWnd))
        return FALSE;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL==pGB)
        return FALSE;

    pGizmo=GizmoPFind(&pGB->pGizmos, uID);

    if (NULL==pGizmo)
        return FALSE;

    GizmoPFree(&pGB->pGizmos, pGizmo);

    if (NULL!=pGB->hWndAssociate)
        {
        SendCommand(pGB->hWndAssociate, GBN_GIZMOREMOVED, pGB->uID
            , hWnd);
        }

    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
    return TRUE;
    }






/*
 * GBGizmoSendMessage
 *
 * Purpose:
 *  Implements the equivalent of SendMessage to a gizmo in the
 *  GizmoBar.  Separators, command buttons, and attribute buttons
 *  do not accept messages.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo to affect.
 *  iMsg            UINT message to send.
 *  wParam          WPARAM of the message.
 *  lParam          LPARAM of the message.
 *
 * Return Value:
 *  LRESULT         Return value from the message.  0L if the
 *                  gizmo does not accept messages.
 */

LRESULT WINAPI GBGizmoSendMessage(HWND hWnd, UINT uID, UINT iMsg
    , WPARAM wParam, LPARAM lParam)
    {
    PGIZMO      pGizmo;
    LONG        lRet=0L;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo && NULL!=pGizmo->hWnd)
        lRet=SendMessage(pGizmo->hWnd, iMsg, wParam, lParam);

    return lRet;
    }






/*
 * GBGizmoShow
 *
 * Purpose:
 *  Shows or hides a control, adjusting the positions of all others
 *  to make room for or reuse the space for this control.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo to affect.
 *  fShow           BOOL TRUE to show the gizmo, FALSE to hide it.
 *
 * Return Value:
 *  BOOL            TRUE if the function was successful, FALSE
 *                  otherwise.
 */

BOOL WINAPI GBGizmoShow(HWND hWnd, UINT uID, BOOL fShow)
    {
    BOOL        fRet=FALSE;
    PGIZMO      pGizmo;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        {
        if (fShow && pGizmo->fHidden)
            {
            if (NULL!=pGizmo->hWnd)
                ShowWindow(pGizmo->hWnd, SW_SHOWNORMAL);

            GizmosExpand(pGizmo);
            }

        if (!fShow && !pGizmo->fHidden)
            {
            if (NULL!=pGizmo->hWnd)
                ShowWindow(pGizmo->hWnd, SW_HIDE);

            GizmosCompact(pGizmo);
            }

        //This will be right even if we didn't change anything.
        pGizmo->fHidden=!fShow;
        }

    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
    return fRet;
    }






/*
 * GBGizmoEnable
 *
 * Purpose:
 *  Enables or disables a control on the GizmoBar.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo to affect.
 *  fEnable         BOOL TRUE to enable the gizmo, FALSE otherwise.
 *
 * Return Value:
 *  BOOL            TRUE if the gizmo was previously disabled, FALSE
 *                  otherwise.
 */

BOOL WINAPI GBGizmoEnable(HWND hWnd, UINT uID, BOOL fEnable)
    {
    PGIZMO      pGizmo;
    BOOL        fRet=FALSE;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL==pGizmo)
        return FALSE;

    fRet=(BOOL)(BUTTONGROUP_DISABLED & pGizmo->uState);

    //Use windows to enable or disable window gizmos
    if (NULL!=pGizmo->hWnd)
        EnableWindow(pGizmo->hWnd, fEnable);
    else
        {
        /*
         * If we're not down, command and attribute buttons act
         * the same.
         */
        if (!(BUTTONGROUP_DOWN & pGizmo->uState))
            {
            GizmoPStateSet(hWnd, pGizmo, fEnable
                ? COMMANDBUTTON_UP : COMMANDBUTTON_DISABLED);
            }
        else
            {
            /*
             * Attribute buttons are a little more sensitive with
             * DOWNDISABLED
             */
            GizmoPStateSet(hWnd, pGizmo, fEnable
                ? ATTRIBUTEBUTTON_DOWN
                : ATTRIBUTEBUTTON_DOWNDISABLED);
            }
        }

    return fRet;
    }







/*
 * GBGizmoCheck
 *
 * Purpose:
 *  Checks or unchecks an attribute button in the GizmoBar.  If the
 *  gizmo is part of a group of mutually exclusive attributes, then
 *  other gizmos are unchecked when this one is checked.  If this is
 *  the only one checked in these circumstances, this function is a
 *  NOP.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo to affect.
 *  fCheck          BOOL TRUE to check this gizmo, FALSE to uncheck.
 *
 * Return Value:
 *  BOOL            TRUE if the change took place.  FALSE otherwise.
 */

BOOL WINAPI GBGizmoCheck(HWND hWnd, UINT uID, BOOL fCheck)
    {
    PGIZMOBAR   pGB;
    PGIZMO      pGizmo;

    if (!IsWindow(hWnd))
        return FALSE;

    pGB=(PGIZMOBAR)GetWindowLong(hWnd, GBWL_STRUCTURE);

    if (NULL==pGB)
        return FALSE;

    pGizmo=GizmoPFind(&pGB->pGizmos, uID);

    if (NULL!=pGizmo)
        GizmoPCheck(hWnd, pGizmo, fCheck);

    return TRUE;
    }






/*
 * GBGizmoFocusSet
 *
 * Purpose:
 *  Sets the focus to a partuclar gizmo in the gizmo if that gizmo
 *  can accept the focus.  Separators, attribute buttons, text,
 *  and command buttons cannot have the focus.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo to affect.
 *
 * Return Value:
 *  BOOL            TRUE if the focus was set.  FALSE otherwise,
 *                  such as when uID identifies a control that cannot
 *                  have focus.
 */

UINT WINAPI GBGizmoFocusSet(HWND hWnd, UINT uID)
    {
    PGIZMO      pGizmo;
    BOOL        fRet=FALSE;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo && NULL!=pGizmo->hWnd)
        {
        fRet=TRUE;
        SetFocus(pGizmo->hWnd);
        }

    return fRet;
    }





/*
 * GBGizmoExist
 *
 * Purpose:
 *  Determines if a gizmo of a given identifier exists in the
 *  GizmoBar.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier to verify.
 *
 * Return Value:
 *  BOOL            TRUE if the gizmo exists, FALSE otherwise.
 */

BOOL WINAPI GBGizmoExist(HWND hWnd, UINT uID)
    {
    return (NULL!=PGizmoFromHwndID(hWnd, uID));
    }





/*
 * GBGizmoTypeGet
 *
 * Purpose:
 *  Returns the type of the gizmo specified by the given identifer.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier to find.
 *
 * Return Value:
 *  int             A GIZMOTYPE_* value if the function is
 *                  successful, otherwise -1.
 */

int WINAPI GBGizmoTypeGet(HWND hWnd, UINT uID)
    {
    int         iRet=-1;
    PGIZMO      pGizmo;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        iRet=pGizmo->iType;

    return iRet;
    }





/*
 * GBGizmoDataSet
 * GBGizmoDataGet
 *
 * Purpose:
 *  Sets or retrieves an extra DWORD value associated with the given
 *  gizmo.  Applications can store any information here they please.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo.
 *  dwData          (Set only) DWORD data to store with the gizmo.
 *
 * Return Value:
 *  DWORD           Set:  Previous value
 *                  Get:  Current value
 */

DWORD WINAPI GBGizmoDataSet(HWND hWnd, UINT uID, DWORD dwData)
    {
    PGIZMO      pGizmo;
    DWORD       dw=0L;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        {
        dw=pGizmo->dwData;
        pGizmo->dwData=dwData;
        }

    return dw;
    }



DWORD WINAPI GBGizmoDataGet(HWND hWnd, UINT uID)
    {
    PGIZMO      pGizmo;
    DWORD       dw=0L;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        dw=pGizmo->dwData;

    return dw;
    }






/*
 * GBGizmoNotifySet
 * GBGizmoNotifyGet
 *
 * Purpose:
 *  Sets or retrieves the notify status of a gizmo.  If notify is
 *  FALSE, the no WM_COMMAND messages are sent from the GizmoBar to
 *  the parent window when this gizmo is used.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifier of the gizmo.
 *  fNotify         (Set only) BOOL new notify status to set.
 *
 * Return Value:
 *  BOOL            Set:  Previous value of the notify flag.
 *                  Get:  Current value of the notify flag.
 */

BOOL WINAPI GBGizmoNotifySet(HWND hWnd, UINT uID, BOOL fNotify)
    {
    PGIZMO      pGizmo;
    BOOL        fRet=FALSE;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        {
        fRet=pGizmo->fNotify;
        pGizmo->fNotify=fNotify;
        }

    return fRet;
    }


BOOL WINAPI GBGizmoNotifyGet(HWND hWnd, UINT uID)
    {
    PGIZMO      pGizmo;
    BOOL        fRet=FALSE;

    pGizmo=PGizmoFromHwndID(hWnd, uID);

    if (NULL!=pGizmo)
        fRet=pGizmo->fNotify;

    return fRet;
    }







/*
 * GBGizmoTextSet
 * GBGizmoTextGet
 *
 * Purpose:
 *  Retrieves or sets text in a GizmoBar gizmo.  Separators, command
 *  buttons, and attribute buttons are not affected by this call.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifying the gizmo.
 *  psz             LPTSTR (Set) providing the text to show in the
 *                  window or (Get) pointing to a buffer to receive
 *                  the text.
 *  cch             (Get only) UINT maximum number of chars to copy
 *                  to psz.
 *
 * Return Value:
 *  int             Number of characters copied to psz.
 */

void WINAPI GBGizmoTextSetW(HWND hWnd, UINT uID, LPWSTR psz)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    SetDlgItemTextW(hWnd, uID, psz);
    return;
    }

void WINAPI GBGizmoTextSetA(HWND hWnd, UINT uID, LPSTR psz)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    SetDlgItemTextA(hWnd, uID, psz);
    return;
    }


int WINAPI GBGizmoTextGetW(HWND hWnd, UINT uID, LPWSTR psz, UINT cch)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    return GetDlgItemTextW(hWnd, uID, psz, cch);
    }

int WINAPI GBGizmoTextGetA(HWND hWnd, UINT uID, LPSTR psz, UINT cch)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    return GetDlgItemTextA(hWnd, uID, psz, cch);
    }



/*
 * GBGizmoIntSet
 * GBGizmoIntGet
 *
 * Purpose:
 *  Retrieves or sets an integer in a GizmoBar gizmo.  Separators,
 *  command buttons, and attribute buttons are not affected by this
 *  call.
 *
 * Parameters:
 *  hWnd            HWND of the GizmoBar.
 *  uID             UINT identifying the gizmo.
 *
 *  (Set only)
 *  u               UINT value to set in the gizmo.
 *  fSigned         BOOL TRUE to indicate if the value is signed.
 *
 *  (Get only)
 *  pfTrans         BOOL FAR * in which the success of the function
 *                  is returned.
 *  fSigned         BOOL TRUE to indicate if the value is signed.
 *
 * Return Value:
 *  (Set): None
 *  (Get): UINT     Integer translation of the gizmo's text.
 */

void WINAPI GBGizmoIntSet(HWND hWnd, UINT uID, UINT u, BOOL fSigned)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    SetDlgItemInt(hWnd, uID, u, fSigned);
    return;
    }



UINT WINAPI GBGizmoIntGet(HWND hWnd, UINT uID, BOOL FAR *pfTrans
    , BOOL fSigned)
    {
    //This fails on non-windowed gizmos anyway, so we don't check.
    return GetDlgItemInt(hWnd, uID, pfTrans, fSigned);
    }
