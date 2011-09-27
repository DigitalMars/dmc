// pssamp.cpp - sample proxy and stub

#define INITGUID

#include <windows.h>
#include <ole2.h>
#include <cobjps.h>
#include "coguid.h"

#pragma warning(disable:4355)

#include "odcsamp.h"
#include "pssamp.h"


// CLSID for proxy/stub objects; see pssamp.reg
DEFINE_GUID(CLSID_PSODCSample, 0xF783AFA0, 0x9801, 0x1068,
	0xAC, 0x1D, 0x00, 0x80, 0xC7, 0x0E, 0x55, 0x04);

/*
 *  IMPLEMENTATION of CPrxSample
 *
 */


// create unconnected CPrxSample; return controlling IProxy/IUnknokwn FAR*
IRpcProxy FAR* CPrxSample::Create(IUnknown FAR* pUnkOuter)
{
	CPrxSample FAR* pPrxSample;

	if ((pPrxSample = new CPrxSample(pUnkOuter)) == NULL)
		return NULL;

	return &pPrxSample->m_Proxy;
}


CPrxSample::CPrxSample(IUnknown FAR* pUnkOuter) : m_Proxy(this)
{
	// if not aggregated, point pUnkOuter to self (avoids if tests)
	if (pUnkOuter == NULL)
		pUnkOuter = &m_Proxy;

	m_refs = 1;
	m_pUnkOuter = pUnkOuter;
	m_pRpcChannel = NULL;
}


CPrxSample::~CPrxSample(void)
{
	m_Proxy.Disconnect();
}


// Methods for controlling unknown
STDMETHODIMP CPrxSample::CProxyImpl::QueryInterface(REFIID iid, LPVOID FAR* ppv)
{
	if (iid == IID_IUnknown || iid == IID_IRpcProxy)
		*ppv = (void FAR *)this;
	else if (iid ==  IID_IOLE2DevConfSample)
		*ppv = (void FAR *)m_pPrxSample;
	else
	{
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}

	// simplest way to be correct: always addref the pointer we will return;
	// easy since all interfaces here are derived from IUnknown.
	((IUnknown FAR*) *ppv)->AddRef();
	return NOERROR;
}

STDMETHODIMP_(ULONG) CPrxSample::CProxyImpl::AddRef(void)
{
	return ++m_pPrxSample->m_refs;
}

STDMETHODIMP_(ULONG) CPrxSample::CProxyImpl::Release(void)
{
	if (--m_pPrxSample->m_refs == 0)
	{
		delete m_pPrxSample;
		return 0;
	}
	return m_pPrxSample->m_refs;
}


// connect proxy to channel given
STDMETHODIMP CPrxSample::CProxyImpl::Connect(IRpcChannel FAR* pRpcChannel)
{
	if (m_pPrxSample->m_pRpcChannel != NULL)
		return ResultFromScode(E_UNEXPECTED);

	if (pRpcChannel == NULL)
		return ResultFromScode(E_INVALIDARG);

	(m_pPrxSample->m_pRpcChannel = pRpcChannel)->AddRef();
	return NOERROR;
}


// disconnect proxy from any current channel
STDMETHODIMP_(void) CPrxSample::CProxyImpl::Disconnect(void)
{
	if (m_pPrxSample->m_pRpcChannel)
	{
		m_pPrxSample->m_pRpcChannel->Release();
		m_pPrxSample->m_pRpcChannel = NULL;
	}
}



// IUnknown methods for external interface(s); always delegate
STDMETHODIMP CPrxSample::QueryInterface(REFIID iid, LPVOID FAR* ppv)
{
	return m_pUnkOuter->QueryInterface(iid, ppv);
}

STDMETHODIMP_(ULONG) CPrxSample::AddRef(void)
{
	return m_pUnkOuter->AddRef();
}

STDMETHODIMP_(ULONG) CPrxSample::Release(void)
{
	return m_pUnkOuter->Release();
}



// IOLE2DevConfSample interface methods

