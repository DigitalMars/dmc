/*** 
*dispcalc.h
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
*  UNDONE
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
# define NEARDATA
#else
# ifdef WIN32
#  define NEARDATA
# else
#  define NEARDATA __near
# endif
#endif

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif

#define DIM(X) (sizeof(X)/sizeof(X[0]))



// forward decl
CLASS CCalc;

// Introduced "calculator" interface
//
// This class implementes core arithmetic functionality
// (such as it is) *and* is the interface that will be
// exposed via IDispatch for external programmability.

#if defined(_MAC) && !(!defined(applec) || defined(__SC__) || defined(_MSC_VER))

// Mac Note: The default implementation of IDispatch places
// a couple of requirements on the layout of an instance that
// in can invoke on.
//
//   1. It assumes that the vtable pointer is at offset 0
//      from the beginning of the instance. This appears
//      to always be the case if the class derives from
//      an interface, and hence the games I play below
//      creating a stub _CArith interface containing a
//      single pure virtual function. The only reason for
//      this is to ensure that the vtable ptr is at offset
//      0 when this class is a member of CCalc. If your
//      class derives from an Ole interface, then you are ok.
//
//   2. It assumes that the vtable is a simple array of
//      function pointers, with index zero reserved. In
//      order to ensure this, the class must derive from
//      SingleObject. Note that IUnknown derives from SingleObject,
//      so if your class derives from any Ole interface, then
//      you are also ok - otherwise you need to explicitly
//      derive from SingleObject as I have done below.
//
//  (The above comments apply to MPW C++ v3.3)
//      

interface _CArith : public SingleObject
{
    STDMETHOD_(void,  put_Accum)(long l) PURE;
};
CLASS CArith : public _CArith
#else
CLASS CArith
#endif
{
public:
#if defined(_MAC)
    BEGIN_INTERFACE
#endif //_MAC

    STDMETHOD_(void,  put_Accum)(long l);
    STDMETHOD_(long,  get_Accum)(void);
    STDMETHOD_(void,  put_Opnd)(long l);
    STDMETHOD_(long,  get_Opnd)(void);
    STDMETHOD_(void,  put_Op)(short op);
    STDMETHOD_(short, get_Op)(void);
    STDMETHOD_(short, Eval)(void);
    STDMETHOD_(void,  Clear)(void);
    STDMETHOD_(void,  Display)(void);
    STDMETHOD_(void,  Quit)(void);
    STDMETHOD_(short, Button)(BSTR button);

    // the following method is internal, and not exposed for programmability
    int ButtonPush(int button);

    CArith(CCalc FAR* pcalc){
      m_pcalc = pcalc;
      Clear();
    }
    enum states { STATE_LOPND, STATE_OP, STATE_ROPND, STATE_EVAL };

private:
    CCalc FAR*  m_pcalc;

    short	m_op;
    long	m_opnd;
    long	m_accum;
    enum states m_state;
};


CLASS CCalc : public IUnknown {
public:
    friend CArith;

    static CCalc FAR* Create();

    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);

    CCalc();

#ifdef _MAC
    DialogPtr m_pdlg; 
#else
    HWND m_hwnd;
#endif
    CArith m_arith;

private:
    unsigned long m_refs;
    IUnknown FAR* m_punkStdDisp;
};

enum operators {
    OP_NONE = 0,
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV
};

// the following enum defines method indices used by the
// default IDispatch implementation - DispInvoke().
//
// Note: these must match the order of the preceeding declarations
//
enum IMETH_CARITH {
    IMETH_PUTACCUM = 0,
    IMETH_GETACCUM,
    IMETH_PUTOPERAND,
    IMETH_GETOPERAND,
    IMETH_PUTOPERATOR,
    IMETH_GETOPERATOR,
    IMETH_EVAL,
    IMETH_CLEAR,
    IMETH_DISPLAY,
    IMETH_QUIT,
    IMETH_BUTTON,

    // Define the "property" indices. these are defined to be
    // the first index in a set/get property method pair. These
    // definitions are used to build the METHODDATA that drives
    // our implementation of IDispatch. see cdisp.cpp.
    //
    IMETH_ACCUM    = IMETH_PUTACCUM,
    IMETH_OPERAND  = IMETH_PUTOPERAND,
    IMETH_OPERATOR = IMETH_PUTOPERATOR
};

// the following enum defines the IDs used by IDispatch
//
// Note: these values do *not* depend on order of declaration,
// but are sensitive to the kind of the method - ie, if a get/set
// method pair implements a property, then they need to share
// an ID.
//
// Note: by assigning "accum" the ID 'DISPID_VALUE', we are
// choosing to expose it as the default "value" property.
//
enum IDMEMBER_CARITH {
    IDMEMBER_ACCUM = DISPID_VALUE,	// the default property
    IDMEMBER_OPERAND,
    IDMEMBER_OPERATOR,
    IDMEMBER_EVAL,
    IDMEMBER_CLEAR,
    IDMEMBER_DISPLAY,
    IDMEMBER_QUIT,
    IDMEMBER_BUTTON
};


// the CCalc Class Factory
//
CLASS CCalcCF : public IClassFactory {
public:
    static IClassFactory FAR* Create();

    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);

    STDMETHOD(CreateInstance)(
      IUnknown FAR* punkOuter, REFIID riid, void FAR* FAR* ppv);
#ifdef _MAC
    STDMETHOD(LockServer)(unsigned long fLock);
#else
    STDMETHOD(LockServer)(BOOL fLock);
#endif

    CCalcCF() { m_refs = 1; }

private:
    unsigned long m_refs;
};

extern HRESULT InitOle(void);
extern HRESULT UninitOle(void);

extern CCalc FAR* g_pcalc;

