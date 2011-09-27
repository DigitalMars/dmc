################################################################################
# File:       makefile
#
# Purpose:    This makefile builds the test programs for the STLSoft libraries
#             that are bundled with Digital Mars.  Use Digital Mars' make.exe to
#             execute
#
# Created:    7th April 2003
# Updated:    8th April 2003
#
# Author:     Matthew Wilson, Synesis Software (c) 2003. All rights reserved.
#
# Copyright:  Copyright of the STLSoft libaries resides with Synesis Software.
#             Copyright of this makefile and associated shell files reside with
#               Digital Mars.
#
# Home:       Digital Mars      -   http://www.digitalmars.com
#             STLSoft           -   http://www.stlsoft.org
#             Synesis Software  -   http://www.synesis.com.au
#
################################################################################

################################################################################
# Commands

# The C compiler
CC            =   dmc.exe
# The C++ compiler
CPPC          =   dmc.exe
# The linker
#LINK          =   link.exe
LINK          =   dmc.exe

# The STLSoft include directory
SSINC         =   ..\

TEST_DIR      =   .
ATLSTL_DIR    =   .\atlstl
COMSTL_DIR    =   .\comstl
MFCSTL_DIR    =   .\mfcstl
STLSOFT_DIR   =   .\stlsoft
UNIXSTL_DIR   =   .\unixstl
WINSTL_DIR    =   .\winstl

# The compilation include paths
CCINC         =   -I$(SS4D_OVERRIDE_INC) -I$(INCLUDE)
CPPCINC       =   -I$(SS4D_OVERRIDE_INC) -I$(SSINC) -I$(INCLUDE)

LNARGS        =   
SYSLIBS       =   kernel32.lib user32.lib gdi32.lib shell32.lib comctl32.lib winmm.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib
#SYSLIBS       =   kernel32.lib user32.lib gdi32.lib comctl32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib

CCARGS        =   -c $(CCINC)
CPPCARGS      =   -c $(CPPCINC) -Ae -D_STLSOFT_FORCE_ANY_COMPILER=1

################################################################################
# Objects

OBJS_ATLSTL  =	\
				        \
                $(ATLSTL_DIR)\property_accessors_test\property_accessors_test.obj \
                $(ATLSTL_DIR)\string_access_test\string_access_test.obj \
                \

OBJS_COMSTL  =	\
				        \
                $(COMSTL_DIR)\allocator_test\allocator_test.obj \
                $(COMSTL_DIR)\bstr_functions_test\bstr_functions_test.obj \
                $(COMSTL_DIR)\enumerator_sequence_test\enumerator_sequence_test.obj \
                $(COMSTL_DIR)\functionals_test\functionals_test.obj \
                $(COMSTL_DIR)\initialisers_test\initialisers_test.obj \
                $(COMSTL_DIR)\interface_cast_test\interface_cast_test.obj \
                $(COMSTL_DIR)\memory_functions_test\memory_functions_test.obj \
                $(COMSTL_DIR)\olestring_functions_test\olestring_functions_test.obj \
                $(COMSTL_DIR)\refcount_functions_test\refcount_functions_test.obj \
                $(COMSTL_DIR)\rot_functions_test\rot_functions_test.obj \
                $(COMSTL_DIR)\security_initialisers_test\security_initialisers_test.obj \
                $(COMSTL_DIR)\string_access_test\string_access_test.obj \
                $(COMSTL_DIR)\thread_marshal_test\client\client.obj \
                $(COMSTL_DIR)\thread_marshal_test\server\MarshalTest.obj \
                $(COMSTL_DIR)\thread_marshal_test\server\server.obj \
                $(COMSTL_DIR)\thread_marshal_test\server\StdAfx.obj \
                \

OBJS_MFCSTL  =	\
				        \
                $(MFCSTL_DIR)\array_adaptor_test\array_adaptor_test.obj \
                $(MFCSTL_DIR)\array_veneer_test\array_veneer_test.obj \
                $(MFCSTL_DIR)\cstring_veneer_test\cstring_veneer_test.obj \
                $(MFCSTL_DIR)\grab_cstring_buffer_test\grab_cstring_buffer_test.obj \
                $(MFCSTL_DIR)\list_adaptor_test\list_adaptor_test.obj \
                $(MFCSTL_DIR)\resource_string_test\resource_string_test.obj \
                $(MFCSTL_DIR)\string_access_test\string_access_test.obj \
                \

OBJS_STLSOFT1 = \
				        \
                $(STLSOFT_DIR)\algorithm_test\algorithm_test.obj \
                $(STLSOFT_DIR)\allocator_test\allocator_test.obj \
                $(STLSOFT_DIR)\associative_container_veneer_test\associative_container_veneer_test.obj \
                $(STLSOFT_DIR)\auto_buffer_test\auto_buffer_test.obj \
                $(STLSOFT_DIR)\auto_destructor_test\auto_destructor_test.obj \
                $(STLSOFT_DIR)\constraints_test\constraints_test.obj \
                $(STLSOFT_DIR)\conversion_veneer_test\conversion_veneer_test.obj \
                $(STLSOFT_DIR)\delete_instance_test\delete_instance_test.obj \
                $(STLSOFT_DIR)\first_class_promoter_test\first_class_promoter_test.obj \
                $(STLSOFT_DIR)\fixed_array_test\fixed_array_test.obj \
                $(STLSOFT_DIR)\frame_array_test\frame_array_test.obj \
                $(STLSOFT_DIR)\frame_string_test\frame_string_test.obj \
                $(STLSOFT_DIR)\inheritance_disambiguator_test\inheritance_disambiguator_test.obj \
                $(STLSOFT_DIR)\integer_to_string_test\integer_to_string_test.obj \
                \

