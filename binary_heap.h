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
      this->_ixs[k] = this->_heap.size();
      this->_heap.push_back({k, v});
      this->_heapify_up(this->_heap.size()-1);
    } else {
      throw std::invalid_argument("key already exists");
    }
  }

  // Extract the minimum element from the heap.
  std::pair<K, V> extract() {
    auto root = this->_heap.front();
    this->_heap[0] = this->_heap.back();
    this->_heap.pop_back();
    this->_ixs.erase(root.first);
    this->_ixs[this->_heap[0].first] = 0;
    this->_heapify_down(0);
    return root;
  }

  // Associate to key [k] a new value [v] (must be less than or equal
  // to the previous value associated with [k]).
  void decrease_key(const K &k, const V &v) {
    uint i = this->_ixs[k];
    this->_heap[i].second = v;
    this->_heapify_up(i);
  }

  constexpr uint size() const {
    return this->_heap.size();
  }

  constexpr bool contains(const K &k) {
    return this->_ixs.contains(k);
  }
  
private:
  std::vector<std::pair<K, V>> _heap;
  std::unordered_map<K, uint> _ixs;

  void _swap(uint i, uint j) {
    this->_ixs[this->_heap[i].first] = j;
    this->_ixs[this->_heap[j].first] = i;
    std::swap(this->_heap[i], this->_heap[j]);
  }

  // Heapify up at index [i].
  void _heapify_up(uint i) {
    if (i > 0) {
      uint parent_i = i / 2;
      if (this->_heap[i].second < this->_heap[parent_i].second) {
        this->_swap(i, parent_i);
        this->_heapify_up(parent_i);
      }
    }
  }

  // Heapify down at index [i].
  void _heapify_down(uint i) {
    uint left_i = 2 * i;
    uint right_i = 2 * i + 1;

    uint smallest = i;
    if (left_i < this->_heap.size() &&
        this->_heap[left_i].second < this->_heap[smallest].second) {
      smallest = left_i;
    }
    if (right_i < this->_heap.size() &&
        this->_heap[right_i].second < this->_heap[smallest].second) {
      smallest = right_i;
    }
    if (smallest != i) {
      this->_swap(smallest, i);
      this->_heapify_down(smallest);
    }
  }
};
