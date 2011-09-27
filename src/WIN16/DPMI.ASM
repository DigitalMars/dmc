;_ dpmi.asm
; Copyright (C) 1991 Walter Bright
; All Rights Reserved
; Written by G. Eric Engstrom

include macros.asm

	begdata
	enddata
 
	begcode dpmi

	c_public dpmi_AllocLDTDescriptors
func	dpmi_AllocLDTDescriptors
	WINENTER
	mov	CX,P[BP]
	clr	AX
dpmi_do:
	dpmi
	jnc	ALDTD0
	  clr	  AX		; on error return -1
	  dec	  AX
ALDTD0:	WINLEAVE
	ret
c_endp	dpmi_AllocLDTDescriptors

	c_public dpmi_SegToDescriptor
func	dpmi_SegToDescriptor
	WINENTER
	mov	AX,2
dpmi_do_bp:
	mov	BX,P[BP]
	jmp	short dpmi_do
c_endp	dpmi_SegToDescriptor

	c_public dpmi_GetNextDescriptorIncrement
func	dpmi_GetNextDescriptorIncrement
	WINENTER
	mov	AX,3
	jmp	short dpmi_do
c_endp	dpmi_GetNextDescriptorIncrement

	c_public dpmi_SetDescriptorRights
func	dpmi_SetDescriptorRights
	WINENTER
	mov	AX,9
	mov	CX,P+2[BP]
	jmp	dpmi_do_bp
c_endp	dpmi_SetDescriptorRights

	c_public dpmi_CreateCodeSegAlias
func	dpmi_CreateCodeSegAlias
	WINENTER
	mov	AX,0Ah
	jmp	dpmi_do_bp
c_endp	dpmi_CreateCodeSegAlias

	c_public dpmi_GetDescriptor
func	dpmi_GetDescriptor
	WINENTER
	mov	AX,0Bh
dpmi_desc:
	push	ES
	push	DI
	mov	BX,P[BP]
	les	DI,P+2[BP]
dpmi_exit_esdi_null:
	dpmi
	jnc	gd0
	  clr	  AX
	  mov	  DX,AX
	  jmp	  short dpmi_exit_esdi
gd0:	mov	DX,ES
	mov	AX,DI

dpmi_exit_esdi:
	pop	DI
	pop	ES
	WINLEAVE
	ret
c_endp	dpmi_GetDescriptor

	c_public dpmi_SetDescriptor
func	dpmi_SetDescriptor
	WINENTER
	mov	AX,0Ch
	jmp	short dpmi_desc
c_endp	dpmi_SetDescriptor

	c_public dpmi_AllocThisLDTDescriptor
func	dpmi_AllocThisLDTDescriptor
	WINENTER
	mov	AX,0Dh
	jmp	dpmi_do_bp
c_endp	dpmi_AllocThisLDTDescriptor

	c_public dpmi_DOSAlloc
func	dpmi_DOSAlloc
	WINENTER
	push	ES
	push	DI
	mov	BX,P[BP]
	mov	AX,0100h
dpmi_alloc:
	dpmi
	les	DI,P+2[BP]
	mov	ES:[DI],AX
	clr	AX
	jnc	DOS0
	  mov	  ES:[DI]+2,BX
	  dec	  AX
	  jmp	  short dpmi_exit_esdi
DOS0:	mov	ES:[DI]+2,DX
	jmp	short dpmi_exit_esdi
c_endp	dpmi_DOSAlloc

	c_public dpmi_DOSFree
func	dpmi_DOSFree
	WINENTER
	mov	DX,P[BP]
	dpmi	0101h
	jc	DOS1
	  clr	  AX
DOS1:	WINLEAVE
	ret
c_endp	dpmi_DOSFree

	c_public dpmi_DOSRealloc
func	dpmi_DOSRealloc
	WINENTER
	push	ES
	push	DI
	mov	DX,P[BP]
	mov	BX,P+2[BP]
	inc	BP
	inc	BP
	mov	AX,0102h
	jmp	short dpmi_alloc
c_endp	dpmi_DOSRealloc

	c_public dpmi_GetRealModeVector
func	dpmi_GetRealModeVector
	WINENTER
	mov	AX,0200h
dpmi_getvector:
	mov	BX,P[BP]
	dpmi
	mov	AX,DX
	mov	DX,CX
	WINLEAVE
	ret
c_endp	dpmi_GetRealModeVector

	c_public dpmi_SetRealModeVector
func	dpmi_SetRealModeVector
	WINENTER
	mov	AX,0201h
