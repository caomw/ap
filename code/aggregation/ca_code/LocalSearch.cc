
#include "LocalSearch.h"
#include "Utils.h"

int furthestpoint(Matrix *dist) {
  int N = dist->size;
  float maxdist = 0.0;
  int furthest = 0;
  for (int i=0; i<N; i++) {
    float disti = 0.0;  
    for (int j=0; j<N; j++) 
      disti += dist->matrix[i][j];
    if (disti>maxdist) {
      maxdist = disti;
      furthest = i;
    }
  }
  return furthest;
}

void Gonzalez(Matrix *dist, int *labels) {
  int N = dist->size;
  int centers[N];
  int noCenters = 1;
  centers[0] = 0; 
  // strangely enough, the replacing the previous line with the following
  // (starting with the furthest point) does not work so well!!
  //  centers[0] = furthestpoint(dist);
  for (int i=0; i<N; i++)
    labels[i] = centers[0];
  int newlabels[N];
  float cost = clusteringcost(dist, labels);
  //  cerr << "Original cost " << cost << endl;
  while (1) {
    float maxdist = 0.0;
    int furthest = 0;
    for (int i=0; i<N; i++) {
      float disti = FLT_MAX;  
      for (int j=0; j<noCenters; j++) 
	disti = MIN(dist->matrix[i][centers[j]], disti);
      if (disti>maxdist) {
	maxdist = disti;
	furthest = i;
      }
    }
    for (int j=0; j<noCenters; j++)
      if (furthest==centers[j])
	break; // an already existing point found as center, probably run out of points
    //    cerr << "furthest point " << furthest << endl;
    for (int i=0; i<N; i++) 
      if (dist->matrix[i][furthest]<dist->matrix[i][labels[i]])
	newlabels[i] = furthest;
      else
    	newlabels[i] = labels[i];
    float newcost = clusteringcost(dist, newlabels);
    //    cerr << "New cost " << newcost << endl;
    if (newcost<cost) {
      cost = newcost;
      for (int i=0; i<N; i++) 
	labels[i] = newlabels[i];
      centers[noCenters++] = furthest;
    } else 
      break; // no improvement in the score
  }
}

void RandomSplits(Matrix *dist, int *labels, int maxIters) {
  int N = dist->size;
  int newlabels[N];
  for (int i=0; i<N; i++) 
    labels[i] = 0;
  srand48(19);
  int noIters = 0;
  float cost = clusteringcost(dist, labels);
  //  cerr << "Original cost " << cost << endl;
  while (noIters++ < maxIters) {
    int candidatecenter = (int) floor(drand48()*N);
    //    cerr << candidatecenter << " ";
    if (labels[candidatecenter] == candidatecenter) {
      //      cerr << "center exists" << endl;   
      continue;  // it is already a center
    }
    for (int i=0; i<N; i++) 
      if (dist->matrix[i][candidatecenter]<dist->matrix[i][labels[i]])
	newlabels[i] = candidatecenter;
      else
    	newlabels[i] = labels[i];
    float c = clusteringcost(dist, newlabels);
    if (c<cost) {
      //      cerr << "center accepted " << endl;
      cost = c;
      for (int i=0; i<N; i++) 
	labels[i] = newlabels[i];
    }// else cerr << "center rejected" << endl;   
    //    cerr << "New cost: " << cost << endl;
  }
}

void findbestcluster(Matrix *dist, int *labels, int noclusters, 
		     int toreassign, int& bestcluster, float& improvement) {
  int N = dist->size;
  int clustersizes[noclusters];
  float disttoclusters[noclusters];
  for(int i=0; i<noclusters; i++) {
    disttoclusters[i] = 0.0;
    clustersizes[i] = 0;
  }
  for(int i=0; i<N; i++) {
    if (i==toreassign)
      continue;
    disttoclusters[labels[i]] += dist->matrix[toreassign][i];
    clustersizes[labels[i]]++;
  }
  float totaldist = 0.0;
  for(int i=0; i<noclusters; i++) 
    totaldist += (clustersizes[i]-disttoclusters[i]);
  float mincost = FLT_MAX;
  for(int i=0; i<noclusters; i++) {
    float cost = totaldist - clustersizes[i] + 2*disttoclusters[i];
    if (cost<mincost) {
      mincost = cost;
      bestcluster = i;
    }
  }
  // Examine if it would be better as singleton
  if (totaldist<mincost) {
    mincost = totaldist;
    bestcluster = noclusters;
  }
  float originalcost = totaldist - clustersizes[labels[toreassign]] + 2*disttoclusters[labels[toreassign]];
  improvement = originalcost - mincost;
}

