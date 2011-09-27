	This is a part of the Microsoft Foundation Classes C++ library.
	Copyright (C) 1994-1995 Microsoft Corporation
	All rights reserved.

	This source code is only intended as a supplement to the
	Microsoft Foundation Classes Reference and related
	electronic documentation provided with the library.
	See these sources for detailed information regarding the
	Microsoft Foundation Classes product.


-------------------------------------------------------
MTMDI Sample Microsoft Foundation Classes Application
-------------------------------------------------------

The MTMDI sample illustrates an MFC user-interface thread, where user
interface events are processed in a thread separate from the main
application thread.  This sample is a modified version of the single
thread MDI sample.

The MTMDI sample does not claim a strong rationale for putting the
bouncing ball window in a separate thread.  An end-user would not be
able to detect the difference between the MDI and MTMDI samples on a
single processor machine.  Even on a multi-processor machine, the
end-user would not be able to detect the difference given that the
ball movement is based on a window timer.

Further, the MTMDI sample does not claim a strong rationale for using
an MFC worker thread instead of an MFC user-interface thread.  MFC worker
threads generally are easier to use and more appropriate than user-interface
threads for tasks that do not involve processing of user interface events.
The drawing of the bouncing ball could have been implemented in
a worker thread instead of a user-interface thread, if some technique
other than window timers were used to advance the ball.  The use of the
window timer in a separate thread requires a message pump; therefore
the separate thread must be a user-interface thread instead of a worker
thread.  To add slightly more justification for using a user-interface
thread, the MTMDI sample includes one additional user-interface feature
not in the MDI sample:  you can click anywhere in the bounce window to
immediately change the position of the moving ball.

Although the MDI sample does not claim a strong rationale for using
threads, it nevertheless does illustrate techniques for implementing an
MFC user-interface thread.  The remainder of this readme describes
the differences between the implementation of the single thread MDI
application and the multiple thread MTMDI application.  These differences
illustrate that it is more difficult to implement an application with
a user-interface thread than a corresponding application that executes
in a single thread.  This should be a warning that you should not use
user-interface threads unless you have good reasons.

The overall differences between the implementation of the MDI and MTMDI
samples are these:

1.  The CBounceWnd window runs in a separate user-interface thread
	in the MTMDI application.

2.  In the MDI sample, CBounceWnd is derived from CMDIChildWnd.  In the
	MTMDI sample, CBounceWnd is derived from CWnd, and a CBounceWnd
	window is a child of the MDI child window.  In the MTMDI sample,
	the CBounceWnd child window fills exactly the client area of the
	parent MDI child window (a CMDIChildBounceWnd).

3.  In the MDI sample, normal MFC command routing and command user-
	interface initialization are used for menu commands associated with
	the bounce window.  In the MTMDI sample, the MDI child window's
	OnCmdMsg function is overridden in order to send the OnCmdMsg
	parameters via SendMessage to the CBounceWnd executing in the
	separate thread.  In general, SendMessage typically needs to be
	used us to make calls from a window in one thread to a window in
	another thread in an MFC application.

Note:  The Hello window is left in the main application thread.
	   Implementing the Bounce window in a separate thread is sufficient
	   to illustrate the MFC multithread techniques.

This MTMDI sample does not directly illustrate a view running in a separate
thread, because MTMDI and the original MDI sample do not use MFC's
document/view architecture.  Still, the design of MTMDI can be applied
to an application where you might want the view to run in a separate thread.
In general, you cannot successfully implement member functions of a CView
to run in a separate thread, because the MFC document/view architecture
relies on thread local storage (TLC) for some of the data that coordinates
documents and views.  However, you can apply the design of MTMDI by
implementing a child window of the CView window, and processing user-
interface events of this child window in a separate thread.

The design of MTMDI is summarized below.  For additional details, see
source code comments.


=================
CWinThread object
=================

The thread object for the bouncing ball window is implemented in
a CWinThread-derived class, CBounceThread, in mtbounce.cpp.

Beginning the user-interface thread
-----------------------------------
The overridden CMDIChildBounceWnd::OnCreate begins the thread for
the bouncing ball.   There are two ways to begin an MFC user-interface
thread:  (1) call AfxBeginThread, passing the CRuntimeClass of the
CWinThread-derived class; or (2) implement two-stage construction of
the CWinThread-derived object by new'ing it and then calling
CWinThread::CreateThread.  We use the second method because it offers
the easiest way to pass the HWND of the CMainFrame window to the
thread, which needs the parent HWND to create the child window.
We simply pass the HWND to the CBounceThread constructor.

