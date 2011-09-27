/*_ mbjmsjis.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef _MBCS

#include <mbctype.h>
#include <mbstring.h>
#include <setmbcp.h>

unsigned int __cdecl _mbcjmstojis (unsigned int cch) {
 union{
  unsigned short w;
 struct{
  unsigned char  l;
  unsigned char  h;
 };
 } c;
  if (__mbcodepage == _KANJI_CP) {
    c.w = cch;
    if (_ismbblead (c.h) && _ismbbtrail (c.l)) {
# if 0
      if (c.h >= 0xE0) {
        c.h -= 0x40;
      }
      c.h -= 0x70;
      c.h <<= 1;
      c.h--;
      if (c.l >= 0x80) {
        c.l--;
      }
      c.l -= 0x1F;
      if (c.l >= 0x7F) {
        c.h++;
        c.l -= 0x5E;
      }
# else
      asm {
	mov	ax, c.w
	cmp	ah, 0E0h
	jb	stoj_1
	sub	ah, 040h
stoj_1:
	sub	ah, 070h
	shl	ah, 1
	dec	ah
	cmp	al, 080h
	cmc
	sbb	al, 01Fh
	cmp	al, 07Fh
	jb	stoj_2
	inc	ah
	sub	al, 05Eh
stoj_2:
	mov	c.w, ax
      }
# endif
      if (c.h < 0x7F) {
        return c.w;
      }
    }
  }
  return 0;
}

#endif

