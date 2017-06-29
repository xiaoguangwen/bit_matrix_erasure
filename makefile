CC = gcc  
CFLAGS = -g 

ALL =  my_encode \
	 my_decode \
          my_complute

all: $(ALL)

clean:
	rm -f *.o my_encode my_decode my_complute

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

my_encode: 
	$(CC) $(CFLAGS) -o my_encode my_encode.c -lpthread

my_decode:
	$(CC) $(CFLAGS) -o my_decode decode_my.c -lpthread

my_complute:
	$(CC) $(CFLAGS) -o my_complute matrix_compute.c -lpthread