OBJS_STLSOFT2 = \
				        \
                $(STLSOFT_DIR)\mutable_access_test\mutable_access_test.obj \
                $(STLSOFT_DIR)\null_test\null_test.obj \
                $(STLSOFT_DIR)\num_elements_test\num_elements_test.obj \
                $(STLSOFT_DIR)\placement_aid_test\placement_aid_test.obj \
                $(STLSOFT_DIR)\pod_veneer_test\pod_veneer_test.obj \
                $(STLSOFT_DIR)\printf_traits_test\printf_traits_test.obj \
                $(STLSOFT_DIR)\remove_from_scope_test\remove_from_scope_test.obj \
                $(STLSOFT_DIR)\return_value_dtor_test\return_value_dtor_test.obj \
                $(STLSOFT_DIR)\sequence_container_veneer_test\sequence_container_veneer_test.obj \
                \

OBJS_STLSOFT3 = \
				        \
                $(STLSOFT_DIR)\simple_string_test\simple_string_test.obj \
                $(STLSOFT_DIR)\stlsoft_test\stlsoft_test.obj \
                $(STLSOFT_DIR)\string_tokeniser_test\string_tokeniser_test.obj \
                $(STLSOFT_DIR)\true_typedef_test\true_typedef_test.obj \
                $(STLSOFT_DIR)\unordered_sequence_test\unordered_sequence_test.obj \
                \

OBJS_UNIXSTL  = \
				        \
                $(UNIXSTL_DIR)\findfile_sequence_test\findfile_sequence_test.obj \
                $(UNIXSTL_DIR)\performance_counter_test\performance_counter_test.obj \
                $(UNIXSTL_DIR)\readdir_sequence_test\readdir_sequence_test.obj \
                \

OBJS_WINSTL1  = \
				        \
                $(WINSTL_DIR)\absolute_path_test\absolute_path_test.obj \
                $(WINSTL_DIR)\allocator_test\allocator_test.obj \
                $(WINSTL_DIR)\alt_name_test\alt_name_test.obj \
                $(WINSTL_DIR)\combobox_sequence_test\combobox_sequence_test.obj \
                $(WINSTL_DIR)\compare_path_test\compare_path_test.obj \
                $(WINSTL_DIR)\control_inserters_test\control_inserters_test.obj \
                $(WINSTL_DIR)\current_directory_scope_test\current_directory_scope_test.obj \
                $(WINSTL_DIR)\current_directory_test\current_directory_test.obj \
                $(WINSTL_DIR)\file_path_buffer_test\file_path_buffer_test.obj \
                $(WINSTL_DIR)\findfile_sequence_test\findfile_sequence_test.obj \
                $(WINSTL_DIR)\findvolume_sequence_test\findvolume_sequence_test.obj \
                \


OBJS_WINSTL2  = \
				        \
                $(WINSTL_DIR)\heapwalk_sequence_test\heapwalk_sequence_test.obj \
                $(WINSTL_DIR)\listbox_sequence_test\listbox_sequence_test.obj \
                $(WINSTL_DIR)\module_filename_test\module_filename_test.obj \
                $(WINSTL_DIR)\performance_counter_test\performance_counter_test.obj \
                $(WINSTL_DIR)\reg_key_sequence_test\reg_key_sequence_test.obj \
                $(WINSTL_DIR)\reg_key_test\reg_key_test.obj \
                $(WINSTL_DIR)\reg_value_sequence_test\reg_value_sequence_test.obj \
                $(WINSTL_DIR)\searchpath_sequence_test\searchpath_sequence_test.obj \
                $(WINSTL_DIR)\shell_browse_test\shell_browse_test.obj \
                $(WINSTL_DIR)\string_access_test\string_access_test.obj \
                $(WINSTL_DIR)\system_directory_test\system_directory_test.obj \
                \

OBJS_WINSTL3 = \
				        \
                $(WINSTL_DIR)\system_info_test\system_info_test.obj \
                $(WINSTL_DIR)\system_info_test\system_info_test_bug.obj \
                $(WINSTL_DIR)\system_version_test\system_version_test.obj \
                $(WINSTL_DIR)\thread_mutex_test\thread_mutex_test.obj \
                $(WINSTL_DIR)\treeview_sequence_test\treeview_sequence_test.obj \
                $(WINSTL_DIR)\window_access_test\window_access_test.obj \
                $(WINSTL_DIR)\window_enable_scope_test\window_enable_scope_test.obj \
                $(WINSTL_DIR)\window_redraw_scope_test\window_redraw_scope_test.obj \
                $(WINSTL_DIR)\window_update_scope_test\window_update_scope_test.obj \
                $(WINSTL_DIR)\windows_directory_test\windows_directory_test.obj \
                \

# Executables

EXES_ATLSTL  =	\
				        \
                $(ATLSTL_DIR)\property_accessors_test\property_accessors_test.exe \
                $(ATLSTL_DIR)\string_access_test\string_access_test.exe \
                \

