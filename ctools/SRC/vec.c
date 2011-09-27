/*_ vec.c   Mon Oct 31 1994 */
/* $Header$ */
/* Copyright (C) 1986-1994 by Digital Mars		*/
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* www.digitalmars.com					*/
/* Bit vector package					*/

#include	<stdio.h>
#include	<string.h>
#ifndef	assert
#include	<assert.h>
#endif
#include	"vec.h"
#include	"mem.h"

static int vec_count;		/* # of vectors allocated		*/
static int vec_initcount = 0;	/* # of times package is initialized	*/

#if 1
#define MASK(b)		(1 << ((b) & VECMASK))
#else
#define MASK(b)		bmask[(b) & VECMASK]
static unsigned bmask[VECMASK + 1] =
{
	1,2,4,8,0x10,0x20,0x40,0x80,
	0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,
#if __INTSIZE == 4
	0x10000,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,
	0x1000000,0x2000000,0x4000000,0x8000000,
	0x10000000,0x20000000,0x40000000,0x80000000
#endif
};
#endif

/**************************
 * Initialize package.
 */

void vec_init()
{
    assert(sizeof(vec_base_t)==2&&VECSHIFT==4||sizeof(vec_base_t)==4&&VECSHIFT== 5);
    if (vec_initcount++ == 0)
	    vec_count = 0;
}

/**************************
 * Terminate package.
 */

void vec_term()
{
    if (--vec_initcount == 0)
    {
#ifdef DEBUG
	if (vec_count != 0)
	{
		printf("vec_count = %d\n",vec_count);
		assert(0);
	}
#else
	assert(vec_count == 0);
#endif
    }
}

/********************************
 * Allocate a vector given # of bits in it.
 * Clear the vector.
 */

vec_t vec_calloc(numbits)
register unsigned numbits;
{ vec_t v;
  int dim;

  if (numbits == 0)
	return (vec_t) NULL;
  vec_count++;
  dim = (numbits + (VECBITS - 1)) >> VECSHIFT;
  v = (vec_t) mem_calloc((dim + 2) * sizeof(vec_base_t)) + 2;
  if (v)
  {	vec_dim(v) = dim;
	vec_numbits(v) = numbits;
	/*printf("vec_calloc(%d): v = %p vec_numbits = %d vec_dim = %d\n",
	    numbits,v,vec_numbits(v),vec_dim(v));*/
  }
  return v;
}

/**************************
 * Free a vector.
 */

void vec_free(v)
vec_t v;
{
    /*printf("vec_free(%p)\n",v);*/
    if (v)
    {	mem_free(v - 2);
	vec_count--;
    }
}

/**************************
 * Realloc a vector to have numbits bits in it.
 * Extra bits are set to 0.
 */

vec_t vec_realloc(v,numbits)
vec_t v;
unsigned numbits;
{	vec_t newv;

	/*printf("vec_realloc(%p,%d)\n",v,numbits);*/
	newv = vec_calloc(numbits);
	if (v && newv)
	{   unsigned nbytes;

	    nbytes = (vec_dim(v) < vec_dim(newv)) ? vec_dim(v) : vec_dim(newv);
	    memcpy(newv,v,nbytes * sizeof(vec_base_t));
	    vec_clearextrabits(newv);
	}
	vec_free(v);
	return newv;
}

/**************************
 * Set bit b in vector v.
 */

#if _M_I86 && __INTSIZE == 4 && __SC__
__declspec(naked) void __pascal vec_setbit(unsigned b,vec_t v)
{
    _asm
    {
	mov	EAX,b-4[ESP]
	mov	ECX,v-4[ESP]
	bts	[ECX],EAX
	ret	8
    }
}
#else
void vec_setbit(unsigned b,vec_t v)
{
#ifdef DEBUG
  if (!(v && b < vec_numbits(v)))
	printf("vec_setbit(v = x%p,b = %d): numbits = %d dim = %d\n",
	    v,b,vec_numbits(v),vec_dim(v));
#endif
  assert(v && b < vec_numbits(v));
  *(v + (b >> VECSHIFT)) |= MASK(b);
}
#endif

/**************************
 * Clear bit b in vector v.
 */

#if _M_I86 && __INTSIZE == 4 && __SC__
__declspec(naked) void __pascal vec_clearbit(unsigned b,vec_t v)
{
    _asm
    {
	mov	EAX,b-4[ESP]
	mov	ECX,v-4[ESP]
	btr	[ECX],EAX
	ret	8
    }
}
#else
void vec_clearbit(unsigned b,vec_t v)
{
  assert(v && b < vec_numbits(v));
  *(v + (b >> VECSHIFT)) &= ~MASK(b);
}
#endif

/**************************
 * Test bit b in vector v.
 */