void LocalSearch(Matrix *dist, int *labels, int maxIters) {
  //  cout << "Cost before " << clusteringcost(dist, labels) << endl;
  int N = dist->size;
  int noclusters = getnoclusters(N, labels);
  relabelclusters(N, labels);
  srand48(19);
  int noIters = 0;
  while (noIters++ < maxIters) {
    int toreassign = (int) floor(drand48()*N);
    //    cerr << "point " << toreassign ;
    int bestcluster;
    float improvement;
    findbestcluster(dist, labels, noclusters, toreassign, bestcluster, improvement);
    if (improvement>0) {
      if (bestcluster==noclusters) {
	// better as a singleton
	//	cout << "SINGLETON " << toreassign << " -- " << flush;
	labels[toreassign] = noclusters++;
      } else {
	// reassign it to another cluster center
	//	cout << "Reassign " << toreassign << " to " << bestcluster << " -- " << flush;
	labels[toreassign] = bestcluster;
      }
    } else {
      // cerr << " Move rejected! -- " << flush;
      continue;
    }
  }
  //  cout << "Cost after " << clusteringcost(dist, labels) << endl;
}

void BestLocalSearch(Matrix *dist, int *labels) {
  //  cout << "Cost before " << clusteringcost(dist, labels) << endl;
  int N = dist->size;
  int noclusters = getnoclusters(N, labels);
  relabelclusters(N, labels);
  float improvement;
  do {
    improvement = 0.0;
    int bestpoint = 0;
    int bestcluster = labels[bestpoint];
    for (int i=0; i<N; i++) {
      int clusteri;
      float imprv;
      findbestcluster(dist, labels, noclusters, i, clusteri, imprv);
      if (imprv>improvement) {
        improvement = imprv;
        bestpoint = i;
        bestcluster = clusteri;
      }
    }
    if (improvement<=0.0)
      break;
    if (bestcluster==noclusters) {
      // better as a singleton
      //      cout << "SINGLETON " << bestpoint << " -- " << flush;
      labels[bestpoint] = noclusters++;
    } else {
      // reassign it to another cluster center
      //      cout << "Reassign " << bestpoint << " to " << bestcluster << " -- " << flush;
      labels[bestpoint] = bestcluster;
    }
  } while(1);
  //  cout << "Cost after " << clusteringcost(dist, labels) << endl;
}