EXES_COMSTL  =	\
				        \
                $(COMSTL_DIR)\allocator_test\allocator_test.exe \
                $(COMSTL_DIR)\bstr_functions_test\bstr_functions_test.exe \
                $(COMSTL_DIR)\enumerator_sequence_test\enumerator_sequence_test.exe \
                $(COMSTL_DIR)\functionals_test\functionals_test.exe \
                $(COMSTL_DIR)\initialisers_test\initialisers_test.exe \
                $(COMSTL_DIR)\interface_cast_test\interface_cast_test.exe \
                $(COMSTL_DIR)\memory_functions_test\memory_functions_test.exe \
                $(COMSTL_DIR)\olestring_functions_test\olestring_functions_test.exe \
                $(COMSTL_DIR)\refcount_functions_test\refcount_functions_test.exe \
                $(COMSTL_DIR)\rot_functions_test\rot_functions_test.exe \
                $(COMSTL_DIR)\security_initialisers_test\security_initialisers_test.exe \
                $(COMSTL_DIR)\string_access_test\string_access_test.exe \
                $(COMSTL_DIR)\thread_marshal_test\client\client.exe \
                $(COMSTL_DIR)\thread_marshal_test\server\MarshalTest.exe \
                $(COMSTL_DIR)\thread_marshal_test\server\server.exe \
                $(COMSTL_DIR)\thread_marshal_test\server\StdAfx.exe \
                \

EXES_MFCSTL  =	\
				        \
                $(MFCSTL_DIR)\array_adaptor_test\array_adaptor_test.exe \
                $(MFCSTL_DIR)\array_veneer_test\array_veneer_test.exe \
                $(MFCSTL_DIR)\cstring_veneer_test\cstring_veneer_test.exe \
                $(MFCSTL_DIR)\grab_cstring_buffer_test\grab_cstring_buffer_test.exe \
                $(MFCSTL_DIR)\list_adaptor_test\list_adaptor_test.exe \
                $(MFCSTL_DIR)\resource_string_test\resource_string_test.exe \
                $(MFCSTL_DIR)\string_access_test\string_access_test.exe \
                \

EXES_STLSOFT1 = \
				        \
                $(STLSOFT_DIR)\algorithm_test\algorithm_test.exe \
                $(STLSOFT_DIR)\allocator_test\allocator_test.exe \
                $(STLSOFT_DIR)\associative_container_veneer_test\associative_container_veneer_test.exe \
                $(STLSOFT_DIR)\auto_buffer_test\auto_buffer_test.exe \
                $(STLSOFT_DIR)\auto_destructor_test\auto_destructor_test.exe \
                $(STLSOFT_DIR)\constraints_test\constraints_test.exe \
                $(STLSOFT_DIR)\conversion_veneer_test\conversion_veneer_test.exe \
                $(STLSOFT_DIR)\delete_instance_test\delete_instance_test.exe \
                $(STLSOFT_DIR)\first_class_promoter_test\first_class_promoter_test.exe \
                $(STLSOFT_DIR)\fixed_array_test\fixed_array_test.exe \
                $(STLSOFT_DIR)\frame_array_test\frame_array_test.exe \
                $(STLSOFT_DIR)\frame_string_test\frame_string_test.exe \
                $(STLSOFT_DIR)\inheritance_disambiguator_test\inheritance_disambiguator_test.exe \
                $(STLSOFT_DIR)\integer_to_string_test\integer_to_string_test.exe \
                \

EXES_STLSOFT2 = \
				        \
                $(STLSOFT_DIR)\mutable_access_test\mutable_access_test.exe \
                $(STLSOFT_DIR)\null_test\null_test.exe \
                $(STLSOFT_DIR)\num_elements_test\num_elements_test.exe \
                $(STLSOFT_DIR)\placement_aid_test\placement_aid_test.exe \
                $(STLSOFT_DIR)\pod_veneer_test\pod_veneer_test.exe \
                $(STLSOFT_DIR)\printf_traits_test\printf_traits_test.exe \
                $(STLSOFT_DIR)\remove_from_scope_test\remove_from_scope_test.exe \
                $(STLSOFT_DIR)\return_value_dtor_test\return_value_dtor_test.exe \
                $(STLSOFT_DIR)\sequence_container_veneer_test\sequence_container_veneer_test.exe \
                \

EXES_STLSOFT3 = \
				        \
                $(STLSOFT_DIR)\simple_string_test\simple_string_test.exe \
                $(STLSOFT_DIR)\stlsoft_test\stlsoft_test.exe \
                $(STLSOFT_DIR)\string_tokeniser_test\string_tokeniser_test.exe \
                $(STLSOFT_DIR)\true_typedef_test\true_typedef_test.exe \
                $(STLSOFT_DIR)\unordered_sequence_test\unordered_sequence_test.exe \
                \