dpmi_setvector:
	mov	BX,P[BP]
	mov	DX,P+2[BP]
	mov	CX,P+4[BP]
	dpmi
	mov	AX,0
	WINLEAVE
	ret
c_endp	dpmi_SetRealModeVector

	c_public dpmi_GetExceptionHandler
func	dpmi_GetExceptionHandler
	WINENTER
	mov	AX,0202h
	jmp	short dpmi_getvector
c_endp	dpmi_GetExceptionHandler

	c_public dpmi_SetExceptionHandler
func	dpmi_SetExceptionHandler
	WINENTER
	mov	AX,0203h
	jmp	short dpmi_setvector
c_endp	dpmi_SetExceptionHandler

	c_public dpmi_GetProtectedModeVector
func	dpmi_GetProtectedModeVector
	WINENTER
	mov	AX,0204h
	jmp	short dpmi_getvector
c_endp	dpmi_GetProtectedModeVector

	c_public dpmi_SetProtectedModeVector
func	dpmi_SetProtectedModeVector
	WINENTER
	mov	AX,0205h
	jmp	short dpmi_setvector
c_endp	dpmi_SetProtectedModeVector

	c_public dpmi_SimRealModeInterrupt
func	dpmi_SimRealModeInterrupt
	WINENTER
	push	ES
	push	DI
	mov	AX,0300h
	mov	BL,P[BP]
dpmi_transload:
	mov	BH,P+2[BP]
	mov	CX,P+4[BP]
	mov	DI,P+6[BP]
	mov	ES,P+8[BP]
	jmp	dpmi_exit_esdi_null
c_endp	dpmi_SimRealModeInterrupt

	c_public dpmi_CallRealModeProcWithFarRet
func	dpmi_CallRealModeProcWithFarRet
	WINENTER
	push	ES
	push	DI
	mov	AX,0301h
dpmi_transload_call:
	inc	BP
	inc	BP
	jmp	dpmi_transload
c_endp	dpmi_CallRealModeProcWithFarRet

	c_public dpmi_CallRealModeProcWithIRet
func	dpmi_CallRealModeProcWithIRet
	WINENTER
	push	ES
	push	DI
	mov	AX,0302h
	jmp	dpmi_transload_call
c_endp	dpmi_CallRealModeProcWIthIRet

; this function should build a thunk that passes the ES:DI,DS:SI
; as parameters and initializes DS and ES registers properly

	c_public dpmi_RealModeCallBackAlloc
func	dpmi_RealModeCallBackAlloc
	WINENTER
	push	DS
	push	ES
	push	SI
	push	DI
	lds	SI,P[BP]
	les	DI,P+4[BP]
	dpmi	0303h
	jnc	ARMCB0
	  clr	  DX
	  mov	  CX,DX
ARMCB0: mov	AX,DX
	mov	DX,CX
	pop	DI
	pop	SI
	pop	ES
	pop	DS
	WINLEAVE
	ret
c_endp	dpmi_RealModeCallBackAlloc

; this function should free the thunk created by AllocRealModeCallBack

	c_public dpmi_RealModeCallBackFree
func	dpmi_RealModeCallBackFree
	WINENTER
	mov	DX,P[BP]
	mov	CX,P+2[BP]
	mov	AX,0304h
	jmp	dpmi_do
c_endp	dpmi_RealModeCallBackFree

;dpmi_GetStateSaveRestoreInfo
;struct
;  {
;  ushort	BufferSize
;  void far *	(RealModeSaveFunction)(ushort Operation,voide far *buffer);
;  void far *	(PModeSaveFunction)(ushort Operation,void far *buffer);
;  }
	c_public dpmi_GetStateSaveRestoreInfo
func	dpmi_GetStateSaveRestoreInfo
	WINENTER
	push	SI
	push	DI
	dpmi	0305h
	jc	GSSRP0
	  mov	  DX,DI
	  push	  ES
	  les	  DI,P[BP]
	  mov	  ES:[DI],AX
	  mov	  ES:[DI].2,CX
	  mov	  ES:[DI].4,BX
	  mov	  ES:[DI].6,DX
	  mov	  ES:[DI].8,SI
	  pop	  ES
	  clr	  AX
	  jmp	  short dpmi_sidi_exit
GSSRP0:	clr	AX
	neg	AX
dpmi_sidi_exit:
	pop	DI
	pop	SI
	WINLEAVE
	ret
c_endp	dpmi_GetStateSaveRestoreInfo

