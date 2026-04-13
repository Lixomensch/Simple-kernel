.section .text

.extern isr_handler
.extern irq_handler

.macro isr_no_err_stub num
isr_stub_\num:
    push $0
    push $\num
    jmp isr_common
.endm

.macro isr_err_stub num
isr_stub_\num:

    push $\num
    jmp isr_common
.endm

.macro irq_stub num
isr_stub_\num:
    push $0
    push $\num
    jmp irq_common
.endm

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_err_stub    21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_err_stub    29
isr_err_stub    30
isr_no_err_stub 31

irq_stub 32
irq_stub 33
irq_stub 34
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47

.global isr_stub_table
isr_stub_table:
.long isr_stub_0, isr_stub_1, isr_stub_2, isr_stub_3, isr_stub_4, isr_stub_5, isr_stub_6, isr_stub_7
.long isr_stub_8, isr_stub_9, isr_stub_10, isr_stub_11, isr_stub_12, isr_stub_13, isr_stub_14, isr_stub_15
.long isr_stub_16, isr_stub_17, isr_stub_18, isr_stub_19, isr_stub_20, isr_stub_21, isr_stub_22, isr_stub_23
.long isr_stub_24, isr_stub_25, isr_stub_26, isr_stub_27, isr_stub_28, isr_stub_29, isr_stub_30, isr_stub_31

.long isr_stub_32, isr_stub_33, isr_stub_34, isr_stub_35, isr_stub_36, isr_stub_37, isr_stub_38, isr_stub_39
.long isr_stub_40, isr_stub_41, isr_stub_42, isr_stub_43, isr_stub_44, isr_stub_45, isr_stub_46, isr_stub_47

isr_common:
    pusha

    mov %esp, %eax
    push %eax
    call isr_handler
    pop %eax
    popa
    add $8, %esp
    iret

irq_common:
    pusha
    mov %esp, %eax
    push %eax
    call irq_handler
    pop %eax
    popa
    add $8, %esp
    iret
