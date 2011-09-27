#include "stdafx.h"
#include "utils.h"
#include "realtdoc.h"
#include "realtime.h"


/*
 *  Initialize the count of objects to zero.
 */

CControllingUnknown::CControllingUnknown( LPUNKNOWN pUnkOuter )
	: m_refs(1)
{
	if (m_pUnkOuter = pUnkOuter) pUnkOuter->AddRef();
}

CControllingUnknown::~CControllingUnknown( void )
{
}

STDMETHODIMP CControllingUnknown::QueryInterface (THIS_ REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	return ResultFromScode( E_NOINTERFACE );
}


STDMETHODIMP_(ULONG) CControllingUnknown::AddRef (THIS)
{
	if (m_pUnkOuter) return m_pUnkOuter->AddRef();
	return ++m_refs;
}



STDMETHODIMP_(ULONG) CControllingUnknown::Release (THIS)
{
	if (m_pUnkOuter) return m_pUnkOuter->Release();
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}
