/***
*crempoly.h
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  This file defines the CRemPoly remote polygon object.
*
*Implementation Notes:
*
*****************************************************************************/


class CRemPoint : public IUnknown {
public:
	static CRemPoint FAR* Create(void);

	// standard IUnknown methods
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(unsigned long, AddRef)(void);
	STDMETHOD_(unsigned long, Release)(void);

	short GetX();
	void SetX(short x);
	short GetY();
	void SetY(short y);

private:
	CRemPoint() { m_refs = 0; }

	unsigned long m_refs;
	IDispatch FAR* m_pdisp;
};


class CRemPoly : public IUnknown {
public:
	static HRESULT Create(CLSID clsid, CRemPoly FAR* FAR*);

	// IUnknown methods
	//
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
	STDMETHOD_(unsigned long, AddRef)(void);
	STDMETHOD_(unsigned long, Release)(void);

	// Introduced methods
	//
	HRESULT Draw(void);
	HRESULT Dump(void);
	HRESULT Reset(void);
	HRESULT AddPoint(short x, short y);
	HRESULT EnumPoints(IEnumVARIANT FAR* FAR* ppenum);
	HRESULT GetXOrigin(short FAR* pxorg);
	HRESULT SetXOrigin(short xorg);
	HRESULT GetYOrigin(short FAR* pyorg);
	HRESULT SetYOrigin(short yorg);
	HRESULT GetWidth(short FAR* pwidth);
	HRESULT SetWidth(short width);

	HRESULT get_red(short FAR* psRed);
	HRESULT set_red(short sRed);
	HRESULT get_green(short FAR* psGreen);
	HRESULT set_green(short sGreen);
	HRESULT get_blue(short FAR* psBlue);
	HRESULT set_blue(short sBlue);

private:
	CRemPoly();

	HRESULT get_i2(DISPID dispid, short FAR* ps);
	HRESULT set_i2(DISPID dispid, short s);

	unsigned long m_refs;
	IDispatch FAR* m_pdisp;

	// NOTE: this enumeration exists simply to allow us to symbolicly
	// index the member name and id arrays (m_rgid and m_rgszMethods).
	// This doesn't (necessarrily) have any connection to the vtable
	// indices, it *only* needs to correspond correctly to the m_rgid
	// and m_rgszMethods arrays.
	//
	enum CREMPOLY_METHODS {
	IMETH_CREMPOLY_DRAW = 0,
	IMETH_CREMPOLY_DUMP,
	IMETH_CREMPOLY_RESET,
	IMETH_CREMPOLY_ADDPOINT,
	IMETH_CREMPOLY_ENUMPOINTS,
	IMETH_CREMPOLY_GETXORIGIN,
	IMETH_CREMPOLY_SETXORIGIN,
	IMETH_CREMPOLY_GETYORIGIN,
	IMETH_CREMPOLY_SETYORIGIN,
	IMETH_CREMPOLY_GETWIDTH,
	IMETH_CREMPOLY_SETWIDTH,
	IMETH_CREMPOLY_GETRED,
	IMETH_CREMPOLY_SETRED,
	IMETH_CREMPOLY_GETGREEN,
	IMETH_CREMPOLY_SETGREEN,
	IMETH_CREMPOLY_GETBLUE,
	IMETH_CREMPOLY_SETBLUE,
	IMETH_CREMPOLY_MAX
	};

	// member IDs - these are used by IDispatch::Invoke to identify the
	// method or property on the remote object we accessing.
	//
	DISPID m_rgdispid[IMETH_CREMPOLY_MAX];

	// member names - these are used to learn the member IDs when we
	// connect to the remote object.
	//
	static char FAR* m_rgszMethods[IMETH_CREMPOLY_MAX];
};
