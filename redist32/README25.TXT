****************************************************************
            README.TXT File for Microsoft ODBC 2.5                

        (c) Copyright Microsoft (R) Corporation, 1995.
                       All Rights Reserved
****************************************************************

================================================================
                           INTRODUCTION
================================================================

This document contains release notes for Microsoft ODBC 2.5.
Information in this document should be considered to be the 
most up-to-date of any source.

================================================================
                             CONTENTS
================================================================

This document contains the following information:

Section         Description
=======         ===========

   1            Configuration Information
                  ODBC 2.5 Installation
                  System Requirements
                  RISC Platforms Not Supported
                  Components
                  32-Bit Multithreaded Drivers on Windows 95
                  ODBC Driver's Base Address
                  Language Library File
                  Header Files
                  3D Controls
                  ODBC.INF File
		  
   2            Installation Changes
                  Uninstall
                    Component Usage Count Tracking
                    File Usage Count Tracking
                    ODBC Installation Scenarios
                      Fresh Install Scenario
                      Uninstall Scenario
                      Upgrade/Replace Scenario
                    Installer Registration
                    Control Panel Changes
                  System DSNs
                    Administrator/Control Panel Setup
                    System DSN Setup Using Functions
                  New Installer Functions
                    ConfigDriver
                    SQLConfigDriver
                    SQLInstallTranslator
                    SQLRemoveDriver
                    SQLRemoveDriverManager
                    SQLRemoveTranslator
                  Modified Installer Functions
                    SQLConfigDataSource
                    SQLCreateDataSource
                    SQLGetPrivateProfileString
                    SQLInstallDriver
                    SQLInstallDriverManager
                    SQLInstallODBC
                    SQLManageDataSources
                    SQLWritePrivateProfileString

   3            ODBC Function Changes
                  Rebinding with SQLBindCol
                  Attempting to Add Truncated Data with 
                    SQLSetPos
                  pcbValue in SQLBindParameter
                  SQLSTATE S1C00 Returned By SQLPrepare
                  SQLSTATE 22005 RETURNED BY SQLExtendedFetch 
                    and SQLFetch
                  SQLSTATE 22008 RETURNED BY SQLExtendedFetch 
                    and SQLFetch
                  SQLSTATE 22012 RETURNED BY SQLGetData
                  Cursor Concurrency Set in SQLSetStmtOption
                  Error Message Format
                  Notes to Driver Writers

================================================================
              SECTION 1: CONFIGURATION INFORMATION
================================================================

      +++++++++++++++ODBC 2.5 INSTALLATION+++++++++++++++

If ODBC 2.5 components are being installed on an x86 platform 
over MSDN, the version 2.5 components should replace the 
corresponding ODBC 2.10 components. A new RELNOTES.HLP file 
for ODBC 2.5 will replace this version 2.10 release notes help 
file, and should be consulted for all ODBC 2.5 release notes.

The ODBC 2.5 files should be copied from the subdirectories of 
\ODBC210B\X86\ODBC25 on MSDN, to the appropriate directories 
on your computer, replacing the corresponding version 2.10 
files. Header files should be copied to the \ODBCSDK\INCLUDE 
directory. Library files should be copied to the 
\ODBCSDK\INCLUDE directory. Redistributable files should be 
copied to both the \ODBCSDK\REDIST32 and \WINDOWS\SYSTEM 
(or \SYSTEM32) directories. If you are going to use
ODBC Test, the 32-bit ODBC Test file (ODBCTE32.EXE) should 
also be copied.

        +++++++++++++++SYSTEM REQUIREMENTS+++++++++++++++

ODBC 2.5 is supported on Microsoft Windows 95 and Windows NT 3.5 
and 3.51. ODBC 2.5 does not have 16-bit components. Use 
ODBC 2.10 for 16-bit applications.

   +++++++++++++++RISC PLATFORMS NOT SUPPORTED+++++++++++++++

ODBC 2.5 will not be supported on RISC platforms. Use ODBC 2.10 
on RISC platforms.

          +++++++++++++++COMPONENTS+++++++++++++++

ODBC 2.5 consists of the following components:

Component               Filename

Driver Manager          ODBC32.DLL
Installer               ODBCCP32.DLL
                        ODBCCP32.CPL
Cursor Library          ODBCCR32.DLL
Language Library        ODBCINT.DLL
Administrator           ODBCAD32.EXE
Thunking Files          DS32GT.DLL
                        ODBC16GT.DLL
                        ODBC32GT.DLL
Installer Help          ODBCINST.HLP
                        ODBCINST.CNT
Header Files            ODBCINST.H
                        SQL.H
                        SQLEXT.H
                        SQLTYPES.H
Lib Files               ODBC32.LIB
                        ODBCCP32.LIB

ODBC 2.5 also uses the following thunking file that was 
included in the ODBC 2.10 SDK. It is not included in 
the ODBC 2.5 file list, so should be retained from the 
ODBC 2.10 SDK.

Thunking File          DS16GT.DLL

+++++++++++32-BIT MULTITHREADED DRIVERS ON WINDOWS 95+++++++++++             

On Windows 95, a 32-bit multithreaded driver will not work when
a 16-bit application tries to use it. A 32-bit multithreaded 
driver will work, on the other hand, with a 32-bit application.
The restriction on 16-bit applications occurs because Windows 
95 does not support multiple threads within a 16-bit process 
space.

The options for driver writers are as follows:

1.  Write a driver that is multithreaded when running in a 
32-bit process space, but single-threaded when running in a 
16-bit process space. This allows maximum interoperability and 
performance.

2.  Write a multithreaded driver that does not work with 16-bit
applications on Windows 95. The driver will have decreased 
interoperability, but will be simpler to write. A 16-bit driver 
can be written as well.

3.  Write a single-threaded driver that works with either 
16- or 32-bit applications on Windows 95. The driver will have 
maximum interoperability, but decreased performance.

   +++++++++++++++ODBC DRIVER'S BASE ADDRESS+++++++++++++++

