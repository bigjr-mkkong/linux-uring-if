#include "linux/module.h"
#include "linux/init.h"
#include "linux/fs.h"
#include "linux/cdev.h"
#include "linux/device.h"

#include "pi.h"

void kt_sendrecv()


int pi_open(struct inode *inode, struct file *filp){
    return 0;
}

long pi_ioctl(struct file *filp, unsigned int cmd, unsigned long arg){
    return 0;
}

int pi_release(struct inode *inode, struct file *filp){
    return 0;
}
