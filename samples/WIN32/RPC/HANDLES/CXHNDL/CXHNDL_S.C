
#include <string.h>
#include "cxhndl.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE cxhndl_DispatchTable;

static const RPC_SERVER_INTERFACE cxhndl___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xA4F1DB00,0xCA47,0x1067,{0xB3,0x1F,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &cxhndl_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE cxhndl_ServerIfHandle = (RPC_IF_HANDLE)& cxhndl___RpcServerInterface;

extern const MIDL_STUB_DESC cxhndl_StubDesc;

void __RPC_STUB
cxhndl_RemoteOpen(
    PRPC_MESSAGE _pRpcMessage )
{
    short _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    NDR_SCONTEXT pphContext;
    unsigned char __RPC_FAR *pszFile;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cxhndl_StubDesc);
    pphContext = 0;
    pszFile = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&pszFile,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[8],
                              (unsigned char)0 );
        
        pphContext = NDRSContextUnmarshall( (char *)0, _pRpcMessage->DataRepresentation ); 
        
        
        _RetVal = RemoteOpen(( PCONTEXT_HANDLE_TYPE __RPC_FAR * )NDRSContextValue(pphContext),pszFile);
        
        _StubMsg.BufferLength = 20U + 2U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        NdrServerContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_SCONTEXT  )pphContext,
                            ( NDR_RUNDOWN  )PCONTEXT_HANDLE_TYPE_rundown);
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
cxhndl_RemoteRead(
    PRPC_MESSAGE _pRpcMessage )
{
    unsigned char _AM2[ 1024 ];
    short _M3;
    short _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    unsigned char ( __RPC_FAR *achBuf )[ 1024 ];
    short __RPC_FAR *pcbBuf;
    NDR_SCONTEXT phContext;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cxhndl_StubDesc);
    achBuf = 0;
    pcbBuf = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[10] );
        
        phContext = NdrServerContextUnmarshall(( PMIDL_STUB_MESSAGE  )&_StubMsg);
        
        achBuf = &_AM2;
        pcbBuf = &_M3;
        
        _RetVal = RemoteRead(
                     ( PCONTEXT_HANDLE_TYPE  )*NDRSContextValue(phContext),
                     *achBuf,
                     pcbBuf);
        
        _StubMsg.BufferLength = 1024U + 5U + 4U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        NdrFixedArrayMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                               (unsigned char __RPC_FAR *)*achBuf,
                               (PFORMAT_STRING) &__MIDLFormatString.Format[16] );
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = *pcbBuf;
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
cxhndl_RemoteClose(
    PRPC_MESSAGE _pRpcMessage )
{
    short _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    NDR_SCONTEXT pphContext;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cxhndl_StubDesc);
    pphContext = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[24] );
        
        pphContext = NdrServerContextUnmarshall(( PMIDL_STUB_MESSAGE  )&_StubMsg);
        
        
        _RetVal = RemoteClose(( PCONTEXT_HANDLE_TYPE __RPC_FAR * )NDRSContextValue(pphContext));
        
        _StubMsg.BufferLength = 20U + 2U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        NdrServerContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_SCONTEXT  )pphContext,
                            ( NDR_RUNDOWN  )PCONTEXT_HANDLE_TYPE_rundown);
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
cxhndl_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cxhndl_StubDesc);
    RpcTryFinally
        {
        
        Shutdown();
        
        _StubMsg.BufferLength = 0U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


static const MIDL_STUB_DESC cxhndl_StubDesc = 
    {
    (void __RPC_FAR *)& cxhndl___RpcServerInterface,
    NdrRpcSsDefaultAllocate,
    NdrRpcSsDefaultFree,
    0,
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

static RPC_DISPATCH_FUNCTION cxhndl_table[] =
    {
    cxhndl_RemoteOpen,
    cxhndl_RemoteRead,
    cxhndl_RemoteClose,
    cxhndl_Shutdown,
    0
    };
RPC_DISPATCH_TABLE cxhndl_DispatchTable = 
    {
    4,
    cxhndl_table
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