ODBC driver should be linked with a base address of 0x04C00000.

     +++++++++++++++LANGUAGE LIBRARY FILES+++++++++++++++

All of the code that needs to be localized for all of the ODBC 
core components has been centralized into the language
library, ODBCINT.DLL. This file includes all error strings and 
all dialog boxes.

          +++++++++++++++HEADER FILES+++++++++++++++

The standard and extended header files, SQL.H and SQLEXT.H, 
have been modified in ODBC 2.5 to align with changes in the 
X/Open CAE specification. All material in SQL.H that was 
specific to Microsoft has been moved to SQLEXT.H. The format 
of the file was changed so that the datatypes and return types 
conform to the X/Open CAE specification. All material in 
SQLEXT.H that has been adopted by the standard has been moved 
to SQL.H.

If SQL.H and/or SQLEXT.H are included in driver or application 
code, "-DWINDOWS" should be added to the compiler command when 
building 16-bit drivers or applications.

SQLTYPES.H has been added to provide type definition for
program types in ODBC 2.5. SQLTYPES.H defines the handle 
environment, SQL portable types for C, transfer types for
DATE, TIME, and TIMESTAMP, and bookmarks.

           +++++++++++++++3D CONTROLS++++++++++++++

On Windows 95, ODBC 2.5 uses the native 3D controls of 
Windows 95 instead of CTL3D32.DLL. All ODBC drivers must also 
use the native 3D controls on Windows 95, not CTL3D32.DLL.

On Windows NT, the ODBC 2.5 core components will load the 
Unicode version of CTL3D32.DLL. 

          +++++++++++++++ODBC.INF FILE++++++++++++++

In the Driver Specification Section of the ODBC.INF file, the 
WinSysNTnn and WinSys95nn keywords (where nn is a number from 
00 to 99) indicate that the files specified will be installed 
on either Windows NT or Windows 95, but not on both. These 
keywords allow developers to install files selectively. 
A file can have the same name, but a different binary, for 
Windows NT and Windows 95.

================================================================
                SECTION 2: INSTALLATION CHANGES
================================================================

           +++++++++++++++UNINSTALL+++++++++++++++

In the course of installing an application, a user can install 
ODBC components, then uninstall the ODBC components. If the 
ODBC files are not used by another application, then the files 
are deleted. If the files are used by another application, the 
registry entries for ODBC are changed, but the actual files are 
not deleted.

ODBC components are uninstalled by component, not by file. The
ODBC components that can be uninstalled are the ODBC core 
components, ODBC drivers, and ODBC translators. Any component 
can be uninstalled, while the other components are left 
installed. For example, you can uninstall the ODBC translators 
while leaving the core components and drivers installed.

ODBC core components (which include the Driver Manager, Cursor 
Library, Installer, Language Library, Administrator, thunking 
files, etc.) are uninstalled as a whole. ODBC drivers and 
translators, on the other hand, are installed driver by driver, 
or translator by translator. Any number of ODBC drivers or 
translators can be uninstalled, while other drivers and 
translators are left installed. 

The Uninstall process depends upon two types of registry 
entries: a component usage count kept by the ODBC installer 
functions, and file count tracking kept by application setup 
programs. These registry entries are described below.

 +++++++++++++++COMPONENT USAGE COUNT TRACKING+++++++++++++++

The ODBC installer functions change registry and 
configuration information.  With the exception of 
SQLInstallODBC (see the SQLInstallODBC section below), they 
do not copy and delete files. The application setup program 
is responsible for copying and deleting files. 

ODBC installer functions maintain in the registry a count of 
the number of times that an ODBC component has been installed. 
This count is kept for each of the three ODBC components: the 
core components, the ODBC translators, and the ODBC drivers. 
A separate component usage count is kept for each ODBC 
translator and driver installed.

Each time an ODBC component is installed, an installation 
function is called, and the component usage count is 
incremented. The installation functions are 
SQLInstallDriverManager, SQLInstallDriver, and 
SQLInstallTranslator. Each time an ODBC component is 
uninstalled, a removal function is called, and the component 
usage count is decremented. The removal functions are 
SQLRemoveDriverManager, SQLRemoveDriver, and 
SQLRemoveTranslator. 

When an ODBC installer function is called to install an 
ODBC component for the first time, the function creates 
the registry entry and increments the component usage 
count, but does not copy the files. The application setup 
program must do that. When the installer function is 
called to install an ODBC component that has already been 
installed, it again increments the component usage count.

When an ODBC installer function is called by the application
to uninstall a component, and the usage count reaches 0, the 
installer function deletes the registry entry for that 
component. It does not, however, delete the component files. 
The application is responsible for deleting the files, and 
must use the file usage count to determine if the deletion 
should be performed (see the File Usage Count Tracking 
section following this section).

The component usage count is kept in the following registry 
entry:

Registry Subtree:       HKEY_LOCAL_MACHINE
Registry Hive:          SOFTWARE
Subtree:                ODBC
Class Name:             <NO CLASS>
Value Data Type:        REG_DWORD
Value Name:             UsageCount
Value Data:             <Number of times this component was 
                        installed>

The Key Name, which identifies the location of the component 
usage count value, is specified in the following table:

Component               Key Name (Registry Location)

ODBC Core Components    SOFTWARE\ODBC\ODBCINST.INI\ODBC Core
ODBC Translators        SOFTWARE\ODBC\ODBCINST.INI\<Translator 
                         Name>
ODBC Drivers            SOFTWARE\ODBC\ODBCINST.INI\<Driver Name>             

  +++++++++++++++FILE USAGE COUNT TRACKING+++++++++++++++

The files associated with each ODBC component are not copied or 
deleted by the ODBC installer functions. These functions 
manipulate the registry entries for the components. The files 
are installed or deleted by the application setup program. It 
is also the responsibility of the setup program to create, 
change, and delete registry entries, as necessary, for all ODBC 
files it installs or deletes. The registry entry includes a 
file usage count that is the number of times that a file has 
been installed.

