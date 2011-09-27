 Sample: Virtual Memory API Function Demonstration

Summary:

VIRTMEM is a sample of the various virtual memory API
functions available under Win32.

Upon starting the application you are initially given a
RESERVED page of virtual memory.  You can change the
protection and state of the page through menu selections.
Check marks will appear in the menu items to indicate the
current state and protection on the page.  More in depth
information regarding the page can be obtained by selecting
the Show Page menu item.

The Lock menu item allows you to lock and unlock the page in
memory.

The application also uses structured exception handling and
allows you to try and write to the page in its various
states and protections.  To try this, select the Test menu
option.

