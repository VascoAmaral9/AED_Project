/******************************************************************************
 *  File name: Graph.h
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: Header file for the implementation of all the functions needed to handle our map.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/
 
/* Prevent multiple inclusions                                      */
#ifndef GraphHeader
#define GraphHeader

#include "LinkedList.h"
#include "Heap.h"

#define maxWT 100000

typedef struct GraphStruct {
    int V;  /*Nº Vértices*/
    int N;	/*Nº Colunas (x)*/
    int M;	/*Nº Linhas (y)*/
    int P;	/*Nº Pisos (z)*/
    char *type;
    LinkedList **l_adj;
    LinkedList *Acc;
    LinkedList *Ent;
    LinkedList *Cars;
    LinkedList *Spots;   
} Graph;

typedef struct NodeStruct {   
    int id;
    int weight;
} Node;

typedef struct AccessStruct {   
    int ID;
    char type;
    int *st;
    int *wt;
} Access;

typedef struct EntryStruct {   
    int ID;
    int *st;
    int *wt;
} Entry;

typedef struct SpotStruct {   
    int ID;
} Spot;

typedef struct CarStruct {   
    char *name;
    int time;
    int ID;
    char status;
} Car;

typedef struct RestrictionStruct {   
    int ta;
    int tb;
    int ID;
    int px;
    char type;
} Restriction;

Graph *InitGraph(int N, int M, int P);
Node *NewNode(int v, int weight);
void InsertNode(Graph *gph, int a, int b, int weight);
void InsertAccess(Graph *gph, int x, int y, int z, char c);
void InsertEntry(Graph *gph, int x, int y, int z);
void InsertSpot(Graph *gph, int ID);
void InsertCar(Graph *gph, Car *C);
void freeGraph(Graph *gph);

/* End of: Protect multiple inclusions                              */
#endif
