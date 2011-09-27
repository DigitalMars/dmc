Sample: Demonstrating how to share memory between processes.

Summary:


The MEMORY sample demonstrates the use of the file mapping APIs
to create a share memory between processes.

More Information:

Server

    The "server" sets up the named share memory.

    A "server" can be created by selecting the Create.Server menu
    item.

    This opens up a MDI child and swaps the menu bar to the
    server menu bar.

    To set up the equivalent of a named share memory:

    1. Select the Server.Create File... menu item.
       This brings up the File Type dialog box.  The user may create
       either a memory page file or a physical file by clicking on
       the Page File or the Map File button respectively to back up
       the soon-to-be-created named share memory.

       If the Map File button is selected, the named shared memory
       will be backed up by a physical file on the disk.  On the
       other hand, if the Page File button is clicked, the memory
       will be backed up by the memory page file.

    2. Select the Server.Create File Mapping... menu item.
       This brings up the Map Name dialog box.  The user then
       specify a name for the memory map file object which will be
       created for the file created in step 1 above.

       This name will be used to identify the shared memory by the
       clients in the other processes.

    3. Select the Server.Map View of File menu item.
       This essentially maps the map file object created in step 2
       above into the process's address space.

    4. Select the Server.Access menu item.
       This creates a MLE inside the MDI Server child.  Whatever
       written in the MLE is put in the map file object.

Client

    The "client" connects to the named share memory created by
    the server in another process.

    A "client" can be created by selecting the Create.Client menu
    item.

    This opens up a MDI child and swaps the menu bar to the
    client menu bar.

    To set up the connection to the named share memory:

    1. Select the Client.Open File Mapping... menu item.
       This brings up the Map Name dialog box.  The user can then
       enter the name of the map file object which client wanted
       to connect to.

    2. Select the Client.Map View of File menu item.
       This essentially maps the map file object opened in step 1
       above into the process's address space.

    3. Select the Client.Access menu item.
       This creates a MLE inside the MDI Server child.  Whatever
       got written in the map file object by the server will be
       shown in this MLE.

       The client synchronizes with the server at a regular interval.

    4. Select the Client.Refresh Now menu item refreshes the
       contents of the map file object immediately.
