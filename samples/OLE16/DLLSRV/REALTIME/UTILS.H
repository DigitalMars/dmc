/*
 *  This program, like the DataView DLL server, uses an
 *  implementation of the IUnknown interface as a base class for
 *  several aother classes.  It differs from that
 *  CControllingUnknown class in DataView in that it has a static
 *  count of objects that is incremented by the constructor and
 *  decremented by the destructor.  This automatically gives the
 *  application a count of all extant objects (in classes derived
 *  from this base class).
 */



class CControllingUnknown :  public IUnknown
{
public:
	CControllingUnknown( LPUNKNOWN pUnkOuter );
	virtual ~CControllingUnknown( void );

	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

private:
	ULONG m_refs;
protected:
	LPUNKNOWN m_pUnkOuter;
};
