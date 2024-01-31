// Union find data structure (union by rank). Not yet thoroughly tested.

#pragma once

#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

#include "common.h"
#include "graph.h"

template <typename T>
class union_find {
public:
  void add(const T &x) {
    if (!this->nodes.contains(x)) {
      this->nodes[x] = node{x, NULL, 1};
    }
  }

  // T find(const T &x) {
  //   return this->_find(make_shared<node>(this->nodes[x]))->el;
  // }

  // Constant space version of 'find'.
  T find(const T &x) {
    node root = this->nodes[x];
    while (root.parent) {
      root = *root.parent;
    }

    node cur = this->nodes[x];
    while (cur.parent && cur.parent->el != root.el) {
      auto parent = *cur.parent;
      cur.parent = make_shared<node>(root);
      cur = parent;
    }

    return root.el;
  }

  void set_union(const T &x, const T &y) {
    node *x_root = &this->nodes[this->find(x)];
    node *y_root = &this->nodes[this->find(y)];

    if (x_root->el != y_root->el) {
      if (x_root->rank < y_root->rank) {
        std::swap(x_root, y_root);
      }
    }

    // Make x_root the new root.
    y_root->parent = make_shared<node>(*x_root);
    // If necessary, increment the rank of x_root.
    if (x_root->rank == y_root->rank) {
      x_root->rank++;
    }
  }
  
private:
  struct node {
    T el;
    std::shared_ptr<node> parent;
    uint rank;
  };
  std::unordered_map<T, node> nodes;

  // std::shared_ptr<node> _find(const std::shared_ptr<node> &x) {
  //   if (x->parent) {
  //     x->parent = this->_find(x->parent);
  //     return x->parent;
  //   } else {
  //     return x;
  //   }
  // }
};
