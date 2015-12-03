cmake_minimum_required(VERSION 3.2)
project(StableMatching)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES depreciated/galeShapley.cpp)
set(ALLOCATION_SOURCE depreciated/GaleShapleyAllocation.cpp)
set(GENERATE_SOURCE GenerateDataFile.cpp)
set(HUNGARIAN_ALGORITHM HungarianAlgorithm.cpp)
set(GREEDY_SOURCE GreedyMatching.cpp GreedyMatching.h)
set(RUN_ALL_SOURCE RunTests.cpp HungarianAlgorithm.cpp ${GREEDY_SOURCE})

add_executable(StableMatching ${SOURCE_FILES})

add_executable(GSAllocation ${ALLOCATION_SOURCE})

add_executable(GenerateDataFile ${GENERATE_SOURCE})

add_executable(HungarianAlgorithm ${HUNGARIAN_ALGORITHM})

add_executable(RunTests ${RUN_ALL_SOURCE})