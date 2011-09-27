if not exist ..\bin\nul goto MustBeTwoDirsDown
copy ..\bin\*.lib ..\lib
del ..\bin\*.lib
goto End
:MustBeTwoDirsDown
copy ..\..\bin\*.lib ..\..\lib
del ..\..\bin\*.lib
:End
