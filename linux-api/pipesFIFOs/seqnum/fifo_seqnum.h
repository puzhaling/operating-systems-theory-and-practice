#ifndef __fifo_seqnum_h__
#define __fifo_seqnum_h__

#include <sys/types.h>

#define SERVER_FIFO "/tmp/seqnum_sv"
#define SERVER_INIT_FILE "./seqnum_init.dat"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request {
        pid_t pid;
        int seqLen;
};

struct response {
        int seqNum;
};

#endif /* __fifo_seqnum_h__ */
