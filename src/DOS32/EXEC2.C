/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 */

#include <stdlib.h>
#include <scrtl.h>

#pragma SC align 1

int _exec (char *cmd, char *args, char *envblock, int chain)
{
  struct
  {
    char *envoff;
    short envseg;
    char *argoff;
    short argseg;
  } parmblock;
  int error, errcode;

/*
 * Free conventional memory for child process to use.
 */
#if 0
  error = 0;
  _EBX = 1;      /* minimize usage of conventional memory */
  _ECX = 0;      /* reset memory to zero pages above minimum */ 
  _EAX = 0x2536; /* Minimize/Maximize Extended/Conventional Memory code */
  asm
  {
  	int	21h
  	adc	error,0
  	mov	errcode,eax
  }
  if (error)
    goto return_error;
#endif
/*
 * Execute process.
 */
  error = 0;
  parmblock.envoff = envblock;
  parmblock.argoff = args;
  _EBX = (unsigned)&parmblock;
  _EDX = (unsigned)cmd;
  _AL = 0;
  _AH = 0x4b;    /* Execute process code */
  __asm
  {
  	mov	parmblock.envseg,ds
  	mov	parmblock.argseg,ds
  	push    ebp ; DOSX bug destroys EBP
  	int	21h
  	pop     ebp
  	adc	error,0
  	mov	errcode,eax
  }
/*
 * Take memory back whatever the outcome.
 */
#if 0
  _EBX = 3;      /* maximize usage of conventional memory */
  _ECX = 0;      /* reset memory to zero pages below maximum */
  _EAX = 0x2536; /* Minimize/Maximize Extended/Conventional Memory code */
  asm int 21h
#endif
/*
 * Report if there was an error executing the process.
 */
  if (error)
    goto return_error;
/*
 * If an exec-style call, exit.
 */
  if (chain)
    exit (0);
/*
 * Otherwise, get and return the process' return code.
 */
  _AH = 0x4d;    /* get return value code */
  asm int 21h
  return (int)_AL;
return_error:
  __set_dos_errno (errcode);
  return -1;
}
