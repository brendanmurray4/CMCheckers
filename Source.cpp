#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

const int MAX_ARRAY_SIZE = 18;
const int MIN_ARRAY_SIZE = 8;
const int MAX_PIECES = 72;
const int NOPLAYER = 0;
const int WHITEWINS = 1;
const int REDWINS = 2;
const int NOONEWINS = 0;
const int WHITESOLDIER = 1;
const int WHITEMULE = 2;
const int WHITEKING = 3;
const int REDSOLDIER = 4;
const int REDMULE = 5;
const int REDKING = 6;
const int WHITEPLAYER = 1;
const int REDPLAYER = 2;
const int ZERO = 0;
const int ONE = 1;
const int TWO = 2;
const int THREE = 3;
const int TEN = 10;

void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex);

int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[]);

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);

bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc);

bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped);

bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard);

bool evenCheck(int x);

int main()
{
	//Initialize local main variables
	const int THREE = 3;
	int numTries = 0;
	int numRowsInBoard = 0;
	int myCMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = { 0 };
	int xIndicesMove[MAX_PIECES];
	int yIndicesMove[MAX_PIECES];
	int xIndicesJump[MAX_PIECES];
	int yIndicesJump[MAX_PIECES];
	int currentPlayer = WHITEPLAYER;
	int numJumps = 0;
	int numMove = 0;
	bool jump = false;
	int initialSquare = 0;
	int initialX = 0;
	int initialY = 0;
	int failCount = 0;
	

	//Get the user to input how many rows are in the game board, If incorrect input prompt again and tell them what was wrong with their input
	//Only let them try 3 times
	while (numTries < THREE)
	{

		cout << "Enter the number of squares along each edge of the board\n";
		cin >> numRowsInBoard;

		if (!cin)
		{
			cerr << "ERROR:  Board size is not an integer\n";
			cout << "8 <= number of squares <= 18\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			numTries++;
			continue;
		}
		else if (numRowsInBoard > MAX_ARRAY_SIZE)
		{
			cerr << "ERROR:  Board size too large\n";
			cout << "8 <= number of squares <= 18\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			numTries++;
			continue;
		}
		else if (numRowsInBoard < MIN_ARRAY_SIZE)
		{
			cerr << "ERROR:  Board size too small\n";
			cout << "8 <= number of squares <= 18\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			numTries++;
			continue;
		}
		else if (!evenCheck(numRowsInBoard))
		{
			cerr << "ERROR:  Board size odd\n";
			cout << "8 <= number of squares <= 18\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			numTries++;
			continue;
		}
		else
		{
			break;
		}
	}
	if (numTries == THREE)
	{
		cerr << "ERROR:  Too many errors entering the size of the board";
		return 1;
	}

	//Set and display the gameboard to the screen
	InitializeBoard(myCMCheckersBoard, numRowsInBoard);
	DisplayBoard(myCMCheckersBoard, numRowsInBoard);

	//Start the actual game
	while (NOPLAYER != currentPlayer)
	{
		numJumps = CountJumps(myCMCheckersBoard, numRowsInBoard, currentPlayer, xIndicesJump, yIndicesJump);
		numMove = CountMove1Squares(myCMCheckersBoard, numRowsInBoard, currentPlayer, xIndicesMove, yIndicesMove);

		//Checking if the player is unable to move

		if (currentPlayer == WHITEPLAYER && numJumps == ZERO && numMove == ZERO)
		{
			cout << "White is unable to move\nGAME OVER, Red has won\n Enter any character to close the game\n";
		}
		if (currentPlayer == REDPLAYER && numJumps == ZERO && numMove == ZERO)
		{
			cout << "Red is unable to move\nGAME OVER, Red has won\n Enter any character to close the game\n";
		}
		//If the player has moves to make then it is a regular turn
		if (currentPlayer == WHITEPLAYER && (numJumps != ZERO || numMove != ZERO))
		{
			cout << "White takes a turn\n";
		}
		if (currentPlayer == REDPLAYER && (numJumps != ZERO || numMove != ZERO))
		{
			cout << "Red takes a turn\n";
		}
		while (NOPLAYER != currentPlayer)
		{
			while (NOPLAYER != currentPlayer)
			{
				//Get the user to input the square number for the checker they want to move and see if it is a valid checker they can mvoe
				cout << "Enter the square number of the checker you want to move\n";
				cin >> initialSquare;
				if (cin.fail())
				{//Not an integer, must clear cin and try again
					cerr << "ERROR:  you did not enter an integer\nTry again\n";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					continue;
				}
				if (initialSquare < ZERO || initialSquare >(pow(numRowsInBoard, 2) - ONE))
				{//Square not on the board, allow to try again
					cerr << "ERROR:  that square is not on the board.\nTry again\n";
					continue;
				}
				initialY = initialSquare / numRowsInBoard;
				initialX = initialSquare % numRowsInBoard;
				if (currentPlayer == WHITEPLAYER)
				{//Not their checker, try again
					if (myCMCheckersBoard[initialY][initialX] == REDSOLDIER || myCMCheckersBoard[initialY][initialX] == REDMULE || myCMCheckersBoard[initialY][initialX] == REDKING)
					{
						cerr << "ERROR:  that square contains an opponent's checker\nTry again\n";
						continue;
					}
				}
				if (currentPlayer == REDPLAYER)
				{//Not their checker, try again
					if (myCMCheckersBoard[initialY][initialX] == WHITESOLDIER || myCMCheckersBoard[initialY][initialX] == WHITEMULE || myCMCheckersBoard[initialY][initialX] == WHITEKING)
					{
						cerr << "ERROR:  that square contains an opponent's checker\nTry again\n";
						continue;
					}
				}
				if (myCMCheckersBoard[initialY][initialX] == ZERO)
				{//No checker in the square, try again
					cerr << "ERROR:  that square is empty\nTry again\n";
					continue;
				}
				for (int i = 0; i < numJumps; i++)//Loop to check if the checker can jump
				{
					if (initialY == yIndicesJump[i] && initialX == xIndicesJump[i])
					{
						jump = true;
						break;
					}
					if (i == numJumps - 1)
					{
						failCount++;
						cerr << "ERROR: You can jump with another checker, you may not move your chosen checker" << endl;
						cerr << "You can jump using checkers on the following squares: " << endl;
						for (int j = 0; j < numJumps; j++)
						{
							initialSquare = yIndicesJump[j] * numRowsInBoard + xIndicesJump[j];
							cout << initialSquare << " ";
						}
						cerr << "Try again\n";
						break;
					}
				}
				if (failCount == ONE)
				{
					continue;
				}
			}
		}
	}

	return 0;
}