;dpmi_GetRawModeSwitchInfo
;struct SwitchTable
;  {
;  ushort	newDS;
;  ushort	newES;
;  ushort	newSS;
;  ushort	newCS;
;  ulong	newIP;	/* upper 16 bits invalid in 16-bit mode */
;  ulong	newSP;	/* upper 16 bits invalid in 16-bit mode */
;  }
;struct
;  {
;  void far *	(RealModeSwitchFunction)(void far *SwitchTable);
;  void far *	(PModeSwitchFunction)(void far *SwitchTable);
;  }
	c_public dpmi_GetRawModeSwitchInfo
func	dpmi_GetRawModeSwitchInfo
	WINENTER
	push	SI
	push	DI
	dpmi	0306h
	jc	GRMSI0
	  mov	  DX,DI
	  push	  ES
	  les	  DI,P[BP]
	  mov	  ES:[DI],CX
	  mov	  ES:[DI].2,BX
	  mov	  ES:[DI].4,DX
	  mov	  ES:[DI].6,SI
	  pop	  ES
	  clr	  AX
	  jmp	  short dpmi_sidi_exit
GRMSI0: clr	AX
	neg	AX
GRMSI1: jmp	dpmi_sidi_exit
c_endp	dpmi_GetRawModeSwitchInfo

;struct
;  {
;  uchar	MajorVersion;
;  uchar	MinorVersion;
;  ushort	Flags;
;  uchar	ProcessorType;
;  uchar	MasterPICInterrupt;
;  uchar	SlavePICInterrupt;
;  }
	c_public dpmi_GetVersionInfo
func	dpmi_GetVersionInfo
	WINENTER
	dpmi	0400h
	push	ES
	push	DI
	les	DI,P[BP]
	mov	ES:[DI],AH
	mov	ES:[DI].1,AL
	mov	ES:[DI].2,BX
	mov	ES:[DI].4,CL
	mov	ES:[DI].5,DH
	mov	ES:[DI].6,DL
	pop	DI
	pop	ES
	clr	AX
	WINLEAVE
	ret
c_endp	dpmi_GetVersionInfo

;struct
;  {
;  ulong	LargestContiguousBlock;
;  ulong	MaxUnlockedPageAllocation;
;  ulong	MaxLockedPageAllocation;
;  ulong	PagesOfLinearSpace;
;  ulong	TotalUnlockedPages;
;  ulong	TotalFreePages;
;  ulong	TotalPhysicalPages;
;  ulong	FreePagesOfLinearSpace;
;  ulong	SizeOfPagingPartition;
;  ulong	Reserved[3];
;  }
	c_public dpmi_GetFreeMemInfo
func	dpmi_GetFreeMemInfo
	WINENTER
	push	ES
	push	DI
	mov	AX,0500h
	jmp	dpmi_exit_esdi_null
c_endp	dpmi_GetFreeMemInfo

;struct
;  {
;  ulong	Handle;
;  ulong	LinearAddress;
;  }
	c_public dpmi_MemBlockAlloc
func	dpmi_MemBlockAlloc
	WINENTER
	push	SI
	push	DI
	mov	AX,0501h
dpmi_memalloc:
	mov	CX,P[BP]
	mov	BX,P+2[BP]
	dpmi
	jc	AMB0
	  mov	  DX,DI
	  push	  ES
	  les	  DI,P+4[BP]
	  mov	  ES:[DI].4,CX
	  mov	  ES:[DI].6,BX
	  mov	  ES:[DI].0,DI
	  mov	  ES:[DI].2,SI
	  clr	  AX
	  pop	  ES
AMB0:	clr	AX
	neg	AX
AMB1:	pop	DI
	pop	SI
	WINLEAVE
	ret
c_endp	dpmi_MemBlockAlloc

	c_public dpmi_MemBlockFree
func	dpmi_MemBlockFree
	WINENTER
	mov	DI,P[BP]
	mov	SI,P+2[BP]
	mov	AX,0502h
	jmp	dpmi_do
c_endp	dpmi_MemBlockFree

	c_public dpmi_MemBlockRealloc
func	dpmi_MemBlockRealloc
	WINENTER
	push	SI
	push	DI
	mov	AX,0503h
	mov	DI,P+4[BP]
	mov	SI,P+6[BP]
	add	BP,4
	jmp	short dpmi_memalloc
c_endp	dpmi_MemBlockRealloc

	c_public dpmi_LockLinearRegion
func	dpmi_LockLinearRegion
	WINENTER
	mov	AX,0600h
dpmi_lock:
	mov	CX,P[BP]
	mov	BX,P+2[BP]
	mov	DI,P+4[BP]
	mov	SI,P+6[BP]
	jmp	dpmi_do
c_endp	dpmi_LockLinearRegion

	c_public dpmi_UnlockLinearRegion
