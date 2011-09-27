/* this ALWAYS GENERATED file contains the RPC server stubs */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:21:51 1995
 */
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#include "yield.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE yield_DispatchTable;

static const RPC_SERVER_INTERFACE yield___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x87A118A0,0x11DB,0x1068,{0x94,0xB2,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &yield_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE yield_ServerIfHandle = (RPC_IF_HANDLE)& yield___RpcServerInterface;

extern const MIDL_STUB_DESC yield_StubDesc;

void __RPC_STUB
yield_YieldProc(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    short cSeconds;
    RPC_STATUS _Status;
    
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &yield_StubDesc);
    
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        cSeconds = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        YieldProc(cSeconds);
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
yield_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_STATUS _Status;
    
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &yield_StubDesc);
    
    RpcTryFinally
        {
        
        Shutdown();
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


static const MIDL_STUB_DESC yield_StubDesc = 
    {
    (void __RPC_FAR *)& yield___RpcServerInterface,
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
    0,
    0x2000066, /* MIDL Version 2.0.102 */
    0
    };

static RPC_DISPATCH_FUNCTION yield_table[] =
    {
    yield_YieldProc,
    yield_Shutdown,
    0
    };
RPC_DISPATCH_TABLE yield_DispatchTable = 
    {
    2,
    yield_table
    };

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/*  2 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/*  4 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {

			0x0
        }
    };
