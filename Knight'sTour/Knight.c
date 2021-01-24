/*	Knight's Tour
		De tal forma que para cualquier casilla arbitraria, recorra todas las demás casillas, y regrese de nuevo a la casilla original,
		sin pasar 2 veces por una misma casilla en el circuito (exceptuando por la casilla de partida).
		Pueden resolverlo de forma iterativa ó recursiva y de preferencia usando estructuras de datos
		Reference: https://en.wikipedia.org/wiki/Knight%27s_tour

		By Peduzzi Acevedo Gustavo Alain
				20/01/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#define RST_T	"\x1b[0m"
#define ERR_T	"\x1b[31m"
#define SHOW_T	"\x1b[34m"
#define SCND_T	"\x1b[32m"

int N = 5;

typedef struct box
{
	int x;
	int y;
	int content;
} s_box, * Box;

typedef struct node
{
	Box box;
	int sNodes;
	struct node** nodes;
} s_node, * Node;

void delay(int number_of_seconds);
int tour(Node* checkerboard, Node currentNode);
int isNear(Node origin, Node wanted);
int posibleMovements(Node node, Node toExcluse);
void nodesLinker(Node* nodes);
void printSolutionwTittle(Node* checkerboard, char* tittle, int type);
void printAllNode(Node node);
void printAllBox(Box box);

int initBox(Box* box);
int setContent(Box box, int data);
int destroyBox(Box box);
int initNode(Node* node);
int destroyNode(Node node);
int linkNode(Node node, Node nodeToLink);
int getNodeLinked(Node* nodeWanted, Node node, int idx);

int main()
{
	Node* checkerboard = NULL;
	Box startPos = NULL;
	initBox(&startPos);

	while (1)
	{
		printf("Enter board size: ");
		scanf("%i", &N);

		if (N < 5)
			printf("%sMinimum checker board size is 5 %s\n", ERR_T, RST_T);
		else if (N > 8)
			printf("%sMaximum checker size is 8 for this PC Health	%s\n", ERR_T, RST_T);
		else
			break;
	}

	while (1)
	{
		printf("Start position (x y): ");
		scanf("%i %i", &(startPos->x), &(startPos->y));

		if (startPos->x > N || startPos->y > N || startPos->x < 0 || startPos->y < 0)
			printf("%sOnly natural numbers less than %d %s\n", ERR_T, N, RST_T);
		else
			break;
	}
	//printf("(%d, %d) \n", startPos->x, startPos->y);

	int **sol = (int**) calloc(N, sizeof(int*));

	for (int i = 0; i < N; i++)
	{
		*(sol+i) = (int*) calloc(N, sizeof(int));
	}
	checkerboard = (Node*)calloc(N * N, sizeof(Node));

	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		initNode(checkerboard + i + (N * j));
		(*(checkerboard + i + (N * j)))->box->x = i;
		(*(checkerboard + i + (N * j)))->box->y = j;

		if (i + 1 == N)
		{
			i = -1;
			j++;
		}
	}

	printSolutionwTittle(checkerboard, "Initation", 1);

	nodesLinker(checkerboard, N, N);

	(*checkerboard + startPos->x + (N * startPos->y))->box->content = 0;
	if (tour(checkerboard, (*checkerboard + startPos->x + (N * startPos->y))))
		printf("Solution does not exist");
	else
		printSolutionwTittle(checkerboard, "Result", 24);

	return 0;
}

int initBox(Box* box)
{
	*box = (Box)malloc(sizeof(s_box));
	if (!*box)
		return 1;

	(*box)->content = -1;
	(*box)->x = -1;
	(*box)->y = -1;

	return 0;
}

int setContent(Box box, int data)
{
	if (!box)
		return 1;

	box->content = data;

	return 0;
}

int destroyBox(Box box)
{
	if (!box)
		return 1;

	free(box);

	return 0;
}

int initNode(Node* node)
{
	*node = (Node)malloc(sizeof(s_node));

	if (!*node)
		return 1;

	(*node)->box = NULL;
	(*node)->nodes = NULL;
	(*node)->sNodes = 0;

	return !initBox(&((*node)->box))
		? -1 : 0;
}

int destroyNode(Node node)
{
	if (!node)
		return 1;

	destroyBox(node->box);
	node->box = NULL;
	node->nodes = NULL;
	free(node->nodes);	// Just destroy list, no its elements
	free(node);

	return 0;
}

int getNodeLinked(Node* nodeWanted, Node node, int idx)
{
	if (!node)
		return 1;

	*nodeWanted = *(node->nodes + idx);

	return 0;
}

int linkNode(Node node, Node nodeToLink)
{
	if (node==NULL)
		return 2;
	if (nodeToLink == NULL)
		return 3;


	node->nodes = (Node*) realloc(node->nodes, (node->sNodes+1)*sizeof(Node));
	if (!node->nodes)
	{
		//printf("%c%d !<- %c%d", node->box->x + 'A', node->box->y, nodeToLink->box->x + 'A', nodeToLink->box->y);
		return 1;
	}
	*(node->nodes + node->sNodes) = nodeToLink;
	node->sNodes++;

	return 0;
}

void nodesLinker(Node* nodes)
{
	const s_box relativeMovements[8] = {{ 1, -2, 0}, 
										{ 2, -1, 0},
										{ 2,  1, 0}, 
										{ 1,  2, 0}, 
										{-1,  2, 0}, 
										{-2,  1, 0}, 
										{-2, -1, 0}, 
										{-1, -2, 0}};
	Box nextBox = NULL;
	initBox(&nextBox);

	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node currentNode = *(nodes+i+(N*j));

		for (int k = 0; k < 8; k++)
		{
			nextBox->x = (i % N) + (relativeMovements + k)->x;
			nextBox->y = (j % N) + (relativeMovements + k)->y;

			//printf("%d%d\n", nextBox->x, nextBox->y);
			if (nextBox->x >= 0 && nextBox->x < N && nextBox->y >= 0 && nextBox->y < N)
				linkNode(currentNode, *(nodes + nextBox->x + (N * nextBox->y)));
		}

		if (i + 1 == N)
		{
			i = -1;
			j++;
		}
	}

	printSolutionwTittle(nodes, "Movements", 0);
}

int posibleMovements(Node node, Node toExcluse)
{
	int counter = 0;
	for (int i = 0; i < node->sNodes; i++)
	{
		Node curNode = *(node->nodes + i);
		if (curNode != toExcluse && curNode->box->content == -1)
			counter++;
	}
	return counter;
}

int isNear(Node origin, Node wanted)
{

	return 0;
}

int tour(Node* checkerboard, Node currentNode)
{
	/*if (currentNode->box->content+1 == N * N)
		return 1;
	/*	 PARA IMPRIMIR EL PRCESO DEL PROGRAMA
	system("cls");
	delay(0.5);
	printSolutionwTittle(checkerboard, "Steps", currentNode->box->content);
	*/
	int idxSmallest = -1;
	for (int i = 0, aux = 0, smallestMovements = 8; i < currentNode->sNodes; i++)	// Recorrido de todos los nodos vincuados
		if ((*(currentNode->nodes + i))->box->content < 0)		// Verificación que no es un numero ya utilizado
		{
			aux = posibleMovements(*(currentNode->nodes + i), currentNode);
			if (smallestMovements > aux)	// Si el numero de movimientos es menor, se elige
			{
				smallestMovements = aux;
				idxSmallest = i;	// Seleccion del indice
				//printf("smallest movements: %d on %d \n", smallestMovements, idxSmallest);
			}
		}
	if (idxSmallest < 0)
		return 0;

	Node nextNode = NULL;
	getNodeLinked(&nextNode, currentNode, idxSmallest);
	nextNode->box->content = currentNode->box->content + 1;

	int catcher = tour(checkerboard, nextNode);
	if (catcher == 2)
	{
		printf("%sError No INDEX FOUNDED on tour, lvl %d %s", ERR_T, currentNode->box->content, RST_T);
		return 2;
	}
	/*	Basic backtraking
	for (int i = 0; i < currentNode->sNodes; i++)
	{
		Node nextNode = NULL;
		getNodeLinked(&nextNode, currentNode, i);
		if (nextNode->box->content < 0)
		{
			nextNode->box->content = currentNode->box->content + 1;
			if (tour(checkerboard, nextNode))
				return 1;
			else
				nextNode->box->content *= -1;
		}
	}
	*/
	return 0;
}

