
#include <string.h>
#include "nhello.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE nhello_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE nhello___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x5D6E4DA0,0x0D21,0x1068,{0x94,0xB1,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &nhello_v1_0_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE nhello_v1_0_s_ifspec = (RPC_IF_HANDLE)& nhello___RpcServerInterface;

extern const MIDL_STUB_DESC nhello_StubDesc;

void __RPC_STUB
nhello_HelloProc(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    handle_t nBinding;
    unsigned char __RPC_FAR *pszString;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &nhello_StubDesc);
    nBinding = _pRpcMessage->Handle;
    pszString = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&pszString,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        
        HelloProc(nBinding,pszString);
        
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

void __RPC_STUB
nhello_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    handle_t nBinding;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &nhello_StubDesc);
    nBinding = _pRpcMessage->Handle;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        
        Shutdown(nBinding);
        
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


static const MIDL_STUB_DESC nhello_StubDesc = 
    {
    (void __RPC_FAR *)& nhello___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
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

static RPC_DISPATCH_FUNCTION nhello_table[] =
    {
    nhello_HelloProc,
    nhello_Shutdown,
    0
    };
RPC_DISPATCH_TABLE nhello_v1_0_DispatchTable = 
    {
    2,
    nhello_table
    };

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xf,		/* FC_IGNORE */
/*  2 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/*  4 */	0x0, 0x0,	/* Type Offset=0 */
/*  6 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/*  8 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0xf,		/* FC_IGNORE */
/* 10 */	0x5b,		/* FC_END */
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
