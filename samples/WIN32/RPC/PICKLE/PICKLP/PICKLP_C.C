
#include <string.h>
#ifdef _ALPHA_
#include <stdarg.h>
#endif

#include "picklp.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;
handle_t ImplicitPicHandle;


static const RPC_CLIENT_INTERFACE proc_pickle___RpcClientInterface =
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
RPC_IF_HANDLE proc_pickle_ClientIfHandle = (RPC_IF_HANDLE)& proc_pickle___RpcClientInterface;

extern const MIDL_STUB_DESC proc_pickle_StubDesc;

static RPC_BINDING_HANDLE proc_pickle__MIDL_AutoBindHandle;


/* [decode][encode] */ void ProcPickle( 
    /* [string][out][in] */ unsigned char __RPC_FAR *pTextId,
    /* [out][in] */ OBJECT1 __RPC_FAR *pObject1,
    /* [out][in] */ OBJECT2 __RPC_FAR *pObject2)
{

    NdrMesProcEncodeDecode(
                          ImplicitPicHandle,
                          ( PMIDL_STUB_DESC  )&proc_pickle_StubDesc,
                          ( PFORMAT_STRING  )&__MIDLProcFormatString.Format[0],
                          ( unsigned char __RPC_FAR * )&pTextId,
                          ( unsigned char __RPC_FAR * )&pObject1,
                          ( unsigned char __RPC_FAR * )&pObject2);
}


static const MIDL_STUB_DESC proc_pickle_StubDesc = 
    {
    (void __RPC_FAR *)& proc_pickle___RpcClientInterface,
    NdrRpcSmClientAllocate,
    NdrRpcSmClientFree,
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
			0x32,		/* FC_BIND_PRIMITIVE */
			0x70,		/* 112 */
/*  2 */	0x0, 0x0,	/* 0 */
#ifndef _ALPHA_
/*  4 */	0xc, 0x0,	/* Stack offset= 12 */
#else
			0x18, 0x0,	/* Stack offset= 24 */
#endif
/*  6 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/*  8 */	0x0, 0x0,	/* Type Offset=0 */
/* 10 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/* 12 */	0x4, 0x0,	/* Type Offset=4 */
/* 14 */	
			0x50,		/* FC_IN_OUT_PARAM */
			0x1,		/* 1 */
/* 16 */	0x1e, 0x0,	/* Type Offset=30 */
/* 18 */	0x5b,		/* FC_END */
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
/*  4 */	
			0x11, 0x0,	/* FC_RP */
/*  6 */	0x8, 0x0,	/* Offset= 8 (14) */
/*  8 */	
			0x1d,		/* FC_SMFARRAY */
			0x3,		/* 3 */
/* 10 */	0x10, 0x0,	/* 16 */
/* 12 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 14 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 16 */	0x14, 0x0,	/* 20 */
/* 18 */	0x0, 0x0,	/* 0 */
/* 20 */	0x0, 0x0,	/* Offset= 0 (20) */
/* 22 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 24 */	0xf0, 0xff,	/* Offset= -16 (8) */
/* 26 */	0x6,		/* FC_SHORT */
			0x3e,		/* FC_STRUCTPAD2 */
/* 28 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 30 */	
			0x11, 0x0,	/* FC_RP */
/* 32 */	0xc, 0x0,	/* Offset= 12 (44) */
/* 34 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 36 */	0x2, 0x0,	/* 2 */
/* 38 */	0x6,		/* 6 */
			0x0,		/*  */
/* 40 */	0xfe, 0xff,	/* -2 */
/* 42 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 44 */	
			0x17,		/* FC_CSTRUCT */
			0x1,		/* 1 */
/* 46 */	0x2, 0x0,	/* 2 */
/* 48 */	0xf2, 0xff,	/* Offset= -14 (34) */
/* 50 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */

			0x0
        }
    };
