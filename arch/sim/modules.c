#include "sim-assert.h"
#include <linux/moduleparam.h>
#include <linux/kmod.h>
#include <linux/module.h>

int modules_disabled = 0;
char modprobe_path[KMOD_PATH_LEN] = "/sbin/modprobe";

static struct kernel_param g_empty_param_buffer;
// we make the array empty by default because, really, we don't need
// to look at the builtin params
struct kernel_param *__start___param = &g_empty_param_buffer;
struct kernel_param *__stop___param = &g_empty_param_buffer;

const struct module_version_attribute no_modules[1] = {
		{
		.mattr	= {
			.attr	= {
				.name	= "version",
				.mode	= S_IRUGO,
			},
			.show	= __modver_version_show,
		},
		.module_name	= KBUILD_MODNAME,
		.version	= "V0",
	}
};

const struct module_version_attribute **__start___modver = &no_modules;
const struct module_version_attribute **__stop___modver  = &no_modules;

static ssize_t store_uevent(struct module_attribute *mattr,
			    struct module_kobject *mk,
			    const char *buffer, size_t count)
{
	sim_assert (false);
	return 0;
}

struct module_attribute module_uevent =
	__ATTR(uevent, 0200, NULL, store_uevent);

int __request_module(bool wait, const char *fmt, ...)
{
  // we really should never be trying to load modules that way.
  return 0;
}

struct ctl_table usermodehelper_table[] = {
	{ }
};
