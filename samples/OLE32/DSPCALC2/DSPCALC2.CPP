/*** 
*dspcalc2.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1993-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  This module implements the basic user interface and arithmetic
*  functionality of the IDispatch calculator.
*
*  The implementation of IDispatch is via aggregation with an
*  instance of the "standard" IDispatch implementation, which is
*  initialized with a TypeInfo loaded from the TypeLib that was
*  constructed from the ODL description of the calculator.
*
*Implementation Notes:
*
*****************************************************************************/

#include "dspcalc2.h"

CCalc FAR* g_pcalc = NULL;

unsigned long g_dwCCalcCF = 0;
unsigned long g_dwRegisterCCalc = 0;

#ifdef _MAC	    
extern Boolean g_fQuit;
#endif //_MAC


/***
*CCalc *CCalc::Create(void)
*Purpose:
*  Create an instance of the IDispatch calculator, load the
*  TypeInfo that describes the exposed functionality and
*  aggregate with an instance of CStdDispatch that has been
*  initialized with this TypeInfo.
*
*Entry:
*  None
*
*Exit:
*  return value = CCalc*, NULL if the creation failed.
*
***********************************************************************/
CCalc FAR*
CCalc::Create()
{
    HRESULT hresult;
    CCalc FAR* pcalc;
    ITypeLib FAR* ptlib;
    ITypeInfo FAR* ptinfo;
    IUnknown FAR* punkStdDisp;

    ptlib = NULL;
    ptinfo = NULL;

    if((pcalc = new FAR CCalc()) == NULL)
      return NULL;
    pcalc->AddRef();
    
    // first try to load the type library from the information in the registry
    if((hresult = LoadRegTypeLib(LIBID_DspCalc2, 1, 0, 0x0409, &ptlib)) != NOERROR){

      #define TLB_NAME OLESTR("dspcalc2.tlb")

      // if it wasn't registered, try to load it from the path/current directory
      // if this succeeds, it will have registered the type library for us
      // for the next time.
      if((hresult = LoadTypeLib(TLB_NAME, &ptlib)) != NOERROR){
#ifndef _MAC	    
        MessageBox(NULL, TSTR("error loading TypeLib"),
		   TSTR("dspcalc2"), MB_OK);
#endif	 
        goto LError0;
      }

    }

    if((hresult = ptlib->GetTypeInfoOfGuid(IID_ICalculator, &ptinfo)) != NOERROR){
#ifndef _MAC	    
      MessageBox(NULL, TSTR("error accessing TypeInfo"), 
	         TSTR("dspcalc2"), MB_OK);
#endif	 
      goto LError0;
    }

    // Create and aggregate with an instance of the default
    // implementation of IDispatch that is initialized with our
    // TypeInfo.
    //
    hresult = CreateStdDispatch(
      pcalc,                            // controlling unknown
      &(pcalc->m_arith),                // vtable* to dispatch on
      ptinfo,
      &punkStdDisp);

    if(hresult != NOERROR)
      goto LError0;

    pcalc->m_punkStdDisp = punkStdDisp;

    ptinfo->Release();
    ptlib->Release();

    return pcalc;

LError0:;
    pcalc->Release();
    if(ptinfo != NULL)
      ptinfo->Release();
    if(ptlib != NULL)
      ptlib->Release();
    return NULL;
}


//---------------------------------------------------------------------
//                        IUnknown methods
//---------------------------------------------------------------------


STDMETHODIMP
CCalc::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(IsEqualIID(riid, IID_IUnknown)){
      *ppv = this;
    }else
    if(IsEqualIID(riid, IID_IDispatch) ||
       IsEqualIID(riid, IID_DCalculator)){
      return m_punkStdDisp->QueryInterface(IID_IDispatch, ppv);
    }else 
    if(IsEqualIID(riid, IID_ICalculator)){
      *ppv = &m_arith;
    }else {
      *ppv = NULL;	    
      return ResultFromScode(E_NOINTERFACE);
    }

    AddRef();
    return NOERROR;
}


STDMETHODIMP_(ULONG)
CCalc::AddRef()
{
    return ++m_refs;
}


