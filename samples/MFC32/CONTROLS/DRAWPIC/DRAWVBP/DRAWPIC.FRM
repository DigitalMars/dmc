VERSION 4.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   4140
   ClientLeft      =   1140
   ClientTop       =   1515
   ClientWidth     =   6690
   Height          =   4545
   Left            =   1080
   LinkTopic       =   "Form1"
   ScaleHeight     =   4140
   ScaleWidth      =   6690
   Top             =   1170
   Width           =   6810
   Begin VB.CommandButton Command3 
      Caption         =   "Add Icons"
      Height          =   375
      Left            =   2520
      TabIndex        =   3
      Top             =   3360
      Width           =   1455
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Clear Pictures"
      Height          =   375
      Left            =   4080
      TabIndex        =   2
      Top             =   3360
      Width           =   1455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Add Pictures"
      Height          =   375
      Left            =   960
      TabIndex        =   1
      Top             =   3360
      Width           =   1455
   End
   Begin DRAWPICLib.DrawPic DrawPic1 
      Height          =   2295
      Left            =   1080
      TabIndex        =   0
      Top             =   480
      Width           =   4335
      _Version        =   65536
      _ExtentX        =   7646
      _ExtentY        =   4048
      _StockProps     =   0
   End
End
Attribute VB_Name = "Form1"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
' This is a part of the Microsoft Foundation Classes C++ library.
' Copyright (C) 1992-1996 Microsoft Corporation
' All rights reserved.
'
' This source code is only intended as a supplement to the
' Microsoft Foundation Classes Reference and related
' electronic documentation provided with the library.
' See these sources for detailed information regarding the
' Microsoft Foundation Classes product.'
'
Private Sub Command1_Click()
    Dim Pic As Picture
    Dim RetVal As Boolean
    
    Set Pic = LoadPicture(App.Path + "\Pic1.bmp")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic2.bmp")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic3.bmp")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic4.bmp")
    RetVal = DrawPic1.AddPicture(Pic)
    
End Sub


Private Sub Command2_Click()
    DrawPic1.ClearImageList
End Sub


Private Sub Command3_Click()
    Dim Pic As Picture
    Dim RetVal As Boolean
    
    Set Pic = LoadPicture(App.Path + "\Pic1.ico")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic2.ico")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic3.ico")
    RetVal = DrawPic1.AddPicture(Pic)
    
    Set Pic = LoadPicture(App.Path + "\Pic4.ico")
    RetVal = DrawPic1.AddPicture(Pic)
    

End Sub