func	dpmi_UnlockLinearRegion
	WINENTER
	mov	AX,0601h
	jmp	short dpmi_lock
c_endp	dpmi_UnlockLinearRegion

	c_public dpmi_UnlockRealModeRegion
func	dpmi_UnlockRealModeRegion
	WINENTER
	mov	AX,0602h
	jmp	short dpmi_lock
c_endp	dpmi_UnlockRealModeRegion

	c_public dpmi_LockRealModeRegion
func	dpmi_LockRealModeRegion
	WINENTER
	mov	AX,0603h
	jmp	short dpmi_lock
c_endp	dpmi_LockRealModeRegion

	c_public dpmi_IsPageSize
func	dpmi_IsPageSize
	WINENTER
	dpmi	0604h
	jnc	IPS0
	  clr	  AX
	  mov	  DX,AX
	  jmp	  short IPS1
IPS0:	mov	AX,CX
	mov	DX,BX
IPS1:	WINLEAVE
	ret
c_endp	dpmi_IsPageSize

	c_public dpmi_MarkAsDemandPage
func	dpmi_MarkAsDemandPage
	WINENTER
	mov	AX,0702h
	jmp	short dpmi_lock
c_endp	dpmi_MarkAsDemandPage

	c_public dpmi_DiscardPage
func	dpmi_DiscardPage
	WINENTER
	mov	AX,0703h
	jmp	short dpmi_lock
c_endp	dpmi_DiscardPage

	c_public dpmi_IsLinearAddress
func	dpmi_IsLinearAddress
	WINENTER
	mov	CX,P[BP]
	mov	BX,P+2[BP]
	mov	DI,P+4[BP]
	mov	SI,P+6[BP]
	dpmi	0800h
	jnc	ILA0
	  clr	  CX
	  neg	  CX
	  mov	  BX,CX
ILA0:	mov	AX,CX
	mov	DX,BX
ILA1:	WINLEAVE
	ret
c_endp	dpmi_IsLinearAddress

	c_public dpmi_DisableVirtualInterruptState
func	dpmi_DisableVirtualInterruptState
	WINENTER
	mov	AX,0900h
	jmp	dpmi_do
c_endp	dpmi_DisableVirtualInterruptState

	c_public dpmi_EnableVirtualInterruptState
func	dpmi_EnableVirtualInterruptState
	WINENTER
	mov	AX,0901h
	jmp	dpmi_do
c_endp	dpmi_EnableVirtualInterruptState

	c_public dpmi_IsVirtualInterruptState
func	dpmi_IsVirtualInterruptState
	WINENTER
	mov	AX,0902h
	jmp	dpmi_do
c_endp	dpmi_IsVirtualInterruptState

	c_public dpmi_GetVendorAPI
func	dpmi_GetVendorAPI
	WINENTER
	push	ES
	push	DS
	push	SI
	push	DI
	lds	SI,P[BP]
	dpmi	0A00h
	jnc	GVAPI0
	  clr	  AX
	  mov	  DX,AX
	  jmp	  short GVAPI1
GVAPI0:	mov	DX,ES
	mov	AX,DI
GVAPI1: pop	DI
	pop	SI
	pop	DS
	pop	ES
	WINLEAVE
	ret
c_endp	dpmi_GetVendorAPI

	c_public dpmi_SetDebugWatchPoint
func	dpmi_SetDebugWatchPoint
	WINENTER
	mov	CX,P[BP]
	mov	BX,P+2[BP]
	mov	DL,P+4[BP]
	mov	DH,P+6[BP]
	dpmi	0B00h
	jnc	SDWP0
	  clr	  BX
	  neg	  BX
SDWP0:	mov	AX,BX
	WINLEAVE
	ret
c_endp	dpmi_SetDebugWatchPoint

	c_public dpmi_ClearDebugWatchPoint
func	dpmi_ClearDebugWatchPoint
	WINENTER
	mov	AX,0B01h
	jmp	dpmi_do_bp
c_endp	dpmi_ClearDebugWatchPoint

	c_public dpmi_IsDebugWatchPointState
func	dpmi_IsDebugWatchPointState
	WINENTER
	mov	AX,0B02h
	jmp	dpmi_do_bp
c_endp	dpmi_IsDebugWatchPointState

	c_public dpmi_ResetDebugWatchPoint
func	dpmi_ResetDebugWatchPoint
	WINENTER
	mov	AX,0B03h
	jmp	dpmi_do_bp
c_endp	dpmi_ResetDebugWatchPoint

	endcode dpmi
	end
	end
