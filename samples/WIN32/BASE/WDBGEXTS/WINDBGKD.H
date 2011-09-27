/*++ BUILD Version: 0045    // Increment this if a change has global effects

Copyright (c) 1990-1994  Microsoft Corporation

Module Name:

    windbgkd.h

Abstract:

    This module defines the 32-Bit Windows Kernel Debugger interface.

Revision History:

--*/

#ifndef _WINDBGKD_
#define _WINDBGKD_

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

#ifdef _X86_

//
// DBGKD_CONTROL_REPORT
//
// This structure contains machine specific data passed to the debugger
// when a Wait_State_Change message is sent.  Idea is to allow debugger
// to do what it needes without reading any more packets.
// Structure is filled in by KdpSetControlReport
//

#define DBGKD_MAXSTREAM 16

typedef struct _DBGKD_CONTROL_REPORT {
    DWORD   Dr6;
    DWORD   Dr7;
    WORD    InstructionCount;
    WORD    ReportFlags;
    BYTE    InstructionStream[DBGKD_MAXSTREAM];
    WORD    SegCs;
    WORD    SegDs;
    WORD    SegEs;
    WORD    SegFs;
    DWORD   EFlags;
} DBGKD_CONTROL_REPORT, *PDBGKD_CONTROL_REPORT;

#define REPORT_INCLUDES_SEGS    0x0001  // this is for backward compatibility

//
// DBGKD_CONTROL_SET
//
// This structure control value the debugger wants to set on every
// continue, and thus sets here to avoid packet traffic.
//

typedef struct _DBGKD_CONTROL_SET {
    DWORD   TraceFlag;                  // WARNING: This must NOT be a BOOLEAN,
                                        //     or host and target will end
                                        //     up with different alignments!
    DWORD   Dr7;
    DWORD   CurrentSymbolStart;         // Range in which to trace locally
    DWORD   CurrentSymbolEnd;
} DBGKD_CONTROL_SET, *PDBGKD_CONTROL_SET;

#endif //_X86_


typedef struct _DESCRIPTOR_TABLE_ENTRY {
    DWORD Selector;
    LDT_ENTRY Descriptor;
} DESCRIPTOR_TABLE_ENTRY, *PDESCRIPTOR_TABLE_ENTRY;


#if defined(_MIPS_) | defined(_ALPHA_) | defined(_PPC_)

//
// Define RISC specific kernel debugger information.
//
// The following structure contains machine specific data passed to
// the host system kernel debugger in a wait state change message.
//

#define DBGKD_MAXSTREAM 16

typedef struct _DBGKD_CONTROL_REPORT {
    DWORD InstructionCount;
    BYTE  InstructionStream[DBGKD_MAXSTREAM];
} DBGKD_CONTROL_REPORT, *PDBGKD_CONTROL_REPORT;

//
// The following structure contains information that the host system
// kernel debugger wants to set on every continue operation and avoids
// the need to send extra packets of information.
//

typedef DWORD DBGKD_CONTROL_SET, *PDBGKD_CONTROL_SET;

#endif                          // RISC

#ifdef _X86_
//
// Special Registers for i386
//

typedef struct _DESCRIPTOR {
    WORD    Pad;
    WORD    Limit;
    DWORD   Base;
} KDESCRIPTOR, *PKDESCRIPTOR;

typedef struct _KSPECIAL_REGISTERS {
    DWORD Cr0;
    DWORD Cr2;
    DWORD Cr3;
    DWORD Cr4;
    DWORD KernelDr0;
    DWORD KernelDr1;
    DWORD KernelDr2;
    DWORD KernelDr3;
    DWORD KernelDr6;
    DWORD KernelDr7;
    KDESCRIPTOR Gdtr;
    KDESCRIPTOR Idtr;
    WORD   Tr;
    WORD   Ldtr;
    DWORD Reserved[6];
} KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;

//
// Processor State frame: Before a processor freezes itself, it
// dumps the processor state to the processor state frame for
// debugger to examine.
//

typedef struct _KPROCESSOR_STATE {
    struct _CONTEXT ContextFrame;
    struct _KSPECIAL_REGISTERS SpecialRegisters;
} KPROCESSOR_STATE, *PKPROCESSOR_STATE;
#endif // _X86_

typedef struct _DBGKM_EXCEPTION {
    EXCEPTION_RECORD ExceptionRecord;
    DWORD FirstChance;
} DBGKM_EXCEPTION, *PDBGKM_EXCEPTION;


//
// DbgKd APIs are for the portable kernel debugger
//

