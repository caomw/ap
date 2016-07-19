#include <math.h>
#include <matrix.h>
#include <mex.h>

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{

/*mexPrintf("Computing XUV\n");*/
/*inputs*/
double *npoints;
npoints = mxGetPr(prhs[0]); 
double *C;
C = mxGetPr(prhs[1]);
const mwSize *dims;
dims = mxGetDimensions(prhs[1]);
int nc = (int)dims[1]; /*number of clusterings*/
int nv = (int)dims[0]; /*number of vectors*/

double total_npoints = npoints[0]*(npoints[0]-1)/2;
/*mexPrintf("XUV size %g \n",total_npoints);*/

/*outputs*/
plhs[0] = mxCreateDoubleMatrix(total_npoints,1,mxREAL);
double *XUV;
XUV = mxGetPr(plhs[0]);
  
int xx, yy, tnp, cc;
/*convert to int*/
tnp = total_npoints;

int indexi, indexj;
int r = 0;
for(xx=0; xx<npoints[0]-1; xx++)
{
  for(yy=xx+1; yy<npoints[0]; yy++)
  {
     for(cc=0; cc<nc; cc++)
     {
	indexi = xx + cc*nv;
        indexj = yy + cc*nv;   	
	if(C[indexi] != C[indexj])
           XUV[r]++;
     }  
     XUV[r] = XUV[r]/(double)nc;
     r++;
  }  
}


}


