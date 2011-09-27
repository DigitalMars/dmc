//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       engnlist.cxx
//
//  Contents:   implementation for CEngineList
//
//  Classes:    CEngineList
//
//  Functions:
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include "engnlist.h"
#include <mandcid.h>
#include <juliacid.h>
#include <plasmcid.h>
#include <rgencid.h>

//+---------------------------------------------------------------------------
//
//  Member:     CEngineList::CEngineList
//
//  Synopsis:   constructor
//
//  Arguments:  [hmenuOuter] - handle to the menu where the list of engines
//                             will be attached
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CEngineList::CEngineList(HMENU hmenuOuter)
{
    _hmenuOuter = hmenuOuter;
    _hMenu = NULL;
    _rgCID = NULL;
    _rgszName = NULL;
    _nEntries = 0;
}

//+---------------------------------------------------------------------------
//
//  Member:     CEngineList::~CEngineList
//
//  Synopsis:   destructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CEngineList::~CEngineList()
{
    if (_rgCID)
    {
        delete(_rgCID);
    }
    if (_rgszName)
    {
        for (unsigned x = _nEntries; x--;)
        {
            delete[](_rgszName[x]);
        }
        delete[](_rgszName);
    }
    if (_hMenu)
    {
        DestroyMenu(_hMenu);
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CEngineList::Read
//
//  Synopsis:   Reads the list of engines from the registry.
//
//              If there are no entries in the registry for MFract then this
//              routine creates some entries with the default set of
//              fractal engines.
//
//  Returns:    S_OK on success
//              E_FAIL if there was a problem
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CEngineList::Read()
{
    HRESULT hr = S_OK;
    if (FAILED(hr))
    {
        return(hr);
    }
    DWORD dwDisposition;
    HKEY hkSoftware;
    LONG lReturn = RegCreateKeyEx(
        HKEY_LOCAL_MACHINE,
        TEXT("Software"),
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_READ,
        NULL,
        &hkSoftware,
        &dwDisposition);
    if (lReturn == ERROR_SUCCESS)
    {
        HKEY hkMicrosoft;
        lReturn = RegCreateKeyEx(
            hkSoftware,
            TEXT("Microsoft"),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_READ |
            KEY_WRITE,
            NULL,
            &hkMicrosoft,
            &dwDisposition);
        if (lReturn == ERROR_SUCCESS)
        {
            HKEY hkApp;
            lReturn = RegCreateKeyEx(
                hkMicrosoft,
                TEXT(APP_TITLE),
                0,
                NULL,
                REG_OPTION_NON_VOLATILE,
                KEY_READ |
                KEY_WRITE |
                KEY_SET_VALUE,
                NULL,
                &hkApp,
                &dwDisposition);
            if (lReturn == ERROR_SUCCESS)
            {
                HKEY hkFractalTypes;
                lReturn = RegCreateKeyEx(
                    hkApp,
                    TEXT("Fractal Types"),
                    0,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    KEY_READ |
                    KEY_WRITE |
                    KEY_SET_VALUE,
                    NULL,
                    &hkFractalTypes,
                    &dwDisposition);
                if (lReturn == ERROR_SUCCESS)
                {
                    if (dwDisposition == REG_OPENED_EXISTING_KEY)
                    {
                        // Enumerate the values in this key to build our list
                        DWORD cchClass = 0;
                        DWORD cSubkeys;
                        DWORD cchMaxSubkey;
                        DWORD cchMaxClass;
                        DWORD cValues;
                        DWORD cchMaxValueName;
                        DWORD cbMaxValueData;
                        DWORD cbSecurityDescriptor;
                        FILETIME ftLastWriteTime;
                        lReturn = RegQueryInfoKey(
                            hkFractalTypes,
                            NULL,
                            &cchClass,
                            0,
                            &cSubkeys,
                            &cchMaxSubkey,
                            &cchMaxClass,
                            &cValues,
                            &cchMaxValueName,
                            &cbMaxValueData,
                            &cbSecurityDescriptor,
                            &ftLastWriteTime);
                        _rgCID = new CLSID[cValues];
                        _rgszName = new TCHAR *[cValues];
                        for (DWORD uIndex = cValues; uIndex--;)
                        {
                            TCHAR * szCLSID = (TCHAR *) new BYTE [cbMaxValueData];
                            TCHAR * szName = new TCHAR[cchMaxValueName+1];
                            DWORD cchValue = cchMaxValueName+1;
                            DWORD cbData = cbMaxValueData;
                            lReturn = RegEnumValue(
                                hkFractalTypes,
                                uIndex,
                                szName,
                                &cchValue,
                                0,
                                NULL,
                                NULL,
                                &cbData);
                            cchValue++;
                            _rgszName[uIndex] = new TCHAR[cchValue];
                            lReturn = RegEnumValue(
                                hkFractalTypes,
                                uIndex,
                                _rgszName[uIndex],
                                &cchValue,
                                0,
                                NULL,
                                (BYTE *) szCLSID,
                                &cbData);
#ifdef UNICODE
                            CLSIDFromString(szCLSID, &_rgCID[uIndex]);
#else
                            OLECHAR wszCLSID[40]; // should be more than large enough
                            mbstowcs(wszCLSID, szCLSID, 40);
                            CLSIDFromString(wszCLSID, &_rgCID[uIndex]);
#endif
                            delete[] szCLSID;
                            delete[] szName;
                        }
                        _nEntries = cValues;
                    }
                    else
                    {
                        // This is the first time MFract has been run on this machine.
                        // Create the default key entries.
                        _rgCID = new CLSID[4];
                        _rgszName = new TCHAR * [4];
                        TCHAR szCLSID[40];  // more than large enough
#ifndef UNICODE
                        OLECHAR wszCLSID[40];
#endif
#ifdef UNICODE
                        StringFromGUID2(CLSID_MANDELBROT, szCLSID, 40);
#else
                        StringFromGUID2(CLSID_MANDELBROT, wszCLSID, 40);
                        wcstombs(szCLSID, wszCLSID, 40);
#endif
                        lReturn = RegSetValueEx(
                            hkFractalTypes,
                            TEXT("Mandelbrot Fractal"),
                            0,
                            REG_SZ,
                            (BYTE *) szCLSID,
                            sizeof(TCHAR) * (1 +
#ifdef UNICODE
                                wcslen(szCLSID)
#else
                                strlen(szCLSID)
#endif
                                )
                            );
                        memcpy(&_rgCID[0], &CLSID_MANDELBROT, sizeof (CLSID));
                        _rgszName[0] =
#ifdef UNICODE
                        _wcsdup(TEXT("Mandelbrot Fractal"));
#else
                        _strdup(TEXT("Mandelbrot Fractal"));
#endif
#ifdef UNICODE
                        StringFromGUID2(CLSID_JULIASET, szCLSID, 40);
#else
                        StringFromGUID2(CLSID_JULIASET, wszCLSID, 40);
                        wcstombs(szCLSID, wszCLSID, 40);
#endif
                        lReturn = RegSetValueEx(
                            hkFractalTypes,
                            TEXT("Julia Set Fractal"),
                            0,
                            REG_SZ,
                            (BYTE *) szCLSID,
                            sizeof(TCHAR) * (1 +
#ifdef UNICODE
                                wcslen(szCLSID)
#else
                                strlen(szCLSID)
#endif
                                )
                            );
                        memcpy(&_rgCID[1], &CLSID_JULIASET, sizeof (CLSID));
                        _rgszName[1] =
#ifdef UNICODE
                        _wcsdup(TEXT("Julia Set Fractal"));
#else
                        _strdup(TEXT("Julia Set Fractal"));
#endif
#ifdef UNICODE
                        StringFromGUID2(CLSID_PLASMA, szCLSID, 40);
#else
                        StringFromGUID2(CLSID_PLASMA, wszCLSID, 40);
                        wcstombs(szCLSID, wszCLSID, 40);
#endif
                        lReturn = RegSetValueEx(
                            hkFractalTypes,
                            TEXT("Plasma Cloud Fractal"),
                            0,
                            REG_SZ,
                            (BYTE *) szCLSID,
                            sizeof(TCHAR) * (1 +
#ifdef UNICODE
                                wcslen(szCLSID)
#else
                                strlen(szCLSID)
#endif
                                )
                            );
                        memcpy(&_rgCID[2], &CLSID_PLASMA, sizeof (CLSID));
                        _rgszName[2] =
#ifdef UNICODE
                        _wcsdup(TEXT("Plasma Cloud Fractal"));
#else
                        _strdup(TEXT("Plasma Cloud Fractal"));
#endif
#ifdef UNICODE
                        StringFromGUID2(CLSID_RGEN, szCLSID, 40);
#else
                        StringFromGUID2(CLSID_RGEN, wszCLSID, 40);
                        wcstombs(szCLSID, wszCLSID, 40);
#endif
                        lReturn = RegSetValueEx(
                            hkFractalTypes,
                            TEXT("Recursive Generator Fractal"),
                            0,
                            REG_SZ,
                            (BYTE *) szCLSID,
                            sizeof(TCHAR) * (1 +
#ifdef UNICODE
                                wcslen(szCLSID)
#else
                                strlen(szCLSID)
#endif
                                )
                            );
                        memcpy(&_rgCID[3], &CLSID_RGEN, sizeof (CLSID));
                        _rgszName[3] =
#ifdef UNICODE
                        _wcsdup(TEXT("Recursive Generator Fractal"));
#else
                        _strdup(TEXT("Recursive Generator Fractal"));
#endif
                        _nEntries = 4;
                    }
                    RegCloseKey(hkFractalTypes);
                }
                else
                {
                    // couldn't access registry
                    hr = E_FAIL;
                }
                RegCloseKey(hkApp);
            }
            else
            {
                // couldn't access registry
                hr = E_FAIL;
            }
            RegCloseKey(hkMicrosoft);
        }
        else
        {
            // couldn't access registry
            hr = E_FAIL;
        }
        RegCloseKey(hkSoftware);
    }
    else
    {
        // couldn't access registry
        hr = E_FAIL;
    }
    if (_hMenu)
    {
        DestroyMenu(_hMenu);
    }
    _hMenu = CreatePopupMenu();
    if (_hMenu)
    {
        for (unsigned u=_nEntries; u--;)
        {
            if (!InsertMenu(
                _hMenu,
                0,
                MF_BYPOSITION | (MF_CHECKED * (0 == u)),
                IDM_FRACTAL_TYPE + u,
                _rgszName[u]))
            {
                // Insert Menu Failed
                hr = E_FAIL;
            }
        }
        TCHAR szMenuString[MAX_STRING_LENGTH];
        GetMenuString(_hmenuOuter,
            IDM_SELECT_TYPE,
            szMenuString,
            MAX_STRING_LENGTH,
            MF_BYCOMMAND);
        if (!ModifyMenu(
            _hmenuOuter,
            IDM_SELECT_TYPE,
            MF_BYCOMMAND | MF_POPUP,
            (UINT)_hMenu,
            szMenuString))
        {
            // ModifyMenu failed
            hr = E_FAIL;
        }
    }
    else
    {
        // couldn't create menu
        hr = E_FAIL;
    }
    return(hr);
}

//+---------------------------------------------------------------------------
//
//  Member:     CEngineList::GetEntry
//
//  Synopsis:   returns the CLSID of the fractal engine at a given index
//
//  Arguments:  [uIndex] - index of the engine
//
//  Returns:    CLSID of the fractal engine
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CLSID CEngineList::GetEntry(unsigned uIndex)
{
    return(_rgCID[uIndex]);
}

//+---------------------------------------------------------------------------
//
//  Member:     CEngineList::FindEntry
//
//  Synopsis:   Looks through the list of CLSIDs to find out what engine number
//              has this CLSID.
//
//  Arguments:  [cid] - CLSID of the fractal engine
//
//  Returns:    >= 0 if the CLSID was found
//              -1   if the CLSID was not found
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

int CEngineList::FindEntry(CLSID cid)
{
    for (unsigned u = _nEntries; u--;)
    {
        if (IsEqualGUID(cid, _rgCID[u]))
        {
            return((int) u);
        }
    }
    // not found in the list
    return(-1);
}

