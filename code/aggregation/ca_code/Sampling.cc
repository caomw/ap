#include "Sampling.h"
#include "Utils.h"
#include "omp.h"
#define NUM_THREADS 4
#define MAX_SIZE_CLUSTER 100
#define MAX_NUM_DISTANCES 100

Sampling::Sampling(string filename, int samplesize)
{
  ifstream fn(filename.c_str());
  fn >> N >> K;
  S = MIN(N, samplesize);
  //cerr << "size " << N << " attributes " << K << " sample size "<< S << endl;

  data = new string*[N];
  sample = new string*[S];
  sampleIndex = new int[S];

  srand48(1973);
  for (int i=0; i<N; i++) {
    data[i] = new string[K];
    for (int j=0; j < K; j++)
      fn >> data[i][j];
    if (i < S){
      sample[i] = data[i];
      sampleIndex[i] = i;
    } else {
      int r = (int) floor(drand48() * (i+1));
      if (r < S) {
	sample[r] = data[i];
	sampleIndex[r] = i;
      }
    }
  } 
  fn.close();
  /*
  for (int i = 0; i <N; i ++){
    for (int j = 0; j < K; j ++){
      cout << data[i][j]<< " ";
    }
    cout <<endl;
  }

  
  
  cout<<"\nsample!\n";
  for (int i = 0; i <S; i ++){
    cout << sampleIndex[i]<<":";
    for (int j = 0; j < K; j ++){
      cout << sample[i][j]<< " ";
    }
    cout <<endl;
  }
  cout << "lalalaaaa"<<endl;
  */
}

void Sampling::computeMatrix(Matrix *matrix)
{
  int i,j;
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel private(i,j) 
  {    
#pragma omp for
    for (i = 0; i < S; i ++){
      //cout << "sample: "<< sampleIndex[i]<<endl;
      for (j = 0; j < S; j ++){	
	matrix->matrix[i][j] = distance(sample,i,j,K);
	//cout << matrix->matrix[i][j]<< " ";
      }
      //cout << endl;
    }
  }//end of parallel section
}


Sampling::~Sampling()
{
  for (int i = 0; i < N; i ++)
    delete [] data[i];
  delete [] data;
  /*
  This was a BUG! No new is made for sample[i] 
  for (int i = 0; i < S; i ++)
    delete [] sample[i]; 
  */
  delete [] sample;
  delete [] sampleIndex;
}
  
void Sampling::postProcess(int *clusterVector, int nodesNum, int *sampleClusters, int SampleSize)
  // int *Sampling::postProcess(int *sampleClusters)
{
  omp_set_num_threads(NUM_THREADS);

  vector <string *> singletonVector;
  vector <int> singletonIndex;

  if (SampleSize!=S || nodesNum!=N)
    error("Something has gone terribly wrong");

  map<int, vector<int> > clusterMap;
  map<int,int> clusterSizes;
  set<int> sampleSet;
  vector<int> clusterLabels;
  
  for (int i = 0; i < S; i ++){
    int label = sampleIndex[sampleClusters[i]];
    clusterMap[label].push_back(sampleIndex[i]);
    clusterVector[sampleIndex[i]] = label;
    sampleSet.insert(sampleIndex[i]);
  }

  for (map<int, vector<int> >::iterator entry = clusterMap.begin(); entry != clusterMap.end(); entry ++){
    int key = (*entry).first;
    clusterSizes[key] = ((*entry).second).size();
    cout << "size of cluster "<<key <<" :"<<clusterSizes[key]<<endl;
  }
  
  for (int i = 0; i < N; i ++){    
    //if (i%1==0)
      cout << i << "/ "<< N << endl << flush;
    
    if (sampleSet.find(i) != sampleSet.end()) 
      continue;
    
    map<int,float> distances; /* keeps the sum of disagreements of i with the elements of each cluster 
                                 the key is the cluster label, the entry the sum */
    map<int,float> tot_dist; /* keeps the cost of assigning i to each cluster 
				the key is the cluster label the value the cost */
    
    tot_dist[-1] = 0; // the -1 entry is the case for making i into a sigleton cluster
 
   
    //cout << "Parallel It:";
    for (map<int, vector<int> >::iterator entry = clusterMap.begin(); entry != clusterMap.end(); entry ++){
      int key = (*entry).first;
      distances[key] = 0;
      vector<int> cluster = (*entry).second;

      //Read iterators first
      deque<int> it_que;
      for (vector<int>::iterator iter = cluster.begin(); iter != cluster.end(); iter ++){
	it_que.push_back(*iter);       
      }

      int it_que_size = it_que.size();
      int j;      
      if( it_que_size > MAX_SIZE_CLUSTER )
	{
	  //cout << "-" << key;
#pragma omp parallel private(j) 
	  {   
#pragma omp for schedule(static, it_que_size/NUM_THREADS)
	    for(j=0; j<it_que_size; j++)
	      distances[key] += distance(data, i, it_que[j], K);
	  }//Parallel section
	}
      else
	for(j=0; j<it_que_size; j++)
	  distances[key] += distance(data, i, it_que[j], K);
	
	  
      
      tot_dist[-1] += clusterSizes[key] - distances[key];
      //cout << "dist is " << distances[key] << " tot dist " << tot_dist[-1] << endl;
    }
    //cout << endl;

    
    // cout << "node " << i << " cost for being sigleton " << tot_dist[-1] << endl;
    int num_distances = distances.size();
    for (map<int,float>::iterator iter = distances.begin(); iter != distances.end(); iter ++){
      int key = (*iter).first;
      tot_dist[key] = 0;
      
      if(num_distances < MAX_NUM_DISTANCES){	
	for (map<int,float>::iterator inner = distances.begin(); inner != distances.end(); inner ++){
	  int other_key = (*inner).first;
	  float value = (*inner).second;
	  if (key == other_key){
	    tot_dist[key] += value;
	  }else{
	    tot_dist[key] += clusterSizes[other_key] - value;
	  }
	}
      }
      else{
	
	deque<int> key_que;
	deque<float> value_que;
	
	for (map<int,float>::iterator inner = distances.begin(); inner != distances.end(); inner ++){
	  key_que.push_back((*inner).first);
	  value_que.push_back((*inner).second);
	}
	
	int  j;
	//Parallel loop
#pragma omp parallel private(j) 
	{   
	  
#pragma omp for schedule(static, num_distances/NUM_THREADS)
	  for (j=0;j<num_distances;j++){	  
	    if (key == key_que[j]){
	      tot_dist[key] += value_que[j];
	    }else{
	      tot_dist[key] += clusterSizes[key_que[j]] - value_que[j];
	    }
	  }
	}
	
	


      }
	
	  

      //cout << "node " << i << " cost of merging with " << key << ": " << tot_dist[key] << endl;     
    }
    
    float min_dist = tot_dist[-1]; // used to be -1 to create singletons
    int min_key = -1; //same here
    for (map<int,float>::iterator iter = tot_dist.begin(); iter != tot_dist.end(); iter ++){
      int key = (*iter).first;
      float value = (*iter).second;
      // cout << "key: " << key << " value: " << value << endl;
      if (value < min_dist){ // && key != -1){ // added && to avoid singletons
	min_dist = value;
	min_key = key;
      }
    }
    if (min_key == -1){ // assign i into a sigleton
      clusterVector[i] = i;
      singletonVector.push_back(data[i]);
      singletonIndex.push_back(i);
    }else{
      clusterVector[i] = min_key;
    }
  }
  cout << endl;
  cout << "Cluster singletons" << endl;
  clusterSigletons(clusterVector, &singletonVector, &singletonIndex);
}

