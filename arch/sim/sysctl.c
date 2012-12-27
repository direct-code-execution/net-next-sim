#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/mman.h>
#include <linux/ratelimit.h>
#include "sim-assert.h"
#include "sim-types.h"

int drop_caches_sysctl_handler(struct ctl_table *table, int write,
			       void  *buffer, size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int lowmem_reserve_ratio_sysctl_handler(struct ctl_table *table, int write,
					void  *buffer, size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int min_free_kbytes_sysctl_handler(struct ctl_table *table, int write,
				   void  *buffer, size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}

int percpu_pagelist_fraction_sysctl_handler(struct ctl_table *table, int write,
					    void  *buffer, size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int dirty_background_ratio_handler(struct ctl_table *table, int write,
				   void  *buffer, size_t *lenp,
				   loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int dirty_background_bytes_handler(struct ctl_table *table, int write,
				   void  *buffer, size_t *lenp,
				   loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int dirty_ratio_handler(struct ctl_table *table, int write,
			void  *buffer, size_t *lenp,
			loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int dirty_bytes_handler(struct ctl_table *table, int write,
			void  *buffer, size_t *lenp,
			loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int dirty_writeback_centisecs_handler(struct ctl_table *table, int write,
				      void  *buffer, size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int scan_unevictable_handler(struct ctl_table *table, int write,
			     void __user *buffer,
			     size_t *length, loff_t *ppos)
{
  sim_assert (false);
  return 0;
}
int sched_rt_handler(struct ctl_table *table, int write,
		     void __user *buffer, size_t *lenp,
		     loff_t *ppos)
{
  sim_assert (false);
  return 0;
}

int sysctl_overcommit_memory = OVERCOMMIT_GUESS;
int sysctl_overcommit_ratio = 50;
int sysctl_panic_on_oom = 0;
int sysctl_oom_dump_tasks = 0;
int sysctl_oom_kill_allocating_task = 0;
int sysctl_nr_open = 1024*1024;
int sysctl_nr_open_min = BITS_PER_LONG;
int sysctl_nr_open_max = 1024 * 1024;
int sysctl_nr_trim_pages = 0;
int sysctl_drop_caches = 0;
int sysctl_lowmem_reserve_ratio[MAX_NR_ZONES-1] = {32};
int sysctl_vfs_cache_pressure = 100;
unsigned int sysctl_sched_child_runs_first = 0;
unsigned int sysctl_sched_compat_yield = 0;
unsigned int sysctl_sched_rt_period = 1000000;
int sysctl_sched_rt_runtime = 950000;

int vm_highmem_is_dirtyable;
unsigned long vm_dirty_bytes = 0;
int vm_dirty_ratio = 20;
int dirty_background_ratio = 10;
unsigned int dirty_expire_interval = 30 * 100;
unsigned int dirty_writeback_interval = 5 * 100;
unsigned long dirty_background_bytes = 0;
int percpu_pagelist_fraction = 0;
int panic_timeout = 0;
int panic_on_oops = 0;
int printk_delay_msec = 0;
DEFINE_RATELIMIT_STATE(printk_ratelimit_state, 5 * HZ, 10);

#define RESERVED_PIDS 300
int pid_max = PID_MAX_DEFAULT;
int pid_max_min = RESERVED_PIDS + 1;
int pid_max_max = PID_MAX_LIMIT;
int min_free_kbytes = 1024;
int max_threads = 100;
int laptop_mode = 0;

#define DEFAULT_MESSAGE_LOGLEVEL 4 /* KERN_WARNING */
#define MINIMUM_CONSOLE_LOGLEVEL 1 /* Minimum loglevel we let people use */
#define DEFAULT_CONSOLE_LOGLEVEL 7 /* anything MORE serious than KERN_DEBUG */
int console_printk[4] = {
  DEFAULT_CONSOLE_LOGLEVEL,       /* console_loglevel */
  DEFAULT_MESSAGE_LOGLEVEL,       /* default_message_loglevel */
  MINIMUM_CONSOLE_LOGLEVEL,       /* minimum_console_loglevel */
  DEFAULT_CONSOLE_LOGLEVEL,       /* default_console_loglevel */
};

int print_fatal_signals = 0;
unsigned int core_pipe_limit = 0;
int core_uses_pid = 0;
int vm_swappiness = 60;
int nr_pdflush_threads = 0;
unsigned long scan_unevictable_pages = 0;
int suid_dumpable = 0;
int page_cluster = 0;
int block_dump = 0;
int C_A_D = 0;
#include <linux/nsproxy.h>
struct nsproxy init_nsproxy;
#include <linux/reboot.h>
char poweroff_cmd[POWEROFF_CMD_PATH_LEN] = "/sbin/poweroff";
#include <linux/pipe_fs_i.h>
unsigned int pipe_max_pages =  PIPE_DEF_BUFFERS * 16;
unsigned int pipe_max_size = 1048576;
unsigned int pipe_min_size = PAGE_SIZE;

int pipe_proc_fn(struct ctl_table *table, int write, void __user *buf,
                 size_t *lenp, loff_t *ppos)
{
  return -1;
}

#include <linux/fs.h>

/**
 * Honestly, I don't understand half of that code.
 * It was modeled after fs/proc/proc_sysctl.c proc_sys_readdir
 */
static void iterate_recursive (const struct SimSysIterator *iter, struct ctl_table *table);

static void iterate_table_recursive (const struct SimSysIterator *iter, struct ctl_table *table)
{
  struct ctl_table *cur_table;
  for (cur_table = table; cur_table->procname != NULL; cur_table++)
    {
      if (table->child == 0)
	{
	  bool may_read = (table->mode & MAY_READ);
	  bool may_write = (table->mode & MAY_WRITE);
	  int flags = 0;
	  flags |= may_read?SIM_SYS_FILE_READ:0;
	  flags |= may_write?SIM_SYS_FILE_WRITE:0;
	  iter->report_file (iter, cur_table->procname, flags, (struct SimSysFile *)cur_table);
	}
      else
	{
	  iter->report_start_dir (iter, cur_table->procname);
	  iterate_recursive (iter, table->child);
	  iter->report_end_dir (iter);
	}
    }
}

static void iterate_recursive (const struct SimSysIterator *iter, struct ctl_table *table)
{
  struct ctl_table_header *root = sysctl_head_next (NULL);
  struct ctl_table_header *cur;

  iterate_table_recursive (iter, table);
  for (cur = root; cur != 0; cur = sysctl_head_next (cur))
    {
      if (cur->set != table)
	{
	  continue;
	}
      iterate_table_recursive (iter, cur->root);
    }

}



void sim_sys_iterate_files (const struct SimSysIterator *iter)
{
  /* FIXME !!! */
//	sim_assert (0);
	return;
  struct ctl_table_header *root = sysctl_head_next (NULL);
  iterate_recursive (iter, root->ctl_table);
}

int sim_sys_file_read (const struct SimSysFile *file, char *buffer, int size, int offset)
{
  struct ctl_table *table = (struct ctl_table *)file;
  loff_t ppos = offset;
  size_t result = size;
  int error;
  error = table->proc_handler (table, 0, buffer, &result, &ppos);
  return result;
}
int sim_sys_file_write (const struct SimSysFile *file, const char *buffer, int size, int offset)
{
  struct ctl_table *table = (struct ctl_table *)file;
  loff_t ppos = offset;
  size_t result = size;
  int error;
  error = table->proc_handler (table, 1, (char *)buffer, &result, &ppos);
  return result;
}