When an application's setup program is called to uninstall 
a component, it should call the appropriate removal function 
(SQLRemoveDriverManager, SQLRemoveDriver, or 
SQLRemoveTranslator), which will decrement the component 
usage count. The setup program should then decrement the file
usage count. If the file usage count reaches 0, the setup 
program should delete the file. If the file usage count does 
not reach 0, the file should not be deleted.

  +++++++++++++++ODBC INSTALLATION SCENARIOS+++++++++++++++

The ODBC installation functions are called according to one 
of three installation scenarios: fresh install, uninstall, 
or upgrade/replace.

    +++++++++++++++FRESH INSTALL SCENARIO+++++++++++++++

When a fresh install is performed, the ODBC components have 
not previously been installed. ODBC installation functions 
are called in the following sequence, and the setup program 
must perform the following actions. Additional information 
on the ODBC installer functions is provided in later sections 
of these release notes.

1.  SQLInstallDriverManager is called to return the path for 
ODBC core components and increment the component usage count. 
The application setup program then installs the core component 
files. If a newer version of a core component file has not 
been previously installed, the application setup program 
copies the file, and creates the file usage count. If a newer 
version of a file has previously been installed, the setup 
program increments the file usage count. 

2.  SQLInstallDriver is called for each driver to add 
information about the driver to the ODBCINST.INI section of 
the registry, return the path for the driver, and increment 
the driver usage count. The application setup program then 
installs the driver files. If a newer version of a driver file 
has not been previously installed, the application setup program 
copies the file, and creates the file usage count. If a newer 
version of a file has previously been installed, the setup 
program increments the file usage count.

3.  SQLConfigDriver (a new function) is called with the
ODBC_INSTALL_DRIVER fOption to call the driver setup DLL. The 
driver setup DLL calls the ConfigDriver function to set 
configuration for the driver. (The application setup program 
must have installed the driver files, if necessary, before 
this step can be performed.)

4.  SQLInstallTranslator (a new function) is called to add 
information about the translator to the ODBCINST.INI section of 
the registry, and increment the translator's component usage 
count. The application setup program then installs the 
translator files. If a newer version of a translator file has 
not been previously installed, the application setup program 
copies the file, and creates the file usage count. If a newer 
version of a file has previously been installed, the setup 
program increments the file usage count.

        +++++++++++++++UNINSTALL SCENARIO+++++++++++++++

When an application setup program is called to perform an 
Uninstall, the ODBC component has previously been installed 
by the application. ODBC installation functions are called in 
the following sequence, and the setup program must perform the 
following actions. Additional information on the ODBC installer 
functions is provided in later sections of these release notes.

1.  SQLRemoveTranslator is called to remove information about
the translator from the ODBCINST.INI section of the registry, 
and decrement the translator's component usage count. If the 
component usage count falls to 0, the function removes the 
translator information (including the usage count) from the 
registry. For each translator file, the application setup 
program should check the file usage count. If the file usage 
count has fallen to 0, the setup program should delete the 
file. 

2.  SQLRemoveDriver is called to remove information about the 
driver from the ODBCINST.INI section of the registry and 
decrement the driver usage count. If the component usage count 
falls to 0, the function removes the driver information 
(including the component usage count) from the registry, and
calls SQLConfigDriver with the ODBC_REMOVE_DRIVER fOption. 
SQLConfigDriver calls the ConfigDriver function, which modifies
the configuration as necessary. For each driver file, the 
application setup program should check the file usage count. 
If the file usage count has fallen to 0, the setup program 
should delete the file.

3.  SQLRemoveDriverManager is called to decrement the Driver 
Manager component usage count. If the component usage count 
falls to 0, the function removes the Driver Manager information
(including the usage count) from the registry. For each core 
component file, the application setup program should check 
the file usage count. If the file usage count has fallen to 0, 
the setup program should delete the file.

     +++++++++++++++UPGRADE/REPLACE SCENARIO+++++++++++++++

When a component is upgraded or replaced, the component should 
be removed before being reinstalled, so that the component usage 
count is valid. All steps in the Uninstall scenario 
(SQLRemoveTranslator, SQLConfigDriver, SQLRemoveDriver, and 
SQLRemoveDriverManager) should be performed, then all steps in 
the Fresh Install scenario (SQLInstallDriverManager, 
SQLInstallDriver, SQLConfigDriver, and SQLInstallTranslator) 
should be performed. (See the Fresh Install Scenario and 
Uninstall Scenario sections for more information.)

      +++++++++++++++INSTALLER REGISTRATION+++++++++++++++

The ODBC installer in ODBC 2.5 is registered by copying the 
ODBCCP32.CPL file to the system directory. This action loads 
the control panel device. The ODBC installer no longer modifies 
the MMCPL entry in the registry and the CONTROL.INI file 
directly. When run, the ODBCCP32.CPL control panel device
deletes any existing ODBC entry in the MMCPL registry key.

       +++++++++++++++CONTROL PANEL CHANGES+++++++++++++++

The Add and Delete buttons have been removed from the Drivers 
dialog box that is displayed when the ODBC icon in the Control 
Panel (or Administrator) is chosen, then the Drivers button
in the Data Sources dialog box is chosen. The buttons were 
removed because this program has not yet been redesigned to 
modify the registry or remove files properly. 

A System DSN button has been added to the Data Source dialog 
box. For information on this change, see the following System 
DSNs section.

           +++++++++++++++SYSTEM DSNs++++++++++++++

ODBC 2.5 supports the creation of a system data-source name 
(DSN). A data source set up with a system DSN can be used by 
more than one user on the same machine. It can also be used 
by a system-wide service, which can then gain access to the 
data source even if no user is logged onto the machine.

