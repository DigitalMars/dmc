#include "rpc.h"
#include "rpcndr.h"

#ifndef __auto_h__
#define __auto_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __autoh_INTERFACE_DEFINED__
#define __autoh_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: autoh
 * at Mon Jul 17 13:14:54 1995
 * using MIDL 2.00.72
 ****************************************/
/* [auto_handle][unique][version][uuid] */ 


			/* size is 0 */
void GetTime( 
    /* [out] */ long __RPC_FAR *time);

			/* size is 0 */
void Shutdown( void);



extern RPC_IF_HANDLE autoh_ClientIfHandle;
extern RPC_IF_HANDLE autoh_ServerIfHandle;
#endif /* __autoh_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