//
// KD_PACKETS are the low level data format used in KD. All packets
// begin with a packet leader, byte count, packet type. The sequence
// for accepting a packet is:
//
//  - read 4 bytes to get packet leader.  If read times out (10 seconds)
//    with a short read, or if packet leader is incorrect, then retry
//    the read.
//
//  - next read 2 byte packet type.  If read times out (10 seconds) with
//    a short read, or if packet type is bad, then start again looking
//    for a packet leader.
//
//  - next read 4 byte packet Id.  If read times out (10 seconds)
//    with a short read, or if packet Id is not what we expect, then
//    ask for resend and restart again looking for a packet leader.
//
//  - next read 2 byte byte count.  If read times out (10 seconds) with
//    a short read, or if byte count is greater than PACKET_MAX_SIZE,
//    then start again looking for a packet leader.
//
//  - next read 4 byte packet data checksum.
//
//  - The packet data immediately follows the packet.  There should be
//    ByteCount bytes following the packet header.  Read the packet
//    data, if read times out (10 seconds) then start again looking for
//    a packet leader.
//


typedef struct _KD_PACKET {
    DWORD PacketLeader;
    WORD   PacketType;
    WORD   ByteCount;
    DWORD PacketId;
    DWORD Checksum;
} KD_PACKET, *PKD_PACKET;


#define PACKET_MAX_SIZE 4000
#define INITIAL_PACKET_ID 0x80800000    // DON't use 0
#define SYNC_PACKET_ID    0x00000800    // Or in with INITIAL_PACKET_ID
                                        // to force a packet ID reset.

//
// BreakIn packet
//

#define BREAKIN_PACKET                  0x62626262
#define BREAKIN_PACKET_BYTE             0x62

//
// Packet lead in sequence
//

#define PACKET_LEADER                   0x30303030 //0x77000077
#define PACKET_LEADER_BYTE              0x30

#define CONTROL_PACKET_LEADER           0x69696969
#define CONTROL_PACKET_LEADER_BYTE      0x69

//
// Packet Trailing Byte
//

#define PACKET_TRAILING_BYTE            0xAA

//
// Packet Types
//

#define PACKET_TYPE_UNUSED              0
#define PACKET_TYPE_KD_STATE_CHANGE     1
#define PACKET_TYPE_KD_STATE_MANIPULATE 2
#define PACKET_TYPE_KD_DEBUG_IO         3
#define PACKET_TYPE_KD_ACKNOWLEDGE      4       // Packet-control type
#define PACKET_TYPE_KD_RESEND           5       // Packet-control type
#define PACKET_TYPE_KD_RESET            6       // Packet-control type
#define PACKET_TYPE_MAX                 7

//
// If the packet type is PACKET_TYPE_KD_STATE_CHANGE, then
// the format of the packet data is as follows:
//

#define DbgKdExceptionStateChange   0x00003030L
#define DbgKdLoadSymbolsStateChange 0x00003031L

//
// Pathname Data follows directly
//

typedef struct _DBGKD_LOAD_SYMBOLS {
    DWORD PathNameLength;
    PVOID BaseOfDll;
    DWORD ProcessId;
    DWORD CheckSum;
    DWORD SizeOfImage;
    BOOLEAN UnloadSymbols;
} DBGKD_LOAD_SYMBOLS, *PDBGKD_LOAD_SYMBOLS;

typedef struct _DBGKD_WAIT_STATE_CHANGE {
    DWORD NewState;
    WORD   ProcessorType;
    WORD   Processor;
    DWORD NumberProcessors;
    PVOID Thread;
    PVOID ProgramCounter;
    union {
        DBGKM_EXCEPTION Exception;
        DBGKD_LOAD_SYMBOLS LoadSymbols;
    } u;
    DBGKD_CONTROL_REPORT ControlReport;
    CONTEXT Context;
} DBGKD_WAIT_STATE_CHANGE, *PDBGKD_WAIT_STATE_CHANGE;

//
// If the packet type is PACKET_TYPE_KD_STATE_MANIPULATE, then
// the format of the packet data is as follows:
//
// Api Numbers for state manipulation
//