A system DSN is registered in the HKEY_LOCAL_MACHINE registry, 
rather than the HKEY_CURRENT_USER registry. It is not tied to 
one user who logs on with their particular user name and 
password, but can be used by any user of that machine, or by 
an automatic system-wide service. The system DSN is, however, 
tied to one machine. It does not support the capability of 
using remote DSNs between machines.

System DSNs will be registered in the following registry 
location:

Registry Subtree:       HKEY_LOCAL_MACHINE
Registry Hive:          SOFTWARE
Subtree:                ODBC
Key:                    ODBC.INI

DSNs created for individual users, i.e., registered in the 
HKEY_CURRENT_USER registry, as at present, will be called 
user DSNs, to distinguish them from system DSNs.

System DSNs can be set up through the installer user 
interface or API functions, as described below.

+++++++++++++++ADMINISTRATOR/CONTROL PANEL SETUP++++++++++++++

A System DSN button has been added to the Data Sources dialog 
box displayed when the ODBC Administrator icon in the ODBC 
group is chosen, or when the ODBC icon in the Control Panel 
is chosen. When the System DSN button is chosen, a System Data 
Sources dialog box is displayed with controls that allow you 
to add or delete a system data source to your local computer,
or to set the configuration for a system data source.

+++++++++++++++SYSTEM DSN SETUP USING FUNCTIONS++++++++++++++

Five ODBC installer functions have been modified to 
accommodate system DSNs. Applications can call these 
functions in order to implement automatic or customized 
interactive management of system DSNs.

SQLConfigDataSources has been changed to accommodate the 
addition, configuration, and removal of system DSNs. 
SQLCreateDataSource has been changed to add a System DSN 
check box to the Add Data Source dialog box. 
SQLManageDataSources has been changed to accommodate the 
System DSN button in the Data Sources dialog box.

SQLGetPrivateProfileString and SQLWritePrivateProfileString 
support configuration of system DSNs for existing drivers. 
The system DSN works only if a driver reads from the registry
using SQLGetPrivateProfileString and writes to the registry
using SQLWritePrivateProfileString. If a driver reads from, 
or writes to, the registry itself, the system DSN may not 
work.

   +++++++++++++++NEW INSTALLER FUNCTIONS+++++++++++++++

The following new installer and driver setup functions have 
been added in ODBC 2.5 to support Uninstall and System DSNs. 
The functions are described in the Uninstall and System DSN 
sections above, and in detail below.

        ConfigDriver
        SQLConfigDriver
        SQLInstallTranslator
        SQLRemoveDriver
        SQLRemoveDriverManager
        SQLRemoveTranslator

        +++++++++++++++ConfigDriver+++++++++++++++

Purpose: ConfigDriver allows a driver to perform install and 
uninstall functions without requiring an application to call 
ConfigDSN. This function will perform driver-specific 
functions such as creating driver-specific INI files and 
performing DSN conversions during installation, and cleaning 
up INI files or registry modifications during Uninstall. 
This function is exposed by the driver setup DLL. 

Syntax:  BOOL ConfigDriver (hwndParent, fRequest, lpszDriver, 
lpszArgs, lpszMsg, cbMsgMax, pcbMsgOut)

Type    Argument        Use     Description

HWND    hwndParent      Input   Parent window handle.
WORD    fRequest        Input   Type of request. fRequest 
                                must contain one 
                                of the following values: 
                                  ODBC_INSTALL_DRIVER:  
                                   installing a new driver
                                  ODBC_REMOVE_DRIVER:  
                                   removing a driver
                                This option can also be
                                driver-specific, in which
                                case the first option will
                                be ODBC_CONFIG_DRIVER_MAX+1,
                                and additional options will
                                be incremented by 1 from
                                that value.
LPCSTR  lpszDriver      Input   The name of the driver as 
                                registered in the 
                                ODBCINST.INI key of the 
                                registry.
LPCSTR  lpszArgs        Input   A null-terminated string 
                                containing arguments for
                                a driver-specific fRequest.
LPSTR   lpszMsg         Output  A null-terminated string
                                containing an output message
                                from the driver setup.
WORD    cbMsgMax        Input   Length of lpszMsg.
WORD    pcbMsgOut       Output  Total number of bytes 
FAR *                           available to return in
                                lpszMsg. If the number of 
                                bytes available to return
                                is greater than or equal to
                                cbMsgMax, the output message
                                in lpszMsg is truncated to
                                cbMsgMax-1 characters.


Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails.

Comments: All drivers that make modifications at the time 
ConfigDriver is called with the ODBC_INSTALL_DRIVER option 
should properly delete or uninstall those modifications 
when ConfigDriver is called with the ODBC_REMOVE_DRIVER 
option.

Driver-Specific Options: An application can request driver-
specific features exposed by the driver by using the 
fRequest argument. The fRequest for the first option will
be ODBC_CONFIG_DRIVER_MAX+1, and additional options will
be incremented by 1 from that value. Any arguments required
by the driver for that function should be provided in a
null-terminated string passed in the lpszArgs argument.
Drivers providing such functionality should maintain a
table of driver-specific options. The options should be
fully documented in driver documentation. Application 
writers who make use of driver-specific options should
be aware that this use will make the application less
interoperable.

Messages: A driver setup routine can send a text message  
to an application as null-terminated strings in the lpszMsg 
buffer. The message will be truncated to cbMsgMax-1 
characters by the ConfigDriver function if it is greater 
than or equal to cbMsgMax characters.

         +++++++++++++++SQLConfigDriver+++++++++++++++

Purpose: SQLConfigDriver loads the appropriate driver setup 
DLL and calls the ConfigDriver function.  

Syntax: BOOL SQLConfigDriver (hwndParent, fRequest, lpszDriver, 
lpszArgs, lpszMsg, cbMsgMax, pcbMsgOut)

Type    Argument        Use     Description

