#pragma once

#include <optional>
#include <vector>

#include "graph.h"

namespace common {

  // Although the graph type is fully generic in the type of edge
  // labels, many algorithms require them be numeric (to serve as
  // weights). See, for example, dijkstra.h and astar.h.
  template <typename T>
  concept Numeric = std::integral<T> || std::floating_point<T>;

  template <typename T>
  constexpr bool contains(const std::vector<T> &v, const T &x) {
    return std::find(v.begin(), v.end(), x) != v.end();
  }

  // Argmin of vector [v] wrt. given score function [f]. I.e, [minᵢ(f(v[i]))].
  template <typename T, Numeric N>
  uint min_index(const std::vector<T> &v, std::function<N(const T&)> f) {
    uint min_i = 0;
    uint min = std::numeric_limits<T>::max();
    for (uint i = 0; i < v.size(); i++) {
      if (f(v[i]) < min) {
        min_i = i;
        min = f(v[i]);
      }
    }
    return min_i;
  }

  // Build path (vector of unlabeled edges) in [g] from [src] to
  // [dest] using given predecessors map [pred].
  template <typename V, typename E>
  std::vector<edge<V, E>> build_path(const graph<V, E> &g,
                                     std::unordered_map<V, std::optional<V>> &pred,
                                     const V &src,
                                     const V &dest) {
    std::vector<edge<V, E>> path;

    V cur = dest;
    while (pred[cur].has_value()) {
      path.push_back({pred[cur].value(), cur, {}});
      cur = pred[cur].value();
    }

    std::reverse(path.begin(), path.end());

    return path;
  }

  // This is weird -- perhaps there's a better way (besides
  // 'std::ranges::to'). The idea is simply to collect the elements of
  // a range/view into a vector.
  template<typename T, std::ranges::forward_range R>
  requires std::same_as<T, std::ranges::range_value_t<R>>
  std::vector<T> collect(R &&r) {
    return std::vector<T>(r.begin(), r.end());
  }
}
