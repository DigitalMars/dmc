
#include <string.h>
#include "dunion.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE dunion_DispatchTable;

static const RPC_SERVER_INTERFACE dunion___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x87E5BFA0,0xCA46,0x1067,{0xB3,0x1B,0x00,0xDD,0x01,0x06,0x62,0xDA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &dunion_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE dunion_ServerIfHandle = (RPC_IF_HANDLE)& dunion___RpcServerInterface;

extern const MIDL_STUB_DESC dunion_StubDesc;

void __RPC_STUB
dunion_UnionParamProc(
    PRPC_MESSAGE _pRpcMessage )
{
    DISCRIM_UNION_PARAM_TYPE __RPC_FAR *UnionParam;
    MIDL_STUB_MESSAGE _StubMsg;
    short sDiscrim;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &dunion_StubDesc);
    UnionParam = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&UnionParam,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[0],
                              (unsigned char)0 );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 1) & ~ 0x1);
        sDiscrim = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        UnionParamProc(UnionParam,sDiscrim);
        
        _StubMsg.BufferLength = 0U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        }
    RpcFinally
        {
        _StubMsg.MaxCount = sDiscrim;
        
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)UnionParam,
                        &__MIDLFormatString.Format[0] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
dunion_UnionStructProc(
    PRPC_MESSAGE _pRpcMessage )
{
    DISCRIM_UNION_STRUCT_TYPE __RPC_FAR *UnionStruct;
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &dunion_StubDesc);
    UnionStruct = 0;
    RpcTryFinally
        {
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&UnionStruct,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[36],
                              (unsigned char)0 );
        
        
        UnionStructProc(UnionStruct);
        
        _StubMsg.BufferLength = 0U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, 0 );
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)UnionStruct,
                        &__MIDLFormatString.Format[36] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}

void __RPC_STUB
dunion_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    
NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &dunion_StubDesc);
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


static const MIDL_STUB_DESC dunion_StubDesc = 
    {
    (void __RPC_FAR *)& dunion___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION dunion_table[] =
    {
    dunion_UnionParamProc,
    dunion_UnionStructProc,
    dunion_Shutdown,
    0
    };
RPC_DISPATCH_TABLE dunion_DispatchTable = 
    {
    3,
    dunion_table
    };

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/*  2 */	0x0, 0x0,	/* Type Offset=0 */
/*  4 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/*  6 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/*  8 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 10 */	0x24, 0x0,	/* Type Offset=36 */
/* 12 */	0x5b,		/* FC_END */
			0x5c,		/* FC_PAD */
/* 14 */	0x5b,		/* FC_END */
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
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x6,		/* FC_SHORT */
/*  6 */	0x26,		/* 38 */
			0x0,		/*  */
#ifndef _ALPHA_
/*  8 */	0x4, 0x0,	/* Stack offset= 4 */
#else
			0x8, 0x0,	/* Stack offset= 8 */
#endif
/* 10 */	0x2, 0x0,	/* Offset= 2 (12) */
/* 12 */	0x4, 0x0,	/* 4 */
/* 14 */	0x3, 0x0,	/* 3 */
/* 16 */	0x0, 0x0, 0x0, 0x0,	/* 0 */
/* 20 */	0x6, 0x80,	/* Offset= -32762 (-32742) */
/* 22 */	0x1, 0x0, 0x0, 0x0,	/* 1 */
/* 26 */	0xa, 0x80,	/* Offset= -32758 (-32732) */
/* 28 */	0x2, 0x0, 0x0, 0x0,	/* 2 */
/* 32 */	0x2, 0x80,	/* Offset= -32766 (-32734) */
/* 34 */	0x0, 0x0,	/* Offset= 0 (34) */
/* 36 */	
			0x11, 0x0,	/* FC_RP */
/* 38 */	0xa, 0x0,	/* Offset= 10 (48) */
/* 40 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x6,		/* FC_SHORT */
/* 42 */	0x6,		/* 6 */
			0x0,		/*  */
/* 44 */	0xfc, 0xff,	/* -4 */
/* 46 */	0xde, 0xff,	/* Offset= -34 (12) */
/* 48 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x1,		/* 1 */
/* 50 */	0x8, 0x0,	/* 8 */
/* 52 */	0x0, 0x0,	/* 0 */
/* 54 */	0x0, 0x0,	/* Offset= 0 (54) */
/* 56 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 58 */	0x2,		/* 2 */
			0xed, 0xff,	/* Offset= -19 (40) */
			0x5b,		/* FC_END */

			0x0
        }
    };
