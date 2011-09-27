#include "rpc.h"
#include "rpcndr.h"

#ifndef __qudcln_h__
#define __qudcln_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IQuadrantClient_FWD_DEFINED__
#define __IQuadrantClient_FWD_DEFINED__
typedef interface IQuadrantClient IQuadrantClient;
#endif 	/* __IQuadrantClient_FWD_DEFINED__ */


#include "unknwn.h"
void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IQuadrantClient_INTERFACE_DEFINED__
#define __IQuadrantClient_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IQuadrantClient
 * at Sun Feb 05 16:07:32 1995
 * using MIDL 2.00.72
 ****************************************/
/* [unique][uuid][object][local] */ 



EXTERN_C const IID IID_IQuadrantClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IQuadrantClient : public IUnknown
    {
    public:
        virtual HRESULT __stdcall ComputePoint( 
            unsigned int __RPC_FAR *puColor,
            int x,
            int y) = 0;
        
        virtual HRESULT __stdcall DoneDrawingGraph( void) = 0;
        
    };
    
#else 	/* C style interface */
    
    typedef struct IQuadrantClientVtbl
    {
        
        HRESULT ( __stdcall __RPC_FAR *QueryInterface )( 
            IQuadrantClient __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( __stdcall __RPC_FAR *AddRef )( 
            IQuadrantClient __RPC_FAR * This);
        
        ULONG ( __stdcall __RPC_FAR *Release )( 
            IQuadrantClient __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *ComputePoint )( 
            IQuadrantClient __RPC_FAR * This,
            unsigned int __RPC_FAR *puColor,
            int x,
            int y);
        
        HRESULT ( __stdcall __RPC_FAR *DoneDrawingGraph )( 
            IQuadrantClient __RPC_FAR * This);
        
    } IQuadrantClientVtbl;
    
    interface IQuadrantClient
    {
        CONST_VTBL struct IQuadrantClientVtbl __RPC_FAR *lpVtbl;
    };
    
    

#ifdef COBJMACROS


#define IQuadrantClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuadrantClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQuadrantClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQuadrantClient_ComputePoint(This,puColor,x,y)	\
    (This)->lpVtbl -> ComputePoint(This,puColor,x,y)

#define IQuadrantClient_DoneDrawingGraph(This)	\
    (This)->lpVtbl -> DoneDrawingGraph(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT __stdcall IQuadrantClient_ComputePoint_Proxy( 
    IQuadrantClient __RPC_FAR * This,
    unsigned int __RPC_FAR *puColor,
    int x,
    int y);


void __RPC_STUB IQuadrantClient_ComputePoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IQuadrantClient_DoneDrawingGraph_Proxy( 
    IQuadrantClient __RPC_FAR * This);


void __RPC_STUB IQuadrantClient_DoneDrawingGraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQuadrantClient_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
