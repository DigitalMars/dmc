/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_version_info.h (Originally in MWVerInf.h, ::SynesisWin)
 *
 * Purpose:     Helper for accessing version information.
 *
 * Created:     16th February 1998
 * Updated:     12th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1998-2004, Matthew Wilson and Synesis Software
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
 *
 * ////////////////////////////////////////////////////////////////////////// */


/// \file winstl_version_info.h
///
/// Helper for accessing version information.

#ifndef WINSTL_INCL_H_WINSTL_VERSION_INFO
#define WINSTL_INCL_H_WINSTL_VERSION_INFO

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_VERSION_INFO_MAJOR     3
# define WINSTL_VER_H_WINSTL_VERSION_INFO_MINOR     0
# define WINSTL_VER_H_WINSTL_VERSION_INFO_REVISION  1
# define WINSTL_VER_H_WINSTL_VERSION_INFO_EDIT      62
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"              // stlsoft::sap_cast
#endif /* STLSOFT_INCL_H_STLSOFT_SAP_CAST */
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
# include <stdexcept>                       // std::exception
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
#include <wchar.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_version Version Library
/// \ingroup libraries
/// \brief This library provides facilities for manipulating version APIs

/// \defgroup winstl_version_library Version Library (WinSTL)
/// \ingroup WinSTL libraries_version
/// \brief This library provides facilities for manipulating the Win32 version API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Structure headers
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <int N>
struct hdr_
{
    WORD    wLength;
    WORD    wValueLength;
    WORD    wType;
    WCHAR   szKey[1];
};

typedef hdr_<1> VS_VERSIONINFO_hdr;
typedef hdr_<2> StringFileInfo_hdr;
typedef hdr_<3> VarFileInfo_hdr;
typedef hdr_<4> Var_hdr;
typedef hdr_<5> StringTable_hdr;
typedef hdr_<6> String_hdr;

template<ss_typename_param_k T>
T *rounded_ptr(T *p, ss_size_t n)
{
    union
    {
        T           *p;
        ss_size_t   cb;
    } u;

    u.p = p;

    u.cb = ((n - 1) + u.cb) & ~(n- 1);

    return u.p;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
/// Exception thrown by the version_info class
class version_info_exception
    : public winstl_ns_qual_std(exception)
{
};
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */

/// Represents the fixed part of a version information block
class fixed_file_info
{
    typedef fixed_file_info class_type;

public:
    fixed_file_info(VS_FIXEDFILEINFO const *ffi)
        : m_ffi(ffi)
        , m_fileDateTime(calc_FileDateTime_(ffi))
    {}

public:
    ws_uint16_t ApiVerHigh() const
    {
        return HIWORD(m_ffi->dwStrucVersion);
    }
    ws_uint16_t ApiVerLow() const
    {
        return LOWORD(m_ffi->dwStrucVersion);
    }

    /// The major part of the file version
    ws_uint16_t FileVerMajor() const
    {
        return HIWORD(m_ffi->dwFileVersionMS);
    }
    /// The minor part of the file version
    ws_uint16_t FileVerMinor() const
    {
        return LOWORD(m_ffi->dwFileVersionMS);
    }
    /// The revision part of the file version
    ws_uint16_t FileVerRevision() const
    {
        return HIWORD(m_ffi->dwFileVersionLS);
    }
    /// The build increment part of the file version
    ws_uint16_t FileVerBuild() const
    {
        return LOWORD(m_ffi->dwFileVersionLS);
    }

    /// The major part of the product version
    ws_uint16_t ProductVerMajor() const
    {
        return HIWORD(m_ffi->dwProductVersionMS);
    }
    /// The minor part of the product version
    ws_uint16_t ProductVerMinor() const
    {
        return LOWORD(m_ffi->dwProductVersionMS);
    }
    /// The revision part of the product version
    ws_uint16_t ProductVerRevision() const
    {
        return HIWORD(m_ffi->dwProductVersionLS);
    }
    /// The build increment part of the product version
    ws_uint16_t ProductVerBuild() const
    {
        return LOWORD(m_ffi->dwProductVersionLS);
    }

    /// The file flags mask
    ws_uint32_t FileFlagsMask() const
    {
        return m_ffi->dwFileFlagsMask;
    }
    /// The file flags
    ws_uint32_t FileFlags() const
    {
        return m_ffi->dwFileFlags;
    }

    /// The file operating system
    ws_uint32_t FileOS() const
    {
        return m_ffi->dwFileOS;
    }

    /// The file type
    ws_uint32_t FileType() const
    {
        return m_ffi->dwFileType;
    }
    /// The file subtype
    ws_uint32_t FileSubtype() const
    {
        return m_ffi->dwFileSubtype;
    }

    /// The timestamp of the file
    FILETIME const &FileDateTime() const
    {
        return m_fileDateTime;
    }

private:
    static FILETIME calc_FileDateTime_(VS_FIXEDFILEINFO const *ffi)
    {
        FILETIME    ft = {  ffi->dwFileDateLS, ffi->dwFileDateMS };

        return ft;
    }

private:
    VS_FIXEDFILEINFO const *const   m_ffi;
    FILETIME const                  m_fileDateTime;

private:
    class_type &operator =(class_type const &);
};

/// Represents a variable file part of a version information block
class VsVar
{
public:
    typedef VsVar   class_type;

    /// Represents a language/code-page pair
    struct LangCodePage
    {
        ss_uint16_t language;
        ss_uint16_t codePage;
    };
public:
    VsVar(Var_hdr const *p)
        : m_p(p)
    {
        winstl_assert(0 == wcscmp(p->szKey, L"Translation"));

        m_values = sap_cast<LangCodePage const*>(rounded_ptr(&p->szKey[1 + wcslen(p->szKey)], 4));
    }

    ss_size_t   length() const
    {
        return m_p->wValueLength / sizeof(LangCodePage);
    }

    LangCodePage const  &operator [](ss_size_t index) const
    {
        return m_values[index];
    }

private:
    Var_hdr const       *m_p;
    LangCodePage const  *m_values;
};

/// Represents a string part of a version information block
class VsString
{
public:
    typedef VsString    class_type;

public:
    VsString(String_hdr const *p)
        : m_name(p->szKey)
    {
        m_value =   sap_cast<wchar_t const*>(rounded_ptr(&p->szKey[1 + wcslen(p->szKey)], 2));
    }

    wchar_t const *name() const
    {
        return m_name;
    }
    wchar_t const *value() const
    {
        return m_value;
    }

private:
    wchar_t const   *m_name;
    wchar_t const   *m_value;
};

class VsStringTable
{
public:
    typedef VsStringTable   class_type;

public:
    VsStringTable(StringTable_hdr const *p)
        : m_p(p)
    {
        m_strings = rounded_ptr(&p->szKey[1 + wcslen(p->szKey)], 4);
    }

    wchar_t const *Key() const
    {
        winstl_assert(NULL != m_p);

        return m_p->szKey;
    }

    class const_iterator
    {
    public:
        typedef const_iterator  class_type;

    public:
        const_iterator(void const *p)
            : m_p(p)
        {}

        class_type &operator ++()
        {
            String_hdr const    *str    =   static_cast<String_hdr const *>(m_p);

            m_p = rounded_ptr(ptr_byte_offset(m_p, str->wLength), 4);

            return *this;
        }

        VsString operator *() const
        {
            String_hdr const    *str = static_cast<String_hdr const *>(m_p);

            return VsString(str);
        }

        ss_bool_t operator ==(class_type const &rhs) const
        {
            return m_p == rhs.m_p;
        }

        ss_bool_t operator !=(class_type const &rhs) const
        {
            return !operator ==(rhs);
        }

    private:
        void const  *m_p;
    };

    const_iterator begin() const
    {
        return const_iterator(m_strings);
    }
    const_iterator end() const
    {
        return const_iterator(rounded_ptr(ptr_byte_offset(m_p, m_p->wLength), 4));
    }

private:
    StringTable_hdr const   *m_p;
    void const              *m_strings;
};

/// Represents a variable file info part of a version information block
class VsVarFileInfo
{
public:
    typedef VsVarFileInfo   class_type;

public:
    /// Constructor
    ///
    /// \param p The header of the block for which the instance will act
    VsVarFileInfo(VarFileInfo_hdr const *p)
        : m_p(p)
    {
        winstl_assert(0 == wcscmp(p->szKey, L"VarFileInfo"));

        m_vars = rounded_ptr(&p->szKey[1 + wcslen(p->szKey)], 4);
    }

    /// The Key property
    wchar_t const *Key() const
    {
        winstl_assert(NULL != m_p);

        return m_p->szKey;
    }

    /// Iterator class
    class const_iterator
    {
    public:
        typedef const_iterator  class_type;

    public:
        /// Constructor
        const_iterator(void const *p)
            : m_p(p)
        {}

        /// Pre-increment operator
        class_type &operator ++()
        {
            Var_hdr const   *var = static_cast<Var_hdr const *>(m_p);

            m_p = rounded_ptr(ptr_byte_offset(m_p, var->wLength), 4);

            return *this;
        }

        VsVar operator *() const
        {
            Var_hdr const   *var = static_cast<Var_hdr const *>(m_p);

            return VsVar(var);
        }

        ss_bool_t operator ==(class_type const &rhs) const
        {
            return m_p == rhs.m_p;
        }

        ss_bool_t operator !=(class_type const &rhs) const
        {
            return !operator ==(rhs);
        }

    private:
        const void  *m_p;
    };

    const_iterator begin() const
    {
        return const_iterator(m_vars);
    }
    const_iterator end() const
    {
        return const_iterator(rounded_ptr(ptr_byte_offset(m_p, m_p->wLength), 4));
    }

private:
    VarFileInfo_hdr const   *m_p;
    void const              *m_vars;
};

/// Represents a variable string part of a version information block
class VsStringFileInfo
{
public:
    typedef VsStringFileInfo    class_type;
public:
    /// Constructor
    VsStringFileInfo(StringFileInfo_hdr const *p)
        : m_p(p)
    {
        winstl_assert(0 == wcscmp(p->szKey, L"StringFileInfo"));

        m_vars = rounded_ptr(&p->szKey[1 + wcslen(p->szKey)], 4);
    }

    /// The key of the StringFileInfo block
    wchar_t const *Key() const
    {
        winstl_assert(NULL != m_p);

        return m_p->szKey;
    }

    /// Non-mutating (const) iterator type for the StringFileInfo block
    ///
    /// \note The value type is \c VsStringTable
    class const_iterator
    {
    public:
        /// The class type
        typedef const_iterator  class_type;
        /// The value type
        typedef VsStringTable   value_type;

    public:
        const_iterator(void const *p)
            : m_p(p)
        {}

        /// 
        class_type &operator ++()
        {
            StringTable_hdr const   *strtbl = static_cast<StringTable_hdr const *>(m_p);

            m_p = rounded_ptr(ptr_byte_offset(m_p, strtbl->wLength), 4);

            return *this;
        }

        VsStringTable operator *() const
        {
            StringTable_hdr const   *strtbl = static_cast<StringTable_hdr const *>(m_p);

            return VsStringTable(strtbl);
        }

        ss_bool_t operator ==(class_type const &rhs) const
        {
            return m_p == rhs.m_p;
        }

        ss_bool_t operator !=(class_type const &rhs) const
        {
            return !operator ==(rhs);
        }

    private:
        const void  *m_p;
    };

    const_iterator begin() const
    {
        return const_iterator(m_vars);
    }
    const_iterator end() const
    {
        return const_iterator(rounded_ptr(ptr_byte_offset(m_p, m_p->wLength), 4));
    }

private:
    StringFileInfo_hdr const    *m_p;
    void const                  *m_vars;
};


/// \brief Provides convenient access to aspects of a module's version information
///
class version_info
{
private:
    typedef processheap_allocator<ws_byte_t>    allocator_type;
public:
    typedef version_info                        class_type;

/// \name Construction
/// @{
public:
    /// Creates an instance corresponding to the version information from the given module
    ///
    /// \param moduleName The name of the module (.exe, .dll, etc.) to load
    ss_explicit_k version_info(ws_char_a_t const *moduleName)
        : m_hdr(retrieve_module_info_block_(moduleName))
        , m_key(calc_key_(m_hdr))
        , m_ffi(calc_ffi_(m_key))
        , m_children(calc_children_(m_ffi))
        , m_sfi(NULL)
        , m_vfi(NULL)
    {
        init_();
    }

    /// Creates an instance corresponding to the version information from the given module
    ///
    /// \param moduleName The name of the module (.exe, .dll, etc.) to load
    ss_explicit_k version_info(ws_char_w_t const *moduleName)
        : m_hdr(retrieve_module_info_block_(moduleName))
        , m_key(calc_key_(m_hdr))
        , m_ffi(calc_ffi_(m_key))
        , m_children(calc_children_(m_ffi))
        , m_sfi(NULL)
        , m_vfi(NULL)
    {
        init_();
    }

    /// Releases any allocated resources
    ~version_info()
    {
        allocator_type().deallocate(const_cast<ws_byte_t*>(sap_cast<ws_byte_t const*>(m_hdr)));
    }
/// @}

/// \name Properties
/// @{
public:
    /// The length of the version information
    ws_size_t Length() const
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == m_hdr)
        {
            return 0;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

        return *(sap_cast<WORD const*>(m_hdr) + 0);
    }
    /// The length of the value part of the version block
    ws_size_t ValueLength() const
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == m_hdr)
        {
            return 0;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

        return *(sap_cast<WORD const*>(m_hdr) + 1);
    }
    /// The type field in the version block
    ws_size_t Type() const
    {
        winstl_assert(NULL != m_hdr);

        return *(sap_cast<WORD const*>(m_hdr) + 2);
    }
    /// The key of the version block
    wchar_t const *Key() const
    {
        winstl_assert(NULL != m_hdr);

        return m_key;
    }
    /// The FixedFileInfo part of the block
    fixed_file_info FixedFileInfo() const
    {
        winstl_assert(NULL != m_hdr);

        return fixed_file_info(m_ffi);
    }

    /// Indicates whether the module contains a VarFileInfo block
    ss_bool_t HasVarFileInfo() const
    {
        return NULL != m_vfi;
    }
    /// The VarFileInfo part of the block
    VsVarFileInfo VarFileInfo() const
    {
        winstl_assert(NULL != m_vfi);

        return VsVarFileInfo(m_vfi);
    }

    /// Indicates whether the module contains a StringFileInfo block
    ss_bool_t HasStringFileInfo() const
    {
        return NULL != m_sfi;
    }
    /// The StringFileInfo part of the block
    VsStringFileInfo    StringFileInfo() const
    {
        winstl_assert(NULL != m_sfi);

        return VsStringFileInfo(m_sfi);
    }
