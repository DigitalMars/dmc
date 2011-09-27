/*  hellopro.h
 *
 *  This is the header for CHelloProgrammability, which is the
 *  programmability interface for CHello objects.
 *
 *  This is the user interface that a programmer who uses our
 *  object will be using.
 *
 */

#include "common.h"


// Forward declaration.
CLASS CHello;

#include "ihello.h" // Include MkTypLib-created interface
			// description of _IHello.



CLASS CHelloPro : public _IHello
{
  public:

	// Standard OLE stuff.
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	STDMETHOD_(void, put_HelloMessage)(THIS_ BSTR b);
	STDMETHOD_(BSTR, get_HelloMessage)(THIS);
	STDMETHOD_(BSTR, SayHello)(THIS);

	CHello FAR* m_phello;
};
