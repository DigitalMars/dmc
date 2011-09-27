 Sample: Creating Resource-Only DLLs

Summary:

The RESDLL sample shows how to create a resource-only
dynamic link library (DLL). In short, this is accomplished
by creating and resource-compiling a resource (.RC) file,
and then linking it correctly.

The MAIN.EXE program tests THE_DLL.DLL by loading it and
referencing the DLL's icon, cursor, and bitmap. The icon
and cursor are used by the registered window class, and
the bitmap is used in painting the client area.
