#include "rpc.h"
#include "rpcndr.h"

#ifndef __frhost_h__
#define __frhost_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IFractalHost_FWD_DEFINED__
#define __IFractalHost_FWD_DEFINED__
typedef interface IFractalHost IFractalHost;
#endif 	/* __IFractalHost_FWD_DEFINED__ */


#include "unknwn.h"
void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFractalHost_INTERFACE_DEFINED__
#define __IFractalHost_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IFractalHost
 * at Sun Feb 05 16:06:45 1995
 * using MIDL 2.00.72
 ****************************************/
/* [unique][uuid][object][local] */ 



EXTERN_C const IID IID_IFractalHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface IFractalHost : public IUnknown
    {
    public:
        virtual HRESULT __stdcall SetGraphSize( 
            unsigned int uWidth,
            unsigned int uHeight) = 0;
        
        virtual HRESULT __stdcall GetGraphSize( 
            unsigned int __RPC_FAR *puWidth,
            unsigned int __RPC_FAR *puHeight) = 0;
        
        virtual HRESULT __stdcall GetPoint( 
            unsigned int __RPC_FAR *puColor,
            int x,
            int y) = 0;
        
        virtual HRESULT __stdcall Rect( 
            int iLeft,
            int iTop,
            int iRight,
            int iBottom,
            unsigned int uColor) = 0;
        
        virtual HRESULT __stdcall Line( 
            int x1,
            int y1,
            int x2,
            int y2,
            unsigned int uColor) = 0;
        
        virtual HRESULT __stdcall Point( 
            int x,
            int y,
            unsigned int uColor) = 0;
        
        virtual HRESULT __stdcall DoneDrawingGraph( void) = 0;
        
    };
    
#else 	/* C style interface */
    
    typedef struct IFractalHostVtbl
    {
        
        HRESULT ( __stdcall __RPC_FAR *QueryInterface )( 
            IFractalHost __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( __stdcall __RPC_FAR *AddRef )( 
            IFractalHost __RPC_FAR * This);
        
        ULONG ( __stdcall __RPC_FAR *Release )( 
            IFractalHost __RPC_FAR * This);
        
        HRESULT ( __stdcall __RPC_FAR *SetGraphSize )( 
            IFractalHost __RPC_FAR * This,
            unsigned int uWidth,
            unsigned int uHeight);
        
        HRESULT ( __stdcall __RPC_FAR *GetGraphSize )( 
            IFractalHost __RPC_FAR * This,
            unsigned int __RPC_FAR *puWidth,
            unsigned int __RPC_FAR *puHeight);
        
        HRESULT ( __stdcall __RPC_FAR *GetPoint )( 
            IFractalHost __RPC_FAR * This,
            unsigned int __RPC_FAR *puColor,
            int x,
            int y);
        
        HRESULT ( __stdcall __RPC_FAR *Rect )( 
            IFractalHost __RPC_FAR * This,
            int iLeft,
            int iTop,
            int iRight,
            int iBottom,
            unsigned int uColor);
        
        HRESULT ( __stdcall __RPC_FAR *Line )( 
            IFractalHost __RPC_FAR * This,
            int x1,
            int y1,
            int x2,
            int y2,
            unsigned int uColor);
        
        HRESULT ( __stdcall __RPC_FAR *Point )( 
            IFractalHost __RPC_FAR * This,
            int x,
            int y,
            unsigned int uColor);
        
        HRESULT ( __stdcall __RPC_FAR *DoneDrawingGraph )( 
            IFractalHost __RPC_FAR * This);
        
    } IFractalHostVtbl;
    
    interface IFractalHost
    {
        CONST_VTBL struct IFractalHostVtbl __RPC_FAR *lpVtbl;
    };
    
    

#ifdef COBJMACROS


#define IFractalHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFractalHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFractalHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFractalHost_SetGraphSize(This,uWidth,uHeight)	\
    (This)->lpVtbl -> SetGraphSize(This,uWidth,uHeight)

#define IFractalHost_GetGraphSize(This,puWidth,puHeight)	\
    (This)->lpVtbl -> GetGraphSize(This,puWidth,puHeight)

#define IFractalHost_GetPoint(This,puColor,x,y)	\
    (This)->lpVtbl -> GetPoint(This,puColor,x,y)

#define IFractalHost_Rect(This,iLeft,iTop,iRight,iBottom,uColor)	\
    (This)->lpVtbl -> Rect(This,iLeft,iTop,iRight,iBottom,uColor)

#define IFractalHost_Line(This,x1,y1,x2,y2,uColor)	\
    (This)->lpVtbl -> Line(This,x1,y1,x2,y2,uColor)

#define IFractalHost_Point(This,x,y,uColor)	\
    (This)->lpVtbl -> Point(This,x,y,uColor)

#define IFractalHost_DoneDrawingGraph(This)	\
    (This)->lpVtbl -> DoneDrawingGraph(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT __stdcall IFractalHost_SetGraphSize_Proxy( 
    IFractalHost __RPC_FAR * This,
    unsigned int uWidth,
    unsigned int uHeight);


void __RPC_STUB IFractalHost_SetGraphSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_GetGraphSize_Proxy( 
    IFractalHost __RPC_FAR * This,
    unsigned int __RPC_FAR *puWidth,
    unsigned int __RPC_FAR *puHeight);


void __RPC_STUB IFractalHost_GetGraphSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_GetPoint_Proxy( 
    IFractalHost __RPC_FAR * This,
    unsigned int __RPC_FAR *puColor,
    int x,
    int y);


void __RPC_STUB IFractalHost_GetPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_Rect_Proxy( 
    IFractalHost __RPC_FAR * This,
    int iLeft,
    int iTop,
    int iRight,
    int iBottom,
    unsigned int uColor);


void __RPC_STUB IFractalHost_Rect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_Line_Proxy( 
    IFractalHost __RPC_FAR * This,
    int x1,
    int y1,
    int x2,
    int y2,
    unsigned int uColor);


void __RPC_STUB IFractalHost_Line_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_Point_Proxy( 
    IFractalHost __RPC_FAR * This,
    int x,
    int y,
    unsigned int uColor);


void __RPC_STUB IFractalHost_Point_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT __stdcall IFractalHost_DoneDrawingGraph_Proxy( 
    IFractalHost __RPC_FAR * This);


void __RPC_STUB IFractalHost_DoneDrawingGraph_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFractalHost_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
