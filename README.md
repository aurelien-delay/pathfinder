# Solution for pathfinder exercice

## General explanation

The purpose is to implement a function FindPath(...) to find one of the shortest path between a Start location and a Target location within a map composed of cells, some are impassible.

The algorithm A* was used for this.
The idea is to begin with Start location, and explore one by one the adjacent cells, priorityzing the most promising cells first. For each new explored cells, the previous cell in shortest path is stored - it will be used to recompute the shortest path at the end. 

The priority score is in party based on an estimation (called heuristics) of the distance of the current cell to the Target.
Here we choose a heuristics that cannot overestimate the actual distance. It can then be proven that A* algorythm is guaranteed to find the shortest path.

Since we prioritized most promising cells, the algo is significantly faster than one that would explore the full map.
More on A* : https://en.wikipedia.org/wiki/A*_search_algorithm

## In multi thread environment

While the algo does not use multiple threads, FindPath() could be called in several threads with some shared data.
The function has been designed to be reentrant. Meaning it is safe for the calling code to call it in several threads as long as they ensure :
- pMap is const in all threads
- pOutBuffer is not shared (each thread has its own instance)

Making the function thread safe, i.e. supporting that pMap and pOutBuffer are shared among several threads would necessitate to just lock the full function and disabling thus parrarel execution. 
Ensuring Reentrancy was deemed enough.
