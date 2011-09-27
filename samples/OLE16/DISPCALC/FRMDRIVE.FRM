VERSION 2.00
Begin Form frmDriver 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Choose script to run"
   ClientHeight    =   2496
   ClientLeft      =   4800
   ClientTop       =   1380
   ClientWidth     =   3204
   Height          =   2916
   Icon            =   FRMDRIVE.FRX:0000
   Left            =   4752
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   2496
   ScaleWidth      =   3204
   Top             =   1008
   Width           =   3300
   Begin ListBox lstScript 
      Height          =   2520
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3252
   End
End

Sub lstScript_Click ()

    x = DispCalc.Accum    ' Get Accum property (displayed value).
    
    Select Case lstScript.List(lstScript.ListIndex)
        
        Case "Decrement"
            DispCalc.Accum = x - 1 ' Decrement Accum property and display.
            DispCalc.Display
        
        Case "Count down"
            If DispCalc.Accum < 0 Then
                MsgBox ("Value too small.")
                Exit Sub
            End If
            ' Decrement default property (Accum) till zero.
            Do Until DispCalc = 0
                DispCalc = DispCalc - 1
                DispCalc.Display
            Loop
        
        Case "Cube"
            If DispCalc.Accum > 1000 Then
                MsgBox ("Value too large.")
                Exit Sub
            End If
            DispCalc.Op = 3       ' Use DispCalc's operator method (Op).
            DispCalc.Opnd = x
            DispCalc.Eval         ' Evaluate operation twice (cube).
            DispCalc.Eval
            DispCalc.Display
        
        Case "Factorial"
            If DispCalc.Accum > 16 Then
                MsgBox ("Value too large.")
                Exit Sub
            End If
            DispCalc.Op = 3       ' Use DispCalc's operator method (Op).
            For i = 1 To (x - 1)
                DispCalc.Opnd = i
                DispCalc.Eval
                DispCalc.Display
            Next
        
        Case "Increment"
            DispCalc.Accum = x + 1 ' Increment Accum property and display.
            DispCalc.Display
        
        Case "Square"
            If DispCalc.Accum > 30000 Then
                MsgBox ("Value too large.")
                Exit Sub
            End If
            DispCalc.Op = 3       ' Use DispCalc's operator method (Op).
            DispCalc.Opnd = x
            DispCalc.Eval         ' Evaluate operation once (square).
            DispCalc.Display
        
        Case "Square root"
            If DispCalc.Accum < 1 Then
                MsgBox ("Value too small.")
                Exit Sub
            End If
            DispCalc.Accum = Sqr(x)     ' Use intrinsic function.
            DispCalc.Display            ' Note that Calculator display integers only.
        
        Case "Tangent"
            DispCalc.Value = Tan(x)     ' Use intrinsic function.
            DispCalc.Display            ' Note that Calculator display integers only.
        
        Case "Quit"
            x = MsgBox("Close Calculator?", 35)
            Select Case x
                Case 6          ' If "Yes", then close calculator and end.
                    DispCalc.Quit
                    End
                Case 7          ' If "No", then
                    End         ' leave calculator running.
                Case Else       ' Otherwise, do nothing.
            End Select

        End Select

End Sub

