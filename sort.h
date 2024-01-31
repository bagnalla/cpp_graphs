// In-place sorting algorithms on vectors. Generic in the type of
// elements (subject to the constraint of being a total order).

#pragma once

#include <cstring>
#include <vector>

// #include "binary_heap.h"
#include "common.h"

namespace sort {

  // For testing.
  template <std::totally_ordered T>
  bool is_sorted(std::vector<T> &v) {
    for (uint i = 0; i < v.size()-1; i++) {
      if (v[i] > v[i+1]) {
        return false;
      }
    }
    return true;
  }

  // Early exit when input doesn't change after a swapping pass.
  template <std::totally_ordered T>
  void bubble_sort(std::vector<T> &v) {
    bool changed = true;
    while (changed) {
      changed = false;
      for (uint i = 0; i < v.size()-1; i++) {
        if (v[i] > v[i+1]) {
          std::swap(v[i], v[i+1]);
          changed = true;
        }
      }
    }
  }

  template <std::totally_ordered T>
  void selection_sort(std::vector<T> &v) {
    for (uint i = 0; i < v.size()-1; i++) {
      uint min = std::numeric_limits<uint>::max();
      uint min_j = 0;
      for (uint j = i; j < v.size(); j++) {
        if (v[j] < min) {
          min = v[j];
          min_j = j;
        }
      }
      std::swap(v[i], v[min_j]);
    }
  }

  template <std::totally_ordered T>
  void insertion_sort(std::vector<T> &v) {
    // This algorithm seems nicer with linked lists, since insertion
    // can be done in constant time (after finding the right
    // location).
  }

  template <std::totally_ordered T>
  void heap_sort(std::vector<T> &v) {
  }

  template <std::totally_ordered T>
  void merge_sort(std::vector<T> &v) {
    auto merge = [&v](uint lb, uint mid, uint ub) {
      // TODO: Hmm.. is it possible without using extra space, and
      // without requiring extra linear passes over the elements?'
      uint sz = (ub-lb)*sizeof(uint);
      uint *a = (uint*)std::malloc(sz);
      std::memcpy(a, &v[lb], sz);

      uint i = 0;
      uint j = mid - lb;
      for (uint k = lb; k < ub; k++) {
	if (i < mid - lb && (j == ub - lb || a[i] < a[j])) {
	  v[k] = a[i++];
	} else {
	  v[k] = a[j++];
	}
      }
      
      std::free(a);
    };
    
    std::function<void(uint, uint)> sort;
    sort = [&merge, &sort](uint lb, uint ub) {
      if (ub - lb <= 1) {
        return;
      }
      uint mid = (lb + ub) / 2;
      sort(lb, mid);
      sort(mid, ub);
      merge(lb, mid, ub);
    };
    sort(0, v.size());
  }
}
