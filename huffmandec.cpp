//Linda Zhang, lz9rg, 04/16/13

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "huffmanNode.h"
using namespace std;

int buildTree(char x, string s, huffmanNode *pt)
{
  if(s.length()==1)
    {
      huffmanNode *n = new huffmanNode(x, 0, true, NULL, NULL);
      if(s[0]=='0')
	pt->setLeft(n);
      else 
	pt->setRight(n);
      return 1;
    }
  char dir = s[0];
  s = s.substr(1);
  
  huffmanNode *n;
  if(dir=='0')
    {
      if(pt->getLeft() == NULL)
	{
	  n = new huffmanNode('*', 0, false, NULL, NULL);
	  pt->setLeft(n);
	}
      else
	n = pt->getLeft();
    }
  else
    {
      if(pt->getRight() == NULL)
	{
	  n = new huffmanNode('*', 0, false, NULL, NULL);
	  pt->setRight(n);
	}
      else
	n = pt->getRight();
    }
  buildTree(x, s, n);
  return 0;
}

char getChar(string x, huffmanNode *pt)
{
  if(x.length()==0)
    return pt->getC();
  char dir = x[0];
  x = x.substr(1);

  if(dir == '0')
    return getChar(x, pt->getLeft());
  return getChar(x, pt->getRight());
}

void print(huffmanNode *pt)
{
  cout << pt->getC();
  if(pt->getLeft() != NULL)
    print(pt->getLeft());
  if(pt->getRight() != NULL)
    print(pt->getRight());
}
int main (int argc, char **argv) {

  if ( argc != 2 ) {
    cout << "Must supply the input file name as the only parameter" << endl;
    exit(1);
  }

  // attempt to open the supplied file; must be opened in binary
  // mode, as otherwise whitespace is discarded
  ifstream file(argv[1], ifstream::binary);

  // report any problems opening the file and then exit
  if ( !file.is_open() ) {
    cout << "Unable to open file '" << argv[1] << "'." << endl;
    exit(2);
  }

  huffmanNode *root = new huffmanNode('*', 0, false, NULL, NULL);

  // read in the first section of the file: the prefix codes
  char buffer[256];
  while ( true ) {
    char first = file.get();
    if ( (first == '\n') || (first == '\r') )
      continue;
    char second = file.get();
    if ( (first == '-') && (second == '-') ) {
      file.getline(buffer, 255, '\n');
      break;
    }
    // read in the prefix code
    file.getline(buffer, 255, '\n');
    buildTree(first, buffer, root);
  }

  //print out tree
  //print(root);
  //cout << endl;
  //cout << "----------------------------------------" << endl;
  //read in the second section of the file: the encoded message
  char bit;
  string code = "";
  while ( (bit = file.get()) != '-' ) {
    if ( (bit != '0') && (bit != '1') )
      {
	char z = getChar(code, root);
	if(z != '*')
	  cout << z;
	code = "";
	continue;
      }
    code += bit;
  }
  cout << endl;
  // close the file before exiting
  file.close();
}
