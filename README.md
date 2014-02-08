bench_splice
============

A simple bench for splice vs read/write in C

* Please make sure you have enough space in /run/shm

To run:

``` bash
$ dd if=/dev/urandom of=/run/shm/in bs=4M count=1
$ sudo make
```
