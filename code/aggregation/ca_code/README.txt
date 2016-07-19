The C++ code in this directory is written for the "Clustering
Aggregation" algorithms described in 

Aristides Gionis, Heikki Mannila, and Panayiotis Tsaparas, 
Clustering aggregation, 21st International Conference on Data 
Engineering (ICDE) 2005

The code is written for Linux-compatible environment

HOW TO USE THE CODE:	

1. untar the directory using command "tar xvfz ca.tar.gz"

2. compile the code using the command "make"

3. run the code using the command "aggregated" 
   (with no parameters to obtain the help screen with instruction)
   Then run the code with the appropriate parameters
   The help screen is the following:

---

Usage: aggregated <InFile> [O OutFile] [I ClusFile] [C ClassFile] [A options] [L options]

Input from file InFile.
  The input is considered to be a square matrix such that
  the [i,j] entry holds the distance between object i and
  object j. The first line holds the size of the matrix.

[O OutFile] Write to files OutFile.out and OutFile.log (defaults
tmp.out and tmp.log)

[I ClusFile] Read initial clustering for local search from file
ClusFile

[C ClassFile] Read class labels from file ClassFile

[A options] Select base algorithm
For options use:
  'g'  for Gonzalez' algorithm
  'a'  for agglomerative algorithm
  'b <alpha>' for Balls algorithm (default alpha = 0.17)
  'r <maxIter>' for RandomSplit algorithm (default maxIter = 100)

[L options] Select local search algorithm
  Local search uses as initial solution *either* a clustering found by
  a base algorithm (option A) *or* a clustering read from file (option I)
For options use:
  'd'  for deterministic local search
  'r <maxIter>' for randomized local search (default maxIter = 100)

---

June 12, 2006, 
Aristides Gionis, gionis@cs.helsinki.fi
Heikki Mannila, mannila@cs.helsinki.fi
Panayiotis Tsaparas, tsaparas@cs.helsinki.fi
