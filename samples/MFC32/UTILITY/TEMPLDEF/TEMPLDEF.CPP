// templdef.cpp : A simple C++ template expansion utility.
//
//   This utility takes a template file which uses a syntax
//   subset similar to the proposed C++ template syntax
//   and expands it into the equivalent non-template C++ syntax.
//
//   Note that this is intended as an example program, and
//   handles only a specific syntax in the template files.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include <ctype.h>
#include <afxcoll.h>

#define SIGNON_VER 3
#define SIGNON_REV 0

#define istringTypeParamMax 10

CString strHFileName;
CString strInlFileName;
CString strCppFileName;
CString strCttFileName;

CString stringTemplate = "template";
CString stringClass = "class";
CString stringTemplateName;
CString stringTypedefName;

const char chMoreThanOne = '\0';
const char chNot = '\0';

/////////////////////////////////////////////////////////////////////////////

void Error(const char* szErrorMessage, const char* szErrorParam = NULL)
{
	fprintf(stderr, "templdef : error: ");
	fprintf(stderr, szErrorMessage, szErrorParam);
	fprintf(stderr, ".\n");
	exit(1);
}

void PrintUsage()
{
	fprintf(stderr, "templdef usage (one line):\n\n"
		"  templdef [/Dsymbol]... \"CTemplateName<arg1, arg2> CClassName\"\n"
		"\tinfile.ctt outfile.h outfile.inl outfile.cpp\n"
		"\n"
		"  Multiple /D switches may be used to define symbols processed by $ifdef.\n"
		"\n"
		"  This command expands a template class to be equivalent to:\n"
		"\n"
		"\ttypedef CTemplateName<arg1, arg2> CClassName;\n");
}

void CheckSyntax(BOOL fGoodSyntax, const char* sz)
{
	if (!fGoodSyntax)
		Error("expecting %s", sz);
}

/////////////////////////////////////////////////////////////////////////////
// CTokenFile
// For the purpose of reading source code, this sort of CStdioFile derivative
// proves quite handy.  This could be made really powerful, but this is a
// start.

class CTokenFile : public CStdioFile
{
private:
	enum { ichMax = 1024 };
	char ach[ichMax];
	int ich;
	static CString stringToken;
	BOOL fComment;
	void GetBuf();
	char GetChar() { if (ach[ich] == chNot) GetBuf(); return ach[ich++]; }
	void UnGetChar() { --ich; } // note: really a single char pushback
public:
	char PeekChar() { return ach[ich]; }
	void InitBuf(const char* sz);
	static CString& TokenString() { return stringToken; }
	CTokenFile();
	char GetToken();
	char GetPrintToken();
	char GetTypeToken();
	char GetNameToken();
	void PutToken(char ch) { fputc(ch, m_pStream); }
	void PutToken(const CString& string) { WriteString(string); }
	void PutToken() { WriteString(stringToken); }
	BOOL AtFileEnd() { return ach[0] == chNot; }
	BOOL AtBufEnd() { return ach[ich] == chNot; }
	void SafeOpen(const CString& string, UINT nStyleFlags);
};
CString CTokenFile::stringToken;

CTokenFile::CTokenFile()
{
	ach[0] = '\n';
	ach[1] = chNot;
	ich = 1;
	fComment = FALSE;
};

void CTokenFile::GetBuf()
{
	if (ReadString(ach, ichMax) == NULL)
		ach[0] = chNot;

	ich = 0;
}

void CTokenFile::InitBuf(const char* sz)
{
	strncpy(ach, sz, ichMax);
	ich = 0;
}

void CTokenFile::SafeOpen(const CString& string, UINT nStyleFlags)
{
	BOOL fSuccess = Open(string, nStyleFlags, 0);

	if (!fSuccess)
		Error("can't open file \"%s\"", string);
}

