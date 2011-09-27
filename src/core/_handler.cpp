/*_ _handler.c	 Sat Jan  2 1988   Modified by: Walter Bright */
/* $Revision: 1.1.1.1 $ */

#include <new.h>
#include <stddef.h>
#include <stdlib.h>

_PNH _new_handler = 0;
int __new_handler_type;
#define ANSI_COMPAT 1
#define MS_COMPAT 2

struct handler_type
{
   struct handler_type *next;
   _PNH handler;
   int type;
};

static struct handler_type *first_handler_type;
static _PNH set_nh (_PNH p, int type);

/******************************
 * Called to set a new value of _new_handler.
 * Returns:
 *	previous value of _new_handler.
 */

PFVV set_new_handler(PFVV p)
{
   return (PFVV)set_nh ((_PNH)p, ANSI_COMPAT);
}

_PNH _set_new_handler(_PNH p)
{
   return set_nh (p, MS_COMPAT);
}

static _PNH set_nh (_PNH p, int type)
{
   _PNH old_handler;
   struct handler_type *ht;
   
   old_handler = _new_handler;
   _new_handler = p;
   
   for (ht = first_handler_type;
        ht != NULL;
        ht = ht->next)
   {     
      if (ht->handler == p) break;
   }   
      
   if (ht == NULL)
   {
      ht = (handler_type *)malloc(sizeof(handler_type));
      if (ht != NULL)
      {
         ht->next = first_handler_type;
         ht->handler = p;
         ht->type = type;
         first_handler_type = ht;   
      }
   }
   else
      type = ht->type;
      
   __new_handler_type = type;
   return old_handler;
}

#if _WIN32

namespace std
{
    bad_alloc::~bad_alloc() throw() { }

    const char* bad_alloc::what() const throw() { return "class bad_alloc"; }

    new_handler set_new_handler(new_handler new_p) throw()
    {
	return ::set_new_handler(new_p);
    }
}

#endif
