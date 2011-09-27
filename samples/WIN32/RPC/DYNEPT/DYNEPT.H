/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:56:27 1995
 */
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __dynept_h__
#define __dynept_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __dynept_INTERFACE_DEFINED__
#define __dynept_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: dynept
 * at Tue Jul 18 14:56:27 1995
 * using MIDL 2.00.0102
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 0 */
void HelloProc( 
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

			/* size is 0 */
void Shutdown( void);


extern handle_t dynept_IfHandle;


extern RPC_IF_HANDLE dynept_ClientIfHandle;
extern RPC_IF_HANDLE dynept_ServerIfHandle;
#endif /* __dynept_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
