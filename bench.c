#include "bench.h"

void run_readwrite(int, int, char *);

void setup(int *fd_in, int *fd_out, int *pipefd)
{
        char buf[BUFSIZE];
        
        *fd_in = open("/tmp/in", O_RDONLY);
        if (*fd_in < 0) {
                error_at_line(-1, errno, __FILE__, __LINE__, NULL);
        }

        *fd_out = open("/tmp/out", O_CREAT|O_WRONLY);
        if (*fd_out < 0) {
                error_at_line(-1, errno, __FILE__, __LINE__, NULL);
        }

        if (pipefd) {
                CHECK_ERROR(-1, pipe(pipefd));
        }
}

void run_splice(int fd_in, int fd_out, int *pipefd)
{
        CHECK_ERROR(-1, splice(fd_in, NULL, pipefd[1], NULL, BUFSIZE, SPLICE_F_MOVE));
        CHECK_ERROR(-1, splice(pipefd[0], NULL, fd_out, NULL, BUFSIZE, SPLICE_F_MOVE));
}

void run_readwrite(int fd_in, int fd_out, char *buf)
{
        CHECK_ERROR(-1, read(fd_in, buf, BUFSIZE));
        CHECK_ERROR(-1, write(fd_out, buf, BUFSIZE));
}

void run_sendfile(int fd_in, int fd_out)
{
        CHECK_ERROR(-1, sendfile(fd_out, fd_in, NULL, BUFSIZE));
}

int get_diff(struct timeval *before, struct timeval *later)
{
        return ((later->tv_sec - before->tv_sec) * MICROSECOND) + (later->tv_usec - before->tv_usec);
}
