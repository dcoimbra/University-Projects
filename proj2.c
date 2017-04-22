/* Standard library */
#include <stdio.h>
#include <stdlib.h>

/* Structures */
typedef struct node {
	
	int vertex;
	struct node* next;
}* Node;

typedef struct graph {
	
	int vertices;
	int edges;
	Node* adjacency_list;
}* Graph;

/* Global variables */
int** cost;

/* Prototype */
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination);
Node addNode(int vertex, Node head);
int getGraphCost(Graph graph);

/* Main program */
int main() {
	
	int vertices, airport_edges, road_edges;
	int airport_city, airport_cost, road_src, road_dest, road_cost;
	int i;

	Graph no_airports;
	Graph with_airports;

	if (scanf("%d", &vertices) == -1) {
		
		perror("scanf\n");
	}

	/* initializing cost matrix */
	cost = (int **) malloc((vertices + 1) * sizeof(int*));
	
	for (i = 0; i < (vertices + 1); i++) {

		cost[i] = (int *) malloc((vertices + 1) * sizeof(int));
	}

	/* initializing a graph that uses airports and one that doesn't */
	no_airports = initGraph(vertices);
	with_airports = initGraph(vertices + 1);

	if (scanf("%d", &airport_edges) == -1) {

		perror("scanf\n");
	}

	for (i = 0; i < airport_edges; i++) {

		if (scanf("%d %d", &airport_city, &airport_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(with_airports, (airport_city - 1), (vertices));
	
		cost[airport_city - 1][vertices] = airport_cost;
		cost[vertices][airport_city - 1] = airport_cost;
	}

	if (scanf("%d", &road_edges) == -1) {

		perror("scanf\n");
	}

	for (i = 0; i < road_edges; i++) {
		
		if (scanf("%d %d %d", &road_src, &road_dest, &road_cost) == -1) {

			perror("scanf\n");
		}

		addEdge(no_airports, (road_src - 1), (road_dest - 1));
		addEdge(with_airports, (road_src - 1), (road_dest - 1));

		cost[road_src - 1][road_dest - 1] = road_cost;
		cost[road_dest - 1][road_src - 1] = road_cost;
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

	return graph;
}

/* adds a directed edge to a given graph */
void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin] = addNode(destination, graph->adjacency_list[origin]);
	graph->adjacency_list[destination] = addNode(origin, graph->adjacency_list[destination]);

	graph->edges++;
}

/* adds a node to the beginning of a linked list */
Node addNode(int vertex, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->next = head;

	return new;
}