/* CTRLBRK */
/* GETCBRK */
/* SETCBRK */
/* $Revision: 1.1.1.1 $ */

#include <dos.h>
#include <process.h>
#include <int.h>


static  int     (*CtrlBrkFnc)(void);
static void (__interrupt __far *HandlerAddr)(void);


/*------------------------------------------------------*/
static void __interrupt __far CtrlBrkHandler()
{
	if(!(*CtrlBrkFnc)())
           _exit(0);
}

/*------------------------------------------------------*/
void ctrlbrk(int (*FarPtr)())
{
        CtrlBrkFnc = FarPtr;
        HandlerAddr = CtrlBrkHandler;
        int_setvector (0x23, FP_OFF (HandlerAddr), FP_SEG (HandlerAddr));
}

/*-------------------------------------------------------*/
int getcbrk(void)
{
	int 	result;

	_AX=0x3300;
	int_gen(0x21);
	return(_DL);
}

/*-------------------------------------------------------*/
int setcbrk(int value)
{
	_AX=0x3301;
	_DL=value;
        int_gen(0x21);
	return(_DL);
}
