/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 2.00.0102 */
/* at Tue Jul 18 14:25:42 1995
 */
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __usrdef_h__
#define __usrdef_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __usrdef_INTERFACE_DEFINED__
#define __usrdef_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: usrdef
 * at Tue Jul 18 14:25:42 1995
 * using MIDL 2.00.0102
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 20 */
typedef struct  _DATA_TYPE
    {
    unsigned char __RPC_FAR *pszUuid;
    unsigned char __RPC_FAR *pszProtocolSequence;
    unsigned char __RPC_FAR *pszNetworkAddress;
    unsigned char __RPC_FAR *pszEndpoint;
    unsigned char __RPC_FAR *pszOptions;
    }	DATA_TYPE;

			/* size is 4 */
typedef /* [handle] */ DATA_TYPE __RPC_FAR *DATA_HANDLE_TYPE;

			/* size is 0 */
void UsrdefProc( 
    /* [in] */ DATA_HANDLE_TYPE hBinding,
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

			/* size is 0 */
void Shutdown( 
    /* [in] */ DATA_HANDLE_TYPE hBinding);


extern handle_t hUnused;


extern RPC_IF_HANDLE usrdef_ClientIfHandle;
extern RPC_IF_HANDLE usrdef_ServerIfHandle;
#endif /* __usrdef_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

handle_t __RPC_USER DATA_HANDLE_TYPE_bind( DATA_HANDLE_TYPE );
void __RPC_USER DATA_HANDLE_TYPE_unbind( DATA_HANDLE_TYPE, handle_t );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
