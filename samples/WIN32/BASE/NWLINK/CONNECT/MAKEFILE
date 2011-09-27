!include <ntwin32.MAK>

proj = connect
mylibs = wsock32.lib ..\testlib\testlib.lib

all: $(proj).exe

# Update the object files if necessary
$(proj).obj: $(proj).c ..\testlib\testlib.lib
    $(cc) $(cflags) $(cvarsmt) $(cdebug) $(proj).c

# Create library if necessary.
..\testlib\testlib.lib :
  cd ..\testlib
  $(MAKE) /A
  cd ..\connect

# Since the link line has some severe differences depending on what
# platform we are running on, we need to special case this so that
# we execute the correct commands:

$(proj).exe: $(proj).obj
    $(link) $(linkdebug) $(conlflags) $(proj).obj  $(conlibs) $(mylibs) -out:$(proj).exe


# Clean up everything
clean :
    -del *.exe
    -del *.obj
