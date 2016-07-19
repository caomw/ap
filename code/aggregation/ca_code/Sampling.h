#ifndef _SAMPLE_H
#define _SAMPLE_H

#include "Global.h"
#include "Matrix.h"
#include "Agglomerative.h"
#include "LocalSearch.h"
#include "Balls.h"
#include <string>
extern void RandomSplits(Matrix *dist, int *labels, int maxIters);
extern void Gonzalez(Matrix *dist,  int *labels);
extern void LocalSearch(Matrix *dist, int *labels, int maxIters);
extern void BestLocalSearch(Matrix *dist, int *labels);
extern void Balls(Matrix *dist, int *clusterVector, float alpha);

class Sampling
{
 public:
  int N,K,S;
  string **data;
  string **sample;
  int *sampleIndex;
  
  Sampling(string filename, int samplesize);
  ~Sampling();
  void computeMatrix(Matrix *matrix);
  void postProcess(int *clusterVector, int nodesNum, int *sampleClusters, int SampleSize);
  void clusterSigletons(int *, vector<string *> *, vector<int> *);

};

#endif
