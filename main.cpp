#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>

#include "BSTree2.hpp"

using namespace std;

int main() {
  BSTree<int> bt;
  bt.insert(10);
  bt.insert(20);
  bt.insert(30);
  bt.insert(5);
  bt.insert(15);
  cout << bt << endl;
  auto it = bt.begin();
  cout << *it << endl;
  ++it;
  cout << *it << endl;
  ++it;
  cout << *it << endl;
    for (auto &item : bt) {
      cout << "item: ";
      cout << item << endl;
    }
}