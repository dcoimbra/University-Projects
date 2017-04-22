/* Standard library */
#include <stdio.h>
#include <stdlib.h>

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

/* Prototype */
Graph initGraph(int vertices);
void addEdge(Graph graph, int origin, int destination, int cost);
Node addNode(int vertex, int cost, Node head);
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