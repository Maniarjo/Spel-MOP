# Vector table setup för interrupts

.global init_vector_table
.extern systick_handler
.extern exti_handler

.section .text

# Interrupt vector table
# handler för systick (IRQ 12) och EXTI0-3 (IRQ 22-25)
.align 2
vector_table:
  # Plats för systick interrupt (offset 12*4 bytes)
  .org vector_table + 12 * 4
  j systick_handler
  
  # Plats för EXTI0 interrupt (offset 22*4 bytes)
  .org vector_table + 22 * 4
  j exti_handler
  
  # Plats för EXTI1 interrupt (offset 23*4 bytes)
  .org vector_table + 23 * 4
  j exti_handler
  
  # Plats för EXTI2 interrupt (offset 24*4 bytes)
  .org vector_table + 24 * 4
  j exti_handler
  
  # Plats för EXTI3 interrupt (offset 25*4 bytes)
  .org vector_table + 25 * 4
  j exti_handler

# Init vektor table
.align 4
init_vector_table:
  # Ladda vektor table adress
  la t0, vector_table
  # Sätt vectored mode (bit 0 = 1)
  ori t0, t0, 1
  # Skriv till mtvec register
  csrw mtvec, t0
  # Returnera
  ret