HWND    hwndParent      Input   Parent window handle.
WORD    fRequest        Input   Type of request. fRequest must 
                                contain one of the following 
                                values:
                                 ODBC_INSTALL_DRIVER:  
                                  installing a new driver
                                 ODBC_REMOVE_DRIVER:  removing 
                                  a driver
                                This option can also be
                                driver-specific, in which
                                case the first option will
                                be ODBC_CONFIG_DRIVER_MAX+1,
                                and additional options will
                                be incremented by 1 from
                                that value.
LPCSTR  lpszDriver      Input   The name of the driver as 
                                registered in the ODBCINST.INI 
                                key of the registry.
LPCSTR  lpszArgs        Input   A null-terminated string 
                                containing arguments for
                                a driver-specific fRequest.
LPSTR   lpszMsg         Output  A null-terminated string
                                containing an output message
                                from the driver setup.
WORD    cbMsgMax        Input   Length of lpszMsg.
WORD    pcbMsgOut       Output  Total number of bytes 
FAR *                           available to return in
                                lpszMsg. If the number of 
                                bytes available to return
                                is greater than or equal to
                                cbMsgMax, the output message
                                in lpszMsg is truncated to
                                cbMsgMax-1.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails.

Comments: SQLConfigDriver allows an application to call a 
driver’s ConfigDriver routine without having to know the name 
and load the driver-specific setup DLL.  A setup program calls 
this function after the driver setup DLL has been installed.  
The calling program should be aware that this function may not 
be available for all drivers. In such a case, the calling 
program should continue without error.

Driver-Specific Options: An application can request driver-
specific features exposed by the driver by using the 
fRequest argument. The fRequest for the first option will
be ODBC_CONFIG_DRIVER_MAX+1, and additional options will
be incremented by 1 from that value. Any arguments required
by the driver for that function should be provided in a
null-terminated string passed in the lpszArgs argument.
Drivers providing such functionality should maintain a
table of driver-specific options. The options should be
fully documented in driver documentation. Application 
writers who make use of driver-specific options should
be aware that this use will make the application less
interoperable.

Messages: A driver setup routine can send a text message  
to an application as null-terminated strings in the lpszMsg 
buffer. The message will be truncated to cbMsgMax-1 
characters by the ConfigDriver function if it is greater 
than or equal to cbMsgMax characters.

       +++++++++++++++SQLInstallTranslator+++++++++++++++

Purpose: SQLInstallTranslator adds information about a 
translator to the ODBCINST.INI section of the registry 
and increments the translator’s UsageCount by 1. 

Syntax: BOOL SQLInstallTranslator (lpszInfFile, lpszTranslator, 
lpszPathIn, lpszPathOut, cbPathOutMax, pcbPathOut, fRequest, 
lpdwUsageCount)

Type    Argument        Use     Description

LPCSTR  lpszInfFile     Input   Full path of the ODBC.INF file  
                                or a null pointer.  
                                If lpszInfFile is a null 
                                pointer, lpszTranslator must 
                                contain a list of keyword-value 
                                pairs describing the 
                                translator.
LPCSTR  lpszTranslator  Input   If lpszInfFile is the path of 
                                the ODBC.INF file, this must 
                                be the key in the ODBC.INF file
                                that describes the translator.
                                If lpszInfFile is a null
                                pointer, this must contain a 
                                list of keyword-value pairs 
                                describing the translator.
                                The Translator and Setup keywords
                                have to be included in the 
                                lpszTranslator string. The 
                                translation DLL is listed with
                                the Translator keyword, and the
                                translator setup DLL is listed
                                with the Setup keyword.
LPCSTR  lpszPathIn      Input   Full path of where the 
                                translator is to be installed 
                                or a null pointer. If lpszPath 
                                is a null pointer, then the 
                                translators will be installed 
                                in the System directory.
LPSTR   lpszPathOut     Output  The path of where the translator 
                                should be installed.  If the 
                                translator has never been 
                                installed, then lpszPathOut is 
                                the same as lpszPathIn.  If 
                                there exists a prior 
                                installation of the translator, 
                                then lpszPathOut is the path of 
                                the prior installation.
WORD    cbPathOutMax    Input   Length of lpszPathOut.
WORD    pcbPathOut      Output  Total number of bytes 
FAR *                           available to return in 
                                lpszPathOut. If the number of 
                                bytes available to return is
                                greater than or equal to 
                                cbPathOutMax, the output path
                                in lpszPathOut is truncated to
                                pcbPathOutMax-1 characters.
WORD    fRequest        Input   Type of request.  fRequest must 
                                contain one of the following 
                                values:
                                 ODBC_INSTALL_INQUIRY:  inquire 
                                  about where a translator can 
                                  be installed
                                 ODBC_INSTALL_COMPLETE:  
                                  complete the installation 
                                  request
LPDWORD lpdwUsageCount  Output  The usage count of the 
                                translator after this function 
                                has been called.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails.

Comments: SQLInstallTranslator provides a mechanism to install 
just the translator. This function does not actually copy any 
files. The calling program is responsible for copying the 
translator files.

If a version of the translator already exists, but 
the UsageCount value for the translator does not exist, the 
new UsageCount value is set to 2.

Length of the Path in lpszPathOut: SQLInstallTranslator 
allows for a two-phase install process, so an application can 
determine what cbPathOutMax should be by calling 
SQLInstallTranslator with an fRequest of ODBC_INSTALL_INQUIRY 
mode.  This will return the total number of bytes available 
in the pcbPathOut buffer. 
SQLInstallTranslator can then be called with an fRequest of 
ODBC_INSTALL_COMPLETE and the cbPathOutMax argument set to
the value in the pcbPathOut buffer, plus 1.

If you choose not to use the two-phase model for 
SQLInstallTranslator, then you must set cbPathOutMax to 
the value _MAX_PATH, as defined in STDLIB.H, to prevent 
truncation.

When fRequest is ODBC_INSTALL_COMPLETE, SQLInstallTranslator 
does not allow lpszPathOut to be NULL (or cbPathOutMax to 
be 0). If fRequest is ODBC_INSTALL_COMPLETE, FALSE is 
returned when the number of bytes available to return is 
greater than or equal to cbPathOutMax, with the result
that truncation occurs.

        +++++++++++++++SQLRemoveDriver+++++++++++++++

