.global main
.global reset_handler

reset_handler:
    @ ldr r0, =_fdata
    @ ldr r1, =_data

    
    b main

