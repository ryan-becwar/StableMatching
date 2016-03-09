CC=gcc
CPP=g++
CFLAGS=-Wall -g -std=c++11

BINS=pagerank readAnnotationFormat build_lp GenerateDataFile

all: $(BINS)

pagerank:  PageRank.cpp 
	$(CPP) $(CFLAGS) -o pagerank PageRank.cpp matrix.cpp

build_lp: cplex/build_lp.cpp
	$(CPP) $(CFLAGS) -o cplex/build_lp cplex/build_lp.cpp

readAnnotationFormat: readAnnotationFormat.cpp
		$(CPP) $(CFLAGS) -o readAnnotationFormat readAnnotationFormat.cpp

GenerateDataFile: GenerateDataFile.cpp
	$(CPP) $(CFLAGS) -o GenerateDataFile GenerateDataFile.cpp echo_instance.cpp

GreedyMatching: GreedyMatching.cpp
	$(CPP) $(CFLAGS) -o GreedyMatching GreedyMatching.cpp echo_instance.cpp

clean:
	rm $(BINS) cplex/build_lp

gen_cplex:
	./build_lp << datasets/WeightedData.txt
	
.FORCE:	
	
