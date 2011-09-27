/* 
 * Copyright 1988, 1989 Hans-J. Boehm, Alan J. Demers
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */
/* Boehm, July 31, 1995 5:02 pm PDT */


#include <stdio.h>
#ifndef _WIN32_WCE
#include <signal.h>
#endif

#define I_HIDE_POINTERS	/* To make GC_call_with_alloc_lock visible */
#include "private/gc_pmark.h"

#ifdef SOLARIS_THREADS
# include <sys/syscall.h>
#endif
#if defined(MSWIN32) || defined(MSWINCE)
# define WIN32_LEAN_AND_MEAN
# define NOSERVICE
# include <windows.h>
# include <tchar.h>
#endif

# ifdef THREADS
#   ifdef PCR
#     include "il/PCR_IL.h"
      PCR_Th_ML GC_allocate_ml;
#   else
#     ifdef SRC_M3
	/* Critical section counter is defined in the M3 runtime 	*/
	/* That's all we use.						*/
#     else
#	ifdef SOLARIS_THREADS
	  mutex_t GC_allocate_ml;	/* Implicitly initialized.	*/
#	else
#          ifdef WIN32_THREADS
#	      if defined(_DLL) || defined(GC_DLL)
		 __declspec(dllexport) CRITICAL_SECTION GC_allocate_ml;
#	      else
		 CRITICAL_SECTION GC_allocate_ml;
#	      endif
#          else
#             if defined(IRIX_THREADS) \
		 || (defined(LINUX_THREADS) && defined(USE_SPIN_LOCK))
	        pthread_t GC_lock_holder = NO_THREAD;
#	      else
#	        if defined(HPUX_THREADS) \
		   || defined(LINUX_THREADS) && !defined(USE_SPIN_LOCK)
		  pthread_mutex_t GC_allocate_ml = PTHREAD_MUTEX_INITIALIZER;
	          pthread_t GC_lock_holder = NO_THREAD;
			/* Used only for assertions, and to prevent	 */
			/* recursive reentry in the system call wrapper. */
#		else 
	          --> declare allocator lock here
#		endif
#	      endif
#	   endif
#	endif
#     endif
#   endif
# endif

#ifdef ECOS
#undef STACKBASE
#endif

GC_FAR struct _GC_arrays GC_arrays /* = { 0 } */;


GC_bool GC_debugging_started = FALSE;
	/* defined here so we don't have to load debug_malloc.o */

void (*GC_check_heap) GC_PROTO((void)) = (void (*) GC_PROTO((void)))0;

void (*GC_start_call_back) GC_PROTO((void)) = (void (*) GC_PROTO((void)))0;

ptr_t GC_stackbottom = 0;

#ifdef IA64
  ptr_t GC_register_stackbottom = 0;
#endif

GC_bool GC_dont_gc = 0;

GC_bool GC_dont_precollect = 0;

GC_bool GC_quiet = 0;

GC_bool GC_print_stats = 0;

#ifdef FIND_LEAK
  int GC_find_leak = 1;
#else
  int GC_find_leak = 0;
#endif

#ifdef ALL_INTERIOR_POINTERS
  int GC_all_interior_pointers = 1;
#else
  int GC_all_interior_pointers = 0;
#endif

/*ARGSUSED*/
GC_PTR GC_default_oom_fn GC_PROTO((size_t bytes_requested))
{
    return(0);
}

GC_PTR (*GC_oom_fn) GC_PROTO((size_t bytes_requested)) = GC_default_oom_fn;

extern signed_word GC_mem_found;

