/*      exitstat.c
 *     pushs and pops exit frame, so exit & atexit return points can
 *     be controlled. Useful in Windows and for turning a standalone program
 *     into a subroutine. A maximum of 16 states can be saved.
 *
 *     (C) Copyright 1991-1993 Digital Mars
 *     All Rights Reserved
 *     Written by: G. Eric Engstrom
 *      $Revision: 1.1.1.1 $
 */

#include       <setjmp.h>
#include       <exitstat.h>
#include       <string.h>
#include       <stdlib.h>
#include       <scrtl.h>

jmp_buf   _exit_state;

typedef struct EXITSTATT
  {
     jmp_buf _exit_state;
     void (_32BIT * *_atexitp)(void);
     void (_32BIT **_atexit_tbl)(void);
  }
ExitStateT;

static void exit_exit(int errstatus);

static ExitStateT   _state_tbl[16+1];
static ExitStateT  *_exitstatep = NULL;
static    jmp_buf   _exit_state1;

void (*_hookexitp)(int errstatus);

/* exit_PUSHSTATE
 *
 * returns 0            if successful
 * returns exit code + 1 if unsuccessful or user exited from with in this frame
 */
int exit_PUSHSTATE(void)
{
size_t   tbl_size;

    if (_exitstatep == NULL)           /* if first call */
      {
      _exitstatep = _state_tbl;                /* initialize to start of table */
      }
    if (_exitstatep >= _state_tbl + 16)        /* if table is full     */
      {
      return 2;                                /* fail                 */
      }
    tbl_size = _atexit_max * sizeof (void (_32BIT **)(void));
    _exitstatep->_atexit_tbl = (void (_32BIT **)(void))malloc (tbl_size);
    if (_exitstatep->_atexit_tbl == NULL)
      return 2;
    memcpy(&(_exitstatep->_atexit_tbl[0]),&_atexit_tbl[0],tbl_size);   /* save atexit function list */
    memcpy(_exitstatep->_exit_state,_exit_state,sizeof(_exit_state));
    _exitstatep->_atexitp = _atexitp;
    _atexitp = NULL;

    ++_exitstatep;

    _hookexitp = exit_exit;
    return 0;                          /* current exit state           */
}

/* exit_popstate
 *
 * restores the previous return state.
 * should be called by user only when exit_pushstate returns 0
 *
 * returns 0   if successful pop
 * returns 1   if no state to restore
 */
int exit_popstate(void)
{
size_t   tbl_size;

    if (_exitstatep == NULL
      || _exitstatep == _state_tbl)
      {
      return 1;                                /* no states to restore */
      }
    --_exitstatep;
    
    tbl_size = _atexit_max * sizeof (void (_32BIT **)(void));
    memcpy(&_atexit_tbl[0],&(_exitstatep->_atexit_tbl[0]),tbl_size);
    free (_exitstatep->_atexit_tbl);
    memcpy(_exit_state,_exitstatep->_exit_state,sizeof(_exit_state));
    _atexitp = _exitstatep->_atexitp;

    if (_exitstatep == _state_tbl)
      {
      _hookexitp = (void (*)(int))NULL;
      }
    return 0;                          /* state restore succesfully */
}

static void exit_exit(int errstatus)
  {
    longjmp(_exit_state,errstatus+1);
  }
