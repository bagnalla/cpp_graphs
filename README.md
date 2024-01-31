# cpp_graphs

A simple graph library for practice/brushing up on basic graph algorithms and experimenting with modern C++ features.

The graph data structure is defined in [graph.h](graph.h). Code shared
between multiple algorithms is in [common.h](common.h). A binary
min-heap data structure is defined in [binary_heap.h](binary_heap.h),
and a union-find (disjoint-set) data structure is defined in
[union_find.h](union_find.h).

Graph algorithms implemented:
* Dijkstra's shortest path ([dijkstra.h](dijkstra.h)),
* A* ([astar.h](astar.h)).
* Prim's minimum spanning tree (forest) ([prim.h](prim.h)).
* Kruskal's minimum spanning tree (forest) ([kruskal.h](kruskal.h)).
* Kahn's topological sort ([kahn.h](kahn.h)).
