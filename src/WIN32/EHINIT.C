/*_ ehinit.c	
 * Copyright (C) 1995 by Digital Mars
 * All rights reserved.
 */



void (*__cpp_init_ptr)(int);


/*
 * This uses the NOCOMDEFSEARCH feature of the 
 * linker to only pull in Except.cpp if necessary.
 * The way it works is the above declaration is a comdef 
 * initialized to zero.  In except.cpp, this same definition
 * appears, except it is set to a routine that initializes 
 * cpp exceptions. Except.cpp will only be pulled in when
 * a module compiled with -EH is pulled in.  This trick
 * relies on the fact that the linker will not try to search
 * a library for a public to match the comdef above.
 */
void __rtl_init_cppexceptions()
{
	if (__cpp_init_ptr)
       (*__cpp_init_ptr)(1);
}

void __rtl_clean_cppexceptions()
{
   if (__cpp_init_ptr)
		(*__cpp_init_ptr)(0);
}


