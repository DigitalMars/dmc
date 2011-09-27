/*  hellopro.cpp
 *
 *      CHelloProgrammability is the programmability interface for
 *  the CHello object.  it contains the implementation of every
 *  member of the CHello class which is visible to a programmer
 *  using the CHello object.
 *
 */

#include "common.h"
#include "resource.h"
#include "hello.h" // This also gets hellopro.h



/*      SetHelloMsg
 *
 *      SetHelloMsg sets the message that the hello object
 *      says when the SayHello method is called.
 *
 */
STDMETHODIMP_(void)
CHelloPro::put_HelloMessage(BSTR b)
{
	SysReAllocString(&(m_phello->m_bstrHelloMsg), b);
}




/*      GetHelloMsg
 *
 *  GetHelloMsg gets the message that the hello object
 *  says when the SayHello method is called.
 *
 */
STDMETHODIMP_(BSTR)
CHelloPro::get_HelloMessage()
{
	return SysAllocString(m_phello->m_bstrHelloMsg);
}




/*      SayHello
 *
 *  SayHello writes the string "Hello world." in the dialog's
 *  text box and returns the same string.
 *
 */
STDMETHODIMP_(BSTR)
CHelloPro::SayHello()
{
	BSTR b;

	b = SysAllocString(m_phello->m_bstrHelloMsg);
	SetDlgItemText(m_phello->m_hwnd, IDC_HELLOAREA, b);
	return b;
}





/*  IUnknown methods
 *
 *  Simply defer to pur controlling IUnknown.
 *
 */
STDMETHODIMP
CHelloPro::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
	return m_phello->QueryInterface(riid, ppv);
}

STDMETHODIMP_(ULONG)
CHelloPro::AddRef()
{
	return m_phello->AddRef();
}

STDMETHODIMP_(ULONG)
CHelloPro::Release()
{
	return m_phello->Release();
}
