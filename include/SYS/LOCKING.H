/* Copyright (C) 1986-1994 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __SYS_LOCKING_H
#define __SYS_LOCKING_H 1

#define _LK_UNLCK    0
#define _LK_LOCK     1
#ifdef M_UNIX
#define _LK_NBLCK    20
#else
#define _LK_NBLCK    2
#endif
#define _LK_RLCK     3
#define _LK_NBRLCK   4

#ifndef __STDC__
#define LK_UNLCK    _LK_UNLCK
#define LK_LOCK     _LK_LOCK
#define LK_NBLCK    _LK_NBLCK
#define LK_RLCK     _LK_RLCK
#define LK_NBRLCK   _LK_NBRLCK
#endif

#endif

