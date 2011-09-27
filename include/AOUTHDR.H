/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __AOUTHDR_H
#define __AOUTHDR_H 1
/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

typedef	struct aouthdr
{
  short	magic;		/* Magic number -- usually set to 0x10B (0413). */
  short	vstamp;		/* Version stamp. */
  long tsize;		/* Text size. */
  long dsize;		/* Initialized data size. */
  long bsize;		/* Uninitialized data size. */
  long entry;		/* Entry point. */
  long text_start;	/* Base address of text. */
  long data_start;	/* Base address of data. */
} AOUTHDR;

#if __cplusplus
}
#endif

#endif
