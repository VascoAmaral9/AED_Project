/******************************************************************************
 *
 * File Name: queue.h
 *	      (c) 2010 AED
 * Author:    AED team
 * Revision:  v2.3 ACR
 * NAME
 *     queue.h - prototypes
 *
 * SYNOPSIS
 *     #include <stdlib.h>
 *     #include <stdio.h>
 *     #include <string.h>
 *
 * DESCRIPTION
 *		Definitions useful to implement breath scan
 * DIAGNOSTICS
 *
 *****************************************************************************/
 
/* Prevent multiple inclusions                                      */  
#ifndef QueueHeader
#define QueueHeader

#include "Graph.h"

#define MAX_TIME 1000000

typedef struct QueueStruct {
  LinkedList *first;
  LinkedList *last;
} Queue;

Queue *QueueNew();                    												/* creates empty list */
LinkedList *GetFirst(Queue *Q);         							 				/* get first element of the queue */
void InsertLastCar(Queue *Q, Car *C);								 				/* insertion at the end of queue */
void InsertLastRestriction(Queue *Q, int ta, int tb, int ID, int px, Graph *Park);	/* insertion at the end of queue */
void QueueCarfree(Queue *Q);
void Queuefree(Queue *Q);

/* End of: Protect multiple inclusions                              */
#endif
