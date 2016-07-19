#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>   // cout, cin, cerr
#include <fstream>   
#include <cfloat>     // FLT_MAX
#include <cmath>      // fabs, log
#include <ctime>
#include <string>     // strtok
#include <iterator>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <list>

#define MISSING "?" 
#define VERY_BIG_INTEGER 999999999999

typedef enum {NO, YES} Boolean;
typedef enum {NOB, AGGLOMERATIVE, BALLS, GONZALEZ, RANDOMSPLITS} BaseAlgorithm;
typedef enum {NOL, LOCALSEARCH, D_LOCALSEARCH, ZEROITERATIONS} LocalSearchAlgorithm;

extern BaseAlgorithm BaseAlgo;
extern LocalSearchAlgorithm LocalSearchAlgo;

extern int LSmaxIters;
extern int RSmaxIters;
extern float Alpha;

using namespace std;

#endif 
