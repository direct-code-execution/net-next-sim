#include <linux/workqueue.h>
#include "sim.h"
#include "sim-assert.h"

/**
 * All these functions work on the global per-cpu workqueue.
 * Since we model only a single cpu, we have a single 
 * global workqueue.
 */

static LIST_HEAD(g_work);

struct wq_barrier {
  struct work_struct  work;
  struct SimTask      *waiter;
};


static void workqueue_barrier_fn (struct work_struct *work)
{
  struct wq_barrier *barr = container_of(work, struct wq_barrier, work);
  sim_task_wakeup (barr->waiter);
}

static void
workqueue_function (void *context)
{
  while (true)
    {
      sim_task_wait ();
      while (!list_empty (&g_work))
       {
         struct work_struct *work = list_first_entry(&g_work,
                                                     struct work_struct, entry);
         if (work->entry.prev != LIST_POISON2) {
           work_func_t f = work->func;
           __list_del (work->entry.prev, work->entry.next);
           work_clear_pending (work);
           f(work);
         }
       }
    }
}

static struct SimTask *workqueue_task (void)
{
  static struct SimTask *g_task = 0;
  if (g_task == 0)
    {
      g_task = sim_task_start (&workqueue_function, 0);
    }
  return g_task;
}

static int flush_entry (struct list_head *prev)
{
  struct wq_barrier barr;
  int active = 0;

  if (!list_empty(&g_work))
    {
      active = 1;
      INIT_WORK_ON_STACK(&barr.work, &workqueue_barrier_fn);
      __set_bit(WORK_STRUCT_PENDING, work_data_bits(&barr.work));
      list_add(&barr.work.entry, prev);
      sim_task_wakeup (workqueue_task ());
      sim_task_wait ();
    }

  return active;
}

static void delayed_work_timer_fn(unsigned long data)
{
  struct delayed_work *dwork = (struct delayed_work *)data;
  struct work_struct *work = &dwork->work;

  schedule_work (work);
}

/**
 * @work: work to queue
 *
 * Returns 0 if @work was already on a queue, non-zero otherwise.
 *
 * We queue the work, the caller must ensure it
 * can't go away.
 */
int schedule_work(struct work_struct *work)
{
  int ret = 0;

  if (!test_and_set_bit (WORK_STRUCT_PENDING, work_data_bits(work))) {
      list_add_tail (&work->entry, &g_work);
      sim_task_wakeup (workqueue_task ());
      ret = 1;
  }
  return ret;
}
void flush_scheduled_work(void)
{
  flush_entry (g_work.prev);
}
int flush_work(struct work_struct *work)
{
  return flush_entry (&work->entry);
}
int cancel_work_sync(struct work_struct *work)
{
  int retval = 0;
  if (!test_and_set_bit(WORK_STRUCT_PENDING, work_data_bits(work)))
    {
      // work was not yet queued
      return 0;
    }
  if (!list_empty(&work->entry))
    {
      // work was queued. now unqueued.
      if (work->entry.prev != LIST_POISON2) {
        list_del_init(&work->entry);
        work_clear_pending (work);
        retval = 1;
      }
    }
  return retval;
}
int schedule_delayed_work(struct delayed_work *dwork, unsigned long delay)
{
  int ret = 0;
  struct timer_list *timer = &dwork->timer;
  struct work_struct *work = &dwork->work;
  if (delay == 0)
    {
      return schedule_work (work);
    }

  if (!test_and_set_bit(WORK_STRUCT_PENDING, work_data_bits(work)))
    {
      sim_assert (!timer_pending (timer));
      /* This stores cwq for the moment, for the timer_fn */
      timer->expires = jiffies + delay;
      timer->data = (unsigned long)dwork;
      timer->function = delayed_work_timer_fn;
      add_timer(timer);
      ret = 1;
    }
  return ret;
}