Purpose: SQLRemoveDriver removes information about the driver 
from the ODBCINST.INI registry entry.  

Syntax: BOOL SQLRemoveDriver (lpszDriver, fRemoveDSN, 
lpdwUsageCount)

Type    Argument        Use     Description

LPCSTR  lpszDriver      Input   The name of the driver as 
                                registered in the ODBCINST.INI 
                                key of the registry.
BOOL    fRemoveDSN      Input   The valid values are:
                                TRUE  Remove DSNs associated
                                      with the driver specified
                                      in lpszDriver.
                                FALSE Do not remove DSNs
                                      associated with the
                                      driver specified in
                                      lpszDriver.
LPDWORD lpdwUsageCount  Output  The usage count of the driver 
                                after this function has been 
                                called.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails. If no entry exists in the 
registry when this function is called, the function returns
FALSE.

Comments: SQLRemoveDriver complements the SQLInstallDriver 
function, and updates the UsageCount. This function does not 
actually remove any files. The calling program is responsible 
for deleting files.

SQLRemoveDriver will decrement the UsageCount value by 1. 
If the UsageCount goes to 0, then the following will occur:

1. SQLConfigDriver with the ODBC_REMOVE_DRIVER option will be 
called. If the fRemoveDSN option is set to TRUE, the ConfigDSN
function calls SQLRemoveDSNFromIni to remove all the data 
associated with the driver specified in lpszDriver.
2. The driver registry entry itself will be removed.

      +++++++++++++++SQLRemoveDriverManager+++++++++++++++

Purpose: SQLRemoveDriverManager removes information about the 
ODBC core components from the ODBCINST.INI registry entry.  

Syntax: BOOL SQLRemoveDriverManager (lpdwUsageCount)

Type    Argument        Use     Description

LPDWORD lpdwUsageCount  Output  The usage count of the Driver
                                Manager after this function 
                                has been called.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails. If no entry exists in the 
registry when this function is called, the function returns
FALSE.

Comments: SQLRemoveDriverManager complements the 
SQLInstallDriverManager function, and updates the UsageCount. 
This function does not remove any files. The calling program 
is responsible for deleting files. 

SQLRemoveDriverManager will decrement the UsageCount value 
of the ODBC core components by 1.  If the UsageCount goes to 
0, then the ODBC core component registry entry will be 
removed.

     +++++++++++++++SQLRemoveTranslator+++++++++++++++

Purpose: SQLRemoveTranslator removes information about a 
translator from the ODBCINST.INI section of the registry 
and decrements the translator’s UsageCount by 1.  

Syntax: BOOL SQLRemoveTranslator (lpszTranslator, 
lpdwUsageCount)

Type    Argument        Use     Description

LPCSTR  lpszTranslator  Input   The name of the translator as 
                                registered in the ODBCINST.INI 
                                key of the registry.
LPDWORD lpdwUsageCount  Output  The usage count of the 
                                translator after this function
                                has been called.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails. If no entry exists in the 
registry when this function is called, the function returns
FALSE.

Comments: If the UsageCount goes to 0, then the translator’s 
registry entry will be removed.  This function does not remove 
any files. The calling program is responsible for properly 
removing the translator files.

   +++++++++++++++MODIFIED INSTALLER FUNCTIONS+++++++++++++++

The following installer functions have been modified in 
ODBC 2.5 to support Uninstall and System DSNs. To support full 
backward compatibility, unless otherwise specified, all of the 
modified APIs will support all existing functionality. The 
functions are described in the Uninstall and System DSN 
sections above, and in detail below.

        SQLConfigDataSource
        SQLCreateDataSource
        SQLGetPrivateProfileString
        SQLInstallDriver
        SQLInstallDriverManager
        SQLInstallODBC
        SQLManageDataSources
        SQLWritePrivateProfileString

      +++++++++++++++SQLConfigDataSource+++++++++++++++

SQLConfigDataSource provides the same functionality as in the 
ODBC 2.10 installer, with the addition of the following 
SystemDSN options in the fRequest argument: ODBC_ADD_SYS_DSN, 
ODBC_CONFIG_SYS_DSN, and ODBC_REMOVE_SYS_DSN. These options are
added to enable adding, removing, or changing the configuration 
of a System DSN. The fRequest argument is now as follows:

Type    Argument        Use     Description

HWND    hwndParent      Input   Parent window handle. The
                                function will not display any
                                dialog boxes if the handle is 
                                null. 
WORD    fRequest        Input   Type of request. fRequest must
                                contain one of the following
                                values:
                                ODBC_ADD_DSN: Add new user data
                                     source.
                                ODBC_CONFIG_DSN: Modify an
                                     existing data source. 
                                ODBC_REMOVE_DSN: Remove an
                                     existing data source.
                                ODBC_ADD_SYS_DSN: Add a new 
                                     system data source.
                                ODBC_CONFIG_SYS_DSN: Modify
                                     an existing system data 
                                     source.
                                ODBC_REMOVE_SYS_DSN: Remove
                                     an existing system data
                                     source.
LPCSTR  lpszDriver      Input   Driver description (usually the
                                name of the associated DBMS)
                                presented to users instead of
                                the physical driver name.
LPCSTR  lpszAttributes  Input   List of attributes in the form 
                                of keyword-value pairs. For
                                more information, see ConfigDSN
                                in the Microsoft ODBC 2.10
                                Programmer's Reference.

Returns: The function returns TRUE if it is successful.
It returns FALSE if it fails. If no entry exists in the 
registry when this function is called, the function returns
FALSE.

