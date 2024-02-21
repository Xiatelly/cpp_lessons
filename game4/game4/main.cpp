// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>

// Game settings
const int rowsCount = 10;
const int columsCount = 15;
const int levelsCount = 3;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 219;
const unsigned char symbolBox = 254;
const unsigned char symbolExit = 176;
const unsigned char symbolMine = 15;
const unsigned char symbolGem = 4;
const unsigned char symbolDoor = 221;
const unsigned char symbolKey = 12;

const unsigned char levelsData[levelsCount][rowsCount][columsCount + 1] = {
	{
		"#####2#########",
		"#  X   #   X ##",
		"# X #M# X G#  #",
		"#X X  X  ## X #",
		"# X    ##  #  #",
		"####M##G   # X#",
		"#   X   XX #X #",
		"#XXX # # X   ##",
		"#1 X #   X X  #",
		"###############",
	},
	{
		"##2############",
		"# X     #  X  #",
		"# X  X  #  X  #",
		"######X #M #  #",
		"# D   X #  #  #",
		"# #GM   #GXM  #",
		"#1########## X#",
		"#  X  X   X X #",
		"#X   M   G   K#",
		"###############",
	},
	{
		"###############",
		"# 1  #  #G    #",
		"#M   #  ### XX#",
		"####    #    G#",
		"#  # XXX    M #",
		"#  X  ##  #   #",
		"#  XG     #G  #",
		"#  ## MX  #X#X2",
		"#M     X  #   #",
		"###############",
	}
};

/*const unsigned char levelData0[rowsCount][columsCount + 1] = 
//const unsigned char levelData1[rowsCount][columsCount + 1] = 
const unsigned char levelData2[rowsCount][columsCount + 1] = */

// Logic variables
HANDLE consoleHandle = 0;
bool isGameActive = true;
bool key = false;
unsigned char levelData[rowsCount][columsCount];
int heroRow = 0;
int heroColumn = 0;
int levelIndex = 0;
int gemsLeft = 0;

// Functions
void SetupSystem()
{
	srand(time(0));

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Initialize()
{
	gemsLeft = 0;
	key = false;

	// Load level
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columsCount; c++)
		{
			unsigned char symbol = levelsData[levelIndex][r][c];

			switch (symbol)
			{
				// Wall
				case '#':
				{
					levelData[r][c] = symbolWall;
					break;
				}

				// Hero
				case '1':
				{
					levelData[r][c] = symbolHero;

					// Catch Hero position
					heroRow = r;
					heroColumn = c;

					break;
				}

				// Exit
				case '2':
				{
					levelData[r][c] = symbolExit;
					break;
				}

				// Box

				case 'X':
				{
					levelData[r][c] = symbolBox;
					break;
				}

				// Mine
				case 'M':
				{
					levelData[r][c] = symbolMine;
					break;
				}

				// Gem
				case 'G':
				{
					levelData[r][c] = symbolGem;
					gemsLeft++;
					break;
				}

				// Door
				case 'D':
				{
					levelData[r][c] = symbolDoor;
					break;
				}

				// Key
				case 'K':
				{
					levelData[r][c] = symbolKey;
					break;
				}

				// Other symbols (like space)
				default:
				{
					levelData[r][c] = symbol;
					break;
				}
			}
		}
	}
}

void Render()
{
	//system("cls");

	// Move console cursor to (0,0)
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);

	printf("\n\n\t");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf("Level %i ", levelIndex + 1);

	printf("\n\n\t");
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columsCount; c++)
		{
			unsigned char symbol = levelData[r][c];
			switch (symbol)
			{
				// Walls - white
				case symbolWall:
				{
					SetConsoleTextAttribute(consoleHandle, 15);
					break;
				}
				// Hero - green
				case symbolHero:
				{
					SetConsoleTextAttribute(consoleHandle, 10);
					break;
				}
				// Boxes - yellow
				case symbolBox:
				{
					SetConsoleTextAttribute(consoleHandle, 14);
					break;
				}
				// Exit - red
				case symbolExit:
				{
					if (gemsLeft > 0)
					{
						SetConsoleTextAttribute(consoleHandle, 15);
						symbol = symbolWall;
					}
					else
					{
						SetConsoleTextAttribute(consoleHandle, 12);
					}
					break;
				}
				// Mine - violet
				case symbolMine:
				{
					SetConsoleTextAttribute(consoleHandle, 13);
					break;
				}

				// Gems - blue
				case symbolGem:
				{
					SetConsoleTextAttribute(consoleHandle, 11);
					break;
				}

				// Key - red
				case symbolKey:
				{
					SetConsoleTextAttribute(consoleHandle, 4);
					break;
				}

				// Door - red
				case symbolDoor:
				{
					SetConsoleTextAttribute(consoleHandle, 4);
					break;
				}
			}
			printf("%c", symbol);
		}
		printf("\n\t");
	}
	SetConsoleTextAttribute(consoleHandle, 7);
	printf("\n\n\tUse AWSD to move ");
	SetConsoleTextAttribute(consoleHandle, 10);
	printf("Hero");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(". R - Restart level.");
	if (gemsLeft > 0)
	{
		printf("\n\tGet %i gems to unlock exit", gemsLeft);
	}
	else
	{
		printf("\n\tSalvation is near                  ");
	}
	printf("\n\t");
	if (key)
	{
		printf("\n\tYou got the Key: ");
		SetConsoleTextAttribute(consoleHandle, 4);
		printf("%c", symbolKey);
	}
	else
	{
		printf("\n\t                                     ");
	}
	printf("\n\t");
}

