#include "rpc.h"
#include "rpcndr.h"

#ifndef __dunion_h__
#define __dunion_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __dunion_INTERFACE_DEFINED__
#define __dunion_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: dunion
 * at Mon Jul 17 13:09:12 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 4 */
typedef /* [switch_type] */ union __MIDL_dunion_0001
    {
    /* [case] */ short sVal;
    /* [case] */ float fVal;
    /* [case] */ unsigned char chVal;
    /* [default] */  /* Empty union arm */ 
    }	DISCRIM_UNION_PARAM_TYPE;

			/* size is 4 */
typedef DISCRIM_UNION_PARAM_TYPE DISCRIM_UNION_FIELD_TYPE;

			/* size is 6 */
typedef struct  __MIDL_dunion_0002
    {
    short sDiscrim;
    /* [switch_is] */ DISCRIM_UNION_FIELD_TYPE u;
    }	DISCRIM_UNION_STRUCT_TYPE;

			/* size is 0 */
void UnionParamProc( 
    /* [switch_is][in] */ DISCRIM_UNION_PARAM_TYPE __RPC_FAR *UnionParam,
    /* [in] */ short sDiscrim);

			/* size is 0 */
void UnionStructProc( 
    /* [in] */ DISCRIM_UNION_STRUCT_TYPE __RPC_FAR *UnionStruct);

			/* size is 0 */
void Shutdown( void);


extern handle_t hDiscrim;


extern RPC_IF_HANDLE dunion_ClientIfHandle;
extern RPC_IF_HANDLE dunion_ServerIfHandle;
#endif /* __dunion_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
