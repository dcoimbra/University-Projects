#include <stdio.h>
#include <stdlib.h>

typedef struct node {

	int data;
	struct node* next;
}* Photo;

typedef struct graph {

	int vertices;
	int edges;
	Photo* adjacency_list;
}* Graph;


Photo addPhoto(int photo_number, Photo adjacent);
Graph initGraph (int vertices_total);
void addEdge(Graph graph, int origin, int destination);

int main() {

	int vertices, edges;
	int i;
	int u, v;

	Graph photo_graph;

	Photo temp;

	scanf("%d %d", &vertices, &edges);

	photo_graph = initGraph(vertices);

	for (i = 0; i < edges; i++) {

	 	scanf("%d %d", &u, &v);
	 	addEdge(photo_graph, u, v);
	}

	for (i = 0; i < vertices; i++) {
		
		printf("photo no. %d\n", i + 1);
		 
		temp = photo_graph->adjacency_list[i];

		while (temp != NULL) {

			printf("%d\n", (temp->data + 1));
			temp = temp->next;
		}
	}

	return 0;
}


Photo addPhoto(int photo_number, Photo adjacent) {

	Photo new = (Photo) malloc(sizeof(struct node));
	
	new->data = photo_number;
	new->next = adjacent;

	return new;
}


Graph initGraph(int vertices_total) {

	int i;

	Graph graph = (Graph) malloc(sizeof(struct graph));
	
	graph->vertices = vertices_total;
	graph->edges = 0;
	graph->adjacency_list = (Photo*) malloc(vertices_total * sizeof(Photo));

	for (i = 0; i < vertices_total; i++) {

		graph->adjacency_list[i] = NULL;
	}

	return graph;
}


void addEdge(Graph graph, int origin, int destination) {

	graph->adjacency_list[origin - 1] = addPhoto((destination - 1), graph->adjacency_list[origin - 1]);
	graph->edges++;
}