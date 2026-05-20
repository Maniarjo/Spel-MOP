#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

// Function prototypes

// Initialize vector table
// - SysTick: systick_handler()
// - EXTI0:   exti_handler()
// - EXTI1:   exti_handler()
// - EXTI2:   exti_handler()
// - EXTI3:   exti_handler()
void init_vector_table();

#endif
