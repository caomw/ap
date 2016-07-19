#ifndef _HDATA_H
#define _HDATA_H

#include "DynArr.h"

class HeapData {
 friend class CDynArrayMinHeap;

 public:
  // This is the distance
  double          m_key;
  // The row and col of the matrix 
  // this entry corresponds to
  int             m_row;
  int             m_col;
  // position in the eviction list
  TEvictionPos	  m_EvictionPos;
  
 public:
  HeapData() : m_key(0.0),
               m_row(0),
               m_col(0),
               m_EvictionPos(RECYCMAN_EVICTION_LIST_INVALID_POS) {}

  inline double GetKey(){
    return m_key;
  }

  inline int GetRow(){
    return m_row;
  }

  inline int GetCol(){
    return m_col;
  }

  inline int GetEvictionPos(){
    return m_EvictionPos;
  }

};

#endif
