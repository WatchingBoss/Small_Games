#include <ncurses.h>
#include <stdlib.h>  /* We need rand() */
#include <time.h>    /* We need time() for actual random */
#include <unistd.h>  /* We need usleep() */

typedef struct WIN_PARAM
{
	int positionY, positionX;
	int width, height;
} WinMain;

typedef struct PLAYER_PARAM
{
	char skin;
	int positionY, positionX;
} Player;

void init_all(WinMain *, Player *, Player *, int *);
void drawWindow(WinMain *);
void action(WinMain *, Player *, Player *, int *);
void input();
void logic(WinMain *, Player *, Player *, int *);
void start();

enum control {STOP, LEFT, RIGHT, SPACE} ctrl;

int main()
{
	WinMain win;       /* Our window   */
	Player pl1;	       /* Player 1     */
	int arr = 10;
	Player sh[arr];         /* Boolet shots */

	char choose = 'y'; /* Play again?*/
	int inputCh;       /* Input from keyboard */
	int score = 0;
	bool gameOver;
	
	initscr();
	curs_set(0);
	start_color();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	init_pair(1, COLOR_CYAN, COLOR_YELLOW);
	init_pair(2, COLOR_RED, COLOR_MAGENTA);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);

	srand(time(NULL)); /* Make actual random for rand() */

	init_all(&win, &pl1, sh, &arr);

    while(choose == 'y' || choose == 'Y')
	{
		nodelay(stdscr, TRUE);
		start(&gameOver);
		while(!gameOver)
		{
			drawWindow(&win);
			action(&win, &pl1, sh, &arr);
			input(&gameOver, &inputCh);
			logic(&win, &pl1, sh, &arr);
			usleep(50000);
		} // end while
		
		if(inputCh != 'q')
		{
			mvprintw(1, 5, "May be although one game? Press: y");
			refresh();
			choose = getch();
			score = 0;
			pl1.positionX = COLS / 2;
		} // end if
		else
			choose = 'n';
	} // end while

	curs_set(1);
	endwin();

    return(0);
}

void init_all(WinMain *pWin, Player *pPl1, Player *pSh, int *arr)
{
	pWin->width     = 50;
	pWin->height    = 50;
	pWin->positionY = 5;
	pWin->positionX = (COLS - pWin->width) / 2;

	pPl1->positionY = pWin->height - 2;
	pPl1->positionX = COLS / 2;
	pPl1->skin = '^';

	for(int i = 0; i < *arr; ++i)
	{
		pSh[i].skin = ' ';
		pSh[i].positionY = pPl1->positionY - 1;
		pSh[i].positionX = pPl1->positionX;
	}
}

void drawWindow(WinMain *pWin)
{
	int y, x, w, h;
	char board = '|';
	w = pWin->width;
	h = pWin->height;
	y = pWin->positionY;
	x = pWin->positionX;

	attron(COLOR_PAIR(1));
	mvvline(y, x, board, h);
	mvvline(y, x + w, board, h);
	attroff(COLOR_PAIR(1));
}

void action(WinMain *pWin, Player *pPl1, Player *pSh, int *arr)
{
	int y, x, shotY, shotX;
	char skin, shotSkin;
	y        = pPl1->positionY;
	x        = pPl1->positionX;
	skin     = pPl1->skin;
	shotY    = pSh->positionY;
	shotX    = pSh->positionX;
    shotSkin = pSh->skin;

	for(int i = pWin->positionX + 1; i < pWin->positionX + pWin->width; i++)
	{
		if(i == x)
			mvprintw(y, x, "%c", skin);
		else
			mvprintw(y, i, " ");
	}

	for(int i = 0; i < *arr; ++i)
	{
		mvprintw(shotY, shotX, "%c", shotSkin);
	}
	refresh();
}

void logic(WinMain *pWin, Player *pPl1, Player *pSh, int *arr)
{
	int i = 0;
	switch(ctrl)
	{
		case LEFT:
			--pPl1->positionX;
			--pSh->positionX;
			break;
		case RIGHT:
			++pPl1->positionX;
			++pSh->positionX;
			break;
		case SPACE:
			pSh[i].skin = '|';
			++i;
			break;
		default:
			break;
			
	} // end switch

	if(pPl1->positionX == pWin->positionX)
		pPl1->positionX = pWin->positionX + pWin->width - 1;
	if(pPl1->positionX == pWin->positionX + pWin->width)
		pPl1->positionX = pWin->positionX + 1;
}

void input(bool *gameOver, int *inputCh)
{
	if((*inputCh = getch()) != ERR)
	{
		switch(*inputCh)
		{
			case KEY_LEFT:
				ctrl = LEFT;
				break;
			case KEY_RIGHT:
				ctrl = RIGHT;
				break;
			case ' ':
				ctrl = SPACE;
				break;
			case 'q':
				*gameOver = true;
				break;
			default:
				break;					 
		} // end switch
	} // end if
}

void start(bool *gameOver)
{
	*gameOver = false;
	ctrl = STOP;
}
