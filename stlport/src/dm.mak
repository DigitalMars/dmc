#	-*- Makefile -*-
.SUFFIXES: .cpp .c .obj .dll .exe .rc .res


CXX=dmc
CC=dmc -cpp
LIB=lib
LINK=dmc
RC=rcc

LIB_BASENAME=stlp45dm

OBJS = build\c_locale.obj build\c_locale_stub.obj build\codecvt.obj \
	build\collate.obj build\complex.obj build\complex_exp.obj \
	build\complex_io.obj build\complex_io_w.obj build\complex_trig.obj \
	build\ctype.obj build\dll_main.obj \
	build\facets_byname.obj build\fstream.obj build\ios.obj \
	build\iostream.obj build\istream.obj build\locale.obj \
	build\locale_catalog.obj build\locale_impl.obj \
	build\messages.obj build\monetary.obj build\num_get.obj \
	build\num_get_float.obj build\num_put.obj \
	build\num_put_float.obj build\numpunct.obj build\ostream.obj \
	build\sstream.obj build\stdio_streambuf.obj \
	build\streambuf.obj build\string_w.obj build\strstream.obj \
	build\time_facets.obj

BUILD_DIRS=..\lib ..\build \
	..\build\static ..\build\static\release \
	..\build\static\debug ..\build\static\stldebug \
	..\build\staticx ..\build\staticx\release \
	..\build\staticx\debug ..\build\staticx\stldebug \
	..\build\dynamic ..\build\dynamic\release \
	..\build\dynamic\debug ..\build\dynamic\stldebug \
	..\build\sdynamic ..\build\sdynamic\release \
	..\build\sdynamic\debug ..\build\sdynamic\stldebug


CXXFLAGS_COMMON = -Ae -Ar -DSTRICT -D__BUILDING_STLPORT -I../stlport
CXXFLAGS_dynamic = -WD

# four versions are currently supported:
#  - static: static STLport library, static RTL
#  - staticx: static STLport library, dynamic RTL
#  - dynamic: dynamic STLport library, dynamic RTL
#  - sdynamic: dynamic STLport library, static RTL

CXXFLAGS_RELEASE_static = $(CXXFLAGS_COMMON) -o+all -Nc -D_MT
CXXFLAGS_RELEASE_staticx = $(CXXFLAGS_COMMON) -o+all -Nc -ND
CXXFLAGS_RELEASE_dynamic = $(CXXFLAGS_COMMON) $(CXXFLAGS_dynamic) -o+all -ND
CXXFLAGS_RELEASE_sdynamic = $(CXXFLAGS_COMMON) $(CXXFLAGS_dynamic) -D_MT -o+all

CXXFLAGS_DEBUG_static = $(CXXFLAGS_COMMON) -g -Nc -D_MT
CXXFLAGS_DEBUG_staticx = $(CXXFLAGS_COMMON) -g -Nc -ND
CXXFLAGS_DEBUG_dynamic = $(CXXFLAGS_COMMON) $(CXXFLAGS_dynamic) -g -ND
CXXFLAGS_DEBUG_sdynamic = $(CXXFLAGS_COMMON) $(CXXFLAGS_dynamic) -g -D_MT

CXXFLAGS_STLDEBUG_static = $(CXXFLAGS_DEBUG_static) -D_STLP_DEBUG -g -Nc -D_MT
CXXFLAGS_STLDEBUG_staticx = $(CXXFLAGS_DEBUG_static) -D_STLP_DEBUG -g -Nc -ND
CXXFLAGS_STLDEBUG_dynamic = $(CXXFLAGS_DEBUG_dynamic) $(CXXFLAGS_dynamic) -D_STLP_DEBUG -g -ND
CXXFLAGS_STLDEBUG_sdynamic = $(CXXFLAGS_DEBUG_dynamic) $(CXXFLAGS_dynamic) -D_STLP_DEBUG -g -D_MT


.cpp{..\build\static\release}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_RELEASE_static) "$<"

.cpp{..\build\static\debug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_DEBUG_static) "$<"

.cpp{..\build\static\stldebug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_STLDEBUG_static) "$<"

.cpp{..\build\staticx\release}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_RELEASE_staticx) "$<"

.cpp{..\build\staticx\debug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_DEBUG_staticx) "$<"

.cpp{..\build\staticx\stldebug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_STLDEBUG_staticx) "$<"

.cpp{..\build\dynamic\release}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_RELEASE_dynamic) "$<"

.cpp{..\build\dynamic\debug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_DEBUG_dynamic) "$<"

.cpp{..\build\dynamic\stldebug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_STLDEBUG_dynamic) "$<"

.cpp{..\build\sdynamic\release}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_RELEASE_sdynamic) "$<"

.cpp{..\build\sdynamic\debug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_DEBUG_sdynamic) "$<"

.cpp{..\build\sdynamic\stldebug}.obj:
	$(CXX) -c -o"$@" $(CXXFLAGS_STLDEBUG_sdynamic) "$<"

.c{..\build\static\release}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_RELEASE_static) "$<"

.c{..\build\static\debug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_DEBUG_static) "$<"

.c{..\build\static\stldebug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_STLDEBUG_static) "$<"

.c{..\build\staticx\release}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_RELEASE_staticx) "$<"

