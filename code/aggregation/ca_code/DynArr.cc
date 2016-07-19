// Constructs a minHeap

#include "DynArr.h"

CDynArrayMinHeap::CDynArrayMinHeap() 
  : m_iMaxArrayChunkPos(RECYCMAN_MAX_ARRAY_CHUNK_INIT - 1),
    m_iInitArrayChunk(RECYCMAN_ARRAY_CHUNK_INIT),
    m_iLastArrayChunkPos(RECYCMAN_EVICTION_LIST_INVALID_POS),
    m_iArrayChunkSizeLog2(RECYCMAN_ARRAY_CHUNK_SIZE_LOG2_INIT),
    m_iArrayChunkSize(1 << m_iArrayChunkSizeLog2),
    m_iModArrayChunkSize(m_iArrayChunkSize - 1),
    m_iDivArrayChunkSize(m_iArrayChunkSizeLog2),
    m_iLastHeapNodePos(-1),
    m_iMaxHeapNodePos(RECYCMAN_EVICTION_LIST_INVALID_POS),
    m_pMinHeap(NULL)
{

  if ((m_pMinHeap = (HeapData ***) new HeapData** [m_iMaxArrayChunkPos+1]) == NULL)
    {
      RECYCMAN_FAILURE("CDynArrayMinHeap::CDynArrayMinHeap",
		       "new m_pMinHeap failed",
		       RECYCMAN_ERROR_NEW_FAILED);
    }
  else
    {
      for (int iIndex = 0; iIndex < m_iInitArrayChunk; iIndex++)
	{
	  if ((m_pMinHeap[iIndex] = (HeapData **) new HeapData** [m_iArrayChunkSize]) == NULL)
	    {
	      RECYCMAN_FAILURE("CDynArrayMinHeap::CDynArrayMinHeap",
			       "new m_pMinHeap[iIndex] failed",
			       RECYCMAN_ERROR_NEW_FAILED);
	      break;
	    }
	  m_iLastArrayChunkPos++;
	}
      m_iMaxHeapNodePos = (m_iArrayChunkSize * (m_iLastArrayChunkPos + 1)) - 1;
    }

  assert(m_iLastArrayChunkPos <= m_iMaxArrayChunkPos);
  assert(m_iLastHeapNodePos < m_iMaxHeapNodePos);
}


CDynArrayMinHeap::CDynArrayMinHeap(int size) 
  : m_iLastArrayChunkPos(RECYCMAN_EVICTION_LIST_INVALID_POS),
    m_iArrayChunkSizeLog2(RECYCMAN_ARRAY_CHUNK_SIZE_LOG2_INIT),
    m_iArrayChunkSize(1 << m_iArrayChunkSizeLog2),
    m_iModArrayChunkSize(m_iArrayChunkSize - 1),
    m_iDivArrayChunkSize(m_iArrayChunkSizeLog2),
    m_iLastHeapNodePos(-1),
    m_iMaxHeapNodePos(RECYCMAN_EVICTION_LIST_INVALID_POS),
    m_pMinHeap(NULL)
{
  m_iInitArrayChunk = size >> m_iArrayChunkSizeLog2;
  m_iMaxArrayChunkPos = m_iInitArrayChunk - 1;
  if ((m_pMinHeap = (HeapData ***) new HeapData** [m_iMaxArrayChunkPos+1]) == NULL)
    {
      RECYCMAN_FAILURE("CDynArrayMinHeap::CDynArrayMinHeap",
		       "new m_pMinHeap failed",
		       RECYCMAN_ERROR_NEW_FAILED);
    }
  else
    {
      for (int iIndex = 0; iIndex < m_iInitArrayChunk; iIndex++)
	{
	  if ((m_pMinHeap[iIndex] = (HeapData **) new HeapData** [m_iArrayChunkSize]) == NULL)
	    {
	      RECYCMAN_FAILURE("CDynArrayMinHeap::CDynArrayMinHeap",
			       "new m_pMinHeap[iIndex] failed",
			       RECYCMAN_ERROR_NEW_FAILED);
	      break;
	    }
	  m_iLastArrayChunkPos++;
	}
      m_iMaxHeapNodePos = (m_iArrayChunkSize * (m_iLastArrayChunkPos + 1)) - 1;
    }

  //assert(m_iLastArrayChunkPos <= m_iMaxArrayChunkPos);
  //assert(m_iLastHeapNodePos < m_iMaxHeapNodePos);
}


CDynArrayMinHeap::~CDynArrayMinHeap() 
{
  for (int iIndex = 0; iIndex < m_iLastArrayChunkPos; iIndex++)
    delete [] m_pMinHeap[iIndex];
  delete [] m_pMinHeap;
}


