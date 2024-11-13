#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/io_uring.h>
#include <uapi/linux/io_uring.h>
#include <linux/spinlock.h>

#include "io_uring.h"
#include "pengops.h"

static DEFINE_SPINLOCK(pp_lock);
unsigned long ping_pong_cnt;

int io_prep_pengpush(struct io_kiocb *req, const struct io_uring_sqe *sqe){
    unsigned long flag = 0;
    spin_lock_irqsave(&pp_lock, flag);
    ping_pong_cnt += 1;
    spin_unlock_irqrestore(&pp_lock, flag);
	return 0;
}

int io_pengpush(struct io_kiocb *req, unsigned int issue_flags){
    int *ptr = u64_to_user_ptr(req->cqe.user_data);
    unsigned long flag = 0;
    unsigned long resp_data;
    if(ping_pong_cnt > 0){
        resp_data = 0xc0ffee;

        spin_lock_irqsave(&pp_lock, flag);
        ping_pong_cnt -= 1;
        spin_unlock_irqrestore(&pp_lock, flag);

    }else{
        resp_data = 0;
    }

	if (copy_to_user(ptr, &resp_data, sizeof(unsigned long)))
		return -EFAULT;
	return IOU_OK;
}

/*
 * io_prep_pengop() and io_pengop() are reserved for future needs
 */
int io_prep_pengpop(struct io_kiocb *req, const struct io_uring_sqe *sqe){
	return 0;
}

int io_pengpop(struct io_kiocb *req, unsigned int issue_flags){
	return IOU_OK;
}
