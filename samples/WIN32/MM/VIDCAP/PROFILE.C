/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/****************************************************************************
 *
 *   profile.c: Stores profile info in the Registry 
 *
 *   Vidcap32 Source code
 *
 ***************************************************************************/

/*
 * win32/win16 utility functions to read and write profile items
 * for multimedia tools
 */

#include <windows.h>
#include <windowsx.h>

#ifdef _WIN32
#define KEYNAME     "Software\\Microsoft\\Multimedia Tools\\"
#define ROOTKEY     HKEY_CURRENT_USER
#else
#define INIFILE    "mmtools.ini"
#endif


/*
 * read a BOOL flag from the profile, or return default if
 * not found.
 */
BOOL
mmGetProfileFlag(LPSTR appname, LPSTR valuename, BOOL bDefault)
{
#ifdef _WIN32
    char achName[MAX_PATH];
    HKEY hkey;
    DWORD dwType;
    BOOL bValue = bDefault;
    DWORD dwData;
    int cbData;


    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegOpenKey(ROOTKEY, achName, &hkey) != ERROR_SUCCESS) {
        return(bDefault);
    }

    cbData = sizeof(dwData);
    if (RegQueryValueEx(
        hkey,
        valuename,
        NULL,
        &dwType,
        (PBYTE) &dwData,
        &cbData) == ERROR_SUCCESS) {
            if (dwType == REG_DWORD) {
                if (dwData) {
                    bValue = TRUE;
                } else {
                    bValue = FALSE;
                }
            }
    }

    RegCloseKey(hkey);

    return(bValue);
#else
    char ach[10];

    GetPrivateProfileString(appname, valuename, "X", ach, sizeof(ach),
            INIFILE);

    switch(ach[0]) {
    case 'N':
    case 'n':
    case '0':
        return(FALSE);

    case 'Y':
    case 'y':
    case '1':
        return(TRUE);

    default:
        return(bDefault);
    }
#endif
}


/*
 * write a boolean value to the registry, if it is not the
 * same as the default or the value already there
 */
VOID
mmWriteProfileFlag(LPSTR appname, LPSTR valuename, BOOL bValue, BOOL bDefault)
{
    if (mmGetProfileFlag(appname, valuename, bDefault) == bValue) {
        return;
    }

#ifdef _WIN32
    {
        char achName[MAX_PATH];
        HKEY hkey;

        lstrcpy(achName, KEYNAME);
        lstrcat(achName, appname);
        if (RegCreateKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {
            RegSetValueEx(
                hkey,
                valuename,
                0,
                REG_DWORD,
                (PBYTE) &bValue,
                sizeof(bValue)
            );

            RegCloseKey(hkey);
        }
    }

#else
    WritePrivateProfileString(
        appname,
        valuename,
        bValue ? "1" : "0",
        INIFILE);
#endif
}

/*
 * read a UINT from the profile, or return default if
 * not found.
 */
UINT
mmGetProfileInt(LPSTR appname, LPSTR valuename, UINT uDefault)
{
#ifdef _WIN32
    char achName[MAX_PATH];
    HKEY hkey;
    DWORD dwType;
    UINT value = uDefault;
    DWORD dwData;
    int cbData;


    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegOpenKey(ROOTKEY, achName, &hkey) != ERROR_SUCCESS) {
        return(uDefault);
    }

    cbData = sizeof(dwData);
    if (RegQueryValueEx(
        hkey,
        valuename,
        NULL,
        &dwType,
        (PBYTE) &dwData,
        &cbData) == ERROR_SUCCESS) {
            if (dwType == REG_DWORD) {
                value = (UINT)dwData;
            }
    }

    RegCloseKey(hkey);

    return(value);
#else
    return(GetPrivateProfileInt(appname, valuename, uDefault, INIFILE);
#endif
}


/*
 * write a UINT to the profile, if it is not the
 * same as the default or the value already there
 */
VOID
mmWriteProfileInt(LPSTR appname, LPSTR valuename, UINT uValue, UINT uDefault)
{
    if (mmGetProfileInt(appname, valuename, uDefault) == uValue) {
        return;
    }

#ifdef _WIN32
    {
        char achName[MAX_PATH];
        HKEY hkey;
        DWORD dwData = uValue;

        lstrcpy(achName, KEYNAME);
        lstrcat(achName, appname);
        if (RegCreateKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {
            RegSetValueEx(
                hkey,
                valuename,
                0,
                REG_DWORD,
                (PBYTE) &dwData,
                sizeof(dwData)
            );

            RegCloseKey(hkey);
        }
    }

#else
    char ach[12];

    wsprintf(ach, "%d", uValue);

    WritePrivateProfileString(
        appname,
        valuename,
        ach,
        INIFILE);
#endif
}


/*
 * read a string from the profile into pResult.
 * result is number of bytes written into pResult
 */
DWORD
mmGetProfileString(
    LPSTR appname,
    LPSTR valuename,
    LPSTR pDefault,
    LPSTR pResult,
    int cbResult
)
{
#ifdef _WIN32
    char achName[MAX_PATH];
    HKEY hkey;
    DWORD dwType;


    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegOpenKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {

        if (RegQueryValueEx(
            hkey,
            valuename,
            NULL,
            &dwType,
            pResult,
            &cbResult) == ERROR_SUCCESS) {

                if (dwType == REG_SZ) {
                    // cbResult is set to the size including null
                    RegCloseKey(hkey);
                    return(cbResult - 1);
                }
        }


        RegCloseKey(hkey);
    }

    // if we got here, we didn't find it, or it was the wrong type - return
    // the default string
    lstrcpy(pResult, pDefault);
    return(lstrlen(pDefault));

#else
    return GetPrivateProfileString(
                appname,
                valuename,
                pDefault,
                pResult,
                cbResult
                INIFILE);
#endif
}


/*
 * write a string to the profile
 */
VOID
mmWriteProfileString(LPSTR appname, LPSTR valuename, LPSTR pData)
{
#ifdef _WIN32
    char achName[MAX_PATH];
    HKEY hkey;

    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegCreateKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {
        RegSetValueEx(
            hkey,
            valuename,
            0,
            REG_SZ,
            pData,
            lstrlen(pData) + 1
        );

        RegCloseKey(hkey);
    }

#else
    WritePrivateProfileString(
        appname,
        valuename,
        pData,
        INIFILE);
#endif
}

/*
 * read binary values from the profile into pResult.
 * result is number of bytes written into pResult
 */
DWORD
mmGetProfileBinary(
    LPSTR appname,
    LPSTR valuename,
    LPVOID pDefault,  
    LPVOID pResult,   // if NULL, return the required size
    int cbSize
)
{
    char achName[MAX_PATH];
    HKEY hkey;
    DWORD dwType;
    int cbResult = cbSize;

    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegOpenKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {

        if (RegQueryValueEx(
            hkey,
            valuename,
            NULL,
            &dwType,
            pResult,
            &cbResult) == ERROR_SUCCESS) {

                if (dwType == REG_BINARY) {
                    // cbResult is the size
                    RegCloseKey(hkey);
                    return(cbResult);
                }
        }


        RegCloseKey(hkey);
    }

    // if we got here, we didn't find it, or it was the wrong type - return
    // the default values (use MoveMemory, since src could equal dst)
    MoveMemory (pResult, pDefault, cbSize);
    return cbSize;

}


/*
 * write binary data to the profile
 */
VOID
mmWriteProfileBinary(LPSTR appname, LPSTR valuename, LPVOID pData, int cbData)
{
    char achName[MAX_PATH];
    HKEY hkey;

    lstrcpy(achName, KEYNAME);
    lstrcat(achName, appname);
    if (RegCreateKey(ROOTKEY, achName, &hkey) == ERROR_SUCCESS) {
        RegSetValueEx(
            hkey,
            valuename,
            0,
            REG_BINARY,
            pData,
            cbData
        );

        RegCloseKey(hkey);
    }
}

