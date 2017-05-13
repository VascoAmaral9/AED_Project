/******************************************************************************
 *  File name: Parking.c
 *
 *  Author: Vasco Amaral and Miguel Santos
 *
 *  Release date: 2015/11/09
 *
 *  Description: C-Implementation of all the main functions that lead with the movements inside the Park.
 * 					Including getting the ID and the coordinates, aplly Dijkstra, write in the output file
 * 					car move's, check restrictions and process each car individually
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
 * GetID()
 *
 * Arguments: x, y, z, N, M - 5 Integers to give ID
 * Returns: Integer with the ID according to map and coordinates
 *
 * Description: Receives 5 integers that describe the dimension of the map and the actual position(x, y, z) and
 * 				applys a formula to get the ID according to how the graph was made.
 *****************************************************************************/

int GetID(int x, int y, int z, int N, int M){
	int ID;

	ID = (M - 1 - y) * N + x + z * N * M;

	return ID;
}

/******************************************************************************
 * GetX()
 *
 * Arguments: ID, z, N, M - 4 Integers to give X
 * Returns: Integer with the coordinate x according to map, coordinates and ID
 *
 * Description: Receives 4 integers that describe the dimension of the map and the actual position(ID, z) and
 * 				applys a formula to get the coordinate x according to how the graph was made.
 *****************************************************************************/
int GetX(int ID, int z, int N, int M){
	int x;

	x = (ID - z * N * M) % N;

	return x;
}

/******************************************************************************
 * GetY()
 *
 * Arguments: ID, x, z, N, M - 5 Integers to give Y
 * Returns: Integer with the coordinate y according to map, coordinates and ID
 *
 * Description: Receives 5 integers that describe the dimension of the map and the actual position(ID, z, x) and
 * 				applys a formula to get the coordinate y according to how the graph was made.
 *****************************************************************************/
int GetY(int ID, int x, int z, int N, int M){
	int y;

	y = -(((ID - x - z * N * M)/N) - M + 1);

	return y;
}

/******************************************************************************
 * GetZ()
 *
 * Arguments: ID, N, M - 3 Integers to give Z
 * Returns: Integer with the coordinate z according to map and ID
 *
 * Description: Receives 3 integers that describe the dimension of the map and the actual position(ID) and
 * 				applys a formula to get the coordinate z according to how the graph was made.
 *****************************************************************************/
int GetZ(int ID, int N, int M){
	int z;

	z = ID/(N * M);

	return z;
}

/******************************************************************************
 * GreatNum()
 *
 * Arguments: a, b - 2 Integers to be compared
 * Returns: Integer with the results of the comparison
 *
 * Description: Comparison function with abstract type to be passed as argument;
 *              Function is specific for "int" variables
 *
 *****************************************************************************/

int GreatNum(int a, int b, int wt[]){
	
	return (wt[a] > wt[b]);
}

/******************************************************************************
 * Dijkstra()
 *
 * Arguments: Park, s, st[], wt[] - Pointer to main struct, integer of the ID where the algorithm 
 * 				begins and 2 integer vectors with the weights and directions of the path
 * Returns: void
 *
 * Description: Changes wt[] and st[] to the values that describe the best path between s and all the other nodes, it has
 * 				 one condition so that everytimes a car enters in a ramp it has to go up or down, it doesn't proceed normally.
 *****************************************************************************/
void Dijkstra(Graph *Park, int s, int st[], int wt[]){
	int v, w;
	LinkedList *LL;
	Node *N;
	Heap *H;

	H = NewHeap(Park->V, GreatNum);
	for(v = 0; v < Park->V; v++){
		st[v] = -1;
		wt[v] = maxWT;
		Insert(H, v);
	}
	wt[s] = 0;
	PQDec(H, s, wt);

	while(!HeapEmpty(H))
		if(wt[v = RemoveMin(H, wt)] != maxWT)
			for(LL = Park->l_adj[v]; LL != NULL; LL = getNextNodeLinkedList(LL)){
				N = getItemLinkedList(LL);
				if((Park->type[v] == 'u' || Park->type[v] == 'd') && (Park->type[st[v]] != 'u' && Park->type[st[v]] != 'd') && 
														(Park->type[N->id] != 'u' && Park->type[N->id] != 'd'))									
					continue;
				if(Park->type[N->id] == '@')
					continue;
				if(wt[w = N->id] > wght){
					wt[w] = wght;
					PQDec(H, w, wt);
					st[w] = v;
				}
			}						
	FreeHeap(H);					
}

