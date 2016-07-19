#ifndef _LOCALSEARCH_H
#define _LOCALSEARCH_H

#include "Matrix.h"

void RandomSplits(Matrix *dist, int *labels, int maxIters);
void Gonzalez(Matrix *dist,  int *labels);
void LocalSearch(Matrix *dist, int *labels, int maxIters);
void BestLocalSearch(Matrix *dist, int *labels);

#endif 
