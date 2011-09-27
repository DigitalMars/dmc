#include "rpc.h"
#include "rpcndr.h"

#ifndef __msg_h__
#define __msg_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __interop_INTERFACE_DEFINED__
#define __interop_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: interop
 * at Fri Jul 21 17:33:11 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][version][uuid] */ 


			/* size is 0 */
void ClientMessage( 
    /* [ref][string][in] */ unsigned char __RPC_FAR *pszMessage);

			/* size is 0 */
void ShutdownServer( void);


extern handle_t interop_binding_handle;


extern RPC_IF_HANDLE interop_v1_0_c_ifspec;
extern RPC_IF_HANDLE interop_v1_0_s_ifspec;
#endif /* __interop_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
