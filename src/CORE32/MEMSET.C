
__declspec(naked) void _opti_stosd(void)
{
   _asm
   {

		push	ecx

		shr	ecx,3
      jz	do_dwords
l0:
		mov	[edi],eax
      mov	[edi+4],eax

      mov	[edi+8],eax
      mov	[edi+12],eax

      mov	[edi+16],eax
      mov	[edi+20],eax

      mov	[edi+24],eax
      mov	[edi+28],eax

      add	edi,32
      dec	ecx

      jnz	l0
do_dwords:
      pop	ecx

      and	ecx,7
      jz		do_7
l1:
      mov	[edi],eax
      add	edi,4

      dec	ecx
      jnz	l1
do_7:
      ret

   }
}

__declspec(naked) void *memset(void *p,int c,unsigned n)
{
   _asm
   {
      xor   eax, eax
      mov	al,8[esp]		/* c */
      mov	ecx,12[esp]		/* n */

      mov	edx,4[esp]		/* p */
      push	edi

      mov	ah,al
	  mov	edi,edx

      cmp	ecx,8
      jb	do_7

      mov	edx,eax

      shl	edx,16

      or	eax,edx
      mov	edx,ecx

      test	edi,3

      jnz	help_align


help_align_ret:

      shr	ecx,2
      and	edx,3

      call	_opti_stosd

      mov	ecx,edx
do_7:      
      test	ecx,ecx
      jz	sto_done

      
do_8:
	  mov	[edi],al
      inc	edi

      dec	ecx
      jnz	do_8
sto_done:
	  mov	eax,8[esp]
      pop	edi

      ret

help_align:
		mov	[edi],al
      inc	edi

      test	edi,3

      lea	ecx,[ecx-1]
      jnz	help_align

      mov	edx,ecx
      jmp	help_align_ret

   }
}

