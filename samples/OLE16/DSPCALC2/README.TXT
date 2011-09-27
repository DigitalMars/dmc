---------------------------------------
OLE Automation Sample Program: Dispcalc
---------------------------------------

DspCalc2 is a simple accumulator-based calculator.  Its
user interface consists of buttons for the numbers (0-9), 
the operators (+, -, *, /), and some other necessary
buttons (C for Clear, = for evaluation).  Its programmability
interface consists of one object, which is described below.

The ProgID for dspcalc2's only object is "DspCalc2.Application".  
An instance of this object can be created by executing the 
following lines of code in Visual Basic or DispTest:

  Sub Foo	
    Dim MyCalculator as Object	

    Set MyCalculator = CreateObject("DspCalc2.Application")
    . . .
  End Sub


-----------------
Program Structure
-----------------
DspCalc2 uses a type library and CreateStdDispatch in order to 
implement the IDispatch interface.



-------------------------
Properties for the object
-------------------------


Name	Type	Description
---------------------------
Value	VT_I4	Same as the value for the accumulator.

Accum	VT_I4	The value that is in the accumulator of the
		calculator.

Opnd	VT_I4	The operand.  This is the number which is
		currently being entered.

Op	VT_I2	The operator that is currently being used.
		This is an enumeration:
			const OP_NONE = 0
			const OP_PLUS = 1
			const OP_MINUS = 2
			const OP_MULT = 3	



-----------------------------
Methods defined on the object
-----------------------------


Name			Description
---------------------------
Eval() as Boolean	If there is an operator, apply it to
			accumulator and the operand, placing the
			result in the accumulator.

			The return value indicates success or
			failure.

Clear()			Resets the calculator.  This sets
			Op to OP_NONE, and both Accum and
			Opnd to 0.
Display()		Updates the display of the calculator.
			(Other operations do not do this.)

Quit()			Close the calculator.

Button(b as string) as Boolean
			Press the indicated button and return
			success or failure.
	
			Valid string values are:
				+, -, *, +
				0-9
				c, C
				=
			
			Note that you may also pass the numbers
			0-9 and these will be converted to strings
			automatically.



---------------------------
Shortcomings of this sample
---------------------------
1. Property and method names should not be abbreviated.
For example, the "Opnd" property should be the "Operand" 
property.

2. Since the object is the application object, it should
have Name and Version properties, which are read-only.
