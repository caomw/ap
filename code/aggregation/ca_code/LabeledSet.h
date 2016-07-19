#ifndef _LSET_H
#define _LSET_H

#include "Global.h"

/*******
 LabeledSet: Implements a union find structure to maintain a set
*******/

class LabeledSet
{
 public:
  int node;
  int label;
  int size;
  LabeledSet *parent;
  
  LabeledSet(int);
  ~LabeledSet();
  LabeledSet *findSet();
  void merge(LabeledSet *);

 private:
  int rank;
  LabeledSet *link(LabeledSet *);
  LabeledSet *unionSets(LabeledSet *);
};

#endif
