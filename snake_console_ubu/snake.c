#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void game();
void action(int, int);
void control();
void logic();
void start();

bool gameOver;
const int width = 30, height = 30;
int x, y, fX, fY, ch;
int tail = 0;
int tailX[50], tailY[50];
int score = 0;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN} ctrl;

int main()
{
	srand(time(NULL));
	
	game();

    return 0;
}

void
game()
{
	char answer = 'y';

    initscr();
	raw();
	noecho();
	curs_set(FALSE); 
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_RED);

	const int startx = (COLS - width) / 2, starty = (LINES - height) / 2;

	while(answer == 'y' || answer == 'Y')
	{
		nodelay(stdscr, TRUE);
		start();
		
		while(!gameOver)
		{ 
			action(starty, startx);
			control();
			logic();
		}

		if(ch != 'x')
		{
			nodelay(stdscr, FALSE);
			mvprintw(starty - 5, startx, "Do you want play again? (y/n)");
			refresh();
			answer = getch();
			tail = 0;
			score = 0;
			mvprintw(starty - 5, startx, "                              ");
			mvprintw(starty - 2, startx + 10, "             ");
		}
		else
			answer = 'x';
	} 

	sleep(1);

	curs_set(TRUE);
	endwin();
}

void
action(int starty, int startx)
{
	WINDOW *win;
	win = newwin(height, width, starty, startx);
	box(win, 0, 0);
	wattron(win,COLOR_PAIR(1));
	wborder(win, '*', '*', '*', '*', '+', '+', '+', '+');
	wattroff(win,COLOR_PAIR(1));

	wattron(win,COLOR_PAIR(2));
	mvwprintw(win, fY, fX, "U");
	wattroff(win,COLOR_PAIR(2));

	wattron(win,COLOR_PAIR(3));
	mvwprintw(win, y, x, "O");
	for(int i = 0; i < tail; ++i)
	{
		mvwprintw(win, tailY[i], tailX[i], "o");
	}
	wattroff(win,COLOR_PAIR(3));
	
	wrefresh(win);

	mvprintw(starty - 2, startx + 10, "Score: %d", score);

	refresh();

	usleep(80000);
}

void
control()
{
	if((ch = getch()) != ERR)
	{
		switch(ch)
		{
			case 'a':
			case KEY_LEFT:
				ctrl = LEFT;
				break;
			case 'w':
			case KEY_UP:
				ctrl = UP;			
				break;
			case 'd':
			case KEY_RIGHT:
				ctrl = RIGHT;			
				break;
			case 's':
			case KEY_DOWN:
				ctrl = DOWN;			
				break;
			case 'x':
				gameOver = true;
				break;
			default:
				break;
		}
	}
}

void
logic()
{
	int prevX = tailX[0], prevY = tailY[0],
		prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for(int i = 1; i < tail; ++i)
	{
		prev2X   = tailX[i];
		prev2Y   = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX    = prev2X;
		prevY    = prev2Y;
	}
	
	switch(ctrl)
	{
		case LEFT:
			--x;
			break;
		case UP:
			--y;
			break;
		case RIGHT:
			++x;
			break;
		case DOWN:
			++y;
			break;
		default:
			break;
	}

	if(x == 0) x = width - 1; else if(x == width - 1) x = 1;
	if(y == 0) y = height -1; else if(y == height - 1) y = 1;

	if(x == fX && y == fY)
	{
		fX = (rand() % (width - 2)) + 1;
		fY = (rand() % (height - 2)) + 1;
		++score;
		++tail;
	}

	for(int i = 0; i < tail; ++i)
	{
		if(x == tailX[i] && y == tailY[i])
			gameOver = true;
	}
}

void
start()
{
	gameOver = false;
	
	x = width / 3;
	y = height / 3;
	fX = (rand() % (width - 2)) + 1;
	fY = (rand() % (height - 2)) + 1;
}


