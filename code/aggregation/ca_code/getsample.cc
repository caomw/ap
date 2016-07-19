#include "Global.h"

#define MAX_LINE_SIZE 5000


int main(int argc, char*argv[])
{

  int S = atoi(argv[1]);
  int N,K;
  cin >> N >> K ;

  cerr << N << " " << K <<endl;
  
  string line;
  getline(cin, line); // clears the endl at the first line

  string *LinesSampled = new string [S];

  srand48(1973);
  for (int i = 0; i < N; i ++){
    getline(cin, line);
    if (i < S){
      LinesSampled[i] = line;
      //cout << "line "<<i<<": "<<LinesSampled[i]<<endl;
    }else{
      int r = (int) floor(drand48() * (i+1));
      if (r < S){
	LinesSampled[r] = line;
	//cout << line<<endl;
	//cout << "line "<<r<<": "<<LinesSampled[r]<<endl;
      }
    }
    
  }

  //cout << "\nsample!\n"<<endl;
  cout << S << " " << K <<endl;
  for (int i = 0; i < S; i ++){
    cout << LinesSampled[i]<<endl;
  }
}