STDMETHODIMP CPrxSample::Method1(DWORD dw)
{
	HRESULT hrMarshal;
	HRESULT hrMethod;
	IRpcChannel FAR* pRpc = m_pRpcChannel;

	if (pRpc == NULL)
		return ResultFromScode(RPC_E_CONNECTION_TERMINATED);

	// format in: dw;
	// format out: hrMethod
	IStream FAR* pStmArgs;
	if ((hrMarshal = pRpc->GetStream(IID_IOLE2DevConfSample, IMETHOD_Method1,
				FALSE, FALSE, sizeof(DWORD), &pStmArgs)) != NOERROR)
		goto ErrExit;

	if ((hrMarshal = pStmArgs->Write(&dw, sizeof(dw), NULL)) != NOERROR)
		goto ReleaseStm;

	if ((hrMarshal = pRpc->Call(pStmArgs)) != NOERROR)
		goto ReleaseStm;

	if ((hrMarshal = CoUnmarshalHresult(pStmArgs, &hrMethod)) != NOERROR)
		goto ReleaseStm;

	pStmArgs->Release();
	return hrMethod;

ReleaseStm:
	pStmArgs->Release();
ErrExit:
	return PropagateResult(hrMarshal, RPC_E_CLIENT_CANTMARSHAL_DATA);
}


STDMETHODIMP CPrxSample::PassUnk(LPUNKNOWN pUnk)
{
	HRESULT hrMarshal;
	HRESULT hrMethod;
	IRpcChannel FAR* pRpc = m_pRpcChannel;

	if (pRpc == NULL)
		return ResultFromScode(RPC_E_CONNECTION_TERMINATED);

	// format in: pUnk
	// format out: hrMethod
	IStream FAR* pStmArgs;
	if ((hrMarshal = pRpc->GetStream(IID_IOLE2DevConfSample, IMETHOD_PassUnk,
				FALSE, FALSE, MARSHALINTERFACE_MIN, &pStmArgs)) != NOERROR)
		goto ErrExit;

	if ((hrMarshal = CoMarshalInterface(pStmArgs, IID_IUnknown, pUnk,
							0, 0, MSHLFLAGS_NORMAL)) != NOERROR)
		goto ReleaseStm;

	// NOTE: error handling is not quite complete; if the stub never
	// unmarshalled the interface, we should call CoReleaseMarshalData.

	if ((hrMarshal = pRpc->Call(pStmArgs)) != NOERROR)
		goto ReleaseStm;

	if ((hrMarshal = CoUnmarshalHresult(pStmArgs, &hrMethod)) != NOERROR)
		goto ReleaseStm;

	pStmArgs->Release();
	return hrMethod;

ReleaseStm:
	pStmArgs->Release();
ErrExit:
	return PropagateResult(hrMarshal, RPC_E_CLIENT_CANTMARSHAL_DATA);
}


STDMETHODIMP CPrxSample::GetUnk(LPUNKNOWN FAR* ppUnk)
{
	HRESULT hrMarshal;
	HRESULT hrMethod;
	IRpcChannel FAR* pRpc = m_pRpcChannel;

	*ppUnk = NULL;

	if (pRpc == NULL)
		return ResultFromScode(RPC_E_CONNECTION_TERMINATED);

	// format in: empty
	// format out: pUnk, hrMethod
	IStream FAR* pStmArgs;
	if ((hrMarshal = pRpc->GetStream(IID_IOLE2DevConfSample, IMETHOD_GetUnk,
				FALSE, FALSE, 0, &pStmArgs)) != NOERROR)
		goto ErrExit;

	if ((hrMarshal = pRpc->Call(pStmArgs)) != NOERROR)
		goto ReleaseStm;

	if ((hrMarshal = CoUnmarshalInterface(pStmArgs, IID_IUnknown, (LPVOID FAR*)ppUnk)) != NOERROR)
		goto ReleaseStm;

	if ((hrMarshal = CoUnmarshalHresult(pStmArgs, &hrMethod)) != NOERROR)
	{
		(*ppUnk)->Release();
		*ppUnk = NULL;
		goto ReleaseStm;
	}

	pStmArgs->Release();
	return hrMethod;

ReleaseStm:
	pStmArgs->Release();
ErrExit:
	return PropagateResult(hrMarshal, RPC_E_CLIENT_CANTMARSHAL_DATA);
}