/******************************************************************************
 * RunAutoPark()
 *
 * Arguments: Park, Q, CarFile, ExitFile, restrictions - Pointer to main struct,
 * 				pointer to Queue containing the restrictions by order of appeareance 
 * 				if restriction's file exists, 2 strings containing the names of the files and 
 * 				a flag, 1 if restrictions exist, 0 otherwise
 * Returns: void
 *
 * Description: Handles all the restrictions, when to activate or erase them, and
 * 				when to process each car.
 *****************************************************************************/
void RunAutoPark(Graph *Park, Queue *Q, char *CarFile, char *ExitFile, int restrictions){		
	FILE *f_in, *f_out;
	Car *C;
	Queue *CQ = NULL;
	Restriction *R = NULL, *AuxR = NULL;
	LinkedList *LL = NULL, *ActRes = NULL , *auxLL = NULL, *auxPLL = NULL, *auxDel = NULL;
	int FlagR = 1, Dij = 0, del = 0, First = 1;
	
	CQ = QueueNew(); /*Initializes queue for cars in waiting list*/
	
	f_in = OpenFile(CarFile, "r"); /*Open files*/
	f_out = OpenFile(ExitFile, "w");
	
	if(restrictions != 0){ /*If restrictions are active*/
		ActRes = initLinkedList(); /*Inicializes linked list for active restrictions*/
		while((C = ReadCar(Park, f_in)) != NULL){ 	/*Reads actual car*/
			while(FlagR == 1){ 						/*Only reads if read flag is active*/
				if(LL == NULL && Q->first != NULL){	
					LL = GetFirst(Q);
					R = getItemLinkedList(LL);
					free(LL);				/*Frees pointer to linked list wich already saved the item*/
					First = 1;				/*Activates flag of the first time that is read*/
				}
				FlagR = 0;				/*Deactivates read flag*/
				/*If the time that the car will start moving is equal or bigger than the time
					that the restriction become active, activates that restriction*/
				if(C->time >= R->ta && First == 1){ 
					if(R->px == -1){				/*Checks if it is a spot restricion*/
						ActRes = insertUnsortedLinkedList(ActRes, R); /*Inserts in active restrictions linked list the new restriction*/
						Park->type[R->ID] = '@'; /*Assumes the new spot it's a wall, so nothing passes through*/
						Dij = 1; /*Activates Dijkstra flag*/
						First = 0;/*Deactivates first time flag*/
					}	
					else{ /*It is a floor restriction*/
						ActRes = insertUnsortedLinkedList(ActRes, R);/*Inserts in active restrictions linked list the new restriction*/
						RestrictFloor(Park, R->px); /*Restricts the floor indicated*/
					}	
					LL = NULL;
					FlagR = 1; /*Activates read flag*/
				}		
			}
			
			/*Checks in the same way the previous code checked if there is a restriction, in the active restricions linked list,
			 that ends, before or at the same time that the new car will move*/
			auxLL = ActRes;
			auxPLL = NULL;
			while(auxLL != NULL){
				AuxR = getItemLinkedList(auxLL);
				if(C->time >= AuxR->tb){
					if(AuxR->px == -1){
						Park->type[AuxR->ID] = AuxR->type;
						Dij = 1;
					}	
					else{
						UnRestrictFloor(Park, AuxR->px);
						if(auxPLL != NULL){
							auxPLL->next = auxLL->next;
							auxDel = auxLL;
							del = 1;
						}
				
						else{
							ActRes = auxLL->next;
							auxDel = auxLL;
							del = 1;
						}
						
					}
				}
				auxPLL = auxLL;					/*Passes for next node of the list*/
				auxLL = getNextNodeLinkedList(auxLL);
				if(del == 1){
					free(auxDel->this); /*Frees restriction that is active if delete flag is on*/
					free(auxDel);
					del = 0;
				}	
			}	
			ProcessCar(Park, CQ, C, f_out, Dij); /*Processes each car*/
			FlagR = 1;	/*Activates flag for reading restrictions*/
			Dij = 0; /*Desactivates flag for running Dijkstra*/
		}
		freeLinkedList(ActRes);
	}	
		
	else /*If restrictions are inactive*/
		while((C = ReadCar(Park, f_in)) != NULL) /*Reads actual Car*/
			ProcessCar(Park, CQ, C, f_out, Dij); /*Processes each car*/
	
	QueueCarfree(CQ);	/*Frees the waiting list of cars*/
	fclose(f_in);	/*close files*/
	fclose(f_out);
}

