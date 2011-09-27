/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __DPMI_H
#define __DPMI_H 1

#if __cplusplus
extern "C"
  {
#endif

#pragma pack()

typedef struct TINFO
  {
  unsigned short	BufferSize;
  void __far *		(*RealModeSaveFunction)(unsigned short Operation,void __far *Buffer);
  void __far *		(*PModeSaveFunction)(unsigned short Operation,void __far *Buffer);
  }
Info_t;

typedef struct TMEMINFO
  {
  unsigned long	LargestContiguousBlock;
  unsigned long	MaxunloackedPageAllocation;
  unsigned long	MaxLoackedPageAllocation;
  unsigned long	PagesOfLinearSpace;
  unsigned long	TotalUnlockedPages;
  unsigned long	TotalFreePages;
  unsigned long	TotalPhysicalPages;
  unsigned long	FreePagesOfLinearSpace;
  unsigned long	SizeOfPagingPartition;
  unsigned long	Reserved[3];
  }
MemInfo_t;

typedef struct TSWITCHTABLE
  {
  unsigned short	newDS;
  unsigned short	newES;
  unsigned short	newSS;
  unsigned short	newCS;
  unsigned long		newIP;	/* upper 16 bits invalid in 16-bit mode */
  unsigned long		newSP;	/* upper 16 bits invalid in 16-bit mode */
  }
SwitchTable_t;

typedef struct TSWITCHFUNCTION
  {
  void __far *	(*RealModeSwitchFunction)(void __far *SwitchTable);
  void __far *	(*PModeSwitchFunction)(void __far *SwitchTable);
  }
SwitchFunction_t;

typedef struct TENVIRON
  {
  unsigned char 	MajorVersion;
  unsigned char 	MinorVersion;
  unsigned short	Flags;
  unsigned char 	ProcessorType;
  unsigned char 	MasterPICInterrupt;
  unsigned char 	SlavePICInterrupt;
  }
Environ_t;

typedef struct TLINEAR
  {
  unsigned long 	Handle;
  unsigned long 	LinearAddress;
  }
Linear_T;

#pragma pack(__DEFALIGN)

/* descriptor management services */
short           dpmi_IsRealMode(void);
short           dpmi_AllocLDTDescriptors(unsigned short count);
short           dpmi_SegToDescriptor(unsigned short segment);
short           dpmi_FreeDescriptor(unsigned short segment);
short           dpmi_GetNextDescriptorIncrement(void);
void __far *     dpmi_GetBaseAddress(unsigned short segment);
short           dpmi_SetBaseAddress(unsigned short segment,void __far *linearaddr);
short           dpmi_SetSegLimit(unsigned short segment,unsigned long linearlen);
short           dpmi_SetDescriptorRights(unsigned short segment,unsigned short rights);
short           dpmi_CreateCodeSegAlias(unsigned short segment);
void __far *     dpmi_GetDescriptor(unsigned short segment,void __far *descriptortable);
void __far *     dpmi_SetDescriptor(unsigned short segment,void __far *descriptortable);
short           dpmi_AllocThisLDTDescriptor(unsigned short segment);

/* dos memory management services */
short		dpmi_DOSAlloc(unsigned short paragraphs,void __far *results);
short		dpmi_DOSFree(unsigned short segment);
short		dpmi_DOSRealloc(unsigned short segment,unsigned short paragraphs,void __far *results);

/* interrupt services */
void __far *	dpmi_GetRealModeVector(unsigned short vector);
short		dpmi_SetRealModeVector(unsigned short vector,void __far *handler);
void __far *	dpmi_GetExceptionHandler(unsigned short exception);
short		dpmi_SetExceptionHandler(unsigned short exception,void __far *handler);
void __far *	dpmi_GetProtectedModeVector(unsigned short exception);
short		dpmi_SetProtectedModeVector(unsigned short exception,void __far *handler);

/* translation services */
void __far *	dpmi_SimRealModeInterrupt(unsigned short vector,unsigned short flags,unsigned short commonstack,void __far *callstruct);
void __far *	dpmi_CallRealModeProcWithFarRet(unsigned short flags,unsigned short commonstack,void __far *callstruct);
void __far *	dpmi_CallRealModeProcWithIRet(unsigned short flags,unsigned short commonstack,void __far *callstruct);
void __far *	dpmi_RealModeCallBackAlloc(void __far *funcptr,void __far *callstruct);
short		dpmi_RealModeCallBackFree(void __far *realmodefuncptr);
short		dpmi_GetStateSaveRestoreInfo(void __far *statesavestruct);
short		dpmi_GetRawModeSwitchInfo(void __far *switchstruct);
short		dpmi_GetVersionInfo(void __far *versionstruct);
short		dpmi_GetFreeMemInfo(void __far *freememstruct);
short		dpmi_MemBlockAlloc(unsigned long size,void __far *newmemstruct);
short		dpmi_MemBlockFree(unsigned long memhandle);
short		dpmi_MemBlockRealloc(unsigned long size,unsigned long memhandle,void __far *newmemstruct);

/* page locking services */
short		dpmi_LockLinearRegion(void __far *regionptr,unsigned long regionsize);
short		dpmi_UnlockLinearRegion(void __far *regionptr,unsigned long regionsize);
short		dpmi_LockRealModeRegion(void __far *regionptr,unsigned long regionsize);
short		dpmi_UnlockRealModeRegion(void __far *regionptr,unsigned long regionsize);
unsigned long	dpmi_IsPageSize(void);

/* demand paging performance tuning */
short		dpmi_MarkAsDemandPage(void __far *regionptr,unsigned long regionsize);
short		dpmi_DiscardPage(void __far *regionptr,unsigned long regionsize);

/* physical address mapping */
short		dpmi_IsLinearAddress(void __far *physicalptr,unsigned long regionsize);

/* virtual interrupt state functions */
short		dpmi_DisableVirtualInterruptState(void);
short		dpmi_EnableVirtualInterruptState(void);
short		dpmi_IsVirtualIntteruptState(void);

/* vendor specific extensions */
void __far *	dpmi_GetVendorAPI(char __far *zstringvendorname);

/* debug support */
short		dpmi_SetDebugWatchPoint(void __far *regionptr,short watchpointsize,short watchpointtype);
short		dpmi_ClearDebugWatchPoint(short watchpointhandle);
short		dpmi_IsDebugWatchPointState(short watchpointhandle);
short		dpmi_ResetDebugWatchPoint(short watchpointhandle);

#if __cplusplus
  }
#endif

#endif
