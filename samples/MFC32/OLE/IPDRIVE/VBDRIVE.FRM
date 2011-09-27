VERSION 2.00
Begin Form Form1 
   Caption         =   "InProc Driver"
   ClientHeight    =   2100
   ClientLeft      =   255
   ClientTop       =   5955
   ClientWidth     =   5310
   Height          =   2505
   Left            =   195
   LinkTopic       =   "Form1"
   ScaleHeight     =   2100
   ScaleWidth      =   5310
   Top             =   5610
   Width           =   5430
   Begin CommandButton Remove 
      Caption         =   "&Remove"
      Height          =   375
      Left            =   3600
      TabIndex        =   8
      Top             =   1200
      Width           =   1335
   End
   Begin CommandButton Add 
      Caption         =   "&Add"
      Height          =   375
      Left            =   3600
      TabIndex        =   7
      Top             =   720
      Width           =   1335
   End
   Begin CommandButton Lookup 
      Caption         =   "&Lookup"
      Height          =   375
      Left            =   3600
      TabIndex        =   6
      Top             =   240
      Width           =   1335
   End
   Begin TextBox MapTo 
      Height          =   285
      Left            =   840
      TabIndex        =   1
      Top             =   720
      Width           =   2055
   End
   Begin TextBox MapFrom 
      Height          =   285
      Left            =   840
      TabIndex        =   0
      Top             =   240
      Width           =   2055
   End
   Begin Label MapCount 
      Height          =   255
      Left            =   960
      TabIndex        =   5
      Top             =   1320
      Width           =   1935
   End
   Begin Label Label3 
	  Caption		  =   "Count:"
      Height          =   255
      Left            =   240
      TabIndex        =   4
      Top             =   1320
      Width           =   615
   End
   Begin Label Label2 
      Caption         =   "&To:"
      Height          =   255
      Left            =   240
      TabIndex        =   3
      Top             =   720
      Width           =   495
   End
   Begin Label Label1 
      Caption         =   "&From:"
      Height          =   255
      Left            =   240
      TabIndex        =   2
      Top             =   240
      Width           =   615
   End
End
Dim map As object

Sub Add_Click ()
    map.SetAt mapFrom.text, MapTo.text
    Lookup_Click
	MapCount.caption = map.Count
End Sub

Sub Form_Load ()
	Set map = CreateObject("mfc.inproc.varmap")
End Sub

Sub Form_Unload (Cancel As Integer)
    Set map = Nothing
End Sub

Sub Lookup_Click ()
	temp = map.Item(mapFrom.text)
    MapTo.text = temp
End Sub

Sub Remove_Click ()
    map.RemoveKey mapFrom.text
    MapTo.text = "<nothing>"
	MapCount.caption = map.Count
End Sub
