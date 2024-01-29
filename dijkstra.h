// Dijkstra's shortest path algorithm.

#pragma once

#include <optional>
#include <variant>
#include <vector>
#include <unordered_map>

#include "graph.h"
#include "path.h"

namespace dijkstra {
  
  // Dijkstra's shortest path algorithm. Returns two maps, the first
  // mapping vertices to their distance via the shortest path from the
  // source vertex (not guaranteed by this implementation to be
  // optimal except for the destination vertex and all vertices on the
  // path to it), and the second mapping vertices to their immediate
  // predecessors on the shortest path from the source vertex (same
  // caveat as above). For simplicity, we use a vector for the
  // 'unvisited' queue and keep it sorted by distance from the
  // source. We do this because std::priority_queue doesn't provide a
  // 'decrease_key' operation, so it would take a bit of work to get a
  // proper priority queue working (although it should be done to
  // improve the time complexity of the algorithm).

  // REMARK: actually, after changing the algorithm to initialize the
  // 'unvisited' queue to only the source node and adding neighbors to
  // the queue when they are first encountered, it performs quite well
  // without a more complicated data structure (presumably due to the
  // good cache performance of vectors compensating for the inferior
  // time complexity -- for *very* large graphs it would still
  // probably be better to use a binary/Fibonacci heap).
  
  template <typename V, Numeric E>
  std::pair<std::unordered_map<V, E>, std::unordered_map<V, std::optional<V>>>
  dijkstra(const graph<V, E> &g, const V &src, const V &dest) {
    // Mapping of each vertex to its current tentative distance value.
    std::unordered_map<V, E> dist;

    // Mapping of each vertex to its immediate predecessor on the
    // current best-known path from the source.
    std::unordered_map<V, std::optional<V>> pred;

    // Priority queue of unvisited vertices.
    std::vector<V> unvisited{src};

    // Comparison function for sorting in descending order so the
    // vertex with the least distance will be kept at the end of the
    // unvisited set.
    auto comp = [&dist = std::as_const(dist)](const V &a, const V &b) {
      return dist.at(a) > dist.at(b);
    };

    // Initialize source vertex distance to 0.
    dist[src] = static_cast<E>(0);

    // Initialize all other vertices with max distance value and push
    // them to the 'unvisited' queue.
    for (const auto v : g.vertices()) {
      if (v != src) {
        dist[v] = std::numeric_limits<E>::max();
      }
      // unvisited.push_back(v);
    }

    // Sort the 'unvisited' queue in descending order.
    std::sort(unvisited.begin(), unvisited.end(), comp);

    // Main loop.
    while (!unvisited.empty()) {
      // Pop the vertex with the smallest tentative distance value
      // from the 'unvisited' queue.
      V u = unvisited.back();
      unvisited.pop_back();

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
        return {dist, pred};
      }

      // For each neighbor of 'u', update their tentative distance
      // values if it becomes shorter through 'u'.
      for (const auto e : g.edges(u)) {
        const E d = dist[u] + e.label;
        if (d < dist[e.v2]) {
          dist[e.v2] = d;
          pred[e.v2] = u;
          if (std::find(unvisited.begin(), unvisited.end(), e.v2) == unvisited.end()) {
            unvisited.push_back(e.v2);
          }
        }
      }

      // Re-sort the 'unvisited' queue after updating neighbors'
      // tentative distance values.
      std::sort(unvisited.begin(), unvisited.end(), comp);
    }

    // If we've processed all vertices and never encountered the
    // destination, then it must not have existed in the graph.
    throw std::invalid_argument("destination doesn't exist");
  }

  // Compute the shortest path from [src] to [dest] in graph [g].
  template <typename V, Numeric E>
  std::vector<edge<V, std::monostate>> shortest_path(const graph<V, E> &g,
                                                     const V &src,
                                                     const V &dest) {
    // Compute shortest paths to all vertices.
    auto dist_pred = dijkstra(g, src, dest);

    // Build and return path to the destination.
    return path::build_path(dist_pred.second, src, dest);
  }
}
