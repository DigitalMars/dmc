//**********************************************************************
// File name: ips.cpp
//
//    Implementation IPersistStorage for CSimpSvrObj
//
// Functions:
//
//    See obj.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"   


// local helper functions
static void OpenStreams(LPSTORAGE lpStg, LPSTREAM* ppColorStm, LPSTREAM* ppSizeStm);
static void CreateStreams(LPSTORAGE lpStg, LPSTREAM* ppColorStm, LPSTREAM* ppSizeStm);


//**********************************************************************
//
// CSimpSvrObj::InitNew
// 
// Purpose:
//                       
//      Used to give a new OLE object a ptr to its storage.
//  
// Parameters:
//                         
//      LPSTORAGE pStg  - Pointer to the storage       
// 
// Return Value:
//          
//      
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API 
//      CSimpSvrObj::HandsOffStorage    IPS.CPP
//      IStorage::AddRef                OLE
//      CreateStreams                   IPS.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::InitNew(LPSTORAGE pStg)
{
    OutputDebugString(TEXT("In IPS::InitNew\r\n"));

    // release any streams and storages that may be open
    HandsOffStorage();

    m_pStorage = pStg;

    // AddRef the new Storage
    if (m_pStorage)
        m_pStorage->AddRef();

    CreateStreams(m_pStorage, &m_pColorStm, &m_pSizeStm);

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::GetClassID
// 
// Purpose:
//                       
//      Returns the CLSID of this object.
//  
// Parameters:
//                         
//      LPCLSID lpClassID   - Out ptr in which to return the CLSID
// 
// Return Value:
//          
//       S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetClassID(LPCLSID lpClassID)
{
    OutputDebugString(TEXT("In IPS::GetClassID\r\n"));

    *lpClassID = GUID_SIMPLESVR32;

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::Save
// 
// Purpose:
//                       
//      Instructs the object to save itself into the storage.
//  
// Parameters:
//                         
//      LPSTORAGE pStgSave  - Storage in which the object should be saved
//
//      BOOL fSameAsLoad    - TRUE if pStgSave is the same as the storage
//                            that the object was originally created with.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      CreateStreams               IPS.CPP
//      IStream::AddRef             OLE
//      IStream::SetSize            OLE
//      IStream::Seek               OLE
//      IStream::Write              OLE
//      IStream::Release            OLE
//      
//
// Comments:
//
//      A real app will want better error checking in this method.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Save(LPSTORAGE pStg, BOOL fSameAsLoad)
{
    OutputDebugString(TEXT("In IPS::Save\r\n"));

    // first get streams to work with
    LPSTREAM lpTempColor, lpTempSize;

    if (!fSameAsLoad)
        CreateStreams(pStg, &lpTempColor, &lpTempSize);
    else
    {
        lpTempColor = m_pColorStm;
        lpTempColor->AddRef();
        lpTempSize = m_pSizeStm;
        lpTempSize->AddRef();
    }

    ULARGE_INTEGER uli;

    uli.LowPart = 0;
    uli.HighPart = 0;

    lpTempColor->SetSize(uli);
    lpTempSize->SetSize(uli);

    LARGE_INTEGER li;

    li.LowPart = 0;
    li.HighPart = 0;

    lpTempColor->Seek(li, STREAM_SEEK_SET, NULL);
    lpTempSize->Seek(li, STREAM_SEEK_SET, NULL);

    // write the colors to the stream
    lpTempColor->Write(&m_red,   sizeof(m_red),   NULL);
    lpTempColor->Write(&m_green, sizeof(m_green), NULL);
    lpTempColor->Write(&m_blue,  sizeof(m_blue),  NULL);

    // write the size to the stream
    lpTempSize->Write(&m_ptSize, sizeof(m_ptSize), NULL);

    lpTempColor->Release();
    lpTempSize->Release();

    m_fSaveWithSameAsLoad = fSameAsLoad;
    m_fNoScribbleMode = TRUE;

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::SaveCompleted
// 
// Purpose:
//                       
//      Called when the container is finished saving the object
//  
// Parameters:
//                         
//      LPSTORAGE pStgNew   - ptr to the new storage
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API
//      CSimpSvrObj::HandsOffStorage    IPS.CPP
//      IStorage::AddRef                OLE
//      OpenStreams                     IPS.CPP
//      IOleAdviseHolder::SendOnSave    Container
//
// Comments:
//          
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SaveCompleted(LPSTORAGE pStgNew)
{
    OutputDebugString(TEXT("In IPS::SaveCompleted\r\n"));

    if (pStgNew)
    {
        HandsOffStorage();
        m_pStorage = pStgNew;
        m_pStorage->AddRef();
        OpenStreams(m_pStorage, &m_pColorStm, &m_pSizeStm);
    }

    // OLE2NOTE: it is only legal to perform a Save or SaveAs operation
    //    on an embedded object. if the document is a file-based document
    //    then we can not be changed to a IStorage-base object.
    //
    //      fSameAsLoad   lpStgNew     Type of Save     Send OnSave
    //    ---------------------------------------------------------
    //         TRUE        NULL        SAVE             YES
    //         TRUE        ! NULL      SAVE *           YES
    //         FALSE       ! NULL      SAVE AS          YES
    //         FALSE       NULL        SAVE COPY AS     NO
    //
    //    * this is a strange case that is possible. It is inefficient
    //    for the caller; it would be better to pass lpStgNew==NULL for
    //    the Save operation.

    if (pStgNew || m_fSaveWithSameAsLoad)
    {
        if (m_fNoScribbleMode)
            GetOleAdviseHolder()->SendOnSave();     // normally would clear a
                                                    // dirty bit
        m_fSaveWithSameAsLoad = FALSE;
    }

    m_fNoScribbleMode = FALSE;

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::Load
// 
// Purpose:
//                       
//      Instructs the object to be loaded from storage.
//  
// Parameters:
//                         
//      LPSTORAGE pStg  - Ptr to the storage in which to be loaded
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API
//      CSimpSvrObj::HandsOffStorage    IPS.CPP
//      IStream::AddRef                 OLE
//      OpenStreams                     IPS.CPP
//      IStream::Read                   OLE API
//      
//
// Comments:
//
//      A real app will want better error checking in this method.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Load(LPSTORAGE pStg)
{
    OutputDebugString(TEXT("In IPS::Load\r\n"));

    // release any storage/streams we might have
    HandsOffStorage();

    // save new storage and open streams
    m_pStorage = pStg;
    m_pStorage->AddRef();
    OpenStreams(m_pStorage, &m_pColorStm, &m_pSizeStm);

    // Read the colors
    m_pColorStm->Read(&m_red,   sizeof(m_red),   NULL);
    m_pColorStm->Read(&m_green, sizeof(m_green), NULL);
    m_pColorStm->Read(&m_blue,  sizeof(m_blue),  NULL);

    // read the size
    m_pSizeStm->Read(&m_ptSize, sizeof(m_ptSize), NULL);

    return S_OK;
}


//**********************************************************************
//
// CSimpSvrObj::IsDirty
// 
// Purpose:
//                       
//      Returns whether or not the object is dirty w/respect to its
//      Storage
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      
//
// Comments:
//               
//      This sample does not implement this function, although a 
//      real application should.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::IsDirty()
{
    OutputDebugString(TEXT("In IPS::IsDirty\r\n"));

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::HandsOffStorage
// 
// Purpose:
//                       
//      Forces the object to release its handle to its storage.
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      IStream::Release            OLE
//      IStorage::Release           OLE
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::HandsOffStorage()
{
    OutputDebugString(TEXT("In IPS::HandsOffStorage\r\n"));

    if (m_pColorStm)
    {
        m_pColorStm->Release();
        m_pColorStm = NULL;
    }

    if (m_pSizeStm)
    {
        m_pSizeStm->Release();
        m_pSizeStm = NULL;
    }

    if (m_pStorage)
    {
        m_pStorage->Release();
        m_pStorage = NULL;
    }

    return S_OK;
}

//**********************************************************************
//
// OpenStreams
// 
// Purpose:
//                       
//      Opens the streams in a storage.
//  
// Parameters:
//                         
//      LPSTORAGE lpStg -   Storage in which to open the streams.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      IStorage::OpenStream        OLE
//
// Comments:
//
//
//********************************************************************

void OpenStreams(LPSTORAGE lpStg, LPSTREAM* ppColorStm, LPSTREAM* ppSizeStm)
{
    // open the color stream
    lpStg->OpenStream(OLESTR("RGB"),
                      0,
                      STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                      0,
                      ppColorStm);

    // open the color stream
    lpStg->OpenStream(OLESTR("SIZE"),
                      0,
                      STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                      0,
                      ppSizeStm);
}

//**********************************************************************
//
// CreateStreams
// 
// Purpose:
//                       
//      Creates streams in a storage.
//  
// Parameters:
//                         
//      LPSTORAGE lpStg                 - Pointer to the storage
//      LPSTREAM* ppColorStm            - Place to return Color Stream
//      LPSTREAM* ppSizeStm             - Place to return Size Stream
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      IStorage::CreateStream      OLE
//
// Comments:
//
//
//********************************************************************

void CreateStreams(LPSTORAGE lpStg, LPSTREAM* ppColorStm, LPSTREAM* ppSizeStm)
{
    // create a stream to save the colors
    lpStg->CreateStream(OLESTR("RGB"),
                        STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
                        0,
                        0,
                        ppColorStm);

    // create a stream to save the size
    lpStg->CreateStream(OLESTR("SIZE"),
                        STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
                        0,
                        0,
                        ppSizeStm);
}
