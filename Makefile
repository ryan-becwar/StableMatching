CC=clang
CPP=g++
CFLAGS=-Wall -g

BINS=pagerank

all: $(BINS)

pagerank:  PageRank.cpp 
	$(CPP) $(CFLAGS) -o pagerank PageRank.cpp matrix.cpp

leaktest: leaktest.c
	$(CC) $(CFLAGS) -o leaktest leaktest.c

broken.o: broken.c
	$(CC) $(CFLAGS) -c broken.c

leakcount: leakcount.c
	$(CC) $(CFLAGS) -o leakcount leakcount.c


clean:
	rm $(BINS) 

test: broken.o broken.h .FORCE
	$(CC) $(CFLAGS) -o test mytests.c broken.o
	./test 

.FORCE:	
	
