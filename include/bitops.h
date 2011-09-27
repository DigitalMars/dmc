
#ifndef __BITOPS_H
#define __BITOPS_H 1

#if __cplusplus
extern "C" {
#endif


int __cdecl _inline_bsf(unsigned v);
int __cdecl _inline_bsr(unsigned v);
int __cdecl _inline_bt(unsigned *p, unsigned bit);
int __cdecl _inline_btc(unsigned *p, unsigned bit);
int __cdecl _inline_btr(unsigned *p, unsigned bit);
int __cdecl _inline_bts(unsigned *p, unsigned bit);

#if __cplusplus
}
#endif

#endif
