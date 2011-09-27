
#include <string.h>
#include "auto.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE autoh_DispatchTable;

static const RPC_SERVER_INTERFACE autoh___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x6B29FC40,0xCA47,0x1067,{0xB3,0x1D,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &autoh_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE autoh_ServerIfHandle = (RPC_IF_HANDLE)& autoh___RpcServerInterface;

extern const MIDL_STUB_DESC autoh_StubDesc;

void __RPC_STUB
autoh_GetTime(
    PRPC_MESSAGE _pRpcMessage )
{
    long _M0;
    MIDL_STUB_MESSAGE _StubMsg;
    long __RPC_FAR *time;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &autoh_StubDesc);
    time = 0;
    RpcTryFinally
        {
        time = &_M0;
        
        GetTime(time);
        
        _StubMsg.BufferLength = 4U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        *(( long __RPC_FAR * )_StubMsg.Buffer)++ = *time;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
autoh_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &autoh_StubDesc);
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


static const MIDL_STUB_DESC autoh_StubDesc = 
    {
    (void __RPC_FAR *)& autoh___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION autoh_table[] =
    {
    autoh_GetTime,
    autoh_Shutdown,
    0
    };
RPC_DISPATCH_TABLE autoh_DispatchTable = 
    {
    2,
    autoh_table
    };

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x51,		/* FC_OUT_PARAM */
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
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  2 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };
