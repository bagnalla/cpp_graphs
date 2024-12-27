// Kruskal's minimum spanning tree algorithm.

#pragma once

#include <vector>

#include "common.h"
#include "graph.h"
#include "union_find.h"

namespace kruskal {
  template <typename V, common::Numeric E>
  std::vector<edge<V, E>> mst(const graph<V, E> &g) {
    union_find<V> uf;
    std::vector<edge<V, E>> ms_forest;

    for (const auto &v : g.vertices()) {
      uf.add(v);
    }

    auto edges = g.all_edges();
    std::sort(edges.begin(), edges.end(), [](const edge<V, E> &a,
                                             const edge<V, E> &b) {
      return a.label < b.label;
    });

    for (const auto &e : edges) {
      auto v1_set = uf.find(e.v1);
      auto v2_set = uf.find(e.v2);
      if (v1_set != v2_set) {
        ms_forest.push_back(e);
        uf.set_union(v1_set, v2_set);
      }
    }

    return ms_forest;
  }
}