STDMETHODIMP_(ULONG)
CCalc::Release()
{
    if(--m_refs == 0){
      if(m_punkStdDisp != NULL)
	m_punkStdDisp->Release();
#ifndef _MAC
      PostQuitMessage(0);
#endif
      delete this;
      return 0;
    }
    return m_refs;
}


STDMETHODIMP
CArith::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    return m_pcalc->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG)
CArith::AddRef()
{
    return m_pcalc->AddRef();
}


STDMETHODIMP_(ULONG)
CArith::Release()
{
    return m_pcalc->Release();
}


//---------------------------------------------------------------------
//                       Arithmetic features
//---------------------------------------------------------------------


STDMETHODIMP_(void)
CArith::Clear()
{
    m_opnd = 0;
    m_accum = 0;
    m_op = OP_NONE;
    m_state = STATE_LOPND;
}

STDMETHODIMP_(void)
CArith::put_Accum(long l)
{
    m_accum = l;
}


STDMETHODIMP_(long)
CArith::get_Accum()
{
    return m_accum;
}


STDMETHODIMP_(void)
CArith::put_Opnd(long l)
{
    m_opnd = l;
}


STDMETHODIMP_(long)
CArith::get_Opnd()
{
    return m_opnd;
}


STDMETHODIMP_(void)
CArith::put_Op(OPERATORS op)
{
    m_op = op;
}


STDMETHODIMP_(OPERATORS)
CArith::get_Op()
{
    return m_op;
}


STDMETHODIMP_(VARIANT_BOOL)
CArith::Eval()
{
    if(m_op == OP_NONE)
      return FALSE;

    switch(m_op){
    case OP_PLUS:
      m_accum += m_opnd;
      break;
    case OP_MINUS:
      m_accum -= m_opnd;
      break;
    case OP_MULT:
      m_accum *= m_opnd;
      break;
    case OP_DIV:
      m_accum = (m_opnd == 0) ? 0 : (m_accum / m_opnd);
      break;
    default:
      // ASSERT(UNREACHED);
      return FALSE;
      
    }

    m_state = STATE_EVAL;

    return VARIANT_TRUE;
}


//---------------------------------------------------------------------
//                       User Interface features
//---------------------------------------------------------------------


/***
*void CArith::Display()
*Purpose:
*  Display the contents of the register currently being edited.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
STDMETHODIMP_(void)
CArith::Display()
{
    VARIANT var;

    VariantInit(&var);
    V_VT(&var) = VT_I4;
    V_I4(&var) = (m_state == STATE_ROPND) ? m_opnd : m_accum;
    VariantChangeType(&var, &var, 0, VT_BSTR);

    #ifdef _MAC
    {
      Rect rcItem;
      Handle hItem;
      char str[255];
      short sItemKind;

      strcpy(str, V_BSTR(&var));
      GetDItem(m_pcalc->m_pdlg, IDC_DISPLAY, &sItemKind, &hItem, &rcItem);
      SetIText(hItem, c2pstr(str));
    }
#else
    SetDlgItemText(m_pcalc->m_hwnd, IDC_DISPLAY, STRING(V_BSTR(&var)));
#endif

    VariantClear(&var);
}


STDMETHODIMP_(VARIANT_BOOL)
CArith::Button(SAFEARRAY FAR * psa)
{
    int i, button;

static struct {
    OLECHAR ch;
    int idc;
} NEAR rgIdcOfCh[] = {
      { OLESTR('+'), IDC_PLUS   }
    , { OLESTR('-'), IDC_MINUS  }
    , { OLESTR('*'), IDC_MULT   }
    , { OLESTR('/'), IDC_DIV    }
    , { OLESTR('C'), IDC_CLEAR  }
    , { OLESTR('c'), IDC_CLEAR  }
    , { OLESTR('='), IDC_EQUALS }
    , { OLESTR('0'), IDC_ZERO   }
    , { OLESTR('1'), IDC_ONE    }
    , { OLESTR('2'), IDC_TWO    }
    , { OLESTR('3'), IDC_THREE  }
    , { OLESTR('4'), IDC_FOUR   }
    , { OLESTR('5'), IDC_FIVE   }
    , { OLESTR('6'), IDC_SIX    }
    , { OLESTR('7'), IDC_SEVEN  }
    , { OLESTR('8'), IDC_EIGHT  }
    , { OLESTR('9'), IDC_NINE   }
    , { (OLECHAR)-1 , -1         }
};
    LONG saIndex, saUbound;
    VARIANT varButton;

    // Since this is a vararg function, we should be given a 1-dimensional
    // array with 0 for the lower bound. The array could be uninitialized
    // if 0 args were passed to us -- this call will give an error in this case.
    if (SafeArrayGetUBound(psa, 1, &saUbound) != NOERROR)
      return FALSE;	// most likely 0 args were passed to us

    for (saIndex = 0; saIndex <= saUbound; saIndex++) {

      // get next parameter
      if (SafeArrayGetElement(psa, &saIndex, &varButton) != NOERROR)
	return FALSE;

      // convert it to a string in-place
      if (VariantChangeType(&varButton, &varButton, 0, VT_BSTR) != NOERROR)
	goto Error;

      // if the string is more that 1 character long, then we know its wrong.
      if(SysStringLen(varButton.bstrVal) > 1)
        goto Error;

      // translate button string into control ID
      for(i = 0;; ++i){
        if(rgIdcOfCh[i].ch == -1)
	  goto Error;
        if(rgIdcOfCh[i].ch == varButton.bstrVal[0]){
	  button = rgIdcOfCh[i].idc;
	  break;
        }
      }

      VariantClear(&varButton);		// done with the parameter

      if (!ButtonPush(button))
	return FALSE;

    } // for

    return VARIANT_TRUE;	// success

Error:
    VariantClear(&varButton);
    return FALSE;	// failure
}


// the following method is internal, and not exposed for programmability
BOOL
CArith::ButtonPush(int button)
{
    if(button >= IDC_ZERO && button <= IDC_NINE){

      long lVal = button - IDC_ZERO;

      switch(m_state){
      case STATE_EVAL:
	m_accum = lVal;
	m_state = STATE_LOPND;
	break;
      case STATE_OP:
	m_opnd = lVal;
	m_state = STATE_ROPND;
	break;
      case STATE_LOPND:
	m_accum = (m_accum * 10) + lVal;
	break;
      case STATE_ROPND:
	m_opnd  = (m_opnd * 10) + lVal;
	break;
      }

    }else if(button >= IDC_PLUS && button <= IDC_DIV){

      if(m_state == STATE_LOPND){
	m_opnd  = m_accum;
	m_state = STATE_OP;
	m_op    = (OPERATORS)(button - IDC_PLUS + OP_PLUS);
      }

    }else if(button == IDC_EQUALS){

      if(m_state > STATE_LOPND)
        Eval();

    }else if (button == IDC_CLEAR){

      Clear();

    } else {

      return 0; // unknown button

    }
	    

    // Flash the button

#ifdef _MAC
    {
      Rect rcItem;
      long lDummy;
      Handle hItem;
      short sItemKind;

      GetDItem(m_pcalc->m_pdlg, button, &sItemKind, &hItem, &rcItem);
      HiliteControl((ControlHandle)hItem, 1);
      Delay(6, &lDummy);
      HiliteControl((ControlHandle)hItem, 0);
    }
#else
    SendMessage(m_pcalc->m_hwnd, BM_SETSTATE, 1, 0L);
    SendMessage(m_pcalc->m_hwnd, BM_SETSTATE, 0, 0L);
#endif

    // Update the calculator display

    Display();

    return TRUE;
}

/***
*void CArith::Quit()
*Purpose:
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
STDMETHODIMP_(void)
CArith::Quit()
{
#ifndef _MAC
    PostQuitMessage(0);
#else
    g_fQuit = TRUE;
#endif
}


//---------------------------------------------------------------------
//                      The CCalc Class Factory
//---------------------------------------------------------------------


IClassFactory FAR*
CCalcCF::Create()
{
    return new FAR CCalcCF();
}


STDMETHODIMP
CCalcCF::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory)){
      AddRef();
      *ppv = this;
      return NOERROR;
    }
    *ppv = NULL;
    return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG)
CCalcCF::AddRef()
{
    return ++m_refs;
}


STDMETHODIMP_(ULONG)
CCalcCF::Release()
{
    if(--m_refs == 0){
      delete this;
      return 0;
    }
    return m_refs;
}


STDMETHODIMP
CCalcCF::CreateInstance(
    IUnknown FAR* punkOuter,
    REFIID riid,
    void FAR* FAR* ppv)
{
extern CCalc FAR* g_pcalc;

    UNUSED(punkOuter);
    return g_pcalc->QueryInterface(riid, ppv);
}


STDMETHODIMP
#ifdef _MAC
CCalcCF::LockServer(unsigned long fLock)
#else
CCalcCF::LockServer(BOOL fLock)
#endif
{
    UNUSED(fLock);
    return NOERROR;
}

#ifdef _MAC
struct regentry{
    char *szKey;
    char *szValue;
} g_rgregentry[] = {

      { "CLSID\\{00020469-0000-0000-C000-000000000046}",
	"OLE Automation DspCalc2 1.0 Application" }

    , { "CLSID\\{00020469-0000-0000-C000-000000000046}\\LocalServer",
	"DCL2" }

    , { "CLSID\\{00020469-0000-0000-C000-000000000046}\\ProgID",
	"Dspcalc2.Application" }

    , { "CLSID\\{00020469-0000-0000-C000-000000000046}\\InprocHandler",
	"OLE2:Def$DefFSet" }

    , { "DCL2", "{00020469-0000-0000-C000-000000000046}" }

    , { "Dspcalc2.Application\\CLSID",
	"{00020469-0000-0000-C000-000000000046}" }

};

HRESULT
EnsureRegistration()
{
    HKEY hkey;

    if(RegOpenKey(HKEY_CLASSES_ROOT, "DCL2", &hkey) == NOERROR){
      RegCloseKey(hkey);
      return NOERROR;
    }

    for(int i = 0; i < DIM(g_rgregentry); ++i){
      if(RegSetValue(HKEY_CLASSES_ROOT, g_rgregentry[i].szKey, REG_SZ, g_rgregentry[i].szValue, 0) != ERROR_SUCCESS)
        return ResultFromScode(E_FAIL);
    }

    return NOERROR;
}
#endif //_MAC


/***
*HRESULT InitOle(void)
*Purpose:
*  Initialize Ole, and register our class factories.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
HRESULT
InitOle()
{
    HRESULT hresult;
    IClassFactory FAR* pcf;


    if((hresult = OleInitialize(NULL)) != NOERROR)
      goto LError0;

#ifdef _MAC
    if((hresult = EnsureRegistration()) != NOERROR)
      goto LError0;
#endif

    // create the single global instance of CCalc
    if((g_pcalc = CCalc::Create()) == NULL){
      hresult = ResultFromScode(E_OUTOFMEMORY);
      goto LError0;
    }

    if((pcf = CCalcCF::Create()) == NULL)
      goto LError1;

    hresult = CoRegisterClassObject(
      CLSID_CCalc2,
      pcf,
      CLSCTX_LOCAL_SERVER,
      REGCLS_MULTIPLEUSE,
      &g_dwCCalcCF);
    if(FAILED(hresult))
      goto LError2;

    hresult = RegisterActiveObject(
      g_pcalc, CLSID_CCalc2, NULL, &g_dwRegisterCCalc);
    if(FAILED(hresult))
      goto LError2;

    pcf->Release();

    return NOERROR;

LError2:;
    pcf->Release();

LError1:;
    UninitOle();

LError0:;
    return hresult;
}


HRESULT
UninitOle()
{
    if(g_dwRegisterCCalc != 0)
      RevokeActiveObject(g_dwRegisterCCalc, NULL);

    if(g_dwCCalcCF != 0)
      CoRevokeClassObject(g_dwCCalcCF);

    // cause the remaining typeinfo to be released
    if(g_pcalc != NULL)
      g_pcalc->Release();

    OleUninitialize();

    return NOERROR;
}
