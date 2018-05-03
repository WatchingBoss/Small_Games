#include <ncurses.h>
#include <stdlib.h> // we use rand() & srand()
#include <time.h>   // we need actual random()
#include <unistd.h> // we use usleep()

#define WIN_WIDTH 25
#define WIN_HEIGHT 30
#define WIN_Y 6
#define WIN_X ((COLS - WIN_WIDTH) / 2)
#define AMOUNT_BOOLET 15
#define AMOUNT_TARGET 20
#define MISS_LIMIT 5

typedef struct GAME_UNIT
{
	float pY, pX;
	char skin;
} GameUnit;

enum controlDefinition {STOP, LEFT, RIGHT} ctrl;
bool shooting;
bool gameOver;
int score;
int keyInput;
int numBoolet;
int lose;

void shoot();
void graphic(WINDOW *, GameUnit *, GameUnit *, GameUnit *);
void input();
void logic(WINDOW *win, GameUnit *, GameUnit *, GameUnit *);
void start(GameUnit *, GameUnit *, GameUnit *);

int main()
{
	srand(time(NULL));
	initscr();
	start_color();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();
	curs_set(FALSE);

	shoot();

	curs_set(TRUE);
	endwin();
	return(0);
}

void shoot()
{
	WINDOW *win;
	char goGame = 'y';

	GameUnit player;
	player.pY   = WIN_HEIGHT - 2;
	player.pX   = WIN_WIDTH / 2;
	player.skin = '^';

	GameUnit boolet[AMOUNT_BOOLET];
	for(int i = 0; i < AMOUNT_BOOLET; ++i)
	{
		boolet[i].pY =  player.pY - 1;
		boolet[i].pX =  player.pX;
		boolet[i].skin =  ' ';
	} // end forn

	GameUnit target[AMOUNT_TARGET];
	for(int i = 0; i < AMOUNT_TARGET; ++i)
	{
		target[i].pY = rand() % (AMOUNT_TARGET * 5) - (AMOUNT_TARGET * 5);
		target[i].pX = rand() % (WIN_WIDTH - 2) + 2;
		target[i].skin = rand() % (91 - 65) + 65; // range A - Z
	} // end for

	win = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_Y, WIN_X);

	while(goGame == 'y' || goGame == 'Y')
	{
		start(&player, boolet, target);
		while(!gameOver)
		{
			input();
			logic(win, &player, boolet, target);
			graphic(win, &player, boolet, target);
			usleep(100000);
		} // end while
		if(keyInput != 'q')
		{
			nodelay(stdscr, FALSE);
			if(score == AMOUNT_TARGET)
				mvprintw(1, COLS / 2.5, "You are good! You win:)");
			else
				mvprintw(1, COLS / 2.5, "You are loser LMAO ^__^");
			mvprintw(3, COLS / 2.5, "Maybe play again? (y/n)");
			refresh();
			goGame = 'P';
			while(goGame != 'y' && goGame != 'Y' && goGame != 'n' && goGame != 'N')
				goGame = getch();
		} // end if
		else
			goGame = 'n';   
	} // end while

//	sleep(1);
}

void graphic(WINDOW *win, GameUnit *player, GameUnit *boolet, GameUnit *target)
{
	werase(win);
	
	box(win, 0, 0);

	mvwaddch(win, player->pY, player->pX, player->skin);
	
	for(int i = 0; i < AMOUNT_BOOLET; ++i)
		mvwaddch(win, boolet[i].pY, boolet[i].pX, boolet[i].skin);

	for(int i = 0; i < AMOUNT_TARGET; ++i)
		if(target[i].pY > 0)
			mvwaddch(win, target[i].pY, target[i].pX, target[i].skin);

	mvprintw(5, COLS / 2.5, "Score: %d", score);
	mvprintw(WIN_Y + 5, 1, "You lost: %d", lose);

	refresh();
	wrefresh(win);
}

void logic(WINDOW *win, GameUnit *player, GameUnit *boolet, GameUnit *target)
{
	switch(ctrl)
	{
		case LEFT:
			--player->pX;
			for(int i = 0; i < AMOUNT_BOOLET; ++i)
			{
				if(boolet[i].pY == player->pY - 1)
					--boolet[i].pX;
			} // end for
			break;
		case RIGHT:
			++player->pX;
			for(int i = 0; i < AMOUNT_BOOLET; ++i)
			{
				if(boolet[i].pY == player->pY - 1)
					++boolet[i].pX;
			} // end for
			break;
		default:
			break;
	} // end switch

	if(shooting)
	{
		++numBoolet;
		boolet[numBoolet].skin = '|';
		shooting = false;
	} // end if

	if(player->pX == 0) player->pX = WIN_WIDTH - 2;
	else if(player->pX == WIN_WIDTH - 1) player->pX = 1;

	if(numBoolet >= 0)
	{
		for(int i = 0; i <= numBoolet; ++i)
		{
			if(boolet[i].pY != 1)
				--boolet[i].pY;
			else
			{
				boolet[i].pY = player->pY - 1;
				boolet[i].pX = player->pX;
				boolet[i].skin = ' ';
			} // end else
		} // end for
	} // end if
	if(numBoolet == AMOUNT_BOOLET - 1) numBoolet = 0;

	for(int i = 0; i < AMOUNT_TARGET; ++i)
	{
		for(int j = 0; j <= numBoolet; ++j)
		{
			if(target[i].pY == boolet[j].pY && target[i].pX == boolet[j].pX)
			{
				target[i].pY = -(AMOUNT_TARGET * 5);
				++score;
			} // end if
		} // end for
		if(target[i].pY == WIN_HEIGHT)
			++lose;
		target[i].pY += 0.25;
	} // end for

	if(score == AMOUNT_TARGET - MISS_LIMIT) gameOver = true;
//	if(lose == MISS_LIMIT) gameOver = true;
}

void input()
{
	if((keyInput = getch()) != ERR)
	{
		switch(keyInput)
		{
			case KEY_LEFT:
				ctrl = LEFT;
				break;
			case KEY_RIGHT:
				ctrl = RIGHT;
				break;
			case ' ':
				shooting = true;
				break;
			case 'q':
				gameOver = true;
				break;
			default:
				break;
		} // end switch
	} // end if
}

void start(GameUnit *player, GameUnit *boolet, GameUnit *target)
{
	clear();
	gameOver = false;
	ctrl = STOP;
	score = 0;
	numBoolet = -1;
	shooting = false;
	nodelay(stdscr, TRUE);

	mvprintw(WIN_Y + 1, 1, "Goal: %d", (AMOUNT_TARGET - MISS_LIMIT));
	mvprintw(WIN_Y + 2, 1, "Whole targets are: %d", AMOUNT_TARGET);
	mvprintw(WIN_Y + 3, 1, "You will lose if miss %d targets", MISS_LIMIT);


	player->pY   = WIN_HEIGHT - 2;
	player->pX   = WIN_WIDTH / 2;

	for(int i = 0; i < AMOUNT_BOOLET; ++i)
	{
		boolet[i].pY =  player->pY - 1;
		boolet[i].pX =  player->pX;
		boolet[i].skin =  ' ';
	}

	for(int i = 0; i < AMOUNT_TARGET; ++i)
	{
		target[i].pY = rand() % (AMOUNT_TARGET * 5) - (AMOUNT_TARGET * 5);
		target[i].pX = rand() % (WIN_WIDTH - 2) + 2;
		target[i].skin = rand() % (91 - 65) + 65; // range A - Z
	}
}
