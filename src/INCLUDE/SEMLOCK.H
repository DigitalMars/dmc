/*****************************************************************************
Programmer: Glenn Anderson
Project:    C++ Runtime Library, Win32

This is the interface for the 32 bit serialization code.
*****************************************************************************/

#ifndef _SEMLOCK_H
#define _SEMLOCK_H       1

#if __cplusplus
extern "C" {
#endif

#include <stdio.h>   /* For _NFILE */

/*==========================================================================*/
/* These defines enumerate the posible semaphores                           */
/*==========================================================================*/
#define _semDestroySem                 0
#define _semAlloc                      1
#define _semEnvironment                2
#define _semIO                         3
#define _semFpBase                     10
#define _semTop                        (_semFpBase + _NFILE)
#define _MAX_SEMAPHORES                _semTop

#if defined(__NT__) && defined(_MT)

/*==========================================================================*/
/* These prototypes define the functions that are called to do the semaphore*/
/* work                                                                     */
/*==========================================================================*/
void _InitSemaphoreSys();
void _WaitSemaphore(int iSemaphore);
void _ReleaseSemaphore(int iSemaphore);
void _DestroySemaphore(int iSemaphore);
void _DestroyAllSemaphores();
void _LockSemaphore(int iSemaphore);
void _UnlockSemaphore(int iSemaphore);

/*==========================================================================*/
/* This is the global data associated with the semaphore system             */
/*==========================================================================*/
extern short int _iSemLockCtrs[_MAX_SEMAPHORES];
extern int       _iSemThreadIds[_MAX_SEMAPHORES];
extern int       _iSemNestCount[_MAX_SEMAPHORES];

/*==========================================================================*/
/* Use these macros to access the semaphore functionality                   */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
/* LockSemaphore will wait if the semaphore has already been locked,        */
/* otherwise it will exit immediately                                       */

#define LockSemaphore(Number) 													\
lsLoop:																					\
	_asm	{mov	edx, Number}														\
	_asm	{lock inc	_iSemLockCtrs[edx * 2]}											\
	_asm	{jz	lsDone}																\
	_asm	{push	edx}																	\
	_asm	{call	_WaitSemaphore}													\
	_asm	{add	esp, 4}																\
lsDone:	;

/*--------------------------------------------------------------------------*/
/* LockSemaphoreNested will wait if the semaphore has already been locked,  */
/* unless it was locked by this thread                                      */

#define LockSemaphoreNested(Number) 											\
lsLoop:																					\
	_asm	{call	GetCurrentThreadId}												\
	_asm	{mov	edx, Number}														\
	_asm	{cmp	_iSemThreadIds[edx * 4], eax}									\
	_asm	{jne	lsTryLock}															\
	_asm	{lock inc	_iSemNestCount[edx * 4]}										\
	_asm	{jmp	lsDone}																\
																							\
lsTryLock:																				\
	_asm	{lock inc	_iSemLockCtrs[edx * 2]}											\
	_asm	{jnz	lsWait}																\
	_asm	{mov	_iSemThreadIds[edx * 4], eax}									\
	_asm	{lock inc	_iSemNestCount[edx * 4]}										\
	_asm	{jmp	lsDone}																\
																							\
lsWait:																					\
	_asm	{push	edx}																	\
	_asm	{call	_WaitSemaphore}													\
	_asm	{add	esp, 4}																\
	_asm	{call	GetCurrentThreadId}												\
	_asm	{mov	edx, Number}														\
	_asm	{mov	_iSemThreadIds[edx * 4], eax}									\
	_asm	{lock inc	_iSemNestCount[edx * 4]}										\
lsDone:	;

/*--------------------------------------------------------------------------*/
/* UnlockSemaphore will release the lock on the semaphore, and allow another*/
/* lock to take place                                                       */

#define UnlockSemaphore(Number)													\
	_asm	{mov	edx, Number}														\
	_asm	{lock dec	_iSemLockCtrs[edx * 2]}											\
	_asm	{js	usDone}																\
	_asm	{push	edx}																	\
	_asm	{call	_ReleaseSemaphore}												\
	_asm	{add	esp, 4}																\
usDone:	;

/*--------------------------------------------------------------------------*/
/* UnlockSemaphoreNested will release the lock on the semaphore if the nest */
/* count is zero, and allow another lock to take place.  It will delete the */
/* semaphore if after releasing it, if Delete is TRUE                       */

#define UnlockSemaphoreNested(Number, Delete)								\
	_asm	{mov	edx, Number}														\
	_asm	{lock dec	_iSemNestCount[edx * 4]}										\
	_asm	{jnz	usDone}																\
	_asm	{mov	_iSemThreadIds[edx * 4], 0}									\
	_asm	{lock dec	_iSemLockCtrs[edx * 2]}											\
	_asm	{js	usDone}																\
	_asm	{push	edx}																	\
	_asm	{call	_ReleaseSemaphore}												\
	_asm	{add	esp, 4}																\
	_asm	{mov	edx, Delete}														\
	_asm	{test	edx, edx}															\
	_asm	{jz	usDone}																\
	_asm	{mov	edx, Number}														\
	_asm	{push	edx}																	\
	_asm	{call _DestroySemaphore}												\
	_asm	{add	esp, 4}																\
usDone:	;

#else

/*--------------------------------------------------------------------------*/
/* These defines are used when not compiled for NT.  Note: If OS/2 is ever  */
/* supported this code will need to be rewritten to support it.  The old    */
/* OS/2 support has not been carried over to this new code.                 */

#define LockSemaphore(Number)
#define LockSemaphoreNested(Number)
#define UnlockSemaphore(Number)
#define UnlockSemaphoreNested(Number)

#endif

#if __cplusplus
}
#endif

#endif
