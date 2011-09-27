Sample: Monitoring System Events

This sample uses a variety of thread-specific hook procedures to monitor
the system for events affecting a thread. It demonstrates how to process 
events for the following types of hook procedures: 


WH_CALLWNDPROC 
WH_CBT 
WH_DEBUG 
WH_GETMESSAGE 
WH_KEYBOARD 
WH_MOUSE 
WH_MSGFILTER 

In this sample, the user can install and remove a hook procedure by using 
the menu. When a hook procedure is installed and an event that is monitored 
by the procedure occurs, the procedure writes information about the event 
to the client area of the application's main window. 


