
// Include libraries
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <cctype>
#include <cstring>

#include "consoleColor.h"
#include "level.h"
#include "weaponType.h"
#include "unitType.h"
#include "unitData.h"


////////////////////////////////////////////
// Constants
const int maxUnitsCount = 35;
const int healthAid = 40;
const int statusBufferSize = 200;
const int tempBufferSize = 100;

///////////////////////////////////////////////
// Logic variables
HANDLE consoleHandle = 0;
bool isGameActive = true;

unsigned char levelData[rowsCount][columnsCount];
bool levelFog[rowsCount][columnsCount];

UnitData unitData[maxUnitsCount];
int unitsCount = 0;
int heroIndex = 0;
int levelIndex = 0;

char statusBuffer[statusBufferSize] = "";
char tempBuffer[tempBufferSize] = "";

///////////////////////////////////////////////
// Functions

void SetupSystem()
{
	srand(time(0));
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Hide console cursor 
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// Fog of war
void RevealFog(int row, int column)
{
	const int radius = 2;
	for (int r = row - radius; r <= row + radius; r++)
	{
		for (int c = column - radius; c <= column + radius; c++)
		{
			if (r >= 0 && r <= rowsCount - 1)
			{
				if (c >= 0 && c <= columnsCount - 1)
				{
					levelFog[r][c] = true;
				}
			}
		}
	}
}

void Initialize()
{
	unitsCount = 0;

	// Load level
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnsCount; c++)
		{ 
			unsigned char cellSymbol = levelsData[levelIndex][r][c];

			levelData[r][c] = cellSymbol;
			switch (cellSymbol)
			{
				case CellSymbol_Hero:
					heroIndex = unitsCount;

				case CellSymbol_Orc:
				case CellSymbol_Spider:
				case CellSymbol_Skeleton:
				{
					UnitType unitType = GetUnitTypeFromCell(cellSymbol);
					unitData[unitsCount].type = unitType;
					unitData[unitsCount].row = r;
					unitData[unitsCount].column = c;
					unitData[unitsCount].weapon = GetUnitDefaultWeapon(unitType);
					unitData[unitsCount].health = GetUnitDefaultHealth(unitType);
					unitsCount++;

					break;
				}
			}

			levelFog[r][c] = false;
		}
	}
	RevealFog(unitData[heroIndex].row, unitData[heroIndex].column);
}

void Render()
{
	// Move console cursor to (0,0)
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);

	// Draw game title
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_Green);
	printf("\n\tDUNGEONS and ORCS");

	// Draw hero HP
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_Red);
	printf("\n\n\tHP: ");
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
	printf("%i ", unitData[heroIndex].health);

	//Draw hero weapon
	DamageRange damageRange = GetWeaponDamage(unitData[heroIndex].weapon);

	SetConsoleTextAttribute(consoleHandle, ConsoleColor_Cyan);
	printf("\n\n\tWeapon: ");
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_White);
	printf("%s ", GetWeaponName(unitData[heroIndex].weapon));
	SetConsoleTextAttribute(consoleHandle, ConsoleColor_Gray);
	printf("  DMG: %i-%i                ", damageRange.min, damageRange.max);

	// Draw level
	printf("\n\n\t");
	for (int r = 0; r < rowsCount; r++)
	{
		for (int c = 0; c < columnsCount; c++)
		{
			unsigned char cellSymbol = levelData[r][c];

			unsigned char renderCellSymbol = GetRenderCellSymbol(cellSymbol);
			ConsoleColor cellColor = GetRenderCellSymbolColor(cellSymbol);

			if (levelFog[r][c] == false)
			{
				SetConsoleTextAttribute(consoleHandle, ConsoleColor_DarkGray);
				printf("%c", 176);
			}
			else
			{
				SetConsoleTextAttribute(consoleHandle, cellColor);
				printf("%c", renderCellSymbol);
			}
		}
		printf("\n\t");
	}

	SetConsoleTextAttribute(consoleHandle, ConsoleColor_Yellow);
	int statusBufferLength = strlen(statusBuffer);
	for (int i = statusBufferLength; i < statusBufferSize - 1; i++) 
	{
		statusBuffer[i] = ' ';
	}
	statusBuffer[statusBufferSize - 1] = 0;
	printf("\n\t");
	printf("%s", statusBuffer);
	statusBuffer[0] = 0;
}

