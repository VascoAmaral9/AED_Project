/******************************************************************************
 *  File name: Parking.h
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: Header for the implementation of all the main functions that lead with the 
 * 					movements inside the Park. Everything that leads with movement inside the park,
 * 					it's handled here.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/

/* Prevent multiple inclusions                                      */
#ifndef ParkingHeader
#define ParkingHeader

#include "Files.h"

#define FWT (E->wt[AuxS->ID] + 3*AuxA->wt[AuxS->ID])
#define wght (wt[v] + N->weight)

int GetID(int x, int y, int z, int N, int M);
int GetX(int ID, int z, int N, int M);
int GetY(int ID, int x, int z, int N, int M);
int GetZ(int ID, int N, int M);
int GreatNum(int a, int b, int wt[]);
void Dijkstra(Graph *gph, int s, int st[], int wt[]);
void RunAutoPark(Graph *Park, Queue *Q, char *CarFile, char *ParkFile, int restrictions);
void ProcessCar(Graph *Park, Queue *Q, Car *C, FILE *f_out, int Dij);
void DriveCar(Car *C, Graph *Park, FILE *f_out, int time, int Dij);
void RemoveCar(Car *C, Graph *Park, FILE *f_out, int ID);
void WriteExit(Graph *Park, char *name, int time, FILE *f_out, int ID, char status);
int WritePath(Graph *Park, char *name, int time, FILE *f_out, int ID, int fID, char status);
void RestrictFloor(Graph *Park, int px);
void UnRestrictFloor(Graph *Park, int px);
void freeCar(Car *C);


/* End of: Protect multiple inclusions                              */
#endif
