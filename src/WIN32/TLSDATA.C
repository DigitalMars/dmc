/*_ tlsdata.c   Tue Jan	24 1995	*/
/* Copyright (C) 1995 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#if defined(_WIN32)

/************************************************************************
 * This file contains the necessary declartions for __declspec(thread)
 * support.  The main work is done by the linker.  The values in tls_used
 * are fixed up into a special segment by the linker that NT will then
 * fix up to FS:__tls_array for each thread.
 *   See file tlsseg.asm for other declarations.
 */
 
 extern long _tlsstart;
 extern long _tlsend;
 
 unsigned _tls_index = 0;  /* index referred to by compiler */
 void *_tls_callbacks_a[1] = { 0 };
 
 struct {
    void *tlsstart;   /* contains address of start of tls segment (Raw Data Start VA) */
    void *tlsend;     /* contains address of end of tls segment */
    void *tlsindex;   /* contains address of index into tls segment */
    void *tlscallbacks;  /* Pointer to an array of TLS callback functions
                            The array is null-terminated, so if there
                            is no callback function support, this field
                            points to four bytes set to 0.  The prototype
                            for these functions is the same as DLL-Entry.
                            These are most useful for declspec(thread) tagged
                            objects which need to be constructed and destructed
                            for each thread that attaches.
                          */
    long  zerofill;   /* The size in bytes of hte template beyond the 
                         initialized data defined by tlsstart 
                         and tlsend.  The total template 
                         size should be the same as the total size of TLS
                         data in the image file.  The zero fill is the amount
                         of data that comes after the initialized non-zero data
                         The linker fills this in at link time.
                       */
    long  characteristics;  /* Reserved, currently not used */                     
                         
 } _tls_used = {
    &_tlsstart,
    &_tlsend,
    &_tls_index,
    &_tls_callbacks_a[0],
    0,
    0
 };
 
#endif