//_ rtti.cpp
// RTL support for rtti

#include <string.h>
#include <stdlib.h>
#include <typeinfo.h>

/*********************************
 * Match types.
 * Input:
 *   tthrow      throw expression type
 *   thandler   handler expression type
 *   poffset      offset to add to 'this' pointer
 * Returns:
 *   !=0   is a match
 *   0   is not a match
 */

/* NOTE: do not put __tiX in the code segment for VCM */
#if (__LARGE__ || __COMPACT__)
#define _FAR_TIX __cs
#else
#define _FAR_TIX
#endif

extern "C" {
    extern char __cdecl _FAR_TIX __tiX[];
}
static char *__get_name (const char *);  // Return the name of the class passed in.
static int __match_name(const char *,const char *);  // Return if 2 classes are of the same name

int __rtti_match(const char *tthrow,const char *thandler,unsigned *poffset)
{
    //printf("rtti_match(%s,%s)\n",__get_name(tthrow),__get_name(thandler));
    // Is a match if:
    //   [1]   tthrow and thandler are the same type.
    //   [2]   thandler is an accessible base class of tthrow at
    //      the throw point.
    //   [3]   thandler is a pointer type and tthrow can be
    //      converted to thandler by a standard pointer conversion
    //      at the throw point.
    //   [4]   thandler matches any type

    /*
        if tthrow and thandler belong to the same exe/dll testing the 
        pointers is sufficient.  If not, the contents have to be tested as well
     */
    if (tthrow == thandler)
        goto match;

    #if defined(_WIN32)
    /* Under NT, tthrow could be 0 for non-CPP exceptions */
    if (!tthrow)
        goto nomatch;
    #endif

    if (__match_name(tthrow,thandler))      // case [1]
        goto match;

    /* In DLL version of RTL, client and RTL DLL have there own copies of __tiX,
       so you must also compare names
     */
    if (thandler == __tiX || __match_name(__tiX,thandler))     // case [4]
        goto match;

    if (*tthrow != *thandler)
        goto nomatch;

    switch (*thandler)
    {
        case 0:
            goto nomatch;      // case [1]

        case 1:            // case [3]
            const char CV = 0xC0;   // const | volatile type flags

            if ((tthrow[1] & ~CV) != (thandler[1] & ~CV))
                goto nomatch;      // different pointer types
            if (tthrow[1] & CV & ~thandler[1])
                goto nomatch;      // can't remove const or volatile
            tthrow = *(char **)(tthrow + 2);
            thandler = *(char **)(thandler + 2);
            return __rtti_match(tthrow,thandler,poffset);

        case 2:            // case [2]
            int nbases;

            nbases = *(short *)(tthrow + 1);
            tthrow += 1 + sizeof(short);
            for (int i = 0; i < nbases; i++)   // for each accessible base
            {   
                char *tbase;
                int offset;

                offset = *(int *)tthrow;   // offset of base class
                tthrow += sizeof(int);
                tbase = *(char **)tthrow;   // rtti of base class
                tthrow += sizeof(char *);
                if (tbase == thandler || __match_name(tbase,thandler) || __rtti_match(tbase,thandler,poffset))
                {
                    if (poffset)
                        *poffset += offset;   // offset to 'this' pointer
                    goto match;
                }
            }
            goto nomatch;

        default:
            abort();         // undefined value
    }

nomatch:
    return 0;

match:
    return 1;
}


/*****************************************************************
 * Given to class pointers, return whether they are the same name.
 */
 
static int __match_name(const char *cl1, const char *cl2)
{
    char *name1, *name2;
    
    name1 = __get_name(cl1);
    name2 = __get_name(cl2);

    if (!name1 || !name2 || strcmp(name1,name2))
        return 0;
        
    return 1;
}
 
/********************************
 * Given a pointer to class data, return
 * the name of the class, this code is copied from Typeinfo.cpp
 */
 
static char *__get_name(const char *cl)
{
    char *p;
    int nbases;
    
    p = (char *)cl;
    switch (*p)
    {
        case 0:
            p++;
            break;
        case 1:
            p+= 2 + sizeof(p);
            break;
        case 2:
            p++;
            nbases = *(unsigned short *)p;
            p += 2 + nbases * (sizeof(int) + sizeof(p));
            break;
        default:
            return NULL;
    }
    return p;
}
       
    

/**********************************
 * C++98 5.2.7
 * Perform dynamic_cast.
 * Input:
 *   t   Type_info for complete object pointed to by p
 *   p   pointer to object
 *   v   static type of object
 *   T   type we wish to cast p to
 *   flag    flag bits
 *       1 throw exception if bad cast
 *       2 T is a virtual base of v
 */

void * __cdecl __rtti_cast(void *t,void *p,const char *v,const char *T,int flag)
{   
    const char *d;
    unsigned offset = 0;
    unsigned offset2 = 0;
    
    if (!p)
        goto retnull;

    // Get dynamic type of p
    d = (const char *)((Type_info *)t)->pdata;

    // If T is a void* then convert p to a pointer
    // to the complete object.
    if (T == __tiX)
    {
       if (!__rtti_match(d,v,&offset))
           goto fail;
       goto cast;
    }

    //printf("rtti_cast(d='%s',v='%s',T='%s')\n",__get_name(d),__get_name(v),__get_name(T));

    /*	ARM 5.2.6:
	If, in the complete object pointed to by v, v points
	to a public base class sub-object of a T object, and if only one
	object of type T is derived from the sub-object referred to by v,
	the result is a pointer to that T object.
	Otherwise, if the type of the complete object has an unambiguous
	public base class of type T, the result is a pointer to the
	T sub-object of the complete object.
	Otherwise, the run-time check fails.
     */

    // If the object pointed to by v is an object representing a base
    // class of an object of type T the result is a pointer to that object.
    if (!(flag & 2) &&			// if not virtual and
	__rtti_match(T,v,&offset) &&	// v is a base of T and
	__rtti_match(d,v,NULL) &&	// v is a base of d and
	__rtti_match(d,T,NULL)		// T is a base of d
       )
    {
        goto cast;
    }

    // Find the complete object pointed to by v.
    offset = 0;
    if (!__rtti_match(d,v,&offset))
        goto fail;

    // If that object has a unique public base class of type T, the
    // result is the object representing that base class.
    if (__rtti_match(d,T,&offset2))
    {   
        offset -= offset2;
        goto cast;
    }

#if 0
    offset = 0;
    if (flag & 2 &&			// if virtual and
	__rtti_match(T,v,&offset) &&	// v is a base of T and
	__rtti_match(d,v,NULL) &&	// v is a base of d and
	__rtti_match(d,T,NULL)		// T is a base of d
       )
    {
        goto cast;
    }
#endif

    goto fail;

cast:
    return (char *)p - offset;

fail:
    if (flag & 1)
    {
#if _WIN32
	std::bad_cast bc;
#else
	bad_cast bc;
#endif
        throw bc;
    }
retnull:
    return NULL;
}

