Global DispCalc As Object  ' Calculator object, scope is global to application.

Sub Main ()

    ' Get a number to put in calculator.
    x = InputBox("Enter a number to begin with.")
    If x = "" Then Exit Sub                       ' Check if canceled,
    
    Set DispCalc = CreateObject("dispcalc.application")   ' Create new instance of calculator.

    DispCalc = x                                     ' Assign x to DispCalc's default property.
    DispCalc.Display                                 ' Display value.

    ' Initialize data in listbox.
    frmDriver.lstScript.AddItem "Cube"
    frmDriver.lstScript.AddItem "Count down"
    frmDriver.lstScript.AddItem "Decrement"
    frmDriver.lstScript.AddItem "Factorial"
    frmDriver.lstScript.AddItem "Increment"
    frmDriver.lstScript.AddItem "Square"
    frmDriver.lstScript.AddItem "Square root"
    frmDriver.lstScript.AddItem "Tangent"
    frmDriver.lstScript.AddItem "Quit"
    
    ' Display form.
    frmDriver.Show

End Sub