# ifdef MERGE_SIZES
    /* Set things up so that GC_size_map[i] >= words(i),		*/
    /* but not too much bigger						*/
    /* and so that size_map contains relatively few distinct entries 	*/
    /* This is stolen from Russ Atkinson's Cedar quantization		*/
    /* alogrithm (but we precompute it).				*/


    void GC_init_size_map()
    {
	register unsigned i;

	/* Map size 0 to something bigger.			*/
	/* This avoids problems at lower levels.		*/
	/* One word objects don't have to be 2 word aligned,	*/
	/* unless we're using mark bytes.	   		*/
	  for (i = 0; i < sizeof(word); i++) {
	      GC_size_map[i] = MIN_WORDS;
	  }
#	  if MIN_WORDS > 1
	    GC_size_map[sizeof(word)] = MIN_WORDS;
#	  else
	    GC_size_map[sizeof(word)] = ROUNDED_UP_WORDS(sizeof(word));
#	  endif
	for (i = sizeof(word) + 1; i <= 8 * sizeof(word); i++) {
	    GC_size_map[i] = ALIGNED_WORDS(i);
	}
	for (i = 8*sizeof(word) + 1; i <= 16 * sizeof(word); i++) {
	      GC_size_map[i] = (ROUNDED_UP_WORDS(i) + 1) & (~1);
	}
#	ifdef GC_GCJ_SUPPORT
	   /* Make all sizes up to 32 words predictable, so that a 	*/
	   /* compiler can statically perform the same computation,	*/
	   /* or at least a computation that results in similar size	*/
	   /* classes.							*/
	   for (i = 16*sizeof(word) + 1; i <= 32 * sizeof(word); i++) {
	      GC_size_map[i] = (ROUNDED_UP_WORDS(i) + 3) & (~3);
	   }
#	endif
	/* We leave the rest of the array to be filled in on demand. */
    }
    
    /* Fill in additional entries in GC_size_map, including the ith one */
    /* We assume the ith entry is currently 0.				*/
    /* Note that a filled in section of the array ending at n always    */
    /* has length at least n/4.						*/
    void GC_extend_size_map(i)
    word i;
    {
        word orig_word_sz = ROUNDED_UP_WORDS(i);
        word word_sz = orig_word_sz;
    	register word byte_sz = WORDS_TO_BYTES(word_sz);
    				/* The size we try to preserve.		*/
    				/* Close to to i, unless this would	*/
    				/* introduce too many distinct sizes.	*/
    	word smaller_than_i = byte_sz - (byte_sz >> 3);
    	word much_smaller_than_i = byte_sz - (byte_sz >> 2);
    	register word low_limit;	/* The lowest indexed entry we 	*/
    					/* initialize.			*/
    	register word j;
    	
    	if (GC_size_map[smaller_than_i] == 0) {
    	    low_limit = much_smaller_than_i;
    	    while (GC_size_map[low_limit] != 0) low_limit++;
    	} else {
    	    low_limit = smaller_than_i + 1;
    	    while (GC_size_map[low_limit] != 0) low_limit++;
    	    word_sz = ROUNDED_UP_WORDS(low_limit);
    	    word_sz += word_sz >> 3;
    	    if (word_sz < orig_word_sz) word_sz = orig_word_sz;
    	}
#	ifdef ALIGN_DOUBLE
	    word_sz += 1;
	    word_sz &= ~1;
#	endif
	if (word_sz > MAXOBJSZ) {
	    word_sz = MAXOBJSZ;
	}
	/* If we can fit the same number of larger objects in a block,	*/
	/* do so.							*/ 
	{
	    size_t number_of_objs = BODY_SZ/word_sz;
	    word_sz = BODY_SZ/number_of_objs;
#	    ifdef ALIGN_DOUBLE
		word_sz &= ~1;
#	    endif
	}
    	byte_sz = WORDS_TO_BYTES(word_sz);
	if (GC_all_interior_pointers) {
	    /* We need one extra byte; don't fill in GC_size_map[byte_sz] */
	    byte_sz--;
	}

    	for (j = low_limit; j <= byte_sz; j++) GC_size_map[j] = word_sz;  
    }
# endif


/*
 * The following is a gross hack to deal with a problem that can occur
 * on machines that are sloppy about stack frame sizes, notably SPARC.
 * Bogus pointers may be written to the stack and not cleared for
 * a LONG time, because they always fall into holes in stack frames
 * that are not written.  We partially address this by clearing
 * sections of the stack whenever we get control.
 */
word GC_stack_last_cleared = 0;	/* GC_no when we last did this */
# ifdef THREADS
#   define BIG_CLEAR_SIZE 2048	/* Clear this much now and then.	*/
#   define SMALL_CLEAR_SIZE 256 /* Clear this much every time.		*/
# endif
# define CLEAR_SIZE 213  /* Granularity for GC_clear_stack_inner */
# define DEGRADE_RATE 50

