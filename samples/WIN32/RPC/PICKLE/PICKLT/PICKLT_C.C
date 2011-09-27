
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "picklt.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;
handle_t ImplicitPicHandle;


static const RPC_CLIENT_INTERFACE type_pickle___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xB75AF600,0x9CF4,0x11CD,{0xA0,0x76,0x08,0x00,0x2B,0x2B,0xD7,0x11}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE type_pickle_ClientIfHandle = (RPC_IF_HANDLE)& type_pickle___RpcClientInterface;

extern const MIDL_STUB_DESC type_pickle_StubDesc;

static RPC_BINDING_HANDLE type_pickle__MIDL_AutoBindHandle;

size_t
OBJECT2_AlignSize(
    OBJECT2 __RPC_FAR * _pType)
{
    return NdrMesTypeAlignSize(
                       ImplicitPicHandle,
                       ( PMIDL_STUB_DESC  )&type_pickle_StubDesc,
                       ( PFORMAT_STRING  )&__MIDLFormatString.Format[22],
                       _pType);
}

void
OBJECT2_Encode(
    OBJECT2 __RPC_FAR * _pType)
{
    NdrMesTypeEncode(
                    ImplicitPicHandle,
                    ( PMIDL_STUB_DESC  )&type_pickle_StubDesc,
                    ( PFORMAT_STRING  )&__MIDLFormatString.Format[22],
                    _pType);
}

void
OBJECT2_Decode(
    OBJECT2 __RPC_FAR * _pType)
{
    NdrMesTypeDecode(
                    ImplicitPicHandle,
                    ( PMIDL_STUB_DESC  )&type_pickle_StubDesc,
                    ( PFORMAT_STRING  )&__MIDLFormatString.Format[22],
                    _pType);
}


static const MIDL_STUB_DESC type_pickle_StubDesc = 
    {
    (void __RPC_FAR *)& type_pickle___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &ImplicitPicHandle,
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

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {
			
			0x1d,		/* FC_SMFARRAY */
			0x3,		/* 3 */
/*  2 */	0x10, 0x0,	/* 16 */
/*  4 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/*  6 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/*  8 */	0x14, 0x0,	/* 20 */
/* 10 */	0x0, 0x0,	/* 0 */
/* 12 */	0x0, 0x0,	/* Offset= 0 (12) */
/* 14 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 16 */	0xf0, 0xff,	/* Offset= -16 (0) */
/* 18 */	0x6,		/* FC_SHORT */
			0x3e,		/* FC_STRUCTPAD2 */
/* 20 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 22 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 24 */	0x8, 0x0,	/* 8 */
/* 26 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 28 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 30 */	0x4, 0x0,	/* 4 */
/* 32 */	0x4, 0x0,	/* 4 */
/* 34 */	0x12, 0x0,	/* FC_UP */
/* 36 */	0xe2, 0xff,	/* Offset= -30 (6) */
/* 38 */	
			0x5b,		/* FC_END */

			0x6,		/* FC_SHORT */
/* 40 */	0x38,		/* FC_ALIGNM4 */
			0x8,		/* FC_LONG */
/* 42 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */

			0x0
        }
    };