/******************************************************************************
 * ProcessCar()
 *
 * Arguments: Park, Q, C, f_out, Dij - Pointer to main struct,
 * 				pointer to Queue containing cars in "wait list", when there aren't any 
 * 				parking spots avaiable, pointer to the struct cointaining everything about 
 * 				the car being processed, pointer to the exit file and a flag, if 1, new Drikstra is needed.
 * Returns: void
 *
 * Description: Receives a car to process, if he wants to exit, writes the output, removes all the memory alocated 
 * 				to that car and checks if there is any car in the "waiting list", if so, calls function to drive him,
 * 				just like it does if the car to process wants to park.
 *****************************************************************************/
void ProcessCar(Graph *Park, Queue *Q, Car *C, FILE *f_out, int Dij){
	Car *C2 = NULL;
	LinkedList *LL, *aux = NULL, *LL2 = Park->Spots;
	int timeQueue;

	if(C->status == 'S'){
		timeQueue = C->time;
		if(C->ID == InvalidID){
			LL = Park->Cars;    
			while(LL != NULL){
				C2 = getItemLinkedList(LL);
				if(strcmp(C2->name, C->name) == 0)
					break;
				aux = LL;	
				LL = getNextNodeLinkedList(LL);
			}
			RemoveCar(C, Park, f_out, C2->ID);
			
			if(aux != NULL){
				aux->next = LL->next;
				free(C2->name);
				free(LL->this);
				free(LL);
				LL = NULL;
			}
			else{
				Park->Cars = LL->next;
				free(C2->name);
				free(LL->this);
				free(LL);
				LL = NULL;
			}
			
			if(Q->first != NULL){
				LL = GetFirst(Q);
				C = getItemLinkedList(LL);
				free(LL);
				DriveCar(C, Park, f_out, timeQueue, Dij);
			}	
		}
		
		else{
			RemoveCar(C, Park, f_out, C->ID);
			if(Q->first != NULL){
				LL = GetFirst(Q);
				C = getItemLinkedList(LL);
				free(LL);
				DriveCar(C, Park, f_out, timeQueue, Dij);
			}	
		}		
	}	
	else{
		if(LL2 != NULL){
			WritePath(Park, C->name, C->time, f_out, C->ID, -1, 'i');
			DriveCar(C, Park, f_out, C->time, Dij);
		}	
		else{
			WritePath(Park, C->name, C->time, f_out, C->ID, -1, 'i');
			InsertLastCar(Q, C);
		}	
	}		
}	


/******************************************************************************
 * DriveCar()
 *
 * Arguments: C, Park, f_out, time, Dij - Pointer to the struct cointaining everything about 
 * 				the car being driven, pointer to main struct, pointer to the exit file, integer 
 * 				with the time of entrance in the park (different from the time of entrance in the wait list)
 * 				and a flag, if 1, new Drikstra is needed.
 * Returns: void
 *
 * Description: Receives a car to drive, first, determines the best spot to park, based in the distances, and that walking on foot
 * 				is 3 times longer than by car. Applys dijkstra when is the first time that uses an access or an entrance. After this,
 * 				drives the car through the path defined in Dijkstra, and applys the algorithm that writes in the exit file, only when
 * 				it changes directions.
 *****************************************************************************/
