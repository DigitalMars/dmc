/* this ALWAYS GENERATED file contains the RPC server stubs */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:56:27 1995
 */
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#include "dynept.h"

extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;

extern RPC_DISPATCH_TABLE dynept_DispatchTable;

static const RPC_SERVER_INTERFACE dynept___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x6238dc00,0xaf17,0x11cd,{0xb7,0x99,0x00,0xaa,0x00,0x3c,0xd5,0x75}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &dynept_DispatchTable,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE dynept_ServerIfHandle = (RPC_IF_HANDLE)& dynept___RpcServerInterface;

extern const MIDL_STUB_DESC dynept_StubDesc;

void __RPC_STUB
dynept_HelloProc(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    unsigned char __RPC_FAR *pszString;
    RPC_STATUS _Status;
    
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &dynept_StubDesc);
    
    NdrRpcSsEnableAllocate(&_StubMsg);
    pszString = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char __RPC_FAR * __RPC_FAR *)&pszString,
                                       (PFORMAT_STRING) &__MIDLFormatString.Format[2],
                                       (unsigned char)0 );
        
        
        HelloProc(pszString);
        
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
dynept_Shutdown(
    PRPC_MESSAGE _pRpcMessage )
{
    MIDL_STUB_MESSAGE _StubMsg;
    RPC_STATUS _Status;
    
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &dynept_StubDesc);
    
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


static const MIDL_STUB_DESC dynept_StubDesc = 
    {
    (void __RPC_FAR *)& dynept___RpcServerInterface,
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

static RPC_DISPATCH_FUNCTION dynept_table[] =
    {
    dynept_HelloProc,
    dynept_Shutdown,
    0
    };
RPC_DISPATCH_TABLE dynept_DispatchTable = 
    {
    2,
    dynept_table
    };

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  2 */	NdrFcShort( 0x0 ),	/* Type Offset=0 */
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