/// @}

private:
    static VS_VERSIONINFO_hdr const *retrieve_module_info_block_(ws_char_a_t const *moduleName)
    {
        ws_dword_t  cb  =   ::GetFileVersionInfoSizeA(const_cast<ws_char_a_t*>(moduleName), NULL);
        void        *pv =   (0 == cb) ? NULL : allocator_type().allocate(cb);

#ifndef __STLSOFT_CF_THROW_BAD_ALLOC
        if( 0 == cb ||
            pv == NULL)
        {
            ::GetLastError();

            return NULL;
        }
#endif /* !__STLSOFT_CF_THROW_BAD_ALLOC */

        if( 0 == cb ||
            !::GetFileVersionInfoA(const_cast<ws_char_a_t*>(moduleName), 0, cb, pv))
        {
            allocator_type().deallocate(static_cast<ws_byte_t*>(pv), cb);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
            throw version_info_exception();
#else /* ? __STLSOFT_CF_EXCEPTION_SUPPORT */
            pv = NULL;
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
        }

        return static_cast<VS_VERSIONINFO_hdr*>(pv);
    }

    static VS_VERSIONINFO_hdr const *retrieve_module_info_block_(ws_char_w_t const *moduleName)
    {
        ws_dword_t  cb  =   ::GetFileVersionInfoSizeW(const_cast<ws_char_w_t*>(moduleName), NULL);
        void        *pv =   (0 == cb) ? NULL : allocator_type().allocate(cb);

#ifndef __STLSOFT_CF_THROW_BAD_ALLOC
        if( 0 != cb &&
            pv == NULL)
        {
            return NULL;
        }
#endif /* !__STLSOFT_CF_THROW_BAD_ALLOC */

        if( 0 == cb ||
            !::GetFileVersionInfoW(const_cast<ws_char_w_t*>(moduleName), 0, cb, pv))
        {
            allocator_type().deallocate(static_cast<ws_byte_t*>(pv), cb);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
            throw version_info_exception();
#else /* ? __STLSOFT_CF_EXCEPTION_SUPPORT */
            pv = NULL;
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
        }

        return static_cast<VS_VERSIONINFO_hdr*>(pv);
    }

    static wchar_t const *calc_key_(void const *pv)
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == pv)
        {
            return NULL;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

        wchar_t const *key  =   reinterpret_cast<wchar_t const*>(static_cast<WORD const*>(pv) + 3);

        winstl_assert(0 == wcscmp(L"VS_VERSION_INFO", key));

        return key;
    }

    static VS_FIXEDFILEINFO const *calc_ffi_(wchar_t const *key)
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == key)
        {
            return NULL;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

        return sap_cast<VS_FIXEDFILEINFO const*>(rounded_ptr(&key[1 + wcslen(key)], 4));
    }

    static WORD const *calc_children_(VS_FIXEDFILEINFO const *ffi)
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == ffi)
        {
            return NULL;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

        return sap_cast<WORD const*>(rounded_ptr(&ffi[1], 4));
    }

