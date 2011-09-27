/* /////////////////////////////////////////////////////////////////////////////
 * File:        inert_test.cpp
 *
 * Purpose:     Implementation file for the inert_test project.
 *
 * Created:     29th July 2003
 * Updated:     22nd April 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 *              email:      software@synesis.com.au
 *
 *              This source code is placed into the public domain 2003
 *              by Synesis Software Pty Ltd. There are no restrictions
 *              whatsoever to your use of the software.
 *
 *              This source code is provided by Synesis Software Pty Ltd "as is"
 *              and any warranties, whether expressed or implied, including, but
 *              not limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software Pty Ltd be liable for any direct,
 *              indirect, incidental, special, exemplary, or consequential
 *              damages (including, but not limited to, procurement of
 *              substitute goods or services; loss of use, data, or profits; or
 *              business interruption) however caused and on any theory of
 *              liability, whether in contract, strict liability, or tort
 *              (including negligence or otherwise) arising in any way out of
 *              the use of this software, even if advised of the possibility of
 *              such damage.
 *
 *              Neither the name of Synesis Software Pty Ltd nor the names of
 *              any subdivisions, employees or agents of Synesis Software Pty
 *              Ltd, nor the names of any other contributors to this software
 *              may be used to endorse or promote products derived from this
 *              software without specific prior written permission.
 *
 * ////////////////////////////////////////////////////////////////////////// */


#include <stlsoft.h>
#include <stlsoft_inert.h>

stlsoft_ns_using(inert)

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
	/* . */
	inert	x;
	inert	y;

#if 0
	if(x < y)
	{}
#endif /* 0 */

#if 0
	reinterpret_cast<int&>(x);
#endif /* 0 */

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */

#if 0
class R
{
};

union X
	: public R
{
	int		i;
	short	s;
};

class Y
	: public X
{};

class Z
	: public int
{};
#endif /* 0 */
