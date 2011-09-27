
# This script builds the test programs for the STLSoft libraries that are bundled with Digital Mars
# Use Digital Mars' shell.exe to execute

@echo "Building STLSoft libraries test suite for Digital Mars"

make -fmakefile target

@echo "Cleaning out the temporaries (.obj, .exe, .map)"

make -fmakefile clean

