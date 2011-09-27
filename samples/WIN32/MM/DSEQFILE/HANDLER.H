/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
extern HMODULE ghModule; // = NULL; // global HMODULE/HINSTANCE for resource access

// Comment out the following line to disable the custom marshalling code....
#define CUSTOMMARSHAL

/*      -       -       -       -       -       -       -       -       */

#define CFactoryImpl    CI      // MSC7 Can't handle long exported names
#define CAVIFileImpl    CF      // MSC7 Can't handle long exported names
#define CAVIStreamImpl  CS      // MSC7 Can't handle long exported names

/* Remove warning of using object during initialization. */
#pragma warning(disable:4355)

/*      -       -       -       -       -       -       -       -       */

#define implement       struct
#define implementations private

/*
** _StdClassImplementations
** Defines the standard implementations for a class object.
*/

#define _StdClassImplementations(Impl)  \
        STDMETHODIMP QueryInterface(const IID FAR& riid, void FAR* FAR* ppv);   \
        STDMETHODIMP_(ULONG) AddRef();  \
        STDMETHODIMP_(ULONG) Release()


// non-member function prototypes
EXTERN_C BOOL PASCAL FAR LibMain(
        HINSTANCE       hInstance,
        HGLOBAL segDS,
        UINT    cbHeapSize,
        LPCSTR  pszCmdLine);
EXTERN_C BOOL FAR PASCAL WEP(
        BOOL    fSystemExit);
STDAPI DllCanUnloadNow(
        void);
STDAPI DllGetClassObject(
        const CLSID FAR&        rclsid,
        const IID FAR&  riid,
        void FAR* FAR*  ppv);
LPSTR FAR FileName(
        LPCSTR lszPath);

/*      -       -       -       -       -       -       -       -       */
class FAR CAVIFileCF : IClassFactory {
public:
        static HRESULT Create(const CLSID FAR& rclsid, const IID FAR& riid, void FAR* FAR* ppv);
private:
        CAVIFileCF(const CLSID FAR& rclsid, IUnknown FAR* FAR* ppUnknown);
        _StdClassImplementations(CFactoryImpl);
        CFactoryImpl(CAVIFileCF FAR* pAVIFileCF);
        STDMETHODIMP CreateInstance(IUnknown FAR* pUnknownOuter, const IID FAR& riid, void FAR* FAR* ppv);
        STDMETHODIMP LockServer(BOOL fLock);

        ULONG   m_refs;
        CLSID   m_clsid;
};

/*      -       -       -       -       -       -       -       -       */

class FAR CAVIFile {
public:
        static HRESULT Create(IUnknown FAR* pUnknownOuter, const IID FAR& riid, void FAR* FAR* ppv);
        HRESULT NEAR PASCAL LoadFrame(LONG lPos);
        HRESULT NEAR PASCAL WriteFrame(LONG lPos, LPVOID lp, LONG cb);

private:
        CAVIFile(IUnknown FAR* pUnknownOuter, IUnknown FAR* FAR* ppUnknown);

implementations:
        implement CUnknownImpl : IUnknown {
        public:
                _StdClassImplementations(
                        CUnknownImpl);
                CUnknownImpl(
                        CAVIFile FAR* pAVIFile);
        private:
                CAVIFile FAR*   m_pAVIFile;
                ULONG   m_refs;
        };
        implement CAVIFileImpl : IAVIFile {
        public:
                _StdClassImplementations(
                        CAVIFileImpl);
                CAVIFileImpl(
                        CAVIFile FAR* pAVIFile);
                ~CAVIFileImpl();
                STDMETHODIMP Info(
                        THIS_
                        AVIFILEINFOW * pfi,
                        LONG lSize);
                STDMETHODIMP GetStream(
                        THIS_
                        PAVISTREAM FAR * ppStream,
                        DWORD fccType,
                        LONG lParam);
                STDMETHODIMP CreateStream(
                        THIS_
                        PAVISTREAM FAR * ppStream,
                        AVISTREAMINFOW * psi);
                STDMETHODIMP WriteData(
                        THIS_
                        DWORD ckid,
                        LPVOID lpData,
                        LONG cbData);
                STDMETHODIMP ReadData(
                        THIS_
                        DWORD ckid,
                        LPVOID lpData,
                        LONG FAR *lpcbData);
                STDMETHODIMP EndRecord(
                        THIS);
                STDMETHODIMP DeleteStream(THIS_
                                            DWORD fccType,
                                            LONG lParam);
        private:
                // private functions here?
                CAVIFile FAR*   m_pAVIFile;
        };
        implement CAVIStreamImpl : IAVIStream {
        public:
                _StdClassImplementations(
                        CAVIStreamImpl);
                CAVIStreamImpl(
                        CAVIFile FAR* pAVIFile);
                ~CAVIStreamImpl();
                STDMETHODIMP Create(
                        THIS_
                        LPARAM lParam1,
                        LPARAM lParam2);
                STDMETHODIMP Info(
                        THIS_
                        AVISTREAMINFOW FAR * psi,
                        LONG lSize);
                STDMETHODIMP_(LONG) FindSample(
                        THIS_
                        LONG lPos,
                        LONG lFlags);
                STDMETHODIMP ReadFormat(
                        THIS_
                        LONG lPos,
                        LPVOID lpFormat,
                        LONG FAR *cbFormat);
                STDMETHODIMP SetFormat(
                        THIS_
                        LONG lPos,
                        LPVOID lpFormat,
                        LONG cbFormat);
                STDMETHODIMP Read(
                        THIS_
                        LONG lStart,
                        LONG lSamples,
                        LPVOID lpBuffer,
                        LONG cbBuffer,
                        LONG FAR * plBytes,
                        LONG FAR * plSamples);
                STDMETHODIMP Write(
                        THIS_
                        LONG lStart,
                        LONG lSamples,
                        LPVOID lpBuffer,
                        LONG cbBuffer,
                        DWORD dwFlags,
                        LONG FAR *plSampWritten,
                        LONG FAR *plBytesWritten);
                STDMETHODIMP Delete(
                        THIS_
                        LONG lStart,
                        LONG lSamples);
                STDMETHODIMP ReadData(
                        THIS_
                        DWORD fcc,
                        LPVOID lp,
                        LONG FAR *lpcb);
                STDMETHODIMP WriteData(
                        THIS_
                        DWORD fcc,
                        LPVOID lp,
                        LONG cb);
                STDMETHODIMP SetInfo(AVISTREAMINFOW FAR * lpInfo, LONG cbInfo);
        private:
                // private functions here?
                CAVIFile FAR*   m_pAVIFile;
        };

