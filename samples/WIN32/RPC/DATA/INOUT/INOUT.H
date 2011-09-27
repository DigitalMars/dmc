#include "rpc.h"
#include "rpcndr.h"

#ifndef __inout_h__
#define __inout_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __inout_INTERFACE_DEFINED__
#define __inout_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: inout
 * at Mon Jul 17 13:09:46 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 0 */
void InOutProc( 
    /* [in] */ short s1,
    /* [out][in] */ short __RPC_FAR *ps2,
    /* [out] */ float __RPC_FAR *pf3);

			/* size is 0 */
void Shutdown( void);


extern handle_t inout_IfHandle;


extern RPC_IF_HANDLE inout_ClientIfHandle;
extern RPC_IF_HANDLE inout_ServerIfHandle;
#endif /* __inout_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