SQLConfigDataSource maps the system DSN fRequests to the
user DSN fRequests (ODBC_ADD_SYS_DSN to ODBC_ADD_DSN,
ODBC_CONFIG_SYS_DSN to ODBC_CONFIG_DSN, and 
ODBC_REMOVE_SYS_DSN to ODBC_REMOVE_DSN). To distinguish
user and system DSNs, SQLConfigDataSource sets the 
wSystemDSN state variable according to the following table. 
Prior to returning, SQLConfigDataSource resets wSystemDSN 
to BOTHDSN.

  fRequest                 wSystemDSN

  ODBC_ADD_DSN             USERDSN_ONLY
  ODBC_CONFIG_DSN          USERDSN_ONLY
  ODBC_REMOVE_DSN          USERDSN_ONLY
  ODBC_ADD_SYS_DSN         SYSTEMDSN_ONLY
  ODBC_CONFIG_SYS_DSN      SYSTEMDSN_ONLY
  ODBC_REMOVE_SYS_DSN      SYSTEMDSN_ONLY

      +++++++++++++++SQLCreateDataSource+++++++++++++++

SQLCreateDataSource provides the same functionality as in the 
ODBC 2.10 installer, with the addition of a System DSN check
box in the Add Data Source dialog box. This check box allows 
the user to specify whether a user DSN or a system DSN should 
be created. If a driver is chosen with the System DSN check box
selected, SQLCreateDataSource sets the wSystemDSN flag to
SYSTEMDSN_ONLY, and calls ConfigDSN in the driver 
setup DLL with an fRequest of ODBC_ADD_DSN.

The System DSN check box is displayed whenever the 
SQLCreateDataSource function is called to display the Add
Data Source dialog box, except when this dialog box is
displayed through the Control Panel. In this case, a data 
source is added either from the Data Sources dialog box (for
user data sources) or from the System Data Sources dialog box,
so the System DSN check box is not needed.

   +++++++++++++++SQLGetPrivateProfileString+++++++++++++++

This function gets a list of DSN values from the ODBC.INI entry 
in the registry. A wSystemDSN state variable indicates where 
the registry entry is. If the DSN is a User DSN (the state 
variable is USERDSN_ONLY), the function reads from the ODBC.INI 
entry in HKEY_CURRENT_USER. If the DSN is a system DSN 
(SYSTEMDSN_ONLY), the ODBC.INI entry in HKEY_LOCAL_MACHINE is 
used. If the state variable is BOTHDSN, HKEY_CURRENT_USER is 
tried, and if it fails, then HKEY_LOCAL_MACHINE is used.

       +++++++++++++++SQLInstallDriver+++++++++++++++

This function will perform the same functionality as listed in 
the ODBC 2.10 Programmer’s Reference. It will also increment 
the UsageCount for the installed driver by 1.  However, if a 
version of the driver already exists, but the UsageCount value 
for the driver does not exist, the new UsageCount value is set 
to 2. 

    +++++++++++++++SQLInstallDriverManager+++++++++++++++

This function will perform the same functionality as listed in 
the ODBC 2.10 Programmer’s Reference. It will also increment 
the UsageCount for the ODBC core components by 1. However, if 
a version of the Driver Manager already exists, but the 
UsageCount value for the core components does not exist, the 
new UsageCount value is set to 2. 

        +++++++++++++++SQLInstallODBC+++++++++++++++

In addition to performing the same functions it currently does, 
as described in the ODBC 2.10 Programmer's Reference, this 
function will also perform the following:

1. Increment the component usage count of the ODBC core 
components.
2. Increment the component usage count of the drivers being 
installed.
3. Increment the component usage count of the translators being 
installed.
4. Increment the file usage count of each file being installed.
5. Modify the FileList key with the appropriate files (see the 
File Count Tracking section of this readme for more information).

Note that SQLInstallODBC is the only one of the ODBC
installer functions that actually copies files. SQLInstallODBC 
does not support the Uninstall process. If an application's 
setup program uses SQLInstallODBC, the application will not 
be Windows 95 logo compliant. 

SQLInstallODBC performs the same functions as 
SQLInstallDriverManager, SQLInstallDriver, SQLConfigDriver 
(with ODBC_INSTALL_DRIVER), and SQLInstallTranslator. 
SQLInstallODBC does not perform the functions accomplished by 
SQLRemoveTranslator, SQLConfigDriver (with SQL_REMOVE_DRIVER), 
SQLRemoveDriver, and SQLRemoveDriverManager). 

SQLInstallODBC should incorporate the removal functions, and so
support Uninstall and Windows 95 logo compliance, in ODBC 3.0.

       +++++++++++++++SQLManageDataSources+++++++++++++++

SQLManageDataSources has been changed to accommodate the user 
interface changes necessary to support system DSNs (see the 
Administrator/Control Panel Setup section). The function 
provides the same functionality as in the ODBC 2.10 installer, 
with the addition of the SystemDSN button.

   +++++++++++++++SQLWritePrivateProfileString+++++++++++++++

This function writes DSN information to the ODBC.INI entry in 
the registry. A wSystemDSN state variable indicates where the 
registry entry is. If the DSN is a User DSN (the state variable 
is USERDSN_ONLY), the function writes to the ODBC.INI entry in 
HKEY_CURRENT_USER. If the DSN is a system DSN (SYSTEMDSN_ONLY), 
the ODBC.INI entry in HKEY_LOCAL_MACHINE is used. If the state 
variable is BOTHDSN, HKEY_CURRENT_USER is used.

================================================================
                SECTION 3: ODBC FUNCTION CHANGES
================================================================

The following changes have been made to the ODBC functions 
documented in the ODBC 2.10 Programmer's Reference and SDK 
Guide.

    +++++++++++++++REBINDING WITH SQLBindCol+++++++++++++++

An application can call SQLBindCol to bind a column to a new 
storage location, regardless of whether data has already been 
fetched. The new binding replaces the old binding. This is true 
for bookmark columns as well as other bound columns. Note that 
the new binding does not apply to data already fetched--it 
takes effect the next time SQLFetch, SQLExtendedFetch, or 
SQLSetPos is called.