        implement CPersistFileImpl : IPersistFile {
        public:
            CPersistFileImpl(CAVIFile FAR* pAVIFile);
            STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppv);
            STDMETHODIMP_(ULONG) AddRef();
            STDMETHODIMP_(ULONG) Release();

            // *** IPersist methods ***
            STDMETHODIMP GetClassID (LPCLSID lpClassID);

            // *** IPersistFile methods ***
            STDMETHODIMP IsDirty ();
            STDMETHODIMP Load (LPCOLESTR lpszFileName, DWORD grfMode);
            STDMETHODIMP Save (LPCOLESTR lpszFileName, BOOL fRemember);
            STDMETHODIMP SaveCompleted (LPCOLESTR lpszFileName);
            STDMETHODIMP GetCurFile (LPOLESTR FAR * lplpszFileName);

        private:
            CAVIFile FAR*       m_pAVIFile;
        };


#ifdef CUSTOMMARSHAL
        implement CMarshalImpl : IMarshal {
        public:
                _StdClassImplementations(
                        CMarshalImpl);
                CMarshalImpl(
                        CAVIFile FAR* pAVIFile);
                // *** IMarshal methods ***
                STDMETHODIMP GetUnmarshalClass (
                        THIS_
                        REFIID riid,
                        LPVOID pv,
                        DWORD dwDestContext,
                        LPVOID pvDestContext,
                        DWORD mshlflags,
                        LPCLSID pCid);
                STDMETHODIMP GetMarshalSizeMax (
                        THIS_
                        REFIID riid,
                        LPVOID pv,
                        DWORD dwDestContext,
                        LPVOID pvDestContext,
                        DWORD mshlflags,
                        LPDWORD pSize);
                STDMETHODIMP MarshalInterface(
                        THIS_
                        LPSTREAM pStm,
                        REFIID riid,
                        LPVOID pv,
                        DWORD dwDestContext,
                        LPVOID pvDestContext,
                        DWORD mshlflags);
                STDMETHODIMP UnmarshalInterface (
                        THIS_
                        LPSTREAM pStm,
                        REFIID riid,
                        LPVOID FAR* ppv);
                STDMETHODIMP ReleaseMarshalData (
                        THIS_
                        LPSTREAM pStm);
                STDMETHODIMP DisconnectObject (
                THIS_
                DWORD dwReserved);
        private:
                CAVIFile FAR*   m_pAVIFile;
        };
#endif
public:
        CUnknownImpl    m_Unknown;
        CAVIFileImpl    m_AVIFile;
        CAVIStreamImpl  m_AVIStream;
        CPersistFileImpl m_Persist;
#ifdef CUSTOMMARSHAL
        CMarshalImpl    m_Marshal;
#endif
public:
        IUnknown FAR*   m_pUnknownOuter;

        //
        // DIB Sequence instance data
        //
        DWORD   dwFirstFrame;
        DWORD   dwMaxValue;
#ifndef _MAX_PATH
        #define _MAX_PATH       260
#else
        #if _MAX_PATH !=        260     // Error check
                #error
        #endif
#endif
        char    achFilenameTemplate[_MAX_PATH];
        LONG    lCurFrame;
        AVIFILEINFO     finfo;
        AVISTREAMINFOW sinfo;

        BOOL    fStreamPresent;
        LPVOID  lpFormat;       // stream format
        LONG    cbFormat;
        LONG    cbFormatBuffer;
        LPVOID  lpFrame;
        LONG    cbFrame;
        LONG    cbFrameBuffer;
        BOOL    fDirty;
        UINT    mode;
};

/*      -       -       -       -       -       -       -       -       */

/*
** The usage counter keeps track of the overall usage of objects based on
** implementations provided by the component. This allows one to determine
** when the implementation is no longer in use.
*/

extern UINT     uUseCount;
extern BOOL     fLocked;

/*      -       -       -       -       -       -       -       -       */


DEFINE_AVIGUID(CLSID_DIBSEQFileReader, 0x00020006, 0, 0);