#define DbgKdReadVirtualMemoryApi     0x00003130L
#define DbgKdWriteVirtualMemoryApi    0x00003131L
#define DbgKdGetContextApi            0x00003132L
#define DbgKdSetContextApi            0x00003133L
#define DbgKdWriteBreakPointApi       0x00003134L
#define DbgKdRestoreBreakPointApi     0x00003135L
#define DbgKdContinueApi              0x00003136L
#define DbgKdReadControlSpaceApi      0x00003137L
#define DbgKdWriteControlSpaceApi     0x00003138L
#define DbgKdReadIoSpaceApi           0x00003139L
#define DbgKdWriteIoSpaceApi          0x0000313AL
#define DbgKdRebootApi                0x0000313BL
#define DbgKdContinueApi2             0x0000313CL
#define DbgKdReadPhysicalMemoryApi    0x0000313DL
#define DbgKdWritePhysicalMemoryApi   0x0000313EL
#define DbgKdQuerySpecialCallsApi     0x0000313FL
#define DbgKdSetSpecialCallApi        0x00003140L
#define DbgKdClearSpecialCallsApi     0x00003141L
#define DbgKdSetInternalBreakPointApi 0x00003142L
#define DbgKdGetInternalBreakPointApi 0x00003143L
#define DbgKdReadIoSpaceExtendedApi   0x00003144L
#define DbgKdWriteIoSpaceExtendedApi  0x00003145L
#define DbgKdGetVersionApi            0x00003146L
#define DbgKdWriteBreakPointExApi     0x00003147L
#define DbgKdRestoreBreakPointExApi   0x00003148L
#define DbgKdCauseBugCheckApi         0x00003149L
#define DbgKdSwitchProcessor          0x00003150L

//
// Response is a read memory message with data following
//

typedef struct _DBGKD_READ_MEMORY {
    PVOID TargetBaseAddress;
    DWORD TransferCount;
    DWORD ActualBytesRead;
} DBGKD_READ_MEMORY, *PDBGKD_READ_MEMORY;

//
// Data follows directly
//

typedef struct _DBGKD_WRITE_MEMORY {
    PVOID TargetBaseAddress;
    DWORD TransferCount;
    DWORD ActualBytesWritten;
} DBGKD_WRITE_MEMORY, *PDBGKD_WRITE_MEMORY;

//
// Response is a get context message with a full context record following
//

typedef struct _DBGKD_GET_CONTEXT {
    DWORD ContextFlags;
} DBGKD_GET_CONTEXT, *PDBGKD_GET_CONTEXT;

//
// Full Context record follows
//

typedef struct _DBGKD_SET_CONTEXT {
    DWORD ContextFlags;
} DBGKD_SET_CONTEXT, *PDBGKD_SET_CONTEXT;

typedef struct _DBGKD_WRITE_BREAKPOINT {
    PVOID BreakPointAddress;
    DWORD BreakPointHandle;
} DBGKD_WRITE_BREAKPOINT, *PDBGKD_WRITE_BREAKPOINT;

typedef struct _DBGKD_RESTORE_BREAKPOINT {
    DWORD BreakPointHandle;
} DBGKD_RESTORE_BREAKPOINT, *PDBGKD_RESTORE_BREAKPOINT;

typedef struct _DBGKD_BREAKPOINTEX {
    DWORD     BreakPointCount;
    DWORD     ContinueStatus;
} DBGKD_BREAKPOINTEX, *PDBGKD_BREAKPOINTEX;

typedef struct _DBGKD_CONTINUE {
    DWORD    ContinueStatus;
} DBGKD_CONTINUE, *PDBGKD_CONTINUE;

typedef struct _DBGKD_CONTINUE2 {
    DWORD    ContinueStatus;
    DBGKD_CONTROL_SET ControlSet;
} DBGKD_CONTINUE2, *PDBGKD_CONTINUE2;

typedef struct _DBGKD_READ_WRITE_IO {
    DWORD DataSize;                     // 1, 2, 4
    PVOID IoAddress;
    DWORD DataValue;
} DBGKD_READ_WRITE_IO, *PDBGKD_READ_WRITE_IO;

typedef struct _DBGKD_READ_WRITE_IO_EXTENDED {
    DWORD DataSize;                     // 1, 2, 4
    DWORD InterfaceType;
    DWORD BusNumber;
    DWORD AddressSpace;
    PVOID IoAddress;
    DWORD DataValue;
} DBGKD_READ_WRITE_IO_EXTENDED, *PDBGKD_READ_WRITE_IO_EXTENDED;

typedef struct _DBGKD_QUERY_SPECIAL_CALLS {
    DWORD NumberOfSpecialCalls;
    // DWORD SpecialCalls[];
} DBGKD_QUERY_SPECIAL_CALLS, *PDBGKD_QUERY_SPECIAL_CALLS;

typedef struct _DBGKD_SET_SPECIAL_CALL {
    DWORD SpecialCall;
} DBGKD_SET_SPECIAL_CALL, *PDBGKD_SET_SPECIAL_CALL;

