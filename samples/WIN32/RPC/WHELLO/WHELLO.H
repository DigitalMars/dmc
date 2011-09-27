/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:22:36 1995
 */
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __whello_h__
#define __whello_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __whello_INTERFACE_DEFINED__
#define __whello_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: whello
 * at Tue Jul 18 14:22:36 1995
 * using MIDL 2.00.0102
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 0 */
void HelloProc( 
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

			/* size is 0 */
void Shutdown( void);


extern handle_t hWHello;


extern RPC_IF_HANDLE whello_ClientIfHandle;
extern RPC_IF_HANDLE whello_ServerIfHandle;
#endif /* __whello_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
