/******************************************************************************
 *  File name: Gestor.c
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: C-Main of the program that implements the solution to the problem
 * 				entitled by "Parque Automático".
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Parking.h"

int main(int argc, char **argv){
	Graph *Park; /*Main structure that represents the Park*/
	Queue *Q;	/*Queue to save all the restrictions by order*/
	char *ParkFile, *CarFile, *ExitFile, *RestrictionsFile = NULL; /*strings to save the names of the files to open and write solution*/
	int i, restrictions = 0; /*auxiliar integer to change sizes of strings with file names / flag restrictions (1 = active)*/
	
    if(argc < 3){  	 /*must have at least two arguments besides the exec name*/ 
        fprintf(stderr, "This program must have at least two arguments besides the exec name.\n");
        exit(0);
    }
    
    ParkFile = (char *) malloc((strlen(argv[1]) + 1) * sizeof(char)); /*Allocates memory for strings with file names*/
    CarFile = (char *) malloc((strlen(argv[2]) + 1) * sizeof(char));
    ExitFile = (char *) malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(ParkFile, argv[1]);
    strcpy(CarFile, argv[2]);
    
    Park = ReadPark(ParkFile); /*Reads the configuration file and saves everything in Park*/
    
    i = strlen(ParkFile);
    ParkFile[i-4]='\0';
    strcpy(ExitFile, ParkFile);
    strcat(ExitFile, ".pts");
    
    if(argc > 3){ /*Checks if restrictions file exists*/
		RestrictionsFile = (char *) malloc((strlen(argv[3]) + 1) * sizeof(char)); 
		strcpy(RestrictionsFile, argv[3]); /*Allocates memory for string with Restrictions file name and copies argv[3]*/
		Q = QueueNew(); /*Initializes queue for restrictions*/
		Q = ReadRestrictions(RestrictionsFile, Q, Park); /*Read all the restrictions and saves it in the queue*/
		restrictions = 1; /*Activates restrictions flag*/
		RunAutoPark(Park, Q, CarFile, ExitFile, restrictions); /*Runs the autopark manager with restrictions active*/
		free(RestrictionsFile); /* Frees memory for string with Restrictions file name */
		Queuefree(Q); /*Fress queue with restrictions*/
	}	
	
	else	
		RunAutoPark(Park, NULL, CarFile, ExitFile, restrictions); /*Runs the autopark manager with restrictions inactive*/
	
	free(ParkFile); /*Frees strings with file names*/
	free(CarFile);
	free(ExitFile);
	freeGraph(Park); /*Frees the entire map*/

	exit (0);
}
