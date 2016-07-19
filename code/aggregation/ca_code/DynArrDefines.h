#ifndef __DYNARDEFINES_H__
//#error resultcache.h included twice
//#endif
#define __DYNARDEFINES_H__

// ***************************************************************************
// function return codes definitions
// ***************************************************************************
const int RECYCMAN_SUCCESS          = 14100;
const int RECYCMAN_ERROR            = 24100;
const int RECYCMAN_ERROR_NEW_FAILED = 24500;

const int RECYCMAN_ERROR_EVICTION_LIST_INSERT_FAILED = 31100;
const int RECYCMAN_ERROR_EVICTION_LIST_INVALID_POS   = 32100;

// ***************************************************************************
// for the heap structure
// ***************************************************************************
const int RECYCMAN_MAX_ARRAY_CHUNK_INIT = 10;	        // initial max number of array chunks
const int RECYCMAN_ARRAY_CHUNK_INIT     = 5;
const int RECYCMAN_ARRAY_CHUNK_INDEX_GROWTH_FACTOR = 10;// once full it will grow by 10 entries
const int RECYCMAN_ARRAY_CHUNK_SIZE_LOG2_INIT = 10;	// that will be 2^10=1024 elements

const int RECYCMAN_EVICTION_LIST_INVALID_POS = -1;	// has to be negative (>=0 are valid pos)


// ***************************************************************************
// typedefs
// ***************************************************************************
typedef	int TReturnCode;
typedef int TEvictionPos;

// ***************************************************************************
// Macro definitions
// ***************************************************************************
#define RECYCMAN_FAILURE(L,D,R) { printf("\n%s%s\n",L,D); exit(1); }


// ***************************************************************************
// class declarations
// ***************************************************************************
class CDynArrayMinHeap;
class HeapData;

#endif	// __RESULTCACHEDEFINES_H__

