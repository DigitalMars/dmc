//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: installd.cpp 
//  date:    
//  author:  
//
//  purpose: 
//
//----------------------------------------------------------------------------
#include "stdafx.h" 

/////////////////////////////////////////////////////////////////////////////
// CInstallDlg dialog

CInstallDlg::CInstallDlg()
  : CModalDialog(CInstallDlg::IDD, NULL)
{
}

BEGIN_MESSAGE_MAP(CInstallDlg, CModalDialog)
  ON_BN_CLICKED(IDC_INSTALL, OnClickedInstall)
  ON_BN_CLICKED(IDC_REMOVE, OnClickedRemove)
  ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
  ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
  ON_BN_CLICKED(IDC_SETUP, OnClickedSetup)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInstallDlg message handlers

BOOL CInstallDlg::
  OnInitDialog()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  CModalDialog::OnInitDialog();
  
  CWnd *pWnd = (CWnd *)GetDlgItem(IDC_SETUP);
  pWnd->EnableWindow(FALSE);

  pWnd = (CWnd *)GetDlgItem(IDC_INSTALL);
  pWnd->EnableWindow(FALSE);
  
  pWnd = (CWnd *)GetDlgItem(IDC_REMOVE);
  pWnd->EnableWindow(FALSE);   
  
  FillLists();

  return TRUE;  // return TRUE  unless you set the focus to a control
}


void CInstallDlg::
  OnClickedInstall()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{ 
  CListBox *pAvailable = (CListBox *)GetDlgItem(IDC_LIST2);
  int nItems = pAvailable->GetCount();
  int FAR *pnItems;
  LPMALLOC pMalloc=NULL;
  HKEY hKey=NULL;
  CLSID clsid;
  LONG lRet;
  LPSTR pszProgID;
  char szInprocHandler[CBMAXKEY];
  LPSTR pszClass;

  if (CoGetMalloc(MEMCTX_TASK, &pMalloc) != NOERROR) {
    // do some error thing
    goto errReturn;
  }
  
  pnItems = (int FAR *)pMalloc->Alloc(nItems);
  if (pnItems != NULL) {
    nItems = pAvailable->GetSelItems(nItems, pnItems);

    lRet=RegOpenKey(HKEY_CLASSES_ROOT, "clsid", &hKey);
    if ((LONG)ERROR_SUCCESS!=lRet) {
      // do some error thing
      goto errReturn;
    }

    for (int i=0; i<nItems; i++) { 
      pszProgID = (LPSTR)pMalloc->Alloc(pAvailable->GetTextLen(pnItems[i]));
      if (pAvailable->GetText(pnItems[i], pszProgID)) {

        CLSIDFromProgID(pszProgID, &clsid);
        StringFromCLSID(clsid, (LPSTR FAR*)&pszClass);          
        
        lstrcpy(szInprocHandler, pszClass);
        lstrcat(szInprocHandler, "\\InprocHandler");
        
        lRet=RegSetValue(hKey, szInprocHandler, REG_SZ, 
                         (LPCSTR)"dbghndlr.dll", sizeof("dbghndlr.dll"));
        if ((LONG)ERROR_SUCCESS!=lRet) {
        }
      
        pMalloc->Free(pszClass);
      }

      pMalloc->Free(pszProgID);
    }
    
    pMalloc->Free(pnItems);
  }

errReturn:
  if (pMalloc != NULL) {
    pMalloc->Release();
  }

  RegCloseKey(hKey);
  FillLists();  
}


void CInstallDlg::
  OnClickedRemove()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  CListBox *pInstalled = (CListBox *)GetDlgItem(IDC_LIST1);
  int nItems = pInstalled->GetCount();
  int FAR *pnItems;
  LPMALLOC pMalloc=NULL;
  HKEY hKey=NULL;
  CLSID clsid;
  LONG lRet;
  LPSTR pszProgID;
  char szInprocHandler[CBMAXKEY];
  LPSTR pszClass;

  if (CoGetMalloc(MEMCTX_TASK, &pMalloc) != NOERROR) {
    // do some error thing
    goto errReturn;
  }
  
  pnItems = (int FAR *)pMalloc->Alloc(nItems);
  if (pnItems != NULL) {
    nItems = pInstalled->GetSelItems(nItems, pnItems);

    lRet=RegOpenKey(HKEY_CLASSES_ROOT, "clsid", &hKey);
    if ((LONG)ERROR_SUCCESS!=lRet) {
      // do some error thing
      goto errReturn;
    }

    for (int i=0; i<nItems; i++) { 
      pszProgID = (LPSTR)pMalloc->Alloc(pInstalled->GetTextLen(pnItems[i]));
      if (pInstalled->GetText(pnItems[i], pszProgID)) {

        CLSIDFromProgID(pszProgID, &clsid);
        StringFromCLSID(clsid, (LPSTR FAR*)&pszClass);          
        
        lstrcpy(szInprocHandler, pszClass);
        lstrcat(szInprocHandler, "\\InprocHandler");
        
        lRet=RegSetValue(hKey, szInprocHandler, REG_SZ, 
                         (LPCSTR)"ole2.dll", sizeof("ole2.dll"));
        if ((LONG)ERROR_SUCCESS!=lRet) {
        }
      
        pMalloc->Free(pszClass);
      }

      pMalloc->Free(pszProgID);
    }
    
    pMalloc->Free(pnItems);
  }

