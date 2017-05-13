/******************************************************************************
 *
 * File Name: heap.c
 *	      (c) 2010 AED
 * Author:    AED team
 * Revision:  v2.3 ACR
 *
 * NAME
 *     Heap11 - Heap operations
 *
 * SYNOPSIS
 *     #include <stdlib.h>
 *     #include <stdio.h>
 *     #include <malloc.h>
 *
 * DESCRIPTION
 *		implements tree auxiliary functions
 * DIAGNOSTICS tested
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "Heap.h"

/******************************************************************************
 * FixUp()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixup
 * 			  wt[] - weights vector
 * Returns:
 * Side-Effects: none
 *
 * Description: Performs fixup from index
 *
 *****************************************************************************/

void FixUp(Heap *h, int k, int wt[]){
  int t;
  
  while((k > 0) && (h->great) ((h->heapdata)[(k - 1) / 2], (h->heapdata)[k], wt)){ 
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[(k - 1) / 2];
    (h->heapdata)[(k - 1) / 2] = t;
    h->qp[(h->heapdata)[k]] = k;
    h->qp[(h->heapdata)[(k - 1) / 2]] = (k - 1) / 2;

    k = (k - 1) / 2;
  }

  return;
}


/******************************************************************************
 * FixDown()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixdown
 * 			  wt[] - weights vector
 * Returns:
 * Side-Effects: none
 *
 * Description: performs fixdown from index
 *
 *****************************************************************************/

void FixDown(Heap *h, int k, int wt[]){
  int j, t;

  while ((2 * k + 1) < h->n_elements) {
    j = 2 * k + 1;
    if (((j + 1) < h->n_elements) && (h->great) (h->heapdata[j], h->heapdata[j + 1], wt)){
      /* second offspring is lesser */
      j++;
    }
    if (!(h->great) (h->heapdata[k], h->heapdata[j], wt)){
      /* Elements are in correct order. */
      break;
    }

    /* the 2 elements are not correctly sorted, it is
       necessary to exchange them */
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[j];
    (h->heapdata)[j] = t;
    h->qp[(h->heapdata)[k]] = k;
    h->qp[(h->heapdata)[j]] = j;
    
    k = j;
	}
	
  return;
}


/******************************************************************************
 * NewHeap()
 *
 * Arguments: size - heap size
 *            great - Integer comparison function
 * Returns: Heap
 * Side-Effects: none
 *
 * Description: allocates space for a new heap of specified size
 *
 *****************************************************************************/

Heap *NewHeap(int size, int (*great) (int, int, int*)){
  Heap *h;

  h = (Heap *) malloc(sizeof(Heap));
  if (h == ((Heap *) NULL)) {
    fprintf(stderr, "Error in malloc of heap\n");
    exit(0);
  }

  h->n_elements = 0;
  h->great = great;
  h->size = size;
  h->heapdata = (int *) malloc(size * sizeof(int));
  h->qp = (int *) malloc(size * sizeof(int));
  if (h->heapdata == NULL || h->qp == NULL) {
    fprintf(stderr, "ERROR: Failed malloc of heap data\n");
    exit(0);
  }

  return (h);
}

/******************************************************************************
 * FreeHeap()
 *
 * Arguments: h - pointer to heap
 *    
 * Returns: Void
 * Side-Effects: none
 *
 * Description: Desalocates the memory of a heap
 *
 *****************************************************************************/

void FreeHeap(Heap *h){
	 
  free(h->heapdata);
  free(h->qp);
  free(h);

}

/******************************************************************************
 * Insert()
 *
 * Arguments: h - pointer to heap
 *            element - pointer to new element
 * Returns: vopid
 * Side-Effects: none
 *
 * Description: add element at the end of heap
 *
 *****************************************************************************/

int Insert(Heap *h, int element){
	
  if (h->n_elements == h->size){
    fprintf(stderr, "ERROR: Heap full (size = %d) !\n", h->size);
    exit(0);
  }
  
  h->heapdata[h->n_elements] = element;
  h->qp[h->n_elements] = h->n_elements;

  h->n_elements++;

  return 1;
}

/******************************************************************************
 * RemoveMin()
 *
 * Arguments: h - pointer to heap
 * Returns: Integer
 * Side-Effects: none
 *
 * Description: exchange first and last element of heap, remove last element
 *              (max element) from heap and maintain heap order by performing
 *              a fixdown of the first element
 *
 *****************************************************************************/

int RemoveMin(Heap *h, int wt[]){
	int t;
		
	if (h->n_elements > 0){
		t = (h->heapdata)[0];
		(h->heapdata)[0] = (h->heapdata)[h->n_elements - 1];
		(h->heapdata)[h->n_elements - 1] = t;
		h->n_elements--;
		FixDown(h, 0, wt);
		return t;
	}
	
	fprintf(stderr, "ERROR: Heap empty\n");
    exit( 0 );
}

/******************************************************************************
 * HeapEmpty()
 *
 * Arguments: h - pointer to heap
 * Returns: Integer
 * Side-Effects: none
 *
 * Description: Returns 1 if heap is empty, 0 otherwise
 *
 *****************************************************************************/

int HeapEmpty(Heap *h){
	if(h->n_elements == 0)
		return 1;
		
	return 0;
}	

/******************************************************************************
 * Heapify()
 *
 * Arguments: Heap
 * Returns: void
 * Side-Effects:
 *
 * Description: forces the argument to be an heap
 *
 *****************************************************************************/

void PQDec(Heap *h, int v, int wt[]){
	
	FixUp(h, h->qp[v], wt);	 
		
    return;
}

