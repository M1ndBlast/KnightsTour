/*	Knight's Tour
		De tal forma que para cualquier casilla arbitraria, recorra todas las dem�s casillas, y regrese de nuevo a la casilla original,
		sin pasar 2 veces por una misma casilla en el circuito (exceptuando por la casilla de partida).
		Pueden resolverlo de forma iterativa � recursiva y de preferencia usando estructuras de datos
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

typedef enum results { OK, NOHAMILTONIAN, HAMILTONIAN, NULLMEMORY, ALRADYNULL, NORESIZE } coderes;
typedef enum status { NO, YES } bool;
typedef enum statBox { NOSELECTED, CANDIDATE, SELECTED } statBox;


typedef struct box
{
	int x;
	int y;
	int content;
	statBox status;
} s_box, * Box;

typedef struct node
{
	Box box;
	int sNodes;
	struct node** nodes;
} s_node, * Node;


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
int isHamiltonian(Node* checkerboard);
int movement(Node* checkerboard, Node currentNode, Node* start);

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
			/* Can solve for bigger checkerboards, but take a loooong time if u don't select specific boxes
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
		Node* currentNode = checkerboard + i + (N * j);
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

	Node* nextNode = checkerboard + startPos->x + (N * startPos->y);
	(*nextNode)->box->content = 0;
	(*nextNode)->box->status = SELECTED;
	if (movement(checkerboard, (*nextNode), nextNode))
		printSolutionwTittle(checkerboard, "Result", N * N);
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
	(*box)->status = NOSELECTED;
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

	node->nodes = (Node*)realloc(node->nodes, (node->sNodes + 1) * sizeof(Node));
	if (!node->nodes)
		return 1;
	*(node->nodes + node->sNodes) = nodeToLink;
	node->sNodes++;

	return 0;
}

int getNodeLinked(Node* nodeWanted, Node node, int idx)
{
	// not defined
	if (!node)
		return 1;

	*nodeWanted = *(node->nodes + idx);

	return 0;
}

void nodesLinker(Node* nodes)
{
	const s_box relativeMovements[8] = { { 1, -2, 0},
										{ 2, -1, 0},
										{ 2,  1, 0},
										{ 1,  2, 0},
										{-1,  2, 0},
										{-2,  1, 0},
										{-2, -1, 0},
										{-1, -2, 0} };
	Box nextBox = NULL;
	initBox(&nextBox);

	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node currentNode = *(nodes + i + (N * j));

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
	for (int i = 0; i < N * 5 + 1; i++)
		printf("-");
	printf("\n");
	for (int i = 0, j = 0; i < N && j < N; i++)
	{
		Node currentNode = *(checkerboard + i + (N * j));
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

		if (i + 1 == N)
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
		// here CANDIDATE is discarded for double reviwing
		if (curNode != toExcluse && curNode->box->status == NOSELECTED)
			counter++;
	}
	return counter;
}

int isNear(Node origin, Node wanted)
{
	//goes trought whole jumpeable boxes
	for (int i = 0; i < origin->sNodes; i++)
		//	are same wanted box and jumpeable box?
		if (wanted == *(origin->nodes + i))
			return 1;
	return 0;
}

/*
* returns if it is NOHAMILTONIAN or HAMILTONIAN path
*/
int isHamiltonian(Node* checkerboard)
{
	for (int i = 0; i < N*N; i++)
		if ((*(checkerboard + i))->box->status != SELECTED)
			return 0;
	return 1;
}

int movement(Node* checkerboard, Node currentNode, Node* start)
{
	int *idxedSmallest;	// Here got boxes to jump indexes
	int idxedSmallestSize = indexCandidates(currentNode, &idxedSmallest);

	// If there aren't indexes to jump
	if (!idxedSmallestSize)
	{
		// if the knights toured all boxes
		if (isHamiltonian(checkerboard))
		{
				/*	 TO PRINT ALL HAMILTONIAN PATHS
			system("cls");
			delay(1);
			printSolutionwTittle(checkerboard, "Steps", currentNode->box->content);
				*/
			// is near the start box?
			if (isNear(currentNode, *start))
			{
				// set value as last jump
				(*start)->box->content = currentNode->box->content+1;
				return 1;	// End Back-track
			}
		}
	}
	// if there are at least one index
	else
	{
		// Goes trought whole jumpeable boxes with lowest next jumps
		for (int i = 0; i < idxedSmallestSize; i++)
		{
			Node nextNode = NULL;
			getNodeLinked(&nextNode, currentNode, *(idxedSmallest+i));
			// set value to jump in it
			nextNode->box->content = currentNode->box->content + 1;
			nextNode->box->status = SELECTED;

			// Do next jumps and if returns 1, backtracking is ended
			if (movement(checkerboard, nextNode, start))
				return 1;
			// discard box, set default status and tries with next box
			else
			{
				// change to negative value only for visibility
				nextNode->box->content *= -1;
				nextNode->box->status = NOSELECTED;
			}
		}
	}
	return 0;
}

/*
  -1〉 not selected
  -2〉 in reviewing
*/
int indexCandidates(Node currentNode, int** toIndexing)
{
	int *indexedCandidates = calloc(8, sizeof(int));
	int idxSize = 0;

	for (int i=0; i<8; i++)
		*(indexedCandidates + i) = -1;
	
	for (int i = 0, smallest = 8; i < currentNode->sNodes; i++)
	{
		Node iNode = *(currentNode->nodes + i);

		if (iNode->box->status == NOSELECTED) // Node no SELECTED and not set as CANDIDATE
		{
			int curNodeMovements = posibleMovements(*(currentNode->nodes + i), currentNode);
			if (curNodeMovements < smallest) // New smaller movements
			{
				smallest = curNodeMovements;
				for (int j = 0; j < idxSize; j++)
					*(indexedCandidates + j) = -1;
				idxSize = 1;
				*indexedCandidates = i;
			}
			else if (curNodeMovements == smallest) // Another small movements
			{
				for (int j = 0; j < 8; j++)
					if (*(indexedCandidates + j) == -1)
					{
						*(indexedCandidates + j) = i;
						break;
					}
				idxSize++;
			}
			//else // bigger movements
			 //for(int j=1; j<8; j++)
			  //if(*(indexedCandidates+j)==-1)
		}
	}

	*toIndexing = indexedCandidates;
	return idxSize;
}