#if _M_I86 && __INTSIZE == 4 && __SC__
__declspec(naked) int __pascal vec_testbit(unsigned b,vec_t v)
{
    _asm
    {
	mov	EAX,v-4[ESP]
	test	EAX,EAX
	jz	L1
	mov	ECX,b-4[ESP]
	bt	[EAX],ECX
	sbb	EAX,EAX
    L1:	ret	8
    }
}
#else
int vec_testbit(unsigned b,vec_t v)
{
  if (!v)
	return 0;
#ifdef DEBUG
  if (b >= vec_numbits(v))
  {	printf("vec_testbit(v = %p,b = %d): numbits = %d dim = %d\n",
	    v,b,vec_numbits(v),vec_dim(v));
	b = *(unsigned *)0;		// generate GP fault
	b = -1;
  }
#endif
  assert(b < vec_numbits(v));
#if __I86__ >= 3
  _asm
  {
#if __INTSIZE == 4
	mov	EAX,b
	mov	ECX,v
	bt	[ECX],EAX
	sbb	EAX,EAX
#elif __COMPACT__ || __LARGE__ || __VCM__
	mov	AX,b
	les	BX,v
	bt	ES:[BX],AX
	sbb	AX,AX
#else
	mov	AX,b
	mov	CX,v
	bt	[CX],AX
	sbb	AX,AX
#endif
  }
#ifdef DEBUG
  {	int x = _AX;
	assert((x != 0) == ((*(v + (b >> VECSHIFT)) & MASK(b)) != 0));
  }
#endif
#else
  return *(v + (b >> VECSHIFT)) & MASK(b);
#endif
}
#endif

/********************************
 * Find first set bit starting from b in vector v.
 * If no bit is found, return vec_numbits(v).
 */

unsigned vec_index(b,vec)
register unsigned b;
vec_t vec;
{	register unsigned starv;
	register vec_t v,vtop;
	unsigned bit;

    if (!vec)
	return 0;
#if 1
    v = vec;
    if (b < vec_numbits(v))
    {	vtop = &vec[vec_dim(v)];
	bit = b & VECMASK;
	if (bit != b)			/* if not starting in first word */
		v += b >> VECSHIFT;
	starv = *v >> bit;
	while (1)
	{
		while (starv)
		{	if (starv & 1)
				return b;
			b++;
			starv >>= 1;
		}
		b = (b + VECBITS) & ~VECMASK;	/* round up to next word */
		if (++v >= vtop)
		    break;
		starv = *v;
	}
    }
    return vec_numbits(vec);
#else
#if 0
	numbits = vec_numbits(v);
	assert(b < numbits);
	vtop = &v[vec_dim(v)];
	v += b >> VECSHIFT;
	if (!*v)			/* if remainder of word is 0	*/
	{	b += VECBITS - (b & VECMASK);
		bit = 1;
		v++;			/* move up to next word in vector */
	}
	else
		bit = MASK(b);
	for (; v < vtop; v++)
	{
		if (!*v)
		{	b += VECBITS;
			continue;	/* next word in vector		*/
		}
		while (bit)
		{
			if (b >= numbits)
				return numbits;
			if (*v & bit)
				return b;
			bit <<= 1;
			b++;
		}
		bit = 1;
	}
	return numbits;
#else
	assert(b < vec_numbits(v));
	for (; b < vec_numbits(v); b++)
		if (testbit(b,v))	/* if bit is set		*/
			break;
	return b;
#endif
#endif
}

/********************************
 * Compute v1 = v2 & v3.
 */

void vec_and(v1,v2,v3)
vec_t v1,v2,v3;
{   vec_t vtop;

    if (v1)
    {
	assert(v2 && v3);
	assert(vec_numbits(v1)==vec_numbits(v2) && vec_numbits(v1)==vec_numbits(v3));
	vtop = &v1[vec_dim(v1)];
	if (v1 == v2)				/* the usual case	*/
	      for (; v1 < vtop; v1++,v3++)
		      *v1 &= *v3;
	else
	      for (; v1 < vtop; v1++,v2++,v3++)
		      *v1 = *v2 & *v3;
    }
    else
	assert(!v2 && !v3);
}

/********************************
 * Compute v1 = v2 ^ v3.
 */

void vec_xor(v1,v2,v3)
vec_t v1,v2,v3;
{   vec_t vtop;

    if (v1)
    {
	assert(v2 && v3);
	assert(vec_numbits(v1)==vec_numbits(v2) && vec_numbits(v1)==vec_numbits(v3));
	vtop = &v1[vec_dim(v1)];
	if (v1 == v2)				/* the usual case	*/
	      for (; v1 < vtop; v1++,v3++)
		      *v1 ^= *v3;
	else
	      for (; v1 < vtop; v1++,v2++,v3++)
		      *v1 = *v2 ^ *v3;
    }
    else
	assert(!v2 && !v3);
}

