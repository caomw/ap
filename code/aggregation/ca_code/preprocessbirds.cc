
#include <fstream>
#include <iostream>   // cout, cin, cerr
#include <string>     // strtok

using namespace std;

int main(int argc, char *argv[])
{
  int N = 3813;
  int B = 248;
  int x, y, maa, iyle, iran, ise;
  int rare[B];
  for (int i=0; i<B; i++)
    rare[i]=0;

  rare[14-1]=1;
  rare[167-1]=1;
  rare[198-1]=1;
  rare[205-1]=1;
  rare[46-1]=1;
  rare[77-1]=1;
  rare[147-1]=1;
  rare[177-1]=1;
  rare[209-1]=1;
  rare[75-1]=1;
  rare[153-1]=1;
  rare[231-1]=1;
  rare[12-1]=1;
  rare[16-1]=1;
  rare[64-1]=1;
  rare[112-1]=1;
  rare[163-1]=1;
  rare[154-1]=1;
  rare[91-1]=1;
  rare[226-1]=1;
  rare[41-1]=1;
  rare[83-1]=1;
  rare[95-1]=1;
  rare[3-1]=1;
  rare[207-1]=1;
  rare[164-1]=1;
  rare[37-1]=1;
  rare[133-1]=1;
  rare[60-1]=1;

  int data[N][B];
  int sum[N];
  for (int i=0; i<N; i++)
    sum[i]=0;
  
  for (int i=0; i<N; i++) {
    cin >> x >> y >> maa >> iyle >> iran >> ise;
    for (int j=0; j<B; j++) {
      cin >> data[i][j];
      if (data[i][j])
	sum[i]++;
    }
  }
  for (int j=0; j<B; j++) 
    if (!rare[j]) {
      for (int i=0; i<N; i++) 
	if (sum[i]>70 && sum[i]<120)
	  cout << data[i][j] << " ";
      cout << endl;
    }
}

/*
int main(int argc, char *argv[])
{
  int N = 3813;
  int B = 248;
  int x, y, maa, iyle, iran, ise, bird;

  ifstream  maskfile(argv[1]);
  int mask[B];
  for (int j=0; j<B; j++)
    maskfile >> mask[j];
  
  for (int i=0; i<N; i++) {
    cin >> x >> y >> maa >> iyle >> iran >> ise;
    //    cout << x << " " << y << endl;
    for (int j=0; j<B; j++) {
      cin >> bird;
      if (mask[j])
	cout << bird << " ";
    }
    cout << endl;
  }
}
*/
