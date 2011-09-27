/*** 
*dspcalc2.h
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*
*
*Implementation Notes:
*
*****************************************************************************/

#include "hostenv.h"
#include "resource.h"
#include "clsid.h" 

#ifndef CLASS
# ifdef __TURBOC__
#  define CLASS class huge
# else
#  define CLASS class FAR
# endif
#endif

#pragma warning(disable:4355)

#ifdef _MAC
typedef void * LPVOID;
typedef unsigned long ULONG;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef long LONG;
#define FALSE 0
#define TRUE  1
#define NEAR 
#endif

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif


/* 
 * The .h file included below is created via MkTypLib
 */
#include "calctype.h"

#pragma warning(disable:4355)

#define DIM(X) (sizeof(X)/sizeof(X[0]))


// forward decl
CLASS CCalc;


// Introduced "calculator" interface
//
// This nested class implementes core arithmetic functionality
// (such as it is) *and* is the interface that will be exposed via
// IDispatch for external programmability.
//
CLASS CArith : public _ICalculator {
 public:

  /* IUnknown methods */
  STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR * ppvObj);
  STDMETHOD_(ULONG, AddRef)(THIS);
  STDMETHOD_(ULONG, Release)(THIS);

  /* ICalculator methods */
  STDMETHOD_(void, put_Accum)(long l);
  STDMETHOD_(long, get_Accum)(void);
  STDMETHOD_(void, put_Opnd)(long l);
  STDMETHOD_(long, get_Opnd)(void);
  STDMETHOD_(void, put_Op)(OPERATORS op);
  STDMETHOD_(OPERATORS, get_Op)(void);
  STDMETHOD_(VARIANT_BOOL, Eval)(void);
  STDMETHOD_(void, Clear)(void);
  STDMETHOD_(void, Display)(void);
  STDMETHOD_(void, Quit)(void);
  STDMETHOD_(VARIANT_BOOL, Button)(SAFEARRAY FAR* psa);

  // the following method is internal, and not exposed for programmability
  BOOL ButtonPush(int button);

  CArith(CCalc FAR* pcalc){
     m_pcalc = pcalc;
     Clear();
  }

  enum states { STATE_LOPND, STATE_OP, STATE_ROPND, STATE_EVAL };

 private:
  CCalc FAR* m_pcalc;
  OPERATORS	m_op;
  long	m_opnd;
  long	m_accum;
  enum states m_state;
};

    
CLASS CCalc : public IUnknown {
public:
    friend CArith;
    
    static CCalc FAR* Create();

    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);

    CCalc() : m_arith(this)
    {
      m_refs = 0;
#ifdef _MAC
      m_pdlg = nil;
#else
      m_hwnd = 0;
#endif
      m_punkStdDisp = NULL;
    }
    
#ifdef _MAC
    DialogPtr m_pdlg; 
#else
    HWND m_hwnd;
#endif
    CArith m_arith;

private:
    ULONG m_refs;
    IUnknown FAR* m_punkStdDisp;
};


// the CCalc Class Factory
//
CLASS CCalcCF : public IClassFactory {
public:
    static IClassFactory FAR* Create();

    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);

    STDMETHOD(CreateInstance)(
      IUnknown FAR* punkOuter, REFIID riid, void FAR* FAR* ppv);
#ifdef _MAC
    STDMETHOD(LockServer)(unsigned long fLock);
#else
    STDMETHOD(LockServer)(BOOL fLock);
#endif

    CCalcCF() { m_refs = 1; }

private:
    ULONG m_refs;
};

extern HRESULT InitOle(void);
extern HRESULT UninitOle(void);

extern CCalc FAR* g_pcalc;

