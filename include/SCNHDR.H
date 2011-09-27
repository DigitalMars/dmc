/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif


#ifndef __SCNHDR_H
#define __SCNHDR_H 1

/* Used when dealing with COFF files. */

#if __cplusplus
extern "C" {
#endif

struct scnhdr
{
  char s_name[8];		/* Section name, (.text, .data, .bss). */
  long s_paddr;			/* Physical address. */
  long s_vaddr;			/* Virtual address. */
  long s_size;			/* Section size. */
  long s_scnptr;		/* File ptr to raw data for section. */
  long s_relptr;		/* File ptr to relocation. */
  long s_lnnoptr;		/* File ptr to line numbers. */
  unsigned short s_nreloc;	/* Number of relocation entries. */
  unsigned short s_nlnno; 	/* Number of line number entries. */
  long s_flags;  		/* Type and content. */
};

#define SCNHDR struct scnhdr
#define	SCNHSZ sizeof(SCNHDR)

/*  Constants for names of special sections. */
#define _BSS  ".bss"
#define _DATA ".data"
#define _FINI ".fini"
#define _INIT ".init"
#define _TEXT ".text"
#define _TV  ".tv"
#define _COMM ".comment"

/* Section numbers for main sections */
#define COFF_TEXT       1
#define COFF_DATA       2
#define COFF_BSS        3
#define COFF_COMMENT    4
#define COFF_STIDATA    5       /* For static constructor function pointers */
#define COFF_STDDATA    6       /* For static destructors function pointers */
#define COFF_MAX_SCNS   6       /* Total number of possible sections */

/* s_flags -- The lower 2 bytes are the section type, upper 2 bytes are
section handling. */
#define STYP_REG 0x00		/* regular section: allocated, relocated, */
				/* loaded */
#define STYP_DSECT 0x01		/* dummy section: not allocated, relocated, */
				/* not loaded */
#define STYP_NOLOAD 0x02	/* noload section: allocated, relocated, */
				/* not loaded */
#define STYP_GROUP 0x04		/* grouped section: formed of input sections */
#define STYP_PAD 0x08		/* padding section: not allocated, not */
				/* relocated, loaded */
#define STYP_COPY 0x10		/* copy section: for decision function used */
				/* by field update; not	allocated, not */
                                /* relocated, loaded; reloc & lineno entries */
                                /* processed normally */
#define STYP_TEXT 0x20		/* text only */
#define STYP_DATA 0x40		/* data only */
#define STYP_BSS 0x80		/* bss only */
#define STYP_INFO 0x200		/* comment section : not allocated not */
				/* relocated, not loaded */
#define STYP_LIB 0x800		/* for .lib section : same as INFO */
#define STYP_OVER 0x400		/* overlay section : relocated not allocated */
				/* or loaded */

#if __cplusplus
}
#endif

#endif
