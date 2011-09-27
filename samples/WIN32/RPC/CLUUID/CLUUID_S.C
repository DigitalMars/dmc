
#include <string.h>
#include "cluuid.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE cluuid_DispatchTable;

extern cluuid_SERVER_EPV DEFAULT_EPV;

static const RPC_SERVER_INTERFACE cluuid___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x4AF72C40,0x4DAA,0x1068,{0xA9,0xAD,0x08,0x00,0x2B,0x2E,0xB2,0x3C}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &cluuid_DispatchTable,
    0,
    0,
    &DEFAULT_EPV,
    0
    };
RPC_IF_HANDLE cluuid_ServerIfHandle = (RPC_IF_HANDLE)& cluuid___RpcServerInterface;

extern const MIDL_STUB_DESC cluuid_StubDesc;

void __RPC_STUB
cluuid_HelloProc(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    unsigned char __RPC_FAR *pszString;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cluuid_StubDesc);
    NdrRpcSsEnableAllocate(&_StubMsg);
    pszString = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&pszString,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        
        ((cluuid_SERVER_EPV *)(_pRpcMessage->ManagerEpv)) -> HelloProc(pszString);
        
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

void __RPC_STUB
cluuid_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &cluuid_StubDesc);
    RpcTryFinally
        {
        
        ((cluuid_SERVER_EPV *)(_pRpcMessage->ManagerEpv)) -> Shutdown();
        
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


static const MIDL_STUB_DESC cluuid_StubDesc = 
    {
    (void __RPC_FAR *)& cluuid___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION cluuid_table[] =
    {
    cluuid_HelloProc,
    cluuid_Shutdown,
    0
    };
RPC_DISPATCH_TABLE cluuid_DispatchTable = 
    {
    2,
    cluuid_table
    };

static cluuid_SERVER_EPV DEFAULT_EPV = 
    {
    HelloProc,Shutdown
};

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x4d,		/* FC_IN_PARAM */
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
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  2 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };
