// Prim's minimum spanning tree algorithm. Technically, the result is
// a forest of minimum spanning trees for each connected component of
// the graph, which is simply an MST when the graph is connected.

#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

#include "binary_heap.h"
#include "common.h"
#include "graph.h"

namespace prim {

  template <typename V, common::Numeric E>
  std::vector<edge<V, E>> mst(const graph<V, E> &g) {
    // Mapping of each vertex to the edge providing its cheapest
    // connection to the MST so far (if one exists).
    std::unordered_map<V, std::optional<edge<V, E>>> edges;

    // Mapping of each vertex to the cost of its cheapest connection
    // to MST so far.
    std::function<E(const V&)> cost = [&edges = std::as_const(edges)](const V &v) {
      if (edges.at(v).has_value()) {
        return edges.at(v).value().label;
      } else {
        return std::numeric_limits<E>::max();
      }
    };

    // Initialize all edges to 'none'.
    for (const auto v : g.vertices()) {
      edges[v] = {};
    }

    // The MST to be built and returned.
    std::vector<edge<V, E>> mst;

    // Initialize the open set to contain all the vertices.
    std::vector<V> open = g.vertices();

    // Main loop.
    while (!open.empty()) {
      // Remove from the open set the vertex with the lowest cost to
      // add to the MST.
      uint min_i = common::min_index(open, cost);
      V u = open[min_i];
      open.erase(open.begin() + min_i);

      // If the vertex is connected to the MST built so far, add the
      // connecting edge. If this isn't true, then all the remaining
      // vertices must be disconnected from the MST built so far, so
      // we're starting an MST of a new connected component of g.
      if (edges[u].has_value()) {
        mst.push_back(edges[u].value());
      }

      // For all of the vertex's neighbors still in the open set,
      // update their cheapest edges if necessary (in case there's now
      // a cheaper edge through the current vertex).
      for (const auto e : g.edges(u)) {
        if (common::contains(open, e.v2)) {
          if (e.label < cost(e.v2)) {
            edges[e.v2] = e;
          }
        }
      }
    }
    
    return mst;
  }

  // Alternate version that uses a binary min-heap for the open
  // set. Appears to perform about the same on the PE#107 example.
  template <typename V, common::Numeric E>
  std::vector<edge<V, E>> mst2(const graph<V, E> &g) {
    // Mapping of each vertex to the edge providing its cheapest
    // connection to the MST so far (if one exists).
    std::unordered_map<V, std::optional<edge<V, E>>> edges;

    // Mapping of each vertex to the cost of its cheapest connection
    // to MST so far.
    std::function<E(const V&)> cost = [&edges = std::as_const(edges)](const V &v) {
      if (edges.at(v).has_value()) {
        return edges.at(v).value().label;
      } else {
        return std::numeric_limits<E>::max();
      }
    };

    // Initialize all edges to 'none'.
    for (const auto v : g.vertices()) {
      edges[v] = {};
    }

    // The MST to be built and returned.
    std::vector<edge<V, E>> mst;

    // Initialize the open set to contain all the vertices.
    // std::vector<V> open = g.vertices();
    binary_heap<V, E> open;
    for (const auto v : g.vertices()) {
      open.insert(v, cost(v));
    }

    // Main loop.
    while (open.size()) {
      // Remove from the open set the vertex with the lowest cost to
      // add to the MST.
      // uint min_i = common::min_index(open, cost);
      // V u = open[min_i];
      // open.erase(open.begin() + min_i);
      V u = open.extract().first;

      // If the vertex is connected to the MST built so far, add the
      // connecting edge. If this isn't true, then all the remaining
      // vertices must be disconnected from the MST built so far, so
      // we're starting an MST of a new connected component of g.
      if (edges[u].has_value()) {
        mst.push_back(edges[u].value());
      }

      // For all of the vertex's neighbors still in the open set,
      // update their cheapest edges if necessary (in case there's now
      // a cheaper edge through the current vertex).
      for (const auto e : g.edges(u)) {
        if (open.contains(e.v2)) {
          if (e.label < cost(e.v2)) {
            edges[e.v2] = e;
            open.decrease_key(e.v2, cost(e.v2));
          }
        }
      }
    }
    
    return mst;
  }
}
