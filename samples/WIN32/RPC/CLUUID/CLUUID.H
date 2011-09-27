#include "rpc.h"
#include "rpcndr.h"

#ifndef __cluuid_h__
#define __cluuid_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __cluuid_INTERFACE_DEFINED__
#define __cluuid_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: cluuid
 * at Mon Jul 17 13:08:31 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 0 */
void HelloProc( 
    /* [string][in] */ unsigned char __RPC_FAR *pszString);

			/* size is 0 */
void Shutdown( void);


extern handle_t ImpHandle;


typedef struct _cluuid_SERVER_EPV
{
    void ( __RPC_FAR *HelloProc )( 
        /* [string][in] */ unsigned char __RPC_FAR *pszString);
    void ( __RPC_FAR *Shutdown )( void);
    
    } cluuid_SERVER_EPV;

extern RPC_IF_HANDLE cluuid_ClientIfHandle;
extern RPC_IF_HANDLE cluuid_ServerIfHandle;
#endif /* __cluuid_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
