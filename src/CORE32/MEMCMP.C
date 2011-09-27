/* Copyright (C) 1995 by Digital Mars
 * All Rights Reserved
 * Highly optimized string routines.
 */
 
#if __INTSIZE != 4
#error  32 bit only
#endif

#include <string.h>
#undef memcmp                // expose function declaration

#define _STDCALL __OS2__        // if use stdcall calling conventions
 

/****************************************************************************/
/* Compare two strings                                                      */

__declspec(naked) int __CLIB memcmp(const void * str1, const void * str2, unsigned n)
{
    _asm
    {
    ;; under DOSX, es and ds are not the same, so we need to make them so.. 
    #if DOS386
        push ds
        pop  es
    #endif
        push    edi
        push    esi

        mov    esi,str1[4+esp]    ;str1
        mov    edi,str2[4+esp]    ;str2

        mov    ecx,n[4+esp]    ;n

        cmp    ecx,4
        jb    DoBytes

        mov    eax,esi        ;do alignment if neither is dword aligned
        mov    edx,edi

        and    al,3
        jz    Aligned

        and    dl,3
        jz    Aligned
DoAlign:
        mov    al,[esi]    ;align esi to dword bounds
        mov    dl,[edi]

        cmp    al,dl
        jnz    Unequal

        inc    esi
        inc    edi

        test    esi,3

        lea    ecx,[ecx-1]
        jnz    DoAlign
Aligned:
        push    ecx
        ;
        ;actually, do multiple of 8 bytes at a time
        ;
        shr    ecx,2
        jz    TryOdd

        repe    cmpsd

        jnz    UnequalQuad

TryOdd:
        pop    ecx
DoBytes:
        ;
        ;if still equal and not end of string, do up to 7 bytes slightly
        ;slower.
        ;
        and    ecx,3
        jz    Equal
BytesLoop:
        repe    cmpsb

        jnz    Unequal
Equal:
        xor    eax,eax
        pop    esi

        pop    edi

#if _STDCALL
        ret    12
#else
        ret
#endif

UnequalQuad:
        pop    eax
        mov    edx,[edi-4]

        mov    eax,[esi-4]

        cmp    al,dl
        jnz    Unequal

        cmp    ah,dh
        jnz    Unequal

        shr    eax,16

        shr    edx,16

        cmp    al,dl
        jnz    Unequal

        cmp    ah,dh
Unequal:
        sbb    eax,eax
        pop    esi

        or     eax,1
        pop    edi

#if _STDCALL
        ret    12
#else
        ret
#endif
    }
}

