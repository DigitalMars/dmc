---------------------------------------
OLE Automation Sample Program: DispDemo
---------------------------------------

DispDemo is a sample OLE Automation controller.  DispDemo
drives the two polygon servers which are also provided
as samples.


-----------------
Program Structure
-----------------
The interesting part of DispDemo is in the remote polygon
class.  The remote polygon class was constructed so that 
the IDispatch members could be accessed conveniently from 
C++.  The remote polygon class does this by transforming
the C++ calls into IDispatch calls.  The code that calls
these routines has no idea that it is going through IDispatch.



