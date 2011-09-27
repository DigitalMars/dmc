#include "precomp.h"
#include "defo2v.h"
#include "idisp.h"
#include <commdlg.h>
#include <math.h>

#include "debug.h"
ASSERTDATA

BOOL TypeInfoToFile( HFILE  hfile, LPTYPEINFO lpTypeInfo ) ;
LPSTR BuildParam( LPSTR sz, LPELEMDESC lpelemdesc, LPSTR lpszName, BOOL fOptional ) ;

HFILE GetFileToSave( HWND hwnd, LPSTR szTitle )
{
	OPENFILENAME ofn;
	char szFile[256], szFileTitle[256];
	UINT  i, cbString;
	char  chReplace;    /* string separator for szFilter */
	HFILE hf = HFILE_ERROR ;
	static char  szFilter[] = "Text Files (*.txt)|*.txt|" ;

	cbString = lstrlen( szFilter ) ;
	chReplace = szFilter[cbString - 1]; /* retrieve wildcard */
	for (i = 0; szFilter[i] != '\0'; i++)
	{
		if (szFilter[i] == chReplace)
		   szFilter[i] = '\0';
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
		OFSTRUCT of ;
		hf = OpenFile( ofn.lpstrFile, &of, OF_CREATE ) ;
	}

	return hf ;

}

BOOL DoTypeInfoToFile( HWND hwnd, LPTYPEINFO lpTypeInfo )
{
	HFILE hf = GetFileToSave( hwnd, "Save TypeInfo to File" ) ;
	if (hf == HFILE_ERROR)
		return FALSE ;

	HCURSOR hcur = SetCursor(LoadCursor( NULL, IDC_WAIT )) ;
	TypeInfoToFile( hf, lpTypeInfo ) ;
	SetCursor( hcur ) ;
	_lclose( hf ) ;

	return TRUE ;
}

