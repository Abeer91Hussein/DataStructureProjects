/*Name :Abeer Salah Hussien
ID:1220425
sec:2*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#define MAX_NODES 100

typedef struct Edge* Edge;
struct Edge {
    char from[10];
    char to[10];
    int latency;
};

typedef struct Graph* Graph;
struct Graph {
    char routers[MAX_NODES][10];
    int adjacencyMatrix[MAX_NODES][MAX_NODES];
    int numNodes;
    int numEdges;
    Edge edges[MAX_NODES * MAX_NODES]; //array of Edge pointers
};

typedef struct QueueNode QueueNode;
struct QueueNode {
    int data;
    struct QueueNode* next;
};

typedef struct Queue Queue;
 struct Queue {
    QueueNode* front;
    QueueNode* rear;
};

typedef struct cell* cell;
struct cell {
    int nodeId;
    int distance;
};

typedef struct minHeap *minHeap;
struct minHeap {
    cell* heapArray;
    int size;
    int max_size;
};

void minHeapify(int pos, minHeap H);
void bfs(Graph graph, int sourceIndex, int destIndex, int* dist, int* prev);
void dijkstra(Graph graph, int sourceIndex, int destIndex, int* dist, int* prev);


int findRouterIndex(Graph graph, char *router) {// Find router index
    for (int i = 0; i < graph->numNodes; i++) {
        if (strcmp(graph->routers[i], router) == 0) {//comparing
            return i;
        }
    }
    return -1;//there are not node (router)
}

void addEdge(Graph graph, char *from, char *to, int latency) {//add edge function
    int fromIndex = findRouterIndex(graph, from);
    int toIndex = findRouterIndex(graph, to);

    if (fromIndex == -1) {// Add from to the graph if it doesn't exist
        if (graph->numNodes < MAX_NODES) {
            strcpy(graph->routers[graph->numNodes], from);
            fromIndex = graph->numNodes++;
        } else {
            printf("can not add the router %s\n",from);
            return;
        }
    }
    if (toIndex == -1) {
        if (graph->numNodes < MAX_NODES) {
            strcpy(graph->routers[graph->numNodes], to);
            toIndex = graph->numNodes++;
        } else {
            printf("can not add the router %s\n",to);
            return;
        }
    }
    if (graph->adjacencyMatrix[fromIndex][toIndex] == INT_MAX) {
        graph->adjacencyMatrix[fromIndex][toIndex] = latency;
    }
    Edge newEdge = (Edge)malloc(sizeof(struct Edge));
    if (newEdge == NULL) {
        printf("out of memory!\n");
        return;
    }
    strcpy(newEdge->from, from);
    strcpy(newEdge->to, to);
    newEdge->latency = latency;
    graph->edges[graph->numEdges++] = newEdge;//add edge and incremant the array size
}

void loadGraph(Graph graph) {//load graph from file
    FILE *file = fopen("routers.txt", "r");
    if (!file) {
        printf("could not open file !\n");
        return;
    }

    char line[256],from[10], to[10];
    int latency;
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
           if(i==j){
            graph->adjacencyMatrix[i][j]=0;//the dis from the node to itself =0
           }
           else
            graph->adjacencyMatrix[i][j]=INT_MAX;
        }
    }
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^-]-%[^-]-%d", from, to, &latency) == 3) {
            addEdge(graph, from, to, latency);//indirect graph
            addEdge(graph,to,from,latency);
        }
    }
    fclose(file);
}

void printGraph(Graph graph) {//print graph
    printf("Adjacency Matrix:\n\t");
    for (int i = 0; i < graph->numNodes; i++) {
        printf("%s\t", graph->routers[i]);
    }
    printf("\n");
    for (int i = 0; i < graph->numNodes; i++) {
        printf("%s\t", graph->routers[i]);//print the routers
        for (int j = 0; j < graph->numNodes; j++) {//print the wight
            if (graph->adjacencyMatrix[i][j] == INT_MAX) {
                printf("--\t");
            }
            else {
                printf("%d\t", graph->adjacencyMatrix[i][j]);
            }
        }
        printf("\n");
    }
}

Queue* createQueue() {// function to create the queue
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

int isQueueEmpty(Queue *q){//function check if the queue is empty
return q->front==NULL;
}

void enqueue(Queue* q, int value) {//function to enqueue an element into the queue
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if(newNode==NULL){
        printf("out of memory\n");
        return ;
    }
    newNode->data = value;
    newNode->next = NULL;
    if (q->rear == NULL) {  //If the queue is empty
        q->front = newNode;
        q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue* q) {//function to dequeue an element from the queue
    if (q->front == NULL) {  // If the queue is empty
        printf("queue is empty\n");
        return INT_MIN;
    }
    int data=q->front->data;
    QueueNode*temp=q->front;
    q->front = q->front->next;
    if (q->front == NULL) {  // If the queue become empty
        q->rear = NULL;
    }
    free(temp);
    return data;
}

void bfs(Graph graph, int sourceIndex, int destIndex, int* dist, int* prev) {//Breadth First Search Function
    bool visited[MAX_NODES] = {false};
    Queue* queue = createQueue();

    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    visited[sourceIndex] = true;
    dist[sourceIndex] = 0;
    enqueue(queue, sourceIndex);

    while (!isQueueEmpty(queue)) {
        int u = dequeue(queue);
        for (int v = 0; v < graph->numNodes; v++) {
            if (graph->adjacencyMatrix[u][v] != INT_MAX && !visited[v]) {
                visited[v] = true;
                prev[v] = u;
                dist[v] = dist[u] + graph->adjacencyMatrix[u][v];
                enqueue(queue, v);

                if (v == destIndex) break;  //break if destination is found
            }
        }
    }
    free(queue);
    if (!visited[destIndex]) {
        printf("no path found using BFS\n");
        return;
    }
    int path[MAX_NODES], pathLength = 0, current = destIndex;
    while (current != -1) {
        path[pathLength++] = current;
        current = prev[current];
    }
    int totalCost = dist[destIndex];
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%s%s", graph->routers[path[i]], (i > 0) ? " -> " : "");
    }
    printf(" with a total cost of %d\n", totalCost);
}

minHeap CreateHeap(int maxSize) {//function to create the minHeap
    minHeap H = (minHeap)malloc(sizeof(struct minHeap));
    if (H == NULL) {
        printf("out of memory\n");
        return NULL;
    }

    H->heapArray = (cell*)malloc(sizeof(cell) * (maxSize + 1)); // +1 for 1 based indexing
    if (H->heapArray == NULL) {
        printf("out of memory of the heap array\n");
        free(H);
        return NULL;
    }
    H->max_size = maxSize;
    H->size = 0;
    return H;
}

int parent(int pos) {//function return the parent in the node in the minHeap
    return pos / 2;
}

int leftChild(int pos) {//function return the left child in the node in the minHeap
    return 2 * pos;
}

int rightChild(int pos) {//function return the right child in the node in the minHeap
    return (2 * pos) + 1;
}

int isLeaf(int pos, minHeap H) {//function check if the node (router) is leaf or not
    return (leftChild(pos) > H->size && rightChild(pos) > H->size);
}

void swap(int pos1, int pos2, minHeap H) {//swap function
    cell temp = H->heapArray[pos1];
    H->heapArray[pos1] = H->heapArray[pos2];
    H->heapArray[pos2] = temp;
}

void insertToHeap(int nodeId, int distance, minHeap H) {//insert to the minHeap
    if (H->size >= H->max_size) {
        printf("heap full can not insert node %d with distance %d\n", nodeId, distance);
        return;
    }
    int current = ++H->size; //increment size and insert at the new position
    H->heapArray[current] = (cell)malloc(sizeof(struct cell)*current);//malloc for array
    if (H->heapArray[current] == NULL) {
        printf("out of memory \n");
        return;
    }
    H->heapArray[current]->nodeId = nodeId;
    H->heapArray[current]->distance = distance;
    while (current > 1 && H->heapArray[current]->distance < H->heapArray[parent(current)]->distance) {//insert to the less value
        swap(current, parent(current), H);
        current = parent(current);
    }
}

void minHeapify(int pos, minHeap H) {//main function in the minHeap
    while (!isLeaf(pos, H)) {
        int left = leftChild(pos);
        int right = rightChild(pos);
        int smallest = pos;

        if (left <= H->size && H->heapArray[left]->distance < H->heapArray[smallest]->distance) {
            smallest = left;
        }
        if (right <= H->size && H->heapArray[right]->distance < H->heapArray[smallest]->distance) {
            smallest = right;
        }
        if (smallest != pos) {
            swap(pos, smallest, H);
            pos = smallest;
        } else {
            break;
        }
    }
}

cell deleteFromHeap(minHeap H) {//delete function in the minHeap
    if (H->size == 0) {
        printf("Heap is empty\n");
        return 0;
    }
    cell minNode = H->heapArray[1];
    H->heapArray[1] = H->heapArray[H->size--]; // Decrease the size after moving the last element to root
    minHeapify(1, H);//fit the heap

    return minNode; // return the removed element
}

void printHeap(minHeap H) {//print minHeap function
    if (H->size == 0) {
        printf("Heap is empty.\n");
        return;
    }
    printf("Heap (nodeId, distance):\n");
    for (int i = 1; i <= H->size; i++) {
        printf("%d, %d ", H->heapArray[i]->nodeId, H->heapArray[i]->distance);
    }
    printf("\n");
}

void dijkstra(Graph graph, int sourceIndex, int destIndex, int* dist, int* prev) {//Dijkstra Function
    bool visited[MAX_NODES] = {false};
    minHeap heap = CreateHeap(graph->numNodes);

    for (int i = 0; i < graph->numNodes; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[sourceIndex] = 0;

    insertToHeap(sourceIndex, 0, heap);
    while (heap->size > 0) {
        cell u = deleteFromHeap(heap);
        int uIndex = u->nodeId;
        if (visited[uIndex]) continue;
        visited[uIndex] = true;

        for (int v = 0; v < graph->numNodes; v++) {
            if (graph->adjacencyMatrix[uIndex][v] != INT_MAX && !visited[v]) {
                int newDist = dist[uIndex] + graph->adjacencyMatrix[uIndex][v];
                if (newDist < dist[v]) {//minHeap
                    dist[v] = newDist;
                    prev[v] = uIndex;
                    insertToHeap(v, dist[v], heap);
                }
            }
        }
    }
    free(heap);
    if (dist[destIndex] == INT_MAX) {
        printf("No path found using Dijkstra's algorithm.\n");
    }
        int path[MAX_NODES], pathLength = 0, current = destIndex;
    while (current != -1) {
        path[pathLength++] = current;
        current = prev[current];
    }
    int totalCost = dist[destIndex];
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%s%s", graph->routers[path[i]], (i > 0) ? " -> " : "");
    }
    printf(" with a total cost of %d\n", totalCost);
}
void topsort( Graph g )
{
    int counter;
    Vertex w, v;
    for( counter = 0; counter < NUM_VERTICES; counter++ )
    {
        v = find_new_vertex_of_indegree_zero( );
        if( v == null )
        {
            printf("Graph has a cycle!\n");
            break;
        }
        topNum[v] = counter;
        for each Vertex w adjacent to v
            indegree[w]--;
    }
}


int main() {//main function
    Graph graph = (Graph)malloc(sizeof(struct Graph));
    if (graph == NULL) {
        printf("out of memory!\n");
        return -1;
    }
    topsort(graph);
    printGraph(graph);

    int sourceIndex = -1, destIndex = -1;
    int dist[MAX_NODES], prev[MAX_NODES];
    bool calculated = false;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Load routers: loads the file and construct the graph\n");
        printf("2. Enter source: read the source router\n");
        printf("3. Enter destination: print the full route of the shortest path including the total shortest cost for both algorithms (Dijkstra and BFS)\n");
        printf("4. Exit: prints the information of step 3 to a file called shortest_distance.txt and exits the program\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                loadGraph(graph);
                if (graph) {
                    printf("the graph successfully loaded \n");
                } else {
                    printf(" can not load the graph\n");
                }
                break;
            }

           case 2: {
    if (graph==NULL) {
        printf("the graph is not loaded yet \n");
        break;
    }
    char source[10];
    while (1) {
        printf("enter the source router: ");
        scanf(" %s", source);
        sourceIndex = findRouterIndex(graph, source);
        if (sourceIndex == -1) {
            printf("router %s not found,try again\n", source);
        } else {
            printf("the source router is %s\n", graph->routers[sourceIndex]);
            break;
        }
    }
    break;
}

case 3: {
    if (graph==NULL) {
        printf("the graph is not loaded yet\n");
        break;
    }
    if (sourceIndex == -1) {
        printf("the source router not set yet \n");
        break;
    }
    char destination[10];
    while (1) {
        printf("enter destination router: ");
        scanf(" %s", destination);
        destIndex = findRouterIndex(graph, destination);
        if (destIndex == -1) {
            printf("the router %s not found,Try again\n", destination);
        } else {
            printf("the destination router is %s\n", graph->routers[destIndex]);
            break;
        }
    }

    printf("Dijkstra's Algorithm Path:\n");
    dijkstra(graph, sourceIndex, destIndex, &dist, &prev);

    int bfsDist[MAX_NODES], bfsPrev[MAX_NODES], bfsTotalCost;
    printf("BFS Algorithm Path:\n");
  bfs(graph, sourceIndex, destIndex, &bfsDist, &bfsPrev);

    FILE* outputFile = fopen("shortest_distance.txt", "w");
    if (outputFile == NULL) {
        printf("can't open the file\n");
        break;
    }
    fprintf(outputFile, "Dijkstra Path:\n");// Writing Dijkstra algorithm result to the file
    if (dist[destIndex] == INT_MAX) {
        fprintf(outputFile, "No path found using Dijkstra's algorithm.\n");
    }
    else {
        int path[MAX_NODES], pathLength = 0, current = destIndex;
        while (current != -1) {
            path[pathLength++] = current;
            current = prev[current];
        }
      //  fprintf(outputFile, "Dijkstra: ");
        for (int i = pathLength - 1; i >= 0; i--) {
            fprintf(outputFile, "%s%s", graph->routers[path[i]], (i > 0) ? " -> " : "");
        }
        fprintf(outputFile, " with a total cost of %d\n", dist[destIndex]);
    }

fprintf(outputFile, "BFS Path:\n");// Writing BFS result to the file
if (bfsDist[destIndex] == INT_MAX) {
    fprintf(outputFile, "No path found using BFS.\n");
}
else {
    int bfsPath[MAX_NODES], bfsPathLength = 0, bfsCurrent = destIndex;
    while (bfsCurrent != -1) {
        bfsPath[bfsPathLength++] = bfsCurrent;
        bfsCurrent = bfsPrev[bfsCurrent];
    }
    for (int i = bfsPathLength - 1; i >= 0; i--) {
        fprintf(outputFile, "%s%s", graph->routers[bfsPath[i]], (i > 0) ? " -> " : "");
    }
    fprintf(outputFile, " with a total cost of %d\n",bfsDist[destIndex]);
}
    fclose(outputFile);
    calculated = true;
    break;
}


case 4: { // Exit
    if (!graph) {
        printf("the graph is not loaded\n");
        break;
    }

    if (!calculated) {
        printf("shortest paths not calculated yet\n");
        break;
    }
    printf("results written to the file ,Exit\n");
    exit(0);
}
            default: {
                printf("Invalid choice. Please select a valid option.\n");
            }
        }
    }
    free(graph);
    return 0;
}
