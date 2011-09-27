#include "rpc.h"
#include "rpcndr.h"

#ifndef __rpcssm_h__
#define __rpcssm_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __rpcssm_INTERFACE_DEFINED__
#define __rpcssm_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: rpcssm
 * at Mon Jul 17 13:22:41 1995
 * using MIDL 2.00.72
 ****************************************/
/* [enable_allocate][implicit_handle][unique][version][uuid] */ 


			/* size is 2 */
#define	LIST_SIZE	( 4 )

			/* size is 8 */
typedef struct  _lbox
    {
    long data;
    struct _lbox __RPC_FAR *next;
    }	LBOX;

			/* size is 4 */
typedef struct _lbox __RPC_FAR *PBOX;

			/* size is 0 */
void InOutList( 
    /* [out][in] */ PBOX __RPC_FAR *ppBox);

			/* size is 0 */
void Shutdown( void);


extern handle_t hStarter;


extern RPC_IF_HANDLE rpcssm_ClientIfHandle;
extern RPC_IF_HANDLE rpcssm_ServerIfHandle;
#endif /* __rpcssm_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
