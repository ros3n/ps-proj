#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/netlink.h>

#define BUFF_SIZE 20

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafal Rozak");
MODULE_DESCRIPTION("");

struct net_user {
  struct net_user *next;
  int uid;
};


int __init filter_init(void);
void __exit filter_exit(void);

module_init(filter_init);
module_exit(filter_exit);

ssize_t filter_write(struct file *filp, const char *user_buf, size_t count, loff_t *f_pos);
int filter_open(struct inode *inode, struct file *filp);
int filter_release(struct inode *inode, struct file *filp);
void append(struct net_user *list, struct net_user *user);
int find(struct net_user *list, int uid);
void free_list(struct net_user *list);

struct file_operations filter_fops = {
    write: filter_write,
    open: filter_open,
    release: filter_release
};