void Sampling::clusterSigletons(int *clusterVector, vector<string *> *singletonVector, vector<int> *singletonIndex)
{

  int singletonSize = singletonVector->size();
  if (singletonSize < 2){
    cout << "Less than 2 singletons"<<endl;
    return;
  }
  cout << "number of singletons: "<< singletonSize<<endl;
  string **singletonData = new string*[singletonSize];
  for (int i = 0; i < singletonSize; i ++){
    singletonData[i] = (*singletonVector)[i];
  }
  Matrix *singletonMatrix = new Matrix(singletonSize);
  int i,j;
omp_set_num_threads(NUM_THREADS);
#pragma omp parallel private(i,j) 
  {    
#pragma omp for
  for (i = 0; i < singletonSize; i ++){
    //cout << "sample: "<< sampleIndex[i]<<endl;
    for (j = 0; j < singletonSize; j ++){
      singletonMatrix->matrix[i][j] = distance(singletonData,i,j,K);
      //cout << matrix->matrix[i][j]<< "\t";
    }
  }
  }
  
  int singletonLabels[singletonSize];
  if (BaseAlgo==NOB){
    for (int i = 0; i < singletonSize; i ++){
      singletonLabels[i] = i;
    }
  }
  else if (BaseAlgo==BALLS){
    Balls(singletonMatrix, singletonLabels, Alpha);
  }
  else if (BaseAlgo==RANDOMSPLITS){
    RandomSplits(singletonMatrix, singletonLabels, RSmaxIters);
  }
  else if (BaseAlgo==GONZALEZ){
    Gonzalez(singletonMatrix, singletonLabels);
  }
  else if (BaseAlgo==AGGLOMERATIVE) {
    Agglomerative agglo(singletonMatrix);  
    agglo.runAlgorithm(singletonLabels);
  } 

  if (LocalSearchAlgo==LOCALSEARCH){
    LocalSearch(singletonMatrix, singletonLabels, LSmaxIters);
  }
  else if (LocalSearchAlgo==D_LOCALSEARCH){
    BestLocalSearch(singletonMatrix, singletonLabels);
  }
  


  for (int i = 0; i < singletonSize; i ++){
    clusterVector[(*singletonIndex)[i]] = (*singletonIndex)[singletonLabels[i]];
    //cout << "cluster label for entry "<< i <<" "<<(*singletonIndex)[i]<< ": "<<singletonLabels[i] << " "<<(*singletonIndex)[singletonLabels[i]]<<endl; 
  }

  delete [] singletonData;
  delete singletonMatrix;
}
  
