/******************************************************************************Ccompute chi squared distance between vectors
If one argument is a matrix of size m x o (where m is the number of vectors and
o is the dimensionality of each vector) and the second one is of size n x o,
the output is a matrix of size m x n.

From MATLAB, compile this mex function with the following command: 
mex chi_squared_c.c -lm 
******************************************************************************/
#include <math.h>
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  double *x1, *x2, *y;   
  int row, i, j, k, m, n, dim, N;
  double d = 0, a,b,sum_ab,diff_ab;
  
  
  /* check number of input and output arguments */   
  if (nrhs != 2)   
    {      
      mexErrMsgTxt("Wrong number of input arguments.");   
    }  
  else if (nlhs > 1)   
    {     
      mexErrMsgTxt("Too many output arguments.");   
  
    }

  /* get input arguments */   
  if (!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0]))   
    {     
      mexErrMsgTxt("x1 must be a double matrix.");   
    }  
  
  m  = mxGetM(prhs[0]);   
  x1 = mxGetPr(prhs[0]);	 
  
  if (!mxIsDouble(prhs[1]) || mxIsComplex(prhs[1]))   
    {      
      mexErrMsgTxt("x2 must be a double matrix.");
    }
  
  n  = mxGetM(prhs[1]);
  dim  = mxGetN(prhs[1]);
  x2 = mxGetPr(prhs[1]);
  
  /* allocate and initialise output matrix */
  
  plhs[0] = mxCreateDoubleMatrix(m, n, mxREAL);
  
  y = mxGetPr(plhs[0]);
  
  /* compute kernel matrix */
  for ( i = 0; i < m; i++)
    {
      for ( j = 0; j < n; j++)
	{
	  /*By default the distance is zero*/
	  d = 0;
	  for ( k = 0; k < dim; k++ )
	    {
	      a = x1[i+k*m];
	      /*printf("a = %f\n",a);*/
	      b = x2[j+k*n];
	      /*printf("b = %f\n",b);*/
	      sum_ab = a + b;
	      diff_ab = a - b;
	      if( (sum_ab > 0) && (diff_ab != 0))
		{
		  d += (diff_ab*diff_ab)/sum_ab;
		}      
	    }
	  y[i+j*m] = d/2;
	}
      mexPrintf("Distance Chi^2 (%d/%d)\n",i+1,m);
    }  
}
