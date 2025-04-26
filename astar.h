// A* shortest path algorithm. Agrees with Dijkstra's algorithm on
// Project Euler problem 83 when the heuristic is set to the constant
// zero function, but runs slightly slower (presumably due to the
// extra overhead of calling the heuristic function). Need to
// test/compare on another problem for which a suitable heuristic is
// available. Euclidean and Manhattan distance heuristics don't seem
// useful at all for PE#83.

#pragma once

#include <functional>
#include <utility>

#include "common.h"
#include "graph.h"

namespace astar {

  // Find the shortest path in [g] from [src] to [dest] using
  // heuristic function [h].
  template <typename V, common::Numeric E>
  std::vector<edge<V, E>> shortest_path(const graph<V, E> &g,
                                        const V &src,
                                        const V &dest,
                                        const std::function<E(const V&)> &h) {
    // Mapping of each vertex to its current tentative distance value.
    std::unordered_map<V, E> dist;

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, V> pred;

    // Open set (priority queue).
    std::vector<V> open{src};

    std::function<E(const V&)> f = [&dist = std::as_const(dist),
                                    &h = std::as_const(h)](const V &v) {
      return dist.at(v) + h(v);
    };

    // Initialize all vertices with max distance value.
    for (const auto &v : g.vertices()) {
      dist[v] = std::numeric_limits<E>::max();
    }

    // Initialize source vertex tentative distance value.
    dist[src] = 0;

    // Main loop.
    while (!open.empty()) {
      // Remove the vertex with the smallest h score from the open set.
      uint min_i = common::min_index(open, f);
      V u = open[min_i];
      open.erase(open.begin() + min_i);

      if (u == dest) {
        return common::build_path(g, pred, src, dest);
      }

      for (const auto &e : g.edges(u)) {
        const E d = dist[u] + e.label;
        if (d < dist[e.v2]) {
          dist[e.v2] = d;
          pred[e.v2] = u;
          if (!common::contains(open, e.v2)) {
            open.push_back(e.v2);
          }
        }
      }
    }

    // If we've processed all vertices and never encountered the
    // destination, then it must not have existed in the graph.
    throw std::invalid_argument("destination doesn't exist");
  }
}
