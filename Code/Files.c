/******************************************************************************
 *  File name: Files.c
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: C-Implementation of all the functions needed to read the files.
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

/******************************************************************************
 * OpenFile()
 *
 * Arguments: name - pointer to string holding name of file to open
 *            mode - pointer to string holding mode to use for opening file
 * Returns: pointer to opened file
 * Side-Effects: exits if given file cannot be opened with given mode
 *
 * Description: From AED Team
 *
 *****************************************************************************/

FILE *OpenFile (char *name, char *mode){
	FILE *f;
	f = fopen(name, mode);
	if ( f == NULL ){
		fprintf(stderr, "ERROR: It is not possible to open file %s!\n", name );
		return NULL;
		/* Error messages are sent to the stderr channel.
        Use an exit number different from 0 (zero) in case of error.*/
	}
	return f;
}

/******************************************************************************
 * ReadPark()
 *
 * Arguments: ParkFile - String with the name of the file with the configuraton of the park. 
 * Returns: Pointer to the main structure.
 *
 * Description: Opens the file in read mode and reads the first line with sizes of the map, then initiates
 * 				the main structure with this values, finally for each floor, reads the floor and the access 
 * 				and entrances in it, for every entrance and access inserts them in the respective list.
 *****************************************************************************/
Graph *ReadPark(char *ParkFile){
	FILE *f;
	Graph *Park;
	int N, M, P, E, S;
	int z, x1 = 0, y1 = 0, z1 = 0;
	char c[MAX_STR], c1[MAX_WORD], type;
	
	f = OpenFile(ParkFile, "r");
	if(fscanf(f, "%d %d %d %d %d\n", &N, &M, &P, &E, &S) != 5)
		exit( 0 );
	
	Park = InitGraph(N, M, P);

	for(z = 0; z < Park->P; z++){
		ReadFloor(Park, f, z);
		while(fgets(c, MAX_STR, f)){
			if(c[0] == 'E'){
				sscanf(c, "%s %d %d %d %c", c1, &x1, &y1, &z1, &type); 
				InsertEntry(Park, x1, y1, z1);
			}
			else if(c[0] == 'A'){
				sscanf(c, "%s %d %d %d %c", c1, &x1, &y1, &z1, &type); 
				InsertAccess(Park, x1, y1, z1, type);
			}
			else
				break;
		}	
	}	
		
	fclose(f);
	
	return Park;
}

/******************************************************************************
 * ReadFloor()
 *
 * Arguments: Park, f, z - Pointer to the main structure, pointer to the file with park configuration 
 * 							and an integer containing the current floor.
 * Returns: void
 *
 * Description: Allocates memory for 3 vectors, one per line, they represent the actual, the previous and the next line,
 * 				after this for each character read, compares with all others around him and depending of the conditions
 * 				of the graph, it can insert a node with the right weight and ID in the other's Adjacency list or not.
 *****************************************************************************/
