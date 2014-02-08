BUFSIZE := 4096
BENCHNUM := 100000

SRC := bench.c
OBJ := bench.o
HEADER := bench.h

all: obj
	@gcc -I./ -std=c99 -DBUFSIZE=$(BUFSIZE) -DBENCHNUM=$(BENCHNUM) benchsplice.c $(OBJ)
	@./a.out
	@rm a.out

	@gcc -I./ -std=c99 -DBUFSIZE=$(BUFSIZE) -DBENCHNUM=$(BENCHNUM) benchreadwrite.c $(OBJ)
	@./a.out
	@rm a.out

	@gcc -I./ -std=c99 -DBUFSIZE=$(BUFSIZE) -DBENCHNUM=$(BENCHNUM) benchsendfile.c $(OBJ)
	@./a.out
	@rm a.out

obj:
	@gcc -c -std=c99 -DBUFSIZE=$(BUFSIZE) -DBENCHNUM=$(BENCHNUM) $(SRC) 