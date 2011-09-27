/*_ tnodata.c						*/
/* Copyright (C) 1995 by Digital Mars		*/
/* All Rights Reserved					*/

#ifdef __NT__

/* Temporary Number openning flag, see TMPNAM.C, WTMPNAM.C
*/
char		__seed = 0;

/* Temporary file Sequence(Serial) Number
*/
unsigned	__tmpnum;

#endif
