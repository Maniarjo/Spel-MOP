# Exports
.global init_vector_table
# Imports
.extern systick_handler
.extern exti_handler

# Inform linker where to put the code
.section .text

# Vector table including:
# - SysTick (IRQ no. 12)
# - EXTI0   (IRQ no. 22)
# - EXTI1   (IRQ no. 23)
# - EXTI2   (IRQ no. 24)
# - EXTI3   (IRQ no. 25)
.align 2
vector_table:
  .org vector_table + 12 * 4
  j systick_handler
  .org vector_table + 22 * 4
  j exti_handler
  .org vector_table + 23 * 4
  j exti_handler
  .org vector_table + 24 * 4
  j exti_handler
  .org vector_table + 25 * 4
  j exti_handler

.align 4
init_vector_table:
  la t0, vector_table
  # Use vectored mode (two least significant bits set to 01)
  ori t0, t0, 1
  csrw mtvec, t0
  ret
