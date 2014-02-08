/**
 * a simple benchmark program for splice vs user space read/write
 *
 * Yifan Gu guyifan1121@gmail.com
 * Feb, 7, 2014
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/sendfile.h>
#include <unistd.h>

#define MICROSECOND (1000000)

#define CHECK_ERROR(status, func)                                       \
        if ((func) < 0) {                                               \
                error_at_line((status), errno, __FILE__, __LINE__, NULL); \
        }

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

        CHECK_ERROR(-1, pipe(pipefd));
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

int main(int argc, char *argv[])
{
        int fd_in, fd_out;
        int pipefd[2];
        char buf[BUFSIZE];

        struct timeval t0, t1, t2, t3;

        setup(&fd_in, &fd_out, pipefd);

        CHECK_ERROR(-1, gettimeofday(&t0, NULL));
        
        for (int i = 0; i < BENCHNUM; i++) {
                run_splice(fd_in, fd_out, pipefd);
        }

        CHECK_ERROR(-1, gettimeofday(&t1, NULL));
        
        for (int i = 0; i < BENCHNUM; i++) {
                run_readwrite(fd_in, fd_out, buf);
        }

        CHECK_ERROR(-1, gettimeofday(&t2, NULL));

        for (int i = 0; i < BENCHNUM; i++) {
                run_sendfile(fd_in, fd_out);
        }
        
        CHECK_ERROR(-1, gettimeofday(&t3, NULL));
        
        int diff1 = get_diff(&t0, &t1);
        int diff2 = get_diff(&t1, &t2);
        int diff3 = get_diff(&t2, &t3);

        printf("====== BENCH RESULT ======\n");
        printf("bench splice: %d us / %d ops\n", diff1, BENCHNUM);
        printf("bench readwrite: %d us / %d ops\n", diff2, BENCHNUM);
        printf("bench sendfile: %d us / %d ops\n", diff3, BENCHNUM);
        
        return 0;
}