void DriveCar(Car *C, Graph *Park, FILE *f_out, int time, int Dij){
	Entry *E = NULL;
	Access *A = NULL, *AuxA;
	Spot *S = NULL, *AuxS;
	LinkedList *LL = Park->Ent, *LL2 = Park->Spots;
	int FinalWT = maxWT, *stf = NULL, i = 0, auxID = -1, timeP, PlusWT = (time - C->time), flagWrite = 0;
	
	while (LL != NULL){
		E = getItemLinkedList(LL);
		if(E->ID == C->ID)
			break;
		LL = getNextNodeLinkedList(LL);
    }
    if(Dij == 1 || E->wt[0] == -1)
		Dijkstra(Park, E->ID, E->st, E->wt);
	
	LL = Park->Acc;
	while (LL != NULL){
		AuxA = getItemLinkedList(LL);
		if(AuxA->type != C->status){
			LL = getNextNodeLinkedList(LL);
			continue;
		}	
		if(Dij == 1 || AuxA->wt[0] == -1)
			Dijkstra(Park, AuxA->ID, AuxA->st, AuxA->wt);
			
		LL2 = Park->Spots;
		while (LL2 != NULL){
			AuxS = getItemLinkedList(LL2);
			if(FWT < FinalWT){
				FinalWT = FWT;
				S = AuxS;
				A = AuxA;
			}	
			LL2 = getNextNodeLinkedList(LL2);
		}	
		LL = getNextNodeLinkedList(LL);
    }
    
    stf = (int *) malloc(FinalWT * sizeof(int)); /*Size of Final Weight, because is the biggest that this vector can get*/
    stf[0] = E->st[S->ID];
    for(i = 1; i < FinalWT; i++)
		if((stf[i] = E->st[stf[i-1]]) == -1)
			break;	
	
    for(i = i-2; i > 0; i--){
		if((Park->type[stf[i]] == 'u' || Park->type[stf[i]] == 'd') && (Park->type[C->ID] == 'u' || Park->type[C->ID] == 'd'))
			time+=2;
		else
			time++;
		C->ID = stf[i];
		if(WritePath(Park, C->name, time, f_out, C->ID, stf[i-1], 'm') == 1)
			flagWrite = 1;
	}
	if((Park->type[stf[i]] == 'u' || Park->type[stf[i]] == 'd') && (Park->type[C->ID] == 'u' || Park->type[C->ID] == 'd'))
			time+=2;
		else
			time++;
	C->ID = stf[i];
	if(WritePath(Park, C->name, time, f_out, C->ID, S->ID, 'm') == 1)
		flagWrite = 1;
		
	if(flagWrite == 0)
		WriteExit(Park, C->name, time, f_out, C->ID, 'm');	
	
	flagWrite = 0;
	time++;
	auxID = C->ID = S->ID; 		/*When vector stf ends, only position left is the parking spot*/
	WritePath(Park, C->name, time, f_out, auxID, -1,'e');
	timeP = time;				/*Time of Parking*/
	InsertCar(Park, C); 		/*Insert after reaching the destination spot*/
	
	while(A->st[auxID] != A->ID){
			if((Park->type[A->st[auxID]] == 'u' || Park->type[A->st[auxID]] == 'd') 
										&& (Park->type[auxID] == 'u' || Park->type[auxID] == 'd'))
			time+=2;
		else
			time++;
		auxID = A->st[auxID];
		if(WritePath(Park, C->name, time, f_out, auxID, A->st[auxID], 'p') == 1)
			flagWrite = 1;
	}
	if(flagWrite == 0)
		WriteExit(Park, C->name, time, f_out, auxID, 'p');

	time++;
	auxID = A->ID;
	WritePath(Park, C->name, time, f_out, auxID, -1, 'a');
	escreve_saida(f_out, C->name, C->time, timeP, time, FinalWT + PlusWT, 'x');
	
	free(stf);
}		

/******************************************************************************
 * RemoveCar()
 *
 * Arguments: C, Park, f_out, ID - Pointer to the struct cointaining everything about 
 * 				the car being removed, pointer to main struct, pointer to the exit file, integer 
 * 				with the ID of the car parking spot.
 * Returns: void
 *
 * Description: Receives a car to remove, first, writes the output excpeted, then inserts a new free spot in the linked list of 
 * 				free spots, and finally frees the memory of that car.
 *****************************************************************************/
void RemoveCar(Car *C, Graph *Park, FILE *f_out, int ID){
	
	WriteExit(Park, C->name, C->time, f_out, ID, 's');
	InsertSpot(Park, ID);	

	freeCar(C);
}

/******************************************************************************
 * WriteExit()
 *
 * Arguments: Park, name, time, f_out, ID, status - Pointer to main struct, string with the indicator of the car, 
 * 				pointer to the exit file, integer with the ID where the car is, char with the car status, to be written.
 * Returns: void
 *
 * Description: Receives all the values needed to write the car position in the exit file, first converts ID into coordinates,
 * 				and then writes with the function given by AED TEAM.
 *****************************************************************************/
