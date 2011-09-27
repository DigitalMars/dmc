VERSION 4.00
Begin VB.Form Form1 
   Caption         =   "AutoClik Test Driver"
   ClientHeight    =   2895
   ClientLeft      =   1635
   ClientTop       =   2190
   ClientWidth     =   5100
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   1
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   3300
   Left            =   1575
   LinkTopic       =   "Form1"
   ScaleHeight     =   2895
   ScaleWidth      =   5100
   Top             =   1845
   Width           =   5220
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
   Begin VB.Label Label3 
      Caption         =   "&Text"
      Height          =   255
      Left            =   1680
      TabIndex        =   2
      Top             =   240
      Width           =   1335
   End
   Begin VB.Label Label2 
      Caption         =   "&Y"
      Height          =   255
      Left            =   960
      TabIndex        =   1
      Top             =   240
      Width           =   615
   End
   Begin VB.Label Label1 
      Caption         =   "&X"
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
Dim clik As Object
Dim pos As Object

Private Sub AnimatePosition_Click()
    Set pos = clik.position
    For i = 10 To 100 Step 5
        pos.X = i
        pos.Y = i
        Set clik.position = pos
        DoEvents
    Next i
End Sub

Private Sub AnimateXY_Click()
    For i = 10 To 100 Step 5
        clik.X = i
        clik.Y = i
        DoEvents
    Next i
End Sub

Private Sub Close_Click()
    End
End Sub

Private Sub Form_Load()
    Set clik = CreateObject("AutoClick.Document")
    clik.ShowWindow
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set clik = Nothing
End Sub

Private Sub GetAll_Click()
    X.Text = clik.X
    Y.Text = clik.Y
    Text.Text = clik.Text
End Sub

Private Sub GetPosition_Click()
    Set pos = clik.position
    X.Text = pos.X
    Y.Text = pos.Y
End Sub

Private Sub RefreshDisplay_Click()
    clik.RefreshWindow
End Sub

Private Sub SetAll_Click()
    clik.SetAllProps X.Text, Y.Text, Text.Text
End Sub

Private Sub SetPosition_Click()
    Set pos = clik.position
    pos.X = X.Text
    pos.Y = Y.Text
    Set clik.position = pos
End Sub

Private Sub SetText_Click()
    clik.Text = Text.Text
End Sub

Private Sub SetX_Click()
    clik.X = X.Text
End Sub

Private Sub SetY_Click()
    clik.Y = Y.Text
End Sub

