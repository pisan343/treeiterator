
#ifndef BSTree_h
#define BSTree_h

#include <iostream>
#include <iterator>

// Node with left and right child
template <class T> class Node {
public:
  explicit Node(T val) : val{val} {}
  T val = 0;
  Node *left = nullptr;
  Node *right = nullptr;
};

// forward declaration of Binary Search Tree iterator (primary template)
template <class T> class BSTreeIter;

// templated Binary Search Tree
template <class T> class BSTree {

  friend class BSTreeIter<T>;

  // simplifying Node<T>
  using BNode = Node<T>;
  // simplifying BSTreeIter<T>
  using iterator = BSTreeIter<T>;

  // friend function needs its own template that is not T
  template <class T2>
  friend std::ostream &operator<<(std::ostream &out, const BSTree<T2> &n);

private:
  // helper to printing
  static std::ostream &printSideways(std::ostream &out, const BNode *curr,
                                     int level);
  // helper to find Node
  BNode *findNodeWithParent(int value, BNode *curr, BNode *&p);
  // root of tree
  BNode *_root = nullptr;
  // number of Nodes
  int _size = 0;

public:
  // iterator pointing to the first element
  iterator begin();

  // iterator pointing to the past-the-end element
  iterator end();

  // size of tree
  int size();

  // add a new item, return true if successful
  bool insert(int value);
};

// Binary Search Tree iterator
template <class T> class BSTreeIter {
  // simplifying Node<T>
  using BNode = Node<T>;
  // simplifying BSTreeIter<T>
  using iterator = BSTreeIter<T>;

public:
  // constructor
  BSTreeIter(BSTree<T> *bstree, int counter);
  // dereference
  T &operator*();
  // preincrement
  iterator &operator++();
  // postincrement
  iterator &operator++(int);
  bool operator==(const iterator &other);
  bool operator!=(const iterator &other);

private:
  BSTree<T> *_bstree;
  std::stack<BNode *> _stack;
  int _counter;
};

template <class T> BSTreeIter<T> BSTree<T>::begin() {
  return BSTreeIter<T>(this, 0);
}

template <class T> BSTreeIter<T> BSTree<T>::end() {
  return BSTreeIter<T>(this, _size);
}

template <class T> int BSTree<T>::size() { return _size; }

template <class T>
Node<T> *BSTree<T>::findNodeWithParent(int value, BNode *curr, BNode *&p) {
  if (curr == nullptr || curr->val == value) {
    return curr;
  }
  p = curr;
  auto *child = value < curr->val ? curr->left : curr->right;
  return findNodeWithParent(value, child, p);
}

template <class T> bool BSTree<T>::insert(int value) {
  if (_root == nullptr) {
    _size++;
    _root = new BNode(value);
    return true;
  }
  BNode *parent = nullptr;
  if (findNodeWithParent(value, _root, parent) != nullptr) {
    return false;
  }
  auto *n = new BNode(value);
  if (value < parent->val) {
    parent->left = n;
  } else {
    parent->right = n;
  }
  _size++;
  return true;
}

template <class T2>
std::ostream &operator<<(std::ostream &out, const BSTree<T2> &n) {
  return n.printSideways(out, n._root, 0);
}

template <class T>
std::ostream &BSTree<T>::printSideways(std::ostream &out, const BNode *curr,
                                       int level) {
  const static char space = ' ';
  const static int readabilitySpaces = 4;
  if (curr == nullptr) {
    return out;
  }
  printSideways(out, curr->right, ++level);
  out << std::setfill(space) << std::setw(level * readabilitySpaces) << space;
  out << curr->val << std::endl;
  printSideways(out, curr->left, level);
  return out;
}

template <class T>
BSTreeIter<T>::BSTreeIter(BSTree<T> *bstree, int counter)
    : _bstree{bstree}, _counter(counter) {
  if (_counter != 0) {
    return;
  }
  auto curr = _bstree->_root;
  while (curr != nullptr) {
    _stack.push(curr);
    curr = curr->left;
  }
}
template <class T> T &BSTreeIter<T>::operator*() {
  assert(!_stack.empty() && "No more values left");
  return _stack.top()->val;
}
// preincrement
template <class T> BSTreeIter<T> &BSTreeIter<T>::operator++() {
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
template <class T> BSTreeIter<T> &BSTreeIter<T>::operator++(int) {
  BSTreeIter<T> clone(*this);
  operator++();
  return clone;
}
template <class T> bool BSTreeIter<T>::operator==(const iterator &other) {
  return this->_bstree == other._bstree && this->_counter == other._counter;
}

template <class T> bool BSTreeIter<T>::operator!=(const iterator &other) {
  // cout << "Calling != ";
  // cout << (this->mytree != other.mytree || this->mystack != other.mystack) <<
  // endl;
  return !this->operator==(other);
}

#endif // BSTree_h