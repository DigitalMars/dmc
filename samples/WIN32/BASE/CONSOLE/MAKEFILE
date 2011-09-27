!include <ntwin32.mak>

all: console.exe

console.exe: console.obj alocfree.obj create.obj fillatt.obj fillchar.obj \
    flush.obj cursor.obj conmode.obj coninfo.obj contitle.obj \
    getnumev.obj getlrgst.obj numbut.obj readout.obj readchar.obj \
    scroll.obj size.obj handler.obj writein.obj codepage.obj console.res
  $(link) $(ldebug) $(conlflags) -out:$*.exe $** $(conlibs) \
    user32.lib advapi32.lib

.c.obj:
  $(cc) $(cdebug) $(cflags) $(cvars) $*.c

console.res: console.rc console.ico
    $(rc) $(rcvars) -r -fo $*.res $*.rc
