
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "cxhndl.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;
handle_t hStarter;


static const RPC_CLIENT_INTERFACE cxhndl___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xA4F1DB00,0xCA47,0x1067,{0xB3,0x1F,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE cxhndl_ClientIfHandle = (RPC_IF_HANDLE)& cxhndl___RpcClientInterface;

extern const MIDL_STUB_DESC cxhndl_StubDesc;

static RPC_BINDING_HANDLE cxhndl__MIDL_AutoBindHandle;


short RemoteOpen( 
    /* [out] */ PCONTEXT_HANDLE_TYPE __RPC_FAR *pphContext,
    /* [string][in] */ unsigned char __RPC_FAR *pszFile)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    short _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&cxhndl_StubDesc,
                          0);
        
        
        _Handle = hStarter;
        
        
        _StubMsg.BufferLength = 12U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)pszFile,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[8] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)pszFile,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[8] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        *pphContext = (void *)0;
        NdrClientContextUnmarshall(
                              ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                              ( NDR_CCONTEXT __RPC_FAR * )pphContext,
                              _Handle);
        
        _RetVal = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


short RemoteRead( 
    /* [in] */ PCONTEXT_HANDLE_TYPE phContext,
    /* [out] */ unsigned char __RPC_FAR achBuf[ 1024 ],
    /* [out] */ short __RPC_FAR *pcbBuf)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    short _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&cxhndl_StubDesc,
                          1);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )phContext);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )phContext,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[10] );
        
        NdrFixedArrayUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                 (unsigned char __RPC_FAR * __RPC_FAR *)&achBuf,
                                 (PFORMAT_STRING) &__MIDLFormatString.Format[16],
                                 (unsigned char)0 );
        
        *pcbBuf = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        _RetVal = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


short RemoteClose( 
    /* [out][in] */ PCONTEXT_HANDLE_TYPE __RPC_FAR *pphContext)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    short _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&cxhndl_StubDesc,
                          2);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )*pphContext);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )*pphContext,
                            0);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[24] );
        
        NdrClientContextUnmarshall(
                              ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                              ( NDR_CCONTEXT __RPC_FAR * )pphContext,
                              _Handle);
        
        _RetVal = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
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
                          ( PMIDL_STUB_DESC  )&cxhndl_StubDesc,
                          3);
        
        
        _Handle = hStarter;
        
        
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


static const MIDL_STUB_DESC cxhndl_StubDesc = 
    {
    (void __RPC_FAR *)& cxhndl___RpcClientInterface,
    NdrRpcSmClientAllocate,
    NdrRpcSmClientFree,
    &hStarter,
    0,
    0,
    0,
    0,
    __MIDLFormatString.Format,
    0, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0, /* Reserved */
    0, /* Reserved */
    0  /* Reserved */
    };

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/*  2 */	0x0, 0x0,	/* Type Offset=0 */
/*  4 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/*  6 */	0x8, 0x0,	/* Type Offset=8 */
/*  8 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/* 10 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 12 */	0xc, 0x0,	/* Type Offset=12 */
/* 14 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/* 16 */	0x10, 0x0,	/* Type Offset=16 */
/* 18 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/* 20 */	0x16, 0x0,	/* Type Offset=22 */
/* 22 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/* 24 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/* 26 */	0x1a, 0x0,	/* Type Offset=26 */
/* 28 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/* 30 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {
			0x11, 0x0,	/* FC_RP */
/*  2 */	0x2, 0x0,	/* Offset= 2 (4) */
/*  4 */	0x30,		/* FC_BIND_CONTEXT */
			0xa0,		/* -96 */
/*  6 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/*  8 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 10 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/* 12 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* 64 */
/* 14 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 16 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 18 */	0x0, 0x4,	/* 1024 */
/* 20 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 22 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 24 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 26 */	
			0x11, 0x0,	/* FC_RP */
/* 28 */	0x2, 0x0,	/* Offset= 2 (30) */
/* 30 */	0x30,		/* FC_BIND_CONTEXT */
			0xe0,		/* -32 */
/* 32 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */

			0x0
        }
    };