void MoveHeroTo(int row, int column)
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;

	switch (destinationCell)
	{
		// Empty cell
		case ' ':
		{
			canMoveToCell = true;
			break;
		}

		// Exit cell
		case symbolExit:
		{
			if (gemsLeft == 0) 
			{
				if (levelIndex < levelsCount - 1)
				{
					levelIndex++;
					Initialize();
				}
				else
				{
					isGameActive = false;
				}
			}
			break;			
		}

		// Mine cell
		case symbolMine:
		{	
			Initialize();
			break;
		}
		
		// Door cell
		case symbolDoor:
		{
			if (key == true)
			{
				canMoveToCell = true;
			}
			break;
		}

		// Key cell
		case symbolKey:
		{
			canMoveToCell = true;
			key = true;
			break;
		}
		
		// Gem cell
		case symbolGem:
		{
			canMoveToCell = true;
			gemsLeft--;

			break;
		}
		
		// Box
		case symbolBox:
		{
			// Calculate hero move direction
			int heroDirectionR = row - heroRow;
			int heroDirectionC = column - heroColumn;

			// Check for empty space behind the box
			if (levelData[row + heroDirectionR][column + heroDirectionC] == ' ')
			{
				canMoveToCell = true;

				// Remove box symbol from previous position
				levelData[row][column] = ' ';

				// Set box symbol to new position
				levelData[row + heroDirectionR][column + heroDirectionC] = symbolBox;
			}
			else if(levelData[row + heroDirectionR][column + heroDirectionC] == symbolMine)
			{
				canMoveToCell = true;
				levelData[row][column] = ' ';
				levelData[row + heroDirectionR][column + heroDirectionC] = ' ';
			}
			break;
		}
	}

	if (canMoveToCell)
	{
		// Remove hero symbol from previous position
		levelData[heroRow][heroColumn] = ' ';

		// Set new hero position
		heroRow = row;
		heroColumn = column;

		// Set hero symbol on new position
		levelData[heroRow][heroColumn] = symbolHero;
	}
}

void Update()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	switch (inputChar)
	{
		// Up
		case 'w':
		{
			MoveHeroTo(heroRow - 1, heroColumn);
			break;
		}

		// Down
		case 's':
		{
			MoveHeroTo(heroRow + 1, heroColumn);
			break;
		}

		// Left
		case 'a':
		{
			MoveHeroTo(heroRow, heroColumn - 1);
			break;
		}

		// Right
		case 'd':
		{
			MoveHeroTo(heroRow, heroColumn + 1);
			break;
		}

		// Restart level
		case 'r':
		{
			Initialize();
			break;
		}
		// Restart go to lvl
		case '1':
		{
			levelIndex = 0;
			Initialize();
			break;
		}
		case '2':
		{
			levelIndex = 1;
			Initialize();
			break;
		}
		case '3':
		{
			levelIndex = 2;
			Initialize();
			break;
		}
	}
}

void Shutdown()
{
	system("cls");
	SetConsoleTextAttribute(consoleHandle, 10);
	printf("\n\tCongratulations! Your Hero alive!");
	printf("\n\tPress any key to continue...");
	_getch();
}

int main()
{
	SetupSystem();
	Initialize();

	do
	{
		Render();
		Update();
	} while(isGameActive);
	
	Shutdown();

	return 0;


}