
                       Readme for owncombo.exe

The owncombo.exe example application illustrates the use of functions
and messages for combo boxes and owner draw.  The user interface of
this example application is fairly self-explanatory except, perhaps,
for the text combo box dialog.  This readme provides an explanation
of this portion of the application's user interface, to help you better
interpret the source code.

The text combo box dialog buttons send various messages to the combo box 
and the edit control.  These buttons allow the user to vary the data sent 
with each message. 

The following actions are performed by the buttons:

Unsl All:  This button unselects any selection in the combo box.

Sel No:    This button takes an integer value from the edit control and
           attempts to select an entry in the combo box given this index
           value.

Sel Txt:   This button takes a text string from the edit control and
           attempts to select the item with the given text prefix.

Find Txt:  This button searches for the text given in the edit control
           and returns the item number where it was found in the combo
           box. 

Cb Dir:    This appends a directory listing of the current directory
           into the combo box.

Clr It:    This clears all the items in the combo box.

Add It:    This takes the string given in the edit control and adds it
           to the combo box.

Del It:    This deletes the currently selected item from the combo box.

Cpy It:    This copies the currently selected item in the combo box to
           the edit control.


Combo Notifications Check Box:

           When this box is checked, a message box will appear showing
           what notification codes the combo box is returning to the app in
           response to the messages sent by the buttons.
