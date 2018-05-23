#pragma once

struct Node {
  Node() :__next(this) {}
  void setNext(Node * _val) {__next = _val;}
  Node * getNext() {return __next;}
  Node * getLast() {
    Node * last = this;
    while(last->getNext() != this) {
      last = last->getNext();
    }
    return last;
  }
private:
  Node * __next;
};
