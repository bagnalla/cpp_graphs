// Dijkstra's shortest path algorithm.

#pragma once

#include <optional>
#include <vector>
#include <unordered_map>

#include "common.h"
#include "graph.h"

namespace dijkstra {
  
  // Find the shortest path in [g] from [src] to [dest]. For
  // simplicity, we use a vector for the 'unvisited' queue and keep it
  // sorted by distance from the source. We do this because
  // std::priority_queue doesn't provide a 'decrease_key' operation,
  // so it would take a bit of work to get a proper priority queue
  // working (although it should be done to improve the time
  // complexity of the algorithm).

  // REMARK: Actually, after changing the algorithm to initialize the
  // 'unvisited' queue to only the source node and adding neighbors to
  // the queue when they are first encountered, it performs quite well
  // without a more complicated data structure (presumably due to the
  // good cache performance of vectors compensating for the inferior
  // time complexity -- for *very* large graphs it would still
  // probably be better to use a binary/Fibonacci heap).

  // UPDATE: It appears to be significantly faster when *not* keeping
  // the 'unvisited' set (not calling it a queue anymore) sorted, and
  // simply performing a linear scan to find the minimum element (and
  // another to remove it).
  
  template <typename V, Numeric E>
  std::vector<edge<V, E>> shortest_path(const graph<V, E> &g,
                                        const V &src,
                                        const V &dest) {
    // Mapping of each vertex to its current tentative distance value.
    std::unordered_map<V, E> dist;

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, std::optional<V>> pred;

    // Set of unvisited vertices.
    std::vector<V> unvisited{src};

    // Initialize source vertex distance to 0.
    dist[src] = static_cast<E>(0);

    // Initialize all other vertices with max distance value and push
    // them to the 'unvisited' set.
    for (const auto v : g.vertices()) {
      if (v != src) {
        dist[v] = std::numeric_limits<E>::max();
      }
    }

    // Main loop.
    while (!unvisited.empty()) {
      // Remove the vertex with the smallest tentative distance value
      // from the 'unvisited' set.
      std::function<E(const V&)> f = [&dist = std::as_const(dist)](const V &v) {
        return dist.at(v);
      };
      uint min_i = common::min_index(unvisited, f);
      V u = unvisited[min_i];
      unvisited.erase(unvisited.begin() + min_i);

      // If its tentative distance value is still the max value, there
      // is no path to this vertex and thus there is no path to the
      // destination (because we would have terminated by now if the
      // shortest path to the destination was found, and all remaining
      // vertices are not reachable from the source).
      if (dist[u] == std::numeric_limits<E>::max()) {
        throw std::invalid_argument("path doesn't exist");
      }

      // If 'u' is the destination, then we're done. We know we've
      // found the shortest path to it because the algorithm always
      // explores the shortest known current path -- basically it's
      // like a breadth-first search but wrt. weight rather than
      // number of edges/vertices, so if it were to visit the same
      // vertex later on it would be via a longer path (although that
      // never happens because it never visits the same node twice
      // because it doesn't have to, for exactly the reason we just
      // described).
      if (u == dest) {
        return common::build_path(g, pred, src, dest);
      }

      // For each neighbor of 'u', update their tentative distance
      // values if it becomes shorter through 'u'.
      for (const auto e : g.edges(u)) {
        const E d = dist[u] + e.label;
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