.c{..\build\staticx\debug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_DEBUG_staticx) "$<"

.c{..\build\staticx\stldebug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_STLDEBUG_staticx) "$<"

.c{..\build\dynamic\release}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_RELEASE_dynamic) "$<"

.c{..\build\dynamic\debug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_DEBUG_dynamic) "$<"

.c{..\build\dynamic\stldebug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_STLDEBUG_dynamic) "$<"

.c{..\build\sdynamic\release}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_RELEASE_sdynamic) "$<"

.c{..\build\sdynamic\debug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_DEBUG_sdynamic) "$<"

.c{..\build\sdynamic\stldebug}.obj:
	$(CC) -c -o"$@" $(CXXFLAGS_STLDEBUG_sdynamic) "$<"


.rc{..\build}.res:
	$(RC) -32 -o"$@" "$<"


all:	directories all_static all_staticx all_dynamic all_sdynamic


directories:	$(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir $@


all_static:	..\lib\$(LIB_BASENAME)_static.lib ..\lib\$(LIB_BASENAME)_debug_static.lib ..\lib\$(LIB_BASENAME)_stldebug_static.lib

..\lib\$(LIB_BASENAME)_static.lib:	$(OBJS:build\=..\build\static\release\)
	*$(LIB) -c -n -p256 "$@" "$**"

..\lib\$(LIB_BASENAME)_debug_static.lib:	$(OBJS:build\=..\build\static\debug\)
	*$(LIB) -c -n -p256 "$@" "$**"

..\lib\$(LIB_BASENAME)_stldebug_static.lib:	$(OBJS:build\=..\build\static\stldebug\)
	*$(LIB) -c -n -p256 "$@" "$**"


all_staticx:	..\lib\$(LIB_BASENAME)_staticx.lib ..\lib\$(LIB_BASENAME)_debug_staticx.lib ..\lib\$(LIB_BASENAME)_stldebug_staticx.lib

..\lib\$(LIB_BASENAME)_staticx.lib:	$(OBJS:build\=..\build\staticx\release\)
	*$(LIB) -c -n -p256 "$@" "$**"

..\lib\$(LIB_BASENAME)_debug_staticx.lib:	$(OBJS:build\=..\build\staticx\debug\)
	*$(LIB) -c -n -p256 "$@" "$**"

..\lib\$(LIB_BASENAME)_stldebug_staticx.lib:	$(OBJS:build\=..\build\staticx\stldebug\)
	*$(LIB) -c -n -p256 "$@" "$**"


all_dynamic:	..\lib\$(LIB_BASENAME).dll ..\lib\$(LIB_BASENAME)_debug.dll ..\lib\$(LIB_BASENAME)_stldebug.dll

..\lib\$(LIB_BASENAME).dll:	$(OBJS:build\=..\build\dynamic\release\) ..\build\stlport.res
	*$(LINK) -WD -ND -L/IMPLIB:$(@R).lib -L/NOMAP -L/NODEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<

..\lib\$(LIB_BASENAME)_debug.dll:	$(OBJS:build\=..\build\dynamic\debug\) ..\build\stlport.res
	*$(LINK) -WD -ND -g -L/IMPLIB:$(@R).lib -L/NOMAP -L/DEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<

..\lib\$(LIB_BASENAME)_stldebug.dll:	$(OBJS:build\=..\build\dynamic\stldebug\) ..\build\stlport.res
	*$(LINK) -WD -ND -g -L/IMPLIB:$(@R).lib -L/NOMAP -L/DEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<


all_sdynamic:	..\lib\$(LIB_BASENAME)s.dll ..\lib\$(LIB_BASENAME)s_debug.dll ..\lib\$(LIB_BASENAME)s_stldebug.dll

..\lib\$(LIB_BASENAME)s.dll:	$(OBJS:build\=..\build\sdynamic\release\) ..\build\stlport.res
	*$(LINK) -WD -ND -L/IMPLIB:$(@R).lib -L/NOMAP -L/NODEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<

..\lib\$(LIB_BASENAME)s_debug.dll:	$(OBJS:build\=..\build\sdynamic\debug\) ..\build\stlport.res
	*$(LINK) -WD -ND -g -L/IMPLIB:$(@R).lib -L/NOMAP -L/DEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<

..\lib\$(LIB_BASENAME)s_stldebug.dll:	$(OBJS:build\=..\build\sdynamic\stldebug\) ..\build\stlport.res
	*$(LINK) -WD -ND -g -L/IMPLIB:$(@R).lib -L/NOMAP -L/DEBUG -o"$@" "$**" user32.lib kernel32.lib <<$(@R).def
LIBRARY "$(@F)"
DESCRIPTION 'STLport DLL for Digital Mars C/C++'
EXETYPE NT
SUBSYSTEM WINDOWS
CODE SHARED EXECUTE
DATA READWRITE

EXPORTS
	"?cin@std@@3V?$basic_istream@std@DV?$char_traits@std@D@1@@1@A"
	"?cout@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?cerr@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?clog@std@@3V?$basic_ostream@std@DV?$char_traits@std@D@1@@1@A"
	"?wcin@std@@3V?$basic_istream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcout@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wcerr@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
	"?wclog@std@@3V?$basic_ostream@std@_YV?$char_traits@std@_Y@1@@1@A"
<<
