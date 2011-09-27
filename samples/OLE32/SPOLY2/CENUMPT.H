/*** 
*cenumpt.h
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
