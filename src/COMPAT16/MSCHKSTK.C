/* Copyright (C) 1995 by Digital Mars. */


void _chkstk();

/*
 *  _aFchkstk is an emulation of the Microsoft
 *  __far intrinsic to do stack checking.
 *  Our internal _chkstk does the same work as Microsoft's
 *  and in fact, the size is passed in AX for us as it is
 *  for Microsoft, so it simply jmps to our own __chkstk.
 *  However, in near code models, this will not work and in
 *  in this case this routine is a stub and does no real checking.
 *  instead, it simply sets up SP based on what is in AX
 *  and returns as that is what MSOFT Objs expect to 
 *  at least occur.
 */

__declspec(naked) _aFchkstk()
{
    __asm 
    {
#if !(__LARGE__ || __MEDIUM__)
        pop CX
        pop DX
        mov BX,SP
        sub BX,AX
        mov SP,BX
        push DX
        push CX
        retf
#else
        jmp _chkstk
#endif
    }
}

/*
 *  _aNchkstk is an emulation of the Microsoft
 *  __near intrinsic to do stack checking.
 *  Our internal _chkstk does the same work as Microsoft's
 *  and in fact, the size is passed in AX for us as it is
 *  for Microsoft, so it simply jmps to our own __chkstk.
 *  However, in large code models, this will not work and in
 *  in this case this routine is a stub and does no real checking.
 *  instead, it simply sets up SP based on what is in AX
 *  and returns as that is what MSOFT Objs expect to 
 *  at least occur.
 */

__declspec(naked) _aNchkstk()
{
    __asm 
    {
#if !(__SMALL__ || __COMPACT__)
        pop CX
        mov BX,SP
        sub BX,AX
        mov SP,BX
        jmp CX
#else
        jmp _chkstk
#endif
    }

}
