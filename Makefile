CC = gcc  
CFLAGS = -g 

ALL =   my_complute

all: $(ALL)

clean:
	rm -f *.o my_complute

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

my_complute:
	$(CC) $(CFLAGS) -o my_complute matrix_compute.c -lpthread

