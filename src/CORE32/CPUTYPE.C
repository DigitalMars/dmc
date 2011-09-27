//_ test.c   Tue Mar  7 1995   Modified by: Walter Bright */

int __cputype = -1;  // Variable to hold cputype

///////////////////////////////////
// Determine which CPU is running.
//	int cputype(void);
// Returns:
//	3	80386
//	4	80486
//	5	Pentium

int __declspec(naked) cputype()
{
    __asm
    {
	;This code that distinguishes a 386 from a 486 depends on
	;the 386's inability to toggle the AC bit in the EFLAGS register,
	;but the 486 can. This technique is apparently blessed by Intel.

        cmp     __cputype,-1
        je      L2
        mov     EAX,__cputype
        ret
L2:
	push	EBX
	mov	EBX,ESP
	and	ESP,0FFFFFFFCh	;align down to nearest dword
	pushfd			;save original flags
	pushfd
	pop	EDX
	mov	ECX,EDX
	xor	EDX,40000h	;toggle AC bit
	push	EDX
	popfd
	pushfd
	pop	EDX
	and	EDX,40000h
	and	ECX,40000h
	mov	EAX,3		;assume 80386
	cmp	EDX,ECX
	je	L1		;it's a 386

	;It's at least a 486.
	;Determine if chip supports CPUID

	pushfd
	pop	EAX
	mov	ECX,EAX
	xor	EAX,200000h	;toggle ID bit
	push	EAX
	popfd
	pushfd
	pop	EAX
	xor	EAX,ECX
	mov	EAX,4
	je	L1		;it's a 486 w/o support for CPUID

	;Execute CPUID to determine processor type
	push	EBX		;CPUID modifies EBX
	mov	EAX,1
	cpuid
	mov	AL,AH
	and	EAX,0Fh
	pop	EBX

L1:	popfd			;restore original flags
	mov	ESP,EBX		;restore original ESP
        mov     __cputype,EAX    ;save value for subsequent calls
	pop	EBX
	ret
    }
}
