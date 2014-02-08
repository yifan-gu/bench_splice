#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/time.h>
#include <unistd.h>

#define MICROSECOND (1000000)

#define CHECK_ERROR(status, func)                                       \
        if ((func) < 0) {                                               \
                error_at_line((status), errno, __FILE__, __LINE__, NULL); \
        }

void setup(int *fd_in, int *fd_out, int *pipefd);
void run_splice(int fd_in, int fd_out, int *pipefd);
void run_readwrite(int fd_in, int fd_out, char *buf);
void run_sendfile(int fd_in, int fd_out);
int get_diff(struct timeval *before, struct timeval *later);
void print_result(int diff);
