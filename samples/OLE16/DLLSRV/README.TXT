		  Custom Moniker, InProc Server and Data Service

	  (c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved


	  The DATAVIEW server is an example of a server implemented as a
	  DLL.  An embedded DATAVIEW object displays a simulated stock
	  name and price, and the display is in black text if the stock
	  price is below some threshold value and in red if the price is
	  above the threshold.  When the stock value increases past the
	  threshold, the object optionally beeps.

	  Part of the data of a DATAVIEW object is a new moniker type which
	  allows the DATAVIEW object to bind to a REALTIME object, which is an
	  object that simulates a realtime stock service. The implementation of
	  the REALTIME objects uses a random number generator, and is in a
	  separate executable realtime.exe; the objects support the IDataObject
	  interface but not the IOleObject or IViewObject interfaces, and so
	  they are not strictly speaking OLE objects. The DATAVIEW objects get
	  notifications of changes in the REALTIME objects through the OLE
	  notification mechanism, and they get data from the REALTIME objects
	  through IDataObject::GetData.

	  This example supports a division between view and content.  The
	  view state includes the threshold value and whether there is a
	  beep when the threshold value is crossed, and the data includes
	  the value of the simulated stock price.  If there are two
	  embedded data view objects that connect to the same stock item,
	  then the threshold values may be different, and hence the
	  display color may be different, but the underlying stock value
	  will be the same for both objects.  In this case there are two
	  independent view states and a common, shared stock value.

	  TO COMPILE:

	  1.  Build the OLE2UI libraries for DLLs and executables.
	  2.  Build PRGIDMON.DLL
	  3.  Build DATAVIEW.DLL
	  4.  Build REALTIME.EXE

	  TO RUN:

	  Since DATAVIEW is an OLE2 server dll, it cannot be run by itself,
	  only inserted as an embedding into OLE2 containers.  In order to
	  be an embedding, the reg files dataview.reg, prgidmon.reg and
	  realtime.reg must have been registered.
