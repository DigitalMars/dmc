================================================================================

File:     readme.txt

Purpose:  Information for installing the STLSoft libraries for Digital Mars users.

Author:   Matthew Wilson, Synesis Software Pty Ltd.

License:  (Licensed under the Synesis Software Standard Source License)

          Copyright (C) 2002-2004, Synesis Software Pty Ltd.
          All rights reserved.

          www:        http://www.synesis.com.au/stlsoft
                      http://www.stlsoft.org/

          email:      submissions@stlsoft.org  for submissions
                      admin@stlsoft.org        for other enquiries

Created:  2nd June 2003
Updated:  11th June 2004

================================================================================
================================================================================


Almost everything you need to know is in the ..\readme\readme.txt file.

The only thing you might want to do additional to the instructions in that
file would be to add the stlsoft directory to the SC.INI file for your
Digital Mars compiler. If you do that, you do not need to specify the
directory in your project/makefiles.

To do this, you need only add

   "%@P%\..\stlsoft"

to the INCLUDE variable under the Environment section in SC.INI, located in
the BIN directory in your Digital Mars installation. The original might
look something like

  INCLUDE="%@P%\..\include";"%@P%\..\mfc\include";"%@P%\..\stl";%INCLUDE%

and when modified would look like

  INCLUDE="%@P%\..\include";"%@P%\..\mfc\include";"%@P%\..\stl";"%@P%\..\stlsoft";%INCLUDE%

That's it - simple!


================================================================================

