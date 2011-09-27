-------------------------------------
OLE Automation Sample Program: SPoly2
-------------------------------------

SPoly2 is a program which draws polygons.  The only way to 
make spoly2 draw a polygon is to use its programmability 
interface.

One OLE Automation object is exposed by spoly2:
  * spoly2.application

Spoly2.Application is the object associated with spoly2's main
window.  It controls drawing polygons and clearing the display.


-----------------
Program Structure
-----------------
SPoly2 implements IDispatch by using INTERFACEDATA, DispGetIDsOfNames
and DispInvoke.



------------------------------------
Methods defined on spoly2.application
------------------------------------


Name			        Description
------------------------------------------------------------------
Draw()				Draw the polygon.

Reset()				Delete all points from the polygon.

AddPoint(X, Y)			Add a point with coordinates (x,y) 
				to the polygon

EnumPoints() as VT_ENUM		Return a collection of the polygon's 
				points

GetXOrigin() as short		Get the X origin of the polygon.
			
SetXOrigin(x as short)		Set the X origin of the polygon.
			
GetYOrigin() as short		Get the Y origin of the polygon.
			
SetYOrigin(y as short)		Set the Y origin of the polygon.
			
GetWidth() as short		Get and the line width of the polygon.

SetWidth(width as short)	Set the line width of the polygon.



---------------------------
Shortcomings of this sample
---------------------------
1. Many items in this sample should be properties.  Instead,
they are implemented as methods.  Anything which behaves like
an attribute of the object should be a property.  

2. This is not a good example of how to implement a collection.