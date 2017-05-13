
/******************************************************************************
 *
 * File Name: heap.h
 *	      (c) 2010 AED
 * Author:    AED team
 * Revision:  v2.3 ACR
 * NAME
 *     heap.h - prototypes
 *
 * SYNOPSIS
 *     #include <stdlib.h>
 *     #include <stdio.h>
 *     #include <string.h>
 *
 * DESCRIPTION
 *
 * DIAGNOSTICS
 *
 *****************************************************************************/

/* A heap is represented by a structure: */
typedef struct _heap {
  int (*great) (int, int, int*);/* Surprise! this field is a function pointer
								 * to elements in the heap. */
  int n_elements;               /* # elements in heap */
  int size;                     /* max size of the heap. */
  int *heapdata;				/* An array of elements. */
  int *qp;
} Heap;

void FixUp(Heap *h, int k, int wt[]);
void FixDown(Heap *h, int k, int wt[]);
Heap *NewHeap(int size, int (*great) (int, int, int*));
void FreeHeap(Heap *h);
int Insert(Heap *h, int element);
int RemoveMin(Heap *h, int wt[]);
int HeapEmpty(Heap *h);
void PQDec(Heap *h, int v, int wt[]);


