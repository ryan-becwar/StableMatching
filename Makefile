CC=gcc
CPP=clang++
CFLAGS=-g -std=c++11 -Wall

BINS= matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o main pagerank build_lp GenerateDataFile GreedyMatching regret

all: $(BINS)

matrix.o: matrix.cpp matrix.h
	$(CPP) $(CFLAGS) -c matrix.cpp

echo_instance.o: echo_instance.cpp echo_instance.h
	$(CPP) $(CFLAGS) -c echo_instance.cpp

matching_utilities.o: matching_utilities.cpp matching_utilities.h echo_instance.o
	$(CPP) $(CFLAGS) -c matching_utilities.cpp

ordering_evaluator.o: ordering_evaluator.cpp ordering_evaluator.h echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -c ordering_evaluator.cpp

pagerank:  PageRank.cpp matrix.o echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -o pagerank PageRank.cpp matrix.o echo_instance.o matching_utilities.o

regret: regret_matching.cpp matrix.o echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -o regret regret_matching.cpp matrix.o echo_instance.o matching_utilities.o

main: main.cpp echo_instance.o ordering_evaluator.o
	$(CPP) $(CFLAGS) -o main main.cpp echo_instance.o ordering_evaluator.o

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
