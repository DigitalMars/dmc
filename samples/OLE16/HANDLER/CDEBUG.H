//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  cdebug.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __CDEBUG_H__
#define __CDEBUG_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING CDEBUG.H from " __FILE__)
#endif  /* RC_INVOKED */


#define DEBUGBUFSIZE   512
#define FMTNAMESIZE     64

class CDebug {
private:
  char m_szBuffer[DEBUGBUFSIZE];
  BOOL m_bPrevFmt;
  DWORD m_dwDebugLevel;

  static DWORD dwDebugLevel;
  static CStdioFile LogFile;
  static bOkToLog;

public:
  ~CDebug();
  CDebug();

  static BOOL SetLogFileName(LPCSTR pszFileName);
  static void CloseLogFile(void);

  void Reset(void);
  void DebugOutput(CListBox* pListBox);

  CDebug& operator <<(LPCSTR psz);
  CDebug& operator <<(WORD w);
  CDebug& operator <<(DWORD dw);
  CDebug& operator <<(BOOL b);
  CDebug& operator <<(LONG l);
  CDebug& operator <<(IID iid);
  CDebug& operator <<(const LPRECT prc);
  CDebug& operator <<(LPCRECT prc);
  CDebug& operator <<(LPCRECTL prc);
  CDebug& operator <<(SIZE size);
  CDebug& operator <<(const LPSIZEL psizel);
  CDebug& operator <<(const LPMONIKER pmk);
  CDebug& operator <<(const LPFORMATETC pfmtetc);
  CDebug& operator <<(const LPSTGMEDIUM pstgmed);
  CDebug& operator <<(const LPUNKNOWN punk);
  CDebug& operator <<(const LPOLEINPLACEFRAMEINFO lpFrameInfo);
  CDebug& operator <<(HRESULT hRes);

  LPSTR GetFormat(CLIPFORMAT cfFormat);
  LPSTR GetTargetDevice(DVTARGETDEVICE FAR* ptd);
  LPSTR GetTymed(DWORD tymed);


}; /* CDebug */

#endif /* __CDEBUG_H__ */
