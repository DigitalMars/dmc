CDTEST consists of a number of dialog boxes that allow you to edit
the values of each of the common dialog creation structure members.
This will allow you to create nearly any type of common dialog box
on the fly.

Here are some things that you need to know about how it works:


For ALL the CDTEST dialogs:
***************************

1.  CDTEST does not create any type of dialog on its own besides
    a default one.  If you want to create your own type of common dialog,
    you have to edit the structure element fields.  For instance, if
    you want to hide the network button in the GetOpenFileName()
    dialog, you have to "OR" the value for OFN_NONETWORKBUTTON in
    "Flags" structure element field.  To get the values for the
    common dialog flags, look in the commdlg.h header file.


2.  There are some check boxes in each dialog that need some explanation:

    A.  The "NULL Structure" box.

        If you check this box, CDTEST will send a NULL value for the common
        dialog creation structure to the common dialog function.  The common
        dialog should fail at startup, and the common dialog function should
        return FALSE and set the correct CommDlgExtendedError().

    B.  The "Preloaded Template" box(es).

        If this box is marked, CDTEST will preload a custom template
        resource and put the value of the handle to the resource
        in the hInstance member of the common dialog structure (or in
        the hPrintTemplate / hSetupTemplate if you are in the PrintDlg()
        dialog).  BUT, CDTEST will not create a custom template dialog
        with this preloaded custom template unless you also put the
        XXX_ENABLETEMPLATEHANDLE flag in the "Flags" box.  So to create
        a GetOpenFileName() dialog with a multiple selection files list box
        and a preloaded custom dialog template:

        1.  Put (OFN_ENABLETEMPLATEHANDLE | OFN_ALLOWMULTISELECT)
            in the "Flags" edit box.

        2.  Check the "Preloaded Template" box.

        3.  Click OK.

        * Note: Be sure you are in the right "number mode".  If you are
        in "Decimal Mode" and you enter hex values into the structure
        you will get unexpected results.  Choose Hex Mode or Decimal
        mode from the main menu.


3.  The RESET Button will set all the values for the common dialog
    creation structure back to CDTEST's defaults.

4.  "Add" and "Clear" buttons.  These are used to edit arrays that
    might be needed in common dialog box creation.  "Clear" clears
    the whole array and "Add" adds what you have typed into the
    edit control part of the combo box to the left of these buttons.

5.  "MultiThread" buttons.  These will create dialogs in exactly the
    same way that CDTEST always does, but it will create two threads
    and two dialogs.

    *Note: The modeless-ness of the find and replace dialogs causes
    some problems with multithreading them, so multithreading
    for FindText() and ReplaceText() has not been implemented.




Information specific to only some of the CDTEST dialogs:
********************************************************

ChooseFont:
-----------

1.  When you open the choosefont dialog you see a list of ChooseFont()
    flags and fonts on the right of the vertical dividing line.  These
    do not affect the choosefont dialog creation at all.  These only exist
    to allow the user to compare the fonts found by Choosefont() with a
    list of fonts found independently of ChooseFont().  The lists should
    be indentical.  So, if you want to see a list of only the 
    scalable screen fonts, mark the "CF_SCALABLEONLY" and "CF_SCREENFONTS"
    boxes.  If you create a ChooseFont() dialog with the flags of
    (CF_SCREENFONTS | CF_SCALABLEONLY) you should get the same list of
    fonts.

2.  NOTE:  The ChooseFont() dialog will give you a "No fonts found" 
    error if you don't include the CF_SCREENFONTS or the CF_PRINTERFONTS 
    flag as part of the value in the "Flags" edit box.  This is by common 
    dialog design.

3.  If you want printer fonts to be listed in the ChooseFont() dialog, you
    have to mark the "Printer" radio button in the "HDC Type" group.

    Example:

    To create a ChooseFont() dialog containing only the scalable screen
    fonts that exist on the current printer:

    A.  Put the value for (CF_PRINTERFONTS | CF_SCALABLEONLY) in the "Flags"
        edit box.

    B.  Mark the "Printer" option in the "HDC Type" group.

    C.  Click OK.



Open/Save:
----------

1.  If you want to create a custom template Open or Save dialog with
    the OFN_ENABLETEMPLATE flag, you need to specify the name of the
    template in the "lpTemplateName" structure field.  The names are:

    opentemp1 -- single selection files list box dialog template.

    opentemp2 -- multiple selection files list box dialog template.


Find/Replace:
-------------

1.  If you want to create a custom template Find or Replace dialog with
    the FR_ENABLETEMPLATE flag, you need to specify the name of the
    template in the "lpTemplateName" structure field.  The names are:

    frtemp1 -- find template.

    frtemp2 -- replace template.



Final Notes:
************


* There is no way to create a NULL pointer as a value to be used for one of
  the dialog elements that requires a string pointer such as the "lpstrFile"
  element of the GetOpenFileName() structure.  If you leave this field blank,
  CDTEST will call GetOpenFileName with a valid pointer to a string containing
  a NULL character as its first value.


* If you click the multithreading button, the two dialogs that are created
  are positioned in exactly the same spot on the screen so it looks like only
  one was created.


* It is best to click the "Reset" button after you return to CDTEST from
  a common dialog because CDTEST fills the current structure-editing fields
  with the values contained in the common dialog creation structure AFTER
  the common dialog function has returned.  These values may not be what
  you expect.

  An example of when this would cause a problem would be the following:

  1.  Go to Dialogs, GetOpenFileName.

  2.  Click OK to create the default GetOpenFileName dialog.

  3.  Cancel the GetOpenFileName dialog.

  4.  Put the value for OFN_ENABLEHOOK in the "Fields" edit box, and click
      OK.  This will fail because if you call GetOpenFileName() without
      OFN_ENABLEHOOK, it sets the value for OPENFILENAME.lpfnHook to
      NULL - by common dialog design.  So when you returned from the first
      GetOpenFileName() dialog, the NULL value for OPENFILENAME.lpfnHook was
      put in the "lpfnHook" edit box, and when you tried to call the function
      again with the OFN_ENABLEHOOK flag, you get a failure because the
      address for your hook proc is NULL.
