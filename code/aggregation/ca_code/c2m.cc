#include "Global.h"
#include "distance.h"

int main(int argc, char *argv[])
{
  int N,k;
  cin >> N >> k;
  
  string **data = new string*[N];
  for (int i=0; i<N; i++) {
    data[i] = new string[k];
    for (int j=0; j<k; j++)
      cin >> data[i][j];
  }

  cout << N << endl;
  for (int i=0; i<N; i++) {
    if (i%100==0)
      cerr << i << " ";
    for (int j=0; j<N; j++) 
      cout << distance(data,i,j,k) << " ";
    cout << endl;
  }

  for (int i=0; i<N; i++) 
    delete[] data[i];
  delete[] data;
}
