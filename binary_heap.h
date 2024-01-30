// Array-backed min-heap.

#pragma once

#include <unordered_map>
#include <vector>

template <typename K, std::totally_ordered V>
class binary_heap {
public:
  
  // Insert a key/value pair into the heap.
  void insert(const K &k, const V &v) {
    if (!this->contains(k)) {
      this->ixs[k] = this->heap.size();
      this->heap.push_back({k, v});
      this->heapify_up(this->heap.size()-1);
    } else {
      throw std::invalid_argument("key already exists");
    }
  }

  // Extract the minimum element from the heap.
  std::pair<K, V> extract() {
    auto root = this->heap.front();
    this->heap[0] = this->heap.back();
    this->heap.pop_back();
    this->ixs.erase(root.first);
    this->ixs[this->heap[0].first] = 0;
    this->heapify_down(0);
    return root;
  }

  // Associate to key [k] a new value [v] (must be less than or equal
  // to the previous value associated with [k]).
  void decrease_key(const K &k, const V &v) {
    uint i = this->ixs[k];
    this->heap[i].second = v;
    this->heapify_up(i);
  }

  constexpr uint size() const {
    return this->heap.size();
  }

  constexpr bool contains(const K &k) {
    return this->ixs.contains(k);
  }
  
private:
  std::vector<std::pair<K, V>> heap;
  std::unordered_map<K, uint> ixs;

  void swap(uint i, uint j) {
    this->ixs[this->heap[i].first] = j;
    this->ixs[this->heap[j].first] = i;
    std::swap(this->heap[i], this->heap[j]);
  }

  // Heapify up at index [i].
  void heapify_up(uint i) {
    if (i > 0) {
      uint parent_i = i / 2;
      if (this->heap[i].second < this->heap[parent_i].second) {
        this->swap(i, parent_i);
        this->heapify_up(parent_i);
      }
    }
  }

  // Heapify down at index [i].
  void heapify_down(uint i) {
    uint left_i = 2 * i;
    uint right_i = 2 * i + 1;

    uint smallest = i;
    if (left_i < this->heap.size() &&
        this->heap[left_i].second < this->heap[smallest].second) {
      smallest = left_i;
    }
    if (right_i < this->heap.size() &&
        this->heap[right_i].second < this->heap[smallest].second) {
      smallest = right_i;
    }
    if (smallest != i) {
      this->swap(smallest, i);
      this->heapify_down(smallest);
    }
  }
};