typedef struct _DBGKD_SET_INTERNAL_BREAKPOINT {
    DWORD BreakpointAddress;
    DWORD Flags;
} DBGKD_SET_INTERNAL_BREAKPOINT, *PDBGKD_SET_INTERNAL_BREAKPOINT;

typedef struct _DBGKD_GET_INTERNAL_BREAKPOINT {
    DWORD BreakpointAddress;
    DWORD Flags;
    DWORD Calls;
    DWORD MaxCallsPerPeriod;
    DWORD MinInstructions;
    DWORD MaxInstructions;
    DWORD TotalInstructions;
} DBGKD_GET_INTERNAL_BREAKPOINT, *PDBGKD_GET_INTERNAL_BREAKPOINT;

#define DBGKD_INTERNAL_BP_FLAG_COUNTONLY 0x00000001 // don't count instructions
#define DBGKD_INTERNAL_BP_FLAG_INVALID   0x00000002 // disabled BP
#define DBGKD_INTERNAL_BP_FLAG_SUSPENDED 0x00000004 // temporarily suspended
#define DBGKD_INTERNAL_BP_FLAG_DYING     0x00000008 // kill on exit

typedef struct _DBGKD_GET_VERSION {
    WORD    MajorVersion;
    WORD    MinorVersion;
    WORD    ProtocolVersion;
    WORD    Flags;
    DWORD   KernBase;
    DWORD   PsLoadedModuleList;
} DBGKD_GET_VERSION, *PDBGKD_GET_VERSION;

#define DBGKD_VERS_FLAG_MP      0x0001      // kernel is MP built


typedef struct _DBGKD_MANIPULATE_STATE {
    DWORD ApiNumber;
    WORD   ProcessorType;
    WORD   Processor;
    DWORD    ReturnStatus;
    union {
        DBGKD_READ_MEMORY ReadMemory;
        DBGKD_WRITE_MEMORY WriteMemory;
        DBGKD_GET_CONTEXT GetContext;
        DBGKD_SET_CONTEXT SetContext;
        DBGKD_WRITE_BREAKPOINT WriteBreakPoint;
        DBGKD_RESTORE_BREAKPOINT RestoreBreakPoint;
        DBGKD_CONTINUE Continue;
        DBGKD_CONTINUE2 Continue2;
        DBGKD_READ_WRITE_IO ReadWriteIo;
        DBGKD_READ_WRITE_IO_EXTENDED ReadWriteIoExtended;
        DBGKD_QUERY_SPECIAL_CALLS QuerySpecialCalls;
        DBGKD_SET_SPECIAL_CALL SetSpecialCall;
        DBGKD_SET_INTERNAL_BREAKPOINT SetInternalBreakpoint;
        DBGKD_GET_INTERNAL_BREAKPOINT GetInternalBreakpoint;
        DBGKD_GET_VERSION GetVersion;
        DBGKD_BREAKPOINTEX BreakPointEx;
    } u;
} DBGKD_MANIPULATE_STATE, *PDBGKD_MANIPULATE_STATE;

//
// This is the format for the trace data passed back from the kernel to
// the debugger to describe multiple calls that have returned since the
// last trip back.  The basic format is that there are a bunch of these
// (4 byte) unions stuck together.  Each union is of one of two types: a
// 4 byte unsigned long interger, or a three field struct, describing a
// call (where "call" is delimited by returning or exiting the symbol
// scope).  If the number of instructions executed is too big to fit
// into a WORD   -1, then the Instructions field has
// TRACE_DATA_INSTRUCTIONS_BIG and the next union is a LongNumber
// containing the real number of instructions executed.
//
// The very first union returned in each callback is a LongNumber
// containing the number of unions returned (including the "size"
// record, os it's always at least 1 even if there's no data to return).
//
// This is all returned to the debugger when one of two things
// happens:
//
//   1) The pc moves out of all defined symbol ranges
//   2) The buffer of trace data entries is filled.
//
// The "trace done" case is hacked around on the debugger side.  It
// guarantees that the pc address that indicates a trace exit never
// winds up in a defined symbol range.
//
// The only other complexity in this system is handling the SymbolNumber
// table.  This table is kept in parallel by the kernel and the
// debugger.  When the PC exits a known symbol range, the Begin and End
// symbol ranges are set by the debugger and are allocated to the next
// symbol slot upon return.  "The next symbol slot" means the numerical
// next slot number, unless we've filled all slots, in which case it is
// #0.  (ie., allocation is cyclic and not LRU or something).  The
// SymbolNumber table is flushed when a SpecialCalls call is made (ie.,
// at the beginning of the WatchTrace).
//

