#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafal Rozak");

ssize_t filter_write(struct file *filp, const char *user_buf, size_t count, loff_t *f_pos);
void filter_exit(void);
int filter_init(void);

struct file_operations filter_fops = {
    write: filter_write,
};

module_init(filter_init);
module_exit(filter_exit);
