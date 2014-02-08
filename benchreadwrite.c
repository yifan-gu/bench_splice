/**
 * a simple benchmark program for splice vs user space read/write
 *
 * Yifan Gu guyifan1121@gmail.com
 * Feb, 7, 2014
 */

#include "bench.h"

int main(int argc, char *argv[])
{
        int fd_in, fd_out;
        char buf[BUFSIZE];
        struct timeval t0, t1;

        setup(&fd_in, &fd_out, NULL);

        CHECK_ERROR(-1, gettimeofday(&t0, NULL));

        for (int i = 0; i < BENCHNUM; i++) {
                run_readwrite(fd_in, fd_out, buf);
        }

        CHECK_ERROR(-1, gettimeofday(&t1, NULL));

        int diff = get_diff(&t0, &t1);

        printf("====== BENCH READWRITE ======\n");
        print_result(diff);

        return 0;
}
