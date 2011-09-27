#include "rpc.h"
#include "rpcndr.h"

#ifndef __cxhndl_h__
#define __cxhndl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __cxhndl_INTERFACE_DEFINED__
#define __cxhndl_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: cxhndl
 * at Mon Jul 17 13:15:31 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 2 */
#define	BUFSIZE	( 1024 )

			/* size is 4 */
typedef /* [context_handle] */ void __RPC_FAR *PCONTEXT_HANDLE_TYPE;

			/* size is 2 */
short RemoteOpen( 
    /* [out] */ PCONTEXT_HANDLE_TYPE __RPC_FAR *pphContext,
    /* [string][in] */ unsigned char __RPC_FAR *pszFile);

			/* size is 2 */
short RemoteRead( 
    /* [in] */ PCONTEXT_HANDLE_TYPE phContext,
    /* [out] */ unsigned char __RPC_FAR achBuf[ 1024 ],
    /* [out] */ short __RPC_FAR *pcbBuf);

			/* size is 2 */
short RemoteClose( 
    /* [out][in] */ PCONTEXT_HANDLE_TYPE __RPC_FAR *pphContext);

			/* size is 0 */
void Shutdown( void);


extern handle_t hStarter;


extern RPC_IF_HANDLE cxhndl_ClientIfHandle;
extern RPC_IF_HANDLE cxhndl_ServerIfHandle;
#endif /* __cxhndl_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

void __RPC_USER PCONTEXT_HANDLE_TYPE_rundown( PCONTEXT_HANDLE_TYPE );

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
