#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX 30
#define MAX_NEIGHBORS 10
#define NUMBER_OF_NODES 13

typedef struct Node{
	int degree;
	char name[MAX];
	int color;
	int visited;
	struct Node** neighbors;

}Node;

Node* initialize_node(char* name, int degree){
	Node* n = (Node*) malloc(sizeof(Node)); 
	strcpy(n->name,name);
	n->degree = degree;
	n->visited = 0;
	n->neighbors = (Node**) malloc(sizeof(Node*)*MAX_NEIGHBORS);
	return n;
}

Node* initialize_neighbors(int num, ...){
	va_list valist;
	va_start(valist,num);

	Node* n = va_arg(valist,Node*);
	
	for(int i = 1; i < num; i++){
		n->neighbors[i - 1] = va_arg(valist,Node*);
	}

	va_end(valist);

	return n;
}

void colorfy(Node** n){ // the colors are goint to be 0,1,2,3.
	for(int j = 0; j < NUMBER_OF_NODES; j ++){
		n[j]->color = 0;
		for(int i = 0; i < n[j]->degree; i++){
			if(n[j]->neighbors[i]->visited == 1){
				if(n[j]->neighbors[i]->color == 0 && n[j]->color == 0){
					n[j]->color = 1;
				}else if(n[j]->neighbors[i]->color == 1 && n[j]->color == 1){
					n[j]->color = 2;
				}else if(n[j]->neighbors[i]->color == 2 && n[j]->color == 2){
					n[j]->color = 3;
				}
			}
			
		}
		n[j]->visited = 1;
	}
}

int main(){					//			 0	  1	   2     3   4    5    6    7    8    9    10    11   12
	char* countries[NUMBER_OF_NODES] = {"BR","PR","UR","AR","CH","BO","PE","EC","CO","VN","GY","SU","GF"};
	int number_of_neighbours[NUMBER_OF_NODES] = {10,3,2,5,3,5,5,2,4,3,3,3,2};
	Node* node[NUMBER_OF_NODES];

	for(int i = 0; i < NUMBER_OF_NODES; i++){
		node[i] = initialize_node(countries[i],number_of_neighbours[i]);
		printf("%s %d\n",node[i]->name,node[i]->degree );
	}

	node[0] = initialize_neighbors(node[0]->degree + 1,node[0],node[1],node[2],node[3],node[5],node[6],node[8],node[9],node[10],node[11],node[12]);
	node[1] = initialize_neighbors(node[1]->degree + 1,node[1],node[0],node[3],node[5]);
	node[2] = initialize_neighbors(node[2]->degree + 1,node[2],node[0],node[3]);
	node[3] = initialize_neighbors(node[3]->degree + 1,node[3],node[0],node[1],node[2],node[4],node[5]);
	node[4] = initialize_neighbors(node[4]->degree + 1,node[4],node[3],node[5],node[6]);
	node[5] = initialize_neighbors(node[5]->degree + 1,node[5],node[0],node[1],node[3],node[4],node[6]);
	node[6] = initialize_neighbors(node[6]->degree + 1,node[6],node[0],node[4],node[5],node[7],node[8]);
	node[7] = initialize_neighbors(node[7]->degree + 1,node[7],node[1],node[8]);
	node[8] = initialize_neighbors(node[8]->degree + 1,node[8],node[0],node[6],node[7],node[9]);
	node[9] = initialize_neighbors(node[9]->degree + 1,node[9],node[0],node[8],node[10]);
	node[10] = initialize_neighbors(node[10]->degree + 1,node[10],node[0],node[9],node[11]);
	node[11] = initialize_neighbors(node[11]->degree + 1,node[11],node[0],node[10],node[12]);
	node[12] = initialize_neighbors(node[12]->degree + 1,node[12],node[0],node[11]);

	colorfy(node);

	printf("\n");
	for(int j = 0; j < NUMBER_OF_NODES ; j++){
		printf("Node [%s] -> %d\n",node[j]->name,node[j]->color );
		printf("\n\n");
	}

	return 0;
}
