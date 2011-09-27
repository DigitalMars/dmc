VERSION 2.00
Begin Form MANIP 
   Caption         =   "Manipulate X Objects"
   ClientHeight    =   4605
   ClientLeft      =   1545
   ClientTop       =   2520
   ClientWidth     =   6795
   Height          =   5295
   Left            =   1485
   LinkTopic       =   "Form1"
   ScaleHeight     =   4605
   ScaleWidth      =   6795
   Top             =   1890
   Width           =   6915
   Begin CommandButton Command1 
      Caption         =   "&Get Object"
      Default         =   -1  'True
      Height          =   375
      Index           =   0
      Left            =   4680
      TabIndex        =   11
      Top             =   360
      Width           =   1935
   End
   Begin CommandButton Command1 
      Caption         =   "&Release Object"
      Height          =   375
      Index           =   1
      Left            =   4680
      TabIndex        =   10
      Top             =   840
      Width           =   1935
   End
   Begin CommandButton Command1 
      Caption         =   "&Set Extent"
      Height          =   375
      Index           =   2
      Left            =   4680
      TabIndex        =   9
      Top             =   1320
      Width           =   1935
   End
   Begin Frame Frame1 
      Caption         =   "Object Information"
      Height          =   1815
      Left            =   240
      TabIndex        =   2
      Top             =   240
      Width           =   4215
      Begin TextBox Text1 
         Height          =   285
         Left            =   1560
         TabIndex        =   5
         Top             =   480
         Width           =   2415
      End
      Begin TextBox Text2 
         Height          =   285
         Left            =   1560
         TabIndex        =   4
         Text            =   "6000"
         Top             =   840
         Width           =   975
      End
      Begin TextBox Text3 
         Height          =   285
         Left            =   1560
         TabIndex        =   3
         Text            =   "4242"
         Top             =   1200
         Width           =   975
      End
      Begin Label Label1 
         Alignment       =   1  'Right Justify
         Caption         =   "Object Name:"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   480
         Width           =   1335
      End
      Begin Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "cxExt:            End If"
         Height          =   255
         Index           =   0
         Left            =   960
         TabIndex        =   7
         Top             =   840
         Width           =   615
      End
      Begin Label Label2 
         Alignment       =   1  'Right Justify
         Caption         =   "cyExt:            End If"
         Height          =   255
         Index           =   1
         Left            =   960
         TabIndex        =   6
         Top             =   1200
         Width           =   615
      End
   End
   Begin OLE OLE1 
      fFFHk           =   -1  'True
      Height          =   375
      Left            =   1200
      OLETypeAllowed  =   0  'Linked
      TabIndex        =   1
      Top             =   2160
      Visible         =   0   'False
      Width           =   1815
   End
   Begin CommandButton Command1 
      Caption         =   "Show &Object"
      Height          =   375
      Index           =   3
      Left            =   4680
      TabIndex        =   0
      Top             =   1800
      Width           =   1935
   End
   Begin Menu File 
      Caption         =   "&File"
      Begin Menu FileExit 
         Caption         =   "E&xit"
      End
   End
   Begin Menu Edit 
      Caption         =   "&Edit"
      Begin Menu EditPasteLink 
         Caption         =   "&Paste Link"
      End
   End
End
Option Explicit

Const OLE_PASTE = 5
Const OLE_DELETE_OBJECT = 10

Dim X As Object

Sub Command1_Click (Index As Integer)
    
  Select Case Index
    Case 0
	Set X = GetObject(Text1.Text) ' bind to the X object

	Command1(0).Enabled = False
	Command1(1).Enabled = True
	Command1(2).Enabled = True
	Command1(2).Default = True
	Command1(3).Enabled = True

    Case 1
	Set X = Nothing

	Command1(0).Enabled = True
	Command1(0).Default = True
	Command1(1).Enabled = False
	Command1(2).Enabled = False
	Command1(3).Enabled = False

    Case 2
	X.SetExtent Val(Text2.Text), Val(Text3.Text) ' invoke SetExtent method
    
    Case 3
	X.ShowObject True ' invoke ShowObject method

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

