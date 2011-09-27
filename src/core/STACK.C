/*
 * _ stack.c	06-Jan-85	Walter Bright	
 *
 */

/* default stack size	*/
#if M_UNIX || M_XENIX || DOS386
int _stack = 0x8000;
#elif __OS2__
int _stack = 0x4000;
#else
int _stack = 0x2000;
#endif

