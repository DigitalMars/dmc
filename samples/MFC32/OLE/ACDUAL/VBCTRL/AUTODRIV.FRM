VERSION 4.00
Begin VB.Form Form1 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "AutoClik Test Driver"
   ClientHeight    =   3510
   ClientLeft      =   1845
   ClientTop       =   1620
   ClientWidth     =   5100
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   0
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   3930
   Left            =   1785
   LinkTopic       =   "Form1"
   ScaleHeight     =   3510
   ScaleWidth      =   5100
   Top             =   1260
   Width           =   5220
   Begin VB.CommandButton cmdTestError 
      Caption         =   "Test&Error"
      Height          =   375
      Left            =   2160
      TabIndex        =   19
      Top             =   3000
      Width           =   1455
   End
   Begin VB.TextBox txtWCode 
      Appearance      =   0  'Flat
      Height          =   375
      Left            =   1080
      TabIndex        =   18
      Top             =   3000
      Width           =   855
   End
   Begin VB.CommandButton SetPosition 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Set P&osition"
      Height          =   375
      Left            =   240
      TabIndex        =   9
      Top             =   1560
      Width           =   1335
   End
   Begin VB.CommandButton GetPosition 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Get &Position"
      Height          =   375
      Left            =   240
      TabIndex        =   10
      Top             =   2280
      Width           =   1335
   End
   Begin VB.CommandButton AnimateXY 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Animate X && Y"
      Height          =   375
      Left            =   3360
      TabIndex        =   14
      Top             =   1800
      Width           =   1575
   End
   Begin VB.CommandButton AnimatePosition 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "A&nimate Position"
      Height          =   375
      Left            =   3360
      TabIndex        =   15
      Top             =   2280
      Width           =   1575
   End
   Begin VB.CommandButton RefreshDisplay 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Refresh Display"
      Height          =   375
      Left            =   3360
      TabIndex        =   13
      Top             =   1320
      Width           =   1575
   End
   Begin VB.CommandButton SetAll 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Set All"
      Height          =   375
      Left            =   1680
      TabIndex        =   11
      Top             =   1560
      Width           =   1335
   End
   Begin VB.CommandButton GetAll 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Get All"
      Default         =   -1  'True
      Height          =   375
      Left            =   1680
      TabIndex        =   12
      Top             =   2280
      Width           =   1335
   End
   Begin VB.CommandButton SetText 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Set &Text"
      Height          =   375
      Left            =   1680
      TabIndex        =   8
      Top             =   1080
      Width           =   1335
   End
   Begin VB.CommandButton SetY 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Set &Y"
      Height          =   375
      Left            =   960
      TabIndex        =   7
      Top             =   1080
      Width           =   615
   End
   Begin VB.CommandButton SetX 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Set &X"
      Height          =   375
      Left            =   240
      TabIndex        =   6
      Top             =   1080
      Width           =   615
   End
   Begin VB.TextBox Text 
      Appearance      =   0  'Flat
      Height          =   375
      Left            =   1680
      TabIndex        =   5
      Top             =   480
      Width           =   1335
   End
   Begin VB.TextBox Y 
      Appearance      =   0  'Flat
      Height          =   375
      Left            =   960
      TabIndex        =   4
      Top             =   480
      Width           =   615
   End
   Begin VB.TextBox X 
      Appearance      =   0  'Flat
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   480
      Width           =   615
   End
   Begin VB.CommandButton Close 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Close"
      Height          =   375
      Left            =   3360
      TabIndex        =   16
      Top             =   480
      Width           =   1575
   End
   Begin VB.Label Label4 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&WCode:"
      ForeColor       =   &H80000008&
      Height          =   375
      Left            =   240
      TabIndex        =   17
      Top             =   3000
      Width           =   735
   End
   Begin VB.Label Label3 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Text"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1680
      TabIndex        =   2
      Top             =   240
      Width           =   1335
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&Y"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   960
      TabIndex        =   1
      Top             =   240
      Width           =   615
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "&X"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Dim clik As ACDual.Document
Dim pos As ACDual.Point

Private Sub AnimatePosition_Click()
    Set pos = clik.Position
    For i = 10 To 100 Step 5
        pos.x = i
        pos.y = i
        Set clik.Position = pos
        DoEvents
    Next i
End Sub

Private Sub AnimateXY_Click()
    For i = 10 To 100 Step 5
        clik.x = i
        clik.y = i
        DoEvents
    Next i
End Sub

Private Sub Close_Click()
    End
End Sub

Private Sub cmdTestError_Click()
    On Error Resume Next    ' Defer error handling.
    clik.TestError (txtWCode.Text)
    ' Check for error, then show message.
    If Err.Number <> 0 Then
        Msg = "Error # " & Hex(Err.Number) & " was generated by " _
              & Err.Source & Chr(13) & Err.Description
        MsgBox Msg, , "Error", Err.HelpFile, Err.HelpContext
    End If
End Sub

Private Sub Form_Load()
    Set clik = New ACDual.Document
    clik.ShowWindow
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set clik = Nothing
End Sub

Private Sub GetAll_Click()
    x.Text = clik.x
    y.Text = clik.y
    Text.Text = clik.Text
End Sub

Private Sub GetPosition_Click()
    Set pos = clik.Position
    x.Text = pos.x
    y.Text = pos.y
End Sub

Private Sub RefreshDisplay_Click()
    clik.RefreshWindow
End Sub

Private Sub SetAll_Click()
    clik.SetAllProps x.Text, y.Text, Text.Text
End Sub

Private Sub SetPosition_Click()
    Set pos = clik.Position
    pos.x = x.Text
    pos.y = y.Text
    Set clik.Position = pos
End Sub

Private Sub SetText_Click()
    clik.Text = Text.Text
End Sub

Private Sub SetX_Click()
    clik.x = x.Text
End Sub

Private Sub SetY_Click()
    clik.y = y.Text
End Sub

