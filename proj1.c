#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GREY 1
#define BLACK 2 
#define NIL -1

typedef struct node {
	
	int vertex;
	struct node* next;
}* Node;

typedef struct graph {

	int vertices;
	int edges;
	Node* adjacency_list;
}* Graph;

Node addNode(int vertex, Node head);
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination);
void dfs(Graph graph);
void dfsVisit(Graph graph, int head, int* time);

typedef struct photo {

	int colour;
	int parent;
	int disc_time;
	int fin_time;
} Photo;

Photo* photo_info;

int main()
{
	int vertices, edges;
	int i;
	int u, v;

	Graph photo_graph;

	scanf("%d %d", &vertices, &edges);

	photo_graph = initGraph(vertices);

	for (i = 0; i < edges; i++) {

		scanf("%d %d", &u, &v);
		addEdge(photo_graph, (u - 1), (v - 1));
	}

	dfs(photo_graph);

	return 0;
}

Node addNode(int vertex, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->next = head;

	return new;
}

Graph initGraph(int vertices) {

	int i;
	Graph graph = (Graph) malloc(sizeof(struct graph));

	graph->vertices = vertices;
	graph->edges = 0;
	graph->adjacency_list = (Node*) malloc(vertices * sizeof(struct node));

	for (i = 0; i < vertices; i++) {

		graph->adjacency_list[i] = NULL;
	}

	/*colour = (int*) malloc(vertices * sizeof(int));
	parent = (int*) malloc(vertices * sizeof(int));
	disc_time = (int*) malloc(vertices * sizeof(int));
	fin_time = (int*) malloc(vertices * sizeof(int)); */

	photo_info = (Photo*) malloc(vertices * sizeof(struct photo)); 

	return graph;
}

void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin] = addNode(destination, graph->adjacency_list[origin]);
	graph->edges++;
}

void dfs(Graph graph) {

	int vertices = graph->vertices;
	int i;
	int time;

	for (i = 0; i < vertices; i++) {

		photo_info[i].colour = WHITE;
		photo_info[i].parent = NIL;
	}

	time = 1;

	for (i = 0; i < vertices; i++) {

		if (photo_info[i].colour == WHITE) {

			dfsVisit(graph, i, &time);
		}
	}
}

void dfsVisit(Graph graph, int head, int* time) {

	int adj_idx;

	Node* list;
	Node temp;

	photo_info[head].colour = GREY;
	photo_info[head].disc_time = *time;

	printf("Photo %d is grey at time %d\n", head + 1, photo_info[head].disc_time);

	(*time)++;

	list = graph->adjacency_list;
	temp = list[head];

	while (temp != NULL) {

		adj_idx = temp->vertex;

		if (photo_info[adj_idx].colour == WHITE) {

			photo_info[adj_idx].parent = head;
			dfsVisit(graph, adj_idx, time);
		}

		temp = temp->next;
	}

	photo_info[head].colour = BLACK;
	photo_info[head].fin_time = *time;
	printf("Photo %d is black at time %d\n", head + 1, photo_info[head].fin_time);
	(*time)++;
}