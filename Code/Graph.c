/******************************************************************************
 *  File name: Graph.c
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: C-Implementation of all the functions needed to handle our map.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>

#include "Parking.h"

/******************************************************************************
 * InitGraph()
 *
 * Arguments: N, M, P - Integers with the dimension of the park. 
 * Returns: Main structure of type Graph.
 *
 * Description: Initializes and allocates memory for everything needed in the main structure
 *****************************************************************************/
Graph *InitGraph(int N, int M, int P){
	int i;
    Graph *gph;
    
    gph = (Graph*) malloc(sizeof(Graph));
    gph->l_adj = (LinkedList**) malloc(N * M * P * sizeof(LinkedList*));
    gph->type = (char *) malloc(N * M * P * sizeof(char));
    /* Check memory allocation errors */
	if(gph == NULL || gph->l_adj == NULL) 
		return NULL;
    
    gph->V = N * M * P;
    gph->N = N;
    gph->M = M;
    gph->P = P;
    gph->Acc = initLinkedList();
    gph->Ent = initLinkedList();
    gph->Cars = initLinkedList();
    gph->Spots = initLinkedList();
    for (i = 0; i < gph->V ; i++)
        gph->l_adj[i] = initLinkedList();
        
    return gph;
}

/******************************************************************************
 * NewNode()
 *
 * Arguments: id, weight - Integers with the values to fill in the struct node. 
 * Returns: Node.
 *
 * Description: Allocates memory and initialize with values from arguments a struct node.
 *****************************************************************************/
Node *NewNode(int id, int weight){
	Node *N;
    
	N = (Node *) malloc(sizeof(Node));
	/* Check memory allocation errors                               */
	if(N == NULL)
		return NULL;
    
    N->id = id;
    N->weight = weight;
    
    return N;
}

/******************************************************************************
 * InsertNode()
 *
 * Arguments: gph, a, b, weight - Pointer to main struct where to insert node, ID a and b, weight of the adjacency. 
 * Returns: void.
 *
 * Description: Inserts node b in the adjacencys list of node a.
 *****************************************************************************/
 void InsertNode(Graph *gph, int a, int b, int weight){
    
    gph->l_adj[a] = insertUnsortedLinkedList(gph->l_adj[a], NewNode(b, weight));
}

/******************************************************************************
 * InsertAccess()
 *
 * Arguments: gph, x, y, z, c - Pointer to main struct where to insert access, coordinates of the access, type of access. 
 * Returns: void.
 *
 * Description: Allocates memory for an access and vectors pointed by it, initializes it and fills it with the arguments.
 *****************************************************************************/
void InsertAccess(Graph *gph, int x, int y, int z, char c){
	Access *A;
	int ID = GetID(x, y, z, gph->N, gph->M);
	
	A = (Access *) malloc(sizeof(Access));
	A->st = (int *) malloc(gph->V * sizeof(int));
	A->wt = (int *) malloc(gph->V * sizeof(int));
	A->wt[0] = -1;
	/* Check memory allocation errors                               */
	if(A == NULL || A->st == NULL || A->wt == NULL){
		fprintf(stderr, "ERROR: Allocating memory\n");
		exit ( 0 );
	}
	
	A->ID = ID;
	A->type = c;
	
	gph->Acc = insertUnsortedLinkedList(gph->Acc, A);	
}

/******************************************************************************
 * InsertEntry()
 *
 * Arguments: gph, x, y, z - Pointer to main struct where to insert entry, coordinates of the entry. 
 * Returns: void.
 *
 * Description: Allocates memory for an entry and vectors pointed by it, initializes it and fills it with the arguments.
 *****************************************************************************/
void InsertEntry(Graph *gph, int x, int y, int z){
	Entry *E;
	int ID = GetID(x, y, z, gph->N, gph->M);
	
	E = (Entry *) malloc(sizeof(Entry));
	E->st = (int *) malloc(gph->V * sizeof(int));
	E->wt = (int *) malloc(gph->V * sizeof(int));
	E->wt[0] = -1;
	/* Check memory allocation errors                               */
	if(E == NULL || E->st == NULL || E->wt == NULL){
		fprintf(stderr, "ERROR: Allocating memory\n");
		exit ( 0 );
	}
	
	E->ID = ID;
	
	gph->Ent = insertUnsortedLinkedList(gph->Ent, E);
}

/******************************************************************************
 * InsertSpot()
 *
 * Arguments: gph, ID - Pointer to main struct where to insert spot, ID of Spot. 
 * Returns: void.
 *
 * Description: Allocates memory for a spot, initializes it and fills it with the arguments.
 *****************************************************************************/
void InsertSpot(Graph *gph, int ID){
	Spot *S;
	
	S = (Spot *) malloc(sizeof(Spot));
	/* Check memory allocation errors                               */
	if(S == NULL){
		fprintf(stderr, "ERROR: Allocating memory\n");
		exit ( 0 );
	}
	
	S->ID = ID;
	gph->type[ID] = '.';
	
	gph->Spots = insertUnsortedLinkedList(gph->Spots, S);
}

/******************************************************************************
 * InsertCar()
 *
 * Arguments: Park, C - Pointer to main struct where to insert car, pointer to struct car to insert the LinkedList. 
 * Returns: void.
 *
 * Description: First frees the free spot where the car will be parked, from the linked list, 
 * 				and then inserts a car in a linked list too.
 *****************************************************************************/
void InsertCar(Graph *Park, Car *C){
	LinkedList *LL = Park->Spots, *aux = NULL;
	Spot *S;
    
	while(LL != NULL){
		S = getItemLinkedList(LL);
		if(S->ID == C->ID)
			break;
		aux = LL;	
		LL = getNextNodeLinkedList(LL);
	}
	if(aux != NULL){
		aux->next = LL->next;
		free(LL->this);
		free(LL);
	}
				
	else{
		Park->Spots = LL->next;
		free(LL->this);
		free(LL);
	}
	Park->type[C->ID] = 'x';
		
	Park->Cars = insertUnsortedLinkedList(Park->Cars, C);
}

/******************************************************************************
 * freeGraph()
 *
 * Arguments: gph - Pointer to main struct to be freed. 
 * Returns: void.
 *
 * Description: Go through all the linked lists to free every memory allocated and than free 
 * 				the linked lists and finally the struct.
 *****************************************************************************/
void freeGraph(Graph *gph){
    int i;
    LinkedList *LL = gph->Acc;
    Access *A;
    Entry *E;
    Car *C;
    
    for (i = 0; i < gph->V; i++)
        freeLinkedList(gph->l_adj[i]);
    
    while (LL != NULL){
            A = getItemLinkedList(LL);
			free(A->st);
			free(A->wt);
            LL = getNextNodeLinkedList(LL);
    }
    
    LL = gph->Ent;    
    while (LL != NULL){
            E = getItemLinkedList(LL);
			free(E->st);
			free(E->wt);
            LL = getNextNodeLinkedList(LL);
    }
     
    LL = gph->Cars;    
    while (LL != NULL){
            C = getItemLinkedList(LL);
			free(C->name);
            LL = getNextNodeLinkedList(LL);
    }   
            
    freeLinkedList(gph->Acc);
    freeLinkedList(gph->Ent);
    freeLinkedList(gph->Cars);
    freeLinkedList(gph->Spots);
    free(gph->type);  
    free(gph->l_adj);
    free(gph);
}
