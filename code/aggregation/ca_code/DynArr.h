#ifndef __DYNAR_H__
//#error resultcache.h included twice
//#endif
#define __DYNAR_H__

#include <assert.h>
#include "DynArrDefines.h"
#include "HeapData.h"
#include "Global.h"

class CDynArrayMinHeap{
 private:
  int		m_iMaxArrayChunkPos;	// size of index to array chunks
  int		m_iInitArrayChunk;	// initial number of array chunks
  int		m_iLastArrayChunkPos;	// index of last array chunk
  int		m_iArrayChunkSizeLog2;	// log base 2 of the real size
  int		m_iArrayChunkSize;	// real size of each array chunk
  int		m_iModArrayChunkSize;
  int		m_iDivArrayChunkSize;
  int		m_iLastHeapNodePos;	// index of last heap node
  int		m_iMaxHeapNodePos;	// max available index for heap
  HeapData      ***m_pMinHeap;	        // the actual thing; NOT the heap root

  // Pos1 and Pos2 MUST be valid
  inline void SwapElements
    (
     TEvictionPos Pos1,		//	IN
     TEvictionPos Pos2		//	IN
     )
    {
      HeapData *pTemp1 = (m_pMinHeap[Pos1 >> m_iDivArrayChunkSize])[Pos1 & m_iModArrayChunkSize];
      HeapData *pTemp2 = (m_pMinHeap[Pos2 >> m_iDivArrayChunkSize])[Pos2 & m_iModArrayChunkSize];

#ifdef DEBUG
      assert(Pos1 >= 0 && Pos1 <= m_iLastHeapNodePos);
      assert(Pos2 >= 0 && Pos2 <= m_iLastHeapNodePos);
#endif	//	DEBUG

      (m_pMinHeap[Pos1 >> m_iDivArrayChunkSize])[Pos1 & m_iModArrayChunkSize] = pTemp2;
      pTemp2->m_EvictionPos = Pos1;
      (m_pMinHeap[Pos2 >> m_iDivArrayChunkSize])[Pos2 & m_iModArrayChunkSize] = pTemp1;
      pTemp1->m_EvictionPos = Pos2;
    }

  // Pos MUST be valid
  inline void SetElement
    (
     TEvictionPos Pos,		//	IN
     HeapData *pElement	//	IN
     )
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      (m_pMinHeap[Pos >> m_iDivArrayChunkSize])[Pos & m_iModArrayChunkSize] = pElement;
      pElement->m_EvictionPos = Pos;
    }

  // returns the element at position Pos
  // if Pos is out of the bounds of the heap, it returns NULL
  inline HeapData* GetElement
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0);
#endif	//	DEBUG

      if (Pos <= m_iLastHeapNodePos)
	return ((m_pMinHeap[Pos >> m_iDivArrayChunkSize])[Pos & m_iModArrayChunkSize]);
      return NULL;
    }

  // returns the pos of the root element (the one with min value)
  inline TEvictionPos GetRootPos() const
    {
      return 0;
    }

  // returns the root node
  inline HeapData* GetRootEl() const
    {
      return GetElement(0);
    }

  // returns the root element and its position
  inline HeapData* GetRoot
    (
     TEvictionPos &ParentPos		//	OUT
     ) const
    {
      ParentPos = 0;
      return GetElement(ParentPos);
    }

  // returns the pos of: the parent of the node at position Pos
  // Pos MUST be valid
  inline TEvictionPos GetParentPos
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return ((Pos - 1) >> 1);
    }

  // returns the parent of the node at position Pos
  // Pos MUST be valid
  inline HeapData* GetParentEl
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return GetElement((Pos - 1) >> 1);
    }

  // returns the parent of the node at position Pos, and its position
  // Pos MUST be valid
  inline HeapData* GetParent
    (
     TEvictionPos Pos,			//	IN
     TEvictionPos &ParentPos		//	OUT
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      ParentPos = (Pos - 1) >> 1;
      return GetElement(ParentPos);
    }

  // returns the pos of: the left child of the node at position Pos
  // Pos MUST be valid
  inline TEvictionPos GetLeftChildPos
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return ((Pos << 1) + 1);
    }

  // returns the left child of the node at position Pos
  // Pos MUST be valid
  inline HeapData* GetLeftChildEl
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return GetElement((Pos << 1) + 1);
    }

  // returns the left child of the node at position Pos, and its position
  // Pos MUST be valid
  inline HeapData* GetLeftChild
    (
     TEvictionPos Pos,			//	IN
     TEvictionPos &LeftPos		//	OUT
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      LeftPos = (Pos << 1) + 1;
      return GetElement(LeftPos);
    }

  // returns the pos of: the right child of the node at position Pos
  // Pos MUST be valid
  inline TEvictionPos GetRightChildPos
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return ((Pos << 1) + 2);
    }

  // returns the right child of the node at position Pos
  // Pos MUST be valid
  inline HeapData* GetRightChildEl
    (
     TEvictionPos Pos		//	IN
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      return GetElement((Pos << 1) + 2);
    }

  // returns the right child of the node at position Pos, and its position
  // Pos MUST be valid
  inline HeapData* GetRightChild
    (
     TEvictionPos Pos,			//	IN
     TEvictionPos &RightPos		//	OUT
     ) const
    {
#ifdef DEBUG
      Assert(Pos >= 0 && Pos <= m_iLastHeapNodePos);
#endif	//	DEBUG

      RightPos = (Pos << 1) + 2;
      return GetElement(RightPos);
    }

  // allocates more space for the heap
  TReturnCode Expand();

 public:
  CDynArrayMinHeap();
  CDynArrayMinHeap(int);
  ~CDynArrayMinHeap();

  // move the node at position Pos down into its correct position in the heap
  TReturnCode HeapifyDown(TEvictionPos Pos);

  // move the node at position Pos up into its correct position in the heap
  TReturnCode HeapifyUp(TEvictionPos Pos);

  // move the node at position Pos into its correct position in the heap
  TReturnCode Heapify(TEvictionPos Pos);

  // inserts a node in the heap
  TReturnCode Insert(HeapData *pElement);

  // inserts an element at the last position of the array
  // CAUTION! This may destroy the Heap property
  TReturnCode AddElement(HeapData *pElement);

  // Builds a heap out of the Array 
  // At the end the array is a heap
  TReturnCode BuildHeap();

  // Returns the key stored at the root of the heap
  double RootKey();

  // removes the heap node at position Pos (ONLY from the heap)
  // by default removes the root node
  // returns a pointer to the removed element
  HeapData* Remove(TEvictionPos Pos = 0);

  // prints the array representation of the heap
  // the values printed are the m_dBenefit member variables of the 
  // CCachedResultStats objects
  void Print();
};


#endif
