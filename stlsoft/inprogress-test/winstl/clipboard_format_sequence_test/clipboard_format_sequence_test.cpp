/* /////////////////////////////////////////////////////////////////////////////
 * File:        clipboard_format_sequence_test.cpp
 *
 * Purpose:     Implementation file for the clipboard_format_sequence_test project.
 *
 * Created:     11th May 2003
 * Updated:     30th May 2004
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

#include <windows.h>
#include <stdio.h>

#include <stlsoft.h>

#include <winstl.h>
#include <winstl_clipboard_format_sequence.h>
#include <winstl_error_desc.h>

winstl_ns_using(clipboard_format_sequence)
winstl_ns_using(error_desc_a)

#ifdef _DEBUG
# include <winstl_undocumented.h>
#endif /* _DEBUG */

/* ////////////////////////////////////////////////////////////////////////// */

char const *GetStockClipboardFormatNameA(UINT fmt);

/* ////////////////////////////////////////////////////////////////////////// */

int main(int /* argc */, char ** /*argv*/)
{
#ifdef _DEBUG
	UINT	cf;

	cf = CF_FILENAMEA;
	printf("0x%04x\n", cf);
	cf = CF_FILENAMEW;
	printf("0x%04x\n", cf);
	cf = CF_IDLIST;
	printf("0x%04x\n", cf);
	cf = CF_NETRESOURCE;
	printf("0x%04x\n", cf);
	cf = CF_FILENAMEA;
	printf("0x%04x\n", cf);
	cf = CF_FILENAMEW;
	printf("0x%04x\n", cf);
	cf = CF_IDLIST;
	printf("0x%04x\n", cf);
	cf = CF_NETRESOURCE;
	printf("0x%04x\n", cf);
#endif /* _DEBUG */

	puts("clipboard_format_sequence_test: " __STLSOFT_COMPILER_LABEL_STRING);

	::RegisterClipboardFormat("Something or other");

	/* . */
	clipboard_format_sequence					formats;
	clipboard_format_sequence::const_iterator	begin	=	formats.begin();
	clipboard_format_sequence::const_iterator	end		=	formats.end();

	::RegisterClipboardFormat("Any old iron");

	for(; begin != end; ++begin)
	{
		clipboard_format_sequence::value_type	fmt	=	*begin;
		char									szFmtName[_MAX_PATH + 1];
		char const								*pszFmtName;

		pszFmtName = GetStockClipboardFormatNameA(fmt);

		if(NULL == pszFmtName)
		{
			if(::GetClipboardFormatNameA(fmt, szFmtName, stlsoft_num_elements(szFmtName)))
			{
				pszFmtName = szFmtName;
			}
		}

		if(NULL == pszFmtName)
		{
			sprintf(szFmtName, "not named (%s)", error_desc_a().get_description());
			pszFmtName = szFmtName;
		}

		printf("%u/0x%04x => %s\n", fmt, fmt, pszFmtName);
	}

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */

char const *GetStockClipboardFormatNameA(UINT fmt)
{
	switch(fmt)
	{
#define	ENTRY_(f)	case	f:	return #f;

		ENTRY_(CF_TEXT)
		ENTRY_(CF_BITMAP)
		ENTRY_(CF_METAFILEPICT)
		ENTRY_(CF_SYLK)
		ENTRY_(CF_DIF)
		ENTRY_(CF_TIFF)
		ENTRY_(CF_OEMTEXT)
		ENTRY_(CF_DIB)
		ENTRY_(CF_PALETTE)
		ENTRY_(CF_PENDATA)
		ENTRY_(CF_RIFF)
		ENTRY_(CF_WAVE)
		ENTRY_(CF_UNICODETEXT)
		ENTRY_(CF_ENHMETAFILE)
#if(WINVER >= 0x0400)
		ENTRY_(CF_HDROP)
		ENTRY_(CF_LOCALE)
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0500)
		ENTRY_(CF_DIBV5)
#endif /* WINVER >= 0x0500 */
		ENTRY_(CF_OWNERDISPLAY)
		ENTRY_(CF_DSPTEXT)
		ENTRY_(CF_DSPBITMAP)
		ENTRY_(CF_DSPMETAFILEPICT)
		ENTRY_(CF_DSPENHMETAFILE)
		default:
			return NULL;
	}
}

/* ////////////////////////////////////////////////////////////////////////// */
