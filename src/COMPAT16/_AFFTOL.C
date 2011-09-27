/* _aFftol.c */
/* Copyright (C) 1995 Digital Mars */
/* All Rights Reserved */


#include <float.h>

/*
 * _fac is for MS Compatability, they use this as temporary 
 * storage for floating point operations.
 */
double _fac = 0.0;

/*
 * Microsoft compatable routine to convert a float
 * to a long in large code model.
 * upon entry, the floating point stack contains the float to convert.  
 * Per the C standard, when a float (or double) is converted to integer, 
 * you simply drop the decimal portion. To this end, before popping the 
 * float off of the stack, we must first set the floating point
 * flags to simply chop the decimal, then restore the flags.
 */
long __far _aFftol()
{
   long l;
   int i;
   int j;

   __asm { 
        fstcw i /* Get flags */
   }

   j = i;  /* save previous flags */
   i |= _RC_CHOP;	 /* Turn on chopping */

   __asm {
         fldcw i	; Set flags to "chop"
         fistp l	; Grab float off of stack and store in l
         fldcw j  ; restore previous flags
    }

   return l;  /* return converted float */
}

