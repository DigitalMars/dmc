#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _GENERIC_H_
#define _GENERIC_H_

/*
 *    generic.h -- for faking generic class declarations
 *
 * $Id: generic.h,v 1.1.1.1 1997/01/02 19:16:40 smarx Exp $
 *
 ****************************************************************************
 *
 * Rogue Wave Software, Inc.
 * P.O. Box 2328
 * Corvallis, OR 97339
 * Voice: (503) 754-3010	FAX: (503) 757-6650
 *
 * Copyright (C) 1991,  1993, 1994.
 * This software is subject to copyright protection under the laws of 
 * the United States and other countries.
 * ALL RIGHTS RESERVED
 *
 ***************************************************************************
 *
 * $Log: generic.h,v $
 * Revision 1.1.1.1  1997/01/02 19:16:40  smarx
 * cafe12
 *
 * 
 *    Rev 1.2   08 Oct 1994 14:11:52   BUSHNELL
 * Added pragma once for faster compilations
 * 
 *    Rev 1.1   02 Jun 1994 21:35:42   bushnell
 * added ifdef so that MS RC will not include header twice
 * 
 *    Rev 1.0   28 Apr 1994 19:19:50   thompson                   
 * Initial revision.
 * 
 *    Rev 1.0   20 Apr 1994 17:46:40   thompson                   
 * Initial revision.
 * Revision 1.2  1994/04/14  00:50:17  vriezen
 * Updated copywrite, added ID and LOG and changed comments to indicate .cpp filename
 *
 *
 * 
 */



// token-pasting macros; ANSI requires an extra level of indirection
#define name2(z, y)		name2_x(z, y)
#define name2_x(z, y)		z##y
#define name3(z, y, x)		name3_x(z, y, x)
#define name3_x(z, y, x)	z##y##x
#define name4(z, y, x, w)	name4_x(z, y, x, w)
#define name4_x(z, y, x, w)	z##y##x##w

// macros for declaring and implementing classes
#define declare(z, y)		name2(z, declare)(y)
#define implement(z, y)		name2(z, implement)(y)
#define declare2(z, y, x)	name2(z, declare2)(y, x)
#define implement2(z, y, x)	name2(z, implement2)(y, x)

// macros for declaring error-handling functions
extern genericerror(int, char*);
typedef int (*GPT)(int, char*);
#define set_handler(gen, tp, z)	name4(set_, tp, gen, _handler)(z)
#define errorhandler(gen, tp)	name3(tp, gen, handler)
#define callerror(gen, tp, z, y) (*errorhandler(gen, tp))(z, y)

#endif /* _GENERIC_H_ */
