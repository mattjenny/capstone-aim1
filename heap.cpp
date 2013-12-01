#include "heap.h"
#include "dsexceptions.h"
#include <vector>
#include <iostream> 

heap::heap(int capacity):array(capacity+1), currentSize(0)
{
}

heap::heap(const vector<huffmanNode*> items):array(items.size()+10), currentSize(items.size())
{
  for(int i = 0; i < items.size(); i++)
	array[i+1] = items[i];
      buildHeap();
}

  bool heap::isEmpty() const
  { return currentSize == 0; }

  huffmanNode* heap::findMin() const
  {
    if(isEmpty())
      throw UnderflowException();
    return array[1];
  }

  void heap::insert(huffmanNode* x)
  {
    if(currentSize == array.size()-1)
      array.resize(array.size()*2);

    int hole = ++currentSize;
    for( ; hole > 1 && x->f < array[hole/2]->f; hole /= 2)
      array[hole] = array[hole/2];
    array[hole] = x;
  }

  void heap::deleteMin()
  {
    if(isEmpty())
      throw UnderflowException();

    array[1] = array[currentSize--];
    percolateDown(1);
  }

  void heap::deleteMin(huffmanNode* minItem)
  {
    if(isEmpty())
      throw UnderflowException();

    minItem = array[1];
    array[1] = array[currentSize--];
    percolateDown(1);
  }

  void heap::makeEmpty()
  { currentSize = 0; }

  void heap::buildHeap()
  {
    for(int i = currentSize/2; i > 0; i--)
      percolateDown(i);
  }

void heap::percolateDown(int hole)
  {
    int child;
    huffmanNode* tmp = array[hole];

    for( ; hole*2 <= currentSize; hole = child)
      {
	child = hole*2;
	if(child != currentSize && array[child+1]->f < array[child]->f)
	  child++;
	if(array[child]->f < tmp->f)
	  array[hole] = array[child];
	else
	  break;
      }
    array[hole] = tmp;
  }

int heap::size()
{ return currentSize; }

vector<huffmanNode*> heap::tree()
{ return array; }
