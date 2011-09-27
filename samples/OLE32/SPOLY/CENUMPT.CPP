/*** 
*cenumpt.cpp
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
*  This module implements the CEnumPoint class.
*
*
*Implementation Notes:
*
*****************************************************************************/

#include "hostenv.h"
#include "cenumpt.h"


CEnumPoint::CEnumPoint()
{
    m_refs = 0;

    m_psa = NULL;
    m_celts = 0;
    m_iCurrent = 0;
}


/***
*HRESULT CEnumPoint::Create(SAFEARRAY*, CEnumPoint**)
*Purpose:
*  This routine creates a CPoint enumerator from the given
*  (1 X N) SafeArray of CPoint IDispatch pointers.
*
*Entry:
*  psa = pointer to a SafeArray of VARIANTs
*
*Exit:
*  return value = HRESULT
*
*  *ppenum = pointer to a CPoint enumerator
*
***********************************************************************/
HRESULT
CEnumPoint::Create(SAFEARRAY FAR* psa, CEnumPoint FAR* FAR* ppenum)
{
    long lBound;
    HRESULT hresult;
    CEnumPoint FAR* penum;


    // Verify that the SafeArray is the proper shape.
    //
    if(SafeArrayGetDim(psa) != 1)
      return ResultFromScode(E_INVALIDARG);

    hresult = SafeArrayGetLBound(psa, 1, &lBound);
    if(FAILED(hresult))
      return hresult;

    if(lBound != 0)
      return ResultFromScode(E_INVALIDARG);

    penum = new FAR CEnumPoint();
    if(penum == NULL)
      return ResultFromScode(E_OUTOFMEMORY);
    penum->AddRef();

    hresult = SafeArrayGetUBound(psa, 1, &lBound);
    if(FAILED(hresult))
      goto LError0;

    penum->m_psa = psa;
    penum->m_celts = lBound + 1;

    *ppenum = penum;

    return NOERROR;

LError0:;
    penum->Release();

    return hresult;
}


//---------------------------------------------------------------------
//                        IUnknown methods
//---------------------------------------------------------------------


STDMETHODIMP
CEnumPoint::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{

    if(!IsEqualIID(riid, IID_IUnknown))
      if(!IsEqualIID(riid, IID_IEnumVARIANT)) {
        *ppv = NULL;		      
        return ResultFromScode(E_NOINTERFACE);
    }
	
    *ppv = this;
    AddRef();
    return NOERROR;
}


STDMETHODIMP_(unsigned long)
CEnumPoint::AddRef()
{
    return ++m_refs;
}


STDMETHODIMP_(unsigned long)
CEnumPoint::Release()
{
    if(--m_refs == 0){
      if(m_psa != NULL)
	SafeArrayDestroy(m_psa);
      delete this;
      return 0;
    }
    return m_refs;
}


//---------------------------------------------------------------------
//                        IEnumVARIANT methods
//---------------------------------------------------------------------


/***
*HRESULT CEnumPoint::Next(unsigned long, VARIANT*, unsigned long*)
*Purpose:
*  Attempt to get the next 'celt' items in the enumeration sequence.
*
*Entry:
*  celt = the number of elements to get
*
*Exit:
*  return value = HRESULT
*    S_OK
*    S_FALSE - the end of the sequence was reached
*
*  rgvar = array of the next 'celt' items
*  *pceltFetched = count of the elements actually fetched.
*
***********************************************************************/
STDMETHODIMP
CEnumPoint::Next(
    unsigned long celt,
    VARIANT FAR* rgvar,
    unsigned long FAR* pceltFetched)
{
    long ix;
    unsigned int i;
    HRESULT hresult;


    for(i = 0; i < celt; ++i)
      VariantInit(&rgvar[i]);

    for(i = 0; i < celt; ++i){
      if(m_iCurrent == m_celts){
        hresult = ResultFromScode(S_FALSE);
	goto LDone;
      }

      ix = m_iCurrent++;
      hresult = SafeArrayGetElement(m_psa, &ix, &rgvar[i]);
      if(FAILED(hresult))
	goto LError0;
    }

    hresult = NOERROR;

LDone:;
    if (pceltFetched != NULL)
      *pceltFetched = i;

    return hresult;

LError0:;

    for(i = 0; i < celt; ++i)
      VariantClear(&rgvar[i]);

    return hresult;
}


/***
*HRESULT CEnumPoint::Skip(unsigned long)
*Purpose:
*  Attempt to skip over the next 'celt' elements in the enumeration
*  sequence.
*
*Entry:
*  celt = the count of elements to skip
*
*Exit:
*  return value = HRESULT
*    S_OK
*    S_FALSE -  the end of the sequence was reached
*
***********************************************************************/
STDMETHODIMP
CEnumPoint::Skip(unsigned long celt)
{
    m_iCurrent += celt;

    if(m_iCurrent > m_celts)
     m_iCurrent = m_celts;

    return (m_iCurrent == m_celts)
      ? ResultFromScode(S_FALSE) : NOERROR;
}


/***
*HRESULT CEnumPoint::Reset(void)
*Purpose:
*  Reset the enumeration sequence back to the beginning.
*
*Entry:
*  None
*
*Exit:
*  return value = SHRESULT CODE
*    S_OK
*
***********************************************************************/
STDMETHODIMP
CEnumPoint::Reset()
{
    m_iCurrent = 0;

    return NOERROR; 
}


/***
*HRESULT CEnumPoint::Clone(IEnumVARIANT**)
*Purpose:
*  Retrun a CPoint enumerator with exactly the same state as the
*  current one.
*
*Entry:
*  None
*
*Exit:
*  return value = HRESULT
*    S_OK
*    E_OUTOFMEMORY
*
***********************************************************************/
STDMETHODIMP
CEnumPoint::Clone(IEnumVARIANT FAR* FAR* ppenum)
{
    HRESULT hresult;
    SAFEARRAY FAR* psa;
    CEnumPoint FAR* penum;

    hresult = SafeArrayCopy(m_psa, &psa);
    if(FAILED(hresult))
      return hresult;

    hresult = CEnumPoint::Create(psa, &penum);
    if(FAILED(hresult))
      goto LError0;

    // Assert(penum->m_celts == m_celts);

    penum->m_iCurrent = m_iCurrent;

    *ppenum = penum;

    return NOERROR;

LError0:
    SafeArrayDestroy(psa);

    return hresult;
}