EXES_UNIXSTL  = \
				        \
                $(UNIXSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe \
                $(UNIXSTL_DIR)\performance_counter_test\performance_counter_test.exe \
                $(UNIXSTL_DIR)\readdir_sequence_test\readdir_sequence_test.exe \
                \

EXES_WINSTL1  = \
				        \
                $(WINSTL_DIR)\absolute_path_test\absolute_path_test.exe \
                $(WINSTL_DIR)\allocator_test\allocator_test.exe \
                $(WINSTL_DIR)\alt_name_test\alt_name_test.exe \
                $(WINSTL_DIR)\combobox_sequence_test\combobox_sequence_test.exe \
                $(WINSTL_DIR)\compare_path_test\compare_path_test.exe \
                $(WINSTL_DIR)\control_inserters_test\control_inserters_test.exe \
                $(WINSTL_DIR)\current_directory_scope_test\current_directory_scope_test.exe \
                $(WINSTL_DIR)\current_directory_test\current_directory_test.exe \
                $(WINSTL_DIR)\file_path_buffer_test\file_path_buffer_test.exe \
                $(WINSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe \
                $(WINSTL_DIR)\findvolume_sequence_test\findvolume_sequence_test.exe \
                \


EXES_WINSTL2  = \
				        \
                $(WINSTL_DIR)\heapwalk_sequence_test\heapwalk_sequence_test.exe \
                $(WINSTL_DIR)\listbox_sequence_test\listbox_sequence_test.exe \
                $(WINSTL_DIR)\module_filename_test\module_filename_test.exe \
                $(WINSTL_DIR)\performance_counter_test\performance_counter_test.exe \
                $(WINSTL_DIR)\reg_key_sequence_test\reg_key_sequence_test.exe \
                $(WINSTL_DIR)\reg_key_test\reg_key_test.exe \
                $(WINSTL_DIR)\reg_value_sequence_test\reg_value_sequence_test.exe \
                $(WINSTL_DIR)\searchpath_sequence_test\searchpath_sequence_test.exe \
                $(WINSTL_DIR)\shell_browse_test\shell_browse_test.exe \
                $(WINSTL_DIR)\string_access_test\string_access_test.exe \
                $(WINSTL_DIR)\system_directory_test\system_directory_test.exe \
                \

EXES_WINSTL3 = \
				        \
                $(WINSTL_DIR)\system_info_test\system_info_test.exe \
                $(WINSTL_DIR)\system_info_test\system_info_test_bug.exe \
                $(WINSTL_DIR)\system_version_test\system_version_test.exe \
                $(WINSTL_DIR)\thread_mutex_test\thread_mutex_test.exe \
                $(WINSTL_DIR)\treeview_sequence_test\treeview_sequence_test.exe \
                $(WINSTL_DIR)\window_access_test\window_access_test.exe \
                $(WINSTL_DIR)\window_enable_scope_test\window_enable_scope_test.exe \
                $(WINSTL_DIR)\window_redraw_scope_test\window_redraw_scope_test.exe \
                $(WINSTL_DIR)\window_update_scope_test\window_update_scope_test.exe \
                $(WINSTL_DIR)\windows_directory_test\windows_directory_test.exe \
                \


################################################################################
# Suffix rules

.cpp.obj:
  -@echo Compiling $*.cpp
	@$(CPPC) $(CPPCARGS) $*.cpp -o$*.obj
.obj.exe:
	@$(LINK) $@ $** $(LNARGS) $(SYSLIBS) #-L/map:$*.map

################################################################################
# Targets

usage:
	-@echo STLSoft libraries for Digital Mars test suite
	-@echo "USAGE: make [all | target{_atlstl | _comstl | _mfcstl | _stlsoft | _unixstl | _winstl} | usage | clean | run]"

clean:	\
				\
				clean_atlstl \
				clean_comstl \
				clean_mfcstl \
				clean_stlsoft \
				clean_unixstl \
				clean_winstl \
				clean_map \
				\

clean_map:
	-@del *.map 2>NUL

clean_atlstl:
	-@del $(OBJS_ATLSTL) 2>NUL
	-@del $(EXES_ATLSTL) 2>NUL

clean_comstl:
	-@del $(OBJS_COMSTL) 2>NUL
	-@del $(EXES_COMSTL) 2>NUL

clean_mfcstl:
	-@del $(OBJS_MFCSTL) 2>NUL
	-@del $(EXES_MFCSTL) 2>NUL

clean_stlsoft:
	-@del $(OBJS_STLSOFT1) 2>NUL
	-@del $(OBJS_STLSOFT2) 2>NUL
	-@del $(OBJS_STLSOFT3) 2>NUL
	-@del $(EXES_STLSOFT1) 2>NUL
	-@del $(EXES_STLSOFT2) 2>NUL
	-@del $(EXES_STLSOFT3) 2>NUL

clean_unixstl:
	-@del $(OBJS_UNIXSTL) 2>NUL
	-@del $(EXES_UNIXSTL) 2>NUL

clean_winstl:
	-@del $(OBJS_WINSTL1) 2>NUL
	-@del $(OBJS_WINSTL2) 2>NUL
	-@del $(OBJS_WINSTL3) 2>NUL
	-@del $(EXES_WINSTL1) 2>NUL
	-@del $(EXES_WINSTL2) 2>NUL
	-@del $(EXES_WINSTL3) 2>NUL

# A list of all the targets

# all is equal to target for this makefile
all:					  \
								\
								target_stlsoft \
								target_atlstl \
								target_comstl \
								target_mfcstl \
								target_winstl \
								target_unixstl \
								\

target_announce:
  -@echo Making "target"
  -@echo "target" includes STLSoft ("target_stlsoft"), COMSTL ("target_comstl")  and WinSTL("target_winstl")
  -@echo To compile all targets specify "all", but ATL, MFC and Unix headers and MFC libs are needed in INCLUDE and LIB

target:					\
                target_announce \
								\
								target_stlsoft \
								target_comstl \
								target_winstl \
								\

target_atlstl_announce:
  -@echo Making ATLSTL ("target_atlstl") - ATL header directory must be in INCLUDE environment variable

target_atlstl:  \
                target_atlstl_announce \
                \
                $(ATLSTL_DIR)\string_access_test\string_access_test.exe \
                \

#                $(ATLSTL_DIR)\property_accessors_test\property_accessors_test.exe \

target_comstl_announce:
  -@echo Making COMSTL ("target_comstl")

target_comstl:  \
                target_comstl_announce \
                \
                $(COMSTL_DIR)\allocator_test\allocator_test.exe \
                $(COMSTL_DIR)\bstr_functions_test\bstr_functions_test.exe \
                $(COMSTL_DIR)\functionals_test\functionals_test.exe \
                $(COMSTL_DIR)\initialisers_test\initialisers_test.exe \
                $(COMSTL_DIR)\memory_functions_test\memory_functions_test.exe \
                $(COMSTL_DIR)\olestring_functions_test\olestring_functions_test.exe \
                $(COMSTL_DIR)\refcount_functions_test\refcount_functions_test.exe \
                $(COMSTL_DIR)\rot_functions_test\rot_functions_test.exe \
                $(COMSTL_DIR)\security_initialisers_test\security_initialisers_test.exe \
                $(COMSTL_DIR)\string_access_test\string_access_test.exe \
                \

#                $(COMSTL_DIR)\enumerator_sequence_test\enumerator_sequence_test.exe \
#                $(COMSTL_DIR)\interface_cast_test\interface_cast_test.exe \

#                $(COMSTL_DIR)\thread_marshal_test\client\client.exe \
#                $(COMSTL_DIR)\thread_marshal_test\server\MarshalTest.exe \
#                $(COMSTL_DIR)\thread_marshal_test\server\server.exe \
#                $(COMSTL_DIR)\thread_marshal_test\server\StdAfx.exe \

target_mfcstl_announce:
  -@echo Making MFCSTL ("target_mfcstl") - MFC header and lib directories must be in INCLUDE and LIB environment variables

target_mfcstl:  \
                target_mfcstl_announce \
                \
                $(MFCSTL_DIR)\array_adaptor_test\array_adaptor_test.exe \
                $(MFCSTL_DIR)\array_veneer_test\array_veneer_test.exe \
                $(MFCSTL_DIR)\cstring_veneer_test\cstring_veneer_test.exe \
                $(MFCSTL_DIR)\grab_cstring_buffer_test\grab_cstring_buffer_test.exe \
                $(MFCSTL_DIR)\list_adaptor_test\list_adaptor_test.exe \
                $(MFCSTL_DIR)\resource_string_test\resource_string_test.exe \
                $(MFCSTL_DIR)\string_access_test\string_access_test.exe \
                \

target_stlsoft_announce:
  -@echo Making STLSoft main project ("target_stlsoft")

target_stlsoft: \
                target_stlsoft_announce \
                \
                $(STLSOFT_DIR)\algorithm_test\algorithm_test.exe \
                $(STLSOFT_DIR)\allocator_test\allocator_test.exe \
                $(STLSOFT_DIR)\associative_container_veneer_test\associative_container_veneer_test.exe \
                $(STLSOFT_DIR)\auto_buffer_test\auto_buffer_test.exe \
                $(STLSOFT_DIR)\auto_destructor_test\auto_destructor_test.exe \
                $(STLSOFT_DIR)\constraints_test\constraints_test.exe \
                $(STLSOFT_DIR)\delete_instance_test\delete_instance_test.exe \
                $(STLSOFT_DIR)\first_class_promoter_test\first_class_promoter_test.exe \
                $(STLSOFT_DIR)\frame_array_test\frame_array_test.exe \
                $(STLSOFT_DIR)\fixed_array_test\fixed_array_test.exe \
                $(STLSOFT_DIR)\frame_string_test\frame_string_test.exe \
                $(STLSOFT_DIR)\inheritance_disambiguator_test\inheritance_disambiguator_test.exe \
                $(STLSOFT_DIR)\integer_to_string_test\integer_to_string_test.exe \
                $(STLSOFT_DIR)\mutable_access_test\mutable_access_test.exe \
                $(STLSOFT_DIR)\null_test\null_test.exe \
                $(STLSOFT_DIR)\num_elements_test\num_elements_test.exe \
                $(STLSOFT_DIR)\placement_aid_test\placement_aid_test.exe \
                $(STLSOFT_DIR)\pod_veneer_test\pod_veneer_test.exe \
                $(STLSOFT_DIR)\printf_traits_test\printf_traits_test.exe \
                $(STLSOFT_DIR)\remove_from_scope_test\remove_from_scope_test.exe \
                $(STLSOFT_DIR)\return_value_dtor_test\return_value_dtor_test.exe \
                $(STLSOFT_DIR)\sequence_container_veneer_test\sequence_container_veneer_test.exe \
                $(STLSOFT_DIR)\simple_string_test\simple_string_test.exe \
                $(STLSOFT_DIR)\stlsoft_test\stlsoft_test.exe \
                $(STLSOFT_DIR)\string_tokeniser_test\string_tokeniser_test.exe \
                $(STLSOFT_DIR)\true_typedef_test\true_typedef_test.exe \
                $(STLSOFT_DIR)\unordered_sequence_test\unordered_sequence_test.exe \
                \

#                $(STLSOFT_DIR)\conversion_veneer_test\conversion_veneer_test.exe \

target_unixstl_announce:
  -@echo Making UNIXSTL ("target_unixstl") - UNIX-compatible header directory must be in INCLUDE environment variable

target_unixstl: \
                \
                $(UNIXSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe \
                $(UNIXSTL_DIR)\performance_counter_test\performance_counter_test.exe \
                $(UNIXSTL_DIR)\readdir_sequence_test\readdir_sequence_test.exe \
                \

target_winstl_announce:
  -@echo Making WinSTL ("target_winstl")

target_winstl:  \
                target_winstl_announce \
                \
                $(WINSTL_DIR)\absolute_path_test\absolute_path_test.exe \
                $(WINSTL_DIR)\allocator_test\allocator_test.exe \
                $(WINSTL_DIR)\alt_name_test\alt_name_test.exe \
                $(WINSTL_DIR)\combobox_sequence_test\combobox_sequence_test.exe \
                $(WINSTL_DIR)\compare_path_test\compare_path_test.exe \
                $(WINSTL_DIR)\control_inserters_test\control_inserters_test.exe \
                $(WINSTL_DIR)\current_directory_scope_test\current_directory_scope_test.exe \
                $(WINSTL_DIR)\current_directory_test\current_directory_test.exe \
                $(WINSTL_DIR)\file_path_buffer_test\file_path_buffer_test.exe \
                $(WINSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe \
                $(WINSTL_DIR)\heapwalk_sequence_test\heapwalk_sequence_test.exe \
                $(WINSTL_DIR)\listbox_sequence_test\listbox_sequence_test.exe \
                $(WINSTL_DIR)\module_filename_test\module_filename_test.exe \
                $(WINSTL_DIR)\performance_counter_test\performance_counter_test.exe \
                $(WINSTL_DIR)\reg_key_sequence_test\reg_key_sequence_test.exe \
                $(WINSTL_DIR)\reg_key_test\reg_key_test.exe \
                $(WINSTL_DIR)\reg_value_sequence_test\reg_value_sequence_test.exe \
                $(WINSTL_DIR)\searchpath_sequence_test\searchpath_sequence_test.exe \
                $(WINSTL_DIR)\shell_browse_test\shell_browse_test.exe \
                $(WINSTL_DIR)\string_access_test\string_access_test.exe \
                $(WINSTL_DIR)\system_directory_test\system_directory_test.exe \
                $(WINSTL_DIR)\system_info_test\system_info_test.exe \
                $(WINSTL_DIR)\system_info_test\system_info_test_bug.exe \
                $(WINSTL_DIR)\system_version_test\system_version_test.exe \
                $(WINSTL_DIR)\thread_mutex_test\thread_mutex_test.exe \
                $(WINSTL_DIR)\treeview_sequence_test\treeview_sequence_test.exe \
                $(WINSTL_DIR)\window_access_test\window_access_test.exe \
                $(WINSTL_DIR)\window_enable_scope_test\window_enable_scope_test.exe \
                $(WINSTL_DIR)\window_redraw_scope_test\window_redraw_scope_test.exe \
                $(WINSTL_DIR)\window_update_scope_test\window_update_scope_test.exe \
                $(WINSTL_DIR)\windows_directory_test\windows_directory_test.exe \
                \
                \

#                $(WINSTL_DIR)\findvolume_sequence_test\findvolume_sequence_test.exe \



$(ATLSTL_DIR)\property_accessors_test\property_accessors_test.exe : $(ATLSTL_DIR)\property_accessors_test\property_accessors_test.obj
$(ATLSTL_DIR)\string_access_test\string_access_test.exe : $(ATLSTL_DIR)\string_access_test\string_access_test.obj
$(COMSTL_DIR)\allocator_test\allocator_test.exe : $(COMSTL_DIR)\allocator_test\allocator_test.obj
$(COMSTL_DIR)\bstr_functions_test\bstr_functions_test.exe : $(COMSTL_DIR)\bstr_functions_test\bstr_functions_test.obj
$(COMSTL_DIR)\enumerator_sequence_test\enumerator_sequence_test.exe : $(COMSTL_DIR)\enumerator_sequence_test\enumerator_sequence_test.obj
$(COMSTL_DIR)\functionals_test\functionals_test.exe : $(COMSTL_DIR)\functionals_test\functionals_test.obj
$(COMSTL_DIR)\initialisers_test\initialisers_test.exe : $(COMSTL_DIR)\initialisers_test\initialisers_test.obj
$(COMSTL_DIR)\interface_cast_test\interface_cast_test.exe : $(COMSTL_DIR)\interface_cast_test\interface_cast_test.obj
$(COMSTL_DIR)\memory_functions_test\memory_functions_test.exe : $(COMSTL_DIR)\memory_functions_test\memory_functions_test.obj
$(COMSTL_DIR)\olestring_functions_test\olestring_functions_test.exe : $(COMSTL_DIR)\olestring_functions_test\olestring_functions_test.obj
$(COMSTL_DIR)\refcount_functions_test\refcount_functions_test.exe : $(COMSTL_DIR)\refcount_functions_test\refcount_functions_test.obj
$(COMSTL_DIR)\rot_functions_test\rot_functions_test.exe : $(COMSTL_DIR)\rot_functions_test\rot_functions_test.obj
$(COMSTL_DIR)\security_initialisers_test\security_initialisers_test.exe : $(COMSTL_DIR)\security_initialisers_test\security_initialisers_test.obj
$(COMSTL_DIR)\string_access_test\string_access_test.exe : $(COMSTL_DIR)\string_access_test\string_access_test.obj
$(COMSTL_DIR)\thread_marshal_test\client\client.exe : $(COMSTL_DIR)\thread_marshal_test\client\client.obj
$(COMSTL_DIR)\thread_marshal_test\server\MarshalTest.exe : $(COMSTL_DIR)\thread_marshal_test\server\MarshalTest.obj
$(COMSTL_DIR)\thread_marshal_test\server\server.exe : $(COMSTL_DIR)\thread_marshal_test\server\server.obj
$(COMSTL_DIR)\thread_marshal_test\server\StdAfx.exe : $(COMSTL_DIR)\thread_marshal_test\server\StdAfx.obj
$(MFCSTL_DIR)\array_adaptor_test\array_adaptor_test.exe : $(MFCSTL_DIR)\array_adaptor_test\array_adaptor_test.obj
$(MFCSTL_DIR)\array_veneer_test\array_veneer_test.exe : $(MFCSTL_DIR)\array_veneer_test\array_veneer_test.obj
$(MFCSTL_DIR)\cstring_veneer_test\cstring_veneer_test.exe : $(MFCSTL_DIR)\cstring_veneer_test\cstring_veneer_test.obj
$(MFCSTL_DIR)\grab_cstring_buffer_test\grab_cstring_buffer_test.exe : $(MFCSTL_DIR)\grab_cstring_buffer_test\grab_cstring_buffer_test.obj
$(MFCSTL_DIR)\list_adaptor_test\list_adaptor_test.exe : $(MFCSTL_DIR)\list_adaptor_test\list_adaptor_test.obj
$(MFCSTL_DIR)\resource_string_test\resource_string_test.exe : $(MFCSTL_DIR)\resource_string_test\resource_string_test.obj
$(MFCSTL_DIR)\string_access_test\string_access_test.exe : $(MFCSTL_DIR)\string_access_test\string_access_test.obj
$(STLSOFT_DIR)\algorithm_test\algorithm_test.exe : $(STLSOFT_DIR)\algorithm_test\algorithm_test.obj
$(STLSOFT_DIR)\allocator_test\allocator_test.exe : $(STLSOFT_DIR)\allocator_test\allocator_test.obj
$(STLSOFT_DIR)\associative_container_veneer_test\associative_container_veneer_test.exe : $(STLSOFT_DIR)\associative_container_veneer_test\associative_container_veneer_test.obj
$(STLSOFT_DIR)\auto_buffer_test\auto_buffer_test.exe : $(STLSOFT_DIR)\auto_buffer_test\auto_buffer_test.obj
$(STLSOFT_DIR)\auto_destructor_test\auto_destructor_test.exe : $(STLSOFT_DIR)\auto_destructor_test\auto_destructor_test.obj
$(STLSOFT_DIR)\constraints_test\constraints_test.exe : $(STLSOFT_DIR)\constraints_test\constraints_test.obj
$(STLSOFT_DIR)\conversion_veneer_test\conversion_veneer_test.exe : $(STLSOFT_DIR)\conversion_veneer_test\conversion_veneer_test.obj
$(STLSOFT_DIR)\delete_instance_test\delete_instance_test.exe : $(STLSOFT_DIR)\delete_instance_test\delete_instance_test.obj
$(STLSOFT_DIR)\first_class_promoter_test\first_class_promoter_test.exe : $(STLSOFT_DIR)\first_class_promoter_test\first_class_promoter_test.obj
$(STLSOFT_DIR)\fixed_array_test\fixed_array_test.exe : $(STLSOFT_DIR)\fixed_array_test\fixed_array_test.obj
$(STLSOFT_DIR)\frame_array_test\frame_array_test.exe : $(STLSOFT_DIR)\frame_array_test\frame_array_test.obj
$(STLSOFT_DIR)\frame_string_test\frame_string_test.exe : $(STLSOFT_DIR)\frame_string_test\frame_string_test.obj
$(STLSOFT_DIR)\inheritance_disambiguator_test\inheritance_disambiguator_test.exe : $(STLSOFT_DIR)\inheritance_disambiguator_test\inheritance_disambiguator_test.obj
$(STLSOFT_DIR)\integer_to_string_test\integer_to_string_test.exe : $(STLSOFT_DIR)\integer_to_string_test\integer_to_string_test.obj
$(STLSOFT_DIR)\mutable_access_test\mutable_access_test.exe : $(STLSOFT_DIR)\mutable_access_test\mutable_access_test.obj
$(STLSOFT_DIR)\null_test\null_test.exe : $(STLSOFT_DIR)\null_test\null_test.obj
$(STLSOFT_DIR)\num_elements_test\num_elements_test.exe : $(STLSOFT_DIR)\num_elements_test\num_elements_test.obj
$(STLSOFT_DIR)\placement_aid_test\placement_aid_test.exe : $(STLSOFT_DIR)\placement_aid_test\placement_aid_test.obj
$(STLSOFT_DIR)\pod_veneer_test\pod_veneer_test.exe : $(STLSOFT_DIR)\pod_veneer_test\pod_veneer_test.obj
$(STLSOFT_DIR)\printf_traits_test\printf_traits_test.exe : $(STLSOFT_DIR)\printf_traits_test\printf_traits_test.obj
$(STLSOFT_DIR)\remove_from_scope_test\remove_from_scope_test.exe : $(STLSOFT_DIR)\remove_from_scope_test\remove_from_scope_test.obj
$(STLSOFT_DIR)\return_value_dtor_test\return_value_dtor_test.exe : $(STLSOFT_DIR)\return_value_dtor_test\return_value_dtor_test.obj
$(STLSOFT_DIR)\sequence_container_veneer_test\sequence_container_veneer_test.exe : $(STLSOFT_DIR)\sequence_container_veneer_test\sequence_container_veneer_test.obj
$(STLSOFT_DIR)\simple_string_test\simple_string_test.exe : $(STLSOFT_DIR)\simple_string_test\simple_string_test.obj
$(STLSOFT_DIR)\stlsoft_test\stlsoft_test.exe : $(STLSOFT_DIR)\stlsoft_test\stlsoft_test.obj
$(STLSOFT_DIR)\string_tokeniser_test\string_tokeniser_test.exe : $(STLSOFT_DIR)\string_tokeniser_test\string_tokeniser_test.obj
$(STLSOFT_DIR)\true_typedef_test\true_typedef_test.exe : $(STLSOFT_DIR)\true_typedef_test\true_typedef_test.obj
$(STLSOFT_DIR)\unordered_sequence_test\unordered_sequence_test.exe : $(STLSOFT_DIR)\unordered_sequence_test\unordered_sequence_test.obj
$(UNIXSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe : $(UNIXSTL_DIR)\findfile_sequence_test\findfile_sequence_test.obj
$(UNIXSTL_DIR)\performance_counter_test\performance_counter_test.exe : $(UNIXSTL_DIR)\performance_counter_test\performance_counter_test.obj
$(UNIXSTL_DIR)\readdir_sequence_test\readdir_sequence_test.exe : $(UNIXSTL_DIR)\readdir_sequence_test\readdir_sequence_test.obj
$(WINSTL_DIR)\absolute_path_test\absolute_path_test.exe : $(WINSTL_DIR)\absolute_path_test\absolute_path_test.obj
$(WINSTL_DIR)\allocator_test\allocator_test.exe : $(WINSTL_DIR)\allocator_test\allocator_test.obj
$(WINSTL_DIR)\alt_name_test\alt_name_test.exe : $(WINSTL_DIR)\alt_name_test\alt_name_test.obj
$(WINSTL_DIR)\combobox_sequence_test\combobox_sequence_test.exe : $(WINSTL_DIR)\combobox_sequence_test\combobox_sequence_test.obj
$(WINSTL_DIR)\compare_path_test\compare_path_test.exe : $(WINSTL_DIR)\compare_path_test\compare_path_test.obj
$(WINSTL_DIR)\control_inserters_test\control_inserters_test.exe : $(WINSTL_DIR)\control_inserters_test\control_inserters_test.obj
$(WINSTL_DIR)\current_directory_scope_test\current_directory_scope_test.exe : $(WINSTL_DIR)\current_directory_scope_test\current_directory_scope_test.obj
$(WINSTL_DIR)\current_directory_test\current_directory_test.exe : $(WINSTL_DIR)\current_directory_test\current_directory_test.obj
$(WINSTL_DIR)\file_path_buffer_test\file_path_buffer_test.exe : $(WINSTL_DIR)\file_path_buffer_test\file_path_buffer_test.obj
$(WINSTL_DIR)\findfile_sequence_test\findfile_sequence_test.exe : $(WINSTL_DIR)\findfile_sequence_test\findfile_sequence_test.obj
#$(WINSTL_DIR)\findvolume_sequence_test\findvolume_sequence_test.exe : $(WINSTL_DIR)\findvolume_sequence_test\findvolume_sequence_test.obj
$(WINSTL_DIR)\heapwalk_sequence_test\heapwalk_sequence_test.exe : $(WINSTL_DIR)\heapwalk_sequence_test\heapwalk_sequence_test.obj
$(WINSTL_DIR)\listbox_sequence_test\listbox_sequence_test.exe : $(WINSTL_DIR)\listbox_sequence_test\listbox_sequence_test.obj
$(WINSTL_DIR)\module_filename_test\module_filename_test.exe : $(WINSTL_DIR)\module_filename_test\module_filename_test.obj
$(WINSTL_DIR)\performance_counter_test\performance_counter_test.exe : $(WINSTL_DIR)\performance_counter_test\performance_counter_test.obj
$(WINSTL_DIR)\reg_key_sequence_test\reg_key_sequence_test.exe : $(WINSTL_DIR)\reg_key_sequence_test\reg_key_sequence_test.obj
$(WINSTL_DIR)\reg_key_test\reg_key_test.exe : $(WINSTL_DIR)\reg_key_test\reg_key_test.obj
$(WINSTL_DIR)\reg_value_sequence_test\reg_value_sequence_test.exe : $(WINSTL_DIR)\reg_value_sequence_test\reg_value_sequence_test.obj
$(WINSTL_DIR)\searchpath_sequence_test\searchpath_sequence_test.exe : $(WINSTL_DIR)\searchpath_sequence_test\searchpath_sequence_test.obj
$(WINSTL_DIR)\shell_browse_test\shell_browse_test.exe : $(WINSTL_DIR)\shell_browse_test\shell_browse_test.obj
$(WINSTL_DIR)\string_access_test\string_access_test.exe : $(WINSTL_DIR)\string_access_test\string_access_test.obj
$(WINSTL_DIR)\system_directory_test\system_directory_test.exe : $(WINSTL_DIR)\system_directory_test\system_directory_test.obj
$(WINSTL_DIR)\system_info_test\system_info_test.exe : $(WINSTL_DIR)\system_info_test\system_info_test.obj
$(WINSTL_DIR)\system_info_test\system_info_test_bug.exe : $(WINSTL_DIR)\system_info_test\system_info_test_bug.obj
$(WINSTL_DIR)\system_version_test\system_version_test.exe : $(WINSTL_DIR)\system_version_test\system_version_test.obj
$(WINSTL_DIR)\thread_mutex_test\thread_mutex_test.exe : $(WINSTL_DIR)\thread_mutex_test\thread_mutex_test.obj
$(WINSTL_DIR)\treeview_sequence_test\treeview_sequence_test.exe : $(WINSTL_DIR)\treeview_sequence_test\treeview_sequence_test.obj
$(WINSTL_DIR)\window_access_test\window_access_test.exe : $(WINSTL_DIR)\window_access_test\window_access_test.obj
$(WINSTL_DIR)\window_enable_scope_test\window_enable_scope_test.exe : $(WINSTL_DIR)\window_enable_scope_test\window_enable_scope_test.obj
$(WINSTL_DIR)\window_redraw_scope_test\window_redraw_scope_test.exe : $(WINSTL_DIR)\window_redraw_scope_test\window_redraw_scope_test.obj
$(WINSTL_DIR)\window_update_scope_test\window_update_scope_test.exe : $(WINSTL_DIR)\window_update_scope_test\window_update_scope_test.obj
$(WINSTL_DIR)\windows_directory_test\windows_directory_test.exe : $(WINSTL_DIR)\windows_directory_test\windows_directory_test.obj

################################################################################
