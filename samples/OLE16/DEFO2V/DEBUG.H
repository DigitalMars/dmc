/*
 *  DEBUG code - Contains declarations and macros for include debug support;
 *       Contains null definitions when !_DEBUG
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef _DEBUG
#undef ASSERTDATA
#ifdef _MAC
#ifdef __cplusplus
extern "C" {
#endif
void OutputDebugString(const char *);
#ifdef __cplusplus
}
#endif
#endif

#define ASSERTDATA  static char  _szAssertFile[]= __FILE__;

#undef Assert
// MAC compiler barfs on '(void)0'.
#ifdef _MAC
#define Assert(a) { if (!(a)) FnAssert(#a, NULL, _szAssertFile, __LINE__); }
#undef AssertSz
#define AssertSz(a, b) { if (!(a)) FnAssert(#a, b, _szAssertFile, __LINE__); }
#else
#define Assert(a) ((a) ? NOERROR : FnAssert(#a, NULL, _szAssertFile, __LINE__))
#undef AssertSz
#define AssertSz(a,b) ((a) ? NOERROR : FnAssert(#a, b, _szAssertFile, __LINE__))
#endif
#undef Verify
#define Verify(a)   Assert(a)
#undef Puts
#define Puts(s) OutputDebugString(s)

#define ASSERT(cond, msg)

#else   //  !_DEBUG

#define ASSERTDATA
#define Assert(a) ((void)0)
#define AssertSz(a, b) ((void)0)
#define Verify(a) (a)
#define ASSERT(cond, msg)
#define Puts(s) ((void)0)

#endif  //  _DEBUG

#endif // _DEBUG_H_
