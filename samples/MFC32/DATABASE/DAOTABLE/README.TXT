DAOTable--DAO Database, TableDef, and QueryDef Creation

-----------------------------------------------------------------------
The information in this article applies to:

  - The Microsoft Foundation Classes (MFC), included with:
      - Microsoft Visual C++, 32-bit Edition, version 4.0
-----------------------------------------------------------------------

SUMMARY
=======

The DAOTable sample demonstrates using the MFC DAO classes to create
common database objects:  Databases, Tables, Queries, Fields, and
Indexes.  This dialog-based application maps the properties of these
objects to controls the user can set and/or view.  The program source
code is organized so that most database interaction is isolated from
the user interface code to ease finding examples of how to use MFC DAO.

In addition to demonstrating the use of the MFC DAO classes, this
sample can be a useful tool for creating simple Access databases.
You can create MDB files from scratch, create and delete tables and
queries, add and delete fields and indexes in the tables, and modify
existing queries.

The following files contain the code which performs the database
operations.  Additional files in the project provide the user
interface and high level database manipulation.

    QUERYDEF.CPP, .H -- functions to create and use queries
    TABLEDEF.CPP, .H -- functions to create and use tables
    FIELD.CPP, .H    -- functions to create and use fields
    INDEX.CPP, .H    -- functions to create and use indexes
    DATABASE.CPP, .H -- functions to create and use databases

To use this code and the MFC DAO classes you must have version
4.0 or greater of Visual C++.

To get the code for this sample, download DAOTBL.EXE, a self-
extracting file, from the Microsoft Software Library (MSL) on the
following services:

NOTE: The file must be decompressed by typing "DAOTBL.EXE -d".

 - CompuServe
      GO MSL
      Search for DAOTBL.EXE
      Display results and download

 - Microsoft Download Service (MSDL)
      Dial (206) 936-6735 to connect to MSDL
      Download DAOTBL.EXE

 - Internet (anonymous FTP)
      ftp ftp.microsoft.com
      Change to the SOFTLIB\MSLFILES directory
      Get DAOTBL.EXE

MORE INFORMATION
================

The MFC DAO classes provide a means of directly using the Jet Database
Engine.  While Visual C++'s wizards automatically generate code that
will create and open recordsets for you based on implicitly created
database and workspace objects, the code they provide does not
demonstrate creating and manipulating MFC DAO objects directly. With
MFC DAO classes you can create the majority of DAO objects and set
their properties.

This sample demonstrates using the MFC DAO classes to create and use
the following DAO objects: Databases, Tables, Queries, Fields, and
Indexes.  Exception handling with both MFC exception macros and the C++
exception mechanism is also shown.  Further, DAO exceptions are put to
use in detecting the existence of objects in the DAO collections.

Listed below are all the files in the project (except those which are
common to all Visual C++ projects).  Each file is shown with the type
of components it contains and a brief description

ADDQYDLG.CPP         - USER INTERFACE - query definition dialog
ADDQYDLG.H
DAOTABLE.CPP         - APPLICATION - main application objects
DAOTABLE.H
DAOTDLG.CPP          - USER INTERFACE - main dialog
DAOTDLG.H
DATABASE.CPP         - MFC DAO - database specific functions
DATABASE.H             (also demonstrates C++ exception handling)
ADDIXDLG.CPP         - USER INTERFACE - add indexes dialog
ADDIXDLG.H
ADDTBDLG.CPP         - USER INTERFACE - table definition dialog
ADDTBDLG.H
FIELD.CPP            - MFC DAO - field specific functions
FIELD.H
LISTCTRL.CPP         - USER INTERFACE - derived CListCtrl class
LISTCTRL.H
INDEX.CPP            - MFC DAO - index specific functions
INDEX.H
QUERYDEF.CPP         - MFC DAO - querydef specific functions
QUERYDEF.H
TABLEDEF.CPP         - MFC DAO - tabledef specific functions
TABLEDEF.H
ADDDBDLG.CPP         - USER INTERFACE - database definition dialog
ADDDBDLG.H


REFERENCES
==========

For more information about the MFC DAO classes, refer to the on-line
help for Visual C++ 4.0.  For additional documentation on DAO, refer to
the help files in the DAO SDK which is provided on the VC++ 4.0 CD-ROM.