/*

IN THE FOLLOWING PIECE OF CODE THERE WAS AN ATTEMPT TO USE ONE ADDITIONAL LOCAL "MOVE":
POINT IS CONSIDERED TO BE A "CENTER", TO WHICH THE CLOSEST POINTS ARE ASSIGNED TO.
UNFORTUNATELY THE NEW MOVE WAS EXTREMELY SLOW (DUE TO THE CALL TO CLUSTERINGCOST() IN A 
FOR LOOP -- THAT IS, CUBIC RUNNING TIME), AND THE RESULT DIDN'T CHANGED AT ALL :(((

float useascenter(Matrix *dist, int *labels, int candCenter)
{
  int N = dist->size;
  int newlabels[N];
  for (int i=0; i<N; i++)
    if (dist->matrix[i][candCenter]<dist->matrix[i][labels[i]])
      newlabels[i] = candCenter;
    else
      newlabels[i] = labels[i];
  return clusteringcost(dist, newlabels);
}


void useascenterforreal(Matrix *dist, int *labels, int newCenter)
{
  int N = dist->size;
  for (int i=0; i<N; i++)
    if (dist->matrix[i][newCenter]<dist->matrix[i][labels[i]])
      labels[i] = newCenter;
}

void findbestcenter(Matrix *dist, int *labels, 
		    int *centers, int noCenters, 
		    int toreassign, 
		    int& bestcenter, float& improvement) {
  int N = dist->size;
  // Notice that in the following two arrays of size N only noCenters posititions are used !!!
  // (in particular the positions whose indices are contained in the first noCenter positions of the array centers)
  // (also these are the positions indexed by labels[i], for any i)
  // (OK, I know, a little iffy, but...)
  int clustersizes[N];
  float disttoclusters[N];
  for(int i=0; i<noCenters; i++) {
    disttoclusters[centers[i]] = 0.0;
    clustersizes[centers[i]] = 0;
  }
  for(int i=0; i<N; i++) {
    if (i==toreassign)
      continue;
    disttoclusters[labels[i]] += dist->matrix[toreassign][i];
    clustersizes[labels[i]]++;
  }
  float totaldist = 0.0;
  for(int i=0; i<noCenters; i++) 
    totaldist += (clustersizes[centers[i]]-disttoclusters[centers[i]]);
  float mincost = FLT_MAX;
  // Examine which is the best new center to reassign
  for(int i=0; i<noCenters; i++) {
    float cost = totaldist - clustersizes[centers[i]] + 2*disttoclusters[centers[i]];
    if (cost<mincost) {
      mincost = cost;
      bestcenter = centers[i];
    }
  }
  // Examine if it would be better as singleton
  if (totaldist<mincost) {
    mincost = totaldist;
    bestcenter = toreassign;
  }
  float originalcost = totaldist - clustersizes[labels[toreassign]] + 2*disttoclusters[labels[toreassign]];
  improvement = originalcost - mincost;
}

void findCenters(int *centers, int &noCenters, int *labels, int N) {
  noCenters = 0;
  for(int i=0; i<N; i++) {
    int exists = 0;
    for(int j=0; j<noCenters; j++)
      if (centers[j]==labels[i]) {
	exists = 1;
	break;
      }
    if (!exists)
      centers[noCenters++] = labels[i];
  } 
}

int centerexists(int *centers, int noCenters, int c) {
  for(int i=0; i<noCenters; i++) 
    if (centers[i]==c) 
      return 1;
  return 0;
}

void BestLocalSearch(Matrix *dist, int *labels) {
  cerr << "Cost before " << clusteringcost(dist, labels) << endl;
  int N = dist->size;
  int centers[N];
  int noCenters;
  findCenters(centers, noCenters, labels, N);
  float improvement;
  do {
    improvement = 0.0;
    int bestpoint = 0;
    int bestcenter = labels[bestpoint];
    for (int i=0; i<N; i++) {
      if (centerexists(centers, noCenters, i))
	continue;
      int centerofi;
      float imprv;
      findbestcenter(dist, labels, centers, noCenters, i, centerofi, imprv);
      if (imprv>improvement) {
	improvement = imprv;
	bestpoint = i;
	bestcenter = centerofi;
      }
      float cost = clusteringcost(dist, labels);
      float newcost = useascenter(dist, labels, i);
      if (cost-newcost>improvement) {
	improvement = cost-newcost;
	bestpoint = i;
	bestcenter = -1;
      }
    }
    cout << bestpoint <<  " " << improvement << endl;
    if (improvement<=0.0)
      break;
    if (bestcenter==bestpoint) {
      // better as a singleton
      cout << "SINGLETON " << bestpoint << " -- " << flush;
      labels[bestpoint] = bestpoint;
      centers[noCenters++] = bestpoint;
    } else if (bestcenter==-1) {
      // make it a new center
      useascenterforreal(dist, labels, bestpoint);
      centers[noCenters++] = bestpoint;
    } else {
      // reassign it to another cluster center
      cout << "Reassign " << bestpoint << " to " << bestcenter << " -- " << flush;
      labels[bestpoint] = bestcenter;
    }
  } while(1);
  cerr << "Cost after " << clusteringcost(dist, labels) << endl;
}

*/