word GC_min_sp;		/* Coolest stack pointer value from which we've */
			/* already cleared the stack.			*/
			
word GC_high_water;
			/* "hottest" stack pointer value we have seen	*/
			/* recently.  Degrades over time.		*/

word GC_words_allocd_at_reset;

#if defined(ASM_CLEAR_CODE)
  extern ptr_t GC_clear_stack_inner();
#else  
/* Clear the stack up to about limit.  Return arg. */
/*ARGSUSED*/
ptr_t GC_clear_stack_inner(arg, limit)
ptr_t arg;
word limit;
{
    word dummy[CLEAR_SIZE];
    
    BZERO(dummy, CLEAR_SIZE*sizeof(word));
    if ((word)(dummy) COOLER_THAN limit) {
        (void) GC_clear_stack_inner(arg, limit);
    }
    /* Make sure the recursive call is not a tail call, and the bzero	*/
    /* call is not recognized as dead code.				*/
    GC_noop1((word)dummy);
    return(arg);
}
#endif

/* Clear some of the inaccessible part of the stack.  Returns its	*/
/* argument, so it can be used in a tail call position, hence clearing  */
/* another frame.							*/
ptr_t GC_clear_stack(arg)
ptr_t arg;
{
    register word sp = (word)GC_approx_sp();  /* Hotter than actual sp */
#   ifdef THREADS
        word dummy[SMALL_CLEAR_SIZE];
	static unsigned random_no = 0;
       			 	 /* Should be more random than it is ... */
				 /* Used to occasionally clear a bigger	 */
				 /* chunk.				 */
#   endif
    register word limit;
    
#   define SLOP 400
	/* Extra bytes we clear every time.  This clears our own	*/
	/* activation record, and should cause more frequent		*/
	/* clearing near the cold end of the stack, a good thing.	*/
#   define GC_SLOP 4000
	/* We make GC_high_water this much hotter than we really saw   	*/
	/* saw it, to cover for GC noise etc. above our current frame.	*/
#   define CLEAR_THRESHOLD 100000
	/* We restart the clearing process after this many bytes of	*/
	/* allocation.  Otherwise very heavily recursive programs	*/
	/* with sparse stacks may result in heaps that grow almost	*/
	/* without bounds.  As the heap gets larger, collection 	*/
	/* frequency decreases, thus clearing frequency would decrease, */
	/* thus more junk remains accessible, thus the heap gets	*/
	/* larger ...							*/
# ifdef THREADS
    if (++random_no % 13 == 0) {
	limit = sp;
	MAKE_HOTTER(limit, BIG_CLEAR_SIZE*sizeof(word));
	return GC_clear_stack_inner(arg, limit);
    } else {
	BZERO(dummy, SMALL_CLEAR_SIZE*sizeof(word));
	return arg;
    }
# else
    if (GC_gc_no > GC_stack_last_cleared) {
        /* Start things over, so we clear the entire stack again */
        if (GC_stack_last_cleared == 0) GC_high_water = (word) GC_stackbottom;
        GC_min_sp = GC_high_water;
        GC_stack_last_cleared = GC_gc_no;
        GC_words_allocd_at_reset = GC_words_allocd;
    }
    /* Adjust GC_high_water */
        MAKE_COOLER(GC_high_water, WORDS_TO_BYTES(DEGRADE_RATE) + GC_SLOP);
        if (sp HOTTER_THAN GC_high_water) {
            GC_high_water = sp;
        }
        MAKE_HOTTER(GC_high_water, GC_SLOP);
    limit = GC_min_sp;
    MAKE_HOTTER(limit, SLOP);
    if (sp COOLER_THAN limit) {
        limit &= ~0xf;	/* Make it sufficiently aligned for assembly	*/
        		/* implementations of GC_clear_stack_inner.	*/
        GC_min_sp = sp;
        return(GC_clear_stack_inner(arg, limit));
    } else if (WORDS_TO_BYTES(GC_words_allocd - GC_words_allocd_at_reset)
    	       > CLEAR_THRESHOLD) {
    	/* Restart clearing process, but limit how much clearing we do. */
    	GC_min_sp = sp;
    	MAKE_HOTTER(GC_min_sp, CLEAR_THRESHOLD/4);
    	if (GC_min_sp HOTTER_THAN GC_high_water) GC_min_sp = GC_high_water;
    	GC_words_allocd_at_reset = GC_words_allocd;
    }  
    return(arg);
# endif
}


