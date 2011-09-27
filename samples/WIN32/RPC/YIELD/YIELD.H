/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:21:51 1995
 */
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __yield_h__
#define __yield_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __yield_INTERFACE_DEFINED__
#define __yield_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: yield
 * at Tue Jul 18 14:21:51 1995
 * using MIDL 2.00.0102
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 0 */
void YieldProc( 
    /* [in] */ short cSeconds);

			/* size is 0 */
void Shutdown( void);


extern handle_t hYield;


extern RPC_IF_HANDLE yield_ClientIfHandle;
extern RPC_IF_HANDLE yield_ServerIfHandle;
#endif /* __yield_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
