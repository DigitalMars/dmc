
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "replay.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;
handle_t dict_IfHandle;


static const RPC_CLIENT_INTERFACE dict___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x12345678,0x1234,0xABCD,{0xEF,0x00,0x01,0x23,0x45,0x67,0x89,0xAB}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE dict_ClientIfHandle = (RPC_IF_HANDLE)& dict___RpcClientInterface;

extern const MIDL_STUB_DESC dict_StubDesc;

static RPC_BINDING_HANDLE dict__MIDL_AutoBindHandle;


VDict_Status VDict_New( 
    /* [in] */ short shared_dict,
    /* [out] */ VDict __RPC_FAR *v_dict)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          0);
        
        
        _Handle = dict_IfHandle;
        
        
        _StubMsg.BufferLength = 2U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = shared_dict;
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        *v_dict = (void *)0;
        NdrClientContextUnmarshall(
                              ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                              ( NDR_CCONTEXT __RPC_FAR * )v_dict,
                              _Handle);
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Find( 
    /* [in] */ VDict v_dict,
    /* [out][in] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          1);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)item,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Next( 
    /* [in] */ VDict v_dict,
    /* [out][in] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          2);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)item,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Prev( 
    /* [in] */ VDict v_dict,
    /* [out][in] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          3);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)item,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Curr_Next( 
    /* [in] */ VDict v_dict,
    /* [out] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          4);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[18] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[42],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Curr_Prev( 
    /* [in] */ VDict v_dict,
    /* [out] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          5);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[18] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[42],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Insert( 
    /* [in] */ VDict v_dict,
    /* [in] */ Record __RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          6);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 0U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[46] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)item,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[46] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[28] );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Delete( 
    /* [in] */ VDict v_dict,
    /* [out][in] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          7);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 4U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)item,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[12] );
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[8] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[12],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Get_Dict( 
    /* [in] */ VDict v_dict,
    /* [out] */ RDict __RPC_FAR *__RPC_FAR *r_dict)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          8);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[38] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&r_dict,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[50],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Curr_Item( 
    /* [in] */ VDict v_dict,
    /* [out] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          9);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[18] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[42],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_Curr_Delete( 
    /* [in] */ VDict v_dict,
    /* [out] */ Record __RPC_FAR *__RPC_FAR *item)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          10);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[18] );
        
        NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR * __RPC_FAR *)&item,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[42],
                              (unsigned char)0 );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_X_Dict( 
    /* [in] */ VDict v_dict)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          11);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[48] );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


VDict_Status VDict_I_Dict( 
    /* [in] */ VDict v_dict,
    /* [in] */ short size)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    VDict_Status _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&dict_StubDesc,
                          12);
        
        
        _Handle = NDRCContextBinding(( NDR_CCONTEXT  )v_dict);;
        
        
        _StubMsg.BufferLength = 20U + 2U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        NdrClientContextMarshall(
                            ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                            ( NDR_CCONTEXT  )v_dict,
                            1);
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = size;
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[54] );
        
        NdrSimpleTypeUnmarshall(
                           ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                           ( unsigned char __RPC_FAR * )&_RetVal,
                           13);
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


