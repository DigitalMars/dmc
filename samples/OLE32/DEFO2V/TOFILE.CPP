// tofile.cpp
//
// Dumps a TypeLib or a TypeInfo to a file
//

#include "precomp.h"
#include "defo2v.h"
#include "idisp.h"
#include "tofile.h"
#include <stdlib.h>
#include <commdlg.h>
#include <math.h>

#if defined(WIN32) && !defined(_UNICODE)
    #pragma message("Building ANSI version of " __FILE__)
#endif

static BOOL StringToFile( HFILE hf, LPCTSTR psz )
{
    int cc = lstrlen(psz) ;
#ifndef WIN32
    return _lwrite( hf, psz, cc ) != HFILE_ERROR ;
#else
    DWORD   dw ;

    #ifdef _UNICODE
    char*   sz = new char[cc+1] ;
    cc = WideCharToMultiByte( CP_ACP, 0, psz, cc, sz, cc, NULL, NULL ) ;
    BOOL f = WriteFile((HANDLE)hf, sz, cc, &dw, NULL ) ;
    delete [] sz ;
    return f ;
    #else
    return WriteFile((HANDLE)hf, psz, cc, &dw, NULL ) ;
    #endif
#endif
}

#ifdef WIN32
#define CLOSEFILE(h)      CloseHandle((HANDLE)h)
#else
#define CLOSEFILE(h)      _lclose(h)
#endif

static TCHAR szNotAvail[] = _T("(not available)") ;

#define BSTR_PRT(bstr) (LPOLESTR)((bstr != NULL && SysStringLen(bstr)) ? (LPOLESTR)bstr : (LPOLESTR)szNotAvail)

static BOOL TypeInfoToFile( HFILE  hfile, LPTYPEINFO lpTypeInfo ) ;

static LPTSTR BuildParam( LPTSTR sz, LPELEMDESC lpelemdesc, LPTSTR lpszName, BOOL fOptional ) ;

