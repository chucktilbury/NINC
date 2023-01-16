; comment
; comment

number sadf
const number oieruio = 123


start:
    add r1, r3, r12
lab1:
    not r10, r2
    call end
    exit
$lab2$:
    return
    br lab1
    nop
    nop
    jmp $lab2$
    trap 8188
    nop
    br start
    push sadf
    push r12
;----------------
; comment
;----------------
    store oieruio, r13
    load r13, sadf
    peek r0, r15, 18
    nop nop nop
    load r12, -123.45e-2
    load r11, "this is a string"
    load r0, false
    load r0, r0


end:
    NOP
