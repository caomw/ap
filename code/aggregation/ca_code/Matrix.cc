
#include "Utils.h"
#include "Matrix.h"
#include <iostream>
#include <fstream>

Matrix::Matrix(){
  size = 0;
}

Matrix::Matrix(int N)
{
  size = N;
  matrix = new float* [size];
  for (int i = 0; i < size; i ++){
    matrix[i] = new float[size];
  }
}  

Matrix::Matrix(const string filename)
{
  ifstream input(filename.c_str());
  if (!input.is_open())
    error("Cannot open Matrix input file!");
  input >> size;
  //  cout << "Matrix size: " << size << endl;
  matrix = new float* [size];
  for (int i = 0; i < size; i ++){
    matrix[i] = new float[size];
    for (int j = 0; j < size; j ++){
      input >> matrix[i][j];
      //cout << matrix[i][j]<< " ";
    }
    //cout << endl;
  }
  //  cout << "Matrix read" << endl;
  input.close();
}

Matrix::~Matrix(){
  for (int i = 0; i < size; i ++){
    delete [] matrix[i];
  }
  delete [] matrix;
  
}


