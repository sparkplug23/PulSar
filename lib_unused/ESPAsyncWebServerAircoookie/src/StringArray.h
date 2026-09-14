/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef STRINGARRAY_H_
#define STRINGARRAY_H_

#include "stddef.h"
#include "WString.h"

template <typename T>
class LinkedListNode {
    T _value;
  public:
    LinkedListNode<T>* next;
    LinkedListNode(T val): _value(std::move(val)), next(nullptr) {}
    ~LinkedListNode(){}
    const T& value() const { return _value; };
    T& value(){ return _value; }
};

template <typename T, template<typename> class Item = LinkedListNode>
class LinkedList {
  public:
    typedef Item<T> ItemType;
    typedef std::function<void(const T&)> OnRemove;
  private:
    ItemType* _root;
    ItemType* _last;    
    OnRemove _onRemove;

    class Iterator {
      ItemType* _node;
      friend class LinkedList;
    public:
      Iterator(ItemType* current = nullptr) : _node(current) {};
      Iterator& operator ++() {
          if (_node) _node = _node->next;
          return *this;
      }
      bool operator != (const Iterator& i) const { return _node != i._node; }
      const T& operator * () const { return _node->value(); }
      const T* operator -> () const { return &_node->value(); }
    };

    void _remove(ItemType* pit, ItemType* it) {
      auto* next = pit ? &pit->next : &_root;
      *next = it->next;
      if (_last == it) {
        _last = pit;
      }

      if (_onRemove) {
        _onRemove(it->value());
      }
      delete it;
    }
    
  public:
    typedef const Iterator ConstIterator;
    ConstIterator begin() const { return ConstIterator(_root); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    LinkedList(OnRemove onRemove) : _root(nullptr), _last(nullptr), _onRemove(onRemove) {}
    ~LinkedList() { free(); }

    LinkedList(const LinkedList<T, Item>&) = delete;
    LinkedList(LinkedList<T, Item>&&) = delete;
    LinkedList& operator=(const LinkedList<T, Item>&) = delete;
    LinkedList& operator=(LinkedList<T, Item>&&) = delete;


    void add(T t){
      auto it = new ItemType(std::move(t));
      if(!_root){
        _root = it;
        _last = it;
      } else {
        _last->next = it;
      }
      _last = it;
    }

    T& front() const {
      return _root->value();
    }
    
    bool isEmpty() const {
      return _root == nullptr;
    }

    size_t length() const {
      size_t i = 0;
      for(auto it = _root; it != nullptr; it = it->next) { ++i; };
      return i;
    }

    template<typename Predicate>
    size_t count_if(const Predicate& predicate) const {
      size_t i = 0;
      for(auto it = _root; it != nullptr; it = it->next) {
        if (predicate(it->value())) {
          i++;
        }
      }
      return i;
    }

    const T* nth(size_t N) const {
      size_t i = 0;
      for(auto it = _root; it != nullptr; it = it->next) {
        if(i++ == N) return &(it->value());              
      };
      return nullptr;
    }

    bool remove(const T& t){
      auto pit = (ItemType*) nullptr;
      for(auto it = _root; it != nullptr; pit = it, it = it->next) {
        if(it->value() == t){
          _remove(pit, it);
          return true;
        }
      }
      return false;
    }
    
    template<typename Predicate>
    bool remove_first(const Predicate& predicate){
      auto pit = (ItemType*) nullptr;
      for(auto it = _root; it != nullptr; pit = it, it = it->next) {
        if(predicate(it->value())){
          _remove(pit, it);
          return true;
        }
      }
      return false;
    }

    bool remove(const ConstIterator& t, const ConstIterator& where) {
      if (where._node) {
        if ((where._node->next) != t._node) return false;
        _remove(where._node, t._node);
        return true;
      } else {
        return remove(t);
      }
    }

    bool remove(const ConstIterator& t) {
      auto pit = (ItemType*) nullptr;
      for(auto it = _root; it != nullptr; pit = it, it = it->next) {
        if(it == t._node){
          _remove(pit, it);
          return true;
        }
      }
      return false;      
    }

    void free(){
      while(_root != nullptr){
        _remove(nullptr, _root);
      }
    }
};


class StringArray : public LinkedList<String> {
public:
  
  StringArray() : LinkedList(nullptr) {}
  
  bool containsIgnoreCase(const String& str){
    for (const auto& s : *this) {
      if (str.equalsIgnoreCase(s)) {
        return true;
      }
    }
    return false;
  }
};




#endif /* STRINGARRAY_H_ */
