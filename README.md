bench_splice
============

A simple bench for splice vs read/write in C

** Please make sure you have enough space in /run/shm **

To run:

``` bash
$ dd if=/dev/urandom of=/run/shm/in bs=4M count=1
$ sudo make
```

Sample result: (On Linux 3.11.0-15-generic #25-Ubuntu SMP x86_64, Intel(R) Core(TM) i5-3210M CPU @ 2.50GHz, memery 4G)

```
start benching, it will take a couple of minutes...
====== BENCH READWRITE ======
Time: 3111516 us / 1000 ops
Throughput: 1286 MB/s
====== BENCH SPLICE ======
Time: 1454374 us / 1000 ops
Throughput: 2750 MB/s
====== BENCH SENDFILE ======
Time: 1494805 us / 1000 ops
Throughput: 2676 MB/s
```
