/*_ vec.h   Mon Oct 31 1994 */

#ifndef VEC_H
#define VEC_H

#ifndef P
#if 1
#define P(s)	s
#else
#define P(s)	()
#endif
/*#include	"toolkit.h"*/
#endif

typedef unsigned vec_base_t;			/* base type of vector	*/
typedef vec_base_t *vec_t;

#define vec_numbits(v)	((v)[-1])
#define vec_dim(v)	((v)[-2])

#define VECBITS (sizeof(vec_base_t)*8)		/* # of bits per entry	*/
#define VECMASK	(VECBITS - 1)			/* mask for bit position */
#define VECSHIFT ((VECBITS == 16) ? 4 : 5)	/* # of bits in VECMASK	*/

void vec_init P((void ));
void vec_term P((void ));
vec_t vec_calloc P((unsigned numbits ));
void vec_free P((vec_t v ));
vec_t vec_realloc P((vec_t v , unsigned numbits ));
#if _M_I86 && __INTSIZE == 4 && __SC__
void __pascal vec_setbit P((unsigned b , vec_t v ));
void __pascal vec_clearbit P((unsigned b , vec_t v ));
int  __pascal vec_testbit P((unsigned b , vec_t v ));
#else
void vec_setbit P((unsigned b , vec_t v ));
void vec_clearbit P((unsigned b , vec_t v ));
int  vec_testbit P((unsigned b , vec_t v ));
#endif
unsigned vec_index P((unsigned b , vec_t vec ));
void vec_and P((vec_t v1 , vec_t v2 , vec_t v3 ));
void vec_xor P((vec_t v1 , vec_t v2 , vec_t v3 ));
void vec_orass P((vec_t v1 , vec_t v2 ));
void vec_or P((vec_t v1 , vec_t v2 , vec_t v3 ));
void vec_subass P((vec_t v1 , vec_t v2 ));
void vec_sub P((vec_t v1 , vec_t v2 , vec_t v3 ));
void vec_clear P((vec_t v ));
void vec_set P((vec_t v ));
void vec_copy P((vec_t to , vec_t from ));
int vec_equal P((vec_t v1 , vec_t v2 ));
void vec_clearextrabits P((vec_t v ));
void vec_print P((vec_t v ));

#if _M_I86 && __INTSIZE == 4
#define vec_setclear(b,vs,vc)	{	\
	__asm	mov	EAX,b		\
	__asm	mov	ECX,vs		\
	__asm	bts	[ECX],EAX	\
	__asm	mov	ECX,vc		\
	__asm	btr	[ECX],EAX	\
	}
#else
#define vec_setclear(b,vs,vc)	(vec_setbit((b),(vs)),vec_clearbit((b),(vc)))
#endif

#endif /* VEC_H */

