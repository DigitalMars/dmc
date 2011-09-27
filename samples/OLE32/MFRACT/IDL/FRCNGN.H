#include "rpc.h"
#include "rpcndr.h"

#ifndef __frcngn_h__
#define __frcngn_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IFractalEngine_FWD_DEFINED__
#define __IFractalEngine_FWD_DEFINED__
typedef interface IFractalEngine IFractalEngine;
#endif 	/* __IFractalEngine_FWD_DEFINED__ */


#include "unknwn.h"
#include "frhost.h"
void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFractalEngine_INTERFACE_DEFINED__
#define __IFractalEngine_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFractalEngine
 * at Sun Feb 05 16:07:02 1995
 * using MIDL 2.00.72
 ****************************************/
/* [unique][uuid][object][local] */ 



EXTERN_C const IID IID_IFractalEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IFractalEngine : public IUnknown
    {
    public:
        virtual HRESULT __stdcall Init( 
            IFractalHost __RPC_FAR *pfh) = 0;
        
        virtual HRESULT __stdcall SetDefaults( void) = 0;
        
        virtual HRESULT __stdcall SetProperties( 
            HWND hwnd) = 0;
        
        virtual HRESULT __stdcall GetExtent( 
            double __RPC_FAR *pdLeft,
            double __RPC_FAR *pdTop,
            double __RPC_FAR *pdRight,
            double __RPC_FAR *pdBottom) = 0;
        
        virtual HRESULT __stdcall SetExtent( 
            double dLeft,
            double dTop,
            double dRight,
            double dBottom) = 0;
        
        virtual HRESULT __stdcall SetGraphSize( 
            unsigned int uWidth,
            unsigned int uHeight) = 0;
        
        virtual HRESULT __stdcall Start( void) = 0;
        
        virtual HRESULT __stdcall Stop( void) = 0;
        
    };
    
#else 	/* C style interface */
    
    typedef struct IFractalEngineVtbl
    {
        
        HRESULT ( __stdcall __RPC_FAR *QueryInterface )( 
            IFractalEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( __stdcall __RPC_FAR *AddRef )( 
            IFractalEngine __RPC_FAR * This);
        
        ULONG ( __stdcall __RPC_FAR *Release )( 
            IFractalEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *Init )( 
            IFractalEngine __RPC_FAR * This,
            IFractalHost __RPC_FAR *pfh);
        
        HRESULT ( __stdcall __RPC_FAR *SetDefaults )( 
            IFractalEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *SetProperties )( 
            IFractalEngine __RPC_FAR * This,
            HWND hwnd);
        
        HRESULT ( __stdcall __RPC_FAR *GetExtent )( 
            IFractalEngine __RPC_FAR * This,
            double __RPC_FAR *pdLeft,
            double __RPC_FAR *pdTop,
            double __RPC_FAR *pdRight,
            double __RPC_FAR *pdBottom);
        
        HRESULT ( __stdcall __RPC_FAR *SetExtent )( 
            IFractalEngine __RPC_FAR * This,
            double dLeft,
            double dTop,
            double dRight,
            double dBottom);
        
        HRESULT ( __stdcall __RPC_FAR *SetGraphSize )( 
            IFractalEngine __RPC_FAR * This,
            unsigned int uWidth,
            unsigned int uHeight);
        
        HRESULT ( __stdcall __RPC_FAR *Start )( 
            IFractalEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *Stop )( 
            IFractalEngine __RPC_FAR * This);
        
    } IFractalEngineVtbl;
    
    interface IFractalEngine
    {
        CONST_VTBL struct IFractalEngineVtbl __RPC_FAR *lpVtbl;
    };
    
    

#ifdef COBJMACROS


#define IFractalEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFractalEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFractalEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFractalEngine_Init(This,pfh)	\
    (This)->lpVtbl -> Init(This,pfh)

#define IFractalEngine_SetDefaults(This)	\
    (This)->lpVtbl -> SetDefaults(This)

#define IFractalEngine_SetProperties(This,hwnd)	\
    (This)->lpVtbl -> SetProperties(This,hwnd)

#define IFractalEngine_GetExtent(This,pdLeft,pdTop,pdRight,pdBottom)	\
    (This)->lpVtbl -> GetExtent(This,pdLeft,pdTop,pdRight,pdBottom)

#define IFractalEngine_SetExtent(This,dLeft,dTop,dRight,dBottom)	\
    (This)->lpVtbl -> SetExtent(This,dLeft,dTop,dRight,dBottom)

#define IFractalEngine_SetGraphSize(This,uWidth,uHeight)	\
    (This)->lpVtbl -> SetGraphSize(This,uWidth,uHeight)

#define IFractalEngine_Start(This)	\
    (This)->lpVtbl -> Start(This)

#define IFractalEngine_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT __stdcall IFractalEngine_Init_Proxy( 
    IFractalEngine __RPC_FAR * This,
    IFractalHost __RPC_FAR *pfh);


void __RPC_STUB IFractalEngine_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_SetDefaults_Proxy( 
    IFractalEngine __RPC_FAR * This);


void __RPC_STUB IFractalEngine_SetDefaults_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_SetProperties_Proxy( 
    IFractalEngine __RPC_FAR * This,
    HWND hwnd);


void __RPC_STUB IFractalEngine_SetProperties_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_GetExtent_Proxy( 
    IFractalEngine __RPC_FAR * This,
    double __RPC_FAR *pdLeft,
    double __RPC_FAR *pdTop,
    double __RPC_FAR *pdRight,
    double __RPC_FAR *pdBottom);


void __RPC_STUB IFractalEngine_GetExtent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_SetExtent_Proxy( 
    IFractalEngine __RPC_FAR * This,
    double dLeft,
    double dTop,
    double dRight,
    double dBottom);


void __RPC_STUB IFractalEngine_SetExtent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_SetGraphSize_Proxy( 
    IFractalEngine __RPC_FAR * This,
    unsigned int uWidth,
    unsigned int uHeight);


void __RPC_STUB IFractalEngine_SetGraphSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_Start_Proxy( 
    IFractalEngine __RPC_FAR * This);


void __RPC_STUB IFractalEngine_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalEngine_Stop_Proxy( 
    IFractalEngine __RPC_FAR * This);


void __RPC_STUB IFractalEngine_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFractalEngine_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
