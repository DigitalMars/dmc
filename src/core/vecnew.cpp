/*_ vecnew.cpp */
/* Written by Walter Bright	*/

#include	<stddef.h>
//#include	<stdio.h>

#define STDCALL	(__INTSIZE == 4)	// if using new stdcall calling sequence

#if __INTSIZE == 4
#define XSP	ESP
#else
#define XSP	SP
#endif

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

#if STDCALL
// 'this' is passed in ECX
typedef void *(__pascal *ctor_t)(void);
typedef void *(__pascal *cpct_t)(void *);
typedef void *(__pascal *dtor_t)(int);
#else
typedef void *(__pascal *ctor_t)(void *);
typedef void *(__pascal *cpct_t)(void *,void *);
typedef void *(__pascal *dtor_t)(int,void *);
#endif

#if !STDCALL	// Keep for linking with old code

void *__vec_new(void *Parray,size_t Sizelem,int Nelem,void *(__pascal *Ctor)(void))
{   unsigned n;

    if (!Parray)
    {	Parray = new char[((size_t) Nelem) * Sizelem + sizeof(size_t)];
	if (!Parray)
	    goto ret;
    }
    *((size_t *)Parray)++ = Nelem;
    if (Ctor != 0)
    {
	for (n = 0; n < Nelem; n++)
	    (*(ctor_t)Ctor)((char *)Parray + n * Sizelem);
    }
ret:
    return Parray;
}

void *__vec_ctor(void *Parray,size_t Sizelem,int Nelem,void *(__pascal *Ctor)(void))
{   unsigned n;
	
    if (Parray && Ctor)
    {	for (n = 0; n < Nelem; n++)
	    (*(ctor_t)Ctor)((char *)Parray + n * Sizelem);
    }
    return Parray;
}

#endif

void __vec_delete(void *Parray,int Free,size_t Sizelem,
	int (__pascal *Dtor)(void))
{   unsigned int Nelem;

    if (Parray)
    {
	if (Dtor != 0)
	{
	    Nelem = ((size_t *)Parray)[-1];	/* retrieve number of objects	*/
	    while (Nelem--)
	    {
		int spsave;
		__asm mov spsave,XSP
#if STDCALL
		_ECX = (long)Parray + Nelem * Sizelem;
		(*(dtor_t)Dtor)(DTORmostderived);
#else
		(*(dtor_t)Dtor)(DTORmostderived,(char *)Parray + Nelem * Sizelem);
#endif
		__asm mov XSP,spsave
	    }
	}
	if (Free & DTORfree)
	{   ((size_t *)Parray)--;
	    delete[] Parray;
	}
    }
}

void __vec_dtor(void *Parray,size_t Sizelem,int Nelem,int (__pascal *Dtor)(void))
{
    if (Parray && Dtor)
    {
	while (Nelem--)
	{
	    int spsave;
	    __asm mov spsave,XSP
#if STDCALL
	    _ECX = (long)Parray + Nelem * Sizelem;
	    (*(dtor_t)Dtor)(DTORmostderived);
#else
	    (*(dtor_t)Dtor)(DTORmostderived,(char *)Parray + Nelem * Sizelem);
#endif
	    __asm mov XSP,spsave
	}
    }
}

#if STDCALL
void __vec_invariant(void *Parray,size_t Sizelem,int Nelem,int (__pascal *Inv)(void))
#else
void __vec_invariant(void *Parray,size_t Sizelem,int Nelem,int (__pascal *Inv)(void *))
#endif
{
    if (Parray && Inv)
    {
	while (Nelem--)
	{
#if STDCALL
	    _ECX = (long)Parray + Nelem * Sizelem;
	    (*Inv)();
#else
	    (*Inv)((char *)Parray + Nelem * Sizelem);
#endif
	}
    }
}

// Exception handling versions

void *__vec_new(void *Parray,size_t Sizelem,int Nelem,void *(__pascal *Ctor)(void),int (__pascal *Dtor)(void))
{   unsigned n;
    int Free = 0;

    if (!Parray)
    {	Parray = new char[((size_t) Nelem) * Sizelem + sizeof(size_t)];
	if (!Parray)
	    goto ret;
	Free = DTORfree;
    }
    *((size_t *)Parray)++ = Nelem;
    if (Ctor != 0)
    {
	try
	{
	    for (n = 0; n < Nelem; n++)
	    {
#if STDCALL
		_ECX = (long)Parray + n * Sizelem;
		(*(ctor_t)Ctor)();
#else
		(*(ctor_t)Ctor)((char *)Parray + n * Sizelem);
#endif
	    }
	}
	catch (...)
	{
	    ((size_t *)Parray)[-1] = n;
	    __vec_delete(Parray,Free,Sizelem,Dtor);
	    throw;		// rethrow current exception
	}
    }
ret:
    return Parray;
}

void *__vec_ctor(void *Parray,size_t Sizelem,int Nelem,
	void *(__pascal *Ctor)(void),int (__pascal *Dtor)(void))
{   unsigned n;
	
    if (Parray && Ctor)
    {
	try
	{
	    for (n = 0; n < Nelem; n++)
	    {
#if STDCALL
		_ECX = (long)Parray + n * Sizelem;
		(*(ctor_t)Ctor)();
#else
		(*(ctor_t)Ctor)((char *)Parray + n * Sizelem);
#endif
	    }
	}
	catch (...)
	{
	    __vec_dtor(Parray,Sizelem,n,Dtor);
	    throw;		// rethrow current exception
	}
    }
    return Parray;
}

void *__vec_cpct(void *Parray,size_t Sizelem,int Nelem,
	void *(__pascal *Ctor)(void),int (__pascal *Dtor)(void),
	void *Pinit)
{   unsigned n;

    //printf("vec_cpct(Parray=%p, Pinit=%p, Nelem=%d)\n",Parray,Pinit,Nelem);
    if (Parray && Ctor)
    {
	try
	{
	    for (n = 0; n < Nelem; n++)
	    {
#if STDCALL
		void * volatile pi;

		pi = (char *)Pinit  + n * Sizelem;
		_ECX = (long)Parray + n * Sizelem;
		(*(cpct_t)Ctor)(pi);
#else
		(*(cpct_t)Ctor)((char *)Pinit  + n * Sizelem,
				(char *)Parray + n * Sizelem);
#endif
	    }
	}
	catch (...)
	{
	    __vec_dtor(Parray,Sizelem,n,Dtor);
	    throw;		// rethrow current exception
	}
    }
    return Parray;
}