/*
 *  IMPLEMENTATION of CStubSample
 *
 */

// create connected interface stub
HRESULT CStubSample::Create(IUnknown FAR* pUnkObject, IRpcStub FAR* FAR* ppStub)
{
	CStubSample FAR* pStubSample;

	*ppStub = NULL;

	if ((pStubSample = new CStubSample()) == NULL)
		return ResultFromScode(E_OUTOFMEMORY);

	// must do connect and ensure that the object supports our interface;
	// if this Create returns NOERROR, the object must support the interface
	HRESULT hresult;
	if ((hresult = pStubSample->Connect(pUnkObject)) != NOERROR)
	{
		pStubSample->Release();
		return hresult;
	}

	*ppStub = pStubSample;
	return NOERROR;
}


CStubSample::CStubSample(void)
{
	m_refs       = 1;
	m_pODCSample = NULL;
}


CStubSample::~CStubSample(void)
{
	Disconnect();
}


// controling unknown methods for interface stub
STDMETHODIMP CStubSample::QueryInterface(REFIID iid, LPVOID FAR* ppv)
{

	if (iid == IID_IUnknown || iid == IID_IRpcStub)
	{
		*ppv = this;
		++m_refs;
		return NOERROR;
	}
	else
	{
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

STDMETHODIMP_(ULONG) CStubSample::AddRef(void)
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CStubSample::Release(void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}

	return m_refs;
}


// connect interface stub to server object
STDMETHODIMP CStubSample::Connect(IUnknown FAR* pUnkObj)
{
	if (m_pODCSample)
		// call Disconnect first
		return ResultFromScode(E_UNEXPECTED);

	if (pUnkObj == NULL)
		return ResultFromScode(E_INVALIDARG);

	// NOTE: QI ensures out param is zero if error
	return pUnkObj->QueryInterface(IID_IOLE2DevConfSample, (LPVOID FAR*)&m_pODCSample);
}


// disconnect interface stub from server object
STDMETHODIMP_(void) CStubSample::Disconnect(void)
{
	if (m_pODCSample)
	{
		m_pODCSample->Release();
		m_pODCSample = NULL;
	}
}


// remove method call
STDMETHODIMP CStubSample::Invoke
	(REFIID iid, int iMethod, IStream FAR* pStmArgs,
			DWORD dwDestCtx, LPVOID lpvDestCtx)
{
	HRESULT     hresult;
	HRESULT     hrMethod;
	LARGE_INTEGER liZero;

	if (!m_pODCSample)
		return ResultFromScode(RPC_E_UNEXPECTED);

	if (iid != IID_IOLE2DevConfSample)
		return ResultFromScode(RPC_E_UNEXPECTED);

	LISet32(liZero,0);

	switch (iMethod)
	{
	case IMETHOD_Method1:
		// format on input: dw
		// format on output: hresult
		{
		DWORD dw;

		if ((hresult = pStmArgs->Read(&dw, sizeof(dw), NULL)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTUNMARSHAL_DATA);

		hrMethod = m_pODCSample->Method1(dw);

		if ((hresult = pStmArgs->Seek(liZero,STREAM_SEEK_SET, NULL)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		// write hresult result from LockServer
		if ((hresult = CoMarshalHresult(pStmArgs, hrMethod)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		return NOERROR;
		}

	case IMETHOD_PassUnk:
		// format on input: pUnk
		// format on output: hresult
		{
		IUnknown FAR* pUnk;

		if ((hresult = CoUnmarshalInterface(pStmArgs, IID_IUnknown, (LPVOID FAR*)&pUnk)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTUNMARSHAL_DATA);

		hrMethod = m_pODCSample->PassUnk(pUnk);

		pUnk->Release();

		if ((hresult = pStmArgs->Seek(liZero,STREAM_SEEK_SET, NULL)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		// write hresult result from LockServer
		if ((hresult = CoMarshalHresult(pStmArgs, hrMethod)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		return NOERROR;
		}

	case IMETHOD_GetUnk:
		// format on input: empty
		// format on output: pUnk, hresult
		{
		IUnknown FAR* pUnk;

		hrMethod = m_pODCSample->GetUnk(&pUnk);

		hresult = CoMarshalInterface(pStmArgs, IID_IUnknown, pUnk, 0, 0, MSHLFLAGS_NORMAL);
		pUnk->Release();

		if (hresult != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		// write hresult result from LockServer
		if ((hresult = CoMarshalHresult(pStmArgs, hrMethod)) != NOERROR)
			return PropagateResult(hresult, RPC_E_SERVER_CANTMARSHAL_DATA);

		return NOERROR;
		}

	default:
		// unknown method
		return ResultFromScode(RPC_E_UNEXPECTED);
	}
}


// return TRUE if we support given interface
STDMETHODIMP_(BOOL) CStubSample::IsIIDSupported(REFIID iid)
{
	// if we are connected, we have already checked for this interface;
	// if we are not connected, it doesn't matter.
	return iid == IID_IOLE2DevConfSample;
}


// returns number of refs we have to object
STDMETHODIMP_(ULONG) CStubSample::CountRefs(void)
{
	// return 1 if connected; 0 if not.
	return m_pODCSample != NULL;
}




/*
 *  IMPLEMENTATION of CPSFactory
 *
 */

CPSFactory::CPSFactory(void)
{
	m_refs = 1;
}



// controlling unknown for PSFactory
STDMETHODIMP CPSFactory::QueryInterface(REFIID iid, LPVOID FAR* ppv)
{
	if (iid == IID_IUnknown || iid == IID_IPSFactory)
	{
		*ppv = this;
		++m_refs;
		return NOERROR;
	}
	else
	{
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

STDMETHODIMP_(ULONG) CPSFactory::AddRef(void)
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CPSFactory::Release(void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}

	return m_refs;
}


// create proxy for given interface
STDMETHODIMP CPSFactory::CreateProxy(IUnknown FAR* pUnkOuter, REFIID iid,
	IRpcProxy FAR* FAR* ppProxy, void FAR* FAR* ppv)
{
	IRpcProxy FAR* pProxy;
	HRESULT     hresult;

	*ppProxy = NULL;
	*ppv = NULL;

	if (iid != IID_IOLE2DevConfSample)
		return ResultFromScode(E_NOINTERFACE);

	if ((pProxy = CPrxSample::Create(pUnkOuter)) == NULL)
		return ResultFromScode(E_OUTOFMEMORY);

	hresult = pProxy->QueryInterface(iid, ppv);

	if (hresult == NOERROR)
		*ppProxy = pProxy;          // transfer ref to caller
	else
		pProxy->Release();          // free proxy just created

	return hresult;
}



// create stub for given interface
STDMETHODIMP CPSFactory::CreateStub(REFIID iid, IUnknown FAR* pUnkServer, IRpcStub FAR* FAR* ppStub)
{
	if (iid != IID_IOLE2DevConfSample)
		return ResultFromScode(E_NOINTERFACE);

	return CStubSample::Create(pUnkServer, ppStub);
}


// main driving api; called by compobj.dll
STDAPI  DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
{
	*ppv = NULL;

	if (riid != IID_IUnknown && riid != IID_IPSFactory)
		return ResultFromScode(E_NOINTERFACE);

	if (rclsid != CLSID_PSODCSample)
		return ResultFromScode(E_UNEXPECTED);

	return (*ppv = (LPVOID)new CPSFactory()) != NULL
		? NOERROR : ResultFromScode(E_OUTOFMEMORY);
}


// this dummy function is used to avoid a copy of the environment variables
// and also allows us to get rid of the windows heap.
extern "C" void _setenvp(void) { }

extern "C"
BOOL PASCAL FAR LibMain(HINSTANCE hinst, HANDLE segDS, UINT cbHeapSize, LPSTR lpCmdLine)
{
	// Leave our DS unlocked when we're not running
	UnlockData( 0 );

	return TRUE;
}


extern "C"
VOID FAR PASCAL _export WEP (BOOL fSystemExit)
{
}
