#if __SC__ || __RCC__
#pragma once
#endif

#ifndef _H_RTLHEAP
#define _H_RTLHEAP

#include <stdlib.h>

#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#if __cplusplus
extern "C"
{
#endif
typedef struct {char unused;} *HRTLPOOL;

// Create a pool of fixed sized objects
HRTLPOOL _CRTIMP RTLPoolCreate(size_t);

// Allocate an item from the pool
void * _CRTIMP RTLPoolAlloc(HRTLPOOL hPool);

// Free an item from the pool
void _CRTIMP RTLPoolFree(HRTLPOOL pPool, void *pData);

// Destroy the pool
void _CRTIMP RTLPoolDestroy(HRTLPOOL pPool);

#if __cplusplus
};
#endif

#if __cplusplus

// Macros to implement class-specific new/delete using a fixed size pool.
//
// In the header:
//     class CDataRecord
//         {
//         RTL_DECLARE_POOL()
//     public:
//         };
//
// In the source:
//     RTL_IMPLEMENT_POOL(CDataRecord)
//

class RTLPoolWrapper
    {
public:
    RTLPoolWrapper(size_t size) : m_size(size), hPool(RTLPoolCreate(size)) {}
    ~RTLPoolWrapper() {RTLPoolDestroy(hPool);}
    void *Alloc(size_t size) {return size==m_size ? RTLPoolAlloc(hPool) : malloc(size);}
    void Free(void *p, size_t size) {if (size==m_size) RTLPoolFree(hPool, p); else free(p);}
private:
    size_t m_size;
    HRTLPOOL hPool;
    };

#define RTL_DECLARE_POOL() \
public:             \
    void *operator new(size_t );    \
    void operator delete(void* , size_t ); \
private:            \
    static RTLPoolWrapper m_RTLPool;

#define RTL_IMPLEMENT_POOL(CLASS) \
    RTLPoolWrapper CLASS::m_RTLPool(sizeof(CLASS)); \
    void *CLASS::operator new(size_t n) {return m_RTLPool.Alloc(n);} \
    void CLASS::operator delete(void* p, size_t n) {m_RTLPool.Free(p,n);}

#endif

#endif
