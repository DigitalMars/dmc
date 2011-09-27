
#include <string.h>
#include "rpcssm.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE rpcssm_DispatchTable;

static const RPC_SERVER_INTERFACE rpcssm___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xA4F1DB00,0xCA47,0x1067,{0xB3,0x1F,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &rpcssm_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE rpcssm_ServerIfHandle = (RPC_IF_HANDLE)& rpcssm___RpcServerInterface;

extern const MIDL_STUB_DESC rpcssm_StubDesc;

void __RPC_STUB
rpcssm_InOutList(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    PBOX __RPC_FAR *ppBox;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &rpcssm_StubDesc);
    NdrRpcSsEnableAllocate(&_StubMsg);
    ppBox = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&ppBox,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        
        InOutList(ppBox);
        
        _StubMsg.BufferLength = 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)ppBox,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)ppBox,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        }
    RpcFinally
        {
        NdrRpcSsDisableAllocate(&_StubMsg);
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
rpcssm_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &rpcssm_StubDesc);
    NdrRpcSsEnableAllocate(&_StubMsg);
    RpcTryFinally
        {
        
        Shutdown();
        
        _StubMsg.BufferLength = 0U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        }
    RpcFinally
        {
        NdrRpcSsDisableAllocate(&_StubMsg);
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


static const MIDL_STUB_DESC rpcssm_StubDesc = 
    {
    (void __RPC_FAR *)& rpcssm___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION rpcssm_table[] =
    {
    rpcssm_InOutList,
    rpcssm_Shutdown,
    0
    };
RPC_DISPATCH_TABLE rpcssm_DispatchTable = 
    {
    2,
    rpcssm_table
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
			0x11, 0x10,	/* FC_RP */
/*  2 */	0x2, 0x0,	/* Offset= 2 (4) */
/*  4 */	
			0x12, 0x0,	/* FC_UP */
/*  6 */	0x2, 0x0,	/* Offset= 2 (8) */
/*  8 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 10 */	0x8, 0x0,	/* 8 */
/* 12 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 14 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 16 */	0x4, 0x0,	/* 4 */
/* 18 */	0x4, 0x0,	/* 4 */
/* 20 */	0x12, 0x0,	/* FC_UP */
/* 22 */	0xf2, 0xff,	/* Offset= -14 (8) */
/* 24 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 26 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */

			0x0
        }
    };
