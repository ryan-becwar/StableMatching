CC=gcc
CPP=g++
CFLAGS=-g -std=c++0x -Wall

BINS= matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o pagerank.o generate_data_file.o regret_matching.o build_lp.o greedy_matching.o

all: $(BINS) main

matrix.o: matrix.cpp matrix.h
	$(CPP) $(CFLAGS) -c matrix.cpp

echo_instance.o: echo_instance.cpp echo_instance.h
	$(CPP) $(CFLAGS) -c echo_instance.cpp

matching_utilities.o: matching_utilities.cpp matching_utilities.h echo_instance.o
	$(CPP) $(CFLAGS) -c matching_utilities.cpp

ordering_evaluator.o: ordering_evaluator.cpp ordering_evaluator.h echo_instance.o matching_utilities.o
	$(CPP) $(CFLAGS) -c ordering_evaluator.cpp

generate_data_file.o: generate_data_file.cpp generate_data_file.h echo_instance.o
	$(CPP) $(CFLAGS) -c generate_data_file.cpp

pagerank.o:  pagerank.cpp matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o
	$(CPP) $(CFLAGS) -c pagerank.cpp

regret_matching.o: regret_matching.cpp matrix.o echo_instance.o matching_utilities.o ordering_evaluator.o
	$(CPP) $(CFLAGS) -c regret_matching.cpp

greedy_matching.o: greedy_matching.cpp greedy_matching.h echo_instance.o
	$(CPP) $(CFLAGS) -c greedy_matching.cpp

build_lp.o: build_lp.cpp build_lp.h
	$(CPP) $(CFLAGS) -c build_lp.cpp

main: main.cpp $(BINS)
	$(CPP) $(CFLAGS) -o main main.cpp $(BINS)

readAnnotationFormat: readAnnotationFormat.cpp
		$(CPP) $(CFLAGS) -o readAnnotationFormat readAnnotationFormat.cpp

generate_data_file: generate_data_file.cpp generate_data_file.h runGenerate.cpp echo_instance.o
	$(CPP) $(CFLAGS) -o generate_data_file runGenerate.cpp

clean:
	rm $(BINS)

build_lp: cplex/build_lp.cpp
	$(CPP) $(CFLAGS) -o cplex/build_lp cplex/build_lp.cpp

gen_cplex:
	./cplex/build_lp << datasets/WeightedData.txt

.FORCE:
