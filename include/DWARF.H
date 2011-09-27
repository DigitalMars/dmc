/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __DWARF_H
#define __DWARF_H 1

/* dwarf.h - defines used in the .debug section of ELF files */

#define	TAG_padding			0x0000
#define	TAG_array_type			0x0001
#define	TAG_class_type			0x0002
#define	TAG_entry_point			0x0003
#define	TAG_enumeration_type		0x0004
#define	TAG_formal_parameter		0x0005
#define	TAG_global_subroutine		0x0006
#define	TAG_global_variable		0x0007
#define	TAG_label			0x000a
#define	TAG_lexical_block		0x000b
#define	TAG_local_variable		0x000c
#define	TAG_member			0x000d
#define	TAG_pointer_type		0x000f
#define	TAG_reference_type		0x0010
#define	TAG_compile_unit		0x0011
#define	TAG_string_type			0x0012
#define	TAG_structure_type		0x0013
#define	TAG_subroutine			0x0014
#define	TAG_subroutine_type		0x0015
#define TAG_typedef			0x0016
#define	TAG_union_type			0x0017
#define	TAG_unspecified_parameters	0x0018
#define	TAG_variant			0x0019
#define	TAG_common_block		0x001a
#define	TAG_common_inclusion		0x001b
#define	TAG_inheritance			0x001c
#define	TAG_inlined_subroutine		0x001d
#define	TAG_module			0x001e
#define	TAG_ptr_to_member_type		0x001f
#define	TAG_set_type			0x0020
#define	TAG_subrange_type		0x0021
#define	TAG_with_stmt			0x0022
#define TAG_NUM 			0x0023
#define	TAG_lo_user			0x8000
#define	TAG_hi_user			0xffff

#define TAG_source_file			TAG_compile_unit

#define	FORM_MASK	0xf
#define DWARF_AT(word) 		(word & ~FORM_MASK)
#define DWARF_FORM(word) 	(word & FORM_MASK)

#define FORM_NONE       0x0
#define FORM_ADDR       0x1
#define FORM_REF        0x2
#define FORM_BLOCK2     0x3
#define FORM_BLOCK4     0x4
#define FORM_DATA2      0x5
#define FORM_DATA4      0x6
#define FORM_DATA8      0x7
#define FORM_STRING     0x8


#define AT_padding              (0x0000|FORM_NONE)
#define AT_sibling              (0x0010|FORM_REF)
#define AT_location             (0x0020|FORM_BLOCK2)
#define AT_name                 (0x0030|FORM_STRING)
#define AT_fund_type            (0x0050|FORM_DATA2)
#define AT_mod_fund_type        (0x0060|FORM_BLOCK2)
#define AT_user_def_type        (0x0070|FORM_REF)
#define AT_mod_u_d_type         (0x0080|FORM_BLOCK2)
#define AT_ordering             (0x0090|FORM_DATA2)
#define AT_subscr_data          (0x00a0|FORM_BLOCK2)
#define AT_byte_size            (0x00b0|FORM_DATA4)
#define AT_bit_offset           (0x00c0|FORM_DATA2)
#define AT_bit_size             (0x00d0|FORM_DATA4)
#define AT_element_list         (0x00f0|FORM_BLOCK4)
#define AT_stmt_list            (0x0100|FORM_DATA4)
#define AT_low_pc               (0x0110|FORM_ADDR)
#define AT_high_pc              (0x0120|FORM_ADDR)
#define AT_language             (0x0130|FORM_DATA4)
#define AT_member               (0x0140|FORM_REF)
#define AT_discr                (0x0150|FORM_REF)
#define AT_discr_value          (0x0160|FORM_BLOCK2)
#define AT_string_length        (0x0190|FORM_BLOCK2)
#define AT_common_reference     (0x01a0|FORM_REF)
#define AT_comp_dir             (0x01b0|FORM_STRING)
#define AT_const_value          0x01c0
#define AT_containing_type      (0x01d0|FORM_REF)
#define AT_default_value        0x01e0
#define AT_friends              (0x01f0|FORM_BLOCK2)
#define AT_inline               (0x0200|FORM_STRING)
#define AT_is_optional          (0x0210|FORM_STRING)
#define AT_lower_bound          0x0220
#define AT_program              (0x0230|FORM_STRING)
#define AT_private              (0x0240|FORM_STRING)
#define AT_producer             (0x0250|FORM_STRING)
#define AT_protected            (0x0260|FORM_STRING)
#define AT_prototyped           (0x0270|FORM_STRING)
#define AT_public               (0x0280|FORM_STRING)
#define AT_pure_virtual         (0x0290|FORM_STRING)
#define AT_return_addr          (0x02a0|FORM_BLOCK2)
#define AT_specification        (0x02b0|FORM_REF)
#define AT_start_scope          (0x02c0|FORM_DATA4)
#define AT_stride_size          (0x02e0|FORM_DATA4)
#define AT_upper_bound          0x02f0
#define AT_virtual              (0x0300|FORM_STRING)
#define AT_NUM			0x0031
#define	AT_lo_user		0x2000
#define	AT_hi_user	 	0x3ff0

