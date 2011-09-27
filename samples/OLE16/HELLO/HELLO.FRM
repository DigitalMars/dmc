VERSION 2.00
Begin Form Form1 
   Caption         =   "This is a Disptest application"
   ClientHeight    =   3915
   ClientLeft      =   1995
   ClientTop       =   2115
   ClientWidth     =   7500
   Height          =   4320
   Left            =   1935
   LinkTopic       =   "Form1"
   ScaleHeight     =   3915
   ScaleWidth      =   7500
   Top             =   1770
   Width           =   7620
   Begin CommandButton btnGetHelloValue 
      Caption         =   "Get Hello's Value"
      Height          =   500
      Left            =   240
      TabIndex        =   9
      Top             =   3200
      Width           =   2000
   End
   Begin TextBox txtSetHelloMsg 
      Height          =   285
      Left            =   4300
      TabIndex        =   4
      Top             =   2205
      Width           =   3000
   End
   Begin CommandButton btnSetHelloMsg 
      Caption         =   "Set Hello Message"
      Height          =   500
      Left            =   195
      TabIndex        =   3
      Top             =   2205
      Width           =   2000
   End
   Begin CommandButton btnGetHelloMsg 
      Caption         =   "Get Hello Message"
      Height          =   500
      Left            =   200
      TabIndex        =   2
      Top             =   1200
      Width           =   2000
   End
   Begin PictureBox picOutput 
      Height          =   735
      Left            =   4300
      ScaleHeight     =   705
      ScaleWidth      =   2970
      TabIndex        =   1
      Top             =   195
      Width           =   3000
   End
   Begin CommandButton btnSayHello 
      Caption         =   "Say Hello"
      Height          =   500
      Left            =   200
      TabIndex        =   0
      Top             =   200
      Width           =   2000
   End
   Begin Label lblHelloValue 
      BorderStyle     =   1  'Fixed Single
      Height          =   260
      Left            =   4300
      TabIndex        =   11
      Top             =   3200
      Width           =   3000
   End
   Begin Label zLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "Hello's' value:"
      Height          =   255
      Index           =   3
      Left            =   2280
      TabIndex        =   10
      Top             =   3195
      Width           =   1935
   End
   Begin Label zLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "New Hello message:"
      Height          =   255
      Index           =   2
      Left            =   2300
      TabIndex        =   8
      Top             =   2205
      Width           =   1940
   End
   Begin Label zLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "Hello message:"
      Height          =   255
      Index           =   1
      Left            =   2300
      TabIndex        =   7
      Top             =   1200
      Width           =   1940
   End
   Begin Label zLabel 
      Alignment       =   1  'Right Justify
      Caption         =   "Output pane:"
      Height          =   255
      Index           =   0
      Left            =   2300
      TabIndex        =   6
      Top             =   195
      Width           =   1940
   End
   Begin Label lblGetHelloMsg 
      BorderStyle     =   1  'Fixed Single
      Height          =   260
      Left            =   4300
      TabIndex        =   5
      Top             =   1200
      Width           =   3000
   End
End
Dim MyObj As object

Sub btnGetHelloMsg_Click ()
    lblGetHelloMsg.Caption = MyObj.HelloMessage
End Sub

Sub btnGetHelloValue_Click ()
    lblHelloValue.Caption = MyObj
End Sub

Sub btnSayHello_Click ()
    picOutput.Print MyObj.SayHello
End Sub

Sub btnSetHelloMsg_Click ()
    MyObj.HelloMessage = txtSetHelloMsg.Text
End Sub

Sub Form_Load ()
    Set MyObj = CreateObject("hello.application")
End Sub