void printSolutionwTittle(Node* checkerboard, char* tittle, int type)
{
	printf("\n\t\t%s\n", tittle);
	printf("     ");
	for (int i = 'A'; i < 'A' + N; i++)
		printf(" %3c ", i);
	printf("\n    ");
	for (int i = 0; i < N*5+1; i++)
		printf("-");
	printf("\n");
	//printf("movements %d \n", (*(checkerboard))->sNodes);
	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node currentNode = *(checkerboard + i + (N * j));
		if (i == 0)
			printf("%2d | ", j);
		if (type == 0)
			printf(" %3d ", currentNode->sNodes);
		else
		{
			if (currentNode->box->content == type)
				printf(" %s%3d%s ", SHOW_T, currentNode->box->content, RST_T);
			else if (currentNode->box->content >= 0)
				printf(" %s%3d%s ", SCND_T, currentNode->box->content, RST_T);
			else
				printf(" %3d ", currentNode->box->content);
		}

		if (i+1==N)
		{
			i = -1;
			j++;
			printf("\n");
		}
	}
}

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds 
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time 
	clock_t start_time = clock();

	// looping till required time is not achieved 
	while (clock() < start_time + milli_seconds);
}

void printAllNode(Node node)
{
	printf("%p::%d ", node, node->sNodes);
	printAllBox(node->box);
	printf("\n");
}

void printAllBox(Box box)
{
	printf("[%c%d]%d ", box->x + 'A', box->y, box->content);
}