private:
    void init_()
    {
#if !defined(__STLSOFT_CF_EXCEPTION_SUPPORT) || \
    !defined(__STLSOFT_CF_THROW_BAD_ALLOC)
        if(NULL == m_hdr)
        {
            return;
        }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT || !__STLSOFT_CF_THROW_BAD_ALLOC */

#ifdef _DEBUG
        // Check that ffi is the same as the pointer returned from VerQueryValue("\\");
        VS_FIXEDFILEINFO    *ffi    =   NULL;
        UINT                cchInfo =   0;

        stlsoft_assert(::VerQueryValueA(const_cast<VS_VERSIONINFO_hdr*>(m_hdr), "\\", reinterpret_cast<void**>(&ffi), &cchInfo));
        stlsoft_assert(ffi == m_ffi);
#endif /* _DEBUG */

        // Now we must parse the children.

        void const  *pv = m_children;

        for(; pv != rounded_ptr(ptr_byte_offset(m_hdr, m_hdr->wLength), 4); )
        {
            union
            {
                void const                  *pv_;
                StringFileInfo_hdr const    *psfi;
                VarFileInfo_hdr const       *pvfi;
                ws_size_t                   cb;
            } u;

            u.pv_ = pv;

            winstl_assert(ptr_byte_diff(pv, m_hdr) < m_hdr->wLength);

            if(0 == wcscmp(u.psfi->szKey, L"StringFileInfo"))
            {
                winstl_assert(NULL == m_sfi);

                m_sfi = u.psfi;
            }
            else if(0 == wcscmp(u.psfi->szKey, L"VarFileInfo"))
            {
                winstl_assert(NULL == m_vfi);

                m_vfi = u.pvfi;
            }
            else
            {
                winstl_message_assert("Unexpected contents of VS_VERSIONINFO children", NULL == m_vfi);
            }

            pv = rounded_ptr(ptr_byte_offset(u.pv_, u.psfi->wLength), 4);
        }

        winstl_assert(ptr_byte_diff(pv, m_hdr) == m_hdr->wLength);

#ifdef _DEBUG
        fixed_file_info fixedInfo = FixedFileInfo();

        ws_uint16_t    j   =   fixedInfo.FileVerMajor();
        ws_uint16_t    n   =   fixedInfo.FileVerMinor();
        ws_uint16_t    r   =   fixedInfo.FileVerRevision();
        ws_uint16_t    b   =   fixedInfo.FileVerBuild();

        STLSOFT_SUPPRESS_UNUSED(j);
        STLSOFT_SUPPRESS_UNUSED(n);
        STLSOFT_SUPPRESS_UNUSED(r);
        STLSOFT_SUPPRESS_UNUSED(b);
#endif /* _DEBUG */
    }

private:
    VS_VERSIONINFO_hdr const    *const  m_hdr;
    wchar_t const               *const  m_key;
    VS_FIXEDFILEINFO const      *const  m_ffi;
    WORD const                  *const  m_children;
    StringFileInfo_hdr const            *m_sfi;
    VarFileInfo_hdr const               *m_vfi;

// Not to be implemented
private:
    version_info(class_type const &rhs);
    class_type &operator =(class_type const &rhs);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_version_info(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "version_info", __FILE__);

        version_info            vi1("USER32");

#if 0
        if(NULL != pi1)
        {
            ator1.construct(pi1, 1968);

            if(1968 != *pi1)
            {
                r->report("construct() failed ", __LINE__);
                bSuccess = false;
            }
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_version_info(test_winstl_version_info);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_version_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_VERSION_INFO */

/* ////////////////////////////////////////////////////////////////////////// */
