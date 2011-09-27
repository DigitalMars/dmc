===========
  XSERVER
===========

  IMPORTANT
-------------
you need the following DLL's on your path, inorder for this server
to be used:

  compobj.dll
  ole2.dll
  ole2disp.dll
  ole2prox.dll
  storage.dll
  xserver.dll


  DESCRIPTION
---------------

xserver is a OLE 2.01 InProc Server sample. It is written in C++ and uses
nested classes to implement OLE interfaces. you don't need MFC to build
this app. the objects in this DLL implement the following interfaces.

class CClassFactory
  IUnknown
  IClassFactory

class CXObject
  IUnknown
  IPersist
  IPersistStorage
  IOleObject
  IDataObject
  IViewObject2
  IOleCache2
  IRunnableObject
  IExternalConnection
  IStdMarshalInfo
  IDispatch

class CEnumStatData
  IUnknown
  IEnumSTATDATA

an xserver object can be Inserted into a OLE 2.0x container application
and can be linked inside to, as well as programmed through IDispatch interface.
there are 2 classes supported by the xserver.dll, "XDServer" which prints
out debug information for all methods called on it to DBWin or debug terminal,
and "XNDServer" which doesn't ouput information.

there is no GUI for this server other than the feedback you get through
the object's presentation, which you can use to determine which state
the object is in. the object follows the following state diagram.


		  (1)           (2)                          (3)
  Pasive <---> Loaded <-----> Invisible & Running <-------> Visible & Running
			|  ^                                          |  ^
			|  |------------------------------------------|  |
			|                       (4)                      |
			|------------------------------------------------|
									(5)


(1) OleLoad() is called on the object. IClassFactory::CreateInstance()
  will be called followed by IPersistStorage::InitNew() or
  IPersistStorage::Load() depending if the object existed previously.

(2) OleRun() or IOleObject::DoVerb(OLEIVERB_HIDE) called on the object.

(3) IOleObject::DoVerb(OLEIVERB_HIDE) called on the object, which
  cause the object to remove its "user lock" by calling its own
  IRunnableObject::LockRunning(FALSE, FALSE).

(4) IOleObject::Close() is called on the object, which cause the object
  to go to the loaded state. xserver also supports a verb to let
  this happen, IOleObject::DoVerb(1).

(5) IOleObject::DoVerb(OLEIVERB_SHOW or 0) will place the xserver object
  into the running and "visible" state.


Visual Basic Support
====================
there are 4 methods which this objects supports:

  Draw hDC%, left&, top&, right&, bottom&
  SetExtent cxExt&, cyExt&
  GetExtent cxExt&, cyExt& (not supported with VB 3.0)
  ShowObject fShow%

a demonstration of this code would be:

Sub XServerSample(Picture1 As Control)
  Dim x As Object
  Set x = CreateObject("XDServer") ' create an instance of this object

  x.SetExtent Picture1.ScaleWidth, Picture1.ScaleHeight
  x.Draw Picture1.hDC, Picture1.ScaleLeft, Picture1.ScaleTop,
			 Picture1.ScaleWidth, Picture1.ScaleHeight
End Sub
