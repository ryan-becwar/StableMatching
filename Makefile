CC=gcc
CPP=g++
CFLAGS=-Wall -g

BINS=pagerank readAnnotationFormat

all: $(BINS)

pagerank:  PageRank.cpp 
	$(CPP) $(CFLAGS) -o pagerank PageRank.cpp matrix.cpp

build_lp: build_lp.cpp
	$(CPP) $(CFLAGS) -o build_lp build_lp.cpp
readAnnotationFormat: readAnnotationFormat.cpp
		$(CPP) $(CFLAGS) -o readAnnotationFormat readAnnotationFormat.cpp

clean:
	rm $(BINS) 

gen_cplex:
	./build_lp << datasets/WeightedData.txt
	
.FORCE:	
	
