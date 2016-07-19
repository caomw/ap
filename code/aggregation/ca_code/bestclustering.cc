
#include <cfloat>     // FLT_MAX

using namespace std;

float clusteringdistance(char **data, int a, int b, int N)
{
  if (a==b)
    return 0;
  float dist=0.0;
  for (int i=1; i<N; i++)
    for (int j=0; j<i; j++)
      if (data[i][a]=='?' || data[j][a]=='?' || data[i][b]=='?' || data[j][b]=='?')
	dist += 0.5;
      else if ((data[i][a]==data[j][a])!=(data[i][b]==data[j][b]))
	dist += 1.0;
  return dist;
}

int main(int argc, char *argv[])
{
  int N,k;
  cin >> N >> k;
  
  char **data = new char*[N];
  for (int i=0; i<N; i++) {
    data[i] = new char[k];
    for (int j=0; j<k; j++)
      cin >> data[i][j];
  }
  
  int best = 0;
  float mindist = FLT_MAX;
  for (int i=0; i<k; i++) {
    float dist = 0;
    for (int j=0; j<k; j++) 
      dist += clusteringdistance(data, i, j, N);
    if (dist<mindist) {
      mindist = dist;
      best = i;
    }
  }

  cerr << "Best clustering " << best << endl;        
  for (int i=0; i<N; i++) 
    cout << data[i][best] << endl;

  for (int i=0; i<N; i++) 
    delete[] data[i];
  delete[] data;
}
