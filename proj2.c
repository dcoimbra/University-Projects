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
	int edges;
	Node* adjacency_list;
}* Graph;

/* Global variables */
int* key;
int* pi;
int* visited;

/* Prototype */
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination, int cost);
Node addNode(int vertex, int cost, Node head);
void prim(Graph g, int r, Graph mst);
int extractMin(Graph g);

/* Main program */
int main() {
	
	int vertices, airport_edges, road_edges;
	int airport_city, airport_cost, road_src, road_dest, road_cost;
	int i;

	Graph no_airports;
	Graph with_airports;

	Graph mst_no_airports;
	Graph mst_with_airports;

	if (scanf("%d", &vertices) == -1) {
		
		perror("scanf\n");
	}

	/* initializing a graph that uses airports and one that doesn't */
	no_airports = initGraph(vertices);
	with_airports = initGraph(vertices + 1);

	/* initializing respective minimum spanning trees */
	mst_no_airports = initGraph(vertices);
	mst_with_airports = initGraph(vertices + 1);

	if (scanf("%d", &airport_edges) == -1) {

		perror("scanf\n");
	}

	for (i = 0; i < airport_edges; i++) {

		if (scanf("%d %d", &airport_city, &airport_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(with_airports, (airport_city - 1), (vertices), airport_cost);
	}

	if (scanf("%d", &road_edges) == -1) {

		perror("scanf\n");
	}

	for (i = 0; i < road_edges; i++) {
		
		if (scanf("%d %d %d", &road_src, &road_dest, &road_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(no_airports, (road_src - 1), (road_dest - 1), road_cost);
		addEdge(with_airports, (road_src - 1), (road_dest - 1), road_cost);
	}
	
	printf("No airports\n");

	prim(no_airports, 0, mst_no_airports);

		for (i = 0; i < no_airports->vertices; i++) {

		printf("%d's parent is %d\n", (i+1), pi[i]+1);
	}

	printf("With airports\n");

	prim(with_airports, 0, mst_with_airports);

	for (i = 0; i < with_airports->vertices; i++) {

		printf("%d's parent is %d\n", (i+1), pi[i]+1);
	}

	return 0;
}

/* Initializes a graph with given vertex number */
Graph initGraph(int vertices) {

	int i;
	Graph graph = (Graph) malloc(sizeof(struct graph));

	graph->vertices = vertices;
	graph->edges = 0;
	graph->adjacency_list = (Node*) malloc(vertices * sizeof(struct node));

	for (i = 0; i < vertices; i++) {

		graph->adjacency_list[i] = NULL;
	}

	key = (int *) malloc(vertices * sizeof(vertices));
	pi = (int *) malloc(vertices * sizeof(vertices));
	visited = (int *) malloc(vertices * sizeof(vertices));

	return graph;
}

/* adds a directed edge to a given graph */
void addEdge(Graph graph, int origin, int destination, int cost) {

	graph->adjacency_list[origin] = addNode(destination, cost, graph->adjacency_list[origin]);
	graph->adjacency_list[destination] = addNode(origin, cost, graph->adjacency_list[destination]);

	graph->edges++;
}

/* adds a node to the beginning of a linked list */
Node addNode(int vertex, int cost, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->cost = cost;
	new->next = head;

	return new;
}

void prim(Graph g, int r, Graph mst) {

	int u;
	int i;
	int length;
	Node head;
	Node v;
	
	for (u = 0; u < g->vertices; u++) {

		key[u] = INT_MAX;
		pi[u] = NIL;
		visited[u] = FALSE;
	}

	key[r] = 0;

	length = g->vertices;

	while (length != 0) {

		u = extractMin(g);
		length--;

		head = g->adjacency_list[u];

		for (; head != NULL; head = head->next) {

			v = head;
			
			if ((visited[v->vertex] == FALSE) && (v->cost < key[v->vertex])) {

				pi[v->vertex] = u;
				addEdge(mst, u, v->vertex, v->cost);
				
				key[v->vertex] = v->cost;
			} 
		}
	}

	for (i = 0; i < g->vertices; i++) {
			
		if ((pi[i] == NIL) && (i != r)) {

			printf("Insuficiente\n");
			break;
		}
	}
}

int extractMin(Graph g) {

	int i;
	int min_cost, min_cost_idx;

	min_cost_idx = 0;
	min_cost = INT_MAX;

	for (i = 0; i < g->vertices; i++) {

		if ((visited[i] == FALSE) && (key[i] < min_cost)) {

			min_cost = key[i];
			min_cost_idx = i;
		}
	}

	visited[min_cost_idx] = TRUE;

	return min_cost_idx;
}