static const MIDL_STUB_DESC dict_StubDesc = 
    {
    (void __RPC_FAR *)& dict___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &dict_IfHandle,
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

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/*  2 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/*  4 */	0x0, 0x0,	/* Type Offset=0 */
/*  6 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/*  8 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 10 */	0x8, 0x0,	/* Type Offset=8 */
/* 12 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/* 14 */	0xc, 0x0,	/* Type Offset=12 */
/* 16 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 18 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 20 */	0x8, 0x0,	/* Type Offset=8 */
/* 22 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/* 24 */	0x2a, 0x0,	/* Type Offset=42 */
/* 26 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 28 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 30 */	0x8, 0x0,	/* Type Offset=8 */
/* 32 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 34 */	0x2e, 0x0,	/* Type Offset=46 */
/* 36 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 38 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 40 */	0x8, 0x0,	/* Type Offset=8 */
/* 42 */	
			0x51,		/* FC_OUT_PARAM */
			0x1,		/* 1 */
/* 44 */	0x32, 0x0,	/* Type Offset=50 */
/* 46 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 48 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 50 */	0x8, 0x0,	/* Type Offset=8 */
/* 52 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */
/* 54 */	
			0x4d,		/* FC_IN_PARAM */
			0x1,		/* 1 */
/* 56 */	0x8, 0x0,	/* Type Offset=8 */
/* 58 */	0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/* 60 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xd,		/* FC_ENUM16 */

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
/*  8 */	0x30,		/* FC_BIND_CONTEXT */
			0x40,		/* 64 */
/* 10 */	0x0,		/* 0 */
			0x5c,		/* FC_PAD */
/* 12 */	
			0x11, 0x10,	/* FC_RP */
/* 14 */	0x2, 0x0,	/* Offset= 2 (16) */
/* 16 */	
			0x12, 0x0,	/* FC_UP */
/* 18 */	0x2, 0x0,	/* Offset= 2 (20) */
/* 20 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 22 */	0x8, 0x0,	/* 8 */
/* 24 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 26 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 28 */	0x4, 0x0,	/* 4 */
/* 30 */	0x4, 0x0,	/* 4 */
/* 32 */	0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 34 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/* 36 */	
			0x5b,		/* FC_END */

			0x6,		/* FC_SHORT */
/* 38 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 40 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 42 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 44 */	0xe4, 0xff,	/* Offset= -28 (16) */
/* 46 */	
			0x11, 0x0,	/* FC_RP */
/* 48 */	0xe4, 0xff,	/* Offset= -28 (20) */
/* 50 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/* 52 */	0x2, 0x0,	/* Offset= 2 (54) */
/* 54 */	
			0x12, 0x0,	/* FC_UP */
/* 56 */	0x42, 0x0,	/* Offset= 66 (122) */
/* 58 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 60 */	0xc, 0x0,	/* 12 */
/* 62 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 64 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 66 */	0x0, 0x0,	/* 0 */
/* 68 */	0x0, 0x0,	/* 0 */
/* 70 */	0x12, 0x0,	/* FC_UP */
/* 72 */	0xf2, 0xff,	/* Offset= -14 (58) */
/* 74 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 76 */	0x4, 0x0,	/* 4 */
/* 78 */	0x4, 0x0,	/* 4 */
/* 80 */	0x12, 0x0,	/* FC_UP */
/* 82 */	0xe8, 0xff,	/* Offset= -24 (58) */
/* 84 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 86 */	0x8, 0x0,	/* 8 */
/* 88 */	0x8, 0x0,	/* 8 */
/* 90 */	0x12, 0x0,	/* FC_UP */
/* 92 */	0xb8, 0xff,	/* Offset= -72 (20) */
/* 94 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 96 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 98 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 100 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 102 */	0x8, 0x0,	/* 8 */
/* 104 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 106 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 108 */	0x4, 0x0,	/* 4 */
/* 110 */	0x4, 0x0,	/* 4 */
/* 112 */	0x12, 0x0,	/* FC_UP */
/* 114 */	0xa2, 0xff,	/* Offset= -94 (20) */
/* 116 */	
			0x5b,		/* FC_END */

			0x6,		/* FC_SHORT */
/* 118 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 120 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 122 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 124 */	0xc, 0x0,	/* 12 */
/* 126 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 128 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 130 */	0x0, 0x0,	/* 0 */
/* 132 */	0x0, 0x0,	/* 0 */
/* 134 */	0x12, 0x0,	/* FC_UP */
/* 136 */	0xb2, 0xff,	/* Offset= -78 (58) */
/* 138 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 140 */	0x8, 0x0,	/* 8 */
/* 142 */	0x8, 0x0,	/* 8 */
/* 144 */	0x12, 0x0,	/* FC_UP */
/* 146 */	0xd2, 0xff,	/* Offset= -46 (100) */
/* 148 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 150 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 152 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };
