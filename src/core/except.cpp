//_ except.cpp
// Copyright (C) 1994-1995 by Digital Mars
// All Rights Reserved
// Written by Walter Bright
// Runtime library support for exception handling


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#if _WIN32
#include <exception>
typedef void (__cdecl *__eh_fp)();
#else
#include <eh.h>
#endif

#include "ehsup.h"
#include "mt.h"

#if defined(_WIN32)
#include <stddef.h>
#include <windef.h>
#endif

// Do our own assert()
#undef assert
#define assert(e)   ((void)((e) || __eh_error()))


// If using far function calls
#define LARGECODE !(__SMALL__ || __COMPACT__)

#if LARGECODE
#define ret   retf
#endif

static struct Ehstack *push_exception(const char *, ehfp_t , unsigned , largeprimtype );
static struct Ehstack *pop_exception(struct Staticvar *,struct Ehstack *);


#if defined(_WIN32)


EXCEPTION_DISPOSITION __internal_cpp_framehandler(FUNC_DATA *,EXCEPTION_RECORD *,
                                   CPP_ESTABLISHER_FRAME *,CONTEXT *,void *);
static EXCEPTION_DISPOSITION Find_Foreign_Handler(FUNC_DATA *,EXCEPTION_RECORD *,
                                   CPP_ESTABLISHER_FRAME *,CONTEXT *,void *);                                   
static int Match_Specification(char *,const char *,int,char **);
static char *Skip_Handler(char *,short);
void _call_catch_block(long, unsigned,ehfp_t);
static void Find_CPP_Handler(CPP_ESTABLISHER_FRAME *,char *,Ehstack *,FUNC_DATA *,EXCEPTION_RECORD *);
void __cpp_local_unwind(CPP_ESTABLISHER_FRAME *,char *,int);
static Ehstack *Handle_Foreign_Exception(int);
extern "C" LONG WINAPI __CPPExceptionFilter(struct _EXCEPTION_POINTERS *);  // Default CPP Handler
static DWORD  __except_args[EXCEPTION_MAXIMUM_PARAMETERS];  // Args passed to cpp_frame_handler

// Unhandled Exceptions will pass through this.
extern "C" LPTOP_LEVEL_EXCEPTION_FILTER  _pCPPExceptionFilter = &__CPPExceptionFilter;
extern "C" LPTOP_LEVEL_EXCEPTION_FILTER  _pPreviousUnhandledExceptionFilter = NULL;

// VCX: Visual C++ Exception Handling Formards and Data Types
#define STATUS_VC_CPP_EXCEPTION  ('msc' | 0xE0000000)    
#define VC_CPP_MAGIC_NUMBER  0x19930520              
#pragma pack(4)
// PMD - Pointer to Member Data: generalized pointer-to-member descriptor
typedef struct PMD 
{
	ptrdiff_t       mdisp;          // Offset of intended data within base
	ptrdiff_t       pdisp;          // Displacement to virtual base pointer
	ptrdiff_t       vdisp;          // Index within vbTable to offset of base
} PMD;
// PMFN - Pointer to Member Function
typedef void (*PMFN)(void);

typedef struct TypeDescriptor 
{
	DWORD   hash;                   // Hash value computed from type's decorated name
	void *  spare;                  // reserved, possible for RTTI
	char    name[];                 // The decorated name of the type; 0 terminated.
} TypeDescriptor;

typedef struct CatchableType 
{
    unsigned int    properties;     // Catchable Type properties (Bit field)
    TypeDescriptor *pType;          // Pointer to the type descriptor for this type
    PMD   thisDisplacement;         // Pointer to instance of catch type within
                                                                                   //              thrown object.
    int   sizeOrOffset;             // Size of simple-type object or offset into
                                                                                        //  buffer of 'this' pointer for catch object
  PMFN  copyFunction;             // Copy constructor or CC-closure
} CatchableType;

// CatchableTypeArray - array of pointers to catchable types, with length

typedef struct CatchableTypeArray 
{
	int     nCatchableTypes;
	CatchableType   *arrayOfCatchableTypes[];
} CatchableTypeArray;

typedef struct ThrowInfo 
{
    unsigned int    attributes;     // Throw Info attributes (Bit field)
    PMFN            pmfnUnwind;     // Destructor to call when exception
                                  // has been handled or aborted.
    int     (__cdecl*pForwardCompat)(...);  // Forward compatibility frame handler
    CatchableTypeArray      *pCatchableTypeArray;   // Pointer to list of pointers to types.
} ThrowInfo;
#pragma pack()

Ehstack*Handle_VC_Exception(EXCEPTION_RECORD*,int);
int match_with_vc_throw_type(ThrowInfo* pTI,char *thandler,unsigned *poffset);


#endif                                   

// Macro to read a value of a type and advance the pointer.
#define PARG(ap,type)      (*(type DFAR *)(((ap)+=sizeof(type))-(sizeof(type))))



__eh_fp terminate_fp;
__eh_fp unexpected_fp;

/*********************************
 * A fatal error happened.
 */

void __eh_error()
{
#ifdef DEBUG
    printf("__eh_error()\n");
#endif
    abort();      // try to terminate gracefully
}

#pragma noreturn(__eh_error)

/*********************************
 * Default storage allocator for exception handling.
 */

void * __eh_new(unsigned size)
{
    return malloc(size);
}

void __eh_delete(void *p)
{
    free(p);
}



/******************************
 * Termination handling
 */

static void terminate_default()
{
#ifdef DEBUG
    printf("terminate_default()\n");
#endif
    abort();         // default behavior
}

__eh_fp __cdecl set_terminate(__eh_fp fp)
{   
    __eh_fp oldfp;

    oldfp = terminate_fp;
    terminate_fp = fp;
    return oldfp ? oldfp : terminate_default;
}

void __cdecl terminate()
{
#if defined(_MT)
    _getthreaddata()->t_cppeh_sv.terminate_nest++;
#else
    _cppeh_sv.terminate_nest++;
#endif
    if (terminate_fp)
        (*terminate_fp)();
    else
        terminate_default();
    assert(0);         // should never return
}

/******************************
 * Unexpected handling
 */

static void unexpected_default()
{
#ifdef DEBUG
    printf("unexpected_default()\n");
#endif
    terminate();      // default behavior
}

__eh_fp __cdecl set_unexpected(__eh_fp fp)
{   
    __eh_fp oldfp;

    oldfp = unexpected_fp;
    unexpected_fp = fp;
    return oldfp ? oldfp : unexpected_default;
}

void __cdecl unexpected()
{
    if (unexpected_fp) 
        (*unexpected_fp)();
    else
        unexpected_default();
    {
        __eh_cv e;         // just so we can call the dtor
#if defined(_MT)
        e.p = _getthreaddata()->t_cppeh_sv.recent;
#else
        e.p = _cppeh_sv.recent;
#endif
    }
}

/*********************************
 * Destructor for exception.
 * This destructs the object created in eh space, and pops
 * the exception off of the exception stack.
 */

__eh_cv::~__eh_cv()
{   
    Ehstack *peh;
    Ehstack eh;
    Staticvar *psv;

#ifdef DEBUG
    printf("__eh_cv::~__eh_cv()\n");
#endif
    peh = (Ehstack *) this->p;
    if (!peh)
        return;
    psv = peh->pstatic;

    assert(peh->flags & EHinuse);   // can't destroy twice
    peh->flags &= ~EHinuse;

    psv->recent = pop_exception(psv,peh);  // Get rid of block from list.
    
    // Destruct object
    if (peh->pobj)         // if not already destructed
    {
        if (peh->pdtor)
        {
#if __INTSIZE == 4
            //peh->pobj might be changed, in order to setup the catch variable,
            //in case the catch-type has an offset to the throwtype
            //so use peh->p (which always equals peh->pobj for UDTs) to destruct instead.
            _ECX = (int) peh->p;   // no need to save/restore ECX
            (*peh->pdtor)();
#else 
            (*peh->pdtor)(peh->p);
#endif
        }

        // Be careful here about free'ing object allocated by a DLL!
#if defined(_MT)
        if (!peh->tsize && psv == &_getthreaddata()->t_cppeh_sv)
#else
        if (!peh->tsize && psv == &_cppeh_sv)   // if object allocated by eh_new()
#endif
            (*__eh_delp)(peh->p);   // free object

        peh->pobj = NULL;
    }
    else
        assert(psv->recent != peh);

    assert(!psv->recent || psv->recent->flags & EHinuse);
}

/*********************************
 * Rethrow most recently thrown exception.
 */

void __eh_rethrow()
{   
    Ehstack *peh;

#ifdef DEBUG
    printf("__eh_rethrow()\n");
#endif
#if defined(_MT)
    peh = _getthreaddata()->t_cppeh_sv.recent;
#else
    peh = _cppeh_sv.recent;         // get most recently thrown exception
#endif
    if (!peh)            // if no recently thrown exception
        terminate();         // behavior not specified by standard,
               // assume this is correct

     peh->tsize=peh->prev_tsize;
     peh->pobj=NULL;

    __eh_throw(peh->type,(ehfp_t)peh->pdtor,peh->tsize,peh->obj);
}




/*********************************
 * An exception was thrown.
 * Input:
 *   type   pointer to a string identifying the type of the object
 *      thrown. If that string is "", then a type 'match' is
 *      indicated by the pointers matching.
 *   pdtor   pointer to function that is the destructor for the object
 *   tsize   size of thrown object. 0 if storage allocated by eh_new().
 *   ...   if (tsize == 0)
 *          pointer to the object
 *         else
 *          the object (<= 10 bytes in size)
 *    Non-NT version, has to take care of walking of stack .
 */
void __eh_throw(const char *type,ehfp_t pdtor,unsigned tsize,...)
{ 
    
    Ehstack *peh;
    va_list ap;
    largeprimtype obj;  // Temp passed to push exception
    unsigned regbp;
    unsigned retoffset;
    unsigned spoff;
    unsigned spvalue;
    ehfp_t retaddr;
    ehfp_t fp;
    int farfunc;
    struct Ehfunc FFAR *pfunc;
    int specification_violated = 0;  /* set when an exception specification is violated */

    int temp = sizeof(_cppeh_sv);

#ifdef DEBUG
    printf("__eh_throw(type=%p, pdtor=%p, tsize=%u)\n",type,pdtor,tsize);
#endif

    
    va_start(ap,tsize);
    obj = va_arg(ap,largeprimtype);
    va_end(ap);

#ifdef DEBUG
    printf ("Calling push_exception\n");
#endif
    
    peh = push_exception(type,pdtor,tsize,obj);
    
#ifdef DEBUG
    printf ("Returned from push_exception\n");
#endif

    assert(peh);
    
#if defined(_WIN32)    
    /* Under Win32, we need to simply raise Exception 
       _cpp_framehandler and the system will unwind stack and call catch blocks etc. */
       __except_args[0] = (DWORD)peh;  // Pass Exception information to cppframe
#ifdef DEBUG
    printf ("Raising Exception\n");
#endif
    // Raise the exception, giving it our Exception Code.
    RaiseException(STATUS_SYMANTEC_CPP_EXCEPTION,EXCEPTION_NONCONTINUABLE,1,__except_args);
#else

    /* Non-Win32 version takes care of stack walking etc. */
#if __INTSIZE == 4
    regbp = _EBP;
#else
    regbp = _BP;
#endif
    retaddr = (ehfp_t) __eh_throw;
#if LARGECODE
    farfunc = 2;
#else
    farfunc = 1;
#endif
    while (1)         // for each function on the stack
    {   
        char DFAR *pdata;
        unsigned funcoffset;
        int any;
        int match;
        short index;
        int nhandlers;
        int dim;
        short ntrys;

        regbp = __eh_find_caller(regbp,&retaddr,farfunc);
        if (!regbp) 
            // if end of call chain
            break;
        
        pfunc = __eh_finddata(retaddr);   // find static data associated with function
        if (!pfunc)         // if no static data
        {   
            farfunc = 0;
            continue;
        }
        funcoffset = *(unsigned FFAR *)&(pfunc->funcaddr);
        pdata = pfunc->pdata;

        farfunc = PARG(pdata,short);
        spoff = PARG(pdata,unsigned);
        retoffset = PARG(pdata,unsigned);

#ifdef DEBUG
        printf("retaddr = x%x\n",(unsigned)retaddr);
        printf("regbp=x%04x, funcoffset=x%04x, farfunc=%d, spoff=x%x, retoffset=x%x\n",
        regbp,funcoffset,farfunc,spoff,retoffset);
#endif

        if (specification_violated) 
            goto Lunexpected;

        // Do exception-specification
        any = 0;
        match = 0;
        while (1)
        {   
            char *pes;

            pes = PARG(pdata,char *);
            if (!pes)         // if end of exception-specification
                break;
            if (pes == (char *)(-1L))   // if exception-specification is empty
                goto Lunexpected;
            any = 1;
            if (!match)
                // if type-id is compatible with thrown object type
                match = __rtti_match(type,pes,NULL);
        }
   
   
        if (any && !match)
            specification_violated = 1;

        // Find start index for retaddr in static data
        dim = PARG(pdata,int);
        index = -1;
        for (int i = 0; i < dim; i++)
        {   
            unsigned DFAR *pi;

            pi = (unsigned DFAR *)(pdata + i * (sizeof(int) + 2));
            if ((unsigned)retaddr > funcoffset + *pi)
                index = *(unsigned short DFAR *)(pi + 1);
        }
        pdata += dim * (sizeof(int) + 2);
#ifdef DEBUG
        printf("dim=%d, index=%d\n",dim,index);
#endif

        // Skip over handler-table
        ntrys = PARG(pdata,short);
#ifdef DEBUG
        printf("ntrys=%d\n",ntrys);
#endif
        for (int i = 0; i < ntrys; i++)
        {
            pdata += sizeof(int);   // skip BP offset of catch variable
            nhandlers = PARG(pdata,short);
            pdata += nhandlers * (sizeof(int) + sizeof(void *));
        }

        while (index != -1)
        { 
            int prev;
            char ty;
            char DFAR *ph;
            unsigned spsave;
            void *pthis;
            pdtor_t dtor;
            unsigned tryoff;
            unsigned cvoffset;
            short nhandlers;
            unsigned bpoff;

            ph = pdata + index * (sizeof(int) + 1 + 2 * sizeof(int) + sizeof(void (*)()));
            prev = PARG(ph,int);
            ty = PARG(ph,char);

#ifdef DEBUG
            printf("index=%2d, prev=%2d, ty=%d\n",index,prev,ty);
#endif
            switch (ty)
            {
                case 1:   
                    if (!specification_violated)
                    {
                        // try-block
                        tryoff = PARG(ph,unsigned);
                        ph = pfunc->pdata + tryoff;   // offset to handler data

                        cvoffset = PARG(ph,unsigned);
                        nhandlers = PARG(ph,short);
    #ifdef DEBUG
                        printf("tryoff=x%x, cvoffset=x%x, nhandlers=%d\n",
                                tryoff,cvoffset,nhandlers);
    #endif
                        for (int i = 0; i < nhandlers; i++)   // for each catch block
                        {   
                            char *htype;
                            unsigned thisoffset;
                            unsigned hoffset;

                            hoffset = PARG(ph,unsigned);
                            htype = PARG(ph,char *);
                            // If thrown type matches handler type
                            thisoffset = 0;
                            if (__rtti_match(type,htype,&thisoffset))
                            {   
                                unsigned spvalue;
                                void * __ss *pcv;   // pointer to catch variable

                                // Copy obj into catch variable

                                //for UDT pobj must be changed as this contains the pointer to the actual exception object
                                //(we do not change p as this might be needed to destruct the COMPLETE object)
                                //for pointers pobj points to peh->p which contains the actual pointer
                                //so peh->p must be changed and NOT pobj.

                                if (thisoffset) {
                                   if (peh->tsize==0)
                                     peh->pobj = (char *)peh->p + thisoffset;
                                   else
                                     peh->p=(char*)peh->p+thisoffset;
                                }
                                peh->prev_tsize=peh->tsize;
                                if (peh->tsize==0)
                                  peh->tsize=sizeof(void*);

                                pcv = (void * __ss *)(regbp + cvoffset);
                                *pcv = peh;

#ifdef DEBUG
                                printf("calling catch block at offset x%x\n",hoffset);
#endif
                                spvalue = regbp - spoff - sizeof(fp);
                                fp = pfunc->funcaddr;
                                *(unsigned *)&fp += hoffset;
                                *(ehfp_t __ss *)spvalue = fp;
                                __asm
                                {
                                    mov   AX,spvalue
                                    mov   BP,regbp
                                    mov   SP,AX      // reset stack
                                    ret         // jump to catch block
                                }
                            }
                        }
                    }
                    break;
                // Call destructors for all active auto objects
                case 2:      // BP offset, 0, dtor
                    bpoff = PARG(ph,unsigned);
                    (void) PARG(ph,unsigned);
                    pthis = (void __ss *)(bpoff + regbp);
                    goto Lcalldtor;
                case 3:      // BP offset of this, offset from this, dtor
                    bpoff = PARG(ph,unsigned);
                    pthis = (void __ss *)(bpoff + regbp);
                    pthis = (void *)(*(char **)pthis + PARG(ph,unsigned));
                    goto Lcalldtor;
                case 4:      // pointer, 0, dtor
                    pthis = PARG(ph,void *);
                    if (sizeof(void *) == sizeof(int))
                        (void) PARG(ph,void *);
                    goto Lcalldtor;

                Lcalldtor:
                    dtor = PARG(ph,pdtor_t);
                #if defined(_MT)
                    _getthreaddata()->t_cppeh_sv.dtor_nest++;
                #else
                    _cppeh_sv.dtor_nest++;
                #endif

                    // Note that pdtor type uses a pascal calling sequence,
                    // so that the following will work whether the actual
                    // function called is __cdecl or __pascal.
                    __asm   mov   spsave,SP
#if __INTSIZE == 4
                    _ECX = (int)pthis;
                    (*dtor)();
#else
                    (*dtor)(pthis);
#endif
                    __asm   mov   SP,spsave
                #if defined(_MT)
                    _getthreaddata()->t_cppeh_sv.dtor_nest--;
                #else
                    _cppeh_sv.dtor_nest--;
                #endif
                    break;
                default:
                    goto Lterminate;
            }
            index = prev;      // previous index
        }
    }
    goto Lterminate;         // can't find handler

Lunexpected:
    // Called when a function with an exception-specification throws an
    // exception not listed in the exception-specification.
    // Set up stack so that it is as if unexpected() were called 
    // in place of the function whose exception-specification were violated.
    spvalue = regbp - spoff - 2 * sizeof(fp);
    *(ehfp_t __ss *)(spvalue + sizeof(int)) = retaddr;  
    fp = (ehfp_t) unexpected;
    *(ehfp_t __ss *)spvalue = fp;
    __asm
    {
        mov   AX,spvalue
        mov   BP,regbp
        mov   SP,AX
        ret            // jmp to unexpected()
    }
#endif // defined(_WIN32)    

Lterminate:
    // Called when:
    // o exception handling mechanism cannot find a handler for a
    //    thrown exception
    // o exception handling mechanism finds the stack is corrupted
    // o a destructor called during stack unwinding caused by an
    //   exception tries to exit using an exception
    
    terminate();
    goto Lerror;      // returning from terminate() is an error

Lerror:
    // Programmer error, this should never happen
    assert(0);
}



/********************************
 * Given the current information for an exception to "throw", push it onto the eh stack
 * and return a pointer to it.  
 */
static struct Ehstack *push_exception(const char *etype, ehfp_t epdtor, unsigned esize, largeprimtype eobj)
{
   
    struct Ehstack *peh = NULL;
    int i;
#if defined(_MT)
    struct THREAD *tinfo = _getthreaddata();
    assert(tinfo);

    assert(!tinfo->t_cppeh_sv.terminate_nest);
#else
    assert(!_cppeh_sv.terminate_nest);  // if nested within terminate() call
#endif

#if defined(_MT)
    if (tinfo->t_cppeh_sv.dtor_nest)
#else
    if (_cppeh_sv.dtor_nest) 
#endif
    {
        // if nested within destructor called by stack unwinder
        terminate();
        assert(0);
    }

    // Find an unused one
    for (i = 0; 1; i++)
    {
        assert(i < ehstackmax);   // we really should try to alloc more memory
#if defined(_MT)
        peh = &tinfo->t_cppeh_sv.ehstack[i];
#else
        peh = &_cppeh_sv.ehstack[i];
#endif
        if (!(peh->flags & EHinuse))
            break;      // found an unused one
    }

    peh->type = etype;
    peh->pdtor = (pdtor_t) epdtor;
    peh->tsize = esize;
    peh->obj = eobj;
    peh->pobj = (esize == 0) ? peh->p : &peh->obj;
#if defined(_WIN32)
    peh->flags |= EHspecok;  // Initially, specificiation matches ok
#endif
    peh->flags |= EHinuse;

#if defined(_MT)
    peh->preveh = tinfo->t_cppeh_sv.recent;
    tinfo->t_cppeh_sv.recent = peh;
    peh->pstatic = &tinfo->t_cppeh_sv;
#else
    peh->preveh = _cppeh_sv.recent;
    _cppeh_sv.recent = peh;
    peh->pstatic = &_cppeh_sv;  // initialize pstatic to static in sv.
#endif
    
    return peh;
}    


static Ehstack *pop_exception(struct Staticvar *psv,struct Ehstack *peh)
{
    
    Ehstack *prev;   // Will contain pointer to node prior to one we want deleted.
    Ehstack *temp;
    
    assert(psv);
    assert(peh);
    
    /* walk list finding peh, stopping at node prior to one we want to delete  */
    for (prev = temp = psv->recent; temp && temp != peh; temp = temp->preveh) 
        prev = temp;  
    
       
    /* We didn't find it, we had better return things as is. 
       This could happen if object was thrown in DLL, but caught
       in exe??  */
    if (!temp) 
    {
        prev = psv->recent;
        goto end;
    }
    
    
    /* Was first node, repoint */
    if (temp == prev) 
    {
        prev = prev->preveh;
    }
    else 
    {
        prev->preveh = temp->preveh;
        prev = psv->recent;
    }
end:     
    return prev;
}    
    
    
/******************************
 * Given BP, find return address to caller, and caller's BP.
 * Input:
 *   regbp      Value of BP for current function
 *   *pretaddr   Return address (used for CS value if caller is near)
 *   farfunc      0 don't know
 *         1 near function
 *         2 far function
 * Output:
 *   *pretaddr   return address to caller
 * Returns:
 *   previous BP
 */

unsigned __eh_find_caller(unsigned regbp,ehfp_t *pretaddr,int farfunc)
{
    unsigned bp = *(unsigned __ss *)regbp;
    unsigned sl;      // segment limit

    if (!bp)         // if end of call chain
        return bp;      // return 0

    // Perform sanity checks on new BP.
    // If it is screwed up, terminate() hopefully before we do more damage.
    if (bp <= regbp) 
        // stack should grow to smaller values
        terminate();

#if LARGECODE
    if (farfunc == 0)
    {   // We have to figure it out.
        unsigned short regcs;
        unsigned regip;

        farfunc = 2;      // assume far unless we prove it near

        // If BP is odd, assume far frame (old method of marking far frames)
        if (bp & 1)
           ;
        else if (_cpumode == _PROT_MODE)      // if in protected mode
        {   
            unsigned ar;
            unsigned cslimit;

            regcs = *(unsigned short __ss *)(regbp + sizeof(int) + sizeof(int));
            regip = *(unsigned __ss *)(regbp + sizeof(int));

            // If seg is not a valid code selector, then this is a near function.
            __asm
            {
#if __INTSIZE == 4
                lar   EAX,regcs
                jz   L1      // if selector is visible
                xor   EAX,EAX
            L1:     
                mov   ar,EAX
#else
                lar   AX,regcs
                jz   L1      // if selector is visible
                xor   AX,AX
            L1:   
                mov   ar,AX
#endif
            }
            if (!(ar & 0x800))      // if not a code segment
                goto Lnear;      // must be a near frame

            // Ensure ip is within segment limit
            __asm
            {
#if __INTSIZE == 4
                lsl   EAX,regcs
                mov   cslimit,EAX
#else
                lsl   AX,regcs
                mov   cslimit,AX
#endif
            }
            if (regip > cslimit)
                goto Lnear;
        }
        else
        {   // Real mode
        } 

        // We're still not *sure* if it is a near or far frame.
        // We can be mostly sure by scanning forward from *pretaddr until
        // we encounter a RET instruction, which will either be a
        // near or far instruction.

        // not implemented, assume far frame
    }

    if (farfunc == 2)
        *pretaddr = *(ehfp_t __ss *)(regbp + sizeof(int));
    else
    {
Lnear:
        // Do not disturb CS value in *pretaddr
        *(void __near **)pretaddr = *(void __near * __ss *)(regbp + sizeof(int));
    }
#else
        // Assume all far functions are in the same near code segment
        *pretaddr = *(ehfp_t __ss *)(regbp + sizeof(int));
#endif
    return bp & ~1;
}




#if defined(_WIN32)

/***********
 *  _cpp_framehandler:
 *  Handle CPP Exception under Win32
 */
extern "C"  
EXCEPTION_DISPOSITION  _cpp_framehandler(EXCEPTION_RECORD *eRecord,
                   CPP_ESTABLISHER_FRAME * frame,CONTEXT *context,void *dispatchercontext)
{
    // Local variable that matches Exception Specification Table
    FUNC_DATA *fdata;  // Corresponds to pfunc->data returned from eh_finddata
   
   
   
    // Spec is located in EAX upon entry of this routine
    __asm { mov fdata, EAX }
   
   
    // Now call internal routine to do all the work
    return __internal_cpp_framehandler(fdata,eRecord,frame,context,dispatchercontext);
}
      
      
      
EXCEPTION_DISPOSITION __internal_cpp_framehandler(FUNC_DATA *funcdata, 
                           EXCEPTION_RECORD *eRecord,
                           CPP_ESTABLISHER_FRAME *frame, 
                           CONTEXT *context, 
                           void *dispatchercontext)
{
    Ehstack *peh = NULL;  // Information stored into eRecord in throw
    int nhandlers; // Number of catch blocks for this function
    char *pdata;
    ehfp_t fp;
    int unwinding = eRecord->ExceptionFlags & EXCEPTION_UNWIND;
    int specok = 1; 
    int foreign = 0;

    // Make sure the exception is a Digital Mars C++ Exception, if not
    // Fake everything into thinking it is by setting up eh etc.
    if (eRecord->ExceptionCode != STATUS_SYMANTEC_CPP_EXCEPTION) 
    {
        
        peh = Handle_VC_Exception(eRecord,unwinding);    
        if (!peh ) 
        {
          peh = Handle_Foreign_Exception(unwinding);
          foreign = 1;
        }
    }
   
  
    // Get the exception stack and store into peh for us 
    peh = (peh) ? peh : (Ehstack *)eRecord->ExceptionInformation[0];

    if (!(peh->flags & EHspecok)) 
    {
        peh->flags |= EHspecok; // Ok reset, then call unexpected
        unexpected();
    }
   
    // Check Exception Specification Matching, make sure we do not have an 
    // Unexpected Exception, If unwinding is TRUE, it ignores peh->type and
    // simply returns a pointer past the specification.
    specok = Match_Specification((char *)&funcdata->especification,peh->type,unwinding,&pdata);
    if ( !specok ) 
    {
        _global_unwind((ESTABLISHER_FRAME *)frame,eRecord);
        peh->flags &= ~EHspecok;  // Tag it as bad, *after* global unwind
        unwinding = 1;  /* this forces a local unwind to occur before the unexepected call */
    }
  
    // Skip over handler-table, checking that there are trys.
    int ntrys;
   
    ntrys = PARG(pdata,short);
   
    if (ntrys == 0 && !unwinding)
        goto ret;  // No Trys, so cannot possibly catch anything, 
   
    // Skip Handler Table
    pdata = Skip_Handler(pdata,ntrys);

    if (!unwinding)
        // Since we are not unwinding, search for a Handler for this exception
        Find_CPP_Handler(frame,pdata,peh,funcdata,eRecord);
    else
        // Otherwise, do a local_unwind, calling auto destructors
        __cpp_local_unwind(frame,pdata,-1);
ret:
    /*
       If peh is a foreign exception, it should get removed from the stack
       otherwise it will get full.
     */
    if (!peh || !foreign) 
        goto real_ret;
    assert(peh->flags & EHinuse);  // Cannot destroy twice
    peh->flags &= ~EHinuse;
    peh->pstatic->recent = pop_exception(peh->pstatic,peh); //Get rid of block
real_ret:
   return ExceptionContinueSearch;
}

