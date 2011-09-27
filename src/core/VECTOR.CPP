/*_ vector.cpp   Mon Jan 23 1989   Modified by: Walter Bright */

#include	<stddef.h>

/****************************
 * Call constructors/destructors on an array of objects.
 * Input:
 *	Parray		pointer to array of objects
 *	Nelem		number of those objects
 *	Sizelem		size of each of those objects
 *	Ctor		constructor to call for each of those objects
 *	Free		DTOR bit flags
 */

#define DTORfree	1	/* it is destructor's responsibility to
				   free the pointer
				 */
#define DTORmostderived	2	/* destructor is invoked for most-derived
				   instance, not for a base class
				 */
#define DTORvector	4	/* destructor has been invoked for an
				   array of instances
				 */

typedef void *(*ctor_t)(void *);
typedef void *(*dtor_t)(int,void *);


/* ZTC 3.0 style, left in the library for backwards link compatibility
   with old code
 */

#if __ZTC__ < 0x400

void *_vec_new(void *Parray,unsigned Nelem,unsigned Sizelem,void *(*Ctor)(void))
{   unsigned n;
	
    if (Parray == 0)
    {	Parray = new char[((size_t) Nelem) * Sizelem + sizeof(size_t)];
	if (!Parray)
	    return Parray;
    }
    if (Ctor != 0)
    {	for (n = 0; n < Nelem; n++)
	    (*(ctor_t)Ctor)((char *)Parray + n * Sizelem);
    }
    return Parray;
}

void _vec_delete(void *Parray,unsigned Nelem,unsigned Sizelem,
	int (*Dtor)(void),int Free)
{   unsigned int n;

    if (Parray)
    {
	if (Dtor != 0)
	{   for (n = Nelem; n--;)
		(*(dtor_t)Dtor)(DTORmostderived,(char *)Parray + n * Sizelem);
	}
	if (Free & DTORfree)
	    delete Parray;
    }
}

#endif
