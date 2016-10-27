#include "mm_dramsim2.h"

#include <vpi_user.h>
#include <svdpi.h>

#define WORD_SIZE 8
#define LINE_SIZE 64
#define MEM_SIZE 0x10000000L
#define DRAMSIM

extern "C" void *memory_init(void)
{
    mm_t *mm;
    unsigned long size = 0x10000000L;

#ifdef DRAMSIM
    mm = (mm_t *) (new mm_dramsim2_t);
#else
    mm = (mm_t *) (new mm_magic_t);
#endif
    mm->init(size, WORD_SIZE, LINE_SIZE);

    return mm;
}

extern "C" void memory_tick(
        void *channel,

        unsigned char ar_valid,
        unsigned char *ar_ready,
        int ar_addr,
        int ar_id,
        int ar_size,
        int ar_len,

        unsigned char aw_valid,
        unsigned char *aw_ready,
        int aw_addr,
        int aw_id,
        int aw_size,
        int aw_len,

        unsigned char w_valid,
        unsigned char *w_ready,
        int w_strb,
        long long w_data,
        unsigned char w_last,

        unsigned char *r_valid,
        unsigned char r_ready,
        int *r_id,
        int *r_resp,
        long long *r_data,
        unsigned char *r_last,

        unsigned char *b_valid,
        unsigned char b_ready,
        int *b_id,
        int *b_resp)
{
    mm_t *mm = (mm_t *) channel;

    mm->tick(
        ar_valid,
        ar_addr,
        ar_id,
        ar_size,
        ar_len,

        aw_valid,
        aw_addr,
        aw_id,
        aw_size,
        aw_len,

        w_valid,
        w_strb,
        &w_data,
        w_last,

        r_ready,
        b_ready);

    *ar_ready = mm->ar_ready();
    *aw_ready = mm->aw_ready();
    *w_ready = mm->w_ready();
    *r_valid = mm->r_valid();
    *r_id = mm->r_id();
    *r_resp = mm->r_resp();
    *r_data = *((long *) mm->r_data());
    *r_last = mm->r_last();
    *b_valid = mm->b_valid();
    *b_id = mm->b_id();
    *b_resp = mm->b_resp();
}