/**********************************************************************
 *  Walk the Exception Specification pointed at by Specification, looking
 *  for type.
 *  Returns:  0 if there is no Specification matching Type, 
 *      otherwise it return 1.
 *  Arguements:
 *   spec is a pointer to the Exception_Specification Table
 *      type is the exception type that was thrown
 *      unwindflag is a value indicating if an unwind has occured, a non-zero flag
 *        causes this to always return 1.
 *      ppdata upon return will contain the address beyond the Specification Table. 
 *
 *      A non-zero value in unwindflag causes it to always return a non-null ptr.
 */
static int Match_Specification(char *spec,const char *type, int unwindflag,char **ppdata)
{
    short any = 0, match = 0; // Match variables 
    char *pes; // Temp Pointer
  
   
    pes = PARG(spec, char *);  // grab First Specification
    while (pes)
    {   
        any = 1;
   
        if (pes == (char *)(-1L))   // if exception-specification is empty
            break;
   
        if (!match && !unwindflag)  // When unwinding, we do not want to waste time matching
            // if type-id is compatible with thrown object type
            match = __rtti_match(type,pes,NULL);

        pes = PARG(spec,char *);
    }
   
    *ppdata = spec;
    return ((any && !match) && !unwindflag) ?  0 : 1;
}


/*******************************************************************
 *  Given a pointer to the beginning of the handler table, and number
 *  of trys, return pointer past end of Handler Table.
 */
static char *Skip_Handler(char *pdata,short ntrys)
{
    short nhandlers;  // Contains Number of handlers
   
    for (int i = 0; i < ntrys; i++)
    {
        pdata += sizeof(int);   // skip BP offset of catch variable
        nhandlers = PARG(pdata,short);
        pdata += nhandlers * (sizeof(int) + sizeof(void *));
    }
   
    return pdata;
   
} 


/*****************
 *  A non-Digital Mars CPP Exception has occurred.  Simply push an exception 
 * onto the stack that is a non-CPP, return pointer to it, and then let rest of mechanism handle it!
 */
static Ehstack *Handle_Foreign_Exception(int unwindflag)
{
    Ehstack *peh; 
    largeprimtype largeprimnull;
   
    memset (&largeprimnull, 0, sizeof(largeprimnull));
   
    if (!unwindflag) 
    {
        peh = push_exception(0,0,0,largeprimnull);   // Push it onto stack.
        assert(peh);
    }
    else  
    {
#if defined(_MT)
        struct Ehstack *temp = _getthreaddata()->t_cppeh_sv.recent; 
        if (temp != 0)
            peh = temp;
#else
        if (_cppeh_sv.recent != 0)
            peh = _cppeh_sv.recent;  // Grab TopMost exception
#endif
        else 
        {
            // OK, someone intiated an unwind sequence but there
            // was no exception put onto the stack, so return sv.ehstack[0]
            // with initial values to allow the unwind to occur (I.E. destructors
            // to get called
            peh = push_exception(0,0,0,largeprimnull);  
            //      sv.eh_tos--;  // Now pop it off since we are unwinding and do not want it around
        }
    }
      
    return peh;
}

/*********************************************
 * Walk the Table searching for Try..Catch combinations
 *  that will catch exception in peh.
 */
static void Find_CPP_Handler(CPP_ESTABLISHER_FRAME *frame,char *pdata,Ehstack *peh,FUNC_DATA *funcdata,
                             EXCEPTION_RECORD *eRecord)
{
    int index = frame->dataindex;
    long regebp = (long)&frame->ebp;
    ehfp_t fp;
    int prev;
    unsigned tryoff;  // Offset from pdata to Handler
    unsigned cvoffset;  
    short nhandlers;
    unsigned bpoff;
    char *htype;
    unsigned thisoffset;
    unsigned hoffset;
    char *ph;
   
    while (1)
    {   

        if (index == -1)
            break;      // Reached end of table, and found nothing 
            
        ph = pdata + index * (sizeof(int) + 1 + 2 * sizeof(int) + sizeof(void (*)()));
        prev = PARG(ph,int);

        // try-block, see if it matches type
        if (PARG(ph,char) == 1) 
        {
            tryoff = PARG(ph,unsigned);
            ph = (char *)((char *)funcdata + tryoff);   // offset to handler data

            cvoffset = PARG(ph,unsigned);
            nhandlers = PARG(ph,short);

            for (int i = 0; i < nhandlers; i++)   // for each catch block
            {   
                hoffset = PARG(ph,unsigned);
                htype = PARG(ph,char *);
             
                // If thrown type matches handler type
                thisoffset = 0;
                if (__rtti_match(peh->type,htype,&thisoffset)
                    || ((peh->flags & EHisVC) && match_with_vc_throw_type((ThrowInfo *) peh->pVCInfo,htype,&thisoffset)))
                {  
                    // Setup Catch Variable
                    void * __ss *pcv;
                    //for UDT pobj must be changed as this contains the pointer to the actual exception object
                    //(we do not change p as this might be needed to destruct the COMPLETE object)
                    //for pointers pobj points to peh->p which contains the actual pointer
                    //so peh->p must be changed and NOT pobj.
                    //I guess this should be fixed above (for the Dos version) as well
                    if (thisoffset)
                    {
                        if (peh->tsize==0)
                            peh->pobj = (char *)peh->p + thisoffset;
                        else  
                            peh->p = (char *)peh->p + thisoffset;
                    }
                        
                    peh->prev_tsize=peh->tsize;
                    if (peh->tsize == 0)
                        peh->tsize = sizeof(void *);
                      
                    pcv = (void * __ss *)(regebp + cvoffset);
                    *pcv = peh;

                    // Get address of block
                    fp = (ehfp_t)funcdata->funcoffset;
                    *(unsigned *)&fp += hoffset;
                    goto found;
                }
            }
        }
        index = prev;      // previous index
    }
    return;

found: 
    // found a handler, initiate an unwind and then call the handler
    _global_unwind((ESTABLISHER_FRAME *)frame,eRecord);
    // Call all destructors prior to Catch
    __cpp_local_unwind(frame,pdata,index);
    // Call Catch
    _call_catch_block(regebp,funcdata->spoffset,fp);
}