char CTokenFile::GetToken()
{
	if (AtBufEnd())
		GetBuf();

	if (AtFileEnd())
		exit(0);

	fComment = FALSE;

	char ch = GetChar();
	char* pch = stringToken.GetBuffer(1024);
	char* pchInit = pch;
	*pch++ = ch;
	*pch = '\0';

	// assuming this doesn't "really" release the buffer!
	stringToken.ReleaseBuffer(1);

	if (!isalnum(ch) &&
		ch != '/'    &&
		ch != '_'    &&
		ch != '\''   &&
		ch != '"'    &&
		ch != '#'    &&
		ch != '$')
	{
		return ch;
	}

	if ((ch == '\'') || (ch == '"'))
	{
		char ch2;
		while ((ch2 = GetChar()) != ch)
		{
			*pch++ = ch2;
		}
		*pch++ = ch2;
		stringToken.ReleaseBuffer(pch - pchInit);
		return chMoreThanOne;
	}

	if (ch == '/')
	{
		char ch2 = GetChar();
		if (AtFileEnd()) exit(0);
		if (ch2 == '/')
		{
			char ch3 = GetChar();
			if (ch3 != '$')
			{
				UnGetChar();
				*pch++ = '/';
				fComment = TRUE;
				while ((ch3 = GetChar()) != '\n')
					*pch++ = ch3;
			}
			else
			{
				stringToken.ReleaseBuffer(0);
			}
			*pch++ = '\n';
			stringToken.ReleaseBuffer(pch - pchInit);
			return chMoreThanOne;
		}
		else if (ch2 == '*')
		{
			char ch3 = chNot;
			char ch4 = chNot;
			*pch++ = '*';
			fComment = TRUE;
			while (ch4 != '/')
			{
				while ((ch3 = GetChar()) != '*')
					*pch++ = ch3;
				*pch++ = '*';

				ch4 = GetChar();
				if (ch4 != '/')
					UnGetChar();
			}
			*pch++ = '/';
			stringToken.ReleaseBuffer(pch - pchInit);
			return chMoreThanOne;
		}
		else
		{
			UnGetChar();
			return ch;
		}
	}

	if (isdigit(ch))
	{
		char ch2;
		ch2 = GetChar();
		if (!isalpha(ch2))
		{
			UnGetChar();
			return ch;
		}

		*pch++ = ch2;
		stringToken.ReleaseBuffer(pch - pchInit);
		return chMoreThanOne;
	}

	while ((!AtFileEnd()) &&
		(ch = GetChar(), (isalnum(ch) || (ch == '_'))))
	{
		*pch++ = ch;
	}

	if (!AtFileEnd())
		UnGetChar();

	stringToken.ReleaseBuffer(pch - pchInit);
	return chMoreThanOne;
}

char CTokenFile::GetPrintToken()
{
	char chToken;
	while (((chToken = GetToken()) != chMoreThanOne) &&
		   (isspace(chToken)) || fComment)
		/* try again */ ;

	return chToken;
}

char CTokenFile::GetTypeToken()
{
	char chToken = GetPrintToken();
	CString typeStr = stringToken;
	char chFirst = typeStr[0];

	CheckSyntax(
		isalnum(chFirst) ||
		chFirst == '_'   ||
		chFirst == '\''  ||
		chFirst == '"',
		"a type description or constant starting with an\n"
		"alphanumeric or an underbar, or a string or char constant");

	while ((chToken = GetPrintToken()) != ',' && chToken != '>')
	{
		if (chToken != '*' && chToken != '&' && !isdigit(chToken))
			typeStr += ' ';

		typeStr += stringToken;
	}

	stringToken = typeStr;
	return chToken;
}

char CTokenFile::GetNameToken()
{
	GetPrintToken();

	CheckSyntax((isalpha(stringToken[0]) || stringToken[0] == '_'),
		"a name token starting in an alpha char or underbar");

	int l = stringToken.GetLength();
	for (int i=1; i<l; ++i)
	{
		CheckSyntax(isalnum(stringToken[i]) || stringToken[i] == '_',
			"a name token consisting of alphanumerics or underbars");
	}

	CheckSyntax(stringToken != stringTemplate, "a name token");
	CheckSyntax(stringToken != stringClass, "a name token");

	return chMoreThanOne;
}

/////////////////////////////////////////////////////////////////////////////

CMapStringToString map;

BOOL isTailMatch(CString string, CString tail)
{
	BOOL retval = string.Right(tail.GetLength()) == tail;
	ASSERT(retval);
	return retval;
}

CTokenFile fileH;
CTokenFile fileInl;
CTokenFile fileCpp;
CTokenFile fileCtt;

