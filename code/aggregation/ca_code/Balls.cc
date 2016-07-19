#include "Balls.h"
#include "omp.h"
#define NUM_THREADS 4

void Balls(Matrix *distMatrix, int *clusterVector, float alpha){

  int nodesNum = distMatrix->size;

  /**** new! ****/
  float wDegree[nodesNum];
  int i,j;
  for (i = 0; i < nodesNum; i ++){
    wDegree[i] = 0;    
    for (j = 0; j < nodesNum; j ++){
      wDegree[i] += distMatrix->matrix[i][j];
    }   
  }
  /**** new !*****/

  int indices[nodesNum];
  for (int i = 0; i < nodesNum; i ++){
    indices[i] = i;
  }

  list<int> activeNodes;

  /**********  old! ********
  srand48(1973);
  for (int i = nodesNum - 1; i >= 0; i --){
    int r = (int) floor(drand48() * (i+1));
    int k = indices[r];
    indices[r] = indices[i];
    indices[i] = k;
    activeNodes.push_back(k);  
  }
  ********************/

  /**** new! ****/
 for (int i = 0; i < nodesNum; i ++){
    float min = wDegree[i];
    int minIndex = i;
    
    int j;
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel private(j) 
    {    
#pragma omp for   
      for (j = i+1; j < nodesNum; j ++){	
      if (min > wDegree[j]){
	min = wDegree[j];
	minIndex = j;
      }
    }
    }

    
    if (i != minIndex){
      int k = indices[i];
      //wDegree[i] = max; // the swap is not necessary since we will not consider it again
      wDegree[minIndex] = wDegree[i];
      indices[i] = indices[minIndex];
      indices[minIndex] = k;
    }
    activeNodes.push_back(indices[i]);
    //    cout << "inserted node "<<indices[i]<<endl;
  }
  /**** new! ****/

      
  //  delete [] indices;

  int label = 0;
  while (!activeNodes.empty()){
    //cout << "active num = "<< activeNodes.size() <<endl;
    int node = activeNodes.front();
    activeNodes.pop_front(); // do I need that?
    //    cout << "node "<< node << " selected for cluster center"<<endl;
    clusterVector[node] = node; //better than label since we know the center

    deque<int> cluster;
    float avg_dist = 0;
    for (list<int>::iterator lp = activeNodes.begin(); lp != activeNodes.end(); lp ++){
      int j = *lp;
      //cout << j << " " << distMatrix->matrix[node][j] << endl;
      if (distMatrix->matrix[node][j] <= 0.5){
	avg_dist += distMatrix->matrix[node][j];
	cluster.push_back(j);
	//cout << "pushed "<< j <<endl;
      } 
    }
    //if (cluster.size() == 0 || avg_dist > alpha*cluster.size()){
    //  activeNodes.erase(center);
    //}else{
    if (cluster.size() > 0 && avg_dist<= alpha*cluster.size()){
      //      cout << "cluster size = "<< cluster.size() <<endl;
      list<int>::iterator lp = activeNodes.begin();
      list<int>::iterator start = lp;
      while (!cluster.empty()){
	int j = cluster.front();
	//cout << "j "<< j<< endl;
	cluster.pop_front();
	clusterVector[j] = node;
	for (lp = start; *lp != j && lp!= activeNodes.end(); lp ++){
	  //cout << *lp << endl;
	}
	start = activeNodes.erase(lp);
      }
    }else{
      //  cout << "node "<<node<<" in a sigleton cluster"<<endl;
    }
    label ++ ;
  }
}
    

  
