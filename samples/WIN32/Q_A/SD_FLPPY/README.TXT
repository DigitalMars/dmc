Sample:  Demonstration of code to put a DACL on floppy drives

Summary:  There is no way to put a DACL on the floppy drives or on
  the COM ports with REGEDT32, or using the Control Panel or other
  part of the user interface.  And there is no way to use the
  Win32 api to put a DACL on the floppy drives or on the COM ports
  that survives reboots

sd_flppy.exe, however, does put DACLs on the floppy drives or on
  the COM ports that survive logoff/logon, that is, these DACLs
  are on the floppy drives or on the COM ports until the next
  reboot

A version of this sample program can be installed as a service, so
  that each time the machine boots up the DACLs are automatically
  re-applied

Purpose:  Show sample code that applies DACLs to floppy drives and
  COM ports

There are possibly as many desired user interfaces to this sort of
  functionality as there are people thinking about this, so it is
  not a purpose of this sample (or the Win32 service variation of
  it) to present an incredibly cool user interface to how the
  DACLs get applied.  A very simplistic approach is taken to the
  user interface.  Anyone who desires that more complicated DACLs
  are applied, or desires other variations in the user interface
  hopefully will benefit by being able to use this sample code as
  a starting point for their DACL-applying app

This sample is not a supported utility

To run:  Type sd_flppy to lock the \\.\A: and \\.\B: devices

Putting sd_flppy.exe in a Startup group or logon script could work
  for some people

Further notes:

The version of this program that is packaged as a Win32 service is
  in this same directory and is built along with sd_flppy.exe by
  the same makefile

The packaged-as-a-service approach might better suit people who
  need to change the DACL on the floppy drives without requiring a
  re-boot or logoff.  After installing the floplock.exe service on
  the machine, the client app chgflpsd.exe can be used over the
  network to lock, unlock or query the floppy-locked-state of any
  machine where the floplock.exe service is running

So, this packaged-as-a-service approach might better suit people
  that would like to be able to inquire over the net what the
  DACLs are on the floppy drives of particular machines (to check
  or audit them).  And this approach might better suit people that
  would prefer that the DACls be applied as the system boots up so
  the DACLs are applied before any user has logged on at the
  keyboard

The packaged-as-a-service approach is more aligned with the notion
  of protecting the floppies as a resource on a particular
  _machine_ (regardless of who if anyone is logged on), whereas
  the sd_flppy.exe approach (running an .exe at logon time) is
  more along the lines of keeping a partcular _user_ from using
  the floppies on any machine that that user might use.  However,
  once user Sam6 has logged onto machine \\Mach3 and locked the
  floppies with sd_flppy.exe, the floppies will stay locked until
  reboot.  Of course a utility could easily be written that could
  run in the startup group of a different authorized user such as
  Jane3 to force the floppies on any machine Jane3 logs onto to be
  unlocked

As noted above the service Floplock.exe is built by the single
  makefile in this directory.  Floplock.exe is the main element of
  the packaged-as-a-service version.  Floplock.exe is the .exe to
  install as a Win32 service, and Instsrv.exe is the utility to do
  this installation.  Instsrv.exe, as well as Chgflpsd.exe, is
  also built by the single makefile in this directory

Do "instsrv" to get help on the switches to use when running
  instsrv.exe to install the service .exe

Chgflpsd.exe is the client part of the client/server pair.
  Floplock.exe, which you install as a service, is the server part

Floplock.exe creates a named pipe which Chgflpsd.exe opens to pass
  over the lock/unlock/query operations to a particular machine
  running Floplock.exe - the named pipe is at all times protected
  by an Admins-only DACL

Do "chgflpsd" to get help on the switches to use when running
  Chgflpsd.exe