TReturnCode CDynArrayMinHeap::Expand()
{
  // the current index of array chunks is not full, so we can simply add
  // one more array chunk
  if (m_iLastArrayChunkPos < m_iMaxArrayChunkPos)
    {
      if ((m_pMinHeap[m_iLastArrayChunkPos + 1] = (HeapData **) new HeapData** [m_iArrayChunkSize]) == NULL)
	{
	  RECYCMAN_FAILURE("CDynArrayMinHeap::Expand",
			   "new m_pMinHeap[m_iLastArrayChunkPos + 1] failed",
			   RECYCMAN_ERROR_NEW_FAILED);
	  return RECYCMAN_ERROR_NEW_FAILED;
	}
      m_iLastArrayChunkPos++;
      m_iMaxHeapNodePos += m_iArrayChunkSize;

      assert(m_iLastArrayChunkPos <= m_iMaxArrayChunkPos);
      assert(m_iLastHeapNodePos < m_iMaxHeapNodePos);
      
      return RECYCMAN_SUCCESS;
    }

  // the current index of array chunks is full (all the array chunks have 
  // been allocated), so we have to expand it
#ifdef DEBUG
  assert(m_iLastArrayChunkPos == m_iMaxArrayChunkPos);
#endif	//	DEBUG

  HeapData ***ppTemp;
  int	iIndex;
  int	iNewMaxArrayChunkPos = m_iMaxArrayChunkPos + RECYCMAN_ARRAY_CHUNK_INDEX_GROWTH_FACTOR;
  int	iNewLastArrayChunkPos = m_iMaxArrayChunkPos + RECYCMAN_ARRAY_CHUNK_INIT;

  if ((ppTemp = (HeapData ***) new HeapData** [iNewMaxArrayChunkPos+1]) == NULL)
    {
      RECYCMAN_FAILURE("CDynArrayMinHeap::Expand",
		       "new ppTemp failed",
		       RECYCMAN_ERROR_NEW_FAILED);
      return RECYCMAN_ERROR_NEW_FAILED;
    }

  for (iIndex = 0; iIndex <= m_iMaxArrayChunkPos; iIndex++)
    ppTemp[iIndex] = m_pMinHeap[iIndex];

  for (iIndex = m_iMaxArrayChunkPos + 1; iIndex <= iNewLastArrayChunkPos; iIndex++)
    {
      if ((ppTemp[iIndex] = (HeapData **) new HeapData** [m_iArrayChunkSize]) == NULL)
	{
	  RECYCMAN_FAILURE("CDynArrayMinHeap::Expand",
			   "new ppTemp[iIndex] failed",
			   RECYCMAN_ERROR_NEW_FAILED);
	  for (int iTempIndex = 0; iTempIndex < iIndex; iTempIndex++)
	    delete [] ppTemp[iTempIndex];
	  delete [] ppTemp;
	  return RECYCMAN_ERROR_NEW_FAILED;
	}
    }

  delete [] m_pMinHeap;

  m_iLastArrayChunkPos = iNewLastArrayChunkPos;
  m_iMaxArrayChunkPos = iNewMaxArrayChunkPos;
  m_iMaxHeapNodePos = (m_iArrayChunkSize * (m_iLastArrayChunkPos + 1)) - 1;
  m_pMinHeap = ppTemp;

  assert(m_iLastArrayChunkPos <= m_iMaxArrayChunkPos);
  assert(m_iLastHeapNodePos < m_iMaxHeapNodePos);

  return RECYCMAN_SUCCESS;
}


// will ONLY try to move element down
TReturnCode CDynArrayMinHeap::HeapifyDown
(
 TEvictionPos Pos		//	IN
 )
{
  HeapData	*pEl;
  HeapData	*pLeftEl;
  HeapData	*pRightEl;
  TEvictionPos			LeftPos;
  TEvictionPos			RightPos;

  pLeftEl = GetLeftChild(Pos, LeftPos);
  pRightEl = GetRightChild(Pos, RightPos);
	
  while (pLeftEl != NULL || pRightEl != NULL)
    {
      pEl = GetElement(Pos);
      if (pRightEl && 
	  pRightEl->GetKey() <= pLeftEl->GetKey() &&
	  pEl->GetKey() > pRightEl->GetKey())
	{
	  SwapElements(Pos, RightPos);
	  Pos = RightPos;
	}
      else if (pEl->GetKey() > pLeftEl->GetKey())
	{
	  SwapElements(Pos, LeftPos);
	  Pos = LeftPos;
	}
      else
	return RECYCMAN_SUCCESS;

      pLeftEl = GetLeftChild(Pos, LeftPos);
      pRightEl = GetRightChild(Pos, RightPos);
    }

  return RECYCMAN_SUCCESS;
}


