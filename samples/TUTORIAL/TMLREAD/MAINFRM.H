// mainfrm.h
//
//   Part of TMLRead, Symantec C++ 7.0 demonstration program
//   Copyright (C) 1994 Symantec Corporation
//
//   last mod:
//     rjw, 11/94 (release)
//
// Contents of this file:
//   CMainFrame class declaration
//

//// CMainFrame
////  Standard SDI main frame window.
////  One addition to the standard is the member rectWindRect.
////  We save the window size and position every time it is moved
////  or resized, then write these out to the profile when the window
////  is destructed.  Then, the next time TMLRead is run, we read the
////  profile and put the window back where it was.

class CMainFrame : public CFrameWnd
{
  protected: // create from serialization only
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    // Attributes
    public:

    // Operations
    public:

    // Implementation
    public:
      BOOL PreCreateWindow (CREATESTRUCT &cs);
      virtual ~CMainFrame();

#ifdef _DEBUG
      virtual void AssertValid() const;
      virtual void Dump(CDumpContext& dc) const;
#endif


    protected:  // control bar embedded members
      CStatusBar  m_wndStatusBar;
      CToolBar  m_wndToolBar;

    // Generated message map functions
    protected:
      //{{AFX_MSG(CMainFrame)
      afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
      afx_msg void OnMove(int x, int y);
      afx_msg void OnSize(UINT nType, int cx, int cy);
      //}}AFX_MSG
      DECLARE_MESSAGE_MAP()

    protected:
      CRect rectWindRect;       // holds last position and size
};
