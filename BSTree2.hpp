
#ifndef BSTree_h
#define BSTree_h

#include <iostream>
#include <iterator>

using std::endl;
using std::ostream;
using std::setfill;
using std::setw;
using std::stack;

// templated Binary Search Tree
template <class T> class BSTree {

  // friend function needs its own template that is not T
  template <class T2>
  friend ostream &operator<<(ostream &out, const BSTree<T2> &n);

public:
  //////////////////////////////
  // iterator
  //////////////////////////////
  class Node {
  public:
    explicit Node(T val) : val{val} {}
    T val = 0;
    Node *left = nullptr;
    Node *right = nullptr;
  };

  //////////////////////////////
  // iterator
  //////////////////////////////
  class iterator {
  public:
    // constructor
    iterator(BSTree *bstree, int counter) : _bstree{bstree}, _counter(counter) {
      if (_counter != 0) {
        return;
      }
      auto curr = _bstree->_root;
      while (curr != nullptr) {
        _stack.push(curr);
        curr = curr->left;
      }
    }
    // dereference
    T &operator*() { return _stack.top()->val; }

    // preincrement
    iterator &operator++() {
      assert(!_stack.empty() && "No more values left");
      auto *curr = _stack.top();
      _stack.pop();
      // if it has a right child,
      // put the right child and all its children in stack
      curr = curr->right;
      if (curr != nullptr) {
        _stack.push(curr);
        curr = curr->left;
        while (curr != nullptr) {
          _stack.push(curr);
          curr = curr->left;
        }
      }
      _counter++;
      return *this;
    }
    // postincrement
    iterator &operator++(int) {
      iterator clone(*this);
      operator++();
      return clone;
    }
    bool operator==(const iterator &other) {
      return _bstree == other._bstree && _counter == other._counter;
    }
    bool operator!=(const iterator &other) { return !operator==(other); }

  private:
    BSTree *_bstree;
    stack<Node *> _stack;
    int _counter;
  };

  //////////////////////////////
  // BSTree
  //////////////////////////////
public:
  // iterator pointing to the first element
  iterator begin() { return iterator(this, 0); }

  // iterator pointing to the past-the-end element
  iterator end() { return iterator(this, _size); }

  // size of tree
  int size() { return _size; }

  // add a new item, return true if successful
  bool insert(int value);

private:
  // helper to printing
  static ostream &printSideways(ostream &out, const Node *curr, int level);
  // helper to find Node
  Node *findNodeWithParent(int value, Node *curr, Node *&p);
  Node *_root = nullptr;
  // number of Nodes
  int _size = 0;
};

// insert a Node into BST.return true if successfull
template <class T> bool BSTree<T>::insert(int value) {
  if (_root == nullptr) {
    _size++;
    _root = new Node(value);
    return true;
  }
  Node *parent = nullptr;
  if (findNodeWithParent(value, _root, parent) != nullptr) {
    return false;
  }
  auto *n = new Node(value);
  if (value < parent->val) {
    parent->left = n;
  } else {
    parent->right = n;
  }
  _size++;
  return true;
}

// need to tell compiler that BSTree<T>::Node is a typename and not a namespace
template <class T>
typename BSTree<T>::Node *BSTree<T>::findNodeWithParent(int value, Node *curr,
                                                        Node *&p) {
  if (curr == nullptr || curr->val == value) {
    return curr;
  }
  p = curr;
  auto *child = value < curr->val ? curr->left : curr->right;
  return findNodeWithParent(value, child, p);
}

// insertion operator for BSTree
template <class T> ostream &operator<<(ostream &out, const BSTree<T> &n) {
  return BSTree<T>::printSideways(out, n._root, 0);
}

// helper for insertion operator for BSTree
template <class T>
ostream &BSTree<T>::printSideways(ostream &out, const Node *curr, int level) {
  const static char space = ' ';
  const static int readabilitySpaces = 4;
  if (curr == nullptr) {
    return out;
  }
  printSideways(out, curr->right, ++level);
  out << setfill(space) << setw(level * readabilitySpaces) << space;
  out << curr->val << endl;
  printSideways(out, curr->left, level);
  return out;
}

#endif // BSTree_h