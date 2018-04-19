#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

enum iDirect {STOP = 0, UP, DOWN, LEFT, RIGHT};
enum iDirect dir;

int height = 9, width = 9;

int gameOver;

void
setup()
{
	gameOver = 0;
}

void
draw()
{
//	system("cls");
	
	for(int i = 0; i < height; i++)
	{
		for(int j = 1; j <= width; j++)
			printf(" %d ", j);
		printf("\n");
	}

}

void
input()
{
	if(kbhit())
	{
		switch(getch())
		{
			case 'w': case 'W':
				dir = UP;
				break;
			case 's': case 'S':
				dir = DOWN;
				break;
			case 'a': case 'A':
				dir = LEFT;
				break;
			case 'd': case 'D':
				dir = RIGHT;
				break;
			case 'x':
				gameOver = 1;
			default:
				break;
		}
	}
}

void
logic()
{
	switch(dir)
	{
		case UP:
			printf("GO UP\n");
			dir = STOP;
			break;
		case DOWN:
			printf("GO DOWN\n");
			dir = STOP;			
			break;
		case LEFT:
			printf("GO LEFT\n");
			dir = STOP;
			break;
		case RIGHT:
			printf("GO RIGHT\n");
			dir = STOP;
			break;
		default:
			break;
	}
}

int
main()
{
    setup();
	
	draw();
	
	while(!gameOver)
	{
		input();
		logic();
	}
	
    return 0;
}
