/*
 *  hello.h
 *
 *  Contains the following:
 *
 *  * The CHello class
 *      - Creation
 *      - Creating the standard dispatch object
 *      - Deletion
 *      - Reference count maintenance
 *      - IUnknown methods
 *      * The CHelloCF class, which creates instances of CHello.
 *      - Creation
 *      - Deletion
 *      - Reference count maintenance
 *      - IUnknown methods
 */

#include "common.h"
#include "clsid.h"
#include "hellopro.h"

#pragma warning(disable:4355)



/*  The CHello class.
 *
 */
CLASS CHello : public IUnknown
{

 public:
	CHello();
	~CHello();
	static CHello FAR* Create();
	void ProcessCommand(WPARAM wparam); // Process a Windows WM_COMMAND message

	// Standard OLE stuff.
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

	HWND m_hwnd;            // The window to which we draw.
	BSTR m_bstrHelloMsg;

 private:
	ULONG m_refs;           // Reference count.
	IUnknown FAR* m_disp_interface; // Pointer to the standard dispatch object.
	CHelloPro FAR* m_prog_interface;  // What a programmer sees.
};





/*  The class factory for CHello.
 *
 */
CLASS CHelloCF : public IClassFactory
{
  public:
	CHelloCF();
	static IClassFactory FAR* Create();

	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

	STDMETHOD(CreateInstance)(  IUnknown FAR* punkOuter,
				REFIID riid,
				void FAR* FAR* ppv);
	STDMETHOD(LockServer)(BOOL fLock);

  private:
	ULONG m_refs;
};
