#ifndef _IADVSINK_H_
#define _IADVSINK_H_

//#if defined(WIN32) && !defined(_UNICODE)
//    #define CImpIAdviseSink CImpIAdviseSinkA
//    #define CIDataObjectDlg CIDataObjectDlgA
//#endif

class FAR CImpIAdviseSink : public IAdviseSink
{
protected:
    ULONG               m_cRef;      //Interface reference count.
    CIDataObjectDlg FAR* m_pIDOD ;       //Back pointer to the application

public:
    CImpIAdviseSink(CIDataObjectDlg FAR*);
    ~CImpIAdviseSink(void);

    STDMETHODIMP QueryInterface(REFIID, LPVOID FAR *);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    //We only implement OnDataChange for now.
    STDMETHODIMP_(void)  OnDataChange(LPFORMATETC, LPSTGMEDIUM);
    STDMETHODIMP_(void)  OnViewChange(DWORD, LONG);
    STDMETHODIMP_(void)  OnRename(LPMONIKER);
    STDMETHODIMP_(void)  OnSave(void);
    STDMETHODIMP_(void)  OnClose(void);
};

#endif //_IADVSINK_H_