// TranslateTo:
// Copy fileCtt to fileOut, translating words as necessary.
// On the way, swallow things outside of blocks and after
// the "template" word inside of '<' '>' brackets.
// Return if "//$" comment matches pszEnd string
//
// Also, on the way, any $ifdef or $else statements will be
// swallowed as appropriate.
//
void TranslateTo(CTokenFile& fileOut, const char* pszEnd)
{
	char chToken;
	static BOOL fSwallowParams = TRUE;
	BOOL fSwallow = FALSE;
	BOOL finIf = FALSE;
	BOOL finElse = FALSE;
	BOOL fSwallowIf = FALSE;
	BOOL fSwallowElse = FALSE;
	CString stringTokenOut;

	while ((chToken = fileCtt.GetToken()) != chMoreThanOne ||
			(!fileCtt.TokenString().IsEmpty() && fileCtt.TokenString() != pszEnd))
	{
		if (chToken != chMoreThanOne)
		{
			if (chToken == '{')         /* match } */
				fSwallowParams = FALSE;
			if (fSwallowParams && (chToken == '<'))
			{
				while (chToken != '>')
					chToken = fileCtt.GetToken();
				// if template is at end of line eat end of line as well
				if (fileCtt.PeekChar() == '\n')
					fileCtt.GetToken(); // skip the '\n'
			}
			else
			{
				if (!fSwallow)
					fileOut.PutToken();
			}
		}
		else
		{
			if (fileCtt.TokenString()[0] == '$')
			{
				if (fileCtt.TokenString() == "$ifdef")
				{
					fileCtt.GetPrintToken();
					if (map.Lookup(fileCtt.TokenString(), stringTokenOut))
					{
						fSwallowElse = TRUE;
						fSwallowIf = FALSE;
					}
					else
					{
						fSwallowIf = TRUE;
						fSwallowElse = FALSE;
					}

					if (fSwallowIf || fSwallowElse)
					{
						fileCtt.TokenString() = "";
						finIf = TRUE;
					}
					fSwallow = fSwallowIf;
				}
				else if (fileCtt.TokenString() == "$else")
				{
					if (finIf)
					{
						fileCtt.TokenString() = "";
						finIf = FALSE;
						finElse = TRUE;
					}
					fSwallow = fSwallowElse;
				}
				else if (fileCtt.TokenString() == "$endif")
				{
					if (finIf || finElse)
					{
						fileCtt.TokenString() = "";

						// eat any evil junk after the $endif
						while ((chToken = fileCtt.GetToken()) != '\n' &&
							fileCtt.TokenString()[0] != '/')
							/* spin */ ;

						if (chToken != '\n')
							fileCtt.GetToken();
					}
					finIf = FALSE;
					finElse = FALSE;
					fSwallowIf = FALSE;
					fSwallowElse = FALSE;
					fSwallow = FALSE;
				}
			}

			if (fileCtt.TokenString() == stringTemplate)
				fSwallowParams = TRUE;

			if (map.Lookup(fileCtt.TokenString(), stringTokenOut))
			{
				if (!fSwallow)
					fileOut.PutToken(stringTokenOut);
			}
			else
			{
				if (!fSwallow)
					fileOut.PutToken();
			}
		}
	}
}