/* Return a pointer to the base address of p, given a pointer to a	*/
/* an address within an object.  Return 0 o.w.				*/
# ifdef __STDC__
    GC_PTR GC_base(GC_PTR p)
# else
    GC_PTR GC_base(p)
    GC_PTR p;
# endif
{
    register word r;
    register struct hblk *h;
    register bottom_index *bi;
    register hdr *candidate_hdr;
    register word limit;
    
    r = (word)p;
    if (!GC_is_initialized) return 0;
    h = HBLKPTR(r);
    GET_BI(r, bi);
    candidate_hdr = HDR_FROM_BI(bi, r);
    if (candidate_hdr == 0) return(0);
    /* If it's a pointer to the middle of a large object, move it	*/
    /* to the beginning.						*/
	while (IS_FORWARDING_ADDR_OR_NIL(candidate_hdr)) {
	   h = FORWARDED_ADDR(h,candidate_hdr);
	   r = (word)h;
	   candidate_hdr = HDR(h);
	}
    if (candidate_hdr -> hb_map == GC_invalid_map) return(0);
    /* Make sure r points to the beginning of the object */
	r &= ~(WORDS_TO_BYTES(1) - 1);
        {
	    register int offset = HBLKDISPL(r);
	    register signed_word sz = candidate_hdr -> hb_sz;
	    register signed_word map_entry;
	      
	    map_entry = MAP_ENTRY((candidate_hdr -> hb_map), offset);
	    if (map_entry > CPP_MAX_OFFSET) {
            	map_entry = (signed_word)(BYTES_TO_WORDS(offset)) % sz;
            }
            r -= WORDS_TO_BYTES(map_entry);
            limit = r + WORDS_TO_BYTES(sz);
	    if (limit > (word)(h + 1)
	        && sz <= BYTES_TO_WORDS(HBLKSIZE)) {
	        return(0);
	    }
	    if ((word)p >= limit) return(0);
	}
    return((GC_PTR)r);
}


/* Return the size of an object, given a pointer to its base.		*/
/* (For small obects this also happens to work from interior pointers,	*/
/* but that shouldn't be relied upon.)					*/
# ifdef __STDC__
    size_t GC_size(GC_PTR p)
# else
    size_t GC_size(p)
    GC_PTR p;
# endif
{
    register int sz;
    register hdr * hhdr = HDR(p);
    
    sz = WORDS_TO_BYTES(hhdr -> hb_sz);
    return(sz);
}

size_t GC_get_heap_size GC_PROTO(())
{
    return ((size_t) GC_heapsize);
}

size_t GC_get_free_bytes GC_PROTO(())
{
    return ((size_t) GC_large_free_bytes);
}

size_t GC_get_bytes_since_gc GC_PROTO(())
{
    return ((size_t) WORDS_TO_BYTES(GC_words_allocd));
}

size_t GC_get_total_bytes GC_PROTO(())
{
    return ((size_t) WORDS_TO_BYTES(GC_words_allocd+GC_words_allocd_before_gc));
}

GC_bool GC_is_initialized = FALSE;

void GC_init()
{
    DCL_LOCK_STATE;
    
    DISABLE_SIGNALS();
    LOCK();
    GC_init_inner();
    UNLOCK();
    ENABLE_SIGNALS();

#   if defined(PARALLEL_MARK) || defined(THREAD_LOCAL_ALLOC)
	/* Make sure marker threads and started and thread local */
	/* allocation is initialized, in case we didn't get 	 */
	/* called from GC_init_parallel();			 */
        {
	  extern void GC_init_parallel(void);
	  GC_init_parallel();
	}
#   endif /* PARALLEL_MARK || THREAD_LOCAL_ALLOC */
}

