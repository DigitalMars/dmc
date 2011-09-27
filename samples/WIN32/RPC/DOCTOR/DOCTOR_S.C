
#include <string.h>
#include "doctor.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE doctor_DispatchTable;

static const RPC_SERVER_INTERFACE doctor___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0xC2557720,0xCA46,0x1067,{0xB3,0x1C,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &doctor_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE doctor_ServerIfHandle = (RPC_IF_HANDLE)& doctor___RpcServerInterface;

extern const MIDL_STUB_DESC doctor_StubDesc;

void __RPC_STUB
doctor_Analyze(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    unsigned char __RPC_FAR *pszInOut;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &doctor_StubDesc);
    pszInOut = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&pszInOut,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        
        Analyze(pszInOut);
        
        _StubMsg.BufferLength = 12U;
        _StubMsg.MaxCount = STRSIZE;
        
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)pszInOut,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        _StubMsg.MaxCount = STRSIZE;
        
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)pszInOut,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[0] );
        
        }
    RpcFinally
        {
        _StubMsg.MaxCount = STRSIZE;
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)pszInOut,
                        &__MIDLFormatString.Format[0] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
doctor_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &doctor_StubDesc);
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


static const MIDL_STUB_DESC doctor_StubDesc = 
    {
    (void __RPC_FAR *)& doctor___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION doctor_table[] =
    {
    doctor_Analyze,
    doctor_Shutdown,
    0
    };
RPC_DISPATCH_TABLE doctor_DispatchTable = 
    {
    2,
    doctor_table
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
/*  2 */	0x2, 0x0,	/* Offset= 2 (4) */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x44,		/* FC_STRING_SIZED */
/*  6 */	0x40,		/* 64 */
			0x0,		/* 0 */
/*  8 */	0xf4, 0x1,	/* 500 */

			0x0
        }
    };
