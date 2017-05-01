#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NIL -1
#define TRUE 1
#define FALSE 0

typedef struct node {

	int vertex;
	int cost;
	struct node* next;
}*Node;

typedef struct graph {

	int vertices;
	Node* adjacency_list;
}*Graph;

typedef struct heapnode {

	int vertex;
	int key;
	int parent;
	int visited;
	int position;
} Heapnode;


/* ------------ prototype: ------------ */
void runPrim(Graph graph, int cities, int airport_edges, int road_edges, int has_airports);
int* prim(Graph graph, int has_airports);

/* min heap functions: */
Heapnode** initMinHeap(int len);
void buildMinHeap(Heapnode** pQ, Heapnode* vInfo, int len);
void bubbleUp(Heapnode** pQ, Heapnode* vInfo, int len);
Heapnode* extractMin(Heapnode** pQ, Heapnode* vInfo, int len);
void minHeapify(Heapnode** pQ, Heapnode* vInfo, int k, int len);

/* graph related functions: */
Graph initGraph(int total_vertices);
void addEdge(Graph graph, int origin, int destination, int cost);
Node addNode(int vertex, int cost, Node head);

/* helper functions: */
void swap(Heapnode** queue, Heapnode* vInfo, int a, int b);
void cleanupGraph(Graph graph);
void cleanupList(Node head);

/* other functions: */
int* maxCost(Heapnode* vInfo, int len, int has_airports);


/* ------------ Code ------------ */
int main(int argc, char const *argv[]) {

	int i, vertices; 
	int road_edges, road_source, road_destination, road_cost;
	int airport_edges, airport_city, has_airports, airport_cost;
	Graph graph;

	if (scanf("%d", &vertices) == -1) { perror("scanf\n"); }
	
	graph = initGraph(vertices + 1);
	

	if (scanf("%d", &airport_edges) == -1) { perror("scanf\n"); }
	for (i = 0; i < airport_edges; i++) {
		
		if (scanf("%d %d", &airport_city, &airport_cost) == -1) { perror("scanf\n"); }
		addEdge(graph, (airport_city), (vertices + 1), (airport_cost * 10) + 1);
	}

	has_airports = airport_edges > 0 ? TRUE : FALSE; 

	if (scanf("%d", &road_edges) == -1) { perror("scanf\n"); }
	for (i = 0; i < road_edges; i++) {
		
		if (scanf("%d %d %d", &road_source, &road_destination, &road_cost) == -1) { perror("scanf\n"); }
		addEdge(graph, road_source, road_destination, road_cost * 10);
	}


	runPrim(graph, vertices, airport_edges, road_edges, has_airports);

	cleanupGraph(graph);
	return 0;
}

void runPrim(Graph graph, int cities, int airport_edges, int road_edges, int has_airports) {
	int total_cost, total_airports, total_roads;
	int* result = prim(graph, has_airports);
	total_cost = total_airports = total_roads = 0;

	if (result == NULL) {
		printf("Insuficiente\n");
		return;
	}

	total_cost = result[0];
	total_airports = result[1];
	total_roads = result[2];
	
	if (has_airports && airport_edges > 1 && (road_edges >= (cities - 1)) ) {

		if ((result = prim(graph, FALSE))) {

			if (result[0] == total_cost && result[1] < total_airports) {
				printf("%d\n%d %d\n", result[0], result[1], result[2]);
				return;
			}
		}
	}
	printf("%d\n%d %d\n", total_cost, total_airports, total_roads);
}

/* runs prim's algorythm to find an MST, given an undirectional weighted graph and 
a flag value to indicate if there are any airports. If flag is set to FALSE, it will ignore
all airport related edges and try to build an MST using the remaining edges.*/
int* prim(Graph graph, int has_airports) {
	int i, visited_vertices = 0;
	int len = graph->vertices;
	int total_vertices = graph->vertices;
	int* result;
	Heapnode** pQueue; /* pQueue(priority Queue): each pQueue[i] has a pointer to the heapnode    */
	Heapnode* vInfo;  /*  					 occupying the position i in this priority queue.  */
	Heapnode* u;
	Node v, head;

	vInfo = (Heapnode*)malloc((len + 1) * sizeof(struct heapnode));

	pQueue = initMinHeap(len);
	for (i = 1; i <= len; i++) {
		vInfo[i].vertex = i;
		vInfo[i].key = INT_MAX;
		vInfo[i].visited = FALSE;
		vInfo[i].parent = NIL;
		pQueue[i] = &(vInfo[i]);
		vInfo[i].position = i; /* vertex's current index in pQueue array */
	}

	buildMinHeap(pQueue, vInfo, len);
	
	vInfo[pQueue[1]->vertex].key = 0;
	
	/* if has_airports is FALSE, the algorythm should ignore all airport related edges, which is done by 
	ignoring the last vertex ("airport hub") and setting the number of total vertices accordingly. */
	if (!has_airports) {
		total_vertices--;
		len--;
	}

	while (len) {
		u = extractMin(pQueue, vInfo, len);

		/* if the minimum cost vertex has no parent, there are not enough edges to complete an MST. */
		if (u->vertex != 1 && u->parent == NIL) {
			free(vInfo);
			free(pQueue);
			return NULL;
		}

		visited_vertices++;
		if (visited_vertices == total_vertices) {
			break;
		}

		head = graph->adjacency_list[u->vertex];
		for (; head != NULL; head = head->next) {
			v = head;

			if ((!(vInfo[(v->vertex)].visited)) && (v->cost < vInfo[(v->vertex)].key)) {

				vInfo[(v->vertex)].parent = u->vertex;
				vInfo[(v->vertex)].key = v->cost;
				bubbleUp(pQueue, vInfo, vInfo[(v->vertex)].position);
			}
		}


		len--;
	}

	if (visited_vertices < total_vertices) {
		free(vInfo);
		free(pQueue);
		return NULL;
	}

	result = maxCost(vInfo, total_vertices, has_airports);
	free(vInfo);
	free(pQueue);
	return result;
}