/********************************
 * Compute v1 |= v2.
 */

void vec_orass(v1,v2)
vec_t v1,v2;
{   vec_t vtop;

    if (v1)
    {
#ifdef DEBUG
	assert(v2);
	assert(vec_numbits(v1)==vec_numbits(v2));
#endif
	vtop = &v1[vec_dim(v1)];
#if __INTSIZE == 2 && __I86__ && (__COMPACT__ || __LARGE__ || __VCM__)
	_asm
	{
		push	DS
		lds	SI,v2
		les	DI,v1
		mov	CX,word ptr vtop
		cmp	CX,DI
		jz	L1
	    L2: mov	AX,[SI]
		add	SI,2
		or	ES:[DI],AX
		add	DI,2
		cmp	DI,CX
		jb	L2
	    L1: pop	DS
	#if __SC__ <= 0x610
		jmp	Lret
	#endif
	}
#else	
	for (; v1 < vtop; v1++,v2++)
	    *v1 |= *v2;
#endif
    }
    else
	assert(!v2);
Lret: ;
}

/********************************
 * Compute v1 = v2 | v3.
 */

void vec_or(v1,v2,v3)
vec_t v1,v2,v3;
{   vec_t vtop;

    if (v1)
    {
	assert(v2 && v3);
	assert(vec_numbits(v1)==vec_numbits(v2) && vec_numbits(v1)==vec_numbits(v3));
	vtop = &v1[vec_dim(v1)];
	for (; v1 < vtop; v1++,v2++,v3++)
		*v1 = *v2 | *v3;
    }
    else
	assert(!v2 && !v3);
}

/********************************
 * Compute v1 -= v2.
 */

void vec_subass(v1,v2)
vec_t v1,v2;
{   vec_t vtop;

    if (v1)
    {
	assert(v2);
	assert(vec_numbits(v1)==vec_numbits(v2));
	vtop = &v1[vec_dim(v1)];
	for (; v1 < vtop; v1++,v2++)
	    *v1 &= ~*v2;
    }
    else
	assert(!v2);
}

/********************************
 * Compute v1 = v2 - v3.
 */

void vec_sub(v1,v2,v3)
vec_t v1,v2,v3;
{   vec_t vtop;

    if (v1)
    {
	assert(v2 && v3);
	assert(vec_numbits(v1)==vec_numbits(v2) && vec_numbits(v1)==vec_numbits(v3));
	vtop = &v1[vec_dim(v1)];
	for (; v1 < vtop; v1++,v2++,v3++)
	    *v1 = *v2 & ~*v3;
    }
    else
	assert(!v2 && !v3);
}

/****************
 * Clear vector.
 */

void vec_clear(v)
vec_t v;
{
    if (v)
	memset(v,0,sizeof(v[0]) * vec_dim(v));
}

/****************
 * Set vector.
 */

void vec_set(v)
register vec_t v;
{
    if (v)
    {	memset(v,~0,sizeof(v[0]) * vec_dim(v));
	vec_clearextrabits(v);
    }
}

/***************
 * Copy vector.
 */

void vec_copy(to,from)
vec_t to,from;
{
    if (to != from)
    {
#ifdef DEBUG
	if (!(to && from && vec_numbits(to) == vec_numbits(from)))
	    printf("to = x%lx, from = x%lx, numbits(to) = %d, numbits(from) = %d\n",
		(long)to,(long)from,vec_numbits(to),vec_numbits(from));
#endif
	assert(to && from && vec_numbits(to) == vec_numbits(from));
	memcpy(to,from,sizeof(to[0]) * vec_dim(to));
    }
}

/****************
 * Return 1 if vectors are equal.
 */

int vec_equal(v1,v2)
vec_t v1,v2;
{
    if (v1 == v2)
	return 1;
    assert(v1 && v2 && vec_numbits(v1) == vec_numbits(v2));
    return !memcmp(v1,v2,sizeof(v1[0]) * vec_dim(v1));
}

/*********************
 * Clear any extra bits in vector.
 */

void vec_clearextrabits(v)
vec_t v;
{
	assert(v);
	if ((vec_numbits(v) & VECMASK) != 0)
		v[vec_dim(v) - 1] &= MASK(vec_numbits(v)) - 1;
}

/******************
 * Write out vector.
 */

void vec_print(v)
vec_t v;
{ register unsigned i;

#ifdef DEBUG
  printf(" Vec x%p, numbits %d dim %d",v,vec_numbits(v),vec_dim(v));
  if (v)
  {	fputc('\t',stdout);
	for (i = 0; i < vec_numbits(v); i++)
		fputc((vec_testbit(i,v)) ? '1' : '0',stdout);
  }
  fputc('\n',stdout);
#endif
}
