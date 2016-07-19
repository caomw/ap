#ifndef _MATRIX_H
#define _MATRIX_H

#include "Global.h"
#include <string>

using namespace std;

class Matrix
{
 public:
  float **matrix;
  int size;

  Matrix();
  Matrix(int);
  Matrix(const string filename);
  ~Matrix();
  //  void readMatrixFile(const char *);
};

#endif 
