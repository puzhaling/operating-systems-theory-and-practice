#ifndef __dgram_narrow_space_h__
#define __dgram_narrow_space_h__

#define BUF_SIZE 100
#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

const char *sockpath = "/tmp/reciever";

#endif // __dgram_narrow_space_h__
