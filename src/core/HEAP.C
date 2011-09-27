/*_ heap.c
 * Copyright (C) 1993 by Digital Mars
 * All Rights Reserved
 * Written by Walter Bright
 * $Revision: 1.1.1.1 $ 
 */

#include <string.h>
#include <malloc.h>
#include <dos.h>
#include <io.h>
#include "mt.h"
#include	"semlock.h"

/*
 * FARPTRS	Using seg:offset pointers
 * PROTMODE	Running in protected mode. Treat Windows real mode
 *		as protected mode.
 * ASMIT	Do our allocator, as opposed to Windows' Local???? functions.
 * NORMPTR	Using normalized far pointers, as in DOS large data real mode.
 */

#define FARPTRS  (__INTSIZE == 2 && (__COMPACT__ || __LARGE__ || __VCM__))
#define PROTMODE (DOS16RM || __OS2__ || _WINDOWS || __NT__)
#define ASMIT	 (!(_WINDOWS && __INTSIZE == 2 && (__SMALL__ || __MEDIUM__)))
#define NORMPTR	 (!(PROTMODE || __SMALL__ || __MEDIUM__))

#if !ASMIT
#include <windows.h>
#endif

#pragma pack(1)

struct heapfree
{   struct heapfree *next;	/* next free block in list	*/
    int size;			/* size of this free block	*/
};

#pragma pack()

/* Exported by heap manager	*/
extern struct heapfree _baslnk;
extern struct heapfree *_allocp;

/* Multithreading support	*/
#define talloc_lock()		LockSemaphore(_semAlloc)
#define talloc_unlock()		UnlockSemaphore(_semAlloc)

/*********************
 * Normalize a real mode pointer.
 */

#if NORMPTR

static void *normptr(void *p)
{   unsigned short seg;
    unsigned offset;

    seg = FP_SEG(p);
    offset = FP_OFF(p);
    seg += offset >> 4;
    offset &= 0xF;
    return MK_FP(seg,offset);
}

#else

#define normptr(p)	((void*)(p))

#endif

/********************************
 * Check heap for any errors.
 */

int _heapchk()
{
#if ASMIT
    struct heapfree *p;
    int result;

    talloc_lock();
#if 0 && FARPTRS
    if ((int)_allocp == -1)
    {	result = _HEAPEMPTY;		/* not initialized		*/
        goto Lret;
    }
#endif
    if (_baslnk.size != 0)
    {
        result = _HEAPBADBEGIN;		/* bad header			*/
        goto Lret;
    }
    result = _HEAPOK;
    
    for (p = _baslnk.next; normptr(p) != normptr(&_baslnk); p = p->next)
    {	volatile char lastbyte;
        volatile char *plast;			/* pointer to last byte	*/
        unsigned size;

        /*printf("_heapchk(): p = %p, next = %p, size = x%04x\n",
            p,p->next,p->size);*/
        if (FP_OFF(p) & (sizeof(size_t) - 1))	/* pointer must be aligned */
            goto Lbad;
        size = p->size;
        if (size & (sizeof(size_t) - 1))		/* pointer must be aligned */
            goto Lbad;
        if (size < sizeof(struct heapfree))
            goto Lbad;

        /* Read/write last byte in free block	*/
        plast = (char *)p + size - 1;
        lastbyte = *plast;
        *plast ^= 0x55;
        if (*plast != (lastbyte ^ 0x55))
            goto Lbad;
        *plast = lastbyte;

        /* Do a read/write of the first byte, in order to verify we are not
           pointing into ROM or some read-only area.
         */
        plast = (char *)p;
        lastbyte = *plast;
        *plast ^= 0x55;
        if (*plast != (lastbyte ^ 0x55))
            goto Lbad;
        *plast = lastbyte;

#if FARPTRS
        if (FP_SEG(p) == 0)
            goto Lbad;

        /* Block should not cross segment boundary	*/
        if (FP_OFF(p) + (unsigned long)size > 0x10000)
            goto Lbad;

#if PROTMODE
        /* Block should not overlap succeeding block	*/
        if (FP_SEG(p) == FP_SEG(p->next) &&
            FP_OFF(p) + size > FP_OFF(p->next))
            goto Lbad;
#else
        {
        long lp,lpn,lb;

        lp = (long)normptr(p);
        lpn = (long)normptr(p->next);
        lb = (long)normptr(&_baslnk);

        /* Block should not overlap succeeding block	*/
        if (lpn != lb)
            if ((long)normptr((char *)p + size) > lpn)
                goto Lbad;

        /* Check ordering	*/
        if (lp > lpn)
        {
            if (lpn > lb)
                goto Lbad;
            if (lpn < lb && lp < lb)
                goto Lbad;
        }
        }
#endif
#else
        /* Block should not overlap succeeding block	*/
        if (p->next != &_baslnk)
            if (p < p->next && (char *)p + size > (char *)(p->next))
                goto Lbad;

        /* Check ordering	*/
        if (p > p->next)
        {
            if (p->next > &_baslnk)
                goto Lbad;
            if (p->next < &_baslnk && p < &_baslnk)
                goto Lbad;
        }
#endif
    }
Lret:
    talloc_unlock();
    return result;

Lbad:
    result = _HEAPBADNODE;
    goto Lret;
#else
    return _HEAPOK;
#endif
}