//Checks if an integer is an even number by using modulus and checking for a remainder when dividing by 2
bool evenCheck(int x)
{
	const int TWO = 2;
	const int ZERO = 0;
	int mod = x % 2;

	if (mod == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Sets the board with the correct initial conditions for a game of CMCheckers
void InitializeBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	//Sets the first row of the board, which is always the same, to have whitemules on the odd squares
	for (int column = ONE; column < numRowsInBoard; column += TWO)
	{
		CMCheckersBoard[0][column] = WHITEMULE;
	}
	//sets the last row of the board, which is always the same, to have red mules on the even squares
	for (int column = ZERO; column < numRowsInBoard; column += TWO)
	{
		CMCheckersBoard[numRowsInBoard - ONE][column] = REDMULE;
	}

	//For each row print the correct arrangement of soldiers either red or white
	for (int row = ONE; row < numRowsInBoard - ONE; row++)
	{
		// if the row is one of the middle two, skip it
		if (row == numRowsInBoard / TWO || row == (numRowsInBoard / TWO) - ONE)
		{
			continue;
		}
		else
		{
			//For each row after row 0 before the middle two rows that are not even, such as 1 3 5 print the arrangement of white soldiers
			if (row >= ONE && row < numRowsInBoard / TWO && !evenCheck(row))
			{
				for (int column = ZERO; column < numRowsInBoard; column++)
				{
					if (evenCheck(column))
					{
						CMCheckersBoard[row][column] = WHITESOLDIER;
					}
				}
				continue;
			}
			//For each row after row 0 before the middle two rows that are even, such as 2, 4, 6 print the arrangement of white soldiers
			if (row >= ONE && row < numRowsInBoard / TWO && evenCheck(row))
			{
				for (int column = ZERO; column < numRowsInBoard; column++)
				{
					if (!evenCheck(column))
					{
						CMCheckersBoard[row][column] = WHITESOLDIER;
					}
				}
				continue;
			}
			//For each row after the middle rows, that is even, print the correct arrangement of red soldiers
			if (row > (numRowsInBoard / TWO) && evenCheck(row))
			{
				for (int column = ZERO; column < numRowsInBoard; column++)
				{
					if (!evenCheck(column))
					{
						CMCheckersBoard[row][column] = REDSOLDIER;
					}
				}
				continue;
			}
			//For each row after the middle rows, that is odd, print the correct arrangement of red soldiers
			if (row > (numRowsInBoard / TWO) && !evenCheck(row))
			{
				for (int column = ZERO; column < numRowsInBoard; column++)
				{
					if (evenCheck(column))
					{
						CMCheckersBoard[row][column] = REDSOLDIER;
					}
				}
				continue;
			}
		}
	}
}

//This function displays the board initialized board to the screen
void DisplayBoard(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{

	//For each row and column in the array that is contained within the table, check the value and either display a string corresponding to the piece
	// or display the square number if the array has no checker in that position
	for (int i = 0; i < numRowsInBoard; i++)
	{
		for (int j = 0; j < numRowsInBoard; j++)
		{

			if (CMCheckersBoard[i][j] == ZERO)
			{
				cout << setw(4) << right << (i * TEN + j);
			}

			if (CMCheckersBoard[i][j] == WHITESOLDIER)
			{
				cout << "  WS";
			}

			if (CMCheckersBoard[i][j] == WHITEMULE)
			{
				cout << "  WM";
			}

			if (CMCheckersBoard[i][j] == WHITEKING)
			{
				cout << "  WK";
			}

			if (CMCheckersBoard[i][j] == REDSOLDIER)
			{
				cout << "  RS";
			}

			if (CMCheckersBoard[i][j] == REDMULE)
			{
				cout << "  RM";
			}

			if (CMCheckersBoard[i][j] == REDKING)
			{
				cout << "  RK";
			}

			if (j == numRowsInBoard - 1)
			{
				cout << "\n";
				continue;
			}
		}
	}
}

//This function will count the number of jumps that the player whose turn it is can make, it also will record into two different arrays the position of the checker
int CountJumps(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	int numJumps = 0;
	//Running two loops to set each array to the value -1
	for (int i = 0; i < numRowsInBoard; i++)
	{
		xLocArray[i] = -1;
	}

	for (int i = 0; i < numRowsInBoard; i++)
	{
		yLocArray[i] = -1;
	}

	//This loop checks each location in the array using the isJump function, each time the function returns true it increments
	// the number of jumps, and also records the x y coordinates in the two different arrays
	for (int yLoc = 0; yLoc < numRowsInBoard; yLoc++)
	{
		for (int xLoc = 0; xLoc < numRowsInBoard; xLoc++)
		{
				
			if (IsJump(CMCheckersBoard, numRowsInBoard, player, xLoc, yLoc))
			{
				yLocArray[numJumps] = yLoc;
				xLocArray[numJumps] = xLoc;
				numJumps++;
				continue;
			}
		}
	}

return numJumps;
}
	
// This function takes a location on the board, and based on what piece is in that place and what player it is 
bool IsJump(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{

	if (player == WHITEPLAYER)
	{
		//Kings are the only piece that can move backwards relative to the user, so they have their own section
		if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an opponent checker, then checks if the next checker after is open, then checks to ensure it is not going over the north and south boundaries of the board, 
			//Jumping down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc + ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc + TWO][ONE] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc - ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc - TWO][ONE] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping down to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][ZERO] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping down to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc + ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - ONE] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping up to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc - ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - ONE] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping down to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][xLoc + TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping down to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][xLoc - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][xLoc + TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping up to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][xLoc - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//If none of the statements complete, the piece cannot jump
			return false;
		}
		//since soldiers and mules have the same movement rules they are contained within the same if statment. White team can only move down since they start up top
		if (CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER || CMCheckersBoard[yLoc][xLoc] == WHITEMULE)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an opponent checker, then checks if the next checker after is open, then checks to ensure it is not going over the north and south boundaries of the board, 
			//Jumping down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc + ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc + TWO][ONE] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping down to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping down to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc + ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - ONE] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping down to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][xLoc + TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping down to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDMULE || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDSOLDIER || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == REDKING) && (CMCheckersBoard[yLoc + TWO][xLoc - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// If none of the if statements complete, the piece cannot jump
			return false;
		}
		//If the location called by the function did not contain a white piece
		return false;
	}
	

	if(player == REDPLAYER)
	{
		if (CMCheckersBoard[xLoc][yLoc] == REDKING)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an opponent checker, then checks if the next checker after is open, then checks to ensure it is not going over the north and south boundaries of the board, 
			//Jumping down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == WHITEMULE || CMCheckersBoard[yLoc + ONE][ZERO] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][ONE] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == WHITEMULE || CMCheckersBoard[yLoc - ONE][ZERO] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][ONE] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITEMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITEMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping down to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITEMULE || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITEMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][ZERO] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping down to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == WHITEMULE || CMCheckersBoard[yLoc + ONE][ZERO] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][ZERO] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][numRowsInBoard - ONE] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			// Jumping up to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == WHITEMULE || CMCheckersBoard[yLoc - ONE][ZERO] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - ONE] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping down to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == WHITEMULE || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][xLoc + ONE] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][xLoc + TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping down to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == WHITEMULE || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc + ONE][xLoc - ONE] == WHITEKING) && (CMCheckersBoard[yLoc + TWO][xLoc - TWO] == 0) && (yLoc != numRowsInBoard - ONE && yLoc != numRowsInBoard - TWO))
			{
				return true;
			}
			//Jumping up to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == WHITEMULE || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][xLoc + TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping up to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == WHITEMULE || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == WHITESOLDIER || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == WHITEKING) && (CMCheckersBoard[yLoc - TWO][xLoc - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//If none of the statements complete, the piece cannot jump
			return false;
		}
		// Since the mule and soldier have the same movement rules they can follow the same if statement. They both can only move up on the board since the red team starts on the bottom
		if (CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDSOLDIER)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an opponent checker, then checks if the next checker after is open, then checks to ensure it is not going over the north and south boundaries of the board, 
			//Jumping up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc - ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc - TWO][ONE] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping up to the right if at the second last square of the board
			if ((xLoc == numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][ZERO] == ZERO) && (yLoc != ZERO || yLoc != ONE))
			{
				return true;
			}
			// Jumping up to the left if at the second last square of the board (LEFT)
			if ((xLoc == ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == REDMULE || CMCheckersBoard[yLoc - ONE][ZERO] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][ZERO] == REDKING) && (CMCheckersBoard[yLoc - TWO][numRowsInBoard - ONE] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping up to the right if not at the edge of the board
			if ((xLoc != numRowsInBoard - ONE && xLoc != numRowsInBoard - TWO) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][xLoc + ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][xLoc + TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//Jumping up to the left if not at the edge of the board
			if ((xLoc != ZERO && xLoc != ONE) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDMULE || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDSOLDIER || CMCheckersBoard[yLoc - ONE][xLoc - ONE] == REDKING) && (CMCheckersBoard[yLoc - TWO][xLoc - TWO] == 0) && (yLoc != ZERO && yLoc != ONE))
			{
				return true;
			}
			//If none of the statements complete, the piece cannot jump
			return false;
		}
		//If the location given by the function call did not have a piece of the red team
		return false;
	}

	//If when the function was called, the integer value given for which player was not WHITEPLAYER or REDPLAYER the function returns false to prevent any crashes
	return false;
}

