#ifndef HEAP_H
#define HEAP_H

#include "huffmanNode.h"
#include <vector>
using namespace std;

class heap
{
 public:
  explicit heap(int capacity = 100);
  explicit heap(const vector<huffmanNode*> v);
  bool isEmpty() const;
  huffmanNode* findMin() const;
  void insert(huffmanNode* x);
  void deleteMin();
  void deleteMin(huffmanNode* minItem);
  void makeEmpty();
  int size();
  vector<huffmanNode*> tree();

 private:
  int currentSize;
  vector<huffmanNode*> array;
  void buildHeap();
  void percolateDown(int hole);
};
#endif
