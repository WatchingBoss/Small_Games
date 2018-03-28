#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

typedef enum {false, true} bool;

bool gameOver;

const int width = 40, height = 30;
int x, y, fruitX, fruitY, score;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
enum eDirection dir;

int i, j;

void
setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void
draw()
{
	system("cls");

	for (i = 0; i < width; i++)
		printf("-");
	printf("\n");

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			if(j == 0  || j == width - 1)
				printf("|");
			else
				printf(" ");
		}
		printf("\n");
	}

	for (i = 0; i < width; i++)
		printf("-");
	printf("\n");
}

void
input()
{

}

void
logic()
{

}

int main()
{
	setup();    

	while(!gameOver)
	{
		draw();
		input();
		logic();
	}

    return 0;
}
