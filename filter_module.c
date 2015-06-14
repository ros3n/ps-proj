#include <filter_module.h>

struct sk_buff *sock_buff;
struct iphdr *ip_header;
static struct nf_hook_ops nfho;

static unsigned int filter_hook(unsigned int hooknum,
	       		struct sk_buff *skb,
				const struct net_device *in,
				const struct net_device *out,
				int (*okfn)(struct sk_buff *))
{
	sock_buff = skb;

	if (!sock_buff) {
		  return NF_ACCEPT;
	}
  else
  {
      int pid = (int)NETLINK_CREDS(sock_buff)->pid;
      printk(KERN_INFO "sender PID: %d\n", pid);

  //
		// ip_header = (struct iphdr *)skb_network_header(sock_buff);
	// 	if (!ip_header)
  //   {
	// 		return NF_ACCEPT;
	// 	}
  //   else
  //   {
	// 		if (ip_header->protocol == 153)
  //     {
	// 			return NF_DROP;
	// 		}
  //     else
  //     {
	 return NF_ACCEPT;
	// 		}
	// 	}
	}
}

static int __init filter_init(void) {
    printk(KERN_INFO "Hello world! I'ma a trivial module!\n");

    nfho.hook     = filter_hook;
   	nfho.hooknum  = 1;
  	nfho.pf       = PF_INET;
  	nfho.priority = NF_IP_PRI_FIRST;
  	nf_register_hook(&nfho);

    return 0;
}

static void __exit filter_exit(void) {
    printk(KERN_INFO "Oh no, why are you doing this to me? Argh!\n");
    nf_unregister_hook(&nfho);
}
