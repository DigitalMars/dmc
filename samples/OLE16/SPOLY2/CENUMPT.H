/***
*cenumpt.h
*
*  Copyright (C) 1992, Microsoft Corporation.  All Rights Reserved.
*  Information Contained Herein Is Proprietary and Confidential.
*
*Purpose:
*  Definition of the CEnumPoint class.
*
*Implementation Notes:
*
*****************************************************************************/

class CEnumPoint : public IEnumVARIANT
{
public:
	static HRESULT Create(SAFEARRAY FAR* psa, CEnumPoint FAR* FAR* ppenum);

	/* IUnknown methods */
	STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
	STDMETHOD_(unsigned long, AddRef)(void);
	STDMETHOD_(unsigned long, Release)(void);

	/* IEnumVARIANT methods */
	STDMETHOD(Next)(unsigned long celt, VARIANT FAR* rgvar, unsigned long FAR* pceltFetched);
	STDMETHOD(Skip)(unsigned long celt);
	STDMETHOD(Reset)(void);
	STDMETHOD(Clone)(IEnumVARIANT FAR* FAR* ppenum);

	CEnumPoint();

private:

	unsigned long m_refs;

	unsigned long m_celts;
	unsigned long m_iCurrent;
	SAFEARRAY FAR* m_psa;
};