An alternative method is:
- call AfxBeginThread with a CREATE_SUSPENDED parameter;
- pass the parent HWND via a new CBounceThread::SetParentWnd function;
  or make the CBounceThread::m_hwndParent member variable public and
  set it directly; and then
- call CWinThread::ResumeThread.

Thread instance initialization
------------------------------
CWinThread::InitInstance is the only member function that must be
overridden for a user-interface thread.  The implementation of
CBounceThread::InitInstance is typical in that it creates (using
CWnd::Create) the window that processes messages in the separate thread.

Terminating the user-interface thread
-------------------------------------
The easiest way to terminate a user-interface thread is to rely on
automatic termination of the thread when the main window associated with
the thread is destroyed.  The only thing you need to do to implement
such automatic thread termination is to set the CWinThread::m_pMainWnd
to the main window.  This is illustrated in CBounceThread::InitInstance.
The default CWnd::OnNcDestroy handler checks whether the window being
destroyed is the thread's m_pMainWnd and, if so, terminates the thread,
provided it isn't the main application thread.

Unless you change the default TRUE value of CWinThread::m_bAutoDelete,
the framework will automatically delete the CWinThread object when the
thread terminates.

Avoiding memory leak detection of CWinThread object
---------------------------------------------------
When the application terminates, it destroys each window in the window
hierarchy, and then, in debug mode, checks for memory leaks.
It is possible that the application will falsely detect a memory leak of the
CWinThread object before the user-interface thread has had a chance to
automatically terminate.  The reported memory leak is false because
eventually the CWinThread object will be automatically destroyed anyway.

Nevertheless, the dumping of memory leak information can be disconcerting.
To avoid this, we use a "bounce thread killed" event.  The CBounceThread
delete operator sets the event.  The main application waits for this event
before terminating.  It is better to set the event in the delete operator
rather than in the CBounceThread destructor, because it is remotely possible
that the application might terminate (and report a memory leak) between
the time the CBounceThread destructor completes and the time that the
CBounceThread object is actually deleted.


================
MDI child window
================
In the original MDI sample, the CBounceWnd was a CMDIChildWnd.  It
handled the Color and Speed menu commands.  In the MTMDI sample, the
CBounceWnd is a CWnd; and the CBounceWnd window is a child of the
MDI child window.  There still needs to be a CMDIChildWnd class.
It is named CMDIChildBounceWnd, and is implemented in bounce.cpp,
where CBounceWnd is also implemented.


Creation of the MDI child window
--------------------------------
As in the original MDI sample, the CMainFrame::OnBounce handler
creates the CMDIChildWnd when the user requests a new bouncing ball
window.  The implementation of CMDIChildBounceWnd::Create borrows
half of the implementation of the original MDI sample's
CBounceWnd::Create.  The half borrowed is the implementation of the
shared menu.  The other half, the window class registration, is
left in the implementation of CBounceWnd::Create.

The overridden CMDIChildBounce::Create also creates the bounce thread.

Delegation of commands and command user-interface initialization
----------------------------------------------------------------
CMDIChildWnd delegates commands and command user-interface initialization
to the child CBounceWnd.  One laborious way to do this is delegate each
command on a per command basis.  An easier but more sophisticated way to
do this is to override OnCmdMsg and pass the OnCmdMsg parameters to the
CBounceWnd in a structure via SendMessage.  For more details, see source
code comments for CMDIChildBounceWnd::OnCmdMsg.


============================
User-interface thread window
============================

The bouncing ball window is implemented in a CWnd-derived class,
CBounceWnd, in bounce.cpp.  It reuses almost all of the CBounceWnd code
in the original MDI sample, where the CBounceWnd was a CMDIChildWnd instead
of a plain CWnd.


Creation of the bouncing ball window
------------------------------------
CBounceThread::InitInstance calls the CBounceWnd-override of CWnd::Create
to create the window.  The implementation of CBounceWnd::Create is borrowed
from the original MDI sample's implementation, less the shared menu
initialization code that is now in CMDIChildBounceWnd::Create.

Command handling and command user-interface initialization
----------------------------------------------------------
All commands and command user-interface initialization is delegated by
the CMDIChildBounceWnd object to the CBounceWnd object.
CMDIChildBounceWnd::OnCmdMsg sends a user-defined WM_USER_ONCMDMSG message
to the CBounceWnd window.  This message contains all of the information
originally passed to CMDIChildBounceWnd::OnCmdMsg.
The CBounceWnd handler for WM_USER_ONCMDMSG, named OnDelegatedCmdMsg,
unpacks the COnCmdMsg struct passed via the lParam, and calls the default
CWnd::OnCmdMsg for the CBounceWnd object.
