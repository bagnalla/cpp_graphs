// Kahn's algorithm for topological sorting the vertices of a
// graph. Not yet thoroughly tested.

#pragma once

#include <ranges>
#include <vector>

#include "common.h"
#include "graph.h"

namespace views = std::ranges::views;

namespace kahn {

  template <typename V, typename E>
  std::vector<V> topsort(const graph<V, E> &g) {
    // Create copy of the graph.
    graph<V, E> g2(g);

    // Vector of topologically sorted vertices. Initially empty.
    std::vector<V> vertices;

    // Find vertices with no incoming edges.    
    // This is unfortunately strange and annoying compared to the
    // corresponding code in Rust. I would probably favor just writing
    // a simple loop instead. I'm not sure at the moment why we must
    // declare the variable vs instead of using 'g.vertices()'
    // directly on the LHS of the '|' operator. Presumably it has to
    // be an l-value, but I don't know why.
    const std::vector<V> vs = g.vertices();
    std::vector<V> no_inc = common::collect<V>(vs | views::filter([g](const V &v) {
      return g.in_degree(v) == 0;
    }));
    // In C++23 we should be able to do this instead of our custom
    // 'collect' function.
    // | std::ranges::to<std::vector>();

    // Main loop.
    while (!no_inc.empty()) {
      // Pop a vertex with no incoming edges.
      const V v = no_inc.back();
      no_inc.pop_back();

      // Add it to the end of our vector of topologically sorted vertices.
      vertices.push_back(v);

      // Delete the edges coming out of v. If this reduces the
      // indegree of any vertex to 0 then add it to 'no_inc'.
      for (const auto e : g2.edges(v)) {
        g2.remove_edge(e, true);
        if (g2.in_degree(e.v2) == 0) {
          no_inc.push_back(e.v2);
        }
      }
    }

    return vertices;
  }
}
