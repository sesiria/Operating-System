MBT_HDR_FLAGS       EQU     0x00010003
MBT_HDR_MAGIC       EQU     0x1BADB002
global              _start
extern              inithead_entry
[section .text]
[bits 32]
_start:
    jmp _entry
align 4
mbt_hdr:
    dd      MBT_HDR_MAGIC
    dd      MBT_HDR_FLAGS
    dd      -(MBT_HDR_MAGIC + MBT_HDR_FLAGS)
    dd      mbt_hdr
    dd      _start
    dd      0
    dd      0
    dd      _entry
    ;
    ; multiboot header
    ;
ALIGN 8
mbhdr:
    DD      0xE85250D6
    DD      0
    DD      mhdrend - mbhdr
    DD      -(0xE85250D6 + 0 + (mhdrend - mbhdr))
    DD      2, 0
    DD      24
    DD      mbhdr
    DD      _start
    DD      0
    DD      0
    DD      3, 0
    DD      12
    DD      _entry
    DD      0
    DW      0, 0
    DD      8
mhdrend:

_entry:
    cli                 ; close irq

    in      al,     0x70
    or      al,     0x80
    out     0x70,   al        ; close unblockable irq

    lgdt [GDT_PTR]      ; add GDT address to GDTR register
    jmp     dword 0x8 :_32bits_mode ; long jump and refresh the shadow register

_32bits_mode:
    mov     ax,     0x10
    mov     ds,     ax
    mov     ss,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax
    xor     eax,    eax
    xor     ebx,    ebx
    xor     ecx,    ecx
    xor     edx,    edx
    xor     edi,    edi
    xor     esi,    esi
    xor     ebp,    ebp
    xor     esp,    esp
    mov     esp,    0x7c00     ; set top of the stack fo 0x7c00
    call inithead_entry ; call the inithead_entry function implemented in inithead.c
    jmp 0x200000        ; jump to 0x200000 address

; GDT global segment descriptor table
GDT_START:
knull_dsc:  dq      0
kcode_dsc:  dq      0x00cf9e000000ffff
kdata_dsc:  dq      0x00cf92000000ffff
k16cd_dsc:  dq      0x00009e000000ffff    ; 16bit code segment descriptor
k16da_dsc:  dq      0x000092000000ffff    ; 16bit data segment descriptor
GDT_END:
GDT_PTR:
GDTLEN      dw      GDT_END-GDT_START-1       ; boundary of GDT
GDTBASE     dd      GDT_START
