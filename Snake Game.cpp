#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <conio.h>
using namespace std;

const int screenwidth = 60;
const int screenheight = 20;
int eatfood;
int input;
int gameover = 0;



//C++ standard method of setting console handle, gotoxy allows for easy access to the coordinates of the console.
void gotoxy(int x, int y)
{
	HANDLE hc;
	COORD cursor;

	hc = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hc == INVALID_HANDLE_VALUE) return;

	cursor.X = x;
	cursor.Y = y;
	SetConsoleCursorPosition(hc, cursor);
}
//C++ standard method of clearing the screen of the console after each iteration, taken from stackoverflow
void clearScreen()
{
	HANDLE                     hc;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hc = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hc == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells and cell attributes in the current buffer */
	if (!GetConsoleScreenBufferInfo(hc, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hc,		//handle to console screen buffer
		(TCHAR) ' ',	//character to write to the buffer
		cellCount,		//number of cells to write to
		homeCoords,		//coordinates of first cell
		&count			//receives the number of characters written
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hc,			//handle to console screen buffer
		csbi.wAttributes,	//Character attributes to use
		cellCount,			//Number of cells to set attribute
		homeCoords,			//coordinate of first cell
		&count				//receives the number of characters written
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hc, homeCoords);
}
//initialize structure for snake, to be used for the snake's body(entire length of its body) and its tail segment
struct snakesegment
{
	int x;
	int y;
};
//OOP method, defining the snake attributes
class snake
{
private:
	int snakelength;
	int direction;
	snakesegment snaketail[screenwidth*screenheight];
	snakesegment tailsegment[screenwidth*screenheight];
	int board[100][100];
	int x;
	int y;
public:
	void initialscreen();
	void instructions();
	void boarddetails();
	void snakedetails();
	void snakemovement();
	void KeyInput();
	void foodclass();
	void printgame();
};
//initial screen to the game
void snake::initialscreen() {
	system("cls");
	cout << "Snake Game" << endl;
	cout << "For instructions on the game, press 9." << endl;
	cout << "To start on the game, choose a difficulty level (1 to 8), 1 being the easiest and 8 being the hardest." << endl;
	cout << "You score higher for choosing a higher difficulty level!" << endl;

}
// game instructions
void snake::instructions() {
	system("cls");
	cout << "Instructions for snake game" << endl;
	cout << endl;
	cout << "Use the 'w' key to go 0." << endl;
	cout << "Use the 'a' key to turn 3." << endl;
	cout << "Use the 's' key to go 2." << endl;
	cout << "Use the 'd' key to turn 1." << endl;
	cout << "You cannot reverse movement of the snake." << endl;
	cout << "You lose if you knock into the edges of the -1." << endl;
	cout << "You lose if you knock into your own snaketail/tail" << endl;
	cout << "Press 'b' now to go back to the start screen" << endl;

}
//initializing the gameboard, empty terrain is safe, but walls are dangerous!
void snake::boarddetails()
{

	for (int i = 0; i < screenheight; i++)
		for (int j = 0; j < screenwidth; j++)
			board[i][j] = 0; //passable terrain is defined as 0

	for (int i = 0; i <= screenwidth + 1; i++)
	{
		board[0][i] = -1;
		board[screenheight + 1][i] = -1; //top and bottom wall defined as -1
	}
	for (int j = 0; j <= screenheight + 1; j++)
	{
	
		board[j][0] = -2;
		board[j][screenwidth + 1] = -2; //left and right wall are -2
	}
}


//initializing the snake
void snake::snakedetails()
{
	snakelength = 5;		//set head of snake to be at the centre, snake to have initial legnth of 5
	snaketail[0].x = screenwidth / 2;
	snaketail[0].y = screenheight / 2;
	direction = 0;
	for (int a = 1; a < snakelength; a++)
	{	
		{
			snaketail[a].x = snaketail[a - 1].x;
			snaketail[a].y = snaketail[a - 1].y; //to align the snake 
			
		}
	}
	for (int i = 0; i < snakelength; i++)
		board[snaketail[i].y][snaketail[i].x] = -3; //Position of snake is given by -3, to enable easy drawing of snake later on
}
void snake::snakemovement()
{
	for (int i = 0; i < snakelength; i++)
	{
		tailsegment[i].x = snaketail[i].x;
		tailsegment[i].y = snaketail[i].y;
	}

	if (direction == 0 && input != 2 || direction == 1 && input != 3 || direction == 2 && input != 0 || direction == 3 && input != 1) //prevent opposite directions
		direction = input; //direction the user chooses is the input for the snake
	if (direction == 0)
	{
		snaketail[0].x = tailsegment[0].x;
		snaketail[0].y = tailsegment[0].y - 1; //upwards, x coordinate does not change but y coordinates decreases
	}
	else if (direction == 1)
	{
		snaketail[0].x = tailsegment[0].x + 1; //rightwards x coordinates increase, y coordinates do not change
		snaketail[0].y = tailsegment[0].y;
	}
	else if (direction == 2)
	{
		snaketail[0].x = tailsegment[0].x; //downwards, x coordinates do not change, y coordinates increase
		snaketail[0].y = tailsegment[0].y + 1;
	}
	else if (direction == 3)
	{
		snaketail[0].x = tailsegment[0].x - 1; //leftwards, x coordinates decreases, y coordinates do not change
		snaketail[0].y = tailsegment[0].y;
	}
	for (int i = 0; i < screenheight + 2; i++)
		for (int j = 0; j < screenwidth + 2; j++) {
			if (snaketail[0].y == 0 && snaketail[0].x == j || snaketail[0].y == screenheight + 1 && snaketail[0].x == j ||
				snaketail[0].y == i && snaketail[0].x == 0 || snaketail[0].y == i && snaketail[0].x == screenwidth + 1)

			{
				gameover = 1;	//if snake hits wall it dies, game over and loop exits 
				return;
			}
		}
	for (int i = 1; i < snakelength; i++) {
		if (snaketail[0].y == snaketail[i].y && (snaketail[0].x == snaketail[i].x))
		{
			gameover = 1;
			return;  //if snake hits itself it dies
		}
	}
		if (board[snaketail[0].y][snaketail[0].x] == 1)
				{
			snakelength++;		// length of snake increases as it eats food
			eatfood = 1;         // if eatfood is 1, snake has eaten food
				}
		else
				{
			eatfood = 0;
			board[snaketail[snakelength - 1].y][snaketail[snakelength - 1].x] = 0;
			gotoxy(snaketail[snakelength - 1].x, snaketail[snakelength - 1].y);
			cout << " "; //if snake is moving, last part of snake body is removed and replaced with a blank space
				}

	for (int i = 1; i < snakelength; i++)
			{
		snaketail[i].x = tailsegment[i - 1].x;	//snake's body follows location
		snaketail[i].y = tailsegment[i - 1].y;	
			}

	gotoxy(snaketail[1].x, snaketail[1].y);
	cout << "o";					// head of snake becomes body of snake as it moves
	gotoxy(snaketail[0].x, snaketail[0].y);
	cout << "O";					// area in front of snake grows a head instead

	return;
		}
void snake::foodclass() //initializing the food class
{
	int x;
	int y;

	do {
		x = rand() % screenwidth + 1;
		y = rand() % screenheight + 1;
	}
	while (board[y][x] != 0); //always spawn food on empty space, if it doesnt it loops back

	board[y][x] = 1;
	if (board[y][x] == 1) {
		gotoxy(x, y);
		cout << "f";
	}
}
void snake::printgame()
{
	clearScreen();
	for (int i = 0; i <= screenheight + 1; i++)
	{
		for (int j = 0; j <= screenwidth + 1; j++)
		{
			switch (board[i][j]) //printing out the food, snake and borders
			{
			case 1:
				cout << "f";
			case 0:
				cout << " ";
				break;
			case -1:
				cout << "*";
				break;
			case -2:
				cout << "*";
				break;

			case -3:
				if (i == snaketail[0].y && j == snaketail[0].x)
					cout << "O";
				else
					cout << "o";
				break;	
			}
		}
		cout << endl;
	}
}

void snake::KeyInput() //obtains the input that the player has pressed to change the snake's direction
{
	if (_kbhit()) {//check to see if a key is pressed. IF true, execute the block of code below
		char k = _getch();//save the key the user pressed

		if (k == 'w')
			input = 0;

		if (k == 'd')
			input = 1;

		if (k == 's')
			input = 2;

		if (k == 'a')
			input = 3;

	}
}

int main()
{	
	srand(time(NULL)); 
	snake ekans;
	int k;
	int score = 0;
	INITIALSCREEN:
	ekans.initialscreen();
	int input;
	cin >> input;
	if (input == 9) {
		ekans.instructions();
		char goback;
		cin >> goback;
		if (goback == 'b') {
			goto INITIALSCREEN;
		}
	}
	if (input == 1) k = 120; //setting the delay of the snake between each iteration depending on difficulty chosen
	if (input == 2) k = 100;
	if (input == 3) k = 85;
	if (input == 4) k = 75;
	if (input == 5) k = 65;
	if (input == 6) k = 45;
	if (input == 7) k = 30;
	if (input == 8) k = 15;

	int delay = k;
	ekans.boarddetails();
	ekans.snakedetails();
	ekans.printgame();
	ekans.foodclass();

	while (gameover == 0)
	{
		ekans.KeyInput(); //input is registered here 
		ekans.snakemovement(); //snake's head changes movement depending on input
		Sleep(delay);
		if (eatfood == 1)
		{
			ekans.foodclass();
			score = score + input; //score increases depending on difficulty chosen
		}
		gotoxy(screenwidth / 2 - 10, screenheight + 5);
		cout << "Current Score:" << score << endl;
	} 

	gotoxy(screenwidth / 2 - 5, screenheight / 2 );
	cout << "GAME OVER!!";

	return 0;
}
