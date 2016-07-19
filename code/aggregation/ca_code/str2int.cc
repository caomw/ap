
/*
Replaces string/char class members with integers and missing values '?' with -1
*/

#include <iostream>   // cout, cin, cerr
#include <cfloat>     // FLT_MAX
#include <string>     // strtok
#include <iterator>
#include <set>
#include <map>

using namespace std;

typedef map<string, int> wordmap;

int main(int argc, char *argv[])
{
  wordmap hash;
  int clusterId=1;
  string clusterlabel;

  cin >> clusterlabel;
  while(!cin.eof()) {
    if (clusterlabel=="?")
      cout << -1 << endl;
    else {
      if (hash[clusterlabel]==0) 
	hash[clusterlabel] = clusterId++;
      cout << hash[clusterlabel] << endl;
    }
    cin >> clusterlabel;
  }
}