void MoveUnitTo(UnitData* pointerToUnitData, int row, int column)
{
	// Ignore dead units
	if (pointerToUnitData->health <= 0)
	{
		return;
	}

	unsigned char unitSymbol = levelData[pointerToUnitData->row][pointerToUnitData->column];
	unsigned char destinationCellSymbol = levelData[row][column];
	bool canMoveToCell = false;

	// All units actions
	switch (destinationCellSymbol)
	{
		// Empty cell
		case CellSymbol_Empty:
		{
			canMoveToCell = true;
			break;
		}

		// Units cells
		case CellSymbol_Hero:
		case CellSymbol_Orc:
		case CellSymbol_Skeleton:
		case CellSymbol_Spider:
		{
			UnitType destinationUnitType = GetUnitTypeFromCell(destinationCellSymbol);

			// If destination unit have other type
			if (pointerToUnitData->type != destinationUnitType)
			{
				// Find enemy unit struct
				for (int u = 0; u < unitsCount; u++)
				{
					// Ignore dead units
					if (unitData[u].health <= 0)
						continue;
					if (unitData[u].row == row && unitData[u].column == column)
					{
						// Calculate weapon damage
						int damage = GenerateDamage(GetWeaponDamage(pointerToUnitData->weapon));

						//Deal damage
						unitData[u].health = unitData[u].health - damage;
						sprintf_s(tempBuffer, tempBufferSize, " %s dealt %i damage to %s.", GetUnitName(pointerToUnitData->type), damage, GetUnitName(destinationUnitType));
						strcat_s(statusBuffer, statusBufferSize, tempBuffer);

						// If enemy unit die
						if (unitData[u].health <= 0)
						{
							levelData[row][column] = CellSymbol_Empty;
							sprintf_s(tempBuffer, tempBufferSize, " %s died.", GetUnitName(destinationUnitType));
							strcat_s(statusBuffer, statusBufferSize, tempBuffer);
						}
						break;
					}
				}
			}
			break;
		}
	}

	// Only hero actions

	if (pointerToUnitData->type == UnitType_Hero)
	{
		switch (destinationCellSymbol)
		{
			// Weapon Cell
			case CellSymbol_Stick:
			case CellSymbol_Club:
			case CellSymbol_Spear:
			case CellSymbol_Saber:
			case CellSymbol_Flail:
			case CellSymbol_Scythe:
			case CellSymbol_Claymore:
			{
				canMoveToCell = true;

				WeaponType weaponType = GetWeaponTypeFromCell(destinationCellSymbol);
				if (unitData[heroIndex].weapon < weaponType)
				{
					unitData[heroIndex].weapon = weaponType;
					sprintf_s(tempBuffer, tempBufferSize, " Hero found: %s.", GetWeaponName(weaponType));
					strcat_s(statusBuffer, statusBufferSize, tempBuffer);
				}
				break;
			}

			// AID
			case CellSymbol_Heart:
			{
				canMoveToCell = true;				
				unitData[heroIndex].health = unitData[heroIndex].health + healthAid;
				sprintf_s(tempBuffer, tempBufferSize, " Hero healed for: %ihp.", healthAid);
				strcat_s(statusBuffer, statusBufferSize, tempBuffer);
				if (unitData[heroIndex].health > GetUnitDefaultHealth(UnitType_Hero))
				{
					unitData[heroIndex].health = GetUnitDefaultHealth(UnitType_Hero);
				}
				break;
			}

			// Exit cell
			case CellSymbol_Exit:
			{
				if (levelIndex < levelsCount - 1) 
				{
					int currentHealth = unitData[heroIndex].health;
					WeaponType currentWeapon = unitData[heroIndex].weapon;
					levelIndex++;
					Initialize();
					unitData[heroIndex].health = currentHealth;
					unitData[heroIndex].weapon = currentWeapon;
				}
				else
				{
					isGameActive = false;
				}
				break;
			}
		}
	}

	if (canMoveToCell)
	{
		// Remove unit symbol from previous position
		levelData[pointerToUnitData->row][pointerToUnitData->column] = CellSymbol_Empty;

		// Set new unit position
		pointerToUnitData->row = row;
		pointerToUnitData->column = column;

		// Set hero symbol to new position
		levelData[pointerToUnitData->row][pointerToUnitData->column] = unitSymbol;

	}

}

void UpdateAI()
{
	// Pass all units
	for (int u = 0; u < unitsCount; u++)
	{
		// Ignore hero
		if (u == heroIndex)
			continue;
	
		// Ignore dead units
		if (unitData[u].health <= 0)
			continue;

		// Distance to hero
		int distanceToHeroR = abs(unitData[heroIndex].row - unitData[u].row);
		int distanceToHeroC = abs(unitData[heroIndex].column - unitData[u].column);

		// If hero near
		if((distanceToHeroR + distanceToHeroC) == 1)
		{
			// Attack hero
			MoveUnitTo(&unitData[u], unitData[heroIndex].row, unitData[heroIndex].column);			
		}
		else
		{
			// Random move
			switch (rand() % 4)
			{
				case 0:
					MoveUnitTo( &unitData[u], unitData[u].row - 1, unitData[u].column);
					break;
				case 1:
					MoveUnitTo(&unitData[u], unitData[u].row + 1, unitData[u].column);
					break;
				case 2:
					MoveUnitTo(&unitData[u], unitData[u].row, unitData[u].column - 1);
					break;
				case 3:
					MoveUnitTo(&unitData[u], unitData[u].row, unitData[u].column + 1);
					break;
			}
		}
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
			MoveUnitTo(&unitData[heroIndex], unitData[heroIndex].row - 1, unitData[heroIndex].column);
			RevealFog(unitData[heroIndex].row, unitData[heroIndex].column);
			break;

		// Down
		case 's':
			MoveUnitTo(&unitData[heroIndex], unitData[heroIndex].row + 1, unitData[heroIndex].column);
			RevealFog(unitData[heroIndex].row, unitData[heroIndex].column);
			break;

		// Left
		case 'a':
			MoveUnitTo(&unitData[heroIndex], unitData[heroIndex].row, unitData[heroIndex].column - 1);
			RevealFog(unitData[heroIndex].row, unitData[heroIndex].column);
			break;

		// Right
		case 'd':
			MoveUnitTo(&unitData[heroIndex], unitData[heroIndex].row, unitData[heroIndex].column + 1);
			RevealFog(unitData[heroIndex].row, unitData[heroIndex].column);
			break;

		// Restart level
		case 'r':
			Initialize();
			break;
		
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

	// AI turn
	UpdateAI();

	// Hero death
	if (unitData[heroIndex].health <= 0)
	{
		isGameActive = false;
	}
	else
	{
		// Health regeneration
		if (unitData[heroIndex].health < GetUnitDefaultHealth(UnitType_Hero))
		{
			unitData[heroIndex].health++;
		}
	}
}

void Shutdown()
{
	system("cls");
	printf("\n\tGame over...");
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
	} 
	while (isGameActive);
	
	Shutdown();

	return 0;
}