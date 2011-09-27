/***
*dspcalc2.cpp
*
*  Copyright (C) 1993, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
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

#include <windows.h>
#include <ole2.h>
#if !defined(WIN32)
#include <olenls.h>
#endif
#include <dispatch.h>

#include "resource.h"
#include "dspcalc2.h"


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

	// first try to load the type library from the information in the registry
	if((hresult = LoadRegTypeLib(LIBID_DspCalc2, 1, 0, 0x0409, &ptlib)) != NOERROR){

	  #define TLB_NAME "dspcalc2.tlb"

	  // if it wasn't registered, try to load it from the path/current directory
	  // if this succeeds, it will have registered the type library for us
	  // for the next time.
	  if((hresult = LoadTypeLib(TLB_NAME, &ptlib)) != NOERROR){
		MessageBox(NULL, "error loading TypeLib", "dspcalc2", MB_OK);
		goto LError0;
	  }

	}

	if((hresult = ptlib->GetTypeInfoOfGuid(IID_ICalculator, &ptinfo)) != NOERROR){
	  MessageBox(NULL, "error accessing TypeInfo", "dspcalc2", MB_OK);
	  goto LError0;
	}

	// Create and aggregate with an instance of the default
	// implementation of IDispatch that is initialized with our
	// TypeInfo.
	//
	hresult = CreateStdDispatch(
	  pcalc,                            // controlling unknown
	  &pcalc->m_arith,                  // vtable* to dispatch on
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
	if(riid == IID_IUnknown){
	  *ppv = this;
	}else
	if(riid == IID_IDispatch || riid == IID_DCalculator){
	  return m_punkStdDisp->QueryInterface(IID_IDispatch, ppv);
	}else
	if(riid == IID_ICalculator){
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
	  PostQuitMessage(0);
	  delete this;
	  return 0;
	}
	return m_refs;
}


STDMETHODIMP
CCalc::CArith::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
	return m_pcalc->QueryInterface(riid, ppv);
}


STDMETHODIMP_(ULONG)
CCalc::CArith::AddRef()
{
	return m_pcalc->AddRef();
}


STDMETHODIMP_(ULONG)
CCalc::CArith::Release()
{
	return m_pcalc->Release();
}


//---------------------------------------------------------------------
//                       Arithmetic features
//---------------------------------------------------------------------


STDMETHODIMP_(void)
CCalc::CArith::Clear()
{
	m_opnd = 0;
	m_accum = 0;
	m_op = OP_NONE;
	m_state = STATE_LOPND;
}

STDMETHODIMP_(void)
CCalc::CArith::put_Accum(long l)
{
	m_accum = l;
}


STDMETHODIMP_(long)
CCalc::CArith::get_Accum()
{
	return m_accum;
}


STDMETHODIMP_(void)
CCalc::CArith::put_Opnd(long l)
{
	m_opnd = l;
}


STDMETHODIMP_(long)
CCalc::CArith::get_Opnd()
{
	return m_opnd;
}


STDMETHODIMP_(void)
CCalc::CArith::put_Op(OPERATORS op)
{
	m_op = op;
}


STDMETHODIMP_(OPERATORS)
CCalc::CArith::get_Op()
{
	return m_op;
}


STDMETHODIMP_(BOOL)
CCalc::CArith::Eval()
{
	switch(m_op){
	case OP_NONE:
	  m_accum = m_opnd;
	  break;
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

	m_opnd = m_accum;
	m_state = STATE_EVAL;

	return TRUE;
}


//---------------------------------------------------------------------
//                       User Interface features
//---------------------------------------------------------------------


/***
*void CCalc::CArith::Display()
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
CCalc::CArith::Display()
{
	VARIANT var;

	VariantInit(&var);
	V_VT(&var) = VT_I4;
	V_I4(&var) = (m_state != STATE_EVAL) ? m_opnd : m_accum;
	VariantChangeType(&var, &var, 0, VT_BSTR);
	SetDlgItemText(m_pcalc->m_hwnd, IDC_DISPLAY, V_BSTR(&var));
	VariantClear(&var);
}


STDMETHODIMP_(BOOL)
CCalc::CArith::Button(BSTR bstrButton)
{
	int i, button;

static struct {
	char ch;
	int idc;
} NEAR rgIdcOfCh[] = {
	  { '+', IDC_PLUS   }
	, { '-', IDC_MINUS  }
	, { '*', IDC_MULT   }
	, { '/', IDC_DIV    }
	, { 'C', IDC_CLEAR  }
	, { 'c', IDC_CLEAR  }
	, { '=', IDC_EQUALS }
	, { '0', IDC_ZERO   }
	, { '1', IDC_ONE    }
	, { '2', IDC_TWO    }
	, { '3', IDC_THREE  }
	, { '4', IDC_FOUR   }
	, { '5', IDC_FIVE   }
	, { '6', IDC_SIX    }
	, { '7', IDC_SEVEN  }
	, { '8', IDC_EIGHT  }
	, { '9', IDC_NINE   }
	, { -1 , -1         }
};

	// if the string is more that 1 character long, then we know its wrong.
	if(SysStringLen(bstrButton) > 1)
	  return FALSE;

	// translate button string into control ID
	for(i = 0;; ++i){
	  if(rgIdcOfCh[i].ch == -1)
		return FALSE;
	  if(rgIdcOfCh[i].ch == bstrButton[0]){
		button = rgIdcOfCh[i].idc;
		break;
	  }
	}

	return ButtonPush(button);
}


// the following method is internal, and not exposed for programmability
BOOL
CCalc::CArith::ButtonPush(int button)
{
	if(button >= IDC_ZERO && button <= IDC_NINE){

	  long lVal = button - IDC_ZERO;

	  switch(m_state){
	  case STATE_EVAL:
		m_accum = 0;
		m_opnd = lVal;
		m_state = STATE_LOPND;
		break;
	  case STATE_OP:
		m_accum = m_opnd;
		m_opnd = lVal;
		m_state = STATE_LOPND;
		break;
	  case STATE_LOPND:
		m_opnd  = (m_opnd * 10) + lVal;
		m_state = STATE_LOPND;
		break;
	  }

	}else if(button >= IDC_PLUS && button <= IDC_DIV){

	  if(m_state == STATE_EVAL){

		 m_opnd = m_accum;
		 m_accum = 0;

	  }else if (m_op != OP_NONE && m_state == STATE_LOPND){

		 Eval();

	  }

	  m_state = STATE_OP;
	  m_op    = (OPERATORS)(button - IDC_PLUS + OP_PLUS);

	}else if(button == IDC_EQUALS){

	  Eval();

	  m_op = OP_NONE;
	  m_opnd = 0;

	}else if (button == IDC_CLEAR){

	  Clear();

	}

	SendMessage(m_pcalc->m_hwnd, BM_SETSTATE, 1, 0L);
	SendMessage(m_pcalc->m_hwnd, BM_SETSTATE, 0, 0L);

	Display();

	return TRUE;
}

/***
*void CCalc::CArith::Quit()
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
CCalc::CArith::Quit()
{
	PostQuitMessage(0);
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
	if(riid == IID_IUnknown || riid == IID_IClassFactory){
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

	return g_pcalc->QueryInterface(riid, ppv);
}


STDMETHODIMP
CCalcCF::LockServer(BOOL fLock)
{
	return NOERROR;
}
