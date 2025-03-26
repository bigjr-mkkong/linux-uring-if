#ifndef __PI_H__
#define __PI_H__

#include "linux/io_uring.h"

/*
 * Each Process only have one pair of SQ/CQ, and each SQ/CQ only need one kthread to handle
 */

#define PI_CMD_NEWRING  0x1

struct uring_info{
    struct io_uring_sqe *sq;
};

int pi_open(struct inode *inode, struct file *file);
long pi_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
int pi_release(struct inode *inode, struct file *file);

#endif
