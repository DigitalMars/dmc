;_ memcpy.asm
;Copyright (C) 1995 by Digital Mars
;All Rights Reserved

include macros.asm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Copy n bytes from p2 to p1.        
;Returns: p1                        

    begcode memcpy

memcpy_forward_dtable    label    dword

    dd    memcpy_forward_check_bytes
    dd    memcpy_forward_dw1
    dd    memcpy_forward_dw2
    dd    memcpy_forward_dw3
    dd    memcpy_forward_dw4
    dd    memcpy_forward_dw5

memcpy_forward_btable    label    dword
    dd    memcpy_forward_bdone
    dd    memcpy_forward_bw1
    dd    memcpy_forward_bw2
    dd    memcpy_forward_bw3

memcpy_backward_dtable    label    dword
    dd    memcpy_backward_check_bytes
    dd    memcpy_backward_dw1
    dd    memcpy_backward_dw2
    dd    memcpy_backward_dw3
    dd    memcpy_backward_dw4
    dd    memcpy_backward_dw5

memcpy_backward_btable    label    dword
    dd    memcpy_backward_bdone
    dd    memcpy_backward_bw1
    dd    memcpy_backward_bw2
    dd    memcpy_backward_bw3



    public    _memcpy, _memmove

_memmove:
_memcpy    proc    near

    mov    ecx,12[esp]     ;n
    mov    eax,4[esp]    ;p1
    
    ;; Under DOSX, ES and DS are not the same, so make sure ES is set correctly
    ife ESeqDS
    push DS
    pop  ES
    endif

    push    edi
    push    esi

    test    ecx,ecx
    jz      memcpy_done    ;Copy zero bytes.

    mov     esi,8+8[esp]  ;p2 
    mov     edi,eax

    cmp     esi,edi
    jz      memcpy_done    ;Do nothing if same.

    lea     edx,[ecx+esi]
    ja      memcpy_forward    ;forward copy if src > dest

    cmp     edx,edi
    ja      memcpy_backward    ;forward copy if src+size <= dest
memcpy_forward:
    cmp     ecx,4
    jb      Aligned

    mov     eax,esi
    mov     edx,edi

    and     al,3
    jz      Aligned

    and     dl,3
    jz      Aligned
DoAlign:
    mov     al,[esi]
    inc     esi

    mov     [edi],al
    inc     edi

    test    esi,3

    lea     ecx,[ecx-1]
    jnz     DoAlign
Aligned:
    mov     edx,ecx

    shr     ecx,2
    and     edx,3

    cmp     ecx,5
    ja      memcpy_forward_use_dword
    jmp     memcpy_forward_dtable[ecx*4]

memcpy_done:
    pop     esi
    pop     edi
    ret

    align   4

memcpy_forward_dw5:
    mov     eax,[esi]
    add     esi,4

    mov     [edi],eax
    add     edi,4
memcpy_forward_dw4:
    mov     eax,[esi]
    add     esi,4

    mov     [edi],eax
    add     edi,4
memcpy_forward_dw3:
    mov     eax,[esi]
    add     esi,4

    mov     [edi],eax
    add     edi,4
memcpy_forward_dw2:
    mov     eax,[esi]
    add     esi,4

    mov     [edi],eax
    add     edi,4
memcpy_forward_dw1:
    mov     eax,[esi]
    add     esi,4

    mov     [edi],eax
    add     edi,4

memcpy_forward_check_bytes:
    jmp     memcpy_forward_btable[edx*4]

memcpy_forward_bw3:
    mov     cl,[esi]
    inc     esi

    mov     [edi],cl
    inc     edi
memcpy_forward_bw2:
    mov     cl,[esi]
    inc     esi

    mov     [edi],cl
    inc     edi
memcpy_forward_bw1:
    mov     cl,[esi]

    mov     [edi],cl
memcpy_forward_bdone:
    mov     eax,4+8[esp]    ;return p1

    pop     esi
    pop     edi
    ret

memcpy_forward_use_dword:
    rep     movsd

    jmp     memcpy_forward_btable[edx*4]

memcpy_backward:
    lea     esi,[esi+ecx-4]        ;set up for backward dword copy
    lea     edi,[edi+ecx-4]

    cmp     ecx,4
    jb      BackAligned

    mov     eax,esi
    mov     edx,edi

    and     al,3
    jz      BackAligned

    and     dl,3
    jz      BackAligned
DoBackAlign:
    mov     al,[esi+3]
    dec     esi

    mov     [edi+3],al
    dec     edi

    test    esi,3

    lea     ecx,[ecx-1]
    jnz     DoBackAlign
BackAligned:
    mov     edx,ecx

    shr     ecx,2
    and     edx,3

    cmp     ecx,5
    ja      memcpy_backward_use_dword

    jmp     memcpy_backward_dtable[ecx*4]

    align   4


memcpy_backward_dw5:
    mov     ecx,[esi]
    sub     esi,4

    mov     [edi],ecx
    sub     edi,4
memcpy_backward_dw4:
    mov     ecx,[esi]
    sub     esi,4

    mov     [edi],ecx
    sub     edi,4
memcpy_backward_dw3:
    mov     ecx,[esi]
    sub     esi,4

    mov     [edi],ecx
    sub     edi,4
memcpy_backward_dw2:
    mov     ecx,[esi]
    sub     esi,4

    mov     [edi],ecx
    sub     edi,4
memcpy_backward_dw1:
    mov     ecx,[esi]
    sub     esi,4

    mov     [edi],ecx
    sub     edi,4
memcpy_backward_check_bytes:

    jmp     memcpy_backward_btable[edx*4]

memcpy_backward_bw3:
    mov     cl,[esi+3]
    dec     esi

    mov     [edi+3],cl
    dec     edi

memcpy_backward_bw2:
    mov     cl,[esi+3]
    dec     esi

    mov     [edi+3],cl
    dec     edi

memcpy_backward_bw1:
    mov     cl,[esi+3]

    mov     [edi+3],cl
memcpy_backward_bdone:
    mov     eax,4+8[esp]    ;return p1

    pop     esi
    pop     edi
    ret

memcpy_backward_use_dword:
    std
    rep     movsd
    cld
    jmp     memcpy_backward_btable[edx*4]

_memcpy     endp

    endcode    memcpy
    end

