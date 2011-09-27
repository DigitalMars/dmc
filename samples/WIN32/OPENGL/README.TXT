OpenGL(R) 1.1 Release Notes

1.  OpenGL Functionality Notes
2.  Compiler warnings and include files
3.  Redistributable components for Windows 95
4.  Sample Applications


1.  Functionality Notes
-----------------------

The OpenGL API is supported on a variety of graphics hardware; the
software in this release provides support for graphics hardware including
basic emulation on any video adapter that is supported with the operating
system, and accelerated graphics hardware that is supported by an OpenGL
mini-client driver (MCD) or an OpenGL installable client driver (ICD).
This release of OpenGL will run on all supported hardware under 
Windows NT 4.0 and Windows 95, including VGA and Super VGA 16 color mode 
displays, and, for Windows NT, including all RISC platforms.

The OpenGL runtime libraries for Windows NT are bundled with the
NT workstation and server products.  The runtime libraries for Windows 95
are not bundled with the Windows 95 operating system currently, but application
developers may freely redistribute from this SDK along with their applications 
to other Windows 95 systems.  In addition, the Windows 95 libraries have been
bundled with the Windows 95 operating system in the OEM system release 2, so
OEM Windows 95 systems shipping later in 1996 will begin appearing with the
OpenGL runtime libraries included.

To achieve good shaded rendering with OpenGL applications, you should use a 
color graphics mode with 256 or more colors.  Reasonable shading is 
possible for most OpenGL applications with 256 colors.

The Windows NT 4.0 and Windows 95 releases of OpenGL includes new functionality 
and performance enhancements.  These include:

1) A complete implementation of OpenGL 1.1.  OpenGL 1.1 contains several 
functions, including vertex array, polygon offset, logic ops, and several new 
functions for handling textures.  The vertex array and texture calls are 
particularly significant, as they may enable order of magnitude performance 
improvements in some applications.

2) Overlay planes extensions.  These Microsoft OpenGL extensions permit
applications to manage and render into overlay planes where supported
in the graphics hardware.  This permits applications to bring up dialog boxes
and other UI features without overwriting 3D renderings.

3) Extended metafile support.  Applications may encapsulate OpenGL calls and
data in GDI extended metafiles.  This, together with NT 4.0 print spooler 
enhancements for remote metafile rendering, makes it possible to print OpenGL 
graphics at high resolution on the print server.  This feature is limited to 
the Windows NT release.

4) Microsoft extensions.  The Microsoft OpenGL implementation also supports
these performance extensions: GL_WIN_swap_hint, GL_EXT_bgra, and
GL_EXT_paletted_texture.  They improve the performance of some applications
significantly.

5) Performance.  The software renderer has been tuned for this release.  
Performance tuning has been carried out for the front end of the OpenGL 
pipeline as well as for rendering particular primitives, especially 
anti-aliased lines and texturing.  Software rendering is generally 2-4x 
faster.

6) OpenGL hardware acceleration.  This release of OpenGL supports a simpler
mini-client driver (MCD) model to accelerate 3D graphics operations.  In
particular, Windows NT 4.0 includes a Matrox Millennium mini-client driver
that accelerates OpenGL functions.  A corresponding driver for Windows 95
is expected to be available later this year.

2.  Compiler warnings and include files
---------------------------------------

When developing or porting OpenGL source code for Windows NT and Windows 95,
you may notice certain compiler warning messages related to floating point type 
conversions.

Microsoft has recognized "acceptable" type-conversion warnings and has 
provided a means to ignore them in the provided OpenGL sample source files. 
See the included "glos.h" header file.  When using this file, be certain
to include it at an analogous location in your source file.


3. Redistributable components for Windows 95
--------------------------------------------

The OpenGL redistributable components for Windows 95 are in the 
\MSTOOLS\OPENGL95 directory.  Under this is the following subdirectory:

- DLL contains the runtime dynamic-link libraries for OpenGL and GLU.
  We recommend either of two methods for redistributing these libraries
  with your application on Windows 95 (for Windows NT, the libraries are
  bundled with the operating system and should not be redistributed):
  1) In your setup program, install these libraries in the 
     application directory along with your application.  This gives you
     greater control over the version of OpenGL that your application
     will link to (an issue if other applications install other versions
     of the library), but also gives you greater responsibility for 
     updating your customers' libraries if and when that is required
     to address defects, add functionality, improve performance, etc.
  2) In your setup program, install these libraries in the windows
     system directory.  If you do this, you should use the Win32 setup
     API call VerInstallFile to help prevent installing an older version
     of the libraries over another application's installation of a more
     recent version of the libraries.

4.  Sample Applications
-----------------------

a)  Samples included

    BOOK	Sample code from "OpenGL Programming Guide" first edition.

    DEMOS	AUXDEMO  -  AUX Library demonstration.

		BACKTRCE -  BackTrace sample ported from Silicon Graphics Irix/OpenGL.

		GENGL	 -  A Generic Windows NT OpenGL sample application.  This is
			    a great starting point for new programmers learning OpenGL.

                PGONOFFS -  Polygon offsets for efficient translation of objects.

		STONEHNG -  Stonehenge application ported from SGI workstation.

                VASPHERE -  Using vertex arrays for improved performance.

    GLAUX       Source code for our AUXLIB implementation.

    SAMPLES	OpenGL Sample code from SGI.  See SAMPLES\ABOUT.TXT for details.

    SCRSAVE	Source code for OpenGL screen savers.


b)  Disclaimer for Sample Applications that contain SGI copyright notice

ALL RIGHTS RESERVED
Permission to use, copy, modify, and distribute this software for
any purpose and without fee is hereby granted, provided that the above
copyright notice appear in all copies and that both the copyright notice
and this permission notice appear in supporting documentation, and that
the name of Silicon Graphics, Inc. not be used in advertising
or publicity pertaining to distribution of the software without specific,
written prior permission.

THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.

US Government Users Restricted Rights
Use, duplication, or disclosure by the Government is subject to
restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
(c)(1)(ii) of the Rights in Technical Data and Computer Software
clause at DFARS 252.227-7013 and/or in similar or successor
clauses in the FAR or the DOD or NASA FAR Supplement.
Unpublished-- rights reserved under the copyright laws of the
United States.  Contractor/manufacturer is Silicon Graphics,
Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.

OpenGL is a registered trademark of Silicon Graphics, Inc.
Windows is a registered trademark of Microsoft Corporation.
