/******************************************************************************
 *  File name: Queue.c
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: C-Implementation of all the functions needed to a queue, where first in is the first out.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "Parking.h"

Queue *QueueNew(){
    Queue *Q;

    Q = (Queue*) malloc(sizeof(Queue));
    Q->first = NULL;
    Q->last = NULL;

    return Q;
}

/*GetFirst()*/
LinkedList *GetFirst(Queue *Q){
   LinkedList *LL;

    if (Q->first == NULL)
        return NULL;

	if(Q->first == Q->last){
		LL = Q->first;
		Q->first = NULL;
		Q->last = NULL;
	}
	else{	
		LL = Q->first;
		Q->first = getNextNodeLinkedList(LL);
	}

    return LL;
}

/*InsertLastCar()*/
void InsertLastCar(Queue *Q, Car *C){
    LinkedList *LL;

    if (Q->first == NULL)
		  Q->first = Q->last = insertUnsortedLinkedList(NULL, C);

	else{
		LL = Q->last;
		Q->last = insertUnsortedLinkedList(NULL, C);
		LL->next = Q->last;
	}	

    return;
}

/*InsertLastRestriction()*/
void InsertLastRestriction(Queue *Q, int ta, int tb, int ID, int px, Graph *Park){
    Restriction *R;
    LinkedList *LL;

    R = (Restriction *) malloc(sizeof(Restriction));
    R->ta = ta;
    R->ID = ID;
    R->px = px;
    if(tb == 0)
		R->tb = MAX_TIME;
	else
		R->tb = tb;
	if(ID != -1)
		R->type = Park->type[ID];
	else
		R->type = '@';

    if (Q->first == NULL)
		  Q->first = Q->last = insertUnsortedLinkedList(NULL, R);
	else{
		LL = Q->last;
		Q->last = insertUnsortedLinkedList(NULL, R);
		LL->next = Q->last;
	}	

    return;
}

/*QueueCarfree()*/
void QueueCarfree(Queue *Q){
    LinkedList *LL = Q->first;
    Car *C;
    
    while(LL != NULL){
		C = getItemLinkedList(LL);
		free(C->name);	
		LL = getNextNodeLinkedList(LL);
	}
	
	LL = Q->first;
	if(LL != NULL)
		freeLinkedList(LL);
    free(Q);

    return;
}


/*Queuefree()*/
void Queuefree(Queue *Q){
    LinkedList *LL = Q->first;
    
    if(LL != NULL)
		freeLinkedList(LL);
    free(Q);

    return;
}