#if defined(MSWIN32) || defined(MSWINCE)
    CRITICAL_SECTION GC_write_cs;
#endif

#ifdef MSWIN32
    extern void GC_init_win32 GC_PROTO((void));
#endif

extern void GC_setpagesize();

#ifdef UNIX_LIKE

extern void GC_set_and_save_fault_handler GC_PROTO((void (*handler)(int)));

static void looping_handler(sig)
int sig;
{
    GC_err_printf1("Caught signal %d: looping in handler\n", sig);
    for(;;);
}
#endif

void GC_init_inner()
{
#   if !defined(THREADS) && defined(GC_ASSERTIONS)
        word dummy;
#   endif
    word initial_heap_sz = (word)MINHINCR;
    
    if (GC_is_initialized) return;
#   ifdef PRINTSTATS
      GC_print_stats = 1;
#   endif
    if (0 != GETENV("GC_PRINT_STATS")) {
      GC_print_stats = 1;
    } 
    if (0 != GETENV("GC_FIND_LEAK")) {
      GC_find_leak = 1;
    }
    if (0 != GETENV("GC_ALL_INTERIOR_POINTERS")) {
      GC_all_interior_pointers = 1;
    }
    if (0 != GETENV("GC_DONT_GC")) {
      GC_dont_gc = 1;
    }
#   ifdef UNIX_LIKE
      if (0 != GETENV("GC_LOOP_ON_ABORT")) {
        GC_set_and_save_fault_handler(looping_handler);
      }
#   endif
    /* Adjust normal object descriptor for extra allocation.	*/
    if (ALIGNMENT > GC_DS_TAGS && EXTRA_BYTES != 0) {
      GC_obj_kinds[NORMAL].ok_descriptor = ((word)(-ALIGNMENT) | GC_DS_LENGTH);
    }
#   if defined(MSWIN32) || defined(MSWINCE)
	InitializeCriticalSection(&GC_write_cs);
#   endif
    GC_setpagesize();
    GC_exclude_static_roots(beginGC_arrays, endGC_arrays);
    GC_exclude_static_roots(beginGC_obj_kinds, endGC_obj_kinds);
#   ifdef SEPARATE_GLOBALS
      GC_exclude_static_roots(beginGC_objfreelist, endGC_objfreelist);
      GC_exclude_static_roots(beginGC_aobjfreelist, endGC_aobjfreelist);
#   endif
#   ifdef MSWIN32
 	GC_init_win32();
#   endif
#   if defined(SEARCH_FOR_DATA_START)
	GC_init_linux_data_start();
#   endif
#   if defined(NETBSD) && defined(__ELF__)
	GC_init_netbsd_elf();
#   endif
#   if defined(IRIX_THREADS) || defined(LINUX_THREADS) \
       || defined(HPUX_THREADS) || defined(SOLARIS_THREADS)
        GC_thr_init();
#   endif
#   ifdef SOLARIS_THREADS
	/* We need dirty bits in order to find live stack sections.	*/
        GC_dirty_init();
#   endif
#   if !defined(THREADS) || defined(SOLARIS_THREADS) || defined(WIN32_THREADS) \
       || defined(IRIX_THREADS) || defined(LINUX_THREADS) \
       || defined(HPUX_THREADS)
      if (GC_stackbottom == 0) {
	GC_stackbottom = GC_get_stack_base();
#       if defined(LINUX) && defined(IA64)
	  GC_register_stackbottom = GC_get_register_stack_base();
#       endif
      }
#   endif
    GC_ASSERT(sizeof (ptr_t) == sizeof(word));
    GC_ASSERT(sizeof (signed_word) == sizeof(word));
    GC_ASSERT(sizeof (struct hblk) == HBLKSIZE);
#   ifndef THREADS
#     if defined(STACK_GROWS_UP) && defined(STACK_GROWS_DOWN)
  	ABORT(
  	  "Only one of STACK_GROWS_UP and STACK_GROWS_DOWN should be defd\n");
#     endif
#     if !defined(STACK_GROWS_UP) && !defined(STACK_GROWS_DOWN)
  	ABORT(
  	  "One of STACK_GROWS_UP and STACK_GROWS_DOWN should be defd\n");
#     endif
#     ifdef STACK_GROWS_DOWN
        GC_ASSERT((word)(&dummy) <= (word)GC_stackbottom);
#     else
        GC_ASSERT((word)(&dummy) >= (word)GC_stackbottom);
#     endif
#   endif
#   if !defined(_AUX_SOURCE) || defined(__GNUC__)
      GC_ASSERT((word)(-1) > (word)0);
      /* word should be unsigned */
#   endif
    GC_ASSERT((signed_word)(-1) < (signed_word)0);
    
    /* Add initial guess of root sets.  Do this first, since sbrk(0)	*/
    /* might be used.							*/
      GC_register_data_segments();
    GC_init_headers();
    GC_bl_init();
    GC_mark_init();
    {
	char * sz_str = GETENV("GC_INITIAL_HEAP_SIZE");
	if (sz_str != NULL) {
	  initial_heap_sz = atoi(sz_str);
	  if (initial_heap_sz <= MINHINCR * HBLKSIZE) {
	    WARN("Bad initial heap size %s - ignoring it.\n",
		 sz_str);
	  } 
	  initial_heap_sz = divHBLKSZ(initial_heap_sz);
	}
    }
    if (!GC_expand_hp_inner(initial_heap_sz)) {
        GC_err_printf0("Can't start up: not enough memory\n");
        EXIT();
    }
    /* Preallocate large object map.  It's otherwise inconvenient to 	*/
    /* deal with failure.						*/
      if (!GC_add_map_entry((word)0)) {
        GC_err_printf0("Can't start up: not enough memory\n");
        EXIT();
      }
    GC_register_displacement_inner(0L);
#   ifdef MERGE_SIZES
      GC_init_size_map();
#   endif
#   ifdef PCR
      if (PCR_IL_Lock(PCR_Bool_false, PCR_allSigsBlocked, PCR_waitForever)
          != PCR_ERes_okay) {
          ABORT("Can't lock load state\n");
      } else if (PCR_IL_Unlock() != PCR_ERes_okay) {
          ABORT("Can't unlock load state\n");
      }
      PCR_IL_Unlock();
      GC_pcr_install();
#   endif
    /* Get black list set up */
      if (!GC_dont_precollect) GC_gcollect_inner();
    GC_is_initialized = TRUE;
#   ifdef STUBBORN_ALLOC
    	GC_stubborn_init();
#   endif
    /* Convince lint that some things are used */
#   ifdef LINT
      {
          extern char * GC_copyright[];
          extern int GC_read();
          extern void GC_register_finalizer_no_order();
          
          GC_noop(GC_copyright, GC_find_header,
                  GC_push_one, GC_call_with_alloc_lock, GC_read,
                  GC_dont_expand,
#		  ifndef NO_DEBUGGING
		    GC_dump,
#		  endif
                  GC_register_finalizer_no_order);
      }
#   endif
}

