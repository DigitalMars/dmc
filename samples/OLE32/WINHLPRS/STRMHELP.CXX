//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       strmhelp.cxx
//
//  Contents:   Routines for writting variables to a stream in an
//              architecture independent manner.
//
//  Classes:
//
//  Functions:  WriteDouble
//              ReadDouble
//              SizeDouble
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include "strmhelp.h"
#include <stdio.h>

#define MAX_SIZE 128

//+---------------------------------------------------------------------------
//
//  Function:   SizeDouble
//
//  Synopsis:   returns the maximum number of bytes that WriteDouble will
//              take to write a double to an IStream
//
//  Arguments:  none
//
//  Returns:    maximum number of bytes needed to write a double to an IStream
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

DWORD SizeDouble(void)
{
    return(MAX_SIZE);
}

//+---------------------------------------------------------------------------
//
//  Function:   WriteDouble
//
//  Synopsis:   rrites a double to an OLE stream in a machine independent manner
//
//  Arguments:  [pstm]       - pointer to an IStream
//              [d]          - the double to be written
//              [pcbWritten] - pointer to the number of bytes actually written
//                             (may be NULL)
//
//  Returns:    same return values as IStream::Write
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT WriteDouble(IStream * pstm, double d, ULONG FAR * pcbWritten)
{
    char szTemp[MAX_SIZE];
    sprintf(szTemp,"%.100lg", d);
    WORD cch = strlen(szTemp);
    return(pstm->Write(szTemp, cch + 1, pcbWritten));
}

//+---------------------------------------------------------------------------
//
//  Function:   ReadDouble
//
//  Synopsis:   reads a double from an OLE stream in a machine independent
//              manner
//
//  Arguments:  [pstm]    - pointer to an IStream
//              [pd]      - pointer to the double to be read
//              [pcbRead] - pointer to the number of bytes actually read
//                          (may be NULL)
//
//  Returns:    same return values as IStream::Read
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT ReadDouble(IStream * pstm, double * pd, ULONG FAR * pcbRead)
{
    HRESULT hr;
    char szTemp[MAX_SIZE];
    ULONG cbRead;
    DWORD cch = 0;
    do
    {
        hr = pstm->Read(&szTemp[cch], sizeof(char), &cbRead);
        if (pcbRead && SUCCEEDED(hr))
        {
            pcbRead += cbRead;
        }

    } while (SUCCEEDED(hr) && szTemp[cch++]);
    if (SUCCEEDED(hr))
    {
        sscanf(szTemp, "%lf",pd);
    }
    return(hr);
}
