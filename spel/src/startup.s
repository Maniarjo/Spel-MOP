.extern main, _crt_init, _crt_deinit
.section .start_section

# Startpoint för programmet
_start: 
  # Sätt stack pointer till toppen av RAM
  la sp, 0x2001C000
  
  # Anropa C runtime init
  # Sätter upp .data och .bss sections, startar PLL osv
  jal _crt_init

  # Sätt up för interrupt-mode och hoppa till main
  # Vi sätter MPIE=1 så interrupts är enabled vid main()
  la t0, main       # main address
  la ra, exit       # return address om main returnerar
  csrw mepc, t0     # sätt exception program counter
  
  # Status register setup:
  # MPP = Machine mode (bit 11:10)
  # MPIE = Enable interrupts (bit 7)
  # FS = FPU initial (bit 14:13)
  li t1, (3 << 11) | (1 << 7) | (1 << 13)
  csrw mstatus, t1
  
  # Jump till main med interrupts enabled
  mret

# Cleanup om main returnerar (borde inte hända)
exit: 
  jal _crt_deinit
  j .  # infinite loop
