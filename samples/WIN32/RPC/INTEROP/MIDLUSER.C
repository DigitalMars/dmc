/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**            OSF DCE Interop Sample Application           **/
/**           Copyright(c) Microsoft Corp. 1993-1995        **/
/**                                                         **/
/*************************************************************/

#include <stdlib.h>
#include "msg.h"

/*
 * These are used in Microsoft clients and servers for stub memory allocation
 */

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t size)
{
    return malloc(size);
}

void __RPC_USER MIDL_user_free(void __RPC_FAR *p)
{
    free(p);
}
