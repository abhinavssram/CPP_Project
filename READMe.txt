BASEBALL ELIMINATION															

Objective  
- Given  the  points  table  of  "American  baseball  league "  teams  determine  which  team  is  eliminated  and  should  be  able  explain  how  the  team  is  eliminated  in  a  "simpler"  way.

Key Idea
- formulate the given problem as " Max-flow problem ". Here finding max-flow determines which team is eliminated.
- As a result of finding max-flow we can also deduce the " Min-cut " of the problem which is used to determine which team is mathematically eliminated.

### Building the source code

- Implemented " ford fulkerson algorithm " in c++ using BFS technique to find the augmented paths.
- Later DFS is used to find the Min-cut vertices.

### idea behind source code

- There is only one file of source code that is " BaseballEliminator.cpp ".
- This file contains 3 user-defined functions and a main function.
- The 3 user-defined functions includes opt_ff , BFS, DFS.
- opt_ff : It is the implementation of optimised ford fulkerson method using BFS technique.
- BFS 	 : It is used for finding augmented paths in flows graph.
- DFS    : It is used for finding Min-Cut of the flow graph.
- main: This contains the max-flow formulation of graph and calls opt_ff function to compute max-flow which is further used to determine which team is eliminated.

### Time Complexity
- For the implementation purpose " Adjacency matrix " data structure has been used.
- To traverse the matrix it takes O(n^2) where n is number of rows and column.
- Overall time complexity would be O(E*V^3) where V is total vertices & E is total number of edges.
