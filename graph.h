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
  V v1;    // Source vertex.
  V v2;    // Destination vertex.
  E label; // Edge label.
  friend bool operator==(const edge<V, E>&, const edge<V, E>&) = default;
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
      this->indegree.emplace(v, 0);
      this->outdegree.emplace(v, 0);
    }
  }

  // Add an edge to the graph. If directed=false, a symmetric copy of
  // the edge is also added. If multigraph=false and the edge (or its
  // symmetric copy) already exists, its label is updated with the new
  // label. Otherwise, if multigraph=true then there can be multiple
  // edges with the same vertex endpoints.
  void add_edge(const edge<V, E> &e, bool directed=false, bool multigraph=false) {
    if (!this->adj.contains(e.v1)) {
      throw std::invalid_argument("v1 not in graph");
    }
    if (!this->adj.contains(e.v2)) {
      throw std::invalid_argument("v2 not in graph");
    }
    
    if (auto x = std::find_if(this->adj[e.v1].begin(), this->adj[e.v1].end(),
                              [e](const edge<V, E> &x) { return x.v2 == e.v2; });
        !multigraph && x != this->adj[e.v1].end()) {
      x->label = e.label;
    } else {
      this->_add_edge(e);
    }

    if (!directed) {
      if (auto x = std::find_if(this->adj[e.v2].begin(), this->adj[e.v2].end(),
                                [e](const edge<V, E> &x) { return x.v2 == e.v1; });
          !multigraph && x != this->adj[e.v2].end()) {
        x->label = e.label;
      } else {
	this->_add_edge({e.v2, e.v1, e.label});
      }
    }
  }
  

  // Helper function for adding an edge.
  constexpr void add_edge(const V &v1, const V &v2, const E &lbl,
                          bool directed=false, bool multigraph=false) {
    this->add_edge({v1, v2, lbl}, directed, multigraph);
  }

  void remove_edge(const edge<V, E> &e, bool directed=false) {
    this->_remove_edge(e);
    if (!directed) {
      this->_remove_edge(edge<V, E>{e.v2, e.v1, e.label});
    }
  }

  // Get all vertices (copies). Returns a vector for efficiency but it
  // does not contain duplicates.
  std::vector<V> vertices() const {
    std::vector<V> vs;
    for (const auto p : this->adj) {
      vs.push_back(p.first);
    }
    return vs;
  }
  
  // Get all edges (copies).
  constexpr std::vector<edge<V, E>> edges(const V &v) const {
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

  // Subgraph induced by vertices [vs].
  graph<V, E> subgraph(const std::vector<V> &vs) {
    graph<V, E> g;
    for (const auto v : vs) {
      g.add_vertex(v);
    }
    for (const auto v : vs) {
      for (const auto e : this->edges[v]) {
        g.add_edge(e, true, true);
      }
    }
    return g;
  }

  constexpr uint in_degree(const V &v) const {
    if (this->indegree.contains(v)) {
      return this->indegree.at(v);
    } else {
      throw std::invalid_argument("v not in graph");
    }
  }

  constexpr uint out_degree(const V &v) const {
    return this->outdegree.at(v);
  }
  
private:
  std::unordered_map<V, std::vector<edge<V, E>>> adj;
  std::unordered_map<V, uint> indegree;
  std::unordered_map<V, uint> outdegree;

  // Primitive operation for adding a single directed edge. Undirected
  // edges are implemented (by, e.g., public method 'add_edge') by
  // adding two directed edges, one in each direction.
  constexpr void _add_edge(const edge<V, E> &e) {
    this->adj[e.v1].push_back(e);
    this->outdegree[e.v1]++;
    this->indegree[e.v2]++;
  }

  void _remove_edge(const edge<V, E> &e) {
    for (uint i = 0; i < this->adj[e.v1].size();) {
      if (this->adj[e.v1][i] == e) {
        this->adj[e.v1].erase(this->adj[e.v1].begin() + i);
        this->outdegree[e.v1]--;
        this->indegree[e.v2]--;
      } else {
        i++;
      }
    }
  }
};
