//**********************************************************************
// File name: IDO.CPP
//
//    Implementation of IDataObject for CSimpSvrObj
//
// Functions:
//
//    See ido.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"   

//**********************************************************************
//
// CSimpSvrObj::GetData
// 
// Purpose:
//                     
//      Returns the data in the format specified in pformatetcIn.
//  
// Parameters:
//                         
//      LPFORMATETC pformatetcIn    -   The format requested by the caller
//
//      LPSTGMEDIUM pmedium         -   The medium requested by the caller
// 
// Return Value:
//                                                                        
//      DATA_E_FORMATETC    - Format not supported
//      S_OK                - Success 
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API
//      CSimpSvrObj::GetMetaFilePict    OBJ.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetData(LPFORMATETC pformatetcIn,
                                  LPSTGMEDIUM pmedium)
{
    OutputDebugString(TEXT("In IDO::GetData\r\n"));

    pmedium->tymed = 0;
    pmedium->hGlobal = NULL;
    pmedium->pUnkForRelease = NULL;

    // Check to the FORMATETC and fill pmedium if valid.
    if ((pformatetcIn->cfFormat == CF_METAFILEPICT)  &&
        (pformatetcIn->dwAspect == DVASPECT_CONTENT) &&
        (pformatetcIn->tymed & TYMED_MFPICT))
    {
        pmedium->tymed = TYMED_MFPICT;
        pmedium->hMetaFilePict = GetMetaFilePict();
        return S_OK;
    }

    return DATA_E_FORMATETC;
}

//**********************************************************************
//
// CSimpSvrObj::GetDataHere
// 
// Purpose:
//                         
//      Called to get a data format in a caller supplied location
//  
// Parameters:
//                         
//      LPFORMATETC pformatetc  - FORMATETC requested
//
//      LPSTGMEDIUM pmedium     - Medium to return the data
//
// Return Value:
//          
//      DATA_E_FORMATETC    - We don't support the requested format      
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//                                                                 
//      In this simple implementation, we don't really support this
//      method, we just always return DATA_E_FORMATETC.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetDataHere(LPFORMATETC pformatetc,
                                      LPSTGMEDIUM pmedium)
{
    OutputDebugString(TEXT("In IDO::GetDataHere\r\n"));

    return DATA_E_FORMATETC;
}

//**********************************************************************
//
// CSimpSvrObj::QueryGetData
// 
// Purpose:                 
//
//      Called to determine if our object supports a particular
//      FORMATETC.                       
//  
// Parameters:
//                         
//      LPFORMATETC pformatetc  - Pointer to the FORMATETC being queried for.
// 
// Return Value:
//                                                                           
//      DATA_E_FORMATETC    - The FORMATETC is not supported
//      S_OK                - The FORMATETC is supported.
//      
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

STDMETHODIMP CSimpSvrObj::QueryGetData(LPFORMATETC pformatetc)
{
    OutputDebugString(TEXT("In IDO::QueryGetData\r\n"));

    // check the validity of the formatetc.
    if ((pformatetc->cfFormat == CF_METAFILEPICT)  &&
        (pformatetc->dwAspect == DVASPECT_CONTENT) &&
        (pformatetc->tymed == TYMED_MFPICT))
        return S_OK;

    return DATA_E_FORMATETC;
}

//**********************************************************************
//
// CSimpSvrObj::GetCanonicalFormatEtc
// 
// Purpose:
//                                   
//      Returns a FORMATETC that is equivalent to the one passed in.
//  
// Parameters:
//                         
//      LPFORMATETC pformatetc      - FORMATETC to be tested.
//
//      LPFORMATETC pformatetcOut   - Out ptr for returned FORMATETC.
// 
// Return Value:
//          
//      DATA_S_SAMEFORMATETC    - Use the same formatetc as was passed.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API                                   
//      CoGetMalloc                 OLE API
//      IMalloc::Alloc              OLE
//      IMalloc::Release            OLE
//      _fmemcpy                    C run-time
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetCanonicalFormatEtc(LPFORMATETC pformatetc,
                                                LPFORMATETC pformatetcOut)
{
    HRESULT hresult;

    OutputDebugString(TEXT("In IDO::GetCanonicalFormatEtc\r\n"));

    if (!pformatetcOut)
        return E_INVALIDARG;

    // OLE2NOTE: we must make sure to set all out parameters to NULL.
    pformatetcOut->ptd = NULL;

    if (!pformatetc)
        return E_INVALIDARG;

    // OLE2NOTE: we must validate that the format requested is supported 
    if ((hresult = QueryGetData(pformatetc)) != NOERROR)
        return hresult;

    // OLE2NOTE: an app that is insensitive to target device (as
    //    SimpSvr is) should fill in the lpformatOut parameter
    //    but NULL out the "ptd" field; it should return NOERROR if the
    //    input formatetc->ptd is non-NULL. This tells the caller
    //    that it is NOT necessary to maintain a separate screen
    //    rendering and printer rendering. It should return
    //    DATA_S_SAMEFORMATETC if the input and output formatetc's are
    //    identical.

    *pformatetcOut = *pformatetc;
    if (pformatetc->ptd == NULL)
        return DATA_S_SAMEFORMATETC;
    else
    {
        pformatetcOut->ptd = NULL;
        return NOERROR;
    }
}