void GC_enable_incremental GC_PROTO(())
{
# if !defined(SMALL_CONFIG)
  if (!GC_find_leak) {
    DCL_LOCK_STATE;
    
    DISABLE_SIGNALS();
    LOCK();
    if (GC_incremental) goto out;
    GC_setpagesize();
#   ifdef MSWIN32
      {
        extern GC_bool GC_is_win32s();

	/* VirtualProtect is not functional under win32s.	*/
	if (GC_is_win32s()) goto out;
      }
#   endif /* MSWIN32 */
#   ifndef SOLARIS_THREADS
        GC_dirty_init();
#   endif
    if (!GC_is_initialized) {
        GC_init_inner();
    }
    if (GC_dont_gc) {
        /* Can't easily do it. */
        UNLOCK();
    	ENABLE_SIGNALS();
    	return;
    }
    if (GC_words_allocd > 0) {
    	/* There may be unmarked reachable objects	*/
    	GC_gcollect_inner();
    }   /* else we're OK in assuming everything's	*/
    	/* clean since nothing can point to an	  	*/
    	/* unmarked object.			  	*/
    GC_read_dirty();
    GC_incremental = TRUE;
out:
    UNLOCK();
    ENABLE_SIGNALS();
  }
# endif
}


#if defined(MSWIN32) || defined(MSWINCE)
# define LOG_FILE _T("gc.log")

  HANDLE GC_stdout = 0;

  void GC_deinit()
  {
      if (GC_is_initialized) {
  	DeleteCriticalSection(&GC_write_cs);
      }
  }

  int GC_write(buf, len)
  char * buf;
  size_t len;
  {
      BOOL tmp;
      DWORD written;
      if (len == 0)
	  return 0;
      EnterCriticalSection(&GC_write_cs);
      if (GC_stdout == INVALID_HANDLE_VALUE) {
	  return -1;
      } else if (GC_stdout == 0) {
	  GC_stdout = CreateFile(LOG_FILE, GENERIC_WRITE,
        			 FILE_SHARE_READ | FILE_SHARE_WRITE,
        			 NULL, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH,
        			 NULL); 
    	  if (GC_stdout == INVALID_HANDLE_VALUE) ABORT("Open of log file failed");
      }
      tmp = WriteFile(GC_stdout, buf, len, &written, NULL);
      if (!tmp)
	  DebugBreak();
      LeaveCriticalSection(&GC_write_cs);
      return tmp ? (int)written : -1;
  }

