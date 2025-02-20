.global main
.global reset_handler

reset_handler:
    ldr r0, =_stack
    mov sp, r0
    bl main

