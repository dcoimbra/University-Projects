#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GREY 1
#define BLACK 2 
#define NIL -1

#define SUCCESS 1
#define FAILURE 0
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
int findEdge(Graph graph, int origin, int destination);
int uniqueOrder(Graph graph);
void dfs(Graph graph);
void dfsVisit(Graph graph, int head);


int* vertex_colour;
Node head_sorted = NULL;


int main()
{
	int vertices, edges;
	int i, u, v;
	Graph photo_graph;

	/* initializing sorted photos linked list */
	head_sorted = (Node) malloc(sizeof(struct node));
	head_sorted->next = NULL;

	if(scanf("%d %d", &vertices, &edges) == -1)
		perror("scanf\n");

	photo_graph = initGraph(vertices);

	for (i = 0; i < edges; i++) {

		if(scanf("%d %d", &u, &v) == -1)
			perror("scanf\n");
		addEdge(photo_graph, (u - 1), (v - 1));
	}

	dfs(photo_graph);

	/* cheking if topological sort is unique */
	if (!uniqueOrder(photo_graph)) {
		printf("Insuficiente\n");
		return 0;
	}

	/* printing sorted photo list (older to newer) */	
	if (head_sorted) {
		for (; (head_sorted->next)->next; head_sorted = head_sorted->next)
			printf("%d ", head_sorted->vertex + 1);

		printf("%d\n", head_sorted->vertex + 1);
	}
	return 0;
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

	vertex_colour = (int*) malloc(vertices * sizeof(int)); 

	return graph;
}

Node addNode(int vertex, Node head) {

	Node new = (Node) malloc(sizeof(struct node));

	new->vertex = vertex;
	new->next = head;

	return new;
}

void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin] = addNode(destination, graph->adjacency_list[origin]);
	graph->edges++;
}

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

void dfs(Graph graph) {

	int vertices = graph->vertices;
	int i;

	/* initializing colour info for every photo */
	for (i = 0; i < vertices; i++) {
		vertex_colour[i] = WHITE;
	}

	for (i = 0; i < vertices; i++) {
		if (vertex_colour[i] == WHITE)
			dfsVisit(graph, i);
	}
}

void dfsVisit(Graph graph, int head) {

	int adj_idx;
	Node* list;
	Node visitor;

	if (vertex_colour[head] == GREY) {
		printf("Incoerente\n");
		exit(0);
	}
	/* applying dfs algorithm */
	if (vertex_colour[head] == WHITE) {
		
		vertex_colour[head] = GREY;
		list = graph->adjacency_list;
		visitor = list[head];

		for(; visitor; visitor = visitor->next) {
			adj_idx = visitor->vertex;
			dfsVisit(graph, adj_idx);	
		}
				
		vertex_colour[head] = BLACK;	
		head_sorted = addNode(head, head_sorted);
	}
}

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