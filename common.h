#pragma once

#include <variant>
#include <vector>

#include "graph.h"

namespace common {

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
  
  // Build path (vector of unlabeled edges) from src to dest using
  // given predecessors map.
  template <typename V>
  std::vector<edge<V, std::monostate>>
  build_path(std::unordered_map<V, std::optional<V>> &pred,
             const V &src,
             const V &dest) {
    std::vector<edge<V, std::monostate>> path;
    
    V cur = dest;
    while (pred[cur].has_value()) {
      path.push_back({pred[cur].value(), cur, {}});
      cur = pred[cur].value();
    }

    std::reverse(path.begin(), path.end());
    
    return path;
  }
}
