//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  dbghndlr.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __DBGHNDLR_H__
#define __DBGHNDLR_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING DBGHNDLR.H from " __FILE__)
#endif  /* RC_INVOKED */

#pragma warning(disable:4355)
#define NC(a,b) a##::##b

typedef LPVOID FAR* LPLPVOID;
class CHandlerWindow;

/////////////////////////////////////////////////////////////////////////////

class COleHandler : public CWinApp
{
public:
  virtual BOOL InitInstance(VOID);
  virtual int  ExitInstance(VOID);
};

/////////////////////////////////////////////////////////////////////////////


class CHandlerWindow : public CFrameWnd
{
private:
  CListBox m_ListBox;

public:
  CHandlerWindow(LPCSTR pcszLogFileName);
  ~CHandlerWindow();

  CListBox* GetListBox(void) { return &m_ListBox; }

  afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg VOID OnInitMenu(CMenu *pMenu);
  afx_msg void OnPaint();
  afx_msg void OnClose();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnAbout(VOID);
  afx_msg void OnClear(VOID);
  afx_msg void OnBreak(VOID);

  DECLARE_MESSAGE_MAP()

}; /* CHandlerWindow */

/////////////////////////////////////////////////////////////////////////////

#endif // __DBGHNDLR_H__
