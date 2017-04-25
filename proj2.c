/* Standard library */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NIL -1
#define TRUE 1
#define FALSE 0

/* Structures */
typedef struct node {
	
	int vertex;
	int cost;
	struct node* next;
}* Node;

typedef struct graph {
	
	int vertices;
	Node* adjacency_list;
}* Graph;

typedef struct heapnode {

	int vertex;
	int key;
	int position;
} Heapnode;

/* Global variables */
Heapnode* priority;
int* position;
int* visited;

/* Prototype */
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination, int cost);
Node addNode(int vertex, int cost, Node head);
int prim(Graph g, int* key, int* parent, int has_airports);
void buildMinHeap(Heapnode* priority, int heap_length);
void minHeapify(Heapnode* priority, int heap_length, int i);
int extractMin(Heapnode* priority, int heap_length, int* visited, int total_vertices, int has_airports);
void decreaseKey(Heapnode* priority, int vertex, int newKey);
void cleanupGraph(Graph graph);
void cleanupList(Node head);


/* Main program */
int main() {
	
	int vertices, airport_edges, road_edges;
	int airport_city, airport_cost, road_src, road_dest, road_cost;
	int i;
	int cost_no_airports, cost_with_airports, least_cost;
	int airport_total, road_total, road_total_with_airports, road_total_no_airports;
	int sufficiency;

	int* parent_no_airports;
	int* key_no_airports;

	int* parent_with_airports;
	int* key_with_airports;

	Graph no_airports;
	Graph with_airports;

	if (scanf("%d", &vertices) == -1) {
		
		perror("scanf\n");
	}

	/* Allocating relevant arrays for both kinds of network.
		visited[u] - TRUE if u was extracted from the priority list
		parent[u] is the parent of u in the minimum spanning tree 
		key[u] is the minimum cost edge that connects u to the minimum spanning tree */ 

	/* initializing a graph that uses airports and one that doesn't */
	no_airports = initGraph(vertices);
	with_airports = initGraph(vertices + 1);

	if (scanf("%d", &airport_edges) == -1) {

		perror("scanf\n");
	}

	/* Adding airport edges to the graph that uses airports */
	for (i = 0; i < airport_edges; i++) {

		if (scanf("%d %d", &airport_city, &airport_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(with_airports, (airport_city - 1), (vertices), airport_cost);
	}

	if (scanf("%d", &road_edges) == -1) {

		perror("scanf\n");
	}

	/* Adding road edges to both graphs */
	for (i = 0; i < road_edges; i++) {
		
		if (scanf("%d %d %d", &road_src, &road_dest, &road_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(no_airports, (road_src - 1), (road_dest - 1), road_cost);
		addEdge(with_airports, (road_src - 1), (road_dest - 1), road_cost);
	}

	road_total_with_airports = 0;
	airport_total = 0;

	if (airport_edges) { 

		parent_with_airports = (int *) malloc((vertices + 1) * sizeof(int));
		key_with_airports = (int *) malloc((vertices + 1) * sizeof(int));

		/* Finding a minimum spanning tree for a road & airport network using Prim's algorithm */
		sufficiency = prim(with_airports, key_with_airports, parent_with_airports, TRUE);
		free(with_airports);

		if (sufficiency == NIL) {

			printf("Insuficiente\n");
			return 0;
		}

		cost_with_airports = 0;

		for (i = 0; i < (vertices + 1); i++) {

			cost_with_airports += key_with_airports[i];

			if (parent_with_airports[i] != NIL) {

				if ((i == vertices) || (parent_with_airports[i] == vertices)) {

					airport_total++;
				}
				
				else {

					road_total_with_airports++;
				}
			} 
		}

		free(key_with_airports);
		free(parent_with_airports);
	}

	else {

		cost_with_airports = INT_MAX;
	}

	parent_no_airports = (int *) malloc(vertices * sizeof(int));
	key_no_airports = (int *) malloc(vertices * sizeof(int));

	/* Finding a minimum spanning tree for a road-only network using Prim's algorithm */
	sufficiency = prim(no_airports, key_no_airports, parent_no_airports, FALSE);
	free(no_airports);

	road_total_no_airports = 0;

	if (sufficiency != NIL) { 

		cost_no_airports = 0;

		for (i = 0; i < vertices; i++) {

			cost_no_airports += key_no_airports[i];

			if (parent_no_airports[i] != NIL) {

				road_total_no_airports++;
			}
		}
	}

	else {

		cost_no_airports = INT_MAX;
	}

	free(key_no_airports);
	free(parent_no_airports);

	if (cost_no_airports <= cost_with_airports) {

		least_cost = cost_no_airports;
		airport_total = 0;
		road_total = road_total_no_airports;
	}

	else {

		least_cost = cost_with_airports;
		road_total = road_total_with_airports;
	}

	printf("%d\n", least_cost);
	printf("%d %d\n", airport_total, road_total);

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

	return graph;
}

/* adds a directed edge to a given graph */
void addEdge(Graph graph, int origin, int destination, int cost) {

	graph->adjacency_list[origin] = addNode(destination, cost, graph->adjacency_list[origin]);
	graph->adjacency_list[destination] = addNode(origin, cost, graph->adjacency_list[destination]);
}

/* adds a node to the beginning of a linked list */
Node addNode(int vertex, int cost, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->cost = cost;
	new->next = head;

	return new;
}

int prim(Graph g, int* key, int* parent, int has_airports) {

	int u;
	int i;
	int heap_length;
	int total_vertices;
	Node head;
	Node v;

	priority = (Heapnode*) malloc((g->vertices) * sizeof(Heapnode));
	position = (int *) malloc((g->vertices) * sizeof(int)); /* position of a vertex in the priority list */
	visited = (int *) malloc((g->vertices) * sizeof(int));

	for (u = 0; u < g->vertices; u++) {
		
		priority[u].vertex = u;
		priority[u].key = INT_MAX;
		position[u] = u;
		key[u] = INT_MAX;
		parent[u] = NIL;
		visited[u] = FALSE;
	}

	priority[0].key = 0;
	key[0] = 0;

	total_vertices = g->vertices;
	heap_length = g->vertices;

	buildMinHeap(priority, heap_length);	

	while (heap_length != 0) {
			
		u = extractMin(priority, heap_length, visited, total_vertices, has_airports);
		heap_length--;

		head = g->adjacency_list[u];

		for (; head != NULL; head = head->next) {

			v = head;
			
			if ((visited[v->vertex] == FALSE) && (v->cost < key[v->vertex])) {

				parent[v->vertex] = u;
				key[v->vertex] = v->cost;

				decreaseKey(priority, v->vertex, v->cost);
			} 
		}
	}

	/* Checking if algorithm produced a connected spanning tree.
		If there exists a vertex, besides the root, that has no parent,
		then both the original graph is disconnected
		and as such Prim cannot produce a minimum spanning tree. */
	for (i = 0; i < g->vertices; i++) {
			
		if ((parent[i] == NIL) && (i != 0)) {

			return NIL;
		}
	}

	free(priority);
	free(position);
	free(visited);

	return 1;
}

/* Here be heap operations */

void buildMinHeap(Heapnode* priority, int heap_length) {

	int start = ((heap_length) / 2) - 1;
	int i;

	for (i = start; i >= 0; i--) {

		minHeapify(priority, heap_length, i);
	}
}

void minHeapify(Heapnode* priority, int heap_length, int i) {

	int left, right;
	int smallest;

	Heapnode aux;

	left = ((i * 2) + 1);
	right = left + 1;

	if ((left < heap_length) && (priority[left].key < priority[i].key)) {

		smallest = left;
	}

	else {

		smallest = i;
	}

	if ((right < heap_length) && (priority[right].key < priority[smallest].key)) {

		smallest = right;
	}

	if (smallest != i) {

		position[priority[smallest].vertex] = i;
		position[priority[i].vertex] = smallest;

		aux = priority[i];
		priority[i] = priority[smallest];
		priority[smallest] = aux;

		minHeapify(priority, heap_length, smallest);
	}
}

int extractMin(Heapnode* priority, int heap_length, int* visited, int total_vertices, int has_airports) { 

	Heapnode min;
	Heapnode aux;

	/* if the head is the airport vertex,
	   checks if any of its children has the same key.
	   If one of them does, exchanges it with the head.
	   This is done to add as few airports as possible. */

	if (has_airports) {

		if (priority[0].vertex == (total_vertices - 1)) {

			if (priority[0].key == priority[1].key) {

				position[priority[0].vertex] = 1;
				position[priority[1].vertex] = 0;

				aux = priority[0];
				priority[0] = priority[1];
				priority[1] = aux;
			}

			else if (priority[0].key == priority[2].key) {

				position[priority[0].vertex] = 2;
				position[priority[2].vertex] = 0;

				aux = priority[0];
				priority[0] = priority[2];
				priority[2] = aux;
			}
		}
	} 

	min = priority[0];

	position[min.vertex] = heap_length - 1;
	position[priority[0].vertex] = 0;

	priority[0] = priority[heap_length - 1];

	minHeapify(priority, heap_length - 1, 0);

	visited[min.vertex] = TRUE;

	return min.vertex;
}

void decreaseKey(Heapnode* priority, int vertex, int newKey) {

	Heapnode aux;
	int index = position[vertex];
	int parent = (((index - 1) / 2));

	priority[index].key = newKey;

	while ((index > 0) && (priority[parent].key > priority[index].key)) {

		position[priority[index].vertex] = parent;
		position[priority[parent].vertex] = index;

		aux = priority[index];
		priority[index] = priority[parent];
		priority[parent] = aux;

		index = parent;
	}
}

/* Frees memory allocated for a graph */
void cleanupGraph(Graph graph) {

	int i;

	for (i = 0; i < graph->vertices; i++) {

		cleanupList(graph->adjacency_list[i]);
	}

	free(graph);
}

/* Frees memory allocated for a linked list */
void cleanupList(Node head) {

	Node temp; 

	while (head) {

		temp = head;
		head = head->next;
		free(temp);
	}
}