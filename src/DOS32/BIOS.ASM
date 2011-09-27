;_ bios.asm   Tue Mar 20 1990   Modified by: Walter Bright */
; These functions are not available under OS/2
; DOS386 support added by G. Eric Engstrom

include	macros.asm

DISKINFO STRUC
	drive		DW	?
	head		DW	?
	track		DW	?
	sector		DW	?
	nsectors	DW	?
	buffer		DD	?
DISKINFO ENDS

	begcode	bios

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS keyboard function
;	#include <bios.h>
;	int _bios_keybrd(int flag);
;	int bioskey(int flag);		/* both are the same routine	*/
; Usage:
;	flag = 0	Return ascii value in low byte, scan code in high byte
;	     = 1	Return 0 if no key is available else return ascii
;			value in low byte and scan code in high byte. Key is
;			left unread in BIOS
;			A -1 returned means that a ^C was read by the BIOS
;			(^C is turned into 0 by the BIOS, we turn it into
;			 -1 to distinguish it from the no-key-available case)
;	     = 2	Return shift status, bits are:
;			0x01	Right shift key is down
;			0x02	Left shift key is down
;			0x04	Ctrl key is down
;			0x08	Alt key is down
;			0x10	Scroll Lock is toggled
;			0x20	Num Lock is toggled
;			0x40	Caps Lock is toggled
;			0x80	Ins is toggled
;			Other bits are undefined

	public	_bioskey
_bioskey:

	c_public _bios_keybrd
func	_bios_keybrd
	push	EBP
	mov	EBP,ESP
	mov	AH,P[EBP]
	int	16h
	jnz	K1
	; ZF == 1
	_ifs	<byte ptr P[EBP]> ne 1, K2
	clr	EAX		;if flag is 1 and Z was set
	jmp	short K2
K1:	; ZF == 0
	_ifs	<byte ptr P[EBP]> ne 1, K2
	or	AX,AX
	jnz	K2
	mov	EAX,-1		;if flag is 1 and Z was clear, but key to return was 0, then return -1
K2:	pop	EBP
	ret
c_endp	_bios_keybrd


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS equipment list function
;	#include <bios.h>
;	unsigned _bios_equiplist(void);
; Usage:
;	returns the bios equipment list in AX
;
;	bit 0			1if any diskettes
;	bit 1			Not used (set to 0)
;	bits 2 & 3		System board RAM: 11 = 64k (normal)
;	bits 4 & 5		Initial video mode 10 = color, 11 = mono, 01 = 40col
;	bits 6 & 7		Number of disk drives -1
;	bit 8			DMA chip (0 = installed)
;	bits 9,10 & 11	Number of serial ports
;	bit 12			Game adaptor (1 = installed)
;	bit 13			Serial printer installed (=1) PC jnr only.
;	bits 14 & 15	Number of printers installed
;
	c_public _bios_equiplist
func _bios_equiplist
	clr	EAX
	int	11h
	ret
c_endp	_bios_equiplist

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS memory size function
;	#include <bios.h>
;	unsigned _bios_memsize(void)
; Usage:
;	returns the memory size in kilobytes
;	in AX
;	
	c_public _bios_memsize
func	_bios_memsize
	clr	EAX
	int	12h
	ret
c_endp	_bios_memsize


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS printer functions
;	#include <bios.h>
;	int _bios_printer(unsigned service, unsigned printer, unsigned data)
; Usage:
;	service = 0	write the low order byte of data to the printer which
;			was specified in the printer argument.
;			returns the printer status thus
;			0x01	Printer timed out
;			0x02	Not used
;			0x04	Not used
;			0x08	I/O error
;			0x10	Printer selected
;			0x20	Out of paper
;			0x40	Acknowledge
;			0x80	Printer not busy
;	service = 1	Intialize the selected printer. The data argument is
;				ignored. The status is return as above.
;	service = 2	Returns the printer status as defined above
;	
	c_public _bios_printer
func	_bios_printer
	push	EBP
	mov	EBP,ESP
	uses	<EDX>
	mov	AH,P[EBP]
	mov	DX,P+4[EBP]
	mov	AL,P+8[EBP]
	int	17h
	mov	AL,AH		;return status byte in AL
	clr	AH
	unuse	<EDX>
	pop	EBP
	ret

c_endp	_bios_printer

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS serial comms functions
;	#include <bios.h>
;	int _bios_serialcom(unsigned service, unsigned port, unsigned data)
; Usage:
;	service = 0	Sets the data port up as specified in the data argument	
;	service = 1	Transmits the data character over the port
;	service = 2	Accepts a charcter from the serial port
;	service = 3 Returns the current status of the selected serial port
	c_public _bios_serialcom
func	_bios_serialcom
	push	EBP
	mov	EBP,ESP
	uses	<EDX>
	mov	AH,P[EBP]
	mov	DX,P+4[EBP]
	mov	AL,P+8[EBP]
	int	14h
	unuse	<EDX>
	pop	EBP
	ret

c_endp _bios_serialcom

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS time of day functions
;	#include <bios.h>
;	int _bios_timeofday(int service, long *timeval)
; Usage:
;	service = 0	copies the current value of the clock count
;				to the location that timeval points to.
;	service = 1	sets the current value of the system clock 
;				to that in the location pointed to by timeval.
	c_public _bios_timeofday
func	_bios_timeofday
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX>
	mov	AH,P[EBP]
	mov	EBX,P+4[EBP]
	_ifs	AH ne 1,T1
	  mov	  DX,[EBX]
	  mov	  CX,[EBX+2]
	  int	  01Ah
	  jmps	  T3
T1:	int	01Ah
	mov	[EBX],DX
	mov	[EBX+2],CX

T3:	unuse	<EDX,ECX,EBX>
T2:	pop	EBP
	ret
c_endp	_bios_timeofday

if 0    ;_bios_disk is currently not supported in either Pharlap or X386

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Access BIOS disk (int 0x13)  functions
;	#include <bios.h>
;	int _bios_disk(int service, struct diskinfo_t *diskinfo)
; Usage:
;	service = 0	reset the disk system
;	service = 1	get diskette status					
;	service = 2	read diskette sectors					
;	service = 3	write diskette sectors					
;	service = 4	verify diskette sectors					
;	service = 5	format diskette sectors					
	c_public _bios_disk
func	_bios_disk
	push	EBP
	mov	EBP,ESP
	uses	<EBX,ECX,EDX>
	mov	EBX,P+4[EBP]
	mov	DL,byte ptr drive[EBX]
	mov	DH,byte ptr head[EBX]
	mov	CL,byte ptr sector[EBX]
	mov	CH,byte ptr track[EBX]
	mov	AL,byte ptr nsectors[EBX]
	mov	EBX,buffer[EBX]
	mov	AH,P[EBP]
	int	13h
	jc	D1		;error (error code is in AH)
	_ifs	<byte ptr P[EBP]> e 0, D1
	_ifs	<byte ptr P[EBP]> e 1, D2
	_ifs	<byte ptr P[EBP]> ne 5, D3
D1:	clr	AL
	jmp	short D3

D2:	xchg	AH,AL		;put status bits in AH, 0 in AL

D3:	unuse	<EDX,ECX,EBX>
	jmp	T2
c_endp	_bios_disk

endif

	endcode bios

	end
