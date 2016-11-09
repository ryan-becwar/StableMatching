CC=gcc
CPP=clang++
CFLAGS=-g -std=c++11 -Wall

BINS= matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o PageRank.o GenerateDataFile.o GreedyMatching regret_matching.o build_lp.o main

all: $(BINS)

matrix.o: matrix.cpp matrix.h
	$(CPP) $(CFLAGS) -c matrix.cpp

echo_instance.o: echo_instance.cpp echo_instance.h
	$(CPP) $(CFLAGS) -c echo_instance.cpp

matching_utilities.o: matching_utilities.cpp matching_utilities.h echo_instance.o
	$(CPP) $(CFLAGS) -c matching_utilities.cpp

ordering_evaluator.o: ordering_evaluator.cpp ordering_evaluator.h echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -c ordering_evaluator.cpp

GenerateDataFile.o: GenerateDataFile.cpp GenerateDataFile.h echo_instance.o
	$(CPP) $(CFLAGS) -c GenerateDataFile.cpp

PageRank.o:  PageRank.cpp matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o
	$(CPP) $(CFLAGS) -c PageRank.cpp

regret_matching.o: regret_matching.cpp matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o
	$(CPP) $(CFLAGS) -c regret_matching.cpp

build_lp.o: build_lp.cpp build_lp.h
	$(CPP) $(CFLAGS) -c build_lp.cpp

main: main.cpp echo_instance.o ordering_evaluator.o regret_matching.o PageRank.o matching_utilities.o matrix.o build_lp.o
	$(CPP) $(CFLAGS) -o main main.cpp echo_instance.o ordering_evaluator.o regret_matching.o PageRank.o matching_utilities.o matrix.o GenerateDataFile.o build_lp.o

readAnnotationFormat: readAnnotationFormat.cpp
		$(CPP) $(CFLAGS) -o readAnnotationFormat readAnnotationFormat.cpp

GenerateDataFile: GenerateDataFile.cpp GenerateDataFile.h runGenerate.cpp echo_instance.o
	$(CPP) $(CFLAGS) -o GenerateDataFile runGenerate.cpp

GreedyMatching: GreedyMatching.cpp echo_instance.o
	$(CPP) $(CFLAGS) -o GreedyMatching GreedyMatching.cpp echo_instance.o

clean:
	rm $(BINS) cplex/build_lp

build_lp: cplex/build_lp.cpp
	$(CPP) $(CFLAGS) -o cplex/build_lp cplex/build_lp.cpp

gen_cplex:
	./cplex/build_lp << datasets/WeightedData.txt

.FORCE:
