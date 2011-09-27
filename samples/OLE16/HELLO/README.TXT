---------------------------------------
OLE Automation Sample Program: Dispcalc
---------------------------------------

This is the "hello world" sample application.  It is an OLE object
which is programmable via IDispatch.

The application has one window which contains a button and a text box.
When the button is pressed, the string "Hello world." is displayed in
the text box.

The hello application to the SayHello method by performing the same
action as when the "Say Hello" button in the user interface is pressed.
The SayHello method returns the same string.

This is the simplest OLE automation sample application and is a good
place to start if you have not done any OLE Automation programming.

The ProgID for hello's only object is "Hello.Application".
An instance of this object can be created by executing the
following lines of code in Visual Basic or DispTest:

  Sub Foo
	Dim MyCalculator as Object

	Set MyCalculator = CreateObject("Hello.Application")
	. . .
  End Sub



-----------------
Program Structure
-----------------
Hello uses a type library and CreateStdDispatch in order to
implement the IDispatch interface.



-------------------------
Properties for the object
-------------------------


Name        Type        Description
---------------------------------------------------------------------
HelloMessage    VT_BSTR     the message that is printed when the
				SayHello method is called or when the
				SayHello button is pressed.



-----------------------------
Methods defined on the object
-----------------------------


Name            Description
--------------------------------------------------------------
SayHello() as String    Print the hello message and return it.
