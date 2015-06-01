#include "sim.h"
#include <linux/random.h>

__u32 secure_tcpv6_sequence_number(__be32 *saddr, __be32 *daddr,
                                   __be16 sport, __be16 dport)
{
  return sim_random ();
}
__u32 secure_ip_id(__be32 daddr)
{
  return sim_random ();
}
__u32 secure_tcp_sequence_number(__be32 saddr, __be32 daddr,
                                 __be16 sport, __be16 dport)
{
  return sim_random ();
}
u64 secure_dccp_sequence_number(__be32 saddr, __be32 daddr,
                                __be16 sport, __be16 dport)
{
  u64 seq;
  seq = sim_random ();
  seq &= (1ull << 48) - 1;
  return seq;
}

u32 secure_ipv4_port_ephemeral(__be32 saddr, __be32 daddr, __be16 dport)
{
  return sim_random ();
}
u32 secure_ipv6_port_ephemeral(const __be32 *saddr, const __be32 *daddr,
                               __be16 dport)
{
  return sim_random ();
}
u32 random32(void)
{
  return sim_random ();
}

void get_random_bytes(void *buf, int nbytes)
{
  char *p = (char *)buf;
  int i;
  for (i = 0; i < nbytes; i++)
    {
      p[i] = sim_random ();
    }
}
void srandom32(u32 entropy)
{}

#include <linux/sysctl.h>

static int nothing;
struct ctl_table random_table[] = {
  {
    .procname       = "nothing",
    .data           = &nothing,
    .maxlen         = sizeof(int),
    .mode           = 0444,
    .proc_handler   = proc_dointvec,
  },
  {}
};
