// Depth-first path search.

#pragma once

#include <unordered_set>

#include "common.h"
#include "graph.h"

namespace dfs {
  template <typename V, typename E>
  std::vector<edge<V, E>> find_path(const graph<V, E> &g,
                                    const V &src,
                                    const V &dest) {
    // Set of unvisited vertices.
    std::vector<V> unvisited{src};

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, V> pred;

    // Set of visited vertices to avoid processing the same vertex
    // more than once in case of cycles.
    std::unordered_set<V> seen;
    seen.insert(src);

    while (!unvisited.empty()) {
      V u = unvisited.back();
      unvisited.pop_back();

      if (u == dest) {
        return common::build_path(g, pred, src, dest);
      }

      for (const auto &e : g.edges(u)) {
        if (!seen.contains(e.v2)) {
	  seen.insert(e.v2);
	  pred.emplace(e.v2, u);
	  unvisited.push_back(e.v2);
        }
      }
    }

    // If we've processed all vertices and never encountered the
    // destination, then it must not have existed in the graph.
    throw std::invalid_argument("destination doesn't exist");
  }
}
