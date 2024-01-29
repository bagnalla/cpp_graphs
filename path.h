#pragma once

#include <variant>
#include <vector>

#include "graph.h"

namespace path {
  
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
