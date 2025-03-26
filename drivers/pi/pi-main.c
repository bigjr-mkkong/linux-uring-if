#include "linux/module.h"
#include "linux/init.h"
#include "linux/fs.h"

static const char dev_name[] = "PI"; //Pengwing Interface
static int dev_maj = 0;

static struct file_operations pi_fops = {
    .owner = THIS_MODULE,
};

static int pi_init(void){
    printk(KERN_NOTICE "Loading PI module\n");
    
    int result = register_chrdev(0, dev_name, &pi_fops);
    if(result < 0){
        printk(KERN_WARNING "PI: Failed to register device. error code: %d\n", result);
        return result;
    }

    dev_maj = result;
    printk(KERN_NOTICE "Registered PI device with maj number: %d\n", dev_maj);

    return 0;
}

void pi_drop(void){
    printk(KERN_NOTICE "Dropping PI module\n");
    if(dev_maj != 0){
        unregister_chrdev(dev_maj, dev_name);
    }
}

module_init(pi_init);
module_exit(pi_drop);

MODULE_LICENSE("GPL");

