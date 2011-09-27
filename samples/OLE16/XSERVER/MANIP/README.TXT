=========
MANIP.MAK
=========

DESCRIPTION
---------------
MANIP.MAK is DispTest application which can manipulate "X Server" objects 
using IDispatch interface to comunicate. To use MANIP.MAK:

  1. Run DISPTEST.EXE.
  2. Load the project MANIP.MAK in DispTest.
  3. Choose Start from the Run menu.
  2. Run cntroutl.exe (or another OLE 2.0 container which allows
	 linking "inside" to an embedding)
  3. In cntroutl.exe do Edit/Insert Object and choose X object
  4. Save the file, the Edit/Copy the object to clipboard
  5. in MANIP, choose Edit/Paste Link to get the link source
	 for the embedding.
  6. Shutdown cntroutl.exe (optional)
  7. in MANIP, click the "Get Object" button (this step will bind to
	 the embedded object, and cause it to go into a running state if it
	 is not already in there)
  8. Use the other buttons:
	 "Set Extent" - to change the size of the object
	 "Show Object" - to make the object visible
	 "Release Object" - to remove the binding

SOURCE CODE
---------------

Option Explicit

Const OLE_PASTE = 5
Const OLE_DELETE_OBJECT = 10

Dim x As Object

Sub Command1_Click (Index As Integer)

  Select Case Index
	Case 0
		Set x = GetObject(Text1.Text) ' bind to the X object

		Command1(0).Enabled = False
		Command1(1).Enabled = True
		Command1(2).Enabled = True
		Command1(2).Default = True
		Command1(3).Enabled = True

	Case 1
		Set x = Nothing

		Command1(0).Enabled = True
		Command1(0).Default = True
		Command1(1).Enabled = False
		Command1(2).Enabled = False
		Command1(3).Enabled = False

	Case 2
		x.SetExtent Val(Text2.Text), Val(Text3.Text) ' invoke SetExtent method

	Case 3
		x.ShowObject True ' invoke ShowObject method

  End Select

End Sub

Sub Edit_Click ()

	' Check to see if the object on the clipboard can be linked to
	Ole1.OLETypeAllowed = 0 ' Only allow links

	If Ole1.PasteOK Then
		EditPasteLink.Enabled = True
	Else
		EditPasteLink.Enabled = False
	End If

End Sub

Sub EditPasteLink_Click ()

	' grab the binable moniker for an object, and then delete the link

	Ole1.Action = OLE_PASTE
	Text1.Text = Ole1.SourceDoc
	Ole1.Action = OLE_DELETE_OBJECT

End Sub

Sub FileExit_Click ()

	End

End Sub

Sub Form_Load ()

	Command1(1).Enabled = False
	Command1(2).Enabled = False
	Command1(3).Enabled = False

End Sub
