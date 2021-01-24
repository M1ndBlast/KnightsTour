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

typedef enum results {OK, NOCIRCUIT, NULLMEMORY,  ALRADYNULL, NORESIZE} coderes;
typedef enum status {NO, YES} bool;

int initBox(Box* box);
int destroyBox(Box box);
int initNode(Node* node);
int destroyNode(Node node);
int linkNode(Node node, Node nodeToLink);
int getNodeLinked(Node* nodeWanted, Node node, int idx);
void nodesLinker(Node* nodes);
void printSolutionwTittle(Node* checkerboard, char* tittle, int type);
int posibleMovements(Node node, Node toExcluse);
int isNear(Node origin, Node wanted);
int isCircuit(Node* checkerboard);
int movement(Node* checkerboard, Node currentNode, Node *start);

int main()
{
	Node* checkerboard = NULL;
	Box startPos = NULL;
	initBox(&startPos);

	while (1)
	{
		printf("Enter board size: ");
		scanf("%i", &N);

		if (N < 5 || N % 2)
			printf("%sJust even numbers over 5 %s\n", ERR_T, RST_T);
		else if (N > 16)
			/* Can solve for bigger checkerboards, but take a loooong time if u don't select specific boxs
			*	size 22 box (7 8)
			*	size 24 box (7 8)
			*	size 26 box (7 8)
			*	!NO WORKS size 28 box (7 8)
			*	No tried with other options
			*/
			printf("%sMaximum checker size is 16 for this PC Health	%s\n", ERR_T, RST_T);
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

	checkerboard = (Node*)calloc(N * N, sizeof(Node));

	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node* currentNode = checkerboard+i+(N*j);
		initNode(currentNode);
		(*currentNode)->box->x = i;
		(*currentNode)->box->y = j;

		if (i + 1 == N)
		{
			i = -1;
			j++;
		}
	}

	//printSolutionwTittle(checkerboard, "Initation", 1);
	nodesLinker(checkerboard);

	Node* nextNode = checkerboard+startPos->x+(N*startPos->y);
	(*nextNode)->box->content = 0;
	if (movement(checkerboard, (*nextNode), nextNode))
		printSolutionwTittle(checkerboard, "Result", N*N);
	else
		printf("Solution does not exist");

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

int linkNode(Node node, Node nodeToLink)
{
	if (!node || !nodeToLink)
		return 2;

	node->nodes = (Node*) realloc(node->nodes, (node->sNodes+1)*sizeof(Node));
	if (!node->nodes)
		return 1;
	*(node->nodes + node->sNodes) = nodeToLink;
	node->sNodes++;

	return 0;
}

int getNodeLinked(Node* nodeWanted, Node node, int idx)
{
	if (!node)
		return 1;

	*nodeWanted = *(node->nodes + idx);

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
	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node currentNode = *(checkerboard+i+(N*j));
		if (!i)
			printf("%2d | ", j);
		if (!type)
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
	int near = 0;
	for (int i = 0; i < origin->sNodes; i++)
		if (wanted == *(origin->nodes + i))
		{
			near = 1;
			break;
		}
	return near;
}

int isCircuit(Node *checkerboard)
{
	int notCircuit = 0;
	for (int i = 0; i < N*N; i++)
		if ((*(checkerboard + i))->box->content < 0)
		{
			notCircuit = NOCIRCUIT;
			break;
		}

	return !notCircuit;
}

int movement(Node* checkerboard, Node currentNode, Node* start)
{
	int sizeSmallers = 0, idxSmallers[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
	for (int i = 0, aux = 0, smallestMovements = 8; i < currentNode->sNodes; i++)	// Recorrido de todos los nodos vincuados
	{
		//printf("number nodes %d \n", currentNode->sNodes);
		if ((currentNode->nodes[i])->box->content < 0)		// Verificación que no es un numero ya utilizado
		{
			aux = posibleMovements(*(currentNode->nodes + i), currentNode);
			if (smallestMovements > aux)	// Si el numero de movimientos es menor, se elige
			{
				smallestMovements = aux;
				*idxSmallers = i;	// Seleccion del indice
				for (int j=1; j<sizeSmallers; j++)	// Limpieza de indice
					*(idxSmallers+j) = 0;
				sizeSmallers = 1;
			}
			else if (smallestMovements == aux)	// Si hay repetido
			{
				*(idxSmallers+sizeSmallers) = i;
				sizeSmallers++;
			}
		}
	}

	if (sizeSmallers <= 0)	// No hay nodos disponibles a saltos
	{

		if (isCircuit(checkerboard))// Ya todas las casillas fueron recorridas?
			/*	 PARA IMPRIMIR EL PRCESO DEL PROGRAMA con rutas completas
			//system("cls");
			delay(1);
			printSolutionwTittle(checkerboard, "Steps", currentNode->box->content);
			*/
			//printf(".");
			if (isNear(currentNode, *start))	// Esta cerca la cassilla de comienzo?
			{
				(*start)->box->content = currentNode->box->content+1;	// Asignar ultimo valor
				return 1;	// Fin recursividad
			}
		// No esta cerca la casilla de comienzo o no se han recorrido todas las casillas, buscar tro camino
		return 0;
	}
	else	// Hay nodos a saltar
		for (int i = 0; i < sizeSmallers; i++)	// Probar todas las disponibilidades
		{
			Node nextNode = NULL;
			getNodeLinked(&nextNode, currentNode, *(idxSmallers+i));
			nextNode->box->content = currentNode->box->content + 1;	// Asignacion del movimiento posterior

			if (movement(checkerboard, nextNode, start))	// Si es valido, se realizo un ciclo
				return 1;	// Se comunica el resultado
			else
				nextNode->box->content *= -1;	// Se cancela el movimiento realizado y se mantiene un registro de los ultimos movimientos pero no es la ruta actual
		}

	// llegado a este punto no se encontro el resultado
	return 0;	// Se comunica el error
}