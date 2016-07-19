#include "Global.h"
#include "Matrix.h"
#include "Agglomerative.h"
#include "LocalSearch.h"
#include "Balls.h"
#include "Sampling.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/************
typedef enum {NO, YES} Boolean;
typedef enum {NOB, AGGLOMERATIVE, BALLS, GONZALEZ, RANDOMSPLITS} BaseAlgorithm;
typedef enum {NOL, LOCALSEARCH, D_LOCALSEARCH, ZEROITERATIONS} LocalSearchAlgorithm;

**********/

BaseAlgorithm BaseAlgo = NOB;
LocalSearchAlgorithm LocalSearchAlgo = NOL;

int LSmaxIters = 100;
int RSmaxIters = 100;
float Alpha = 0.17;


string InputFile;
string Output = "tmp";
string ClusteringFile = "";
string ClassFile = "";

Boolean SamplingFlag = NO;
int SampleSize = 1000;

void help_screen(char *name)
{
  cerr << endl;
  cerr << "Usage: " << name << " <InFile> [O OutFile] [I ClusFile] [C ClassFile] [A options] [L options]" << endl;
  cerr << endl;
  cerr << "Input from file InFile." << endl;
  cerr << "  The input is considered to be a square matrix such that" << endl;
  cerr << "  the [i,j] entry holds the distance between object i and" << endl;
  cerr << "  object j. The first line holds the size of the matrix." << endl;
  cerr << endl;
  cerr << "[O OutFile] Write to files OutFile.out and OutFile.log (defaults " << Output << ".out and " << Output << ".log)" << endl;
  cerr << endl;
  cerr << "[I ClusFile] Read initial clustering for local search from file ClusFile" << endl;
  cerr << endl;
  cerr << "[C ClassFile] Read class labels from file ClassFile" << endl;
  cerr << endl;
  cerr << "[A options] Select base algorithm" << endl;
  cerr << "For options use:" << endl;
  cerr << "  'g'  for Gonzalez' algorithm" << endl;
  cerr << "  'a'  for agglomerative algorithm" << endl;
  cerr << "  'b <alpha>' for Balls algorithm (default alpha = " << Alpha << ")" << endl;
  cerr << "  'r <maxIter>' for RandomSplit algorithm (default maxIter = " << RSmaxIters << ")" << endl;
  cerr << endl;
  cerr << "[L options] Select local search algorithm" << endl;
  cerr << "  Local search uses as initial solution *either* a clustering found by" << endl;
  cerr << "  a base algorithm (option A) *or* a clustering read from file (option I)" << endl;
  cerr << "For options use:" << endl;
  cerr << "  'd'  for deterministic local search" << endl;
  cerr << "  'r <maxIter>' for randomized local search (default maxIter = " << LSmaxIters << ")" << endl;
  cerr << endl;
}

void process_command_line(int argc, char *argv[])
{
  for (int i=0; i<argc; i++)
    cout << argv[i] << " ";
  cout << endl;
  if (argc < 3){
    help_screen(argv[0]);
    exit(1);
  }  
  InputFile = argv[1];
  for (int i=2; i<argc; i++) {
    switch (argv[i][0]){
    case 'A': 
      if (i<argc-1) {
	i++;
	switch (argv[i][0]){
	case 'a':
	  BaseAlgo = AGGLOMERATIVE;
	  break;
	case 'g':
	  BaseAlgo = GONZALEZ;
	  break;
	case 'b':
	  BaseAlgo = BALLS;
	  if (i<argc-1) {
	    float a = atof(argv[i+1]);
	    if (a!=0.0) {
	      Alpha = a;
	      i++;
	    }
	  }
	  break;
	case 'r':
	  BaseAlgo = RANDOMSPLITS;
	  if (i<argc-1) {
	    int tmp = atoi(argv[i+1]);
	    if (tmp!=0) {
	      RSmaxIters = tmp;
	      i++;
	    }
	  }
	  break;
	default:
	  error("Wrong arguments. Use option h for help.");
	}
      } else error("Wrong arguments. Use option h for help.");
      break;
    case 'L': 
      if (i<argc-1) {
	i++;
	switch (argv[i][0]){
	case 'd':
	  LocalSearchAlgo = D_LOCALSEARCH;
	  break;
	case 'n':
	  LocalSearchAlgo = ZEROITERATIONS;
	  break;
	case 'r':
	  LocalSearchAlgo = LOCALSEARCH;
	  if (i<argc-1) {
	    int tmp = atoi(argv[i+1]);
	    if (tmp!=0) {
	      LSmaxIters = tmp;
	      i++;
	    }
	  }
	  break;
	default:
	  error("Wrong arguments. Use option h for help.");
	}
      } else error("Wrong arguments. Use option h for help.");
      break;
    case 'S': 
      SamplingFlag = YES;
      if (i<argc-1) {
	int tmp = atoi(argv[i+1]);
	if (tmp!=0) {
	  SampleSize = tmp;
	  i++;
	}
      }
      break;
    case 'O':
      if (i<argc-1) 
	Output = argv[++i];
      else error("Wrong arguments. Use option h for help.");
      break;
    case 'I':
      if (i<argc-1) 
	ClusteringFile = argv[++i];
      else error("Wrong arguments. Use option h for help.");
      break;
    case 'C':
      if (i<argc-1) 
	ClassFile = argv[++i];
      else error("Wrong arguments. Use option h for help.");
      break;
    case 'h':
      help_screen(argv[0]);
      exit(1);
    default:
      error("Wrong arguments. Use option h for help.");
    }
  }
  // Make some checks to ensure that parameters have been set correctly! 
  if (BaseAlgo==NOB && ClusteringFile=="")
    error("Either a base algorithm (option A) or an initial clustering file (option I) needs to be specified.");
  if (BaseAlgo!=NOB && ClusteringFile!="")
    error("Either a base algorithm (option A) or an initial clustering file (option I) needs to be specified, but not both");
}

