#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

typedef enum {false, true} bool;
#include "snake.c"

bool gameOver;

const int width = 20, height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail = 0;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
enum eDirection dir;

int i, j, k;
int prevX, prevY, prev2X, prev2Y;

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
			else if (i == y && j == x)
				printf("O");
			else if (i == fruitY && j == fruitX)
				printf("F");
			else
			{
				bool print = false;
				for (k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						printf("o");
						print = true;
					}
				}
				if (!print)
					printf(" ");
			}
		}
		printf("\n");
	}

	for (i = 0; i < width; i++)
		printf("-");
	printf("\n");
	printf("Score: %d\n", score);
}

void
input()
{
	if (_kbhit())
	{
		switch(_getch())
		{
			case 'w':
			{
				dir = UP;	
			} break;
			case 's':
			{
				dir = DOWN;	
			} break;
			case 'a':
			{
				dir = LEFT;	
			} break;
			case 'd':
			{
				dir = RIGHT;	
			} break;
			case 'x':
			{
				gameOver = true;
			} break;
			default:
				break;
		}
	}
}

void
logic()
{
	prevX    = tailX[0];
	prevY    = tailY[0];
	tailX[0] = x;
	tailY[0] = y;
	for (i = 1; i < nTail; i++)
	{
		prev2X   = tailX[i];
		prev2Y   = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX    = prev2X;
		prevY    = prev2Y;
	}

	switch(dir)
	{
		case UP:
		{
			y--;
		} break;
		case DOWN:
		{
			y++;
		} break;
		case LEFT:
		{
			x--;
		} break;
		case RIGHT:
		{
			x++;
		} break;
		default:
			break;
	}

	if (x >= width)  x = 0; else if (x < 0) x = width;
	if (y >= height) y = 0; else if (y < 0) y = height;

	for (i = 0; i < nTail; i++)
	{
		if (x == tailX[i] && y == tailY[i])
			gameOver = true;
	}

	if (x == fruitX && y == fruitY)
	{
		fruitX = rand() % width;
		fruitY = rand() % height;
		score++;
		nTail++;
	}
}

int main()
{
	ShowConsoleCursor(false);
	setup();    

	while(!gameOver)
	{
		draw();
		input();
		logic();
		Sleep(50);
	}
	
	ShowConsoleCursor(true);
	
    return 0;
}
