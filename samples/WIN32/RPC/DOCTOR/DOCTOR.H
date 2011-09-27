#include "rpc.h"
#include "rpcndr.h"

#ifndef __doctor_h__
#define __doctor_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __doctor_INTERFACE_DEFINED__
#define __doctor_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: doctor
 * at Mon Jul 17 13:12:56 1995
 * using MIDL 2.00.72
 ****************************************/
/* [implicit_handle][unique][version][uuid] */ 


			/* size is 2 */
#define	STRSIZE	( 500 )

			/* size is 0 */
void Analyze( 
    /* [size_is][string][out][in] */ unsigned char __RPC_FAR *pszInOut);

			/* size is 0 */
void Shutdown( void);


extern handle_t doctor_IfHandle;


extern RPC_IF_HANDLE doctor_ClientIfHandle;
extern RPC_IF_HANDLE doctor_ServerIfHandle;
#endif /* __doctor_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
