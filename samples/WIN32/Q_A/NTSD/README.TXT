 Sample: Writing NTSD Extensions

Summary:

This article and the associated sample demonstrate how to
write a NTSD extension for the NTSD debugger.
More Information:

While debugging, we often find ourselves looking up certain
fields of a certain structure in the program. Traditionally,
this involves dumping the address of the structure in
question and locating the specific fields in the dump, which
is often very tedious and inefficient.

In NTSD, the programmers can write a dumping routine to be
called by the NTSD debugger.

The routine has to be in a DLL and has the following
prototype:

   void Routine (HANDLE, HANDLE, HANDLE,
PNTSD_EXTENSION_APIS, LPSTR);

   See the file DEBUG.C, which is part of the NTSD sample,
for details.

Then, to invoke the routine in NTSD, the user would do the
following:

   !module.routine argument



