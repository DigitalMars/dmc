/*_ alloca.h
 * Copyright (C) 1990-1995 by Digital Mars
 * All Rights Reserved
 * Written by Walter Bright
 * $Revision: 1.1.1.1 $
 */

#include <stdlib.h>

#ifndef NEW
#define NEW 1 // pass address in ECX rather than -4[EBP]
#endif

#if __INTSIZE == 2
#if _WINDOWS

//Offsets into stack segment maintained by Windows
#define STACKTOP 0xA
#define STACKMIN 0xC
#define STACKBOT 0xE

#elif __SMALL__ || __MEDIUM__

extern unsigned _BASE,_heapbottom;

#endif

#else

#if !(_WIN32 || __OS2__)
#if DOS386
extern size_t _x386_break;
#else
extern size_t _pastdata;
#endif
#endif

#endif

/*******************************************
 * Allocate data from the caller's stack frame.
 * This is a 'magic' function that needs help from the compiler to
 * work right, do not change its name, do not call it from other compilers!
 * Input:
 * nbytes  number of bytes to allocate
 * -4[EBP] number of bytes in the locals in the stack frame (old way)
 * ECX address of variable with # of bytes in locals (new way)
 *  This is adjusted upon return to reflect the additional
 *  size of the stack frame.
 * Returns:
 * EAX allocated data, NULL if stack overflows
 */

#if !_MSDOS
#define __ss
#endif

#if NEW
// Different name to avoid collisions with old name
__declspec(naked) void __ss * __CLIB __alloca(size_t nbytes)
#else
// Obsolete now, leave in to provide link compatibility with old code
__declspec(naked) void __ss * __CLIB alloca(size_t nbytes)
#endif
{
#if __INTSIZE == 4
    _asm
    {
    #if NEW
        mov EDX,ECX
    #endif
        mov EAX,4[ESP] ;get nbytes
        push EBX
        push EDI
        push ESI
        add EAX,3
        and EAX,0FFFFFFFCh ;round up to dword
        jnz Abegin
        mov EAX,4  ;allow zero bytes allocation, 0 rounded to dword is 4..
    Abegin: 
        mov ESI,EAX  ;ESI = nbytes
        neg EAX
        add EAX,ESP  ;EAX is now what the new ESP will be.
        jae Aoverflow
    #if _WIN32 || __OS2__
        // We need to be careful about the guard page
        // Thus, for every 4k page, touch it to cause the OS to load it in.
        mov ECX,EAX  ;ECX is new location for stack
        mov EBX,ESI  ;EBX is size to "grow" stack
    L1:
        test [ECX+EBX],EBX ;bring in page
        sub EBX,01000h ;next 4K page down
        jae L1  ;if more pages
        test [ECX],EBX ;bring in last page
    #elif DOS386
        // is ESP off bottom?
        cmp EAX,_x386_break
        jbe Aoverflow
    #elif !M_ELF
        cmp EAX,_pastdata
        jbe Aoverflow  ;Unlikely!! ~2 Gbytes under UNIX!
    #endif
        ;We must copy down to [ESP] the temps on the stack.
        ;The number of temps is (EBP - ESP - locals).
        mov ECX,EBP
        sub ECX,ESP
    #if NEW
        sub ECX,[EDX]  ;ECX = number of temps (bytes) to move.
        add [EDX],ESI  ;adjust locals by nbytes for next call to alloca()
    #else
        sub ECX,-4[EBP]  ;ECX = number of temps (bytes) to move.
        add -4[EBP],ESI  ;adjust locals by nbytes for next call to alloca()
    #endif
        mov ESP,EAX   ;Set up new stack pointer.
        add EAX,ECX   ;Return value = ESP + temps.
        mov EDI,ESP   ;Destination of copy of temps.
        add ESI,ESP   ;Source of copy.
        shr ECX,2   ;ECX to count of dwords in temps
                    ;Always at least 4 (nbytes, EIP, ESI,and EDI).
    #if DOS386
        mov DX,DS
        mov ES,DX
    #endif
        rep movsd
        jmp done

    Aoverflow:
        ;We overflowed the stack.  Return NULL.
        xor EAX,EAX

    done:
        pop ESI
        pop EDI
        pop EBX
    #if __OS2__
        ret 4
    #else
        ret
    #endif
    }
#else
    _asm
    {
    #if NEW
        mov DX,CX
    #endif
        push SI
        mov BX,SP
        mov BX,SS:nbytes[BX] ;get nbytes
        inc BX
        and BX,0FFFEh   ;round up to word
        jnz Abegin
        mov BX,2   ;allow alloca of zero bytes, minimal allocation is 2 bytes
    Abegin:
        neg BX
        mov SI,BX   ;SI = -nbytes
        add BX,SP
        jae Aoverflow

    #if _WINDOWS
        cmp SS:STACKTOP,BX
        ja Aoverflow
    #elif __SMALL__ || __MEDIUM__
        cmp BX,_BASE  ;is SP off bottom?
        jbe Aoverflow
        cmp BX,_heapbottom  ;has SP wrapped around?
        jae Aoverflow
    #endif
        xchg BX,SP

        ;We must copy down to [SP] the temps on the stack.
        ;The number of temps is (BP - BX - locals).

        mov CX,BP
        sub CX,BX
    #if NEW
        push BP
        mov BP,DX
        sub CX,[BP]   ;CX = number of temps
        sub [BP],SI   ;adjust locals by nbytes for next call to alloca()
        pop BP
    #else
        sub CX,-2[BP]  ;CX = number of temps
        sub -2[BP],SI  ;adjust locals by nbytes for next call to alloca()
    #endif
        mov AX,CX
        shr CX,1   ;to count of words in temps (always at least 3)
        even
    L2: 
        mov DX,SS:[BX]
        mov SS:[BX+SI],DX
        inc BX
        inc BX
        loop L2

        ;The offset of the allocated area is (SP + temps)
        add AX,SP
        mov DX,SS

        pop SI
    #if __SMALL__ || __COMPACT__
        ret
    #else
        retf
    #endif

    Aoverflow:
        ;We overflowed the stack. Return NULL
        xor AX,AX
        cwd
        pop SI
    #if __SMALL__ || __COMPACT__
        ret
    #else
        retf
    #endif
    }
#endif
}






















