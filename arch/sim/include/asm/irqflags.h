#ifndef _ASM_SIM_IRQFLAGS_H
#define _ASM_SIM_IRQFLAGS_H

unsigned long __raw_local_save_flags(void);
void raw_local_irq_restore(unsigned long flags);

#define __raw_local_save_flags __raw_local_save_flags
#define raw_local_irq_restore raw_local_irq_restore

#include <asm-generic/irqflags.h>

#endif /* _ASM_SIM_IRQFLAGS_H */
