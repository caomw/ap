#ifndef _AGGLO_H
#define _AGGLO_H

#include "Global.h"
//#include "HeapData.h"
#include "DynArr.h"
#include "DynArrDefines.h"
#include "Matrix.h"
#include "LabeledSet.h"

class Agglomerative{
 public:
  CDynArrayMinHeap *distHeap;
  HeapData **matrix;
  int elementNum;
  int size;
  LabeledSet **clusterArray;

  Agglomerative(Matrix *);
  ~Agglomerative();
  void merge(int, int);
  void runAlgorithm(int *clusterVector);

  inline int index(int i, int j){
    if (i < j){
      return (elementNum - 1)*i - i*(i+1)/2 + (j-1);
    }    
    if (i > j){
      return (elementNum - 1)*j - j*(j+1)/2 + (i-1);
    }
    return size;
  }
};

#endif