errReturn:
  if (pMalloc != NULL) {
    pMalloc->Release();
  }

  RegCloseKey(hKey);
  FillLists();  
}


void CInstallDlg::
  OnSelchangeList1()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  CListBox *pInstalled = (CListBox *)GetDlgItem(IDC_LIST1);
  CWnd *pWnd1 = (CWnd *)GetDlgItem(IDC_REMOVE);
  
  if (pInstalled->GetSelCount() > 0) {
    pWnd1->EnableWindow(TRUE);
  }else{
    pWnd1->EnableWindow(FALSE);
  }
}


void CInstallDlg::
  OnSelchangeList2()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  CListBox *pAvailable = (CListBox *)GetDlgItem(IDC_LIST2);
  CWnd *pWnd1 = (CWnd *)GetDlgItem(IDC_INSTALL);
  
  if (pAvailable->GetSelCount() > 0) {
    pWnd1->EnableWindow(TRUE);
  }else{
    pWnd1->EnableWindow(FALSE);
  }
  
}


void CInstallDlg::
  OnClickedSetup()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  
}


BOOL CInstallDlg::
  FillLists(VOID)
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
  long        dw;
  UINT        cStrings;
  HKEY        hKey;
  LONG        lRet;
  char        szProgID[CBMAXKEY];
  char        szInprocHandler[CBMAXKEY];
  char        szClass[CBMAXKEY];
  char        szKey1[CBMAXKEY], szKey2[CBMAXKEY];

  //Clean out the existing strings.
  CListBox *pInstalled = (CListBox *)GetDlgItem(IDC_LIST1);
  CListBox *pAvailable = (CListBox *)GetDlgItem(IDC_LIST2);

  pInstalled->ResetContent();
  pAvailable->ResetContent();

  //Open up the root key.
  lRet=RegOpenKey(HKEY_CLASSES_ROOT, "clsid", &hKey);
  if ((LONG)ERROR_SUCCESS!=lRet) {
    return FALSE;
  }

  cStrings=0;
  lRet=RegEnumKey(hKey, cStrings++, szClass, CBMAXKEY);

  while ((LONG)ERROR_SUCCESS==lRet) {

    lstrcpy(szProgID, szClass);
    lstrcat(szProgID, "\\ProgID");

    lstrcpy(szInprocHandler, szClass);
    lstrcat(szInprocHandler, "\\InprocHandler");

    dw=CBMAXKEY;

    lRet=RegQueryValue(hKey, szProgID, szKey1, &dw);
    if ((LONG)ERROR_SUCCESS==lRet) {

      dw=CBMAXKEY;
      lRet=RegQueryValue(hKey, szInprocHandler, szKey2, &dw);
      if ((LONG)ERROR_SUCCESS==lRet) {
        if (!lstrcmp((LPSTR)szKey2, "ole2.dll")) {
          pAvailable->AddString(szKey1);
        }else if (!lstrcmp((LPSTR)szKey2, "dbghndlr.dll")) {
          pInstalled->AddString(szKey1);
        }
      }else{
        /* no InprocHandler Key could be 1.0 server */
      	pAvailable->AddString(szKey1);
      }  
    }          
  
    //Continue with the next key.
    lRet=RegEnumKey(hKey, cStrings++, szClass, CBMAXKEY);
  }
   
  RegCloseKey(hKey);

  OnSelchangeList1();
  OnSelchangeList2();
  
  return TRUE;
  
} /* FillLists()
   */           

/////////////////////////////////////////////////////////////////////////////
// Application class

class CInstallerApp : public CWinApp
{
public:
  virtual BOOL InitInstance();
};

CInstallerApp NEAR theInstallerApp;

BOOL CInstallerApp::
  InitInstance()
//------------------------------------------------------------------
//
//------------------------------------------------------------------
{
#if _MFC_VER == 0x0200 
  // Standard initialization
  SetDialogBkColor();        // set dialog background color to gray
#endif

  CInstallDlg  dlg; 

  if (OleInitialize(NULL) != NOERROR) {
    goto errReturn;
  }

  if (dlg.DoModal() == IDOK) {
  
  }
   
  OleUninitialize();
     
errReturn:   
  ::PostQuitMessage(0);       // Exit application
  return FALSE;               // FALSE means CWinApp::Run is not called
}

/////////////////////////////////////////////////////////////////////////////

