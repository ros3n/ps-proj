#include "filter_module.h"

struct sk_buff *sock_buff;
struct iphdr *ip_header;
static struct nf_hook_ops nfho;
struct task_struct *curr;
struct net_user *users;
char *buff;

static unsigned int filter_hook(unsigned int hooknum,
	       		struct sk_buff *skb,
				const struct net_device *in,
				const struct net_device *out,
				int (*okfn)(struct sk_buff *))
{
	sock_buff = skb;

	if (!sock_buff)
{
		  return NF_ACCEPT;
	}
  else
  {   //
	curr = get_current();

        int uid = (int)(curr->cred->uid.val);

        if(uid == 0)
        {
						printk(KERN_INFO "sender %d accepted\n", uid);

            return NF_ACCEPT;
        }

        if(find(users, uid))
        {
            printk(KERN_INFO "sender %d accepted\n", uid);

            return NF_ACCEPT;
        }
        else
        {
            printk(KERN_INFO "sender %d not allowed to send packets\n", uid);

            return NF_DROP;
        }
  }
}

int __init filter_init(void)
{
		int result;

		result = register_chrdev(666, "filter", &filter_fops);

		if(result < 0)
		{
  			printk(KERN_INFO "Failed to register a chardev\n");
				return -1;
		}

    printk(KERN_INFO "Hello world! I'ma a trivial module!\n");

    nfho.hook     = filter_hook;
   	nfho.hooknum  = 0;
  	nfho.pf       = PF_INET;
  	nfho.priority = NF_IP_PRI_FIRST;
  	nf_register_hook(&nfho);

    users = (struct net_user *) kmalloc(sizeof(struct net_user *), GFP_KERNEL);

		if(!users)
		{
			printk(KERN_WARNING "Users list allocation failed\n");
			return -1;
		}

    users->next = NULL;
    users->uid = -1;

    buff = (char *) kmalloc(BUFF_SIZE, GFP_KERNEL);

		if(!buff)
		{
			printk(KERN_WARNING "Buffer allocation failed\n");
			return -1;
		}

    return 0;
}

void __exit filter_exit(void)
{
    printk(KERN_INFO "Oh no, why are you doing this to me? Argh!\n");
    nf_unregister_hook(&nfho);
}

ssize_t filter_write(struct file *filp, const char *user_buf, size_t count, loff_t *f_pos)
{
    int max = count;
    int err;

    if(max > BUFF_SIZE - 1)
    {
        max = BUFF_SIZE - 1;
    }

    err = copy_from_user(buff, user_buf, max);

    buff[max] = 0;

    char pass[] = "gimmienet";

    int res = strncmp(buff, pass, max);

    if (res == 0)
    {
        curr = get_current();
        struct net_user *new_user = (struct net_user *)kmalloc(sizeof(struct net_user *), GFP_KERNEL);

				if(!new_user)
				{
					printk(KERN_WARNING "User allocation failed\n");
					return -1;
				}

				new_user->next = NULL;
        new_user->uid = curr->cred->uid.val;
        append(users, new_user);
    }

    return max;
}

int filter_open(struct inode *inode, struct file *filp)
{
    return 0;
}

int filter_release(struct inode *inode, struct file *filp)
{
    return 0;
}

void append(struct net_user *list, struct net_user *user)
{
    while(list->next != NULL)
    {
        list = list->next;
    }

    list->next = user;
}

int find(struct net_user *list, int uid)
{
    while(list->next != NULL)
    {
        list = list->next;
        if (list->uid == uid)
        {
            return 1;
        }
    }

    return 0;
}
