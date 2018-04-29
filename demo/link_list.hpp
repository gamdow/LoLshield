#pragma once

struct Node {
  Node() :__next(this) {}
  void setNext(Node * _val) {__next = _val;}
  Node * getNext() const {return __next;}
  Node * getLast() const {
    Node * last = this;
    while(last->getNext() != this) {
      last = last->getNext();
    }
    return last;
  }
private:
  Node * __next;
};
