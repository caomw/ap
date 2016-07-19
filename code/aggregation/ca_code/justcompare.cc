
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


void error(char *errmsg)
{
  cerr << "Error: " << errmsg << endl;
  exit (1);
}

int max(int *a, int n) 
{
  int M = -INT_MAX;
  for (int i=0; i<n; i++)
    M = (a[i]>M)?a[i]:M;
  return M;
}

void readclustering(string filename, int N, int *clustering) 
{
  ifstream input(filename.c_str());
  if (!input.is_open())
    error("Can not open input file!");
  for (int i=0; i<N; i ++)
    input >> clustering[i];
  //  cerr << "Clustering read\n";
  input.close();
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
  int newlabels[N];
  for(int j=0; j<noclusters; j++)
    for(int i=0; i<N; i++)
      if (labels[i]==clusterlabels[j])
	newlabels[i] = j;
  for(int i=0; i<N; i++)
    labels[i] = newlabels[i];
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

int main(int argc, char *argv[])
{
  int N;
  cin >> N;
  
  int *clust1 = new int[N];
  int *clust2 = new int[N];
  
  string file1 = argv[1];
  readclustering(file1, N, clust1);

  string file2 = argv[2];
  readclustering(file2, N, clust2);

  ofstream out(argv[3]);
  printComparison(&out, clust1, clust2, N);

  delete[] clust1;
  delete[] clust2;
}