+++++++ATTEMPTING TO ADD TRUNCATED DATA WITH SQLSetPos+++++++

Using SQLSetPos with an fOption of SQL_ADD to add data from
a buffer that was truncated when bound by a call to 
SQLBindCol, will result in a General-Protection Fault. 
Prior to calling SQLSetPos with an fOption of SQL_ADD,
application writers should verify that the value pointed to by 
pcbValue in the accompanying call to SQLBindCol is not greater 
than cbValueMax.

  +++++++++++++++pcbValue IN SQLBindParameter+++++++++++++++

When pcbValue in SQLBindParameter is SQL_DEFAULT_PARAM, the 
corresponding parameter can only be a parameter for an ODBC 
canonical procedure invocation. 

SQLExecDirect, SQLExecute, and SQLPutData return SQLSTATE 07S01 
(Invalid use of default parameter) when a parameter value was 
set to SQL_DEFAULT_PARAM, and the corresponding parameter was 
not a parameter for an ODBC canonical procedure invocation.

+++++++++++++SQLSTATE S1C00 RETURNED BY SQLPrepare+++++++++++++

SQLPrepare will return SQLSTATE S1C00 (Driver not capable) if 
the cursor/concurrency combination is invalid.

+++SQLSTATE 22005 RETURNED BY SQLExtendedFetch and SQLFetch+++

SQLExtendedFetch and SQLFetch will return SQLSTATE 22005 
(Error in assignment) if a zero-length string was inserted
into a string field, and the field was bound to a numeric data
type, so the string was converted to a zero.

+++SQLSTATE 22008 RETURNED BY SQLExtendedFetch and SQLFetch+++

SQLExtendedFetch and SQLFetch will return SQLSTATE 22008 
(Datetime field overflow) if a SQL_C_TIME, SQL_C_DATE, or 
SQL_C_TIMESTAMP value was converted to a SQL_CHAR data type,
and the value was, respectively, an invalid date, time, or
timestamp.

++++++++++++SQLSTATE 22012 RETURNED BY SQLGetData++++++++++++

SQLGetData will return SQLSTATE 22012 (Division by zero) 
if a value from an arithmetic expression was returned that
resulted in division by zero.

+++++++++++CURSOR CONCURRENCY SET IN SQLSetStmtOption++++++++++

The default value for SQL_CONCURRENCY is SQL_CONCUR_READ_ONLY.
This option can also be set through the fConcurrency argument
in SQLSetScrollOptions. This option cannot be specified for
an open cursor.

If the SQL_CURSOR_TYPE fOption is changed to a type that does
not support the current value of SQL_CONCURRENCY, the value of 
SQL_CONCURRENCY is not automatically changed to a supported 
value, and no error will be reported until SQLExecDirect or
SQLPrepare is called.

If the driver supports the SELECT_FOR_UPDATE statement, and such
a statement is executed while the value of SQL_CONCURRENCY is
set to SQL_CONCUR_READ_ONLY, an error will be returned. If the
value of SQL_CONCURRENCY is changed to a value that the driver 
supports for some value of SQL_CURSOR_TYPE, but not for the
current value of SQL_CURSOR_TYPE, the value of SQL_CURSOR_TYPE
is not automatically changed to a supported value, and no error
will be reported until SQLExecDirect or SQLPrepare is called.

     ++++++++++++++ERROR MESSAGE FORMAT+++++++++++++

Error messages returned by the Driver Manager have the
following format: [Microsoft][ODBC Driver Manager] message-text.
The old format was: [Microsoft][ODBC DLL] message-text.

   ++++++++++++++NOTES TO DRIVER WRITERS+++++++++++++

1.  As described in the ODBC 2.0 Programmer's Reference, 
the ODBC 2.0 Driver Manager uses ordinal number 199 to 
determine whether or not to load functions by ordinal or by 
name. If a driver does not define its functions in the 
ordinal order defined by ODBC, it MUST not define ordinal 
number 199. Note that by default, the Microsoft C/C++ compiler 
defines ordinal values for all far functions, not only exported 
functions. This can cause ordinal 199 to be defined even if
there is no function explicitly mapped to that ordinal.

To make sure the compiler doesn't assign ordinal values for 
non-exported functions, include the "PROTMODE" statement in 
the definition (.DEF) file that describes the dynamic-link 
library. For more information, see "Building Drivers: Use 
of PROTMODE Line in .DEF Files" in the ODBC SDK Knowledge 
Base Help file.

2.  An application might occasionally invoke a driver 
connection while responding to a DDE Initiate request from 
another application. If the driver displays a connect dialog 
box and yields to Windows, it may cause unpredictable system 
behavior. Therefore, drivers must call InSendMessage before 
displaying any dialog or message boxes to see if they are 
being called while the application is in SendMessage 
processing. If so, only SYSTEM MODAL dialogs are permitted
so that Windows can block additional messages being sent 
to the applications.

Drivers that support the asynchronous mode of ODBC 
operations MUST not rely upon windows PeekMessage 
functionality to emulate such support. This also applies 
to ALL underlying components (network libraries, any 
asynchronous DDEML based drivers, underlying data access
components, and so on). Any ODBC application that 
implements asynchronous ODBC operations may show unexpected 
behavior and cause the entire Windows system to become 
unstable, if such a driver was used. Therefore, driver 
writers must make sure that implementation of asynchronous
mode does not yield control to Windows.

3.  For driver-specific options (>999), the thunking layer 
checks to see if vParam is a valid pointer. If it is not, 
vParam is passed unchanged as a 32-bit integer. In the case 
where an application expects vParam to be an integer, and 
the value for vParam happened to be a valid pointer, the 
driver will receive an invalid 32-bit integer value.

In addition, if a driver tries to return a pointer in the 
pvParam argument of SQLGetStmtOption for a driver-specific 
option, it will not be translated. Instead of passing back 
a pointer, the driver should copy the associated value back
into the calling application's buffer.

