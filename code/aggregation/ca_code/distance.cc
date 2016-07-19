#include "distance.h"
#include "omp.h"
#define NUM_THREADS 4

float distance(string **data, int i, int j, int k)
{
  if (i==j)
    return 0.0;
  float disagree=0.0;
  int a;
  omp_set_num_threads(NUM_THREADS);
#pragma omp parallel private(a) 
    {    
#pragma omp for  
  for (a=0; a<k; a++)
    if (data[i][a]== MISSING || data[j][a]==MISSING)
      disagree += 0.5;
    else if (data[i][a]!=data[j][a])
      disagree += 1.0;
    }//end parallel section

  return disagree/(float)k;
}
