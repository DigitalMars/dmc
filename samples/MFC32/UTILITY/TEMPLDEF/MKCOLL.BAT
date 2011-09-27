echo "Building all collection shapes"

rem - copy prefix files
copy afxcoll1.h newcoll.h
copy afxcoll1.inl newcoll.inl

REM ////////////////////////////////////////////////////////////////////////
REM -- Arrays

templdef /DIS_SERIAL /DIS_RAW_SERIAL "CArray<BYTE,BYTE> CByteArray" array.ctt temp.h temp.inl array_b.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DIS_SERIAL /DIS_SWAP_SERIAL "CArray<WORD,WORD> CWordArray" array.ctt temp.h temp.inl array_w.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DIS_SERIAL /DIS_SWAP_SERIAL "CArray<DWORD,DWORD> CDWordArray" array.ctt temp.h temp.inl array_d.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef "CArray<UINT,UINT> CUIntArray" array.ctt temp.h temp.inl array_u.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef "CArray<void*,void*> CPtrArray" array.ctt temp.h temp.inl array_p.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DIS_SERIAL /DIS_ARCHIVE_SERIAL "CArray<CObject*,CObject*> CObArray" array.ctt temp.h temp.inl array_o.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM -- String Arrays require special support
templdef /DIS_SERIAL /DIS_ARCHIVE_SERIAL /DHAS_CREATE "CArray<CString,LPCTSTR> CStringArray" array.ctt temp.h temp.inl array_s.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM ////////////////////////////////////////////////////////////////////////
REM -- Lists

templdef /DUSE_ASSIGN "CList<void*,void*> CPtrList" list.ctt temp.h temp.inl list_p.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DUSE_ASSIGN /DIS_SERIAL "CList<CObject*,CObject*> CObList" list.ctt temp.h temp.inl list_o.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM -- String Lists require special support
templdef /DIS_SERIAL /DHAS_CREATE "CList<CString,LPCTSTR> CStringList" list.ctt temp.h temp.inl list_s.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM ////////////////////////////////////////////////////////////////////////
REM -- Maps

templdef /DUSE_ASSIGN /DUSE_ASSIGN_KEY "CMap<WORD,WORD,void*,void*> CMapWordToPtr" map.ctt temp.h temp.inl map_wp.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DUSE_ASSIGN /DUSE_ASSIGN_KEY "CMap<void*,void*,WORD,WORD> CMapPtrToWord" map.ctt temp.h temp.inl map_pw.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DUSE_ASSIGN /DUSE_ASSIGN_KEY "CMap<void*,void*,void*,void*> CMapPtrToPtr" map.ctt temp.h temp.inl map_pp.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DUSE_ASSIGN /DUSE_ASSIGN_KEY /DIS_SERIAL "CMap<WORD,WORD,CObject*,CObject*> CMapWordToOb" map.ctt temp.h temp.inl map_wo.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM ////////////////////////////////////////////////////////////////////////
REM -- Maps from Strings

templdef /DUSE_ASSIGN "CMapStringTo<void*,void*> CMapStringToPtr" map_s.ctt temp.h temp.inl map_sp.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DUSE_ASSIGN /DIS_SERIAL "CMapStringTo<CObject*,CObject*> CMapStringToOb" map_s.ctt temp.h temp.inl map_so.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

templdef /DIS_SERIAL /DHAS_CREATE "CMapStringTo<CString,LPCTSTR> CMapStringToString" map_s.ctt temp.h temp.inl map_ss.cpp
copy newcoll.h+temp.h newcoll.h
copy newcoll.inl+temp.inl newcoll.inl

REM ////////////////////////////////////////////////////////////////////////
REM - Wrap up/Clean up

rem - append suffix files
copy newcoll.h+afxcoll2.h newcoll.h
copy newcoll.inl+afxcoll2.inl newcoll.inl

erase temp.h
erase temp.inl
