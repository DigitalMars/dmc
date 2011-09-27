#include "rpc.h"
#include "rpcndr.h"

#ifndef __qudngn_h__
#define __qudngn_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IQuadrantEngine_FWD_DEFINED__
#define __IQuadrantEngine_FWD_DEFINED__
typedef interface IQuadrantEngine IQuadrantEngine;
#endif 	/* __IQuadrantEngine_FWD_DEFINED__ */


#include "qudcln.h"
#include "frhost.h"
void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IQuadrantEngine_INTERFACE_DEFINED__
#define __IQuadrantEngine_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IQuadrantEngine
 * at Sun Feb 05 16:07:18 1995
 * using MIDL 2.00.72
 ****************************************/
/* [unique][uuid][object][local] */ 



EXTERN_C const IID IID_IQuadrantEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IQuadrantEngine : public IUnknown
    {
    public:
        virtual HRESULT __stdcall Init( 
            IQuadrantClient __RPC_FAR *pqc,
            IFractalHost __RPC_FAR *pfh) = 0;
        
        virtual HRESULT __stdcall UseBoundingBoxes( 
            BOOL fFlag) = 0;
        
        virtual HRESULT __stdcall SetGraphSize( 
            unsigned int uWidth,
            unsigned int uHeight) = 0;
        
        virtual HRESULT __stdcall Start( void) = 0;
        
        virtual HRESULT __stdcall Stop( void) = 0;
        
        virtual HRESULT __stdcall Reset( void) = 0;
        
    };
    
#else 	/* C style interface */
    
    typedef struct IQuadrantEngineVtbl
    {
        
        HRESULT ( __stdcall __RPC_FAR *QueryInterface )( 
            IQuadrantEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( __stdcall __RPC_FAR *AddRef )( 
            IQuadrantEngine __RPC_FAR * This);
        
        ULONG ( __stdcall __RPC_FAR *Release )( 
            IQuadrantEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *Init )( 
            IQuadrantEngine __RPC_FAR * This,
            IQuadrantClient __RPC_FAR *pqc,
            IFractalHost __RPC_FAR *pfh);
        
        HRESULT ( __stdcall __RPC_FAR *UseBoundingBoxes )( 
            IQuadrantEngine __RPC_FAR * This,
            BOOL fFlag);
        
        HRESULT ( __stdcall __RPC_FAR *SetGraphSize )( 
            IQuadrantEngine __RPC_FAR * This,
            unsigned int uWidth,
            unsigned int uHeight);
        
        HRESULT ( __stdcall __RPC_FAR *Start )( 
            IQuadrantEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *Stop )( 
            IQuadrantEngine __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *Reset )( 
            IQuadrantEngine __RPC_FAR * This);
        
    } IQuadrantEngineVtbl;
    
    interface IQuadrantEngine
    {
        CONST_VTBL struct IQuadrantEngineVtbl __RPC_FAR *lpVtbl;
    };
    
    

#ifdef COBJMACROS


#define IQuadrantEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuadrantEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQuadrantEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQuadrantEngine_Init(This,pqc,pfh)	\
    (This)->lpVtbl -> Init(This,pqc,pfh)

#define IQuadrantEngine_UseBoundingBoxes(This,fFlag)	\
    (This)->lpVtbl -> UseBoundingBoxes(This,fFlag)

#define IQuadrantEngine_SetGraphSize(This,uWidth,uHeight)	\
    (This)->lpVtbl -> SetGraphSize(This,uWidth,uHeight)

#define IQuadrantEngine_Start(This)	\
    (This)->lpVtbl -> Start(This)

#define IQuadrantEngine_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IQuadrantEngine_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT __stdcall IQuadrantEngine_Init_Proxy( 
    IQuadrantEngine __RPC_FAR * This,
    IQuadrantClient __RPC_FAR *pqc,
    IFractalHost __RPC_FAR *pfh);


void __RPC_STUB IQuadrantEngine_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantEngine_UseBoundingBoxes_Proxy( 
    IQuadrantEngine __RPC_FAR * This,
    BOOL fFlag);


void __RPC_STUB IQuadrantEngine_UseBoundingBoxes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantEngine_SetGraphSize_Proxy( 
    IQuadrantEngine __RPC_FAR * This,
    unsigned int uWidth,
    unsigned int uHeight);


void __RPC_STUB IQuadrantEngine_SetGraphSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantEngine_Start_Proxy( 
    IQuadrantEngine __RPC_FAR * This);


void __RPC_STUB IQuadrantEngine_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantEngine_Stop_Proxy( 
    IQuadrantEngine __RPC_FAR * This);


void __RPC_STUB IQuadrantEngine_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantEngine_Reset_Proxy( 
    IQuadrantEngine __RPC_FAR * This);


void __RPC_STUB IQuadrantEngine_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQuadrantEngine_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
