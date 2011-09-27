
#include <string.h>
#include "inout.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE inout_DispatchTable;

static const RPC_SERVER_INTERFACE inout___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x6E3B5060,0xCA46,0x1067,{0xB3,0x1A,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &inout_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE inout_ServerIfHandle = (RPC_IF_HANDLE)& inout___RpcServerInterface;

extern const MIDL_STUB_DESC inout_StubDesc;

void __RPC_STUB
inout_InOutProc(
    PRPC_MESSAGE _pRpcMessage )
{
    float _M0;
    MIDL_STUB_MESSAGE _StubMsg;
    float __RPC_FAR *pf3;
    short __RPC_FAR *ps2;
    short s1;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &inout_StubDesc);
    ps2 = 0;
    pf3 = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        s1 = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        ps2 = ( short __RPC_FAR * )_StubMsg.Buffer;
        _StubMsg.Buffer += sizeof( short  );
        
        pf3 = &_M0;
        
        InOutProc(
             s1,
             ps2,
             pf3);
        
        _StubMsg.BufferLength = 2U + 6U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = *ps2;
        
        _StubMsg.Buffer += 2;
        *(( float __RPC_FAR * )_StubMsg.Buffer)++ = *pf3;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
inout_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &inout_StubDesc);
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


static const MIDL_STUB_DESC inout_StubDesc = 
    {
    (void __RPC_FAR *)& inout___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION inout_table[] =
    {
    inout_InOutProc,
    inout_Shutdown,
    0
    };
RPC_DISPATCH_TABLE inout_DispatchTable = 
    {
    2,
    inout_table
    };

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/*  2 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/*  4 */	0x0, 0x0,	/* Type Offset=0 */
/*  6 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/*  8 */	0x4, 0x0,	/* Type Offset=4 */
/* 10 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/* 12 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  2 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/*  4 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  6 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */

			0x0
        }
    };
