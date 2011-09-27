/* Copyright (C) 1995 by Digital Mars
 * All Rights Reserved
 * Highly optimized string routines.
 */
 
 
/****************************************************************************/
/* Compare two strings                                                      */

__declspec(naked) int strncmp(char const * str1, char const * str2, unsigned n)
{
	_asm
	{
	    
		push	edi
		push	esi		
		
		mov	esi,12[esp]	/*	str1 */
		mov	edi,16[esp]	/*	str2 */
		mov	eax,20[esp]	/*	n    */

		/*
		 *actually, do multiple of 4 bytes at a time
		 */
		shr	eax,2
		jz	TryOdd		
QuadLoop:
		mov	cl,[esi]	/*	Load the bytes (different base */
		mov	dl,[edi]	/*  registers on purpose) */

		cmp	cl,dl		/* Compare bytes */
		jnz	Unequal		
		test	cl,cl	/*	Check end of string */
		jz	Equal		

		mov	cl,[esi+1]	/*  Repeat 3 more times */
		mov	dl,[edi+1]	

		cmp	cl,dl
		jnz	Unequal		

		test	cl,cl
		jz	Equal		

		mov	cl,[esi+2]
		mov	dl,[edi+2]	

		cmp	cl,dl
		jnz	Unequal		

		test	cl,cl
		jz	Equal		

		mov	cl,[esi+3]
		mov	dl,[edi+3]	

		cmp	cl,dl
		jnz	Unequal		

		test	cl,cl
		jz	Equal		

		add	esi,4		/*	Update pointers */
		add	edi,4		

		dec	eax			/* Count by fours */
		jnz	QuadLoop	
TryOdd:
		mov	eax,20[esp]	/*	restore n */
		/*
		 * if still equal and not end of string, do up to 3 bytes slightly
		 * slower.  This could be unrolled, but probably not worth the size.
		 */
		and	eax,3
		jz	Equal		

		mov	cl,[esi]
		mov	dl,[edi]	

		cmp	cl,dl
		jnz	Unequal		

		test	cl,cl
		jz	Equal		

		dec	eax
		jz	Equal		

		mov	cl,[esi+1]
		mov	dl,[edi+1]	

		cmp	cl,dl
		jnz	Unequal		

		test	cl,cl
		jz	Equal		

		dec	eax
		jz	Equal		
		
		mov	cl,[esi+2]
		mov	dl,[edi+2]	
		cmp	cl,dl
		jnz	Unequal		
		
Equal:
		xor	eax,eax
		pop	esi		
		pop	edi		
		ret			

Unequal:
		sbb	eax,eax
		pop	esi		
		or 	eax,1
		pop	edi		
		ret			
	}
}

/****************************************************************************/
/* Compare two strings                                                      */

__declspec(naked) int strcmp(char const * str1, char const * str2)
{
	_asm
	{
		mov	ecx,4[esp]	/*	str1  */
		mov	edx,8[esp]	/*  str2  */

		push	ebx		
		
		/*
		 *do multiple of 4 bytes at a time
		 */
QuadLoop:
		mov	al,[ecx]	/*	Load the bytes (different base */
		mov	bl,[edx]	/*	registers on purpose) */
		

		cmp	al,bl		/*	Compare bytes */
		jnz	Unequal		

		test	al,al	/*	Check end of string */
		jz	Equal		

		mov	al,[ecx+1]	/*	Repeat 3 more times */
		mov	bl,[edx+1]	

		cmp	al,bl
		jnz	Unequal		

		test	al,al
		jz	Equal		

		mov	al,[ecx+2]
		mov	bl,[edx+2]	

		cmp	al,bl
		jnz	Unequal		

		test	al,al
		jz	Equal		

		mov	al,[ecx+3]
		mov	bl,[edx+3]	

		cmp	al,bl
		jnz	Unequal		

        
		add	ecx,4		/*	Update pointers */
		add	edx,4		
		
		test	al,al
		jnz	QuadLoop	
Equal:
		xor	eax,eax
		pop	ebx		
		ret			

Unequal:
		sbb	eax,eax
		pop	ebx		
      or    eax,1
		ret			
	}
}


/****************************************************************************/
/* This routine does a string copy                                          */

