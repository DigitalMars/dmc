/*_ ehsup.h */
/* Copyright (C) 1994 by Digital Mars		*/
/* All Rights Reserved				*/

/* Exception Handling support stuff	*/
#pragma once

#if __cplusplus 
extern "C" {
#endif    


#if __INTSIZE == 4      // if 32 bit model
#define AX   EAX
#define SP   ESP      // streamline inline asm code
#define BP   EBP
#define FFAR         // is function table near or far
#define DFAR         // is data table near or far
#else
#if __SMALL__ || __MEDIUM__
#define FFAR
#define DFAR
#else
#define FFAR
#define DFAR   __far
#endif
#endif

// If you change this, change __cppeh_sv in \dm\src\core32\cinit.asm
typedef struct { char data[sizeof(long double)]; } largeprimtype;

#if __cplusplus
extern "C++"
#endif
extern void (__cdecl * __eh_delp)(void *); // in ehdata.cpp

typedef int (__pascal *ehfp_t)();   // function pointer in ambient memory model

// In 32-bit Models, "this" is passed in ECX, not on stack
#if __INTSIZE == 4
typedef int (__pascal *pdtor_t)();
#else
typedef int (__pascal *pdtor_t)(void *);
#endif

#if __SC__ < 0x620      // if not internally defined by compiler
struct __eh_cv
{   
    volatile void *p;   // pointer to object
    ~__eh_cv();
};
#endif

// Struct returned by __eh_finddata()
#pragma pack(1)
struct Ehfunc
{
    ehfp_t funcaddr;      // pointer to start of function
    char DFAR *pdata;      // pointer to data table for this function
    unsigned size;      // size in bytes of function
};
#pragma pack()


/*********************************
 * Exception record.
 * A pointer to this is what is passed as the 'catch variable initializer'
 * to a catch block. A pointer to Ehstack is actually what the p member of
 * struct __eh_cv is.
 */

// If you change this, change __cppeh_sv in \dm\src\core32\cinit.asm

#pragma pack(1)
struct Ehstack
{
    void *pobj;      // pointer to object thrown. The compiler assumes that
         // this is the first member (the compiler knows nothing
         // about the following members).
    struct Staticvar *pstatic;   // so we can find data for this exception
            // handling entity
    struct Ehstack *preveh;   // previous Ehstack thrown
    const char *type;   // type of object
    pdtor_t pdtor;   // destructor to call
    int tsize;      // size of object (0 if allocated by eh_new())
    int prev_tsize;
    int flags;
    #if defined(_WIN32)
    void* pVCInfo;      // VCX: VC++ ThrowInfo
    #define EHisVC    4 // VCX: this is a visual C++ exception
    #define EHspecok 1   // contains information about specification violations
    #endif
    #define EHinuse   2   // this one is in use
    union
    {   
        largeprimtype obj;   // data for object (tsize != 0)
        void *p;   // pointer to object (tsize == 0)
    };
    // I think __eh_delp should be placed here, so that ~__eh_cv() will always
    // know which free to use.
};
#pragma pack()

#define ehstackmax   10

struct Ehfunc FFAR * __cdecl __eh_finddata(ehfp_t addr);
unsigned __eh_find_caller(unsigned regbp,ehfp_t *pretaddr,int farfunc);

#if __cplusplus
extern "C++" extern int __rtti_match(const char *tthrow,const char *thandler,unsigned *poffset);
#else
extern int __rtti_match(const char *tthrow,const char *thandler,unsigned *poffset);
#endif

/***********************************
 * Gather all statics into one struct, so they can
 * easilly be duplicated, once for each thread.
 */

// If you change this, change __cppeh_sv in \dm\src\core32\cinit.asm

#pragma pack(1)
struct Staticvar
{
    struct Ehstack ehstack[ehstackmax];
    struct Ehstack *recent;   // most recently thrown exception
    int eh_tos;      // current top of ehstack[]
    int dtor_nest;   // nested within destructor called by stack unwinder
    int terminate_nest;
};

#pragma pack()

// Global variable for all cpp exception stuff..
extern struct Staticvar _cppeh_sv;  

#if defined(_WIN32)

#include <windows.h>
#include <excpt.h>

#pragma pack(1)
typedef struct func_data 
{
    short farfunc;  // Always 1 for NT
    void *funcoffset;  // Contains address of beginning of Current Function
    unsigned spoffset; // Contains Offset == ESP -EBP, used to calculate correct ESP value
    unsigned retoffset;  // Contains Ret Offset == RET - FUNCOFFSET.
    unsigned  especification;  // The rest of the structure is variable in length, so point at this address
} FUNC_DATA;
#pragma pack()


 
typedef struct _Scope_Table 
{
    int next;  // Pointer to next record
    void *Filter;  // Pointer to filter
    void *Handler;  // Pointer to exception handler block
} SCOPETABLE;
 
#define SCOPE_TABLE_SIZE 12
#define END_INDEX   -1   // Specifies end of scope table
 

/*
  Information setup by compiler for NT Frame Based Exception Handling
 */
typedef struct  _Establisher_Frame 
{
    void *prev; // Pointer to previous exception list
    void *handler;  // Pointer to routine for exception handler
    SCOPETABLE *scopetable;  // Pointer to current scope table
    DWORD scopeindex;  // Current index into scope table
    // EBP with VC 1.0 for NT, this variable contained the Frame Pointer
    // of the exception block, with VC 2.0, the & of this is the value
    // you need. -- AWB
    DWORD ebp;
    /*
       With VC 2.0, exceptioninfo and esp both reside prior to the frame
       pointer passed in, thus these fields are not accessed in except_handler3
     */  
    EXCEPTION_POINTERS *exceptioninfo;  // Contains pointer to context and exception records
    DWORD esp;  // Stack pointer of scope
 } ESTABLISHER_FRAME; 


/*
  Information pushed onto stack by Compler for C++ Exceptions under NT
 */
typedef struct  _CPP_Establisher_Frame 
{
    void *prev; // Pointer to previous exception list
    void *handler;  // Pointer to routine for exception handler
    DWORD dataindex;  // Current index into data table 
    DWORD ebp;   // Address of this is ebp of routine
 } CPP_ESTABLISHER_FRAME; 

 
 // Prototype RtlUnwind  with __stdcall
 // This routine is used to initiate a system unwind.
 // The first 2 arguements are always 0.
 // This routine takes the address to return to and a pointer
 // to the establisher frame for this exception.
 void __stdcall RtlUnwind(DWORD,DWORD,void *retAddr,void *frame);
 
 
 /*
    Helper routines for Frame Based Exception Handling
  */
 // Helper routine that calls the blocks
 int __cdecl _call_except_block(void *address, long callee_ebp);
 
 // Routine that initiates a global unwind
 void __cdecl _global_unwind(ESTABLISHER_FRAME *frame,EXCEPTION_RECORD *eRecord);
 
 // Routine that calls all termination handlers in the local frame
 void __cdecl _local_unwind (ESTABLISHER_FRAME *frame,int stop_index, int take_addr);
 
 // Exception handler used internally during unwinding.
 EXCEPTION_DISPOSITION __cdecl _local_except_handler (struct _EXCEPTION_RECORD *ExceptionRecord,
 	ESTABLISHER_FRAME * EstablisherFrame,
	struct _CONTEXT *ContextRecord,
	void *DispatcherContext
	);
	
	
// Some data
extern DWORD _except_list;  // Contains offset to exception list under NT

#define EXCEPTION_UNWIND  6  // Flag to indicate if the system is unwinding

#define STATUS_SYMANTEC_CPP_EXCEPTION 0xE0736330  // Corresponds to EOSC0

#endif
          
#if __cplusplus
}
#endif
 
 