BOOL DoTypeLibToFile( HWND hwnd, LPTYPELIB lpTypeLib )
{
	HRESULT     hr ;
	LPTYPEINFO  lpTypeInfo ;
	HFILE       hf ;

	hf = GetFileToSave( hwnd, "Save TypeLib to File" ) ;
	if (hf == HFILE_ERROR)
		return FALSE ;

	HCURSOR hcur = SetCursor(LoadCursor( NULL, IDC_WAIT )) ;

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
			char szBuf[128] ;
			wsprintf( szBuf, "ERROR: Could not get TypeInfo %d: %s\r\n", n, (LPSTR)HRtoString(hr) ) ;
			_lwrite( hf, szBuf, lstrlen(szBuf) ) ;
		}
	}

	SetCursor( hcur ) ;
	_lclose( hf ) ;
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
BOOL TypeInfoToFile( HFILE  hfile, LPTYPEINFO lpTypeInfo )
{
	char szBuf[512] ;

	Assert( hfile != HFILE_ERROR ) ;
	Assert( lpTypeInfo ) ;

	#define MAX_NAMES   128

	HRESULT         hr ;
	BSTR            bstrName, bstrDoc, bstrHelp ;
	DWORD           dwHelpID ;
	BSTR            rgbstrNames[MAX_NAMES] ;
	int             cNames ;
	WORD            cFuncs, cVars ;
	LPFUNCDESC      pFuncDesc ;
	WORD            iIndex ;

	hr = lpTypeInfo->GetDocumentation( MEMBERID_NIL, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
	if (SUCCEEDED(hr))
	{
		wsprintf( szBuf,    "' ---------------------------------------------------------------\r\n"\
							"' %s\r\n"\
							"'   %s\r\n\r\n",
				  (LPSTR)(bstrName ? bstrName : ""),
				  (LPSTR)(bstrDoc ? bstrDoc : "") ) ;

		_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;

		SysFreeString( bstrName ) ;
		SysFreeString( bstrDoc ) ;
		SysFreeString( bstrHelp ) ;
	}
	else
	{
		wsprintf( szBuf, "GetDocumenation for TypeInfo FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
		_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
		return FALSE ;
	}

	LPTYPEATTR  pTA = NULL ;
	hr = lpTypeInfo->GetTypeAttr( &pTA ) ;
	if (FAILED( hr ))
	{
		wsprintf( szBuf, "GetTypeAttr FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
		_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
		return FALSE ;
	}

	cVars = pTA->cVars ;
	cFuncs = pTA->cFuncs ;
	lpTypeInfo->ReleaseTypeAttr( pTA ) ;

	short    cParams ;
	short   cParamsOpt ;
	char    szFunc[128] ;   // Function/Sub name
	LPSTR   szParams ;
	LPSTR   lpsz ;

	//
	// Enumerate through all FUNCDESCS
	for ( iIndex = 0 ; iIndex < cFuncs ; iIndex++)
	{
		hr = lpTypeInfo->GetFuncDesc( iIndex, &pFuncDesc ) ;
		if (FAILED(hr))
		{
			wsprintf( szBuf, "GetFuncDesc FAILED for function #%u (%s)\r\n", iIndex, (LPSTR)HRtoString( hr ) ) ;
			_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
			continue ;
		}

		hr = lpTypeInfo->GetDocumentation( pFuncDesc->memid, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
		if (SUCCEEDED(hr))
		{
			if (bstrDoc)
				wsprintf( szBuf, "' %s\r\n"\
								"'   %s\r\n",
				  (LPSTR)(bstrName ? bstrName : ""),
				  (LPSTR)(bstrName ? bstrDoc : "") ) ;
			else
				wsprintf( szBuf, "' %s\r\n",
				  (LPSTR)(bstrName ? bstrName : "") ) ;

			_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
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
				lstrcpy( szFunc, (LPSTR)rgbstrNames[0] ) ;
				SysFreeString( rgbstrNames[0] ) ;
			}

			// Allocate a string buffer that should be able to hold
			// all of our parameter types and names.
			//
			if (NULL == (szParams = GlobalAllocPtr( GPTR, max(cNames,cParams) * (64) )))
			{
				MessageBox( NULL, "GlobalAlloc Failed!", "Yikes!", MB_OK ) ;
				wsprintf( szBuf, "GlobalAlloc failed!\r\n" ) ;
				_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
				return FALSE ;
			}

			// For each parameter get the type and name.
			// The "max(cNames-1,cParams)" should handle the case
			// where a function has optional parameters (i.e. cParamsOpt is
			// non-zero).
			//
			lstrcpy( szParams, "(" ) ;

			// For each required parameter
			//
			for ( int n = 0 ; n < cParams - cParamsOpt ; n++ )
			{
				if (n+1 < cNames)
					lpsz = rgbstrNames[n+1] ;
				else
					lpsz = NULL ;
				BuildParam( szParams+lstrlen(szParams),
							&pFuncDesc->lprgelemdescParam[n], lpsz , FALSE ) ;

				if (n+1 < cNames)
					SysFreeString( rgbstrNames[n+1] ) ;

				// If it's the last one then no comma
				if (n + 1 != max(cNames-1,cParams))
					lstrcat( szParams, ", " ) ;
			}

			// For each optional parameter
			//
			for (n = cParams - cParamsOpt ; n < cParams ; n++)
			{
				if (n+1 < cNames)
					lpsz = rgbstrNames[n+1] ;
				else
					lpsz = NULL ;
				BuildParam( szParams+lstrlen(szParams),
							&pFuncDesc->lprgelemdescParam[n], lpsz , TRUE ) ;

				if (n+1 < cNames)
					SysFreeString( rgbstrNames[n+1] ) ;

				// If it's the last one then no comma
				if (n + 1 != max(cNames-1,cParams))
					lstrcat( szParams,  ", " ) ;
			}

			lstrcat( szParams , ")"  );

			// Is it a function or sub?
			//
			if (pFuncDesc->elemdescFunc.tdesc.vt == VT_EMPTY ||
				pFuncDesc->elemdescFunc.tdesc.vt == VT_NULL ||
				pFuncDesc->elemdescFunc.tdesc.vt == VT_VOID)
			{
				// Declare Sub <subname>(<arglist>)
				wsprintf( szBuf, "Declare Sub %s ", (LPSTR)szFunc ) ;
				_lwrite( hfile, szBuf, lstrlen(szBuf) ) ;
				_lwrite( hfile, szParams, lstrlen(szParams) ) ;
				_lwrite( hfile, "\r\n\r\n", 4 ) ;
			}
			else
			{
				// Declare Function <funcname>(<arglist>) As <returntype>
				wsprintf( szBuf, "Declare Function %s ", (LPSTR)szFunc ) ;
				_lwrite( hfile, szBuf, lstrlen(szBuf) ) ;
				_lwrite( hfile, szParams, lstrlen(szParams) ) ;
				wsprintf( szBuf, " As %s\r\n\r\n", (LPSTR)VTtoString2( pFuncDesc->elemdescFunc.tdesc.vt ) ) ;
				_lwrite( hfile, szBuf, lstrlen(szBuf) ) ;
			}

			GlobalFreePtr( szParams ) ;
		}
		else
		{
			wsprintf( szBuf, "GetNames (%lu) FAILED: %s", pFuncDesc->memid, (LPSTR)HRtoString( hr ) ) ;
			_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
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
			wsprintf( szBuf, "GetVarDesc FAILED for variable #%u (%s)\r\n", iIndex, (LPSTR)HRtoString( hr ) ) ;
			_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
			continue ;
		}
		else
		{
			hr = lpTypeInfo->GetDocumentation( pVarDesc->memid, &bstrName, &bstrDoc, &dwHelpID, &bstrHelp ) ;
			if (SUCCEEDED(hr))
			{
				if (bstrDoc)
					wsprintf( szBuf, "' %s\r\n"\
									 "'   %s\r\n",
					  (LPSTR)(bstrName ? bstrName : ""),
					  (LPSTR)(bstrName ? bstrDoc : "") ) ;
				else
					wsprintf( szBuf, "' %s\r\n",
					  (LPSTR)(bstrName ? bstrName : "") ) ;

				_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
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
					wsprintf( szBuf, "Const %s As %s = ",
									(LPSTR)rgbstrNames[0],
									(LPSTR)TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc )
									 ) ;
					VARIANT varValue ;
					VariantInit( &varValue ) ;
					hr = VariantChangeType( &varValue, pVarDesc->lpvarValue, 0, VT_BSTR ) ;
					if (FAILED(hr))
						lstrcat( szBuf, (LPSTR)HRtoString( hr ) ) ;
					else
					{
						lstrcat( szBuf, varValue.bstrVal ) ;
						SysFreeString( varValue.bstrVal ) ;
					}
				}
				else
				{
					// Dim varname[([subscripts])][As type]
					wsprintf( szBuf, "Dim %s ", (LPSTR)rgbstrNames[0] ) ;
					if ((pVarDesc->elemdescVar.tdesc.vt & 0x0FFF) == VT_PTR &&
						(pVarDesc->elemdescVar.tdesc.lptdesc->vt & 0x0FFF) == VT_SAFEARRAY)
					{
						lstrcat( szBuf, "() As " ) ;
						lstrcat( szBuf, VTtoString2( pVarDesc->elemdescVar.tdesc.vt ) ) ;
					}
					else
					{
						lstrcat( szBuf, "As " ) ;
						lstrcat( szBuf, TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc ) ) ;
					}
				}
				SysFreeString( rgbstrNames[0] ) ;
				lstrcat( szBuf, "\r\n\r\n" ) ;
				_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
			}
			else
			{
				wsprintf( szBuf, "GetNames (%lu) FAILED: %s\r\n", pVarDesc->memid, (LPSTR)HRtoString( hr ) ) ;
				_lwrite( hfile, szBuf, lstrlen( szBuf ) ) ;
			}

			lpTypeInfo->ReleaseVarDesc( pVarDesc ) ;
		}
	}

	return TRUE ;
}

LPSTR BuildParam( LPSTR sz, LPELEMDESC lpelemdesc, LPSTR lpszName, BOOL fOptional )
{
	// [Optional][ByVal|ByRef][ParamArray] varname [()] As type

	if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR &&
		(lpelemdesc->tdesc.lptdesc->vt & 0x0FFF) == VT_SAFEARRAY)
	{
		lstrcat( sz, "ParamArray " ) ;
	}
	else
	{
		if (fOptional)
			lstrcpy( sz, "Optional " ) ;

		if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR)
			lstrcat( sz, "ByRef " ) ;
		else
			lstrcat( sz, "ByVal " ) ;
	}

	if (lpszName != NULL)
		lstrcat( sz, lpszName ) ;

	if (lpszName != NULL)
		lstrcat( sz, " As " ) ;
	else
		lstrcat( sz, " " ) ;

	if ((lpelemdesc->tdesc.vt & 0x0FFF) == VT_PTR)
		lstrcat( sz, "Variant" ) ;
	else
	{
		lstrcat( sz, TYPEDESCtoString( &lpelemdesc->tdesc ) ) ;
	}
	return sz ;
}
