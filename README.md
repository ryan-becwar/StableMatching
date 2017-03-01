#Matching Project


##A system to implement and evaluate a series of heuristic driven value based bipartite matching algorithms which sacrifice a guarantee of optimality for speed

###Running the project
The system can be built using make and run from the generated executable file main.

The system generates a number of graphs specified the INSTANCE_COUNT variable in main, each with N nodes on each side of the graph, at varying tiers of noise specified by the NOISE_TIERS variable.  The tiers increase linearly increase the randomness factor while decreasing the global priority or proximity factor.

For each graph, the system then calculates the value of the optimal matching, the values of GREEDY_COUNT randomly ordered greedy orderings, and the values of the regret and pagerank matchings.