/********************************
 * Set all free blocks to value.
 * We recommend using 0xF4 as the fill value, because this
 * is the 8088 HLT instruction.
 */

int _heapset(unsigned value)
{
    struct heapfree *p;
    int result;

    result = _heapchk();
#if ASMIT
    if (result == _HEAPOK)
    {
        talloc_lock();
        for (p = _baslnk.next; normptr(p) != normptr(&_baslnk); p = p->next)
        {
            memset(p + 1,value,p->size - sizeof(*p));
        }
        talloc_unlock();
    }
#endif
    return result;
}

/***********************************
 * Add block of memory to the heap.
 */

#if __INTSIZE == 2

int _heapadd(void __far *buf,size_t nbytes)
{
#if ASMIT
    nbytes &= ~(sizeof(size_t) - 1);		/* round down		*/
    *((unsigned __far *)buf)++ = nbytes;
    if (FP_SEG(buf) == getDS())		/* if pointer to DGROUP	*/
    {	/* Add to near heap	*/
      #if FARPTRS
        return -1;			/* not implemented	*/
      #else
        free((void *)buf);
        return 0;
      #endif
    }
    else
    {	/* Add to far heap	*/
      #if FARPTRS
        free(buf);
        return 0;
      #else
        return -1;			/* not implemented	*/
      #endif
    }
#else
    return -1;				/* not implemented	*/
#endif
}

#else

int _heapadd(void *buf,size_t nbytes)
{
    nbytes &= ~(sizeof(size_t) - 1);		/* round down		*/
    *((unsigned *)buf)++ = nbytes;
    free(buf);
    return 0;
}

#endif

/************************************
 * Release unused memory back to operating system.
 */

int _heapmin()
{
    return -1;				/* not implemented	*/
}

/************************************
 * Expand the size of an allocated block without moving it.
 * Difference from MSC: you cannot _expand an already free'd block.
 */

void *_expand(void *p,size_t newsize)
{   size_t oldsize;

    if (newsize == 0)
    {	free(p);
        p = NULL;
    }
    else
    {
#if ASMIT
        newsize = (newsize + sizeof(size_t) * 2 - 1) & ~(sizeof(size_t) - 1);
        oldsize = ((size_t *)p)[-1];
        if (newsize <= oldsize)
            /* realloc() contains all the shrink logic	*/
            p = realloc(p,newsize - sizeof(size_t));
        else
        {   struct heapfree *pn;
            struct heapfree **h;

#if __INTSIZE == 2
            /* Fail if segment wrap	*/
            if ((int)p - sizeof(size_t) + (unsigned long)newsize >= 0x10000)
                goto fail;
#endif
#if __INTSIZE == 4 && __OS2__
            /* Check to see if realloc'd object would still
                be within a 64Kb page.
             */
            if (newsize < 0x10000 &&
                ((unsigned)p & 0xFFFF) + newsize > 0xFFFF)
                goto fail;
#endif

            pn = normptr((char *)p + oldsize - sizeof(size_t));
            talloc_lock();
            for (h = &_baslnk.next; 1; h = &(*h)->next)
            {
                if (*h == normptr(&_baslnk))	/* if wrapped around	*/
                {   p = NULL;			/* then we failed	*/
                    break;
                }
                if (*h == pn)
                {
#if NORMPTR
                    if ((unsigned long)oldsize + pn->size >= 0x10000)
                    {	struct heapfree *pnew;
                        size_t nbytes;

                        nbytes = newsize - oldsize;
                        if (pn->size - nbytes < sizeof(struct heapfree))
                            goto fail;		/* left-over not big enough */
                        ((size_t *)p)[-1] += nbytes;
                        pnew = normptr((char *)pn + nbytes);
                        pnew->size = pn->size - nbytes;
                        pnew->next = pn->next;
                        *h = pnew;
                    }
                    else
#endif
                    /* See if it is big enough	*/
                    if (oldsize + pn->size >= newsize)
                    {
                        *h = pn->next;	/* remove pn from free list	*/
                        ((size_t *)p)[-1] += pn->size;
                        talloc_unlock();
                        realloc(p,newsize - sizeof(size_t));
                        goto Lret;
                    }
                    else
                    {
                     fail:
                        p = NULL;	/* not big enough		*/
                    }
                    break;
                }
            }
            talloc_unlock();
        }
#else
        LockSegment(-1);
        p = (void __near *) LocalReAlloc((HLOCAL) p,newsize,LMEM_NODISCARD);
        UnlockSegment(-1);
#endif
    }
Lret:
    return p;
}