void ReadFloor(Graph *Park, FILE *f, int z){
	int x = 0, y = (Park->M - 1), ID;
	char *prev, *act, *next, *aux, type;
	
	prev = (char*) malloc((Park->N + 2)*sizeof(char));
	act = (char*) malloc((Park->N + 2)*sizeof(char));
	next = (char*) malloc((Park->N + 2)*sizeof(char));
	/* Check memory allocation errors                               */
	if(prev == NULL || act == NULL || next == NULL){
		fprintf(stderr, "ERROR: Allocating memory\n");
		exit ( 0 );
	}	
	
	if(fgets(act, (Park->N + 2), f) == NULL)
		exit(0);
	for(y = (Park->M - 1); y > -1; y--){
		if(y != 0)
			if(fgets(next, (Park->N + 2), f) == NULL)
				exit( 0 );
		for(x = 0; x < Park->N; x++){
			ID = GetID(x, y, z, Park->N, Park->M);
			type = act[x];
			Park->type[ID] = type;
			if(type == '@' || type == 'e' || type == 'a')
				continue;
			else if(type != '.' && type != 'x'){
				if(y != (Park->M - 1) && prev[x] != '@' && prev[x] != '.' && prev[x] != 'x') 		/*Upper spot*/
					InsertNode(Park, GetID(x, y+1, z, Park->N, Park->M), ID, 1);
				if(y != 0 && next[x] != '@' && next[x] != '.' && next[x] != 'x') 					/*Bottom spot*/
					InsertNode(Park, GetID(x, y-1, z, Park->N, Park->M), ID, 1);
				if(x != 0 && act[x-1] != '@' && act[x-1] != '.' && act[x-1] != 'x') 				/*Left spot*/
					InsertNode(Park, GetID(x-1, y, z, Park->N, Park->M), ID, 1);
				if(x != (Park->N - 1) && act[x+1] != '@' && act[x+1] != '.' && act[x+1] != 'x')		/*Right spot*/
					InsertNode(Park, GetID(x+1, y, z, Park->N, Park->M), ID, 1);
				if(type == 'u')																		/*Floor above*/
					InsertNode(Park, GetID(x, y, z+1, Park->N, Park->M), ID, 2);
				if(type == 'd')																		/*Floor below*/
					InsertNode(Park, GetID(x, y, z-1, Park->N, Park->M), ID, 2);	
			}
			else{
				if(y != (Park->M - 1) && (prev[x] == ' ' || prev[x] == 'd' || prev[x] == 'u' )) 		/*Upper spot*/
					InsertNode(Park, GetID(x, y+1, z, Park->N, Park->M), ID, 1);
				if(y != 0 && (next[x] == ' ' || next[x] == 'd' || next[x] == 'u' )) 					/*Bottom spot*/
					InsertNode(Park, GetID(x, y-1, z, Park->N, Park->M), ID, 1);
				if(x != 0 && (act[x-1] == ' ' || act[x-1] == 'd' || act[x-1] == 'u' )) 					/*Left spot*/
					InsertNode(Park, GetID(x-1, y, z, Park->N, Park->M), ID, 1);
				if(x != (Park->N - 1) && (act[x+1] == ' ' || act[x+1] == 'd' || act[x+1] == 'u' ))		/*Right spot*/
					InsertNode(Park, GetID(x+1, y, z, Park->N, Park->M), ID, 1);
				if(type == '.')
					InsertSpot(Park, ID);
			}				
		}
		aux = next;
		next = prev;
		prev = act;
		act = aux;
	}
	free(prev);
	free(act);
	free(next);
}

/******************************************************************************
 * ReadCar()
 *
 * Arguments: Park, f - Pointer to the main structure, pointer to the file with cars entering.
 * Returns: void
 *
 * Description: Uses a fgets to read the all line, allocates memory for struct car, sscanf from 
 * 				the string stored with fgets and if sscanf returns 6, fills the struct with the right values, 
 * 				if sscanf only returns 3 (that's an option described in the formulation of the initial problem) 
 * 				C->ID it will be an "InvalidID", wich means that it leaves the park by the name.
 *****************************************************************************/
Car *ReadCar(Graph *Park, FILE *f){
	Car *C;
	char c[MAX_STR], name[MAX_WORD], status;
	int time, x, y, z;
	
	if(fgets(c, MAX_STR, f) == NULL)
		return NULL;
		
	C = (Car *) malloc(sizeof(Car));
	/* Check memory allocation errors */
	if(C == NULL) 
		return NULL;	

	if(sscanf(c, "%s %d %c %d %d %d", name, &time, &status, &x, &y, &z) == 6){
		C->name = (char *) malloc((strlen(name) + 1)*sizeof(char));
		strcpy(C->name, name);
		C->time = time;
		C->ID = GetID(x, y, z, Park->N, Park->M);
		C->status = status;
	}
	else{
		C->name = (char *) malloc((strlen(name) + 1)*sizeof(char));
		strcpy(C->name, name);
		C->time = time;
		C->ID = InvalidID;
		C->status = status;
	}
	
	return C;
}

/******************************************************************************
 * ReadRestrictions()
 *
 * Arguments: RestrictionsFile, Q, Park - String with the name of the file where restrictions are, 
 * 				pointer do the queue where restrictions will be stored and pointer to the main structure.
 * Returns: void
 *
 * Description: First it opens file in the read mode, that reads each line with an fgets and sscanf, sscanf 
 * 				can only return 3 or 6, if 3 it's a floor restriction, otherwise it's a spot restriction,
 * 				after reading, assigns values to the struct Restriction and inserts it in a linked list.		
 *****************************************************************************/
Queue *ReadRestrictions(char *RestrictionsFile, Queue *Q, Graph *Park){
	FILE *f;
	char c[MAX_STR], aux;
	int ta, tb, x, y, z, ID;
	
	f = OpenFile(RestrictionsFile, "r");
	
	while(fgets(c, MAX_STR, f)){
		if(sscanf(c, "%c %d %d %d %d %d\n", &aux, &ta, &tb, &x, &y, &z) == 6){
			ID = GetID(x, y, z, Park->N, Park->M);
			InsertLastRestriction(Q, ta, tb, ID, -1, Park);
		}
		else
			InsertLastRestriction(Q, ta, tb, -1, x, Park);
	}
	
	fclose(f);
	return Q;
}
