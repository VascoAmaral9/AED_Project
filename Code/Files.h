/******************************************************************************
 *  File name: Files.h
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: Header file for the implementation of all the functions 
 * 				 needed to read the files.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/
 
/* Prevent multiple inclusions                                      */
#ifndef FilesHeader
#define FilesHeader

#include "Queue.h"

#define MAX_STR 200
#define MAX_WORD 16
#define InvalidID -1

FILE *OpenFile(char *name, char *mode);
int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm);
Graph *ReadPark(char *ParkFile);
void ReadFloor(Graph *Park, FILE *f, int z);
Car *ReadCar(Graph *Park, FILE *f);
Queue *ReadRestrictions(char *RestrictionsFile, Queue *Q, Graph *Park);


/* End of: Protect multiple inclusions                              */
#endif
