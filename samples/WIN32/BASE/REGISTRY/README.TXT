Sample: Using API Functions to Access the Registry

Summary:

The Registry Monkey is a simple utility which demonstrates
the Registry API functions needed to access the NT Registry.
Monkey can be used to climb up and down the various branches
of the Registry tree, displaying the individual key's data
values.  Monkey can also be used to print specified trees to
a file named Registry.txt.

The Registry Monkey sample can be found in the
\MSTOOLS\SAMPLES\REGISTRY directory.

More Information:

To use: start an instance of the Monkey.  A dialog box will
appear with several edit fields, list boxes and buttons.
The listbox in the center of the dialog box (labeled CHILD
KEYS: at the bottom) will always hold the child keys of the
current key.  Initially it has four entries, representing
the four pre-defined key handles of the Registry:
HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER, HKEY_USERS, and
HKEY_CLASSES_ROOT.  If you double click on any of these
entries, or high light it and press the "Next/Down" button;
the key that you just selected will appear in the edit field
"Key Name", and the children of that selected key will
replace the entries in "CHILD KEYS:" list box.  I.e. if you
select HKEY_LOCAL_MACHINE, that name will be present in "Key
Name", and it's children will appear in the list box:
HARDWARE, SECURITY, SOFTWARE, and SYSTEM.  To proceed deeper
into the tree, double click another child.  To back out of
the Registry, double click on the ".." at the top of the
listbox, or press the "Back/Up" button.

If the current key has values associated with it, the name
of the values will be listed in the right hand listbox
(labeled "VALUES:").  If it has now values, "VALUES:" will
be followed by a "0".  Once you come upon a key that does
have values associated with it, you can double click on any
of the values in this list box.  At the bottom of the dialog
box are two edit fields: "Value: Data Type", and "Value:
Data Entry".  By double clicking a value entry, these edit
fields will be filled in the data's type and the data's
value.  I.e. if you follow the tree down to
HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System; and double
click on the entry "1) Identifier", the "Value: Data Type"
field will be filled with "REG_SZ: A null-terminated Unicode
string"; and the "Value: Data Entry" may be filled in with
something such as "AT/AT COMPATIBLE"

If the current key has a Class type associated with it, it
will appear in the "Class" edit field.  The "ACL" edit field
is not implemented with this release of the Registry Monkey.

You can use the Registry Monkey to write any part of the
Registry Tree to a file called REGISTRY.TXT.  To do this,
select either the "Full" or "Trimmed" buttons (this
specifies either writing all of the key entries, or only
those having Value data associated with them); and press the
"Print Branch" button.  The Registry Monkey will begin at
the current branch, and will proceed recursively down the
branches to the end of the tree, writing the information to
the file.  To write the entire tree, print each of the four
pre-defined keys.  Note, this can make for a rather large
file (700Kb at the time this was written).

