/*_ mbjisjms.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned int __cdecl _mbcjistojms (unsigned int cch) {
 union{
  unsigned short w;
 struct{
  unsigned char  l;
  unsigned char  h;
 };
 } c;
  if (__mbcodepage == _KANJI_CP) {
    c.w = cch;
    if (c.h >= 0x21 && c.h <= 0x7E && c.l >= 0x21 && c.l <= 0x7E) {
# if 0
      c.h--;
      if (c.h & 1) {
        c.l += 0x5E;
      }
      c.h >>= 1;
      c.h += 0x71;
      if (c.h >= 0xA0) {
        c.h += 0x40;
      }
      c.l += 0x1F;
      if (c.l >= 0x7F) {
        c.l++;
      }
# else
      asm {
	mov	ax, c.w
	dec	ah
	shr	ah, 1
	jnb	jtos_1
	add	al, 05Eh
jtos_1:
	add	ah, 071h
	cmp	ah, 0A0h
	jb	jtos_2
	add	ah, 040h
jtos_2:
	add	al, 01Fh
	cmp	al, 07Fh
	cmc
	adc	al, 0
	mov	c.w, ax
      }
# endif
      return c.w;
    }
  }
  return 0;
}

#endif