/******************************
 * We have found a handler, so call it.
 */
void _call_catch_block(long catch_ebp, unsigned spoff,ehfp_t blockaddr)
{
    unsigned spvalue;
   
    spvalue = catch_ebp - spoff - sizeof(blockaddr);
    *(ehfp_t *)spvalue = blockaddr;
    __asm  {
      mov   AX,spvalue
      mov   BP,catch_ebp
      mov   SP,AX      // reset stack
      ret         // jump to catch block
    }
}


/***************************************
 * Walk the table, calling auto destructors
 */
void __cpp_local_unwind(CPP_ESTABLISHER_FRAME *frame,char *pdata,int curr_index)
{
    int index = frame->dataindex;
    long regbp = (long)&frame->ebp;
    int prev;
    unsigned bpoff;
    char *ph;
    unsigned spsave;
    void *pthis;
    char ty;
    pdtor_t dtor;
    
    while (index != curr_index) 
    {
        
        if (index == -1)
            break;
            
        ph = pdata + index * (sizeof(int) + 1 + 2 * sizeof(int) + sizeof(void (*)()));
        prev = PARG(ph,int);
        ty = PARG(ph,char);

        switch(ty) 
        {
            case 1: 
                break; // Do not care about calling catch, so ignore it.
            // Call destructors for all active auto objects
            case 2:      // BP offset, 0, dtor
                bpoff = PARG(ph,unsigned);
                (void) PARG(ph,unsigned);
                pthis = (void  *)(bpoff + regbp);
                goto Lcalldtor;
            case 3:      // BP offset of this, offset from this, dtor
                bpoff = PARG(ph,unsigned);
                pthis = (void  *)(bpoff + regbp);
                pthis = (void *)(*(char **)pthis + PARG(ph,unsigned));
                goto Lcalldtor;
            case 4:      // pointer, 0, dtor
                pthis = PARG(ph,void *);
                if (sizeof(void *) == sizeof(int))
                    (void) PARG(ph,void *);
                goto Lcalldtor;

            Lcalldtor:
                dtor = PARG(ph,pdtor_t);
#if defined(_MT)
		_getthreaddata()->t_cppeh_sv.dtor_nest++;
#else
                _cppeh_sv.dtor_nest++;
#endif

                // Note that pdtor type uses a pascal calling sequence,
                // so that the following will work whether the actual
                // function called is __cdecl or __pascal.
                __asm 
                {
                    mov  spsave,ESP
                    mov  ECX,pthis
                    push ECX
                }
                (*dtor)();
                __asm   
                {
                    mov	ESP,spsave
                }
#if defined(_MT)
		_getthreaddata()->t_cppeh_sv.dtor_nest--;
#else
                _cppeh_sv.dtor_nest--;
#endif
                break;
            default:
                terminate();
                // Programmer error, this should never happen
                assert(0);
                break;
        }
        index = prev;
    }
}

/****************************************
 *  This filter will be called upon an unhandled
 * exception along the CPP Mechanism. 
 *  If the exception was a CPP exception, then call terminate,
 * otherwise, if there was a previous filter call it
 */
extern "C" LONG WINAPI __CPPExceptionFilter(struct _EXCEPTION_POINTERS *eptr)
{
    if (eptr->ExceptionRecord->ExceptionCode == STATUS_SYMANTEC_CPP_EXCEPTION) 
    {
        terminate();
        return EXCEPTION_EXECUTE_HANDLER;
    }
    return (_pPreviousUnhandledExceptionFilter) ? (*_pPreviousUnhandledExceptionFilter)(eptr) : EXCEPTION_CONTINUE_SEARCH;
}


#include <setjmp.h>

/******************************************
 * This routine is called from within LongJmp
 * upon it being called when -EH is enabled
 *  It simply performs a local unwind, calling CPP Exceptions
 *    Passed to it is a pointer to a JUMP_BUFFER, see setjmp.h 
 *  for a description of this structure.
 */
extern "C" void __stdcall _cpp_longjmp_unwind(_JUMP_BUFFER *jb)
{   
    FUNC_DATA *funcdata = (FUNC_DATA *)(jb->ExceptData[0]);
    char *pdata;
    int ntrys;

    Match_Specification((char *)&funcdata->especification,NULL,1,&pdata);
    ntrys = PARG(pdata,short);
    pdata = Skip_Handler(pdata,ntrys);
    __cpp_local_unwind((_CPP_Establisher_Frame *)jb->Except_Registration,pdata,jb->TryLevel);
}


/*
 * The following will initialize CPP Exceptions to deal with
 * unhandled exceptions appropriately.
 * The status is as follows:
 *       1: __cpp_init is being called at startup, initialize things
 *      0: __cpp_init is being called at exit, clean up.
 */

extern "C" static void __cpp_init(int status)
{
   static int inited = 0;

   switch (status) 
   {
       case 1:
           /* If not already intialized, do so */
           if (!inited) 
           {
               _pPreviousUnhandledExceptionFilter = SetUnhandledExceptionFilter(_pCPPExceptionFilter);
               inited = 1;
           }
           break;
       case 0:
           /* only reset, if we had initialized */
           if (inited) 
           {
               SetUnhandledExceptionFilter(_pPreviousUnhandledExceptionFilter);
               inited = 0;
           }
           break;
       default:
           /* should never happen, but hey, you never know. */
           assert(0);
           break;
    }
}

extern "C" void (*__cpp_init_ptr)(int) = __cpp_init;

//VCX: Exception Handling Functions

