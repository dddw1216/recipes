// Copyright (c) 2024, dddw1216. All rights reserved.

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <cassert>

namespace recipes::cache {

template <class K, class V>
class DemoLruCache {
 public:
  struct Node {
    K key_{};
    V val_{};
    Node* p_prev_ = nullptr;
    Node* p_next_ = nullptr;
  };

 public:
  DemoLruCache(uint32_t cap) {
    assert(cap > 0);
    cap_ = cap;
    p_entries_ = new Node[cap];
    for (uint32_t i = 0; i < cap; i++) {
      free_list_.emplace(&(p_entries_[i]));
    }
    hash_map_.reserve(cap);
    p_head_ = new Node();
    p_tail_ = new Node();
    p_head_->p_prev_ = nullptr;
    p_head_->p_next_ = p_tail_;
    p_tail_->p_prev_ = p_head_;
    p_tail_->p_next_ = nullptr;
  }

  ~DemoLruCache() {
    if (p_head_ != nullptr) {
      delete p_head_;
    }
    if (p_tail_ != nullptr) {
      delete p_tail_;
    }
    if (p_entries_ != nullptr) {
      delete[] p_entries_;
    }
  }

  int Put(const K& key, const V& val) {
    auto it = hash_map_.find(key);
    if (it == hash_map_.end()) {
      if (free_list_.empty()) {
        Node* p_rm_node = NodeRemoveFromTail();
        assert(p_rm_node != nullptr);
        assert(!free_list_.empty());
      }
      Node* p_node = free_list_.front();
      free_list_.pop();
      p_node->key_ = key;
      p_node->val_ = val;
      hash_map_[key] = p_node;
      NodeInsertToHead(p_node);
    } else {
      Node* p_node = it->second;
      p_node->val_ = val;
      NodeDetach(p_node);
      NodeInsertToHead(p_node);
    }
    return 0;
  }

  int Get(const K& get_key, V& get_val) {
    auto it = hash_map_.find(get_key);
    if (it == hash_map_.end()) {
      return 1;
    }

    Node* p_node = it->second;
    get_val = p_node->val_;
    NodeDetach(p_node);
    NodeInsertToHead(p_node);
    return 0;
  }

  std::string DebugToString() const {
    std::stringstream ss;
    ss << "cap=" << cap_ << ",map_size=" << hash_map_.size() << ",free_size=" << free_list_.size() << "\n";
    uint32_t idx = 0;
    Node* p_node = p_head_->p_next_;
    while (p_node != p_tail_) {
      ss << "node_i=" << idx << ",key=" << p_node->key_ << ",val=" << p_node->val_ << "\n";
      p_node = p_node->p_next_;
      idx++;
    }
    return ss.str();
  }

 private:
  void NodeDetach(Node* p_node) {
    Node* p_curr_prev = p_node->p_prev_;
    Node* p_curr_next = p_node->p_next_;
    p_node->p_prev_->p_next_ = p_curr_next;
    p_node->p_prev_ = nullptr;
    p_node->p_next_->p_prev_ = p_curr_prev;
    p_node->p_next_ = nullptr;
  }

  void NodeInsertToHead(Node* p_node) {
    Node* p_curr_head = p_head_->p_next_;
    p_node->p_next_ = p_curr_head;
    p_node->p_prev_ = p_head_;
    p_head_->p_next_ = p_node;
    p_curr_head->p_prev_ = p_node;
  }

  Node* NodeRemoveFromTail() {
    if (p_tail_->p_prev_ == p_head_) {
      return nullptr;
    }
    Node* p_rm_node = p_tail_->p_prev_;
    p_tail_->p_prev_ = p_rm_node->p_prev_;
    p_rm_node->p_prev_->p_next_ = p_tail_;
    p_rm_node->p_prev_ = nullptr;
    p_rm_node->p_next_ = nullptr;
    free_list_.push(p_rm_node);
    return p_rm_node;
  }

 private:
  uint32_t cap_ = 0;
  std::unordered_map<K, Node*> hash_map_;
  std::queue<Node*> free_list_;
  Node* p_head_ = nullptr;
  Node* p_tail_ = nullptr;
  Node* p_entries_ = nullptr;
};

} // namespace recipes::cache