
#*************************************************************#
#**                                                         **#
#**                 Microsoft RPC Examples                  **#
#**                 OSF DCE Interop Application             **#
#**           Copyright(c) Microsoft Corp. 1993-1995        **#
#**                                                         **#
#*************************************************************#

!include <ntwin32.mak>

!if "$(CPU)" == "i386"
cflags = $(cflags:G3=Gz)
cflags = $(cflags:Zi=Z7)
!endif


all : client.exe server.exe

# Make the client
client : client.exe
client.exe : client.obj msg_c.obj midluser.obj
    $(link) $(linkdebug) $(conflags) -out:client.exe -map:client.map \
      client.obj msg_c.obj midluser.obj \
      rpcrt4.lib $(conlibs)

# client main program
client.obj : client.c msg.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c

# client stub
msg_c.obj : msg_c.c msg.h
   $(cc) $(cdebug) $(cflags) $(cvars) $*.c


# Make the server executable
server : server.exe
server.exe : server.obj manager.obj msg_s.obj midluser.obj
    $(link) $(linkdebug) $(conflags) -out:server.exe -map:server.map \
      server.obj manager.obj msg_s.obj midluser.obj \
      rpcrt4.lib $(conlibsmt)

# server main program
server.obj : msg.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# remote procedures
manager.obj : msg.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# server stub
msg_s.obj : msg_s.c msg.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c

# midl_user* routines
midluser.obj : midluser.c
   $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.c


# Stubs and header file from the IDL file
msg.h msg_s.c msg_c.c : msg.idl msg.acf
    midl -cpp_cmd $(cc) -cpp_opt "-E" msg.idl

# Clean up everything
cleanall : clean
    -del *.exe

# Clean up everything but the .EXEs
clean :
    -del *.obj
    -del *.map
    -del msg_?.c
    -del msg.h