void readclustering(string filename, int N, int *clustering) 
{
  ifstream input(filename.c_str());
  if (!input.is_open())
    error("Can not open input file!");
  for (int i=0; i<N; i ++)
    input >> clustering[i];
  //  cerr << "Clustering read\n";
  input.close();
}

int main(int argc, char *argv[])
{
  process_command_line(argc, argv);
  
  string OutputFile = Output + ".out";
  string LogFile = Output + ".log";

  // N is the total number of nodes, and 
  // SampleSize is the number of nodes used in the distance matrix
  // (the two are different if sampling is used, o/w they are the same)
  int N;

  Matrix *distMatrix;
  Sampling *sample;
  if (SamplingFlag==YES) {
    cout << "Sampling ...";
    sample = new Sampling(InputFile, SampleSize);
    cout << "[Done!]" << endl;
    N = sample->N;
    SampleSize = sample->S; // this is just to ensure that S<=N
    distMatrix = new Matrix(SampleSize);
    cout << "Distance matrix ...";
    sample->computeMatrix(distMatrix);
    cout << "[Done!]" << endl;
  } else {
    distMatrix = new Matrix(InputFile);
    N = SampleSize = distMatrix->size;
  }
  
  int classlabels[N];  
  if (ClassFile!="")
    readclustering(ClassFile, N, classlabels);
  
  cout << "Clustering Aggregation ...";
  int samplelabels[SampleSize];
  if (BaseAlgo==NOB)
    readclustering(ClusteringFile, SampleSize, samplelabels);
  else if (BaseAlgo==BALLS)
    Balls(distMatrix, samplelabels, Alpha);
  else if (BaseAlgo==RANDOMSPLITS)
    RandomSplits(distMatrix, samplelabels, RSmaxIters);
  else if (BaseAlgo==GONZALEZ)
    Gonzalez(distMatrix, samplelabels);
  else if (BaseAlgo==AGGLOMERATIVE) {
    Agglomerative agglo(distMatrix);  
    agglo.runAlgorithm(samplelabels);
  }
  
  if (LocalSearchAlgo==LOCALSEARCH)
    LocalSearch(distMatrix, samplelabels, LSmaxIters);
  else if (LocalSearchAlgo==D_LOCALSEARCH)
    BestLocalSearch(distMatrix, samplelabels);

  relabelclusters(SampleSize, samplelabels);
  cout << "[Done!]" << endl;
  

  int labels[N];
  if (SamplingFlag==YES)
  {
    cout << "Post Processing" << endl;
    cout << "---------------" << endl;
    sample->postProcess(labels, N, samplelabels, SampleSize);
  }
  else {
    if (SampleSize!=N)
      error("Something is deeply screwed");
    copy(labels, samplelabels, N);
  }

  printClustering(labels, N, OutputFile);
  printLog(distMatrix, samplelabels, SampleSize, labels, classlabels, N, ClassFile, LogFile);

  delete distMatrix;
  if (SamplingFlag==YES)
    delete sample;
}
