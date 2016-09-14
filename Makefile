CC=gcc
CPP=clang++
CFLAGS=-g -std=c++11 -Wall

BINS= matrix.o echo_instance.o matching_utilities.o pagerank build_lp GenerateDataFile GreedyMatching regret

all: $(BINS)

matrix.o: matrix.cpp matrix.h
	$(CPP) $(CFLAGS) -c matrix.cpp

echo_instance.o: echo_instance.cpp echo_instance.h
	$(CPP) $(CFLAGS) -c echo_instance.cpp

matching_utilities.o: matching_utilities.cpp echo_instance.o
	$(CPP) $(CFLAGS) -c matching_utilities.cpp echo_instance.o

pagerank:  PageRank.cpp matrix.o echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -o pagerank PageRank.cpp matrix.o echo_instance.o matching_utilities.o

regret: regret_matching.cpp matrix.o echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -o regret regret_matching.cpp matrix.o echo_instance.o matching_utilities.o

readAnnotationFormat: readAnnotationFormat.cpp
		$(CPP) $(CFLAGS) -o readAnnotationFormat readAnnotationFormat.cpp

GenerateDataFile: GenerateDataFile.cpp echo_instance.o
	$(CPP) $(CFLAGS) -o GenerateDataFile GenerateDataFile.cpp echo_instance.o

GreedyMatching: GreedyMatching.cpp echo_instance.o
	$(CPP) $(CFLAGS) -o GreedyMatching GreedyMatching.cpp echo_instance.o

clean:
	rm $(BINS) cplex/build_lp

build_lp: cplex/build_lp.cpp
	$(CPP) $(CFLAGS) -o cplex/build_lp cplex/build_lp.cpp

gen_cplex:
	./cplex/build_lp << datasets/WeightedData.txt

.FORCE:
