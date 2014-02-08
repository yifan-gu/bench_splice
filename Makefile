SRC := benchsplice.c
BUFSIZE := 4096
BENCHNUM := 100000

all:
	@gcc $(SRC) -std=c99 -DBUFSIZE=$(BUFSIZE) -DBENCHNUM=$(BENCHNUM)
	@./a.out
	@rm a.out
