
#include "Agglomerative.h"

Agglomerative::Agglomerative(Matrix *dmatrix)
{  
  elementNum = dmatrix->size;
  size = elementNum*(elementNum - 1)/2 ;
  matrix = new HeapData*[size + 1];
  distHeap = new CDynArrayMinHeap;

  for (int i = 0; i < elementNum; i ++){
    for (int j = i+1; j < elementNum; j ++){
      HeapData *hdata = new HeapData;
      hdata->m_row = i;
      hdata->m_col = j;
      hdata->m_key = dmatrix->matrix[i][j];
      matrix[index(i,j)] = hdata;
      distHeap->AddElement(hdata);
      //cout << "pos = "<< index(i,j) <<" pointer = "<<hdata<<endl;
    }
  }
  matrix[size] = NULL;
  distHeap->BuildHeap();
  //delete dmatrix;
}

Agglomerative::~Agglomerative(){
  delete [] matrix;
  delete distHeap;
}

void Agglomerative::merge(int i, int j)
  // merge row i with row j into row i
  // precondition: i < j
{
  
  int old_iClusterSize = clusterArray[i]->findSet()->size;
  int old_jClusterSize = clusterArray[j]->findSet()->size;
  clusterArray[i]->merge(clusterArray[j]);
  int newClusterSize = clusterArray[i]->findSet()->size;
  //  cout <<"new size of cluster "<<i<<" = "<<clusterArray[i]->findSet()->size<<endl;
  matrix[index(i,j)] = NULL;
  //cout << index(i,j)<<endl;

  for (int k = 0; k < elementNum; k ++){
   if (matrix[index(i,k)] != NULL){
     //cout << index(i,k)<<endl;
     //cout << index(j,k)<<endl;
     HeapData *ik_data = matrix[index(i,k)];
     HeapData *jk_data = matrix[index(j,k)];
     
     //int kClusterSize = clusterArray[k]->findSet()->size;
  
     //cout << "i cluster size "<<  iClusterSize << " k cluster size "<<  kClusterSize <<endl;
     ik_data->m_key = (ik_data->GetKey()*old_iClusterSize + jk_data->GetKey()*old_jClusterSize)/(newClusterSize);
     //cout << "update pair ("<<i<<","<<k<<") new dist = "<<ik_data->m_key<<endl;
     TEvictionPos pos = ik_data->GetEvictionPos();
     distHeap->Heapify(pos);
     //cout << "remove pair ("<<j<<","<<k<<") "<<endl;
     //cout << jk_data->GetEvictionPos()<<endl;
     distHeap->Remove(jk_data->GetEvictionPos());
     //cout << "ok?"<<endl;
     delete jk_data;
     //cout << index(j,k)<<endl;
     matrix[index(j,k)] = NULL;
   }
  }
}

void Agglomerative::runAlgorithm(int *clusterVector)
{
  int clusters = elementNum;
  clusterArray = new LabeledSet*[elementNum];
  for (int i = 0; i < clusters; i ++){
    clusterArray[i] = new LabeledSet(i);
  }

  /**
  while (distHeap->RootKey() <= 0.5) {
   cout << "min dist = "<< distHeap->RootKey();
   HeapData *minPair = distHeap->Remove();
   cout << "("<<minPair->GetRow()<<","<<minPair->GetCol()<<")"<<endl;
  }
  **/
    
  //  cout << "min dist = "<< distHeap->RootKey()<<endl;
  while (clusters > 1 && distHeap->RootKey() <= 0.5) {
    //cout << "clusters = "<<clusters<<endl;
    HeapData *minPair = distHeap->Remove();
    int i = minPair->GetRow();
    int j = minPair->GetCol();
    //    cout << "merge nodes "<<i<<","<<j<<" dist = "<<minPair->m_key<< endl;
    merge(i,j);
    clusters -- ;
    delete minPair;
  }
  
  //  cout <<  "clusters = "<< clusters<<endl;

  for (int i = 0; i < elementNum; i ++){
    clusterVector[i] = clusterArray[i]->findSet()->label;
  }
}
