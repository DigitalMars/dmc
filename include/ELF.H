/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __ELF_H
#define __ELF_H 1

#ifdef	__cplusplus
extern "C" {
#endif

#pragma pack(1)

typedef unsigned long	Elf32_Addr;
typedef unsigned short	Elf32_Half;
typedef unsigned long	Elf32_Off;
typedef long		Elf32_Sword;
typedef unsigned long	Elf32_Word;

#define	ELF32_FSZ_ADDR	4
#define	ELF32_FSZ_HALF	2
#define	ELF32_FSZ_OFF	4
#define	ELF32_FSZ_SWORD	4
#define	ELF32_FSZ_WORD	4

/* ELF header */

#define	EI_NIDENT	16

typedef struct {
	unsigned char	e_ident[EI_NIDENT];	/* ident bytes */
	Elf32_Half	e_type;			/* file type */
	Elf32_Half	e_machine;		/* target machine */
	Elf32_Word	e_version;		/* file version */
	Elf32_Addr	e_entry;		/* start address */
	Elf32_Off	e_phoff;		/* phdr file offset */
	Elf32_Off	e_shoff;		/* shdr file offset */
	Elf32_Word	e_flags;		/* file flags */
	Elf32_Half	e_ehsize;		/* sizeof ehdr */
	Elf32_Half	e_phentsize;		/* sizeof phdr */
	Elf32_Half	e_phnum;		/* number phdrs */
	Elf32_Half	e_shentsize;		/* sizeof shdr */
	Elf32_Half	e_shnum;		/* number shdrs */
	Elf32_Half	e_shstrndx;		/* shdr string index */
} Elf32_Ehdr;

#define	EI_MAG0		0		/* e_ident[] indexes */
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4
#define	EI_DATA		5
#define	EI_VERSION	6
#define	EI_PAD		7

#define	ELFMAG0		0x7f		/* EI_MAG */
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

// Where x = e_ident

#define ISELF(x) (((x[EI_MAG0])==ELFMAG0) &&	\
		  ((x[EI_MAG1])==ELFMAG1) &&	\
                  ((x[EI_MAG2])==ELFMAG2) &&	\
		  ((x[EI_MAG3])==ELFMAG3)) 

#define	ELFCLASSNONE	0		/* EI_CLASS */
#define	ELFCLASS32	1
#define	ELFCLASS64	2
#define	ELFCLASSNUM	3

#define	ELFDATANONE	0		/* EI_DATA */
#define	ELFDATA2LSB	1
#define	ELFDATA2MSB	2
#define	ELFDATANUM	3

#define	ET_NONE		0		/* e_type */
#define	ET_REL		1
#define	ET_EXEC		2
#define	ET_DYN		3
#define	ET_CORE		4
#define	ET_NUM		5

#define	ET_LOPROC	0xff00		/* processor specific range */
#define	ET_HIPROC	0xffff

#define	EM_NONE		0		/* e_machine */
#define	EM_M32		1		/* AT&T WE 32100 */
#define	EM_SPARC	2		/* Sun SPARC */
#define	EM_386		3		/* Intel 80386 */
#define	EM_68K		4		/* Motorola 68000 */
#define	EM_88K		5		/* Motorola 88000 */
#define	EM_486		6		/* Intel 80486 */
#define	EM_860		7		/* Intel i860 */
#define	EM_NUM		8

#define	EV_NONE		0		/* e_version, EI_VERSION */
#define	EV_CURRENT	1
#define	EV_NUM		2

/* Program header */

typedef struct {
	Elf32_Word	p_type;		/* entry type */
	Elf32_Off	p_offset;	/* file offset */
	Elf32_Addr	p_vaddr;	/* virtual address */
	Elf32_Addr	p_paddr;	/* physical address */
	Elf32_Word	p_filesz;	/* file size */
	Elf32_Word	p_memsz;	/* memory size */
	Elf32_Word	p_flags;	/* entry flags */
	Elf32_Word	p_align;	/* memory/file alignment */
} Elf32_Phdr;

#define	PT_NULL		0		/* p_type */
#define	PT_LOAD		1
#define	PT_DYNAMIC	2
#define	PT_INTERP	3
#define	PT_NOTE		4
#define	PT_SHLIB	5
#define	PT_PHDR		6
#define	PT_NUM		7

#define	PT_LOPROC	0x70000000	/* processor specific range */
#define	PT_HIPROC	0x7fffffff

#define	PF_R		0x4		/* p_flags */
#define	PF_W		0x2
#define	PF_X		0x1

#define	PF_MASKPROC	0xf0000000	/* processor specific values */

/* Section header */

typedef struct {
	Elf32_Word	sh_name;	/* section name */
	Elf32_Word	sh_type;	/* SHT_... */
	Elf32_Word	sh_flags;	/* SHF_... */
	Elf32_Addr	sh_addr;	/* virtual address */
	Elf32_Off	sh_offset;	/* file offset */
	Elf32_Word	sh_size;	/* section size */
	Elf32_Word	sh_link;	/* misc info */
	Elf32_Word	sh_info;	/* misc info */
	Elf32_Word	sh_addralign;	/* memory alignment */
	Elf32_Word	sh_entsize;	/* entry size if table */
} Elf32_Shdr;

