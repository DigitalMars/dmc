#include "rpc.h"
#include "rpcndr.h"

#ifndef __nhello_h__
#define __nhello_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __nhello_INTERFACE_DEFINED__
#define __nhello_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: nhello
 * at Fri Jul 21 17:15:44 1995
 * using MIDL 2.00.72
 ****************************************/
/* [auto_handle][unique][version][uuid] */ 


			/* size is 0 */
void HelloProc( 
    handle_t nBinding,
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

			/* size is 0 */
void Shutdown( 
    handle_t nBinding);



extern RPC_IF_HANDLE nhello_v1_0_c_ifspec;
extern RPC_IF_HANDLE nhello_v1_0_s_ifspec;
#endif /* __nhello_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
