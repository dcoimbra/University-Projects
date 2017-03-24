/* Standard library*/
#include <stdio.h>
#include <stdlib.h>

/* Constants */
#define WHITE 0
#define GREY 1
#define BLACK 2 
#define NIL -1

#define SUCCESS 1
#define FAILURE 0

/* Structures */
typedef struct node {
	
	int vertex;
	struct node* next;
}* Node;

typedef struct graph {
	
	int vertices;
	Node* adjacency_list;
}* Graph;

/* Protoype */
Node addNode(int vertex, Node head);
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination);
int findEdge(Graph graph, int origin, int destination);
int uniqueOrder(Graph graph);
void dfs(Graph graph);
void dfsVisit(Graph graph, int head);
void printSortedList();
void cleanupGraph(Graph graph);
void cleanupList(Node head);

/* Global variables */
int* vertex_colour;
Node head_sorted = NULL;

/* Main program */
int main() {
	
	int vertices, edges;
	int i, u, v;
	
	Graph photo_graph;

	/* initializing sorted photos linked list */
	head_sorted = (Node) malloc(sizeof(struct node));
	head_sorted->next = NULL;

	if (scanf("%d %d", &vertices, &edges) == -1) {
		
		perror("scanf\n");
	}

	photo_graph = initGraph(vertices);

	for (i = 0; i < edges; i++) {

		if (scanf("%d %d", &u, &v) == -1) {

			perror("scanf\n");
		}

		addEdge(photo_graph, (u - 1), (v - 1));
	}

	dfs(photo_graph);

	/* checking if topological sort is unique */
	if (!uniqueOrder(photo_graph)) {
		
		printf("Insuficiente\n");
		return 0;
	}

	/* printing sorted photo list (older to newer) */	
	printSortedList();
	
	/* freeing all memory*/
	free(vertex_colour);
	cleanupList(head_sorted);
	cleanupGraph(photo_graph);

	return 0;
}

/* Initializes a graph with given vertex number */
Graph initGraph(int vertices) {

	int i;
	Graph graph = (Graph) malloc(sizeof(struct graph));

	graph->vertices = vertices;
	graph->adjacency_list = (Node*) malloc(vertices * sizeof(struct node));

	for (i = 0; i < vertices; i++) {

		graph->adjacency_list[i] = NULL;
	}

	/* allocating memory for DFS colour array */
	vertex_colour = (int*) malloc(vertices * sizeof(int)); 

	return graph;
}

/* adds a node to the beginning of a linked list */
Node addNode(int vertex, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->next = head;

	return new;
}

/* adds a directed edge to a given graph */
void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin] = addNode(destination, graph->adjacency_list[origin]);
}

/* finds a given directed edge in a given graph */
int findEdge(Graph graph, int origin, int destination) {

	Node* list = graph->adjacency_list;
	Node visitor = list[origin];

	for(; visitor; visitor = visitor->next) {
		
		if (visitor->vertex == destination) {
			
			return SUCCESS;
		}
	}

	return FAILURE;
}

/* searches all the vertices in graph, depth-first. */ 
void dfs(Graph graph) {

	int vertices = graph->vertices;
	int i;

	/* initializing colour info for every photo */
	for (i = 0; i < vertices; i++) {
		
		vertex_colour[i] = WHITE;
	}

	for (i = 0; i < vertices; i++) {
		
		if (vertex_colour[i] == WHITE) {
			
			dfsVisit(graph, i);
		}
	}
}

void dfsVisit(Graph graph, int head) {

	int adj_idx;
	
	Node* list;
	Node visitor;

	/* finding incoherences */
	if (vertex_colour[head] == GREY) {
		
		printf("Incoerente\n");
		exit(0);
	}
	
	/* applying DFS algorithm */
	if (vertex_colour[head] == WHITE) {
		
		vertex_colour[head] = GREY;
		list = graph->adjacency_list;
		visitor = list[head];

		for(; visitor; visitor = visitor->next) {
			
			adj_idx = visitor->vertex;
			dfsVisit(graph, adj_idx);	
		}
		
		/* adding closed vertex to the beginning of the sorted list (and marking it as closed) */
		vertex_colour[head] = BLACK;	
		head_sorted = addNode(head, head_sorted);
	}
}

/* Checks if the topological order is unique for a given graph.
   If the topological order is a Hamilton path in the graph, the 
   order is unique */
int uniqueOrder(Graph graph) {

	Node aux = head_sorted;
	int u, v;

	if (aux) {
		
		for (; (aux->next)->next; aux = aux->next) {
			
			u = (aux->vertex);
			v = (aux->next)->vertex;
			
			if (!(findEdge(graph, u, v))) {
				
				return FAILURE; 
			}
		}
	}
	
	return SUCCESS;
}

/* Prints all the values, topologically ordered */
void printSortedList() {

	if (head_sorted) {
		
		for (; (head_sorted->next)->next; head_sorted = head_sorted->next) {
			
			printf("%d ", head_sorted->vertex + 1);
		}
		
		printf("%d\n", head_sorted->vertex + 1);
	}
}

/* Frees all memory allocated for a graph */
void cleanupGraph(Graph graph) {

	int i;

	for (i = 0; i < graph->vertices; i++) {

		cleanupList(graph->adjacency_list[i]);
	}

	free(graph);
}

/* Frees all memory allocated for a linked list */
void cleanupList(Node head) {

	Node temp; 

	while (head) {

		temp = head;

		head = head->next;

		free(temp);
	}
}