/* ------------- other functions --------------------------- */
/* calculates the total cost of an MST, as well as the total number of airports and roads */
int* maxCost(Heapnode* vInfo, int len, int has_airports) {

	int i, airports_cost, total_airports, total_roads, total_cost;
	static int result[3];
	result[0] = result[1] = result[2] = 0;
	airports_cost = total_airports = total_roads = total_cost = 0;
	
	for (i = 1; i <= len; i++) {

		if (vInfo[i].visited && vInfo[i].vertex != 1) {

			if (has_airports && 
				(vInfo[i].parent == vInfo[len].vertex || vInfo[i].vertex == len) ) {

				total_airports++;
				airports_cost += (vInfo[i].key - 1);
			}
			else {
				total_roads++;
				total_cost += vInfo[i].key;
			}
		}
	}

	/* if there is only 1 airport, we only have 1 edge connecting a vertex to the "airport hub" but no real airway 
	between two (or more) cities. So this edge should be ignored, and both airport and associated cost are kept out. */
	if (total_airports > 1) {
		total_cost += airports_cost;
		result[1] = total_airports;
	}
	else
		result[1] = 0;

	result[0] = total_cost/10;
	result[2] = total_roads;
	return result;
}


/* ---------------heap functions --------------------------- */

Heapnode** initMinHeap(int len) {
	Heapnode** pQueue = (Heapnode**)malloc((len + 1) * sizeof(Heapnode*));
	return pQueue;
}


void buildMinHeap(Heapnode** pQueue, Heapnode* vInfo, int len) {
	int i;
	if (len > 0) {
		for (i = (len / 2) - 1; i > 0; i--) {
			minHeapify(pQueue, vInfo, i, len);
		}
	}
}


Heapnode* extractMin(Heapnode** pQueue, Heapnode* vInfo, int len) {
	Heapnode* min;

	swap(pQueue, vInfo, 1, len);
	min = &vInfo[pQueue[len]->vertex];
	min->visited = TRUE;
	minHeapify(pQueue, vInfo, 1, len);
	return min;
}


void minHeapify(Heapnode** pQ, Heapnode* vInfo, int k, int len) {
	int left = 2 * k;
	int right = 2 * k + 1;

	int smallest = k;
	if (left < len && vInfo[pQ[smallest]->vertex].key > vInfo[pQ[left]->vertex].key) {
		smallest = left;
	}

	if (right < len && vInfo[pQ[smallest]->vertex].key > vInfo[pQ[right]->vertex].key) {
		smallest = right;
	}

	if (smallest != k) {
		swap(pQ, vInfo, k, smallest);
		minHeapify(pQ, vInfo, smallest, len);
	}
}

void bubbleUp(Heapnode** pQ, Heapnode* vInfo, int len) {
	int pos = len;

	while ((pos / 2) > 0 && vInfo[pQ[pos / 2]->vertex].key > vInfo[pQ[pos]->vertex].key) {
		
		if (!(vInfo[pQ[pos]->vertex].visited)) {
			swap(pQ, vInfo, pos, pos / 2);
		}
		pos = pos / 2;
	}
}


/* ------------- helper functions --------------------------- */
/* Swaps positions of two nodes in an array and updates the 
new positions in the vertex info array (vInfo) */
void swap(Heapnode** queue, Heapnode* vInfo, int a, int b) {

	Heapnode* temp = queue[a];
	queue[a] = queue[b];
	queue[b] = temp;

	vInfo[queue[a]->vertex].position = a;
	vInfo[queue[b]->vertex].position = b;
}


/* ------------------ graph functions --------------------------- */
/* Initializes a graph with given vertex number */
Graph initGraph(int vertices) {

	int i;
	Graph graph = (Graph)malloc(sizeof(struct graph));

	graph->vertices = vertices;
	graph->adjacency_list = (Node*)malloc((vertices + 1) * sizeof(struct node));

	for (i = 1; i <= vertices; i++) {

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

	Node new = (Node)malloc(sizeof(struct node));

	new->vertex = vertex;
	new->cost = cost;
	new->next = head;

	return new;
}


/* --------------- Freeing memory functions------------------------ */
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