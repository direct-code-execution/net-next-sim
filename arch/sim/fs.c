#include <linux/fs.h>
#include <linux/splice.h>
#include <linux/mount.h>
#include "sim-assert.h"

struct super_block;
void kill_anon_super(struct super_block *sb)
{
  sim_assert (false);
}
int get_sb_pseudo(struct file_system_type *type, char *str,
		  const struct super_operations *ops, unsigned long z,
		  struct vfsmount *mnt)
{
  // called from sockfs_get_sb by kern_mount_data
  mnt->mnt_sb->s_root = 0;
  mnt->mnt_sb->s_op = ops;
  return 0;
}
struct inode *new_inode(struct super_block *sb)
{
  // call sock_alloc_inode through s_op
  struct inode *inode = sb->s_op->alloc_inode(sb);
  inode->i_ino = 0;
  inode->i_sb = sb;
  atomic_set (&inode->i_count, 1);
  inode->i_state = 0;
  return inode;
}
void iput(struct inode *inode)
{
  if (atomic_dec_and_test (&inode->i_count))
    {
      // call sock_destroy_inode
      inode->i_sb->s_op->destroy_inode (inode);
    }
}
void inode_init_once(struct inode *inode)
{
  memset(inode, 0, sizeof(*inode));
}
struct vfsmount *kern_mount_data(struct file_system_type *fs, void *data)
{
  // minimum stuff needed by the network stack to avoid segfaults.
  static struct vfsmount mnt;
  static struct super_block sb;
  mnt.mnt_sb = &sb;
  // initialize the superblock
  fs->get_sb (fs, 0, 0, 0, &mnt);
  return &mnt;
}

int register_filesystem(struct file_system_type *fs)
{
  // We don't need to register anything because we never really implement 
  // any kind of filesystem. return 0 to signal success.
  return 0;
}

int alloc_fd(unsigned start, unsigned flags)
{
  sim_assert (false);
  return 0;
}
void fd_install(unsigned int fd, struct file *file)
{
  sim_assert (false);
}
void put_unused_fd(unsigned int fd)
{
  sim_assert (false);
}

struct file *alloc_file(struct path *path, fmode_t mode,
			const struct file_operations *fop)
{
  sim_assert (false);
  return 0;
}

struct file *fget(unsigned int fd)
{
  sim_assert (false);
  return 0;
}
struct file *fget_light(unsigned int fd, int *fput_needed)
{
  sim_assert (false);
  return 0;
}
void fput(struct file *file)
{
  sim_assert (false);
}

struct dentry * d_alloc(struct dentry *entry, const struct qstr *str)
{
  sim_assert (false);
  return 0;
}
void d_instantiate(struct dentry *entry, struct inode *inode)
{
  sim_assert (false);
}
char *dynamic_dname(struct dentry *dentry, char *buffer, int buflen,
		    const char *fmt, ...)
{
  sim_assert (false);
  return 0;
}

struct dentry_stat_t dentry_stat;
struct files_stat_struct files_stat;
struct inodes_stat_t inodes_stat;

pid_t f_getown(struct file *filp)
{
  sim_assert (false);
  return 0;
}
int f_setown(struct file *filp, unsigned long arg, int force)
{
  sim_assert (false);
  return 0;
}


void kill_fasync(struct fasync_struct **fs, int a, int b)
{
  sim_assert (false);
}
int fasync_helper(int a, struct file *file, int b, struct fasync_struct **c)
{
  sim_assert (false);
  return 0;
}
loff_t no_llseek(struct file *file, loff_t offset, int origin)
{
  sim_assert (false);
  return 0;
}
void path_put(struct path *path)
{
  sim_assert (false);
}
int simple_statfs(struct dentry *entry, struct kstatfs *statfs)
{
  sim_assert (false);
  return 0;
}
long sys_close(unsigned int fd)
{
  sim_assert (false);
  return 0;
}

ssize_t splice_to_pipe(struct pipe_inode_info *info,
		       struct splice_pipe_desc *desc)
{
  sim_assert (false);
  return 0;
}
int splice_grow_spd(struct pipe_inode_info *info, struct splice_pipe_desc *desc)
{
  sim_assert (false);
  return 0;
}
void splice_shrink_spd(struct pipe_inode_info *info,
		       struct splice_pipe_desc *desc)
{
  sim_assert (false);
  return 0;
}

ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe,
				struct file *out, loff_t *poff, size_t len, unsigned int flags)
{
  sim_assert (false);
  return 0;
}
void *generic_pipe_buf_map(struct pipe_inode_info *pipe,
                           struct pipe_buffer *buf, int atomic)
{
  sim_assert (false);
  return 0;
}

void generic_pipe_buf_unmap(struct pipe_inode_info *pipe, struct pipe_buffer *buf, void *address)
{
  sim_assert (false);
}

int generic_pipe_buf_confirm(struct pipe_inode_info *pipe, struct pipe_buffer *buf)
{
  sim_assert (false);
  return 0;
}
