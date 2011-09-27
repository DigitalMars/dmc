/*
 *  hello.cpp
 *
 *  See comment in hello.h
 *
 */


#include "common.h"
#include "resource.h"
#include "hello.h"





IUnknown FAR*
CreateDispatchInterface(
  IUnknown FAR* punkController,
  void FAR* pProgInterface)
{
	HRESULT hresult;
	ITypeLib FAR* ptlib;
	ITypeInfo FAR* ptinfo;
	IUnknown FAR* punkStdDisp;

	// Load the the hellotlb type library.
	hresult = LoadRegTypeLib(LIBID_HELLOTLB, 1, 0, 0x0409, &ptlib);
	if (hresult != NOERROR) {
	  #define TLB_NAME "hello.tlb"

	  // if it wasn't registered, try to load it from the path/current directory
	  // if this succeeds, it will have registered the type library for us
	  // for the next time.
	  if((hresult = LoadTypeLib(TLB_NAME, &ptlib)) != NOERROR)
	 return NULL;
	}

	// Get Dispatch TypeInfo from the type library.
	// Exit if an error occurs.
	hresult = ptlib->GetTypeInfoOfGuid(IID_IHello, &ptinfo);
	if (hresult != NOERROR)
	return NULL;

	ptlib->Release();

	// Create a standard dispatch object.
	// Release the pointer to the previously created TypeInfo.
	// Exit if an error occurred.
	hresult = CreateStdDispatch(punkController,
					pProgInterface,
				ptinfo,
					&punkStdDisp);
	ptinfo->Release();
	if (hresult != NOERROR)
	return NULL;


	// If execution has reached this point, then no errors occurred.
	return punkStdDisp;
}





/*
 *  CHello::CHello(void)
 *
 */
CHello::CHello()
{
	m_refs = 1;
	m_disp_interface = NULL;
	m_prog_interface = new CHelloPro;
	m_prog_interface->m_phello = this;
	m_bstrHelloMsg = SysAllocString("Hello, world");
}





/*
 *  CHello::~CHello(void)
 *
 */
CHello::~CHello()
{
	SysFreeString(m_bstrHelloMsg);
	delete m_prog_interface;
}





/*
 *  CHello *CHello::Create(void)
 *
 *  Creates a new instance of the CHello object.
 *
 *      Returns a pointer to the newly created instance or
 *  NULL if the creation fails.
 *
 */
CHello FAR*
CHello::Create()
{
	CHello FAR* phello;
	IUnknown FAR* punkStdDisp;


	// Create an instance of CHello.  Exit if an error occurs.
	phello = new FAR CHello();
	if(phello == NULL)
	  return NULL;

	punkStdDisp = CreateDispatchInterface((IUnknown FAR*) phello,
					  phello->m_prog_interface);
	if (punkStdDisp == NULL) {
	  phello->Release();
	  return NULL;
	}


	// If execution has reached this point, then no errors have occurred.
	// Save the standard dispatch item and return the new instance.
	phello->m_disp_interface = punkStdDisp;
	return phello;
}





/*
 *  void CHello::ProcessCommand(WPARAM param)
 *
 *  Process a windows WM_COMMAND with the specified parameter.
 *
 */
void CHello::ProcessCommand(WPARAM wparam)
{
	switch(wparam) {
	case IDC_SAYHELLO:
	// The user pressed the "Say Hello" button.
	m_prog_interface->SayHello();
	break;
	}
}





//---------------------------------------------------------------------
//                        IUnknown methods
//---------------------------------------------------------------------


/*  CHello::QueryInterface(...)
 *
 *  Someone wants to know if CHello support the interface identified
 *  by riid.  CHello supports IUnknown and we defer to a member
 *      (m_disp_interfaceatch) and thus support IDispatch.
 *
 *  CHello doesn't support any other interfaces.
 *
 */
STDMETHODIMP
CHello::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
	// riid identifies an interface type.  Do we support it?

	if (riid == IID_IUnknown)
	// Someone is asking us for our controlling IUnknown.
	// CHello is that IUnknown, so "this" is the answer.
	*ppv = this;
	else if (riid == IID_IDispatch || riid == IID_DHello)
		// Someone is asking us for our IDispatch or dispinterface.
		// We simply defer to the standard dispatch interface
		// we created earlier.
		return m_disp_interface->QueryInterface(IID_IDispatch, ppv);
	else if (riid == IID_IHello)
	// Someone is asking us for our IHelloPro interface.
	// We simply defer to the standard dispatch interface
	// we created earlier.
	*ppv = &m_prog_interface;
	else {
	// They must have asked for another interface -- something
	// we don't support.  Report the error.
	*ppv = NULL;
	return ResultFromScode(E_NOINTERFACE);
	}

	// If execution reached this point then no error occurred.
	AddRef();
	return NOERROR;
}






STDMETHODIMP_(ULONG)
CHello::AddRef()
{
	return ++m_refs;
}





STDMETHODIMP_(ULONG)
CHello::Release()
{
	if(--m_refs == 0)
	{
	  if(m_disp_interface != NULL)
		m_disp_interface->Release();
	  PostQuitMessage(0);
	  delete this;
	  return 0;
	}
	return m_refs;
}




//---------------------------------------------------------------------
//                      The CHello Class Factory
//---------------------------------------------------------------------


CHelloCF::CHelloCF()
{
	m_refs = 1;
}





IClassFactory FAR*
CHelloCF::Create()
{
	return new FAR CHelloCF();
}





STDMETHODIMP
CHelloCF::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
	if(riid == IID_IUnknown || riid == IID_IClassFactory) {
	  AddRef();
	  *ppv = this;
	  return NOERROR;
	}
	*ppv = NULL;
	return ResultFromScode(E_NOINTERFACE);
}





STDMETHODIMP_(ULONG)
CHelloCF::AddRef()
{
	return ++m_refs;
}





STDMETHODIMP_(ULONG)
CHelloCF::Release()
{
	if(--m_refs == 0) {
	   delete this;
	   return 0;
	}
	return m_refs;
}





STDMETHODIMP
CHelloCF::CreateInstance(
	IUnknown FAR* punkOuter,
	REFIID riid,
	void FAR* FAR* ppv)
{
	extern CHello FAR* g_phello;


	return g_phello->QueryInterface(riid, ppv);
}





STDMETHODIMP
CHelloCF::LockServer(BOOL fLock)
{
	return NOERROR;
}
