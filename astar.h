// A* shortest path. Agrees with Dijkstra's algorithm on Project Euler
// problem 83 when the heuristic is set to the constant zero function,
// but runs a bit slower. Need to test/compare on another problem for
// which a suitable heuristic is available. Euclidean/Manhattan
// distance doesn't seem useful at all for PE#83.

#pragma once

#include <functional>
#include <variant>

#include "graph.h"
#include "path.h"

namespace astar {
  
  template <typename V, Numeric E>
  std::pair<std::unordered_map<V, E>, std::unordered_map<V, std::optional<V>>>
  astar(const graph<V, E> &g,
        const V &src,
        const V &dest,
        const std::function<E(const V&)> &h) {
    // Mapping of each vertex to its current tentative distance value.
    std::unordered_map<V, E> dist;

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, std::optional<V>> pred;

    // Open set (priority queue).
    std::vector<V> open{src};

    auto f = [&dist = std::as_const(dist), &h = std::as_const(h)](const V &v) {
      return dist.at(v) + h(v);
    };

    // Comparison function for sorting in descending order so the
    // vertex with the least distance will be kept at the end of the
    // unvisited set.
    auto comp = [&f = std::as_const(f)](const V &a, const V &b) {
      return f(a) > f(b);
    };

    // Initialize source vertex tentative distance value.
    dist[src] = 0;

    // Initialize all other vertices with max distance value.
    for (const auto v : g.vertices()) {
      if (v != src) {
        dist[v] = std::numeric_limits<E>::max();
      }
    }

    // Sort the open set in descending order.
    std::sort(open.begin(), open.end(), comp);

    // Main loop.
    while (!open.empty()) {
      // Pop the vertex with the smallest tentative distance value
      // from the open set.
      V u = open.back();
      open.pop_back();

      if (u == dest) {
        return {dist, pred};
      }

      for (const auto e : g.edges(u)) {
        const E d = dist[u] + e.label;
        if (d < dist[e.v2]) {
          dist[e.v2] = d;
          pred[e.v2] = u;
          if (std::find(open.begin(), open.end(), e.v2) == open.end()) {
            open.push_back(e.v2);
          }
        }
      }

      // Re-sort the open set after updating neighbors' tentative
      // distance values.
      std::sort(open.begin(), open.end(), comp);
    }
    
    // If we've processed all vertices and never encountered the
    // destination, then it must not have existed in the graph.
    throw std::invalid_argument("destination doesn't exist");
  }
  
  // Compute the shortest path from [src] to [dest] in graph [g].
  template <typename V, Numeric E>
  std::vector<edge<V, std::monostate>>
  shortest_path(const graph<V, E> &g,
                const V &src,
                const V &dest,
                const std::function<E(const V&)> &h) {
    // Compute shortest paths to all vertices.
    auto dist_pred = astar(g, src, dest, h);

    // Build and return path to the destination.
    return path::build_path(dist_pred.second, src, dest);
  }
}
