%include "ldrasm.inc"
global      _start
global      realadr_call_entry
global      IDT_PTR
extern      ldrkrl_entry
[section .text]
[bits 32]
_start:
_entry:
    cli
    lgdt    [GDT_PTR]
    lidt    [IDT_PTR]
    jmp     dword 0x8 :_32bits_mode

_32bits_mode:
    mov     ax,     0x10    ; target code segment selector
    mov     ds,     ax
    mov     ss,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax
    mov     eax,    eax
    mov     ebx,    ebx
    mov     ecx,    ecx
    xor     edx,    edx
    xor     edi,    edi
    xor     esi,    esi
    xor     ebp,    ebp
    xor     esp,    esp
    mov     esp,    0x90000
    call    ldrkrl_entry
    xor     ebx,    ebx
    jmp     0x2000000
    jmp     $


realadr_call_entry:
    pushad                  ; save common register
    push    ds
    push    es
    push    fs
    push    gs              ; save four segment register
    call    save_eip_jmp    ; call save_eip_jmp
    pop     gs
    pop     fs
    pop     es
    pop     ds              ; recover four segment register
    popad                   ; recover common register
    ret
save_eip_jmp:
    pop     esi             ; pop eip and save to esi.
    mov     [PM32_EIP_OFF],     esi     ; save eip to specific memory address
    mov     [PM32_ESP_OFF],     esp     ; save esp to specific memory address
    jmp     dword far [cmpty_mode]      ; load data from cpmty_mode to CS: EIP
cmpty_mode:
    dd      0x1000
    dw      0x18
    jmp     $

GDT_START:
knull_dsc:  dq      0
kcode_dsc:  dq      0x00cf9a000000ffff      ; a - esp
kdata_dsc:  dq      0x00cf92000000ffff
k16cd_dsc:  dq      0x00009a000000ffff
k16da_dsc:  dq      0x000092000000ffff
GDT_END:

GDT_PTR:
GDTLEN      dw      GDT_END-GDT_START - 1   ; GDT boundary
GDTBASE     dd      GDT_START

IDT_PTR:
IDTLEN      dw      0x3ff
IDTBAS      dd      0
