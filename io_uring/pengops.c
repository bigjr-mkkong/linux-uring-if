//#define COHORT
#include "linux/io_uring_types.h"
#include "linux/printk.h"
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/io_uring.h>
#include <uapi/linux/io_uring.h>
#include <linux/spinlock.h>

#include "io_uring.h"
#include "pengops.h"

#ifdef COHORT
#include "../drivers/cohort_mmu/cohort_syscall.h"
#endif

struct peng_req req_buf;

int io_prep_pengpush(struct io_kiocb *req, const struct io_uring_sqe *sqe){
    unsigned long *pg_cmd = io_kiocb_to_cmd(req, unsigned long);
    struct peng_req *ureq_ptr = u64_to_user_ptr(sqe->addr);

    if(*pg_cmd >= PENG_OP_LAST)
        return -EINVAL;

    if(copy_from_user(&req_buf, ureq_ptr, sizeof(struct peng_req))){
        return -EFAULT;
    }

	return 0;
}

int io_pengpush(struct io_kiocb *req, unsigned int issue_flags){
    unsigned long *pg_cmd = io_kiocb_to_cmd(req, unsigned long);
    switch(*pg_cmd){
        case RV_CONF_IOMMU:
            printk("This is RV_CONF_IOMMU\n");
#ifndef COHORT
            for(size_t i=0; i<COHORT_MAX_ARGS; i++)
                printk("arg[%ld] is %lx\n", i, req_buf.args[i]);
            //Do work
#else
            uint64_t c_head = req_buf.args[0];
            uint64_t q_head = req_buf.args[1];
            uint64_t acc_ptr = req_buf.args[2];
            uint64_t backoff_val = req_buf.args[3];

            cohort_mn_register(c_head, p_head, acc_ptr, backoff_val);
#endif
            req->cqe.user_data = req_buf.user_data;
            io_req_set_res(req, 0, 0);
            break;

        case RV_CONF_IOMMU_EXIT:
            printk("This is RV_CONF_IOMMU_EXIT\n");
#ifndef COHORT
            for(size_t i=0; i<COHORT_MAX_ARGS; i++)
                printk("arg[%ld] is %lx\n", i, req_buf.args[i]);
#else
            cohort_mn_exit();
#endif
            req->cqe.user_data = req_buf.user_data;
            io_req_set_res(req, 0, 0);
            break;

        default:
            printk("Invalid argument");
            return -EINVAL;
    }

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


