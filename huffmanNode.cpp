#include "huffmanNode.h"
#include <stddef.h>

huffmanNode::huffmanNode():c('0'), f(0), t(false), left(NULL), right(NULL)
{
}

huffmanNode::huffmanNode(char x, int y, bool z, huffmanNode* l, huffmanNode* r):c(x), f(y), t(z), left(l), right(r)
{
}

char huffmanNode::getC()
{ return c; }

int huffmanNode::getF()
{ return f; }

bool huffmanNode::getT()
{ return t; }

huffmanNode* huffmanNode::getLeft()
{ return left; }

huffmanNode* huffmanNode::getRight()
{ return right; }

void huffmanNode::setC(char x)
{
  c = x;
}

void huffmanNode::setF(int x)
{
  f = x;
}

void huffmanNode::setLeft(huffmanNode *l)
{
  left = l;
}

void huffmanNode::setRight(huffmanNode *r)
{
  right = r;
}
