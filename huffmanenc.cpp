//Linda Zhang, lz9rg, 04/15/13

#include "huffmanNode.h"
#include "heap.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace std;

int check(char x, char d[])
{
  int ret = -1;
  int i = 0;
  while(d[i] != '*')
    {
      if(d[i] == x)
	ret = i;
      i++;
    }
  return ret;
}

string search(char x, string s, huffmanNode* n)
{
  //cout << s << endl;
  if(!n->getT())
    {
      string l = s;
      string r = s;
      l += '0';
      r += '1';
      if(search(x, l, n->getLeft()).length() < search(x, r, n->getRight()).length())
	{
	  return search(x, r, n->getRight());
	}
      if(search(x, l, n->getLeft()).length() > search(x, r, n->getRight()).length())
	{
	  return search(x, l, n->getLeft());
	}
      if(s.length()>1)
	return s.substr(0,s.length()-2);
      else
	return "";
    }
  if(n->getC() == x)
    {
      //cout << "win: " << s << endl;
      return s;
    }
  else
    {
      //cout << "failed: " << s.substr(0, s.length()-2) << endl;
      if(s.length()>1)
	return s.substr(0,s.length()-2);
      else
	return "";
    }
}

int main(int argc, char *argv[])
{
  if(argc !=2)
    {
      cout << "Must supply the input file name as the one and only parameter" << endl;
      exit(1);
    }
  
  FILE *fp = fopen(argv[1], "r");
  if(fp == NULL)
    {
      cout << "File '" << argv[1] << "' does not exist!" << endl;
      exit(2);
    }

  int count = -1;
  char ch[100];
  int freq[100];

  fill_n(ch, 100, '*');
  fill_n(freq, 100, 0);

  char g;
  while((g = fgetc(fp)) != EOF)
    {
      if(g != '\t' && g != '\r')
	{
	  int test = check(g, ch);
	  if(test != -1)
	    {
	      freq[test]++;
	    }
	  else
	    {
	      count++;
	      ch[count] = g;
	      freq[count] = 1;
	    }
	}
    }

  
  heap *h = new heap();
  int k = 0;

  while(ch[k] != '*')
    {
      huffmanNode *p = new huffmanNode(ch[k], freq[k], true, NULL, NULL);
      h->insert(p);
      k++;
    }

  /*vector<huffmanNode*> v = h->tree();
  for(int z=1; z <= h->size(); z++)
    {
      cout << z << " " <<  v[z]->getC() << " " << v[z]->getF()  << endl;
      }*/


  huffmanNode *root;
  //make huffman tree
  while(!h->isEmpty())
    {
      huffmanNode *a = h->findMin();
      h->deleteMin();
      huffmanNode *b = h->findMin();
      h->deleteMin();
      huffmanNode *n = new huffmanNode('*', a->getF()+b->getF(), false, a, b);
      if(!h->isEmpty())
	h->insert(n);
      else
	root = n;
    }

  //get the huffman code
  string huff[100];
  fill_n(huff, 100, "");

  k = 0;
  while(ch[k] != '*')
    {
      huff[k] = search(ch[k], "", root);
      k++;
    }

  k = 0;
  int frequency = 0;
  int bits = 0;
  while(ch[k] != '*')
    {
      cout << ch[k] << " " << huff[k] << "\t";
      frequency += freq[k];
      bits += freq[k]*huff[k].length();
      k++;
    }
  cout << "--\t";
  rewind(fp);
  while((g = fgetc(fp)) != EOF)
    {
      if(g != '\t' && g != '\r')      
	cout << huff[check(g, ch)] << " ";
    }
  
  float ratio = (float)frequency*8/(float)bits;
  float hratio = (float)bits/(float)frequency;

  cout << "\t--" << endl;
  /*
  cout << "There are a total of " << frequency << " symbols that are encoded." << endl;
  cout << "There are " << k << " distinct symbols used." << endl;
  cout << "There were " << frequency*8 << " bits in the original file." << endl;
  cout << "There were " << bits << " bits in the compressed file." << endl;
  cout << "This gives a compression ratio of " << ratio  << "." << endl;
  cout << "The cost of the Huffman tree is " << hratio << " bits per character." << endl;
  */
  fclose(fp);
}