static HFILE GetFileToSave( HWND hwnd, LPTSTR szTitle )
{
    OPENFILENAME ofn;
    TCHAR szFile[256], szFileTitle[256];
    UINT  i, cbString;
    TCHAR  chReplace;    /* string separator for szFilter */
    HFILE hf = HFILE_ERROR ;
    static TCHAR  szFilter[] = _T("Text Files (*.txt)|*.txt|") ;

    cbString = lstrlen( szFilter ) ;
    chReplace = szFilter[cbString - 1]; /* retrieve wildcard */
    for (i = 0; szFilter[i] != _T('\0'); i++)
    {
        if (szFilter[i] == chReplace)
           szFilter[i] = _T('\0');
    }

    /* Set all structure members to zero. */
    _fmemset(&ofn, 0, sizeof(OPENFILENAME));

    /* Initialize the OPENFILENAME members. */
    szFile[0] = '\0';

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = szFilter;
    ofn.lpstrFile= szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrTitle = szTitle ;

    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL ;
    ofn.Flags =     OFN_SHOWHELP |
                    OFN_OVERWRITEPROMPT |
                    OFN_HIDEREADONLY ;

    if (GetSaveFileName(&ofn))
    {
        #ifndef WIN32
        OFSTRUCT of ;
        hf = OpenFile( ofn.lpstrFile, &of, OF_CREATE ) ;
        #else
        hf = (HFILE)CreateFile( ofn.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
        #endif
    }
    return hf ;

}

BOOL DoTypeInfoToFile( HWND hwnd, LPTYPEINFO lpTypeInfo )
{
    HFILE hf = GetFileToSave( hwnd, _T("Save TypeInfo to File") ) ;
    if (hf == HFILE_ERROR)
        return FALSE ;

    HCURSOR hcur = SetCursor(LoadCursor( NULL, IDC_WAIT )) ;
    TypeInfoToFile( hf, lpTypeInfo ) ;
    SetCursor( hcur ) ;
    CLOSEFILE( hf ) ;

    return TRUE ;
}

BOOL DoTypeLibToFile( HWND hwnd, LPTYPELIB lpTypeLib )
{
    HRESULT     hr ;
    LPTYPEINFO  lpTypeInfo ;
    HFILE       hf ;

    hf = GetFileToSave( hwnd, _T("Save TypeLib to File") ) ;
    #ifdef WIN32
    if (hf == 0)
        return FALSE ;
    #else
    if (hf == HFILE_ERROR)
        return FALSE ;
    #endif

    HCURSOR hcur = SetCursor(LoadCursor( NULL, IDC_WAIT )) ;

    TLIBATTR FAR* pTlibAttr ;
    lpTypeLib->GetLibAttr(&pTlibAttr ) ;

    BSTR           bstrName = NULL ;
    BSTR           bstrDoc = NULL ;
    BSTR           bstrHelp = NULL ;
    DWORD           dwHelpID ;
    hr = lpTypeLib->GetDocumentation( MEMBERID_NIL, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
    if (SUCCEEDED(hr))
    {

        TCHAR           szBuf[256] ;

        wsprintf( szBuf,    _T("'===============================================================\r\n") ) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("' Type Library: %ws, Library Version %d.%03d\r\n"),
                            BSTR_PRT(bstrName), pTlibAttr->wMajorVerNum, pTlibAttr->wMinorVerNum ) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("' GUID: {%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\r\n"),
                            pTlibAttr->guid.Data1, pTlibAttr->guid.Data2, pTlibAttr->guid.Data3,
                            pTlibAttr->guid.Data4[0], pTlibAttr->guid.Data4[1],
                            pTlibAttr->guid.Data4[2], pTlibAttr->guid.Data4[3],
                            pTlibAttr->guid.Data4[4], pTlibAttr->guid.Data4[5],
                            pTlibAttr->guid.Data4[6], pTlibAttr->guid.Data4[7]) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("' LCID: %#08X\r\n"), (DWORD)pTlibAttr->lcid ) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("' Documentation: %ws\r\n"), BSTR_PRT(bstrDoc) ) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("' Help: %ws (Help ID: %#08lX)\r\n"), BSTR_PRT(bstrHelp), (DWORD)dwHelpID ) ;
        StringToFile( hf, szBuf ) ;

        wsprintf( szBuf,    _T("'===============================================================\r\n\r\n") ) ;
        StringToFile( hf, szBuf ) ;

        SysFreeString( bstrName ) ;
        SysFreeString( bstrDoc ) ;
        SysFreeString( bstrHelp ) ;

        lpTypeLib->ReleaseTLibAttr( pTlibAttr ) ;
    }
    else
    {
        lpTypeLib->ReleaseTLibAttr( pTlibAttr ) ;
        TCHAR szBuf[128] ;
        wsprintf( szBuf, _T("GetDocumentation for TypeInfo FAILED: %s"), HRtoString( hr ) ) ;
        StringToFile( hf, szBuf ) ;
        return FALSE ;
    }

    UINT uiTypeInfoCount = lpTypeLib->GetTypeInfoCount() ;
    for (UINT n = 0 ; n < uiTypeInfoCount ; n++)
    {


        hr = lpTypeLib->GetTypeInfo( n, &lpTypeInfo ) ;
        if (SUCCEEDED(hr))
        {
            TypeInfoToFile( hf, lpTypeInfo ) ;
            lpTypeInfo->Release() ;
        }
        else
        {
            TCHAR szBuf[128] ;
            wsprintf( szBuf, _T("ERROR: Could not get TypeInfo %d: %s\r\n"), n, (LPTSTR)HRtoString(hr) ) ;
            StringToFile( hf, szBuf) ;
        }
    }

    SetCursor( hcur ) ;
    CLOSEFILE( hf ) ;
    return TRUE ;
}

