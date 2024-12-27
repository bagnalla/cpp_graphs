// Depth-first path search. Pretty much the exact same as Dijkstra's
// except it uses a simple stack for the unvisited set which causes it
// to search in depth-first order.

#pragma once

#include <limits>

#include "common.h"
#include "graph.h"

namespace dfs {
  template <typename V, typename E>
  std::vector<edge<V, E>> find_path(const graph<V, E> &g,
                                    const V &src,
                                    const V &dest) {
    // Mapping of each vertex to its current tentative distance value.
    std::unordered_map<V, uint> dist;

    // Set of unvisited vertices.
    std::vector<V> unvisited{src};

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, std::optional<V>> pred;

    // Initialize all vertices with max distance value.
    for (const auto &v : g.vertices()) {
      dist[v] = std::numeric_limits<uint>::max();
    }

    // Initialize source vertex distance to 0.
    dist[src] = 0;

    while (!unvisited.empty()) {
      V u = unvisited.back();
      unvisited.pop_back();

      if (u == dest) {
        return common::build_path(g, pred, src, dest);
      }

      for (const auto &e : g.edges(u)) {
        const E d = dist[u] + 1;
        if (d < dist[e.v2]) {
          dist[e.v2] = d;
          pred[e.v2] = u;
          if (!common::contains(unvisited, e.v2)) {
            unvisited.push_back(e.v2);
          }
        }
      }
    }

    // If we've processed all vertices and never encountered the
    // destination, then it must not have existed in the graph.
    throw std::invalid_argument("destination doesn't exist");
  }
}
