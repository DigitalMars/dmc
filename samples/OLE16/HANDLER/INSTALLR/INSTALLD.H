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

#define CBMAXKEY    256

/////////////////////////////////////////////////////////////////////////////
// CInstallDlg dialog

class CInstallDlg : public CModalDialog
{
// Construction
public:
  CInstallDlg();  // standard constructor
  BOOL FillLists(VOID);

// Dialog Data
  enum { IDD = IDD_DIALOG1 };
  CListBox  *m_pInstalled;
  CListBox  *m_pAvailable;

// Implementation
protected:

  virtual BOOL OnInitDialog();
  afx_msg void OnClickedInstall();
  afx_msg void OnClickedRemove();
  afx_msg void OnSelchangeList1();
  afx_msg void OnSelchangeList2();
  afx_msg void OnClickedSetup();

  DECLARE_MESSAGE_MAP()
};