// will ONLY try to move element up
TReturnCode CDynArrayMinHeap::HeapifyUp
(
 TEvictionPos Pos		//	IN
 )
{
  HeapData	*pEl;
  HeapData	*pParentEl;
  TEvictionPos			ParentPos;

  while (Pos > 0)		// position of the heap root
    {
      pEl = GetElement(Pos);
      pParentEl = GetParent(Pos, ParentPos);
#ifdef DEBUG
      assert(ParentPos >= 0 && ParentPos < Pos);
#endif	//	DEBUG
      if (pEl->GetKey() < pParentEl->GetKey())
	{
	  SwapElements(Pos, ParentPos);
	  Pos = ParentPos;
	}
      else
	return RECYCMAN_SUCCESS;
    }

  return RECYCMAN_SUCCESS;
}



TReturnCode CDynArrayMinHeap::Heapify
(
 TEvictionPos Pos		//	IN
 )
{
  if (Pos > 0 && GetElement(Pos)->GetKey() <= GetParentEl(Pos)->GetKey())
    return HeapifyUp(Pos);
  else if (Pos < m_iLastHeapNodePos)
    return HeapifyDown(Pos);
  else
    return RECYCMAN_SUCCESS;
}


TReturnCode CDynArrayMinHeap::BuildHeap
( 
)
{
  TEvictionPos StartPos = GetParentPos(m_iLastHeapNodePos);

  for (int Pos = StartPos; Pos >=0; Pos --){
		HeapifyDown(Pos);
  }
  return RECYCMAN_SUCCESS;
}


double CDynArrayMinHeap::RootKey
( 
)
{
  return GetElement(0)->GetKey();
}


TReturnCode CDynArrayMinHeap::Insert
(
 HeapData *pElement		//	IN
 )
{
  if (m_iLastHeapNodePos == m_iMaxHeapNodePos)
    {
      if (Expand() != RECYCMAN_SUCCESS)
	{
	  RECYCMAN_FAILURE("CDynArrayMinHeap::Insert",
			   "Expand() failed upon insert; insert not completed",
			   RECYCMAN_ERROR_EVICTION_LIST_INSERT_FAILED);
	  return RECYCMAN_ERROR_EVICTION_LIST_INSERT_FAILED;
	}
    }

  m_iLastHeapNodePos++;

  SetElement(m_iLastHeapNodePos, pElement);
  HeapifyUp(m_iLastHeapNodePos);

  return RECYCMAN_SUCCESS;
}


TReturnCode CDynArrayMinHeap::AddElement
(
 HeapData *pElement		//	IN
 )
{
  if (m_iLastHeapNodePos == m_iMaxHeapNodePos)
    {
      if (Expand() != RECYCMAN_SUCCESS)
	{
	  RECYCMAN_FAILURE("CDynArrayMinHeap::Insert",
			   "Expand() failed upon insert; insert not completed",
			   RECYCMAN_ERROR_EVICTION_LIST_INSERT_FAILED);
	  return RECYCMAN_ERROR_EVICTION_LIST_INSERT_FAILED;
	}
    }

  m_iLastHeapNodePos++;

  SetElement(m_iLastHeapNodePos, pElement);

  return RECYCMAN_SUCCESS;
}


HeapData* CDynArrayMinHeap::Remove
(
 TEvictionPos Pos		//	IN
 )
{
  if (Pos < 0 || Pos > m_iLastHeapNodePos)
    {
      RECYCMAN_FAILURE("CDynArrayMinHeap::Remove",
		       "invalid pos",
		       RECYCMAN_ERROR_EVICTION_LIST_INVALID_POS);
      return NULL;
    }

  HeapData *pEl = GetElement(Pos);
  pEl->m_EvictionPos = RECYCMAN_EVICTION_LIST_INVALID_POS;

  if (Pos == m_iLastHeapNodePos)
    {
      m_iLastHeapNodePos--;
    }
  else
    {
      SwapElements(Pos, m_iLastHeapNodePos);
      m_iLastHeapNodePos--;
      Heapify(Pos);
    }

  // ==============================================================
  //
  // may want to call Shrink() here (reduce the size of m_pMinHeap)
  // Shrink() is not implemented yet
  //
  // ==============================================================
  //if (unused space is too large)
  //	Shrink();

  return pEl;
}


void CDynArrayMinHeap::Print()
{
  cout << endl << "*  ";
  for (int iIndex = 0; iIndex <= m_iLastHeapNodePos; iIndex++){
    cout << GetElement(iIndex)->GetKey() << "  ";
    cout << GetElement(iIndex)->GetRow() << "  ";
    cout << GetElement(iIndex)->GetCol() << "  [";
    cout << GetElement(iIndex)->GetEvictionPos() << "] |";
  }
  cout << "*" << endl;
}