#define	SHT_NULL	0		/* sh_type */
#define	SHT_PROGBITS	1
#define	SHT_SYMTAB	2
#define	SHT_STRTAB	3
#define	SHT_RELA	4
#define	SHT_HASH	5
#define	SHT_DYNAMIC	6
#define	SHT_NOTE	7
#define	SHT_NOBITS	8
#define	SHT_REL		9
#define	SHT_SHLIB	10
#define	SHT_DYNSYM	11
#define	SHT_NUM		12
#define	SHT_LOUSER	0x80000000
#define	SHT_HIUSER	0xffffffff

#define	SHT_LOPROC	0x70000000	/* processor specific range */
#define	SHT_HIPROC	0x7fffffff

#define	SHF_WRITE	0x1		/* sh_flags */
#define	SHF_ALLOC	0x2
#define	SHF_EXECINSTR	0x4

#define	SHF_MASKPROC	0xf0000000	/* processor specific values */

#define	SHN_UNDEF	0		/* special section numbers */
#define SHN_TEXT        1
#define SHN_DATA        2
#define SHN_BSS         3
#define SHN_COMMENT     4
#define SHN_DEBUG       5
#define SHN_LINE        6
#define SHN_DATAXI      7
#define SHN_DATAXC      8
#define SHN_REL_TEXT    9
#define SHN_REL_DATA    10
#define SHN_SYMTAB      11
#define SHN_STRTAB      12
#define SHN_REL_DEBUG   13
#define SHN_REL_LINE    14
#define SHN_REL_DATAXI  15
#define SHN_REL_DATAXC  16
#define MAX_SCNS        17
#define SHN_REL         SHN_DATAXC
#define SHN_LORESERVE   0xff00
#define SHN_ABS         0xfff1
#define	SHN_COMMON	0xfff2
#define	SHN_HIRESERVE	0xffff

#define	SHN_LOPROC	0xff00		/* processor specific range */
#define	SHN_HIPROC	0xff1f

/* Symbol table */

typedef struct {
	Elf32_Word	st_name;
	Elf32_Addr	st_value;
	Elf32_Word	st_size;
	unsigned char	st_info;	/* bind, type: ELF_32_ST_... */
	unsigned char	st_other;
	Elf32_Half	st_shndx;	/* SHN_... */
} Elf32_Sym;

#define	STN_UNDEF	0

/*	The macros compose and decompose values for S.st_info
 *
 *	bind = ELF32_ST_BIND(S.st_info)
 *	type = ELF32_ST_TYPE(S.st_info)
 *	S.st_info = ELF32_ST_INFO(bind, type)
 */

#define	ELF32_ST_BIND(info)		((info) >> 4)
#define	ELF32_ST_TYPE(info)		((info) & 0xf)
#define	ELF32_ST_INFO(bind, type)	(((bind)<<4)+((type)&0xf))

#define	STB_LOCAL	0		/* BIND */
#define	STB_GLOBAL	1
#define	STB_WEAK	2
#define	STB_NUM		3

#define	STB_LOPROC	13		/* processor specific range */
#define	STB_HIPROC	15

#define	STT_NOTYPE	0		/* TYPE */
#define	STT_OBJECT	1
#define	STT_FUNC	2
#define	STT_SECTION	3
#define	STT_FILE	4
#define	STT_NUM		5

#define	STT_LOPROC	13		/* processor specific range */
#define	STT_HIPROC	15


/* Relocation */

typedef struct {
	Elf32_Addr	r_offset;
	Elf32_Word	r_info;		/* sym, type: ELF32_R_... */
} Elf32_Rel;

typedef struct {
	Elf32_Addr	r_offset;
	Elf32_Word	r_info;		/* sym, type: ELF32_R_... */
	Elf32_Sword	r_addend;
} Elf32_Rela;

/*	The macros compose and decompose values for Rel.r_info, Rela.f_info
 *
 *	sym = ELF32_R_SYM(R.r_info)
 *	type = ELF32_R_TYPE(R.r_info)
 *	R.r_info = ELF32_R_INFO(sym, type)
 */

#define	ELF32_R_SYM(info)	((info)>>8)
#define	ELF32_R_TYPE(info)	((unsigned char)(info))
#define	ELF32_R_INFO(sym, type)	(((sym)<<8)+(unsigned char)(type))


/* Note entry header */

typedef struct {
	Elf32_Word	n_namesz;	/* length of note's name */
	Elf32_Word	n_descsz;	/* length of note's "desc" */
	Elf32_Word	n_type;		/* type of note */
} Elf32_Nhdr;

/* Known values for note entry types (e_type == ET_CORE) */

#define	NT_PRSTATUS	1
#define	NT_PRFPREG	2
#define	NT_PRPSINFO	3


#define	R_386_NONE		0	/* relocation type */
#define	R_386_32		1
#define	R_386_PC32		2
#define	R_386_GOT32		3
#define	R_386_PLT32		4
#define	R_386_COPY		5
#define	R_386_GLOB_DAT		6
#define	R_386_JMP_SLOT		7
#define	R_386_RELATIVE		8
#define	R_386_GOTOFF		9
#define	R_386_GOTPC		10
#define	R_386_NUM		11

#define	ELF_386_MAXPGSZ		0x1000	/* maximum page size */

struct elf_section
{
  Elf32_Shdr *shdr;
  unsigned char *data;
};

union elf_relocation
{
  Elf32_Rel rel;
  Elf32_Rela rela;
};

#define SOURCE_NO_POS   0xffff

struct elf_lineno
{
  Elf32_Word	linenum;
  Elf32_Half    lineposn;       // = SOURCE_NO_POS for whole line
  Elf32_Word	offset;
};

#pragma pack()

#ifdef  __cplusplus
}
#endif

#endif
