#ifndef _ASM_SIM_THREAD_INFO_H
#define _ASM_SIM_THREAD_INFO_H

#define TIF_NEED_RESCHED 1
#define TIF_SIGPENDING 2

struct thread_info
{
  __u32 flags;
  int preempt_count;
  struct task_struct *task;
};

struct thread_info *current_thread_info(void);
struct thread_info *alloc_thread_info (struct task_struct *task);
void free_thread_info (struct thread_info *ti);

#endif /* _ASM_SIM_THREAD_INFO_H */
