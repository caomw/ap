#ifndef _UTILS_H
#define _UTLIS_H

#include "Global.h"
#include "Matrix.h"
#include <limits.h>

#define MAX(A,B) ((A)>(B)?(A):(B))
#define MIN(A,B) ((A)<(B)?(A):(B))

void error(const char *errmsg);
void printClustering(int *cvector, int N, string filename);
void printLog(Matrix *dist, int *samplelabels, int SampleSize, 
	      int *labels, int *classlabels, int N, 
	      string ClassFile, string LogFile);
int getnoclusters(int N, int *labels);
void relabelclusters(int N, int *labels);
float clusteringcost(Matrix *dist, int *labels);
float distance(string **data, int i, int j, int k);
float distance_parallel(string **data, int i, int j, int k);
void copy(int *dst, int *src, int N);
void printComparison(ofstream *log, int *clustering, int *classlabels, int N);

#endif 
