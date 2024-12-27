// Test Dijkstra's and A* on graphs containing loops.

#include <iostream>

#include "astar.h"
#include "dfs.h"
#include "graph.h"
#include "dijkstra.h"

using namespace std;

int main() {
  graph<int, int> g;

  g.add_vertex(0);
  g.add_vertex(1);
  g.add_vertex(2);
  g.add_vertex(3);

  g.add_edge(0, 1, 0);
  g.add_edge(0, 2, 0);
  g.add_edge(1, 2, 0);
  g.add_edge(2, 3, 0);

  // const auto p = dijkstra::shortest_path(g, 0, 3);
  const auto p = dfs::find_path(g, 0, 3);

  for (const auto &e : p) {
    cout << e.v1 << " " << e.v2 << endl;
  }
}
