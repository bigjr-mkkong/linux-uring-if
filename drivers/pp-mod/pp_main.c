#include "linux/device/class.h"
#include "linux/kdev_t.h"
#include "linux/printk.h"
#include "linux/uaccess.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/acct.h>
#include <linux/device.h>

#define DEV_NAME "pp_mod"
#define CLASS_NAME "pp_class"

static int major;
static struct class *pp_class = NULL;
static struct device *pp_dev = NULL;

static char state;

static ssize_t pp_read(struct file *fp, char __user *ubuf, size_t ulen, loff_t *oft) {
    
    if(copy_to_user(ubuf, &state, 1))
        return -EFAULT;

    return 1;
}

static ssize_t pp_write(struct file *fp, const char __user *ubuf, size_t, loff_t *oft) {
    if(copy_from_user(&state, ubuf, 1))
        return EFAULT;

    return 1;
}

static int pp_open(struct inode *, struct file *) {
    state = 'A';
    return 0;
}

static int pp_release(struct inode *, struct file *) {
    return 0;
}

static struct file_operations pp_fops = {
    .owner = THIS_MODULE,
    .open = pp_open,
    .release = pp_release,
    .read = pp_read,
    .write = pp_write,
};

static int __init pp_init(void) {
    major = register_chrdev(0, DEV_NAME, &pp_fops);
    if (major < 0) {
        pr_err("Failed to register char device\n");
        return major;
    }

    pp_class = class_create(CLASS_NAME);
    if (IS_ERR(pp_class)) {
        unregister_chrdev(major, DEV_NAME);
        return PTR_ERR(pp_class);
    }

    pp_dev = device_create(pp_class, NULL, MKDEV(major, 0), NULL, DEV_NAME);
    if (IS_ERR(pp_dev)) {
        class_destroy(pp_class);
        unregister_chrdev(major, DEV_NAME);
        return PTR_ERR(pp_dev);
    }

    pr_info("/dev/%s created. Major: %d\n", DEV_NAME, major);
    printk("pp_mod successfully registered\n");
    return 0;
}

static void __exit pp_exit(void) {
    device_destroy(pp_class, MKDEV(major, 0));
    class_destroy(pp_class);
    unregister_chrdev(major, DEV_NAME);
    printk("pp_mod successfully unregistered\n");
}

module_init(pp_init);
module_exit(pp_exit);

MODULE_LICENSE("GPL");
