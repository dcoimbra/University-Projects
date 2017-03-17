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
void dfs(Graph graph);
void dfsVisit(Graph graph, int head);

typedef struct photo {

	int colour;
	int parent;
	int disc_time;
	int fin_time;
} Photo;

int* colour;

Node head_sorted = NULL;

int sort_possible = SUCCESS;

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

	colour = (int*) malloc(vertices * sizeof(int)); 

	return graph;
}

void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin] = addNode(destination, graph->adjacency_list[origin]);
	graph->edges++;
}

void dfs(Graph graph) {

	int vertices = graph->vertices;
	int i;

	int u, v;

	Node aux;

	head_sorted = (Node) malloc(sizeof(struct node));
	head_sorted->next = NULL;

	for (i = 0; i < vertices; i++) {

		colour[i] = WHITE;
	}

	for (i = 0; i < vertices; i++) {

		if (colour[i] == WHITE) {

			dfsVisit(graph, i);
		}
	}

	if (sort_possible) {

		aux = head_sorted;

		if (aux) {
			
			for (; (aux->next)->next; aux = aux->next) {
				
				u = (aux->vertex);
				v = (aux->next)->vertex;

				if (!(findEdge(graph, u, v))) {

					printf("Insuficiente\n");
					return; 
				}
			}
		}
			
		if (head_sorted) {
			
			for (; (head_sorted->next)->next; head_sorted = head_sorted->next) {
				
				printf("%d ", head_sorted->vertex + 1);
			}

			printf("%d\n", head_sorted->vertex + 1);
		}
	}
}

void dfsVisit(Graph graph, int head) {

	int adj_idx;

	Node* list;
	
	Node visitor;

	if (!sort_possible) {

		return;
	}

	if (colour[head] == GREY) {

		printf("Inconsistente\n");
		sort_possible = FAILURE;
		return;
	}

	if (colour[head] == WHITE) {
		
		colour[head] = GREY;

		list = graph->adjacency_list;
		visitor = list[head];

		while (visitor != NULL) {

			adj_idx = visitor->vertex;
				
			dfsVisit(graph, adj_idx);

			visitor = visitor->next;
		}
				
		colour[head] = BLACK;
	
		head_sorted = addNode(head, head_sorted);
	}
}

int findEdge(Graph graph, int origin, int destination) {

	Node* list = graph->adjacency_list;

	Node visitor = list[origin];

	while (visitor != NULL) {

		if (visitor->vertex == destination) {

			return SUCCESS;
		}

		visitor = visitor->next;
	}	

	return FAILURE;
}