// main:
// Gets the arguments, checks them, then processes the files.
//
main(int argc, char* argv[])
{
	fprintf(stderr,
		"\nMicrosoft (R) C++ Template Definition Utility   Version %d.%02d\n"
		"Copyright (c) Microsoft Corp. 1992-1995. All rights reserved.\n\n",
		SIGNON_VER, SIGNON_REV);

	// Process any switches
	//
	for (int i=1; i<(argc-3) && argv[i][0] == '/'; ++i)
	{
		switch (argv[i][1])
		{
		case 'D':
			if (argv[i][2] != '\0')
				map.SetAt(&argv[i][2], "1");    // define the symbol as a 1
			break;
		default:
			PrintUsage();
			exit(1);
		}
	}

	// Process template creation arguments
	//
	CString stringTypes;
	for (/*null*/; i<(argc-3); ++i)
		stringTypes += CString(argv[i]) + ' ';

	if (argc < 5)
	{
		PrintUsage();
		exit(1);
	}

	// Copy the template, header and source file name arguments.
	//
	strCttFileName = argv[argc-4];
	strHFileName   = argv[argc-3];
	strInlFileName = argv[argc-2];
	strCppFileName = argv[argc-1];

	// Check to make sure that the args are in the right order.
	if (!isTailMatch(strCttFileName, ".ctt"))
		Error("template file should have a .ctt extension");

	if (!isTailMatch(strCppFileName, ".cpp") &&
		!isTailMatch(strCppFileName, ".cxx"))
		Error("module file should have a .cpp or .cxx extension");

	if (!isTailMatch(strInlFileName, ".inl"))
		Error("inline file should have an .inl extension");

	if (!isTailMatch(strHFileName, ".h") &&
		!isTailMatch(strHFileName, ".hpp") &&
		!isTailMatch(strHFileName, ".hxx"))
		Error("header file should have an .h, .hpp or .hxx extension");

	// Open the files.
	//
	fileCtt.SafeOpen(strCttFileName, CTokenFile::modeRead);
	fileH.SafeOpen(strHFileName,
		CTokenFile::modeWrite | CTokenFile::modeCreate);
	fileInl.SafeOpen(strInlFileName,
		CTokenFile::modeWrite | CTokenFile::modeCreate);
	fileCpp.SafeOpen(strCppFileName,
		CTokenFile::modeWrite | CTokenFile::modeCreate);

	// Push the command line onto the file buffer, so that we can parse it
	// using our standard tool set.
	//
	fileCtt.InitBuf(stringTypes);

	int chToken;
	CString astringTypeParam[istringTypeParamMax];
	int istringTypeParam = 0;

	fileCtt.GetNameToken();
	stringTemplateName = fileCtt.TokenString();

	chToken = fileCtt.GetPrintToken();
	CheckSyntax(chToken == '<', "'<'");

	do
	{
		chToken = fileCtt.GetTypeToken();
		astringTypeParam[istringTypeParam++] = fileCtt.TokenString();
		CheckSyntax(istringTypeParam < istringTypeParamMax,
			"fewer parameterized types (program limit)");
	} while (chToken == ',');

	CheckSyntax(chToken == '>', "'>'");

	fileCtt.GetNameToken();
	stringTypedefName = fileCtt.TokenString();

	map.SetAt(stringTemplate, "");
	map.SetAt(stringTemplateName, stringTypedefName);

	// Done processing the command line part, now eat any initial comments
	// appearing before the //$DECLARE_TEMPLATE flag.
	//
	while ((chToken = fileCtt.GetPrintToken()) != chMoreThanOne ||
		   fileCtt.TokenString() != "DECLARE_TEMPLATE")
	{
		/* spin */ ;
	}

	while ((fileCtt.GetToken() != chMoreThanOne) ||
		   (fileCtt.TokenString() != stringTemplate))
	{
		fileH.PutToken();
	}

	// Token must now be "template".

	// Eat opening '<'.
	//
	chToken = fileCtt.GetPrintToken();

	// Now get a list of type parameters.
	//
	int istringTypeParamMaxPrev = istringTypeParam;
	istringTypeParam = 0;
	while (chToken !='>')
	{
		CString stringParamName;

		// The parameter name is the last thing before a ',' or '>'.
		//
		while ((chToken = fileCtt.GetPrintToken()) != ',' && chToken != '>')
		{
			stringParamName = fileCtt.TokenString();
		}

		map.SetAt(stringParamName, astringTypeParam[istringTypeParam++]);
	}

	CheckSyntax(istringTypeParam == istringTypeParamMaxPrev,
		"same number of template parameters");

	// Copy template to header file, translating words as necessary,
	// terminating when the //$IMPLEMENT_TEMPLATE_INLINES flag is hit.
	TranslateTo(fileH, "IMPLEMENT_TEMPLATE_INLINES");

	// Copy template to inline file, translating words as necessary,
	// terminating when the //$IMPLEMENT_TEMPLATE flag is hit.
	TranslateTo(fileInl, "IMPLEMENT_TEMPLATE");

	// Copy template to source file, translating words as necessary.
	TranslateTo(fileCpp, "END_OF_FILE");

	return 0;
}
