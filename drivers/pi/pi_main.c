#include "linux/module.h"
#include "linux/init.h"
#include "linux/fs.h"
#include "linux/cdev.h"
#include "linux/device.h"

#include "pi.h"

#define DEV_NAME    "PI"

static dev_t dev_maj;
static struct class *pi_class;
static struct cdev  pi_cdev;

static struct file_operations pi_fops = {
    .owner = THIS_MODULE,
    .open = pi_open,
    .release = pi_release,
    .compact_ioctl = pi_ioctl
};

static int __init pi_init(void){
    printk(KERN_NOTICE "Loading PI module\n");

    int result = alloc_chrdev_region(&dev_maj, 0, 1, DEV_NAME);
    if(result < 0){
        printk(KERN_WARNING "PI: Failed to allocate majnum. error code: %d\n", result);
        return result;
    }

    printk(KERN_NOTICE "PI: registered with Major number: %d", MAJOR(dev_maj));
    
    pi_class = class_create(DEV_NAME);
    if(IS_ERR(pi_class)){
        unregister_chrdev_region(dev_maj, 1);
        printk(KERN_ALERT "PI: Failed to create device class\n");
        return PTR_ERR(pi_class);
    }

    if (IS_ERR(device_create(pi_class, NULL, dev_maj, NULL, DEV_NAME))) {
        class_destroy(pi_class);
        unregister_chrdev_region(dev_maj, 1);
        printk(KERN_ALERT "PI: Failed to create the device\n");
        return -1;
    }

    cdev_init(&pi_cdev, &pi_fops);
    result = cdev_add(&pi_cdev, dev_maj, 1);
    if (result < 0) {
        device_destroy(pi_class, dev_maj);
        class_destroy(pi_class);
        unregister_chrdev_region(dev_maj, 1);
        printk(KERN_ALERT "PI: Failed to add cdev: %d\n", result);
        return result;
    }

    printk(KERN_INFO "PI: Device created successfully\n");
    
    return 0;
}

static void __exit pi_drop(void){
    dev_t tmp_dev = MKDEV(dev_maj, 0);

    cdev_del(&pi_cdev);
    device_destroy(pi_class, tmp_dev);
    class_destroy(pi_class);
    unregister_chrdev_region(dev_maj, 1);

    printk(KERN_INFO "PI: Device unregistered\n");
}

module_init(pi_init);
module_exit(pi_drop);

MODULE_LICENSE("GPL");

