//**********************************************************************
// File name: IEC.CPP
//
//    Implementation of IExternalConnection for CSimpSvrObj
//
// Functions:
//
//    See iec.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"   


//**********************************************************************
//
// CSimpSvrObj::AddConnection
// 
// Purpose:
//                       
//      Called when another connection is made to the object.
//  
// Parameters:
//                         
//      DWORD extconn   -   Type of connection
//
//      DWORD reserved  -   Reserved
// 
// Return Value:
//          
//      Strong connection count
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(DWORD) CSimpSvrObj::AddConnection(DWORD extconn, DWORD /* dwReserved */)
{
    OutputDebugString(TEXT("In IEC::AddConnection\r\n"));

    return (extconn & EXTCONN_STRONG ? ++m_dwStrong : 0);
}

//**********************************************************************
//
// CSimpSvrObj::ReleaseConnection
// 
// Purpose:
//                       
//      Called when a connection to the object is released.
//  
// Parameters:
//                         
//      DWORD extconn               - Type of Connection
//
//      DWORD reserved              - Reserved
//
//      BOOL fLastReleaseCloses     - Close flag
// 
// Return Value:
//          
//      The new reference count    
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      COleObject::Close           IOO.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(DWORD) CSimpSvrObj::ReleaseConnection(DWORD extconn,
                                                    DWORD /* dwReserved */,
                                                    BOOL fLastReleaseCloses)
{
    OutputDebugString(TEXT("In IEC::ReleaseConnection\r\n"));

    if (extconn & EXTCONN_STRONG)
    {
        DWORD dwSave = --m_dwStrong;

        if (0 == dwSave && fLastReleaseCloses)
            Close(OLECLOSE_SAVEIFDIRTY);

        return dwSave;
    }

    return 0;
}