#define OP_UNK          0x00
#define OP_REG          0x01
#define OP_BASEREG      0x02
#define OP_ADDR         0x03
#define OP_CONST        0x04
#define OP_DEREF2       0x05
#define OP_DEREF        0x06
#define OP_ADD          0x07
#define OP_NUM		0x08

#define OP_DEREF4	OP_DEREF

#define FT_none                 0x0000
#define FT_char                 0x0001
#define	FT_signed_char		0x0002
#define	FT_unsigned_char	0x0003
#define FT_short                0x0004
#define	FT_signed_short		0x0005
#define	FT_unsigned_short	0x0006
#define FT_integer              0x0007
#define	FT_signed_integer	0x0008
#define	FT_unsigned_integer	0x0009
#define FT_long                 0x000a
#define	FT_signed_long		0x000b
#define	FT_unsigned_long	0x000c
#define FT_pointer              0x000d
#define	FT_float		0x000e
#define	FT_dbl_prec_float	0x000f
#define	FT_ext_prec_float	0x0010
#define	FT_complex		0x0011
#define	FT_dbl_prec_complex	0x0012
#define	FT_void			0x0014
#define	FT_boolean		0x0015
#define	FT_ext_prec_complex	0x0016
#define	FT_label		0x0017
#define FT_NUM			0x0018
#define	FT_lo_user		0x8000
#define	FT_hi_user		0xffff

#define MOD_none                0x00
#define	MOD_pointer_to		0x01
#define	MOD_reference_to	0x02
#define	MOD_const		0x03
#define	MOD_volatile		0x04
#define MOD_NUM			0x05
#define	MOD_lo_user		0x80
#define	MOD_hi_user		0xff

#define FMT_FT                  0
#define FMT_UDT                 1

#define FMT_CONST               0
#define FMT_EXPR                1

#define FMT_FT_C_C              ( (FMT_FT <<2) | (FMT_CONST<<1) | (FMT_CONST) )
#define FMT_FT_C_X              ( (FMT_FT <<2) | (FMT_CONST<<1) | (FMT_EXPR)  )
#define FMT_FT_X_C              ( (FMT_FT <<2) | (FMT_EXPR <<1) | (FMT_CONST) )
#define FMT_FT_X_X              ( (FMT_FT <<2) | (FMT_EXPR <<1) | (FMT_EXPR)  )
#define FMT_UT_C_C              ( (FMT_UDT<<2) | (FMT_CONST<<1) | (FMT_CONST) )
#define FMT_UT_C_X              ( (FMT_UDT<<2) | (FMT_CONST<<1) | (FMT_EXPR)  )
#define FMT_UT_X_C              ( (FMT_UDT<<2) | (FMT_EXPR <<1) | (FMT_CONST) )
#define FMT_UT_X_X              ( (FMT_UDT<<2) | (FMT_EXPR <<1) | (FMT_EXPR)  )

#define FMT_ET          8

#define	ORD_row_major	0
#define ORD_col_major	1
#define ORD_NUM		2

enum LANG {
	LANG_UNK 		= 0,
	LANG_C89 		= 0x0001,
	LANG_C	 		= 0x0002,
	LANG_ADA83 		= 0x0003,
	LANG_C_PLUS_PLUS 	= 0x0004,
	LANG_COBOL74 		= 0x0005,
	LANG_COBOL85 		= 0x0006,
	LANG_FORTRAN77 		= 0x0007,
	LANG_FORTRAN90 		= 0x0008,
	LANG_PASCAL83 		= 0x0009,
	LANG_MODULA2 		= 0x000a,
	LANG_LO_USER		= 0x8000,
	LANG_HI_USER		= 0xffff
};

#define LANG_NUM  		0x000b

#define	LANG_ANSI_C_V1		LANG_C89

#endif
