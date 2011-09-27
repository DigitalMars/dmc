#include "rpc.h"
#include "rpcndr.h"
#include "midles.h"

#ifndef __picklp_h__
#define __picklp_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __proc_pickle_INTERFACE_DEFINED__
#define __proc_pickle_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: proc_pickle
 * at Mon Jul 17 13:21:09 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 2 */
#define	ARR_SIZE	( 4 )

			/* size is 2 */
#define	BUFSIZE	( 1024 )

			/* size is 18 */
typedef struct  _OBJECT1
    {
    long al[ 4 ];
    short s;
    }	OBJECT1;

			/* size is 2 */
typedef struct  _OBJECT2
    {
    short sSize;
    /* [size_is] */ short as[ 1 ];
    }	OBJECT2;

			/* size is 0 */
/* [decode][encode] */ void ProcPickle( 
    /* [string][out][in] */ unsigned char __RPC_FAR *pTextId,
    /* [out][in] */ OBJECT1 __RPC_FAR *pObject1,
    /* [out][in] */ OBJECT2 __RPC_FAR *pObject2);


extern handle_t ImplicitPicHandle;


extern RPC_IF_HANDLE proc_pickle_ClientIfHandle;
extern RPC_IF_HANDLE proc_pickle_ServerIfHandle;
#endif /* __proc_pickle_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