Ehstack* Handle_VC_Exception(EXCEPTION_RECORD* eRecord,int unwindflag)
{
    struct Ehstack *peh;
    ThrowInfo *pTI;
    CatchableTypeArray *pCTA;
    CatchableType *pCT;
    int size;
    unsigned spsave;

#if defined(_MT)
    struct Ehstack *temp = _getthreaddata()->t_cppeh_sv.recent;
    if (unwindflag && temp != 0)
        return temp;    // Grab Topmost
#else
    if (unwindflag && _cppeh_sv.recent!=0) 
        return _cppeh_sv.recent; // Grab Topmost
#endif

    // test if it really is a VC++ exception
    if ( (eRecord->ExceptionCode != STATUS_VC_CPP_EXCEPTION) 
       || (eRecord->ExceptionInformation[0] !=VC_CPP_MAGIC_NUMBER))
    {
        return 0;
    }

    void *pExceptionObject=(void*)eRecord->ExceptionInformation[1];
    union 
    {
        largeprimtype eobj;
        void *eptr;
    };
  
    pTI=(ThrowInfo*)eRecord->ExceptionInformation[2];
    // get the first catchable type (this is the actual throw type)
    pCT=*pTI->pCatchableTypeArray->arrayOfCatchableTypes;
    size=pCT->sizeOrOffset;
    if (pCT->properties & 0x1) 
    { 
        // simple type or pointer, so push the object on Ehstack and set size accordingly
        memcpy(&eobj,pExceptionObject,size);
        peh=push_exception(0,(ehfp_t)pTI->pmfnUnwind,size,eobj);
    } 
    else 
    {
        // its a UDT
        // microsoft allocates the object on a positive offset to EBP
        // as they actually call the catch block, this works
        // Digital Mars returns to the catch block, which will result
        // in overwriting the exception object.
        // therefore allocate new storage with eh_new() and do a shallow copy
        eptr=__eh_new(size);
        memcpy(eptr,pExceptionObject,size);

        // pass size zero, so storage allocated by eh_new will be freed
        peh=push_exception(0,(ehfp_t)pTI->pmfnUnwind,0,eobj);
    }
    peh->flags|=EHisVC; // let us known that this is a VC excepton
    peh->pVCInfo=pTI;   // store a pointer to throwinfo
    return peh;
}

// copied over from rtti.cpp
extern "C" char __cdecl __tiX[];
static char *__get_name(const char *cl)
{
    char *p;
    int nbases;
    
    p = (char *)cl;
    switch (*p)
    {
        case 0:
            p++;
            break;
        case 1:
            p+= 2 + sizeof(p);
            break;
        case 2:
            p++;
            nbases = *(unsigned short *)p;
            p += 2 + nbases * (sizeof(int) + sizeof(p));
            break;
        default:
            return NULL;
    }
    return p;
}

extern "C"  char *__unmangle_vc_exception_type(const char *ps);

//match type names
//cl1 is a VC mangled type
//cl2 is a SC pretty type
static int __vc_match_name(const char *cl1, const char *cl2)
{
    int retval=1;
    char *name1, *name2;
    
    // unmangle the vc mangled type
    // we do not have to worry about const/volatile modifiers
    // as vc doesn't include this in the mangling of exception types
    name1 = __unmangle_vc_exception_type(cl1);
    name2 = __get_name(cl2);

    if (!name1 || !name2 || strcmp(name1,name2)) 
        retval=0;
        
    free(name1);
    return retval;
}

int match_with_vc_throw_type(ThrowInfo* pTI,char *thandler,unsigned *poffset)
{
    CatchableTypeArray *pCTA;
    CatchableType *pCT;
    int nthrowtypes;
    char *tthrow;
    int handler_is_ptr=0;
    int i;
  
    if (thandler == __tiX) 
        goto match;

    if (*thandler==1) 
    {    
        // pointer type
        // if throw has const or volatile attribute, handler
        // should have it as well
        if ((pTI->attributes & 0x1)  && !(thandler[1] & 0xc0)) 
            goto nomatch;
        if ((pTI->attributes & 0x2)  && !(thandler[1] & 0x40)) 
            goto nomatch;
        //advance to the type we are pointing to
        thandler=*(char**)(thandler+2); 
        handler_is_ptr=1;
    }
    pCTA=pTI->pCatchableTypeArray;
    nthrowtypes=pCTA->nCatchableTypes;
    
    for (i=0;i<nthrowtypes;i++) 
    {
        pCT=pCTA->arrayOfCatchableTypes[i];
        tthrow=pCT->pType->name+1;
        if (handler_is_ptr) 
        {
            if(*tthrow=='P') 
            { 
                //both should be pointer
                if (thandler == __tiX) // void * matches any pointer
                    goto match;
                tthrow+=2; //advance to the type we are pointing to by changing the mangling
				//skip over pointer and modifier mangling (PA)
            } 
            else 
                goto nomatch;
        } 
        if (__vc_match_name(tthrow,thandler)) 
        {
            //NOTE: only the normal offset is returned, I did not account for virtual bases
            //as the binary layout of class with virtual bases isn't compatible with MS anyway
            *poffset=pCT->thisDisplacement.mdisp;
            goto match;
        }
    }
nomatch:
    return 0;
match:
    return 1;
}


#endif

/* ===================== C++ 98 Support ======================== */

#if _WIN32

namespace std
{

    // C++98 18.6.2.3
    unexpected_handler __cdecl set_unexpected(unexpected_handler f) throw()
    {
	return ::set_unexpected(f);
    }

    // C++98 18.6.2.4
    void __cdecl unexpected()
    {
	::unexpected();
    }

    // C++98 18.6.3.2
    terminate_handler __cdecl set_terminate(terminate_handler f) throw()
    {
	return ::set_terminate(f);
    }

    // C++98 18.6.3.3
    void __cdecl terminate()
    {
	::terminate();
    }

    // C++98 18.6.4
    bool __cdecl uncaught_exception()
    {
	// Return true if throwing an exception would wind up calling terminate()
	return false;	// BUG: do better than this
    }


    exception::~exception() throw() { }
    const char* exception::what() const throw() { return "class exception"; }

    bad_exception::~bad_exception() throw() { }
    const char* bad_exception::what() const throw() { return "class bad_exception"; }
}

#endif
