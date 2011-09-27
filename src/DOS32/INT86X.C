/*****************************************************************************
Programmer: Glenn Anderson
Project: Digital Mars C++ Runtime Library, DOSX

This file implements the DOSX int86x, int86 routines.  With a little work it
could be massaged to work in any 32 bit model.
*****************************************************************************/

#include <string.h>
#include <dos.h>

#define UINT unsigned short

/*--------------------------------------------------------------------------*/
/* This routine will Free a selector allocated with int 21, funtion 0x3501  */

void FreeSelector(UINT selFree)
{
	_asm
	{
		mov	BX, selFree
		mov	AX, 3502h
		int	21h
	}
}

/*--------------------------------------------------------------------------*/
/* This routine will allocate a data segment alias for the given selector   */

UINT AllocCStoDSAlias(UINT selCS, int Len)
{
	UINT selNew;

	_asm
	{
	/* Allocate selector */
		mov	AX, 3501h
		int	21h
		jc		Error
		
	/* Get base address of given selector */
		mov	selNew, BX
		mov	BX, selCS
		mov	AX, 3504h
		int	21h
		jc    ErrorFree
		
	/* Set base address of new selector */
		mov	BX, selNew
		mov	AX, 3503h
		int	21h
		jc    ErrorFree
		
	/* Set limit of new selector */
		mov	BX, selNew
		mov	ECX, [Len]
		mov	AX, 3505h
		int	21h
		jc    ErrorFree
		jmp   Done

/* Free the allocated selector */
ErrorFree:
      push  selNew
      call  FreeSelector
      
/* Set the return value as an error */
Error:
      mov   selNew, 0

Done:
	}
	return selNew;
}

/*--------------------------------------------------------------------------*/
/* This is the int86x routine, this routine calls the generic interrupt by  */
/* modifying the code through the use of a data segment alias to the code   */
/* segment.  The regsOut and sregs variables can be NULL.  I suppose I could*/
/* allow the regsIn to be NULL as well, so that simple calls to int 0x28    */
/* and the like could be made.  Maybe, but someone could always use         */
/* _asm {int 28h} if they wanted to.                                        */

int int86x(int intNbr, union _REGS * regsIn, union _REGS * regsOut, struct _SREGS * sregs)
{
	UINT selNew;
	unsigned int iAX = 0;
	short int iDS = 0;

	_asm
	{
/* Save the segment registers */
		push	ES
		push	DS

/* Save the general purpose registers */
//		push	EBX
//		push	EDI
//		push	ESI

/* Save the base pointer, we will need this when the interrupt is done */
		push	EBP

/* Create a data segment alias, so that we can modify the int call below */
		lea	EAX, IntrAddr
		add	EAX, 10h
		push	EAX
		push	CS
		call	AllocCStoDSAlias
		add	ESP,8
		cmp   AX, 0                      /* If error then jump to the end */
		mov	selNew, AX
		je    Error

		mov	ES, AX                     /* Use ES to access the new selector */

/* Push the address of the label, retrieve it into BX, set the interrupt number */
		push	dword ptr IntrAddr
		pop	EBX
		inc	EBX
		mov   AX, (short)intNbr
		mov	ES:[byte ptr EBX], AL

/* Set up the registers from the passed in record */
		mov	EDI, [regsIn]

/* Here we are setting things up to set the flag register, to do this we 
	will use the current flag register and set the carry bit to the value of 
	the carry field.  Then we will push the result on the stack.  We actually
	set the flags by doing an iret to the address before the int call.  This 
	lets the interrupt mechanism do the work of settings the flags.  (I'm not
	sure why, but this is how it was done, so I kept it that way -GAA) */
		pushfd
		pop	EAX
		mov	EBX, [EDI+24]              /* EBX = regsIn.CFlags */
		cmp	EBX, 1                     /* Is it one, then set the bit */
		je		CflagSet

		and	EAX, 0FFFFFFFEh            /* otherwise, clear it */
		jmp	CFlagFixupDone

CflagSet:
		or		EAX, 000000001             /* set the bit */

CFlagFixupDone:
		push	EAX                        /* the flags part of the iret */
		push	CS                         /* the address to return to */
		push	IntrAddr

/* Set the rest of the registers from the regsIn variable */
		mov	EAX, [EDI]
		mov	EBX, [EDI+4]
		mov	ECX, [EDI+8]
		mov	EDX, [EDI+12]
		mov	ESI, [EDI+16]
		mov	EDI, [EDI+20]

/* We need these two, so save them */
		push	EDI
		push	EAX

/* This is the adress of the segment registers record */
		mov	EDI, [sregs]

		cmp	EDI, 0                     /* If it is NULL then skip this stuff */
		je		NoSegRegs

/* If ES is a good register then load it */
		mov	AX, [EDI]
		verr	AX
		jnz	NoES
		mov	ES, AX
NoES:

/* If DS is a good register then load it */
		mov	AX, [EDI+6]
		verr	AX
		jnz	NoDS
		mov	DS, AX
NoDS:

NoSegRegs:
/* Get these back */
		pop	EAX
		pop	EDI

/* This iret will load the flags and return control to IntrAddr */
		iretd

/* the interrupt instruction below will be modified above */
IntrAddr:
		int	0

/* Get back our BP, so that we can access the arguments */
		pop	EBP

/* This is the function result */
		mov	iAX, EAX

/* Save the current DS, and then resore ours so that accessing these pointers
	is possible */
		mov	AX, DS
		mov	[iDS], AX

/* Save these two on the stack, we're gonna need them later */
 		pushfd
		push	EDI

/* Get the address of where to save the registers */
		mov	EDI, [regsOut]
		cmp	EDI, 0                     /* Skip all this if NULL */
		je		SkipRegsOut

/* save the registers */
		mov	EAX, iAX
		mov	[EDI], EAX
		mov	[EDI+4], EBX
		mov	[EDI+8], ECX
		mov	[EDI+12], EDX
		mov	[EDI+16], ESI

/* Get back the saved DI */
		pop	EAX
		mov	[EDI+20], EAX

/* Get back the saved flags */
		pop	EAX
		mov	[EDI+28], EAX

/* Assume carry clear */
		mov	[EDI+24], 0
		test	EAX, 1
		jz		LoadSRegs

/* Carry not clear so set this flag */
		mov	[EDI+24], 1
		jmp	LoadSRegs

/* We saved the flags and EDI on the stack, but didn't need them, so
	just pop them off */
SkipRegsOut:
		pop	EAX
		pop	EAX

/* Load the segment registers in the var */
LoadSRegs:
		mov	EDI, [sregs]
		cmp	EDI,0                       /* Skip all this if NULL */
		je		SRegsDone

		mov	[EDI], ES
		mov	[EDI+2], CS
		mov	[EDI+4], SS
		mov	[EDI+6], DS
		mov	[EDI+8], FS
		mov	[EDI+10], GS

SRegsDone:
/* Restore our registers */
//		pop	ESI
//		pop	EDI
//		pop	EBX
		pop	DS
		pop	ES

/* Free our allocated selector */
		movsx	eax, selNew
		push	eax
		call	FreeSelector
		add	ESP,4

/* All done */
		jmp   Done

/* We had an error, restore our registers */
Error:
//		pop	ESI
//		pop	EDI
//		pop	EBX
		pop	EBP
		pop	DS
		pop	ES
Done:
	}

	return iAX;
}

/*--------------------------------------------------------------------------*/
/* This just calls int86x to do the work                                    */

int int86(int intNbr, union _REGS * regsIn, union _REGS * regsOut)
{
	return int86x(intNbr, regsIn, regsOut, NULL);
}