typedef union _DBGKD_TRACE_DATA {
    struct {
        BYTE  SymbolNumber;
        CHAR LevelChange;
        WORD   Instructions;
    } s;
    DWORD LongNumber;
} DBGKD_TRACE_DATA, *PDBGKD_TRACE_DATA;

#define TRACE_DATA_INSTRUCTIONS_BIG 0xffff

#define TRACE_DATA_BUFFER_MAX_SIZE 40

//
// If the packet type is PACKET_TYPE_KD_DEBUG_IO, then
// the format of the packet data is as follows:
//

#define DbgKdPrintStringApi     0x00003230L
#define DbgKdGetStringApi       0x00003231L

//
// For print string, the Null terminated string to print
// immediately follows the message
//
typedef struct _DBGKD_PRINT_STRING {
    DWORD LengthOfString;
} DBGKD_PRINT_STRING, *PDBGKD_PRINT_STRING;

//
// For get string, the Null terminated promt string
// immediately follows the message. The LengthOfStringRead
// field initially contains the maximum number of characters
// to read. Upon reply, this contains the number of bytes actually
// read. The data read immediately follows the message.
//
//
typedef struct _DBGKD_GET_STRING {
    DWORD LengthOfPromptString;
    DWORD LengthOfStringRead;
} DBGKD_GET_STRING, *PDBGKD_GET_STRING;

typedef struct _DBGKD_DEBUG_IO {
    DWORD ApiNumber;
    WORD   ProcessorType;
    WORD   Processor;
    union {
        DBGKD_PRINT_STRING PrintString;
        DBGKD_GET_STRING GetString;
    } u;
} DBGKD_DEBUG_IO, *PDBGKD_DEBUG_IO;


VOID
NTAPI
DbgKdSendBreakIn(
    VOID
    );

PBYTE
NTAPI
DbgKdGets(
    PBYTE  Buffer,
    WORD   Length
    );

DWORD
NTAPI
DbgKdWaitStateChange(
    OUT PDBGKD_WAIT_STATE_CHANGE StateChange,
    OUT PVOID Buffer,
    IN DWORD BufferLength
    );

DWORD
NTAPI
DbgKdContinue (
    IN DWORD    ContinueStatus
    );

DWORD
NTAPI
DbgKdContinue2 (
    IN DWORD    ContinueStatus,
    IN DBGKD_CONTROL_SET ControlSet
    );

DWORD
NTAPI
DbgKdReadVirtualMemory(
    IN PVOID TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesRead OPTIONAL
    );

DWORD
NTAPI
DbgKdWriteVirtualMemory(
    IN PVOID TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesWritten OPTIONAL
    );

DWORD
NTAPI
DbgKdReadPhysicalMemory(
    IN PHYSICAL_ADDRESS TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesRead OPTIONAL
    );

DWORD
NTAPI
DbgKdWritePhysicalMemory(
    IN PHYSICAL_ADDRESS TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesWritten OPTIONAL
    );

DWORD
NTAPI
DbgKdReadControlSpace(
    IN WORD   Processor,
    IN PVOID TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesRead OPTIONAL
    );

DWORD
NTAPI
DbgKdWriteControlSpace(
    IN WORD   Processor,
    IN PVOID TargetBaseAddress,
    OUT PVOID UserInterfaceBuffer,
    IN DWORD TransferCount,
    OUT PDWORD ActualBytesWritten OPTIONAL
    );

DWORD
NTAPI
DbgKdReadIoSpace(
    IN PVOID IoAddress,
    OUT PVOID ReturnedData,
    IN DWORD DataSize
    );

DWORD
NTAPI
DbgKdWriteIoSpace(
    IN PVOID IoAddress,
    IN DWORD DataValue,
    IN DWORD DataSize
    );

DWORD
NTAPI
DbgKdGetContext(
    IN WORD   Processor,
    IN OUT PCONTEXT Context
    );

DWORD
NTAPI
DbgKdSetContext(
    IN WORD   Processor,
    IN PCONTEXT Context
    );

DWORD
NTAPI
DbgKdWriteBreakPoint(
    IN PVOID BreakPointAddress,
    OUT PDWORD BreakPointHandle
    );

DWORD
NTAPI
DbgKdRestoreBreakPoint(
    IN DWORD BreakPointHandle
    );

DWORD
NTAPI
DbgKdReboot(
    VOID
    );

#ifdef _X86_
DWORD
NTAPI
DbgKdLookupSelector(
    IN WORD   Processor,
    IN OUT PDESCRIPTOR_TABLE_ENTRY pDescriptorTableEntry
    );
#endif

#define DBG_EXCEPTION_HANDLED           ((DWORD   )0x00010001L)

#endif // _WINDBGKD_