bool IsMove1Square(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLoc, int yLoc)
{
	
	if (player == WHITEPLAYER)
	{
		//Kings are the only piece that can move backwards relative to the user, so they have their own section
		if (CMCheckersBoard[yLoc][xLoc] == WHITEKING)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an open, then checks to ensure it is not going over the respective north and south boundaries of the board,
			//Moving down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving down to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving down to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//if none of the if statements complete, then the king cannot move
			return false;
		}
		if (CMCheckersBoard[yLoc][xLoc] == WHITEMULE || CMCheckersBoard[yLoc][xLoc] == WHITESOLDIER)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an open, then checks to ensure it is not going over the respective north and south boundaries of the board,
			//Moving down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}				//Moving down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
					return true;
			}
			//Moving down to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving down to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//if none of the if statements complete, then the checker cannot move
			return false;
		}
	}

	if (player == REDPLAYER)
	{
		if (CMCheckersBoard[yLoc][xLoc] == REDKING)
		{
			//Checks if it is at the edge of the board, then checks if the next diagonal square is an open, then checks to ensure it is not going over the respective north and south boundaries of the board,
			//Moving down to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][ZERO] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving down to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc + ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving down to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc + ONE][xLoc - ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//Moving down to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc + ONE][xLoc + ONE] == ZERO) && (yLoc != numRowsInBoard - ONE))
			{
				return true;
			}
			//if none of the if statements complete, then the king cannot move
			return false;
		}
		if (CMCheckersBoard[yLoc][xLoc] == REDMULE || CMCheckersBoard[yLoc][xLoc] == REDSOLDIER)
		{
			//Moving up to the right if at the right edge of the board
			if ((xLoc == numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][ZERO] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the left if at the left edge of the board
			if ((xLoc == ZERO) && (CMCheckersBoard[yLoc - ONE][numRowsInBoard - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the left if not at the left edge
			if ((xLoc != ZERO) && (CMCheckersBoard[yLoc - ONE][xLoc - ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//Moving up to the right if not at the right edge
			if ((xLoc != numRowsInBoard - ONE) && (CMCheckersBoard[yLoc - ONE][xLoc + ONE] == ZERO) && (yLoc != ZERO))
			{
				return true;
			}
			//If none of the if statements complete, then the king cannot move
			return false;
		}
	}
	//if the function does not take a proper value for player, then returns false
	return false;
}

//This function takes the arrays generated by CountJumps or the arrays generated by CountMove1Squares and checks if the checker on the board is in the arrays
bool CheckList(int inArray1[], int inArray2[], int xIndex, int yIndex)
{
	//Initializing local variables
	int i = 0;
	//Checking each element each array to see if both match the provided x and y coordinates, if any of them do, the function returns true
	// If none match, it will eventually return false
	while (i < MAX_ARRAY_SIZE)
	{
		if (inArray1[i] == xIndex && inArray2[i] == yIndex)
		{
			return true;
		}
		else
		{
			i++;
			continue;
		}
	}
	return false;
}

// An adapted version of the countJumps function that instead counts how many movements of 1 square are possible
int CountMove1Squares(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int xLocArray[], int yLocArray[])
{
	int numMoves = 0;
	//Running two loops to set each array to the value -1
	for (int i = 0; i < numRowsInBoard; i++)
	{
		xLocArray[i] = -1;
	}

	for (int i = 0; i < numRowsInBoard; i++)
	{
		yLocArray[i] = -1;
	}

	//This loop checks each location in the array using the IsMove1Square function, each time the function returns true it increments
	// the number of available movements by 1 square, and also records the x y coordinates in the two different arrays
	for (int yLoc = 0; yLoc < numRowsInBoard; yLoc++)
	{
		for (int xLoc = 0; xLoc < numRowsInBoard; xLoc++)
		{

			if (IsMove1Square(CMCheckersBoard, numRowsInBoard, player, xLoc, yLoc))
			{
				yLocArray[numMoves] = yLoc;
				xLocArray[numMoves] = xLoc;
				numMoves++;
				continue;
			}
		}
	}

	return numMoves;
}\

//This function moves the specified checker if it meets every condition required
bool MakeMove(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard, int player, int fromSquareNum, int toSquareNum, bool &jumped)
{
	//Defining all local variables
	int yDistance = 0;
	int xDistance = 0;
	int initialXLoc = 0;
	int finalXLoc = 0;
	int initialYLoc = 0;
	int finalYLoc = 0;
	string endProgram = " " ;

	//Since we know the square number it started on, and what square number we want to reach, we can use integer division to get the row
	initialYLoc = fromSquareNum / numRowsInBoard;
	finalYLoc = toSquareNum / numRowsInBoard;
	//Since we know the square number it started on, and what square number we want to reach, we can use modulus to get the column
	initialXLoc = fromSquareNum % numRowsInBoard;
	finalXLoc = fromSquareNum % numRowsInBoard;
	//The Y coordinate never wraps around so it does not need special cases
	yDistance = abs(finalYLoc - initialYLoc);

	//First check to see if the player wants the checker to wrap around the board and if they do set the appropriate xDistance rather than using the distance formula
	//This does not need to be done for yDistance since the player is unable to wrap around the north and south side
	//If it initally was on the left of the board and is now on the right
	if (initialXLoc == ZERO && finalXLoc == numRowsInBoard - ONE)
	{
		xDistance = ONE;
	}
	//If it initially was on the right of the board and is now on the left
	else if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ZERO)
	{
		xDistance = ONE;
	}
	// If it was initially on the left of the board but jumped and is now two spaces from the right
	else if (initialXLoc == ZERO && finalXLoc == numRowsInBoard - TWO)
	{
		xDistance = TWO;
	}
	//If it was initially on the right side of the board but jumped and is now two spaces from the left
	else if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE)
	{
		xDistance = TWO;
	}
	//If it was initially two from the left and ends on the right
	else if (initialXLoc == ONE && finalXLoc == numRowsInBoard - ONE)
	{
		xDistance = TWO;
	}
	//If it was initially two from the right and ends to the left
	else if (initialXLoc == numRowsInBoard - TWO && finalXLoc == ZERO)
	{
		xDistance = TWO;

	}
	//If none of the above statements are true then the checker did not wrap around the board, and the distance formula works
	else
	{
		xDistance = abs(finalXLoc - initialXLoc);
	}

	//Since the checker is moving diagonally xDistance and yDistance must be equal
	if (yDistance != xDistance)
	{
		return false;
	}

	//Since xDistance and yDistance can never be greater than two, or the move is illegal, check for that
	if (yDistance > TWO || xDistance > TWO)
	{
		return false;
	}

	if (yDistance == TWO)
	{
		jumped = ONE;
	}

	//Since the checkers of different players have different rules we must check which player is moving
	if (player == REDPLAYER)
	{
		//If the distance is two, then the checker has to attempt a jump
		if (xDistance == TWO)
		{
			//If the final value is greater than the initial value that means the checker is trying to move up, and it also is moving down two spaces meaning it must have jumped, so all checkers can do this
			if (initialYLoc > finalYLoc)
			{
				//These if statements check for various failure cases where the user either jumps when there is no checker to jump over, or attempts to jump to a space that is occupied by another checker
				
				if (finalXLoc == numRowsInBoard - TWO && initialXLoc == ZERO && ((CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO)))
				{
					cerr << "Error; illegal move\n";
					return false;
				}
				if ( finalXLoc == 0 && initialXLoc == numRowsInBoard - TWO && ((CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - 1] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}
				if(finalXLoc == numRowsInBoard - ONE && initialXLoc == ONE && ((CMCheckersBoard[initialYLoc - ONE][ZERO] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][ZERO] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][ZERO] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == 0)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}
				if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE && ((CMCheckersBoard[initialXLoc - ONE][ZERO] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][ZERO] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][ZERO] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == 0)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}

				//These if statements check if the checker did not jump over something
				if (finalXLoc < initialXLoc && !(initialXLoc == numRowsInBoard - ONE) && !(initialXLoc == numRowsInBoard - TWO))
				{
					if ((CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move\n";
						return false;
					}
				}

				if (finalXLoc - initialXLoc < ZERO && initialXLoc != numRowsInBoard - ONE && initialXLoc != numRowsInBoard - TWO)
				{
					if ((CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITEMULE && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move\n";
						return false;
					}
				}
			}
			//Since the red mule and soldier cannot move in the other direction, this if checks for that
			if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == REDSOLDIER || CMCheckersBoard[initialYLoc][initialXLoc] == REDMULE))
			{
				cerr << "Error;  illegal move\n";
				return false;
			}
			//This controls fail cases for the redking piece
			if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == REDKING))
			{
				if (initialXLoc == 0 && finalXLoc == numRowsInBoard - TWO && ((CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != WHITEKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != 0)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE && ((CMCheckersBoard[initialYLoc + ONE][ZERO] != WHITESOLDIER && CMCheckersBoard[initialYLoc + 1][0] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][ZERO] != WHITEKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != ZERO)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == ONE && finalXLoc == numRowsInBoard - ONE && ((CMCheckersBoard[initialYLoc + ONE][ZERO] != WHITESOLDIER && CMCheckersBoard[initialYLoc + ONE][ZERO] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][ZERO] != WHITEKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != 0)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == numRowsInBoard - TWO && finalXLoc == 0 && ((CMCheckersBoard[initialYLoc + 1][numRowsInBoard - ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != WHITEKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != ZERO)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (finalXLoc - initialXLoc > ZERO && initialXLoc != ZERO && initialXLoc != ONE)
				{
					if ((CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move" << endl;
						return false;
					}
				}
				if (finalXLoc - initialXLoc < ZERO && initialXLoc != numRowsInBoard - ONE && initialXLoc != numRowsInBoard - TWO)
				{
					if ((CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != WHITESOLDIER && CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != WHITEMULE && CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != WHITEKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move" << endl;
						return false;
					}
				}
			}
		}
		else if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == REDMULE || CMCheckersBoard[initialYLoc][initialXLoc] == REDSOLDIER))
		{
			cerr << "Error;  illegal move" << endl;
			return false;
		}
	}

//Since the checkers of different players have different rules we must check which player is moving
	if (player == WHITEPLAYER)
	{
		//If the distance is two, then the checker has to attempt a jump
		if (xDistance == TWO)
		{
			//If the final value is greater than the initial value that means the checker is trying to move up, and it also is moving down two spaces meaning it must have jumped, so all checkers can do this
			if (initialYLoc > finalYLoc)
			{
				//These if statements check for various failure cases where the user either jumps when there is no checker to jump over, or attempts to jump to a space that is occupied by another checker

				if (finalXLoc == numRowsInBoard - TWO && initialXLoc == ZERO && ((CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != REDMULE && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO)))
				{
					cerr << "Error; illegal move\n";
					return false;
				}
				if (finalXLoc == 0 && initialXLoc == numRowsInBoard - TWO && ((CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] != REDMULE && CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - 1] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}
				if (finalXLoc == numRowsInBoard - ONE && initialXLoc == ONE && ((CMCheckersBoard[initialYLoc - ONE][ZERO] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][ZERO] != REDMULE && CMCheckersBoard[initialYLoc - ONE][ZERO] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == 0)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}
				if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE && ((CMCheckersBoard[initialXLoc - ONE][ZERO] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][ZERO] != REDMULE && CMCheckersBoard[initialYLoc - ONE][ZERO] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == 0)))
				{
					cerr << "Error;  illegal move\n";
					return false;
				}

				//These if statements check if the checker did not jump over something
				if (finalXLoc < initialXLoc && !(initialXLoc == numRowsInBoard - ONE) && !(initialXLoc == numRowsInBoard - TWO))
				{
					if ((CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDMULE && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move\n";
						return false;
					}
				}

				if (finalXLoc - initialXLoc < ZERO && initialXLoc != numRowsInBoard - ONE && initialXLoc != numRowsInBoard - TWO)
				{
					if ((CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDMULE && CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move\n";
						return false;
					}
				}
			}
			//Since the red mule and soldier cannot move in the other direction, this if checks for that
			if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == WHITESOLDIER || CMCheckersBoard[initialYLoc][initialXLoc] == WHITEMULE))
			{
				cerr << "Error;  illegal move\n";
				return false;
			}
			//This controls fail cases for the redking piece
			if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == WHITEKING))
			{
				if (initialXLoc == 0 && finalXLoc == numRowsInBoard - TWO && ((CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != REDMULE && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != REDKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != 0)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE && ((CMCheckersBoard[initialYLoc + ONE][ZERO] != REDSOLDIER && CMCheckersBoard[initialYLoc + 1][0] != REDMULE && CMCheckersBoard[initialYLoc + ONE][ZERO] != REDKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != ZERO)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == ONE && finalXLoc == numRowsInBoard - ONE && ((CMCheckersBoard[initialYLoc + ONE][ZERO] != REDSOLDIER && CMCheckersBoard[initialYLoc + ONE][ZERO] != REDMULE && CMCheckersBoard[initialYLoc + ONE][ZERO] != REDKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != 0)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (initialXLoc == numRowsInBoard - TWO && finalXLoc == 0 && ((CMCheckersBoard[initialYLoc + 1][numRowsInBoard - ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != REDMULE && CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] != REDKING) || (CMCheckersBoard[finalYLoc][finalXLoc] != ZERO)))
				{
					cerr << "Error;  illegal move" << endl;
					return false;
				}
				if (finalXLoc - initialXLoc > ZERO && initialXLoc != ZERO && initialXLoc != ONE)
				{
					if ((CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != REDMULE && CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move" << endl;
						return false;
					}
				}
				if (finalXLoc - initialXLoc < ZERO && initialXLoc != numRowsInBoard - ONE && initialXLoc != numRowsInBoard - TWO)
				{
					if ((CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != REDSOLDIER && CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != REDMULE && CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] != REDKING) || !(CMCheckersBoard[finalYLoc][finalXLoc] == ZERO))
					{
						cerr << "Error;  illegal move" << endl;
						return false;
					}
				}
			}
		}
		else if (finalYLoc > initialYLoc && (CMCheckersBoard[initialYLoc][initialXLoc] == WHITEMULE || CMCheckersBoard[initialYLoc][initialXLoc] == WHITESOLDIER))
		{
			cerr << "Error;  illegal move" << endl;
			return false;
		}
	}

	CMCheckersBoard[finalYLoc][finalXLoc] = CMCheckersBoard[initialYLoc][initialXLoc];
	CMCheckersBoard[initialYLoc][initialXLoc] = ZERO;
	
	//If xDistance is two it must have jumped so the checker has to be removed
	if (xDistance == TWO)
	{
		//If the checker is moving down
		if (finalYLoc > initialYLoc)
		{
			//right off side
			if (finalXLoc == numRowsInBoard - TWO && initialXLoc == 0)
			{
				CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] = ZERO;
			}//right off side
			else if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE)
			{
				CMCheckersBoard[initialYLoc + ONE][ZERO] == ZERO;
			}//left off side
			else if (initialXLoc == ONE && finalXLoc == numRowsInBoard - ONE)
			{
				CMCheckersBoard[initialYLoc + ONE][ZERO] = ZERO;
			}//left off side
			else if (initialXLoc == numRowsInBoard - TWO && finalXLoc == ZERO)
			{
				CMCheckersBoard[initialYLoc + ONE][numRowsInBoard - ONE] == ZERO;
			}//jumping right
			else if (finalXLoc > initialXLoc)
			{
				CMCheckersBoard[initialYLoc + ONE][initialXLoc + ONE] = ZERO;
			}//jumping left
			else if (finalXLoc < initialXLoc)
			{
				CMCheckersBoard[initialYLoc + ONE][initialXLoc - ONE] = ZERO;
			}
		}
		//If the checker is jumping up
		if (initialYLoc > finalYLoc)
		{
			//right off side
			if (finalXLoc == numRowsInBoard - TWO && initialXLoc == 0)
			{
				CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] = ZERO;
			}//right off side
			else if (initialXLoc == numRowsInBoard - ONE && finalXLoc == ONE)
			{
				CMCheckersBoard[initialYLoc - ONE][ZERO] == ZERO;
			}//left off side
			else if (initialXLoc == ONE && finalXLoc == numRowsInBoard - ONE)
			{
				CMCheckersBoard[initialYLoc - ONE][ZERO] = ZERO;
			}//left off side
			else if (initialXLoc == numRowsInBoard - TWO && finalXLoc == ZERO)
			{
				CMCheckersBoard[initialYLoc - ONE][numRowsInBoard - ONE] == ZERO;
			}//jumping right
			else if (finalXLoc > initialXLoc)
			{
				CMCheckersBoard[initialYLoc - ONE][initialXLoc + ONE] = ZERO;
			}//jumping left
			else if (finalXLoc < initialXLoc)
			{
				CMCheckersBoard[initialYLoc - ONE][initialXLoc - ONE] = ZERO;
			}
		}
	}

	if (CMCheckersBoard[finalYLoc][finalXLoc] == WHITEMULE && finalYLoc == numRowsInBoard - ONE)
	{
		CMCheckersBoard[finalYLoc][finalXLoc] = WHITEKING;
		DisplayBoard(CMCheckersBoard, numRowsInBoard);
		cout << "White has created a Mule King, Red wins the game\nEnter any character to terminate the game then press the enter key\n";
		if (cin >> endProgram)
		{
			exit(0);
		}
	}
	if (CMCheckersBoard[finalYLoc][finalXLoc] == WHITESOLDIER)
	{
		CMCheckersBoard[finalYLoc][finalXLoc] == WHITEKING;
		return true;
	}
	if (CMCheckersBoard[finalYLoc][finalXLoc] == REDMULE)
	{
		CMCheckersBoard[finalYLoc][finalXLoc] = REDKING;
		DisplayBoard(CMCheckersBoard, numRowsInBoard);
		cout << "Red has created a Mule King, Red wins the game\nEnter any character to terminate the game then press the enter key\n";
		if (cin >> endProgram)
		{
			exit(1);
		}
	}
	if (finalYLoc == ZERO && CMCheckersBoard[finalYLoc][finalXLoc] == REDSOLDIER)
	{
		CMCheckersBoard[finalYLoc][finalXLoc] = REDKING;
		return true;
	}

	return true;
}
//This function looks to see if a player wins by losing all of their mules or capturing all of the other team's soldiers and kings
bool CheckWin(int CMCheckersBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int numRowsInBoard)
{
	//Define all local variables
	int numWhiteMules = 0;
	int numWhiteSoldiers = 0;
	int numWhiteKings = 0;
	int numRedMules = 0;
	int numRedSoldiers = 0;
	int numRedKings = 0;

	//Counts the number of each checker in the board
	for (int row = 0; row < numRowsInBoard; row++)
	{
		for (int column = 0; column < numRowsInBoard; column++)
		{
			//Counts whitesoldiers
			if (CMCheckersBoard[row][column] == WHITESOLDIER)
			{
				numWhiteSoldiers++;
			}//counts whitemules
			if (CMCheckersBoard[row][column] == WHITEMULE)
			{
				numWhiteMules++;
			}//counts whitekings
			if (CMCheckersBoard[row][column] == WHITEKING)
			{
				numWhiteKings++;
			}//Counts redsoldiers
			if (CMCheckersBoard[row][column] == REDSOLDIER)
			{
				numRedSoldiers++;
			}//counts redmules
			if (CMCheckersBoard[row][column] == REDMULE)
			{
				numRedMules++;
			}//counts redkings
			if (CMCheckersBoard[row][column] == REDKING)
			{
				numRedKings++;
			}
		}
	}
	if (numRedMules == ZERO)
	{
		cout << "The Red Player has won the game by losing all of the Red Mules\n";
		return true;
	}
	if (numWhiteMules == ZERO)
	{
		cout << "The White Player has won the game by losing all of the White Mules\n";
		return true;
	}
	if (numWhiteSoldiers + numWhiteKings == ZERO)
	{
		cout << "The Red Player has won by capturing all of the white players soldiers and kings\n";
		return true;
	}
	if (numRedSoldiers + numRedKings == ZERO)
	{
		cout << "The White Player has won by capturing all of the red players soldiers and kings\n";
		return true;
	}

	return false;

}