// Type of graphs, generic in the types of vertex and edge
// labels. Uses an adjacency list internal representation and exposes
// a simple interface for adding and accessing vertices/edges. It's an
// imperative/mutable/ephemeral data structure: adding a vertex/edge
// modifies the graph in-place rather than creating a modified
// copy. The vertices and edges are themselves immutable for now.

#pragma once

#include <unordered_map>
#include <vector>

// Directed edge.
template <typename V, typename E>
struct edge {
  V v1; // Source vertex.
  V v2; // Destination vertex.
  E label; // Edge label.
};

// Although the graph type is fully generic in the type of edge
// labels, many algorithms require them be numeric (to serve as
// weights). See, for example, dijkstra.h and astar.h.
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// The graph type.
template <typename V, typename E>
class graph {
public:

  // Add a vertex to the graph.
  void add_vertex(V v) {
    if (this->adj.contains(v)) {
      throw std::invalid_argument("vertex already in graph");
    } else {
      this->adj.emplace(v, std::vector<edge<V, E>>{});
    }
  }

  // Add an edge to the graph. If directed=false, a symmetric copy of
  // the edge is also added. If the edge (or its symmetric copy)
  // already exists, its label is updated with the new label.
  void add_edge(const edge<V, E> &e, bool directed) {
    if (!this->adj.contains(e.v1)) {
      throw std::invalid_argument("v1 not in graph");
    }
    if (!this->adj.contains(e.v2)) {
      throw std::invalid_argument("v2 not in graph");
    }
    
    if (auto x = std::find_if(this->adj[e.v1].begin(), this->adj[e.v1].end(),
                              [e](const edge<V, E> &x) { return x.v2 == e.v2; });
        x != this->adj[e.v1].end()) {
      x->label = e.label;
    } else {
      this->adj[e.v1].push_back(e);
    }

    if (!directed) {
      if (auto x = std::find_if(this->adj[e.v2].begin(), this->adj[e.v2].end(),
                                [e](const edge<V, E> &x) { return x.v2 == e.v1; });
          x != this->adj[e.v2].end()) {
        x->label = e.label;
      } else {
        this->adj[e.v2].push_back({e.v2, e.v1, e.label});
      }
    }
  }
  
  // Edges are undirected by default.
  constexpr void add_edge(const edge<V, E> &e) {
    this->add_edge(e, false);
  }

  // Helper function for adding an edge.
  constexpr void add_edge(const V &v1, const V &v2, const E &lbl, bool directed) {
    this->add_edge({v1, v2, lbl}, directed);
  }

  // Helper function for adding an edge.
  constexpr void add_edge(const V &v1, const V &v2, const E &lbl) {
    this->add_edge(v1, v2, lbl, false);
  }

  // Get all vertices (copies).
  std::vector<V> vertices() const {
    std::vector<V> vs;
    for (const auto p : this->adj) {
      vs.push_back(p.first);
    }
    return vs;
  }
  
  // Get all edges (copies).
  std::vector<edge<V, E>> edges(const V &v) const {
    if (!this->adj.contains(v)) {
      throw std::invalid_argument("vertex not in graph");
    }
    return this->adj.at(v);
  }

  edge<V, E> get_edge(const V &v1, const V &v2) const {
    if (!this->adj.contains(v1)) {
      throw std::invalid_argument("v1 not in graph");
    }
    for (const auto e : this->edges(v1)) {
      if (e.v2 == v2) {
        return e;
      }
    }
    throw std::invalid_argument("edge not in graph");
  }
  
private:
  std::unordered_map<V, std::vector<edge<V, E>>> adj;
};
