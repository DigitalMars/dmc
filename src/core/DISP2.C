/*_ disp2.c   Fri Aug 18 1989   Modified by: Walter Bright */
/* Copyright (C) 1986-1994 by Digital Mars  */
/* All Rights Reserved					*/
/* Written by Walter Bright				*/
/* Fast screen display package			*/

#include	<stdio.h>
#include	<disp.h>
#include	<stdarg.h>
#include	<io.h>

/*********************************
 * Send a string to display.
 */

void disp_puts(const char *p)
{
    while (*p)
	disp_putc(*p++);
}

/***********************************
 * Fill a box with the specified attribute and character.
 */

void disp_fillbox(unsigned attrchar,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{
    unsigned x;

    for (; trow <= brow; trow++)
	for (x = lcol; x <= rcol; x++)
	    disp_pokew(trow,x,attrchar);
}

/**********************************
 * Draw a box on the screen.
 * The box must be at least 2 chars wide and 2 chars high.
 *	style		0 = double line
 *			1 = single line
 *			2 = solid
 *			3 = double horizontal, single vertical
 *			4 = single horizontal, double vertical
 *	attr		Attribute byte to use
 *	trow,lcol	Upper left corner
 *	brow,rcol	Lower right corner
 */

void disp_box(int style,int attr,
		unsigned trow, unsigned lcol,
		unsigned brow, unsigned rcol)
{   unsigned i;
    unsigned attchar;
    static struct
    {	unsigned char ul,ur,ll,lr;	/* upper left, etc.		*/
	unsigned char vert,horz;	/* vertical and horizontal segments */
    } *b,boxchar[] =
    {
	{ 0xC9,0xBB,0xC8,0xBC,0xBA,0xCD },	/* double line		*/
	{ 0xDA,0xBF,0xC0,0xD9,0xB3,0xC4 },	/* single line		*/
	{ 0xDB,0xDB,0xDB,0xDB,0xDB,0xDB },	/* solid box		*/
	{ 213, 184, 212, 190, 179, 205  },	/* double hor, single ver */
	{ 214, 183, 211, 189, 186, 196  },	/* single hor, double ver */
    };

    b = &boxchar[style];
    attr *= 256;

    /* Do corners	*/
    disp_pokew(trow,lcol,attr + b->ul);
    disp_pokew(trow,rcol,attr + b->ur);
    disp_pokew(brow,lcol,attr + b->ll);
    disp_pokew(brow,rcol,attr + b->lr);

    /* Do horizontal segments	*/
    attchar = attr + b->horz;
    for (i = lcol + 1; i < rcol; i++)
    {	disp_pokew(trow,i,attchar);
	disp_pokew(brow,i,attchar);
    }

    /* Do vertical segments	*/
    attchar = attr + b->vert;
    for (i = trow + 1; i < brow; i++)
    {	disp_pokew(i,lcol,attchar);
	disp_pokew(i,rcol,attchar);
    }
}