void WriteExit(Graph *Park, char *name, int time, FILE *f_out, int ID, char status){
	int x, y, z;
	
	z = GetZ(ID, Park->N, Park->M);
	x = GetX(ID, z, Park->N, Park->M);
	y = GetY(ID, x, z, Park->N, Park->M);
	escreve_saida(f_out, name, time, x, y, z, status);
	
}

/******************************************************************************
 * WritePath()
 *
 * Arguments: Park, name, time, f_out, ID, fID, status - Pointer to main struct, string with the indicator of the car, 
 * 				pointer to the exit file, 2 integers, one with the ID where the car is and other with the ID of where it will be
 * 				in the next move, char with the car status, to be written.
 * Returns: 1 if it writes or 0 if don't.
 *
 * Description: Receives all the values needed to write the car position in the exit file if it changes directions, 
 * 				first compares the actual ID with its future value(received) and the previous(stored in a static int), 
 * 				and if the car doesn't change direction returns 0, then if it is to continue converts ID into coordinates 
 * 				and finally writes in output with the function given by AED TEAM.
 *****************************************************************************/
int WritePath(Graph *Park, char *name, int time, FILE *f_out, int ID, int fID, char status){
	static int pID = -1;
	int x, y, z;
	
	if((ID - pID) == (fID - ID)){
		pID = ID;
		return 0 ;
	}		
		
	z = GetZ(ID, Park->N, Park->M);
	x = GetX(ID, z, Park->N, Park->M);
	y = GetY(ID, x, z, Park->N, Park->M);
	
	escreve_saida(f_out, name, time, x, y, z, status);
	
	pID = ID;						
	
	return 1;
}	

/******************************************************************************
 * RestrictFloor()
 *
 * Arguments: Park, px - Pointer to main struct and integer with the value of the floor restricted. 
 * Returns: void
 *
 * Description: Receives the value of the floor to restrict, calculates the minimum and maximum ID in that floor,
 * 				then for every spot in that floor, removes it from the list of free spots.
 *****************************************************************************/
void RestrictFloor(Graph *Park, int px){
	LinkedList *LL = Park->Spots, *aux = NULL, *aux2 = NULL;
	Spot *S;
	int IDmin, IDmax;
	
	IDmin = GetID(0, (Park->M - 1), px, Park->N, Park->M);
	IDmax = GetID((Park->N - 1), 0, px, Park->N, Park->M);
    
	while(LL != NULL){
		S = getItemLinkedList(LL);
		if(IDmin <= S->ID && S->ID<= IDmax){
			if(aux != NULL){
				aux->next = LL->next;
				free(LL->this);
				aux2 = LL;
				LL = getNextNodeLinkedList(LL); 	/*aux stays the same*/
				free(aux2);
				
			}
				
			else{
				Park->Spots = LL->next;
				free(LL->this);
				aux2 = LL;
				LL = getNextNodeLinkedList(LL); 	/*aux stays the same*/
				free(aux2);
			}
		}
		else{
			aux = LL;	
			LL = getNextNodeLinkedList(LL);
		}					
	}
}	

/******************************************************************************
 * UnRestrictFloor()
 *
 * Arguments: Park, px - Pointer to main struct and integer with the value of the floor restricted. 
 * Returns: void
 *
 * Description: Receives the value of the floor to unrestrict, calculates the minimum and maximum ID in that floor,
 * 				then for every spot in that floor, inserts it back in the list of free spots.
 *****************************************************************************/
void UnRestrictFloor(Graph *Park, int px){
	int IDmin, IDmax, i = 0;
	
	IDmin = GetID(0, (Park->M - 1), px, Park->N, Park->M);
	IDmax = GetID((Park->N - 1), 0, px, Park->N, Park->M);
	
	for(i = IDmin; i<=IDmax; i++)
		if(Park->type[i] == '.')
			InsertSpot(Park, i);
}

/******************************************************************************
 * freeCar()
 *
 * Arguments: C - Pointer to the car to be freed.
 * Returns: void
 *
 * Description: Receives a car and frees his name and the respective structure
 *****************************************************************************/
void freeCar(Car *C){
	
	free(C->name);
	free(C);
	
}
