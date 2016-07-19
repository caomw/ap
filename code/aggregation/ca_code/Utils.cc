#include <string>
#include <iostream>
#include <fstream>
#include "Matrix.h"
#include "Utils.h"
#include "Optimal.h"
#include "omp.h"
#define NUM_THREADS 3

void error(const char *errmsg)
{
  cerr << "Error: " << errmsg << endl;
  exit (1);
}

void printClustering(int *cvector, int N, string filename) 
{
  ofstream output(filename.c_str());
  if (!output.is_open())
    error("Cannot open output file!");
  for (int i=0; i<N; i++) 
    output << cvector[i] << endl;
  output.close();
}

int max(int *a, int n) 
{
  int M = -INT_MAX;
  for (int i=0; i<n; i++)
    M = (a[i]>M)?a[i]:M;
  return M;
}

void printComparison(ofstream *log, int *clustering, int *classlabels, int N)
{
  int noclusters = getnoclusters(N, clustering);
  relabelclusters(N, clustering);
  int noclasses = getnoclusters(N, classlabels);
  relabelclusters(N, classlabels);
  int confusionMatrix[noclusters][noclasses];
  for (int i=0; i<noclusters; i++)
    for (int j=0; j<noclasses; j++)
      confusionMatrix[i][j]=0;
  int total[noclusters];
  for (int i=0; i<noclusters; i++)
    total[i] = 0;
  for (int i=0; i<N; i++) {
    confusionMatrix[clustering[i]][classlabels[i]]++;
    total[clustering[i]]++;
  }
  // Classification error
  float errors = 0.0;
  for (int i=0; i<noclusters; i++)
    errors += total[i] - max(confusionMatrix[i], noclasses);
  (*log) << "ClassError " << errors/(float)N << endl;
  // Confusion Matrix
  (*log) << "ConfusionMatrix" << endl;
  for (int i=0; i<noclusters; i++) {
    for (int j=0; j<noclasses; j++)
      (*log) << confusionMatrix[i][j] << " ";
    (*log) << endl;
  }
  // Pairs disagree
  int disagree = 0;
  for (int i=1; i<N; i++) 
    for (int j=0; j<i; j++)
      disagree += ((clustering[i]==clustering[j])!=(classlabels[i]==classlabels[j]));
  (*log) << "PairsDisagree " << (float)disagree/((float)N*((float)N-1)/2.0) << endl;
}


void printLog(Matrix *dist, int *samplelabels, int SampleSize, 
	      int *labels, int *classlabels, int N, 
	      string ClassFile, string LogFile) 
{
  ofstream log(LogFile.c_str());
  if (!log.is_open())
   error("Cannot open output log file!");
  log << "SAMPLING STATS" << endl;
  log << "NoClusters " << getnoclusters(SampleSize, samplelabels) << endl;
  log << "DistError " << clusteringcost(dist, samplelabels) << endl;
  log << "OptVector " << optimal(dist) <<endl;
  log << "CLUSTERING STATS" << endl;
  log << "NoClusters " << getnoclusters(N, labels)  << endl;
  if (ClassFile!="")
    printComparison(&log, labels, classlabels, N);
  log.close();
}

int getnoclusters(int N, int *labels) {
  int clusterlabels[N];
  int noclusters = 0;
  for(int i=0; i<N; i++) {
    int exists = 0;
    for(int j=0; j<noclusters; j++)
      if (clusterlabels[j]==labels[i]) {
	exists = 1;
	break;
      }
    if (!exists)
      clusterlabels[noclusters++] = labels[i];
  } 
  return noclusters;
}

void relabelclusters(int N, int *labels) {
  // Relabel cluster labels to be 0,1,2,...,noclusters-1
  int clusterlabels[N];
  int noclusters = 0;

    
  for(int i=0; i<N; i++) {
    int exists = 0;
    for(int j=0; j<noclusters; j++)
      if (clusterlabels[j]==labels[i]) {
	exists = 1;
	break;
      }
    if (!exists)
      clusterlabels[noclusters++] = labels[i];
  }  
  
  omp_set_num_threads(NUM_THREADS);
  

  int newlabels[N];
  for(int j=0; j<noclusters; j++)
    for(int i=0; i<N; i++)
      if (labels[i]==clusterlabels[j])
	newlabels[i] = j;
 
  for(int i=0; i<N; i++)
    labels[i] = newlabels[i];
}

float clusteringcost(Matrix *dist, int *labels) {
  int N = dist->size;
  float cost=0.0;
  int i;
  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel private(i) 
  {    
#pragma omp for  
  for(i=1; i<N; i++)
    for(int j=0; j<i; j++)
      if (labels[i]==-1 || labels[j]==-1) // missing values
	cost += 0.5; 
      else {
	if (labels[i]==labels[j])
	  cost += dist->matrix[i][j];
	else
	  cost += 1-dist->matrix[i][j];
      }
  }//end parallel section
  return cost;
}

float distance(string **data, int i, int j, int k)
{
  if (i==j)
    return 0.0;
  float disagree=0.0;
  int a;


  
  for (a=0; a<k; a++){
    //cout << data[i][a] << "<--> "<<data[j][a]<<endl;
    if (data[i][a]== MISSING || data[j][a]==MISSING){
      disagree += 0.5;
    }
    else if (data[i][a]!=data[j][a]){
      disagree += 1.0;
    }
  }
 
  

  return disagree/(float)k;
}

//Parallel version of the distance function
float distance_parallel(string **data, int i, int j, int k)
{
  if (i==j)
    return 0.0;
  float disagree=0.0;
  int a;
  int chunk_size = k/2;
  
  //--PARALLEL SECTION
#pragma omp parallel private(a) 
  {    
#pragma omp for schedule(dynamic, chunk_size) 
  for (a=0; a<k; a++){
    //cout << data[i][a] << "<--> "<<data[j][a]<<endl;
    if (data[i][a]== MISSING || data[j][a]==MISSING){
      disagree += 0.5;
    }
    else if (data[i][a]!=data[j][a]){
      disagree += 1.0;
    }
  }
  }//--PARALLEL SECTION
 
  

  return disagree/(float)k;
}


void copy(int *dst, int *src, int N)
{
  int i;
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel private(i) 
  {    
#pragma omp for    
  for (i=0; i<N; i++)
    dst[i] = src[i];
  }
}
  
