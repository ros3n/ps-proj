#include <filter_module.h>

static int __init filter_init(void) {
    printk(KERN_INFO "Hello world! I'ma a trivial module!\n");
    return;
}

static void __exit filter_exit(void) {
    printk(KERN_INFO "Oh no, why are you doing this to me? Argh!\n");
}
