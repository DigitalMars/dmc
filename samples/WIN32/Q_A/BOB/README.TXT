 Sample: BOB (Big Orange Button)

Summary:

The BOB sample demonstrates the steps necessary to reboot
or shutdown the machine in Win32/NT and Win32s/MS-DOS
programmatically. A very similar method can be used to log
a user off. Basically the steps necessary are as follows

1) Check to see if the machine is running Win32/NT; if so

a) first get the security token.
b) Fetch the LUID for the SeShutdownPrivilege; this is only
   required for rebooting and shutting down the machine.
c) Enable the shut down privilege.

2) Use ExitWindowsEx() to log off, shut down or reboot the
   machine.

More Information:

Obviously one should use this API with care *and* with plenty
of warning so a user is not rudely surprised.

There are two methods of preventing the system from shutting
down one is at the App level and the other the System level.
At the application level what one needs to process is the
WM_QUERYENDSESSION so it can ask the user if they wish to save
any data which has not been back up to disk. Finally return
TRUE assuming the process can shut down safely or FALSE if
it can not (NOTE this will not keep one from dying if the
parameter to ExitWindowsEx() was or'ed with EXW_FORCE).
At the system level one is able to remove the rights of
certain users/groups to reboot the machine. This is done
via the UserManager applet.

Enjoy
