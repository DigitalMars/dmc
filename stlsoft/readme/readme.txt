================================================================================

File:     readme.txt

Purpose:  Your first port-of-call for information on the STLSoft libraries.

Author:   Matthew Wilson, Synesis Software Pty Ltd.

          Copyright (C) 2002-2004, Synesis Software Pty Ltd.
          All rights reserved.

www:      http://www.synesis.com.au/stlsoft
          http://www.stlsoft.org/

Created:  31st May 2003
Updated:  12th September 2004

================================================================================
================================================================================


================================================================================
Overview:
--------------------------------------------------------------------------------

Hi! I'm Matthew Wilson, and the STLSoft libraries are my creation.

The purpose of the libraries is to make available a lot of technologies that
were formerly the proprietary property of my employer, Synesis Software Pty Ltd.

The ethos of the libraries is, as the STLSoft slogan goes, 

  "... Robust, Lightweight, Cross-Platform, Template Software ..."

Robust because I don't like software that crashes, or is written sloppily.

Lightweight, because I believe that efficiency (speed & size) is *very*
important.

Cross-platform, because I don't like, and rarely use, code that is allied to
only one compiler/technology/operating-system.

Template. Well a lot of STLSoft is template-based, but there's quite a lot that
isn't as well. The original idea for the library came from my having written an
STL-like sequence wrapper class for the Win32 FindFile API, and it dawned on me
how nice it would be to have STL-like facilities for dealing with not just Win32
APIs, but also from the many other areas of C++ software development.

Please feel free to email me at admin@stlsoft.org if you have any feedback; good
or bad is welcome (though abusive mail will result in your address being banned
from the STLSoft servers).

If it's good, please indicate whether you'd be willing to be quoted on a "Users"
page (should I ever get round to putting one up).

If it's bad, please try and be constructive in your criticism. We try to fix
any/all bugs/omissions within a few days, or give you a good indication as to
when such a feature/fix will appear in a forthcoming release.

================================================================================


================================================================================
Installation:
--------------------------------------------------------------------------------

All you need to do is to unzip the distribution into your chosen directory, e.g.
/sdks/stlsoft, and then reference this directory in the configuration for
your compiler, or in the include directories for your project/makefile.

There is no need to compile anything, or set up and directory aliases, or any
other effort on your part. Simply include the files you need, and start working
with the STLSoft library components you need.

================================================================================


================================================================================
Help:
--------------------------------------------------------------------------------

Help is provided in the form of a Compiled HTML Help file (stlsoft.chm), or in
a HTML help format (index.html). The help is located in the help subdirectory of
the stlsoft distribution.

================================================================================


================================================================================
Upgrades:
--------------------------------------------------------------------------------

At any point you can get the latest versions from 
http://stlsoft.org/downloads.html.

As of version 1.6.3, a new, more sensible, directory was adopted, and so you
should be able to simple unzip any new update over the original. Of course,
since the libraries are header-only, you can also wipe your current installation
and unzip the new, without having to worry about losing binaries, etc. (Where
you do do that, however, you would also wipe the test & sample programs, and if
you've edited them then you might want to keep a copy.)

================================================================================


================================================================================
Licensing:
--------------------------------------------------------------------------------

The licensing of all STLSoft library components is according to the BSD
License (http://www.opensource.org/licenses/bsd-license.html), as follows:

 * Home:        http://stlsoft.org/
 *
 * Copyright (c) xxxx-2004, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer. 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the names of
 *   any contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.

Roughly, this means:

 - you can use the software in any commercial/non-commercial products without
   royalty or restriction
 - you cannot claim ownership to the intellectual property of the author's of
   any library components, even where modified. (Of course you would own any
   enhancements, but the original part of the components remain with their
   original author(s).)
 - you cannot use the name of the author(s)/owner(s) in any way without their
   written permission
 - there are NO WARRANTIES OF ANY KIND. If it breaks, is too slow/fast, or in
   any other way fails to satisfy, you have no comeback.

All of this is entirely standard stuff when it comes to free/open-source
software, so you're welcome to forget it and just get on with using the
libraries, and feeding back information on what few bugs you might encounter.

================================================================================


================================================================================
Directory structure:
--------------------------------------------------------------------------------

Assuming you have installed to /sdks/stlsoft, the directory structure (from
version 1.6.3 onwards) will be:

/sdks/stlsoft/  

  Contains all STLSoft and sub-project library files for the current release

/sdks/stlsoft/help/

  Contains the Compiled HTML Help file and all HTML help files for the libraries

/sdks/stlsoft/dmconfig/

  Information files and example configuration files to help you with configuring
  Digital Mars to work with STLSoft (see below)

/sdks/stlsoft/samples/

  Non-trivial sample projects

/sdks/stlsoft/test/stlsoft/

  Test files for the STLSoft main project

/sdks/stlsoft/test/atlstl/

  Test files for the ATLSTL project

. . .

/sdks/stlsoft/test/winstl/

  Test files for the WinSTL project

/sdks/stlsoft/inprogress/

  Contains library files for works in progress, essentially beta release
  components. Files in this directory will likely be incorporated into the next
  release, unless rejected 

/sdks/stlsoft/inprogress-test/stlsoft/

  Test files for the beta release components in the STLSoft main project

. . .

/sdks/stlsoft/inprogress-test/winstl/

  Test files for the beta release components in the WinSTL project


All the test and sample project/makefiles reference the libraries as a relative
directory, as in "../..", so if you move them you'll need to make sure it's on
your include path, or add it to the project/makefile for each test/sample
program.

================================================================================
