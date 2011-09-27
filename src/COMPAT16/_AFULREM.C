/* Copyright (C) 1986-1994 by Digital Mars.  */

/*
   The routine _aFulrem is for link compatability with 
	Microsoft.  It is a far routine that returns the remeinder
	of the 2 unsigned longs passed to it.
*/


unsigned long __far _aFulrem(unsigned long a, unsigned long b)
{
   unsigned long l = a % b;
   return l;
}
   
   
   
   
   