#endif

#if defined(OS2) || defined(MACOS)
FILE * GC_stdout = NULL;
FILE * GC_stderr = NULL;
int GC_tmp;  /* Should really be local ... */

  void GC_set_files()
  {
      if (GC_stdout == NULL) {
	GC_stdout = stdout;
    }
    if (GC_stderr == NULL) {
	GC_stderr = stderr;
    }
  }
#endif

#if !defined(OS2) && !defined(MACOS) && !defined(MSWIN32) && !defined(MSWINCE)
  int GC_stdout = 1;
  int GC_stderr = 2;
# if !defined(AMIGA)
#   include <unistd.h>
# endif
#endif

#if !defined(MSWIN32) && !defined(MSWINCE) && !defined(OS2) && !defined(MACOS)
int GC_write(fd, buf, len)
int fd;
GC_CONST char *buf;
size_t len;
{
     register int bytes_written = 0;
     register int result;
     
     while (bytes_written < len) {
#	ifdef SOLARIS_THREADS
	    result = syscall(SYS_write, fd, buf + bytes_written,
	    			  	    len - bytes_written);
#	else
     	    result = write(fd, buf + bytes_written, len - bytes_written);
#	endif
	if (-1 == result) return(result);
	bytes_written += result;
    }
    return(bytes_written);
}
#endif /* UN*X */

#if defined(ECOS)
int GC_write(fd, buf, len)
{
  _Jv_diag_write (buf, len);
  return len;
}
#endif


#if defined(MSWIN32) || defined(MSWINCE)
#   define WRITE(f, buf, len) GC_write(buf, len)
#else
#   if defined(OS2) || defined(MACOS)
#   define WRITE(f, buf, len) (GC_set_files(), \
			       GC_tmp = fwrite((buf), 1, (len), (f)), \
			       fflush(f), GC_tmp)
#   else
#     define WRITE(f, buf, len) GC_write((f), (buf), (len))
#   endif
#endif

/* A version of printf that is unlikely to call malloc, and is thus safer */
/* to call from the collector in case malloc has been bound to GC_malloc. */
/* Assumes that no more than 1023 characters are written at once.	  */
/* Assumes that all arguments have been converted to something of the	  */
/* same size as long, and that the format conversions expect something	  */
/* of that size.							  */
void GC_printf(format, a, b, c, d, e, f)
GC_CONST char * format;
long a, b, c, d, e, f;
{
    char buf[1025];
    
    if (GC_quiet) return;
    buf[1024] = 0x15;
    (void) sprintf(buf, format, a, b, c, d, e, f);
    if (buf[1024] != 0x15) ABORT("GC_printf clobbered stack");
    if (WRITE(GC_stdout, buf, strlen(buf)) < 0) ABORT("write to stdout failed");
}

