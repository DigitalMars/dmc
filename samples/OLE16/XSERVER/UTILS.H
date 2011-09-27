//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module: utils.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __UTILS_H__
#define __UTILS_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING UTILS.H from " __FILE__)
#endif  /* RC_INVOKED */

#define HIMETRIC_PER_INCH   2540    //number Himetric units Per Inch

/* For Bounds Stuff */
extern int giXppli;
extern int giYppli;

STDAPI OpenOrCreateIStream(LPCSTR pname, LPSTORAGE prootstg,
						   LPSTREAM FAR* ppstm);

STDAPI_(LPVOID) OleStdAlloc(ULONG cb);
STDAPI_(LPVOID) OleStdRealloc(void FAR* pv, ULONG cb);
STDAPI_(VOID) OleStdFree(void FAR* pv);

STDAPI_(VOID) HiMetricToDevice(LPSIZEL psizel);
STDAPI_(VOID) DeviceToHiMetric(LPSIZEL psizel);

STDAPI_(DVTARGETDEVICE FAR*)
  OleStdCopyTargetDevice(DVTARGETDEVICE FAR* ptdSrc);
STDAPI_(BOOL) OleStdCopyFormatEtc(LPFORMATETC petcDest, LPFORMATETC petcSrc);
STDAPI_(BOOL) OleStdCopyStatData(LPSTATDATA pDest, LPSTATDATA pSrc);

STDAPI_(HGLOBAL) OleStdGetMetafile(LPVIEWOBJECT2 pViewObject,
								   DWORD dwDrawAspect,
								   DVTARGETDEVICE FAR* ptd);

STDAPI OleStdRegisterAsRunning(LPUNKNOWN lpUnk, LPMONIKER lpmkFull,
							   DWORD FAR* lpdwRegister);
STDAPI OleStdRevokeAsRunning(DWORD FAR* lpdwRegister);

#endif // __UTILS_H__
