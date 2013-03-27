#include <linux/interrupt.h>
#include "sim.h"
#include "sim-assert.h"

void tasklet_init(struct tasklet_struct *t,
                  void (*func)(unsigned long), unsigned long data)
{
  t->next = NULL;
  t->state = 0;
  atomic_set(&t->count, 0);
  t->func = func;
  t->data = data;
}

void tasklet_kill(struct tasklet_struct *t)
{
  // theoretically, called from user context
  while (test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) {
    do {
      sim_task_yield ();
    } while (test_bit(TASKLET_STATE_SCHED, &t->state));
  }
  clear_bit(TASKLET_STATE_SCHED, &t->state);
}
static void trampoline (void *context)
{
  struct tasklet_struct *tasklet = context;
  // allow the tasklet to re-schedule itself
  sim_assert (tasklet->next != 0);
  tasklet->next = 0;
  if (atomic_read (&tasklet->count) == 0)
    {
      // this tasklet is enabled so, we run it.
      test_and_clear_bit (TASKLET_STATE_SCHED, &tasklet->state);
      tasklet->func (tasklet->data);
    }
}
void __tasklet_schedule(struct tasklet_struct *t)
{
  // Note: no need to set TASKLET_STATE_SCHED because it is set by caller.
  sim_assert (t->next == 0);
  // run the tasklet at the next immediately available opportunity.
  void *event = sim_event_schedule_ns (0, &trampoline, t);
  t->next = event;
}
