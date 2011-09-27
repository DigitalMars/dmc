#include "rpc.h"
#include "rpcndr.h"

#ifndef __xmit_h__
#define __xmit_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __xmit_INTERFACE_DEFINED__
#define __xmit_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: xmit
 * at Mon Jul 17 13:11:30 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 12 */
typedef struct  _DOUBLE_LINK_LIST
    {
    short sNumber;
    struct _DOUBLE_LINK_LIST __RPC_FAR *pNext;
    struct _DOUBLE_LINK_LIST __RPC_FAR *pPrevious;
    }	DOUBLE_LINK_LIST;

			/* size is 2 */
typedef struct  _DOUBLE_XMIT_TYPE
    {
    short sSize;
    /* [size_is] */ short asNumber[ 1 ];
    }	DOUBLE_XMIT_TYPE;

			/* size is 12 */
typedef /* [transmit] */ DOUBLE_LINK_LIST DOUBLE_LINK_TYPE;

			/* size is 0 */
void ModifyListProc( 
    /* [out][in] */ DOUBLE_LINK_TYPE __RPC_FAR *pHead);

			/* size is 0 */
void Shutdown( void);


extern handle_t hXmit;


extern RPC_IF_HANDLE xmit_ClientIfHandle;
extern RPC_IF_HANDLE xmit_ServerIfHandle;
#endif /* __xmit_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */


void __RPC_USER DOUBLE_LINK_TYPE_to_xmit( DOUBLE_LINK_TYPE __RPC_FAR *, DOUBLE_XMIT_TYPE __RPC_FAR * __RPC_FAR * );
void __RPC_USER DOUBLE_LINK_TYPE_from_xmit( DOUBLE_XMIT_TYPE __RPC_FAR *, DOUBLE_LINK_TYPE __RPC_FAR * );
void __RPC_USER DOUBLE_LINK_TYPE_free_inst( DOUBLE_LINK_TYPE __RPC_FAR * );
void __RPC_USER DOUBLE_LINK_TYPE_free_xmit( DOUBLE_XMIT_TYPE __RPC_FAR * );
/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