//**********************************************************************
//
// CSimpSvrObj::SetData
// 
// Purpose:            
//
//      Called to set the data for the object.                       
//  
// Parameters:
//                         
//      LPFORMATETC pformatetc      - the format of the data being passed
//
//      STGMEDIUM* pmedium          - the location of the data.
//
//      BOOL fRelease               - Defines the ownership of the medium
// 
// Return Value:
//          
//      DATA_E_FORMATETC    - Not a valid FORMATETC for this object
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//                                                                 
//      This simple object does not support having its data set, so an
//      error value is always returned.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetData(LPFORMATETC pformatetc,
                                  LPSTGMEDIUM pmedium,
                                  BOOL fRelease)
{
    OutputDebugString(TEXT("In IDO::SetData\r\n"));

    return DATA_E_FORMATETC;
}

//**********************************************************************
//
// CSimpSvrObj::EnumFormatEtc
// 
// Purpose:                  
//
//      Enumerates the formats supported by this object.                       
//  
// Parameters:
//                         
//      DWORD dwDirection                       - Order of enumeration.
//
//      LPENUMFORMATETC* ppenumFormatEtc        - Place to return a pointer
//                                                to the enumerator.
// 
// Return Value:
//          
//      OLE_S_USEREG    - Indicates that OLE should consult the REG DB
//                        to enumerate the formats.
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

STDMETHODIMP CSimpSvrObj::EnumFormatEtc(DWORD dwDirection,
                                        LPENUMFORMATETC* ppenumFormatEtc)
{
    OutputDebugString(TEXT("In IDO::EnumFormatEtc\r\n"));

    // need to NULL the out parameter
    *ppenumFormatEtc = NULL;

    return OLE_S_USEREG;
}

//**********************************************************************
//
// CSimpSvrObj::DAdvise
// 
// Purpose:            
//
//      Called by the container when it would like to be notified of 
//      changes in the object data.
//  
// Parameters:
//                         
//      LPFORMATETC pformatetc      - The format the container is interested in.
//
//      DWORD advf                  - The type of advise to be set up.
// 
//      LPADVISESINK pAdvSink       - Pointer to the containers IAdviseSink
//
//      LPDWORD pdwConnection       - Out parameter to return a unique connection id.
// 
// Return Value:
//          
//      passed on from IDataAdviseHolder
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      CreateDataAdviseHolder      OLE API
//      IDataAdviseHolder::Advise   OLE API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::DAdvise(LPFORMATETC pformatetc,
                                  DWORD advf,
                                  LPADVISESINK pAdvSink,
                                  LPDWORD pdwConnection)
{
    OutputDebugString(TEXT("In IDO::DAdvise\r\n"));

    // First clear the out pointer
    *pdwConnection = 0;

    // Do we support this format?
    HRESULT hr = QueryGetData(pformatetc);
    if (hr != NOERROR)
        return hr;

    // If no DataAdviseHolder has been created, then create one.
    if (!m_pDataAdviseHolder && CreateDataAdviseHolder(&m_pDataAdviseHolder) != NOERROR)
        return E_OUTOFMEMORY;

    // Pass on to the DataAdviseHolder
    return m_pDataAdviseHolder->Advise((LPDATAOBJECT)this,
                                       pformatetc,
                                       advf,
                                       pAdvSink,
                                       pdwConnection);
}

//**********************************************************************
//
// CSimpSvrObj::DUnadvise
// 
// Purpose:
//                       
//      Breaks down an Advise connection.
//  
// Parameters:
//                         
//      DWORD dwConnection  - Advise connection ID.
// 
// Return Value:
//          
//      Returned from the DataAdviseHolder.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      IDataAdviseHolder::Unadvise OLE 
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::DUnadvise(DWORD dwConnection)
{
    OutputDebugString(TEXT("In IDO::DUnadvise\r\n"));

    // We should already have a DataAdviseHolder. If not, fail this call.
    if (NULL == m_pDataAdviseHolder)
        return E_FAIL;

    // Pass on to the DataAdviseHolder
    return m_pDataAdviseHolder->Unadvise(dwConnection);
}

//**********************************************************************
//
// CSimpSvrObj::EnumDAdvise
// 
// Purpose:
//                         
//      Returns an enumerator that enumerates all of the advises
//      set up on this data object.
//  
// Parameters:
//                         
//      LPENUMSTATDATA* ppenumAdvise        - An out ptr in which to 
//                                            return the enumerator.
// 
// Return Value:
//          
//      Passed back from IDataAdviseHolder::EnumAdvise     
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API       
//      IDAtaAdviseHolder::EnumAdvise   OLE
//
// Comments:
//                                      
//      This just delegates to the DataAdviseHolder.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::EnumDAdvise(LPENUMSTATDATA* ppenumAdvise)
{
    OutputDebugString(TEXT("In IDO::EnumDAdvise\r\n"));

    // NULL the out parameter
    *ppenumAdvise = NULL;

    // We should already have a DataAdviseHolder. If not, fail this call.
    if (NULL == m_pDataAdviseHolder)
        return E_FAIL;

    // Pass on to the DataAdviseHolder
    return m_pDataAdviseHolder->EnumAdvise(ppenumAdvise);
}
