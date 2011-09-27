VERSION 2.00
Begin Form Form1 
   Caption         =   "AutoClik Test Driver"
   ClientHeight    =   2895
   ClientLeft      =   135
   ClientTop       =   1275
   ClientWidth     =   5100
   Height          =   3300
   Left            =   75
   LinkTopic       =   "Form1"
   ScaleHeight     =   2895
   ScaleWidth      =   5100
   Top             =   930
   Width           =   5220
   Begin CommandButton SetPosition 
      Caption         =   "Set P&osition"
      Height          =   375
      Left            =   240
      TabIndex        =   9
      Top             =   1560
      Width           =   1335
   End
   Begin CommandButton GetPosition 
      Caption         =   "Get &Position"
      Height          =   375
      Left            =   240
      TabIndex        =   10
      Top             =   2280
      Width           =   1335
   End
   Begin CommandButton AnimateXY 
      Caption         =   "&Animate X && Y"
      Height          =   375
      Left            =   3360
      TabIndex        =   14
      Top             =   1800
      Width           =   1575
   End
   Begin CommandButton AnimatePosition 
      Caption         =   "A&nimate Position"
      Height          =   375
      Left            =   3360
      TabIndex        =   15
      Top             =   2280
      Width           =   1575
   End
   Begin CommandButton RefreshDisplay 
      Caption         =   "&Refresh Display"
      Height          =   375
      Left            =   3360
      TabIndex        =   13
      Top             =   1320
      Width           =   1575
   End
   Begin CommandButton SetAll 
      Caption         =   "&Set All"
      Height          =   375
      Left            =   1680
      TabIndex        =   11
      Top             =   1560
      Width           =   1335
   End
   Begin CommandButton GetAll 
      Caption         =   "&Get All"
      Default         =   -1  'True
      Height          =   375
      Left            =   1680
      TabIndex        =   12
      Top             =   2280
      Width           =   1335
   End
   Begin CommandButton SetText 
      Caption         =   "Set &Text"
      Height          =   375
      Left            =   1680
      TabIndex        =   8
      Top             =   1080
      Width           =   1335
   End
   Begin CommandButton SetY 
      Caption         =   "Set &Y"
      Height          =   375
      Left            =   960
      TabIndex        =   7
      Top             =   1080
      Width           =   615
   End
   Begin CommandButton SetX 
      Caption         =   "&Set &X"
      Height          =   375
      Left            =   240
      TabIndex        =   6
      Top             =   1080
      Width           =   615
   End
   Begin TextBox Text 
      Height          =   375
      Left            =   1680
      TabIndex        =   5
      Top             =   480
      Width           =   1335
   End
   Begin TextBox Y 
      Height          =   375
      Left            =   960
      TabIndex        =   4
      Top             =   480
      Width           =   615
   End
   Begin TextBox X 
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   480
      Width           =   615
   End
   Begin CommandButton Close 
      Caption         =   "&Close"
      Height          =   375
      Left            =   3360
      TabIndex        =   16
      Top             =   480
      Width           =   1575
   End
   Begin Label Label3 
      Caption         =   "&Text"
      Height          =   255
      Left            =   1680
      TabIndex        =   2
      Top             =   240
      Width           =   1335
   End
   Begin Label Label2 
      Caption         =   "&Y"
      Height          =   255
      Left            =   960
      TabIndex        =   1
      Top             =   240
      Width           =   615
   End
   Begin Label Label1 
      Caption         =   "&X"
      Height          =   255
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
End
Dim clik As object
Dim pos As object

Sub AnimatePosition_Click ()
    Set pos = clik.position
    For i = 10 To 100 Step 5
        pos.X = i
        pos.Y = i
        Set clik.position = pos
        DoEvents
    Next i
End Sub

Sub AnimateXY_Click ()
    For i = 10 To 100 Step 5
        clik.X = i
        clik.Y = i
        DoEvents
    Next i
End Sub

Sub Close_Click ()
    End
End Sub

Sub Form_Load ()
    Set clik = CreateObject("AutoClik.Document.1")
    clik.ShowWindow
End Sub

Sub GetAll_Click ()
    X.Text = clik.X
    Y.Text = clik.Y
    Text.Text = clik.Text
End Sub

Sub GetPosition_Click ()
    Set pos = clik.position
    X.Text = pos.X
    Y.Text = pos.Y
End Sub

Sub RefreshDisplay_Click ()
    clik.RefreshWindow
End Sub

Sub SetAll_Click ()
    clik.SetAllProps X.Text, Y.Text, Text.Text
End Sub

Sub SetPosition_Click ()
    Set pos = clik.position
    pos.X = X.Text
    pos.Y = Y.Text
    Set clik.position = pos
End Sub

Sub SetText_Click ()
    clik.Text = Text.Text
End Sub

Sub SetX_Click ()
    clik.X = X.Text
End Sub

Sub SetY_Click ()
    clik.Y = Y.Text
End Sub

