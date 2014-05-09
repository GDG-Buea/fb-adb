#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "util.h"
#include "proto.h"

struct channel;

enum channel_names {
    FROM_PEER,
    TO_PEER,
    NR_SPECIAL_CH = TO_PEER
};

struct adbx_sh {
    size_t max_outgoing_msg;
    unsigned nrch;
    struct channel** ch;
    void (*process_msg)(struct adbx_sh* sh, struct msg mhdr);
};

void queue_message_synch(struct adbx_sh* sh, struct msg* m);
void io_loop_init(struct adbx_sh* sh);
void io_loop_pump(struct adbx_sh* sh);
void io_loop_do_io(struct adbx_sh* sh);
void adbx_sh_process_msg(struct adbx_sh* sh, struct msg mhdr);

void read_cmdmsg(struct adbx_sh* sh,
                 struct msg mhdr,
                 void* mbuf,
                 size_t msz);

#define PUMP_WHILE(_sh, _c)                     \
    ({                                          \
        struct adbx_sh* _m = (_sh);             \
        io_loop_pump(_m);                       \
        while ((_c)) {                          \
            io_loop_do_io(_m);                  \
            io_loop_pump(_m);                   \
        }                                       \
    })

typedef size_t (*reader)(int, void*,size_t);
struct msg* read_msg(int fd, reader rdr);