void GC_err_printf(format, a, b, c, d, e, f)
GC_CONST char * format;
long a, b, c, d, e, f;
{
    char buf[1025];
    
    buf[1024] = 0x15;
    (void) sprintf(buf, format, a, b, c, d, e, f);
    if (buf[1024] != 0x15) ABORT("GC_err_printf clobbered stack");
    if (WRITE(GC_stderr, buf, strlen(buf)) < 0) ABORT("write to stderr failed");
}

void GC_err_puts(s)
GC_CONST char *s;
{
    if (WRITE(GC_stderr, (char *)s, strlen(s)) < 0) ABORT("write to stderr failed");
}

#if defined(LINUX) && !defined(SMALL_CONFIG)
void GC_err_write(buf, len)
GC_CONST char *buf;
size_t len;
{
    if (WRITE(GC_stderr, buf, len) < 0) ABORT("write to stderr failed");
}
#endif

# if defined(__STDC__) || defined(__cplusplus)
    void GC_default_warn_proc(char *msg, GC_word arg)
# else
    void GC_default_warn_proc(msg, arg)
    char *msg;
    GC_word arg;
# endif
{
    GC_err_printf1(msg, (unsigned long)arg);
}

GC_warn_proc GC_current_warn_proc = GC_default_warn_proc;

# if defined(__STDC__) || defined(__cplusplus)
    GC_warn_proc GC_set_warn_proc(GC_warn_proc p)
# else
    GC_warn_proc GC_set_warn_proc(p)
    GC_warn_proc p;
# endif
{
    GC_warn_proc result;

    LOCK();
    result = GC_current_warn_proc;
    GC_current_warn_proc = p;
    UNLOCK();
    return(result);
}


#ifndef PCR
void GC_abort(msg)
GC_CONST char * msg;
{
#   if defined(MSWIN32)
      (void) MessageBoxA(NULL, msg, "Fatal error in gc", MB_ICONERROR|MB_OK);
      DebugBreak();
#   else
      GC_err_printf1("%s\n", msg);
#   endif
    if (GETENV("GC_LOOP_ON_ABORT") != NULL) {
	    /* In many cases it's easier to debug a running process.	*/
	    /* It's arguably nicer to sleep, but that makes it harder	*/
	    /* to look at the thread if the debugger doesn't know much	*/
	    /* about threads.						*/
	    for(;;) {}
    }
#   ifdef MSWIN32
	DebugBreak();
#   else
        (void) abort();
#   endif
}
#endif

#ifdef NEED_CALLINFO

void GC_print_callers (info)
struct callinfo info[NFRAMES];
{
    register int i;
    
#   if NFRAMES == 1
      GC_err_printf0("\tCaller at allocation:\n");
#   else
      GC_err_printf0("\tCall chain at allocation:\n");
#   endif
    for (i = 0; i < NFRAMES; i++) {
     	if (info[i].ci_pc == 0) break;
#	if NARGS > 0
	{
	  int j;

     	  GC_err_printf0("\t\targs: ");
     	  for (j = 0; j < NARGS; j++) {
     	    if (j != 0) GC_err_printf0(", ");
     	    GC_err_printf2("%d (0x%X)", ~(info[i].ci_arg[j]),
     	    				~(info[i].ci_arg[j]));
     	  }
	  GC_err_printf0("\n");
	}
# 	endif
     	GC_err_printf1("\t\t##PC##= 0x%X\n", info[i].ci_pc);
    }
}

#endif /* SAVE_CALL_CHAIN */

/* Needed by SRC_M3, gcj, and should perhaps be the official interface	*/
/* to GC_dont_gc.							*/
void GC_enable()
{
    GC_dont_gc--;
}

void GC_disable()
{
    GC_dont_gc++;
}

#if !defined(NO_DEBUGGING)

void GC_dump()
{
    GC_printf0("***Static roots:\n");
    GC_print_static_roots();
    GC_printf0("\n***Heap sections:\n");
    GC_print_heap_sects();
    GC_printf0("\n***Free blocks:\n");
    GC_print_hblkfreelist();
    GC_printf0("\n***Blocks in use:\n");
    GC_print_block_list();
}

#endif /* NO_DEBUGGING */
