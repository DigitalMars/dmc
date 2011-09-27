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
  BOOL m_bPrevFmt, m_bDebugOutput;
  DWORD m_dwDebugLevel;

  static DWORD dwDebugLevel;
  static BOOL bDoOutput;

public:
  CDebug(BOOL bDebugOutput = TRUE);
  ~CDebug();

  static VOID StartDebugging(VOID) { bDoOutput = TRUE; }
  static VOID StopDebugging(VOID) { bDoOutput = FALSE; }

  VOID Reset(VOID);
  VOID DebugOutput(VOID);

  CDebug& operator <<(LPCSTR psz);
  CDebug& operator <<(WORD w);
  CDebug& operator <<(DWORD dw);
  CDebug& operator <<(BOOL b);
  CDebug& operator <<(LONG l);
  CDebug& operator <<(IID iid);
  CDebug& operator <<(LPCLSID pclsid);
  CDebug& operator <<(const LPRECT prc);
  CDebug& operator <<(LPCRECT prc);
  CDebug& operator <<(LPCRECTL prc);
  CDebug& operator <<(SIZE size);
  CDebug& operator <<(const LPSIZEL psizel);
  CDebug& operator <<(const LPMONIKER pmk);
  CDebug& operator <<(const LPFORMATETC pfmtetc);
  CDebug& operator <<(const LPSTGMEDIUM pstgmed);
  CDebug& operator <<(const DVTARGETDEVICE FAR* ptd);
  CDebug& operator <<(const LPUNKNOWN punk);
  CDebug& operator <<(const LPOLEINPLACEFRAMEINFO lpFrameInfo);
  CDebug& operator <<(HRESULT hRes);

  LPSTR GetFormat(CLIPFORMAT cfFormat);
  LPSTR GetTargetDevice(DVTARGETDEVICE FAR* ptd);
  LPSTR GetTymed(DWORD tymed);


}; /* CDebug */

#endif /* __CDEBUG_H__ */