__declspec(naked) char * strcpy(char * str1, const char * str2)
{
	_asm
	{
		mov	ah,3		

		mov	ecx,4[esp]	/*	str1 */
		mov	edx,8[esp]	/*  str2 */
TestEdx:
		test	dl,ah
		jnz	DoAlign		
		
		/*
		 * do multiple of 4 bytes at a time
		 */
QuadLoop:
		mov	eax,[edx]	/*	Load the bytes */
		add	edx,4		/*	Update pointer */

		test	al,al	/*	Look for null */
		jz	CpyDoneLoLo	

		test	ah,ah		
		jz	CpyDoneLoHi	

		test	eax,0ff0000h	
		jz	CpyDoneHiLo	

		mov	[ecx],eax
		add	ecx,4		/*	(or 4 if misaligned) */

		test	eax,0ff000000h
		jnz	QuadLoop	
CpyDone:
		mov	eax,[esp+4]	
		ret			

CpyDoneLoLo:
		mov	[ecx],al	/* this is a zero */
		mov	eax,[esp+4]	

		ret			

CpyDoneLoHi:
		xor	dl,dl
		mov	[ecx],al	

		mov	[ecx+1],dl
		mov	eax,[esp+4]	

		ret			

CpyDoneHiLo:
		mov	[ecx],al
		xor	dl,dl		

		mov	[ecx+1],ah
		mov	eax,[esp+4]	

		mov	[ecx+2],dl	
		ret			

DoAlign:
		mov	al,[edx]
		inc	edx		

		mov	[ecx],al
		inc	ecx		

		test	al,al
		jnz	TestEdx		

		mov	eax,[esp+4]	
		ret			

	}
}


/****************************************************************************/
/* this routine gets the length of the given string                         */

__declspec(naked) unsigned strlen(char const * str1)
{
	_asm
	{
		mov	ecx,4[esp]	/*	str1  */
		mov	ah,3
TestEcx:
		test	cl,ah
		jnz	DoAlign		
		
		/*
		 * do multiple of 4 bytes at a time
		 */
QuadLoop:
		mov	eax,[ecx]	/*	Load the bytes  */
		add	ecx,4		/*	Update pointer  */

		test	al,al		/*	Look for null */
		jz	LenDoneLoLo	

		test	ah,ah		
		jz	LenDoneLoHi	

		test	eax,0ff0000h	
		jz	LenDoneHiLo	

		test	eax,0ff000000h
		jnz	QuadLoop	

		lea	eax,[ecx-1]
		mov	ecx,[esp+4]	

		sub	eax,ecx		

		ret			
		
LenDoneLoLo:
		lea	eax,[ecx-4]
		mov	ecx,[esp+4]	

		sub	eax,ecx		

		ret			
		
LenDoneLoHi:
		lea	eax,[ecx-3]
		mov	ecx,[esp+4]	;1

		sub	eax,ecx		;1

		ret			;2	4 Clocks Overhead

LenDoneHiLo:
		lea	eax,[ecx-2]
		mov	ecx,[esp+4]	;1

		sub	eax,ecx		;1

		ret			;2	4 Clocks Overhead

DoAlign:
		mov	al,[ecx]
		inc	ecx		;1

		test	al,al
		jnz	TestEcx		;1

		lea	eax,[ecx-1]
		mov	ecx,[esp+4]	;1

		sub	eax,ecx		;1

		ret			;2	4 Clocks Overhead

	}
}


/****************************************************************************/
/* This routine finds an occurance of chr in str1                           */

__declspec(naked) char * strchr(char const * str1, int chr)
{
	_asm
	{
	    mov	eax,4[esp]	/*	str1  */
		mov	cl,8[esp]	/*	chr   */
TestEax:
		test	al,3
		jnz	DoAlign		
		
		/*
		 * do multiple of 4 bytes at a time
		 */
QuadLoop:
		mov	edx,[eax]	/*	Load the bytes */
		add	eax,4		/*	Update pointer */

		cmp	dl,cl
		jz	FoundLoLo	

		test	dl,dl	/*	Look for null */
		jz	NotFound	

		cmp	dh,cl
		jz	FoundLoHi	

		test	dh,dh		
		jz	NotFound	

		shr	edx,16		

		cmp	dl,cl
		jz	FoundHiLo	

		test	dl,dl	/*	Look for null */
		jz	NotFound	

		cmp	dh,cl
		jz	FoundHiHi	

		test	dh,dh	
		jnz	QuadLoop	
NotFound:
		xor	eax,eax		

		ret			

FoundLoLo:
		sub	eax,4		

		ret			

FoundLoHi:
		sub	eax,3		

		ret			

FoundHiLo:
		sub	eax,2		

		ret			

FoundHiHi:
		dec	eax		

		ret			
DoAlign:
		mov	dl,[eax]
		inc	eax		

		cmp	dl,cl
		jz	FoundHiHi	

		test	dl,dl
		jnz	TestEax		

		xor	eax,eax		

		ret			

	}
}