// Given a pointer to a TypeInfo and a handle to an open file, write
// all functions, variables, and constants.   The file is written in
// a form such that it looks like a VBA "include" file.
//
// A header is written first:
//
// '
// ' Type Information for the <wordbasic> TypeInfo
// ' Documentation: <docstring>
// ' Help for this TypeInfo can be found in <helpfile>
// ' The help ID for this TypeInfo is <helpid>
//
// Functions appear in the following form:
//
// '
// ' <funcname>
// ' Documentation: <docstring>
// ' Help for this function can be found in <helpfile>
// ' The help ID for this function is <helpid>
// Declare Function <funcname>(<arglist>) As <returntype>
//
// Subroutines (functions without return types) appear as:
// '
// ' <funcname>
// ' Documentation: <docstring>
// ' Help for this subroutine can be found in <helpfile>
// ' The help ID for this subroutine is <helpid>
// Declare Sub <funcname>(<arglist>)
//
// Properties/Variables appear as:
//
// '
// ' <varname>
// ' Documentation: <docstring>
// ' Help for this property can be found in <helpfile>
// ' The help ID for this property is <helpid>
// Dim <varname> As <vartype>
//
// Constants appear as:
//
// '
// ' <varname>
// ' Documentation: <docstring>
// ' Help for this property can be found in <helpfile>
// ' The help ID for this property is <helpid>
// Const <varname> As <vartype> = <value>
//
static BOOL TypeInfoToFile( HFILE  hfile, LPTYPEINFO lpTypeInfo )
{
    TCHAR szBuf[512] ;

//    Assert( hfile != HFILE_ERROR ) ;
//    Assert( lpTypeInfo ) ;

    #define MAX_NAMES   128

    HRESULT         hr ;
    BSTR           bstrName = NULL ;
    BSTR           bstrDoc = NULL ;
    BSTR           bstrHelp = NULL ;
    BSTR           rgbstrNames[MAX_NAMES] ;
    DWORD           dwHelpID ;
    int             cNames ;
    WORD            cFuncs, cVars ;
    LPFUNCDESC      pFuncDesc ;
    WORD            iIndex ;

    LPTYPEATTR  pTA = NULL ;

    hr = lpTypeInfo->GetTypeAttr( &pTA ) ;
    if (FAILED( hr ))
    {
        wsprintf( szBuf, _T("GetTypeAttr FAILED: %s"), (LPTSTR)HRtoString( hr ) ) ;
        StringToFile( hfile, szBuf ) ;
        return FALSE ;
    }

    hr = lpTypeInfo->GetDocumentation( MEMBERID_NIL, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
    if (SUCCEEDED(hr))
    {
        wsprintf( szBuf,    _T("'===============================================================\r\n") ) ;
        StringToFile( hfile, szBuf ) ;

        wsprintf( szBuf,    _T("' Type Info: %ws, TypeInfo Version %d.%03d\r\n"),
                            BSTR_PRT(bstrName), pTA->wMajorVerNum, pTA->wMinorVerNum ) ;
        StringToFile( hfile, szBuf ) ;

        wsprintf( szBuf,    _T("' GUID: {%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\r\n"),
                            pTA->guid.Data1, pTA->guid.Data2, pTA->guid.Data3,
                            pTA->guid.Data4[0], pTA->guid.Data4[1],
                            pTA->guid.Data4[2], pTA->guid.Data4[3],
                            pTA->guid.Data4[4], pTA->guid.Data4[5],
                            pTA->guid.Data4[6], pTA->guid.Data4[7]);
        StringToFile( hfile, szBuf ) ;

        wsprintf( szBuf,    _T("' LCID: %#08X\r\n"), (DWORD)pTA->lcid ) ;
        StringToFile( hfile, szBuf ) ;

        wsprintf( szBuf,    _T("' TypeKind: %s\r\n"), (LPTSTR)TYPEKINDtoString(pTA->typekind) ) ;
        StringToFile( hfile, szBuf ) ;

        if (bstrDoc && SysStringLen(bstrDoc))
        {
            wsprintf( szBuf,    _T("' Documentation: %ws\r\n"), bstrDoc ) ;
            StringToFile( hfile, szBuf ) ;
        }

        if (bstrHelp && SysStringLen(bstrHelp))
        {
            wsprintf( szBuf,    _T("' Help: %ws (Help ID: %#08lX)\r\n"), bstrHelp,  (DWORD)dwHelpID ) ;
            StringToFile( hfile, szBuf ) ;
        }

        wsprintf( szBuf,    _T("'---------------------------------------------------------------\r\n\r\n") ) ;
        StringToFile( hfile, szBuf ) ;

        SysFreeString( bstrName ) ;
        SysFreeString( bstrDoc ) ;
        SysFreeString( bstrHelp ) ;
    }
    else
    {
        wsprintf( szBuf, _T("GetDocumentation for TypeInfo FAILED: %s"), (LPTSTR)HRtoString( hr ) ) ;
        StringToFile( hfile, szBuf ) ;
        return FALSE ;
    }

    cVars = pTA->cVars ;
    cFuncs = pTA->cFuncs ;
    lpTypeInfo->ReleaseTypeAttr( pTA ) ;

    int      cParams ;
    int      cParamsOpt ;
    TCHAR    szFunc[128] ;   // Function/Sub name
    LPTSTR   szParams ;
    LPTSTR   lpsz ;

    //
    // Enumerate through all FUNCDESCS
    for ( iIndex = 0 ; iIndex < cFuncs ; iIndex++)
    {
        hr = lpTypeInfo->GetFuncDesc( iIndex, &pFuncDesc ) ;
        if (FAILED(hr))
        {
            wsprintf( szBuf, _T("GetFuncDesc FAILED for function #%u (%s)\r\n"), iIndex, (LPTSTR)HRtoString( hr ) ) ;
            StringToFile( hfile, szBuf ) ;
            continue ;
        }

        hr = lpTypeInfo->GetDocumentation( pFuncDesc->memid, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
        if (SUCCEEDED(hr))
        {
            wsprintf( szBuf, _T("' Function: %ws\r\n"), BSTR_PRT(bstrName) ) ;
            StringToFile( hfile, szBuf ) ;

            if (bstrDoc && SysStringLen(bstrDoc))
            {
                wsprintf( szBuf, _T("' Documentation: %ws\r\n"), bstrDoc ) ;
                StringToFile( hfile, szBuf ) ;
            }

            if (bstrHelp && SysStringLen(bstrHelp))
            {
                wsprintf( szBuf, _T("' Help: %ws (Help ID: %#08lX)\r\n"), bstrHelp, (DWORD)dwHelpID ) ;
                StringToFile( hfile, szBuf ) ;
            }

            lstrcpy( szBuf, _T("' \r\n") ) ;
            StringToFile( hfile, szBuf ) ;

            SysFreeString( bstrName ) ;
            SysFreeString( bstrDoc ) ;
            SysFreeString( bstrHelp ) ;

        }

        cParams = pFuncDesc->cParams ;
        cParamsOpt = abs(pFuncDesc->cParamsOpt) ;

        // Get the names of the function and it's parameters into rgbstrNames.
        // cNames gets the number of parameters + 1.
        //
        hr = lpTypeInfo->GetNames( pFuncDesc->memid, rgbstrNames, MAX_NAMES, (UINT FAR*)&cNames );
        if (SUCCEEDED( hr ))
        {
            // rgbstrNames[0] is the name of the function
            if (cNames > 0)
            {
#if defined(WIN32) && !defined(UNICODE)
                wcstombs(szFunc, rgbstrNames[0], 128) ;
#else
                lstrcpy( szFunc, rgbstrNames[0] ) ;
#endif
                SysFreeString( rgbstrNames[0] ) ;

            }

            // Allocate a string buffer that should be able to hold
            // all of our parameter types and names.
            //
            if ((szParams = (LPTSTR) GlobalAllocPtr( GPTR, max(cNames,cParams) * (64) )) == NULL )
            {
                MessageBox( NULL, _T("GlobalAlloc Failed!"), _T("Yikes!"), MB_OK ) ;
                wsprintf( szBuf, _T("GlobalAlloc failed!\r\n") ) ;
                StringToFile( hfile, szBuf ) ;
                return FALSE ;
            }

            // For each parameter get the type and name.
            // The "max(cNames-1,cParams)" should handle the case
            // where a function has optional parameters (i.e. cParamsOpt is
            // non-zero).
            //
            lstrcpy( szParams, _T("(") ) ;

            // For each required parameter
            //
            for ( int n = 0 ; n < cParams - cParamsOpt ; n++ )
            {
#if defined(WIN32) && !defined(UNICODE)
                TCHAR szTemp[128];
#endif
                if (n+1 < cNames)
#if defined(WIN32) && !defined(UNICODE)
                {
                    wcstombs(szTemp, rgbstrNames[n+1], 128);
                    lpsz = szTemp;
                }
#else
                    lpsz = rgbstrNames[n+1] ;
#endif
                else
                    lpsz = NULL ;
                BuildParam( szParams+lstrlen(szParams),
                            &pFuncDesc->lprgelemdescParam[n], lpsz , FALSE ) ;

                if (n+1 < cNames)
                    SysFreeString( rgbstrNames[n+1] ) ;


                // If it's the last one then no comma
                if (n + 1 != max(cNames-1,cParams))
                    lstrcat( szParams, _T(", ") ) ;
            }

            // For each optional parameter
            //
            for (n = cParams - cParamsOpt ; n < cParams ; n++)
            {
#if defined(WIN32) && !defined(UNICODE)
                TCHAR szTemp[128];
#endif
                if (n+1 < cNames)
#if defined(WIN32) && !defined(UNICODE)
                {
                    wcstombs(szTemp, rgbstrNames[n+1], 128);
                    lpsz = szTemp;
                }
#else
                    lpsz = rgbstrNames[n+1] ;
#endif
                else
                    lpsz = NULL ;
                BuildParam( szParams+lstrlen(szParams),
                            &pFuncDesc->lprgelemdescParam[n], lpsz , TRUE ) ;

                if (n+1 < cNames)
                    SysFreeString( rgbstrNames[n+1] ) ;


                // If it's the last one then no comma
                if (n + 1 != max(cNames-1,cParams))
                    lstrcat( szParams,  _T(", ") ) ;
            }

            lstrcat( szParams , _T(")")  );

            // Is it a function or sub?
            //
            if (pFuncDesc->elemdescFunc.tdesc.vt == VT_EMPTY ||
                pFuncDesc->elemdescFunc.tdesc.vt == VT_NULL ||
                pFuncDesc->elemdescFunc.tdesc.vt == VT_VOID)
            {
                // Declare Sub <subname>(<arglist>)
                wsprintf( szBuf, _T("Declare Sub %s "), (LPTSTR)szFunc ) ;
                StringToFile( hfile, szBuf) ;
                StringToFile( hfile, szParams) ;
                StringToFile( hfile, _T("\r\n\r\n") ) ;
            }
            else
            {
                // Declare Function <funcname>(<arglist>) As <returntype>
                wsprintf( szBuf, _T("Declare Function %s "), (LPTSTR)szFunc ) ;
                StringToFile( hfile, szBuf) ;
                StringToFile( hfile, szParams) ;
                wsprintf( szBuf, _T(" As %s\r\n\r\n"), (LPTSTR)VTtoString2( pFuncDesc->elemdescFunc.tdesc.vt ) ) ;
                StringToFile( hfile, szBuf) ;
            }

            GlobalFreePtr( szParams ) ;
        }
        else
        {
            wsprintf( szBuf, _T("GetNames (%lu) FAILED: %s"), pFuncDesc->memid, (LPTSTR)HRtoString( hr ) ) ;
            StringToFile( hfile, szBuf ) ;
        }
        lpTypeInfo->ReleaseFuncDesc( pFuncDesc ) ;
    }

    // '
    // ' <varname>
    // ' Documentation: <docstring>
    // ' Help for this property can be found in <helpfile>
    // ' The help ID for this property is <helpid>
    // Dim <varname> As <vartype>
    // '
    // ' <varname>
    // ' Documentation: <docstring>
    // ' Help for this property can be found in <helpfile>
    // ' The help ID for this property is <helpid>
    // Const <varname> As <vartype> = <value>
    LPVARDESC pVarDesc ;
    for (iIndex = 0 ; iIndex < cVars ; iIndex++)
    {
        hr = lpTypeInfo->GetVarDesc( iIndex, &pVarDesc ) ;
        if (FAILED(hr))
        {
            wsprintf( szBuf, _T("GetVarDesc FAILED for variable #%u (%s)\r\n"), iIndex, (LPTSTR)HRtoString( hr ) ) ;
            StringToFile( hfile, szBuf ) ;
            continue ;
        }
        else
        {
            hr = lpTypeInfo->GetDocumentation( pVarDesc->memid, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
            if (SUCCEEDED(hr))
            {
                wsprintf( szBuf,     _T("' Variable/Constant: %ws\r\n"), BSTR_PRT(bstrName) ) ;
                StringToFile( hfile, szBuf ) ;

                if (bstrDoc && SysStringLen(bstrDoc))
                {
                    wsprintf( szBuf, _T("' Documentation: %ws\r\n"), bstrDoc ) ;
                    StringToFile( hfile, szBuf ) ;
                }

                if (bstrHelp && SysStringLen(bstrHelp))
                {
                    wsprintf( szBuf, _T("' Help: %ws (Help ID: %#08lX)\r\n"), bstrHelp, (DWORD)dwHelpID ) ;
                    StringToFile( hfile, szBuf ) ;
                }
                lstrcpy( szBuf, _T("' \r\n") ) ;
                StringToFile( hfile, szBuf ) ;
                SysFreeString( bstrName ) ;
                SysFreeString( bstrDoc ) ;
                SysFreeString( bstrHelp ) ;

            }
            hr = lpTypeInfo->GetNames( pVarDesc->memid, rgbstrNames, 1, (UINT FAR*)&cNames );
            if (SUCCEEDED( hr ))
            {

                if (pVarDesc->varkind == VAR_CONST)
                {
                    // Const constname [As type] = expression
                    wsprintf( szBuf, _T("Const %ws As %s = "),
                                        rgbstrNames[0],
                                        TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc )
                                     ) ;
                    VARIANT varValue ;
                    VariantInit( &varValue ) ;
                    hr = VariantChangeType( &varValue, pVarDesc->lpvarValue, 0, VT_BSTR ) ;
                    if (FAILED(hr))
                        lstrcat( szBuf, (LPTSTR)HRtoString( hr ) ) ;
                    else
                    {
#if defined(WIN32) && !defined(UNICODE)
                        wsprintf(&szBuf[strlen(szBuf)], "%ws", varValue.bstrVal);
#else
                        lstrcat( szBuf, varValue.bstrVal ) ;
#endif
                        SysFreeString( varValue.bstrVal ) ;

                    }
                }
                else
                {
                    // Dim varname[([subscripts])][As type]
                    wsprintf( szBuf, _T("Dim %ws "), (rgbstrNames[0] ? rgbstrNames[0]: OLESTR("(name less)")) ) ;
                    if ((pVarDesc->elemdescVar.tdesc.vt & 0x0FFF) == VT_PTR &&
                        (pVarDesc->elemdescVar.tdesc.lptdesc->vt & 0x0FFF) == VT_SAFEARRAY)
                    {
                        lstrcat( szBuf, _T("() As ") ) ;
                        lstrcat( szBuf, VTtoString2( pVarDesc->elemdescVar.tdesc.vt ) ) ;
                    }
                    else
                    {
                        lstrcat( szBuf, _T("As ") ) ;
                        lstrcat( szBuf, TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc ) ) ;
                    }
                }
                SysFreeString( rgbstrNames[0] ) ;

                lstrcat( szBuf, _T("\r\n\r\n") ) ;
                StringToFile( hfile, szBuf ) ;
            }
            else
            {
                wsprintf( szBuf, _T("GetNames (%lu) FAILED: %s\r\n"), pVarDesc->memid, (LPTSTR)HRtoString( hr ) ) ;
                StringToFile( hfile, szBuf ) ;
            }

            lpTypeInfo->ReleaseVarDesc( pVarDesc ) ;
        }
    }

    return TRUE ;
}

static LPTSTR BuildParam( LPTSTR sz, LPELEMDESC lpelemdesc, LPTSTR lpszName, BOOL fOptional )
{
    // [Optional][ByVal|ByRef][ParamArray] varname [()] As type

    if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR &&
        (lpelemdesc->tdesc.lptdesc->vt & 0x0FFF) == VT_SAFEARRAY)
    {
        lstrcat( sz, _T("ParamArray ") ) ;
    }
    else
    {
        if (fOptional)
            lstrcpy( sz, _T("Optional ") ) ;

        if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR)
            lstrcat( sz, _T("ByRef ") ) ;
        else
            lstrcat( sz, _T("ByVal ") ) ;
    }

    if (lpszName != NULL)
        lstrcat( sz, lpszName ) ;

    if (lpszName != NULL)
        lstrcat( sz, _T(" As ") ) ;
    else
        lstrcat( sz, _T(" ") ) ;

    if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR)
        lstrcat( sz, _T("Variant") ) ;
    else
    {
        lstrcat( sz, TYPEDESCtoString( &lpelemdesc->tdesc ) ) ;
    }
    return sz ;
}

