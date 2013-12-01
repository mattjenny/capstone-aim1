#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

using namespace std;

class huffmanNode
{
 public:
  huffmanNode();
  huffmanNode(char x, int f, bool z, huffmanNode *left, huffmanNode *right);
  char getC();
  int getF();
  bool getT();
  huffmanNode* getLeft();
  huffmanNode* getRight();
  void setC(char x);
  void setF(int x);
  void setLeft(huffmanNode *l);
  void setRight(huffmanNode *r);

 private:
  char c;
  int f;
  bool t;
  huffmanNode *left, *right;
  friend class heap;
};
#endif
