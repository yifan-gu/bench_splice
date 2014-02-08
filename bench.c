#include "bench.h"

void run_readwrite(int, int, char *);

void setup(int *fd_in, int *fd_out, int *pipefd)
{
        char buf[BUFSIZE];

        *fd_in = open("/run/shm/in", O_RDONLY);
        if (*fd_in < 0) {
                error_at_line(-1, errno, __FILE__, __LINE__, NULL);
        }

        *fd_out = open("/run/shm/out", O_CREAT|O_WRONLY);
        if (*fd_out < 0) {
                error_at_line(-1, errno, __FILE__, __LINE__, NULL);
        }

        if (pipefd) {
                CHECK_ERROR(-1, pipe(pipefd));
        }
}

void run_splice(int fd_in, int fd_out, int *pipefd)
{
        CHECK_ERROR(-1, lseek(fd_in, 0, SEEK_SET));
        CHECK_ERROR(-1, lseek(fd_out, 0, SEEK_SET));

        int finished = 0;
        while (finished < BUFSIZE) {
                int ret = splice(fd_in, NULL, pipefd[1], NULL, BUFSIZE, SPLICE_F_MOVE);
                if (ret < 0) {
                        error_at_line(-1, errno, __FILE__, __LINE__, NULL);
                }
                CHECK_ERROR(-1, splice(pipefd[0], NULL, fd_out, NULL, ret, SPLICE_F_MOVE));
                finished += ret;
        }
}

void run_readwrite(int fd_in, int fd_out, char *buf)
{
        CHECK_ERROR(-1, lseek(fd_in, 0, SEEK_SET));
        CHECK_ERROR(-1, lseek(fd_out, 0, SEEK_SET));

        CHECK_ERROR(-1, read(fd_in, buf, BUFSIZE));
        CHECK_ERROR(-1, write(fd_out, buf, BUFSIZE));
}

void run_sendfile(int fd_in, int fd_out)
{
        CHECK_ERROR(-1, lseek(fd_in, 0, SEEK_SET));
        CHECK_ERROR(-1, lseek(fd_out, 0, SEEK_SET));

        CHECK_ERROR(-1, sendfile(fd_out, fd_in, NULL, BUFSIZE));
}

int get_diff(struct timeval *before, struct timeval *later)
{
        return ((later->tv_sec - before->tv_sec) * MICROSECOND) + (later->tv_usec - before->tv_usec);
}

void print_result(int diff)
{
        printf("Time: %d us / %d ops\n", diff, BENCHNUM);
        printf("Throughput: %.0f MB/s\n", (float)BUFSIZE / (float)(1024 * 1024) / ((float)diff / (float)MICROSECOND) * (float)BENCHNUM);
}
