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

	head_sorted = (Node) malloc(sizeof(struct node));
	head_sorted->next = NULL;

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
	
	if (sort_possible) {
		
		if (head_sorted) {
		
			for(; (head_sorted->next)->next; head_sorted = head_sorted->next) {
			
				printf("%d ", head_sorted->vertex);
			}

			printf("%d\n", head_sorted->vertex);
		}
	}
}

void dfsVisit(Graph graph, int head, int* time) {

	int adj_idx;

	Node* list;
	
	Node visitor;

	if (!sort_possible) {

		return;
	}

	if (photo_info[head].colour == GREY) {

		printf("Inconsistente\n");
		sort_possible = FAILURE;
		return;
	}

	if (photo_info[head].colour == WHITE) {
		
		photo_info[head].colour = GREY;
		photo_info[head].disc_time = *time;
		(*time)++;

		list = graph->adjacency_list;
		visitor = list[head];

		if (visitor) {

			while (visitor != NULL) {

				adj_idx = visitor->vertex;

				photo_info[adj_idx].parent = head;
				
				dfsVisit(graph, adj_idx, time);

				visitor = visitor->next;
			}
		}
		
		photo_info[head].colour = BLACK;
		photo_info[head].fin_time = *time;

		if (head_sorted) {
			
			head_sorted = addNode(head + 1, head_sorted);
		}
		
		(*time)++;
	}
}	