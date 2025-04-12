#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;   /// pentru simplitate, folosim int uri pt a numi restaurantel/locatiile
                /// ex: 1 - restaurantul 1 si tot asa    
    struct Node *next;
} NODE;

typedef struct graph {
    int numberVertices;
    int *visited;
    NODE **listAdjuncte; // lista de adiacenta
} 
GRAPH;

typedef struct stack{
    int top;
    int number_of_elements;
    int *array;
} STACK;

NODE *create_node(int data){
    NODE *newNode=malloc(sizeof(NODE));
    newNode->data=data;
    newNode->next=NULL;
    return newNode;
}

void add_edge(GRAPH *graph,int src,int dest) {
    NODE *newNode = create_node(dest); //de la dest la src
    newNode->next = graph->listAdjuncte[src];
    graph->listAdjuncte[src] = newNode;

    newNode = create_node(src); //de la src la dest
    newNode->next = graph->listAdjuncte[dest];
    graph->listAdjuncte[dest] = newNode;
}

GRAPH *create_graph(int numberVertices)
{
    int i;
    GRAPH *graph = malloc(sizeof(GRAPH));
    graph->numberVertices = numberVertices;
    graph->listAdjuncte = malloc(sizeof(NODE*) * numberVertices);
    graph->visited = malloc(sizeof(int) * numberVertices);

    for (int i = 0; i < numberVertices; i++){
        graph->listAdjuncte[i] = NULL;
        graph->visited[i] = 0;
    }  
    return graph;
}

STACK *create_stack(int number_of_elements) {
    STACK *stack = malloc(sizeof(STACK));

    stack->array = malloc(number_of_elements*sizeof(int));
    stack->top = -1;
    stack->number_of_elements = number_of_elements;

    return stack;
}

void push(int push_data, STACK *stack) {
    if (stack->top < stack->number_of_elements - 1){
        stack->top++;
        stack->array[stack->top] = push_data;
    }
}

void DFS(GRAPH *graph, STACK *stack,int vertix)
{
    NODE *adj_list = graph->listAdjuncte[vertix];
    NODE *aux = adj_list;
    graph->visited[vertix] = 1;

    printf("%d ", vertix);
    push(vertix, stack);

    while (aux != NULL){
        int aux_data = aux->data;
        if (graph->visited[aux_data] == 0)
            DFS(graph, stack, aux_data);
        aux = aux->next;
    }
}

void insert_edges(GRAPH *g,int edgeNumber,int numberVertices)
{
    int src, dest, i;
    printf("adauga %d munchii (de la 1 la %d)\n", edgeNumber, numberVertices);
    for (i = 0; i < edgeNumber; i++)
    {
        scanf("%d%d", &src, &dest);
        add_edge(g, src, dest);
    }
}

void wipe_visited_list(GRAPH *graph, int vertix_number)
{
    for (int i = 0; i < vertix_number; i++){
        graph->visited[i] = 0;
    }
}  

/*int can_reach(GRAPH *graph, int nodes_number, STACK *stack1, STACK *stack2, int node1_to_reach, int node2_to_reach)// 0 sau 1 daca poate fi sau nu ajuns
{
    int can_be_reached = 0;
    for (int i = 0; i < nodes_number; i++){ // aici i tine loc de numar adica de restaurant{for (int j = 0; j < 5; j++)
        for (int j = 0; j < nodes_number; j++){
            stack1->top = 0;
            DFS(graph, stack1, i);

            wipe_visited_list(graph, graph->numberVertices);

            stack2->top = 0;
            DFS(graph, stack2, j);

            if ((stack1->array[node1_to_reach - 1] == node2_to_reach) && (stack2->array[node2_to_reach - 1] == node1_to_reach)) {
                can_be_reached = 1;
                return can_be_reached;
            }

        }
    }
    return can_be_reached;
}*/
int can_reach_with_stack(GRAPH *graph, int from, int to, STACK *stack) {
    wipe_visited_list(graph, graph->numberVertices);

    stack->top = -1;
    push(from, stack);
    graph->visited[from] = 1;

    while (stack->top >= 0) {
        int current = stack->array[stack->top];
        stack->top--;

        if (current == to)
            return 1;

        NODE *adj = graph->listAdjuncte[current];
        while (adj != NULL) {
            if (!graph->visited[adj->data]) {
                push(adj->data, stack);
                graph->visited[adj->data] = 1;
            }
            adj = adj->next;
        }
    }

    return 0;
}


int main() {
    int nodes_number, edge_number;
    int src, dest;
    int vertix_1, vertix_2;
    int ans, i;

    printf("cate noduri are graful? ");
    scanf("%d", &nodes_number);

    printf("cate muchii are graful? ");
    scanf("%d", &edge_number);

    GRAPH *graph = create_graph(nodes_number);

    STACK *stack1 = create_stack(nodes_number);

    insert_edges(graph, edge_number, nodes_number);

    printf("Ce noduri vrei sa vezi daca sunt unite? (de la 0)\n");
    int vertix1, vertix2;
    scanf("%d%d", &vertix1, &vertix2);


    int can_be_reached = can_reach_with_stack(graph, vertix1, vertix2, stack1);

    if (can_be_reached == 1)
        printf("Se poate ajunge la acel nod\n");
    else printf("Nu se poate ajunge la nod\n");

    free(graph);
    free(stack1);

}