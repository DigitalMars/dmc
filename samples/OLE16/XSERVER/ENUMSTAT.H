//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module:
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __ENUMSTAT_H__
#define __ENUMSTAT_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING ENUMSTAT.H from " __FILE__)
#endif  /* RC_INVOKED */

class CEnumStatData : public IEnumSTATDATA
{
public:
  CEnumStatData(ULONG Count, LPSTATDATA pStat, BOOL bDebug, ULONG StartIndex=0);
  ~CEnumStatData();

  //*** IEnumSTATDATA ***
  STDMETHOD(QueryInterface) (REFIID iid, LPVOID FAR* ppobj);
  STDMETHOD_(ULONG,AddRef) (VOID);
  STDMETHOD_(ULONG,Release)(VOID);

  STDMETHOD(Next)(ULONG celt, STATDATA FAR * rgelt, ULONG FAR* pceltFetched);
  STDMETHOD(Skip)(ULONG celt);
  STDMETHOD(Reset)();
  STDMETHOD(Clone)(IEnumSTATDATA FAR* FAR* ppenum);

//----------------------------------------------------------------------------
// shared features common to all implementations
//----------------------------------------------------------------------------
private:
  DWORD m_dwRefs;
  ULONG m_Count, m_Index;
  LPSTATDATA m_pStat;
  BOOL m_bDebug;

};

#endif // __ENUMSTAT_H__
