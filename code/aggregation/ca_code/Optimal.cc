#include "Optimal.h"

float optimal(Matrix *dist)
{

  int size = dist->size;
  float optCost = 0;
  
  for (int i = 0; i < size; i ++){
    for (int j = i+1; j < size; j ++){
      if (dist->matrix[i][j] < 0.5){
	optCost += dist->matrix[i][j];
      }else{
	optCost += 1- dist->matrix[i][j];
      }
    }
  }
  return optCost;
}
