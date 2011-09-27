
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "xmit.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;
handle_t hXmit;


static const RPC_CLIENT_INTERFACE xmit___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x56BF67C0,0x6503,0x1068,{0xBF,0x67,0x00,0xDD,0x01,0x0F,0xBF,0x25}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE xmit_ClientIfHandle = (RPC_IF_HANDLE)& xmit___RpcClientInterface;

extern const MIDL_STUB_DESC xmit_StubDesc;

static RPC_BINDING_HANDLE xmit__MIDL_AutoBindHandle;


void ModifyListProc( 
    /* [out][in] */ DOUBLE_LINK_TYPE __RPC_FAR *pHead)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&xmit_StubDesc,
                          0);
        
        
        _Handle = hXmit;
        
        
        _StubMsg.BufferLength = 0U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)pHead,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)pHead,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&pHead,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
}


void Shutdown( void)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&xmit_StubDesc,
                          1);
        
        
        _Handle = hXmit;
        
        
        _StubMsg.BufferLength = 0U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
}

extern const XMIT_ROUTINE_QUINTUPLE TransmitAsRoutines[];

static const MIDL_STUB_DESC xmit_StubDesc = 
    {
    (void __RPC_FAR *)& xmit___RpcClientInterface,
    NdrRpcSmClientAllocate,
    NdrRpcSmClientFree,
    &hXmit,
    0,
    0,
    0,
    TransmitAsRoutines,
    __MIDLFormatString.Format,
    0, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0, /* Reserved */
    0, /* Reserved */
    0  /* Reserved */
    };
void __RPC_USER
DOUBLE_LINK_TYPE_XmitTranslateToXmit_0000( PMIDL_STUB_MESSAGE pStubMsg )
{
    DOUBLE_LINK_TYPE_to_xmit( (DOUBLE_LINK_TYPE __RPC_FAR *) pStubMsg->pPresentedType, 
        (DOUBLE_XMIT_TYPE __RPC_FAR * __RPC_FAR *) &pStubMsg->pTransmitType );
    
}
void __RPC_USER
DOUBLE_LINK_TYPE_XmitTranslateFromXmit_0000( PMIDL_STUB_MESSAGE pStubMsg )
{
    DOUBLE_LINK_TYPE_from_xmit( (DOUBLE_XMIT_TYPE __RPC_FAR *) pStubMsg->pTransmitType, 
        (DOUBLE_LINK_TYPE __RPC_FAR *) pStubMsg->pPresentedType ); 
}
void __RPC_USER
DOUBLE_LINK_TYPE_XmitFreeXmit_0000( PMIDL_STUB_MESSAGE pStubMsg )
{
    DOUBLE_LINK_TYPE_free_xmit( (DOUBLE_XMIT_TYPE __RPC_FAR *) pStubMsg->pTransmitType );
}
void __RPC_USER
DOUBLE_LINK_TYPE_XmitFreeInst_0000( PMIDL_STUB_MESSAGE pStubMsg )
{
    DOUBLE_LINK_TYPE_free_inst( (DOUBLE_LINK_TYPE __RPC_FAR *) pStubMsg->pPresentedType ); 
}

static const XMIT_ROUTINE_QUINTUPLE TransmitAsRoutines[] = 
        {
            
            {
            DOUBLE_LINK_TYPE_XmitTranslateToXmit_0000
            ,DOUBLE_LINK_TYPE_XmitTranslateFromXmit_0000
            ,DOUBLE_LINK_TYPE_XmitFreeXmit_0000
            ,DOUBLE_LINK_TYPE_XmitFreeInst_0000
            }
            
        };


static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/*  2 */	0x0, 0x0,	/* Type Offset=0 */
/*  4 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/*  6 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {
			0x11, 0x0,	/* FC_RP */
/*  2 */	0x14, 0x0,	/* Offset= 20 (22) */
/*  4 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  6 */	0x2, 0x0,	/* 2 */
/*  8 */	0x6,		/* 6 */
			0x0,		/*  */
/* 10 */	0xfe, 0xff,	/* -2 */
/* 12 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 14 */	
			0x17,		/* FC_CSTRUCT */
			0x1,		/* 1 */
/* 16 */	0x2, 0x0,	/* 2 */
/* 18 */	0xf2, 0xff,	/* Offset= -14 (4) */
/* 20 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 22 */	0x2d,		/* FC_TRANSMIT_AS */
			0x1,		/* 1 */
/* 24 */	0x0, 0x0,	/* 0 */
/* 26 */	0xc, 0x0,	/* 12 */
/* 28 */	0x0, 0x0,	/* 0 */
/* 30 */	0xf0, 0xff,	/* Offset= -16 (14) */

			0x0
        }
    };
