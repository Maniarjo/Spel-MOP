.extern main, _crt_init, _crt_deinit
.section .start_section

_start: 
    # Set stackpointer to the top of RAM
    la sp,0x2001C000
    # Call C runtime initialization
    # This sets up the .data and .bss sections, starts PLL and clocks, etc. 
    jal _crt_init

    # This is a fancy way of enabling interrupts and jumping to main.
    # (Since we set MPIE=1 and then `mret`, we go "back" to having interrupts enabled)
    la t0, main
    la ra, exit
    csrw mepc, t0
    li   t1, (3 << 11) | (1 << 7) | (1 << 13)  # MPP=Machine, MPIE=1, FS=Initial (enable FPU)
    csrw mstatus, t1
    mret

# Call C runtime de-initialization (never reached)
exit: 
    jal _crt_deinit
    j .
