///////////////////////////////////
// Include
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>
#include <cctype>
#include <Windows.h>
#include "consoleColor.h"

#include "renderSystem.h"
#include "input.h"
#include "level.h"
#include "unitData.h"


/////////////////////////////////
// Constans
const int maxUnitCounts = 35;
const float cellBeginValue = 0.001f;
const float cellEndValue = 0.999f;


/////////////////////////////////////
// Logic variables
bool isGameActive = true;
clock_t clockLastFrame = 0;

int framesCounter = 0;
float framesTimeCounter = 0;
int fps = 0;
int heroScore = 0;
int levelIndex = 0;

unsigned char levelData[levelRows][levelColumns];

UnitData unitsData[maxUnitCounts];
int unitsCount = 0;
int heroIndex = 0;
int hero2Index = 0;

/////////////////////////////////////
// Functions
void SetupSystem()
{
	srand(time( 0 ));

	// Initialize render system
	RenderSystemInitialize();
}

void Initialize()
{
	// Set clockLastFrame start value
	clockLastFrame = clock();

	unitsCount = 0;
	heroScore = 0;

	// Load level
	for (int r = 0; r < levelRows; r++)
	{
		for (int c = 0; c < levelColumns; c++)
		{
			unsigned char cellSymbol = levelData0[levelIndex][r][c];

			levelData[r][c] = cellSymbol;

			switch (cellSymbol)
			{
				case CellSymbol_Hero:
					heroIndex = unitsCount;

				case CellSymbol_Hero2:
					if (cellSymbol == CellSymbol_Hero2)
						hero2Index = unitsCount;

				case CellSymbol_Goomba:
				case CellSymbol_Koopa:
				{
					UnitType unitType = GetUnitTypeFromCell(cellSymbol);
					unitsData[unitsCount].type = unitType;
					unitsData[unitsCount].y = float(r);
					unitsData[unitsCount].x = float(c);
					unitsData[unitsCount].health = 1;
					unitsData[unitsCount].ySpeed = 0.0f;
					unitsData[unitsCount].xSpeed = 0.0f;
					unitsData[unitsCount].xOrder = UnitOrder_None;
					unitsData[unitsCount].yOrder = UnitOrder_None;
					unitsCount++;

					break;
				}
			}
		}
	}
}

void Render()
{
	// Start frame
	RenderSystemClear();

	// Draw level
	for (int r = 0; r < levelRows; r++)
	{
		for (int c = 0; c < levelColumns; c++)
		{
			unsigned char cellSymbol = levelData[r][c];

			unsigned char renderSymbol = GetRenderCellSymbol(cellSymbol);
			ConsoleColor symbolColor = GetRenderCellSymbolColor(cellSymbol);
			ConsoleColor backgroundColor = GetRenderCellSymbolBackgroundColor(cellSymbol);

			if (cellSymbol == CellSymbol_Hero)
				symbolColor = GetRenderHeroColor(unitsData[heroIndex].health);
			else 
			if (cellSymbol == CellSymbol_Hero2)
				symbolColor = GetRenderHero2Color(unitsData[hero2Index].health);

			RenderSystemDrawChar(r, c, renderSymbol, symbolColor, backgroundColor);
		}
	}

	// Draw FPS
	char textBuffer[32];
	sprintf_s(textBuffer, "FPS: %i", fps);
	RenderSystemDrawText(0, 0, textBuffer, ConsoleColor_Gray, ConsoleColor_Black);

	// Draw Score
	char scoreBuffer[50];
	sprintf_s(scoreBuffer, "Score: %i", heroScore);
	RenderSystemDrawText(0, 20, scoreBuffer, ConsoleColor_DarkCyan, ConsoleColor_Black);

	// End frame
	RenderSystemFlush();
}

UnitData* GetUnitAt(int row, int column)
{
	for (int u = 0; u < unitsCount; u++)
	{
		if (unitsData[u].health <= 0)
			continue;

		if (int(unitsData[u].y) == row && int(unitsData[u].x) == column)
			return &unitsData[u];
	}

	return 0;
}

void KillUnit(UnitData* pointerToUnitData);

void EatMushroom(int row, int column, UnitData* pointerToUnitData)
{
	heroScore = heroScore + 10;
	UnitData* unitData = GetUnitAt(row, column);
	KillUnit(unitData);

	if (pointerToUnitData->health < 2)
		pointerToUnitData->health = 2;
}

void KillUnit(UnitData* pointerToUnitData)
{
	pointerToUnitData->health = 0;
	int row = int(pointerToUnitData->y);
	int column = int(pointerToUnitData->x);
	levelData[row][column] = CellSymbol_Empty;
}

void UpdateAI()
{
	for (int u = 0; u < unitsCount; u++)
	{
		// Ignore hero
		if (u == heroIndex || u == hero2Index)
			continue;


		// Ignore dead units
		if (unitsData[u].health <= 0)
			continue;

		int row = int(unitsData[u].y);
		int column = int(unitsData[u].x);

		if (unitsData[u].xOrder == UnitOrder_None)
		{
			if (unitsData[u].type != UnitType_TurnedKoopa)
			{
				// Start move to empty cell
				if (levelData[row][column - 1] == CellSymbol_Empty)
					unitsData[u].xOrder = UnitOrder_Backward;
				else
					unitsData[u].xOrder = UnitOrder_Forward;
			}
		}
		else
		{
			if (unitsData[u].xOrder == UnitOrder_Backward)
			{
				unsigned char cellLeft = levelData[row][column - 1];
				UnitType unitType = GetUnitTypeFromCell(cellLeft);

				// Can not move cell left
				if ((unitsData[u].x <= (column + cellBeginValue)) && cellLeft != CellSymbol_Empty && unitType == UnitType_None)
					unitsData[u].xOrder = UnitOrder_Forward;
			}
			else
			{
				unsigned char cellRight = levelData[row][column + 1];
				UnitType unitType = GetUnitTypeFromCell(cellRight);

				// Can not move cell right
				if ((unitsData[u].x >= (column + cellEndValue)) && cellRight != CellSymbol_Empty && unitType == UnitType_None)
					unitsData[u].xOrder = UnitOrder_Backward;
			}
		}
	}
}

bool MoveUnitTo(UnitData* pointerToUnitData, float newX, float newY)
{
	// Ignore dead units
	if (pointerToUnitData->health <= 0)
		return false;

	bool canMoveToCell = false;

	int newRow = int(newY);
	int newColumn = int(newX);
	int oldRow = int(pointerToUnitData->y);
	int oldColumn = int(pointerToUnitData->x);

	unsigned char unitSymbol = levelData[oldRow][oldColumn];
	unsigned char destinationCellSymboll = levelData[newRow][newColumn];

	int directionRow = newRow - oldRow;
	int directionColumn = newColumn - oldColumn;

	// All units actions
	switch (destinationCellSymboll)
	{
		case CellSymbol_Empty:
			canMoveToCell = true;
			break;

		case CellSymbol_Abyss:
			KillUnit(pointerToUnitData);
			break;

		case CellSymbol_Jump:
			canMoveToCell = false;

			if (directionRow > 0)
			{
				pointerToUnitData->ySpeed = -20;
			}
			break;

		case CellSymbol_Box:
			
			if (directionRow < 0 && levelData[newRow - 1][newColumn] == CellSymbol_Empty)
			{
				levelData[newRow - 1][newColumn] = CellSymbol_Crystal;
				levelData[newRow][newColumn] = CellSymbol_OpenedBox;
			}
			break;

		case CellSymbol_MushroomBox:
			
			if (directionRow < 0 && levelData[newRow - 1][newColumn] == CellSymbol_Empty)
			{
				levelData[newRow - 1][newColumn] = CellSymbol_Mushroom;
				unitsData[unitsCount].type = UnitType_Mushroom;
				unitsData[unitsCount].y = float(newRow - 1);
				unitsData[unitsCount].x = float(newColumn);
				unitsData[unitsCount].health = 1;
				unitsData[unitsCount].ySpeed = 0.0f;
				unitsData[unitsCount].xSpeed = 0.0f;
				unitsData[unitsCount].xOrder = UnitOrder_Forward;
				unitsData[unitsCount].yOrder = UnitOrder_None;
				unitsCount++;
				levelData[newRow][newColumn] = CellSymbol_OpenedBox;
			}
			break;
	}

	// Only hero actions
	if (pointerToUnitData->type == UnitType_Hero || pointerToUnitData->type == UnitType_Hero2)
	{
		switch (destinationCellSymboll)
		{
		case CellSymbol_Exit:
		{
			if (levelIndex < levelsCount - 1)
			{
				int tempScore = heroScore;
				int currentHealth = unitsData[heroIndex].health;
				int currentHealth2 = unitsData[hero2Index].health;
				levelIndex++;
				Initialize();
				unitsData[heroIndex].health = currentHealth;
				unitsData[hero2Index].health = currentHealth2;
				heroScore = tempScore;
			}
			else
			{
				isGameActive = false;
			}
			break;
		}
		case CellSymbol_Crystal:
			canMoveToCell = true;
			heroScore = heroScore + 5;
			break;

		case CellSymbol_Mushroom:
		{
			canMoveToCell = true;
			EatMushroom(newRow, newColumn, pointerToUnitData);
			break;
		}

		case CellSymbol_BrickWall:
			if ((directionRow < 0) && (pointerToUnitData->health > 1))
				levelData[newRow][newColumn] = CellSymbol_Empty;
			break;

		case CellSymbol_Goomba:
			
			if (directionRow > 0)
			{
				UnitData* unitData = GetUnitAt(newRow, newColumn);
				if (unitData != 0)
				{
					KillUnit(unitData);
					heroScore= heroScore + 7;
					pointerToUnitData->ySpeed = -GetUnitJumpSpeed(pointerToUnitData->type);
				}
			}
			break;

		case CellSymbol_Koopa:
			if (directionRow > 0)
			{
				UnitData* unitData = GetUnitAt(newRow, newColumn);
				if (unitData != 0)
				{
					unitData->type = UnitType_TurnedKoopa;
					unitData->xOrder = UnitOrder_None;
					levelData[newRow][newColumn] = CellSymbol_TurnedKoopa;
					pointerToUnitData->ySpeed = -GetUnitJumpSpeed(pointerToUnitData->type);
				}
			}
			break;

		case CellSymbol_TurnedKoopa:
			if (directionRow > 0)
			{
				UnitData* unitData = GetUnitAt(newRow, newColumn);
				if (unitData != 0)
				{
					KillUnit(unitData);
					heroScore = heroScore + 10;
					pointerToUnitData->ySpeed = -GetUnitJumpSpeed(pointerToUnitData->type);
				}
			}
			else if (directionColumn != 0)
			{
				UnitData* unitData = GetUnitAt(newRow, newColumn);
				if (directionColumn < 0)
				{
					unitData->xOrder = UnitOrder_Backward;					
				}
				else
				{
					unitData->xOrder = UnitOrder_Forward;
				}
			}
			break;
		}
	}

	//Only monsters actions
	else
	{
		switch (destinationCellSymboll)
		{
			case CellSymbol_Hero:
			case CellSymbol_Hero2:
				if (pointerToUnitData->type == UnitType_Mushroom)
				{
					EatMushroom(oldRow, oldColumn, GetUnitAt(newRow, newColumn));
				}
				else
				{
					//unitsData[heroIndex].health--;
					GetUnitAt(newRow, newColumn)->health--;
					if (pointerToUnitData->xOrder == UnitOrder_Backward)
						pointerToUnitData->xOrder = UnitOrder_Forward;
					else
						pointerToUnitData->xOrder = UnitOrder_Backward;
				}				
				break;

			default:

				UnitType unitType = GetUnitTypeFromCell(destinationCellSymboll);
				if (unitType != UnitType_None)
				{
					if (pointerToUnitData->type == UnitType_TurnedKoopa)
					{
						canMoveToCell = true;
						KillUnit(GetUnitAt(newRow, newColumn));
					}
					else
					{
						if (pointerToUnitData->xOrder == UnitOrder_Backward)
							pointerToUnitData->xOrder = UnitOrder_Forward;
						else
							pointerToUnitData->xOrder = UnitOrder_Backward;
					}
				}
				break;
		}
	}

	if (canMoveToCell)
	{
		levelData[oldRow][oldColumn] = CellSymbol_Empty;

		pointerToUnitData->y = newY;
		pointerToUnitData->x = newX;

		levelData[newRow][newColumn] = unitSymbol;
	}

	return canMoveToCell;
}

void UpdateUnit(UnitData* pointerToUnitData, float deltaTime)
{
	// Unit row and column
	int row = int(pointerToUnitData->y);
	int column = int(pointerToUnitData->x);

	// Y order
	if (pointerToUnitData->yOrder == UnitOrder_Backward)
	{
		// Jump
		if ((pointerToUnitData->y >= (row + cellEndValue))
			&& (levelData[row + 1][column] != CellSymbol_Empty)
			&& (levelData[row + 1][column] != CellSymbol_Abyss))
		{
			pointerToUnitData->ySpeed = -GetUnitJumpSpeed(pointerToUnitData->type);
		}
	}

	// X order
	if (pointerToUnitData->xOrder == UnitOrder_Backward)
	{
		pointerToUnitData->xSpeed = -GetUnitSpeedX(pointerToUnitData->type);
	}
	else
	{
		if (pointerToUnitData->xOrder == UnitOrder_Forward)
		{
			pointerToUnitData->xSpeed = GetUnitSpeedX(pointerToUnitData->type);
		}
		else
		{
			pointerToUnitData->xSpeed = 0;
		}
	}

	// New position
	float deltaY = pointerToUnitData->ySpeed * deltaTime;
	float deltaX = pointerToUnitData->xSpeed * deltaTime;
	float newY = pointerToUnitData->y + deltaY;
	float newX = pointerToUnitData->x + deltaX;
	int newRow = int(newY);
	int newColumn = int(newX);

	// Y (row) step
	if (newRow != row)
	{
		// If unit can go to cell
		if (MoveUnitTo(pointerToUnitData, pointerToUnitData->x, newY))
		{
			row = int(pointerToUnitData->y);
		}
		else
		{
			// Can not move to cell down
			if (newRow > row)
			{
				pointerToUnitData->y = row + cellEndValue;

				if (pointerToUnitData->ySpeed > 0.0f)
					pointerToUnitData->ySpeed = 0.0f;
			}
			// cant move to cell up
			else
			{
				pointerToUnitData->y = row + cellBeginValue;

				if (pointerToUnitData->ySpeed < 0.0f)
					pointerToUnitData->ySpeed = 0.0f;
			}
		}
	}
	else
	{
		pointerToUnitData->y = newY;
	}

	// X (column) step
	if (newColumn != column)
	{
		// If unit can go to cell
		if (MoveUnitTo(pointerToUnitData, newX, pointerToUnitData->y))
		{
			column = int(pointerToUnitData->x);
		}
		else
		{
			// Can not move to cell right
			if (newColumn > column)
			{
				pointerToUnitData->x = column + cellEndValue;

				if (pointerToUnitData->xSpeed > 0.0f)
					pointerToUnitData->xSpeed = 0.0f;
			}
			// cant move to cell left
			else
			{
				pointerToUnitData->x = column + cellBeginValue;

				if (pointerToUnitData->xSpeed < 0.0f)
					pointerToUnitData->xSpeed = 0.0f;
			}
		}
	}
	else
	{
		pointerToUnitData->x = newX;
	}

	// Gravity
	pointerToUnitData->ySpeed += gravitySpeed * deltaTime;
	if (pointerToUnitData->ySpeed > gravitySpeed)
		pointerToUnitData->ySpeed = gravitySpeed;
}

void Update()
{
	// Calculate delta time
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - clockLastFrame;
	if (deltaClock == 0)
		return;

	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	clockLastFrame = clockNow;

	// Calculate FPS
	framesCounter++;
	framesTimeCounter += deltaTime;
	if (framesTimeCounter >= 1.0)
	{
		framesTimeCounter -= 1.0;
		fps = framesCounter;
		framesCounter = 0;
	}

	// Hero control
	if (IsKeyDown(VK_UP))
		unitsData[heroIndex].yOrder = UnitOrder_Backward;
	else
		unitsData[heroIndex].yOrder = UnitOrder_None;

	if (IsKeyDown(VK_LEFT))
		unitsData[heroIndex].xOrder = UnitOrder_Backward;
	else
	{
		if (IsKeyDown(VK_RIGHT))
			unitsData[heroIndex].xOrder = UnitOrder_Forward;
		else
			unitsData[heroIndex].xOrder = UnitOrder_None;
	}

	// Hero2 control
	if (IsKeyDown('W'))
		unitsData[hero2Index].yOrder = UnitOrder_Backward;
	else
		unitsData[hero2Index].yOrder = UnitOrder_None;

	if (IsKeyDown('A'))
		unitsData[hero2Index].xOrder = UnitOrder_Backward;
	else
	{
		if (IsKeyDown('D'))
			unitsData[hero2Index].xOrder = UnitOrder_Forward;
		else
			unitsData[hero2Index].xOrder = UnitOrder_None;
	}

	// Updates all units
	for (int u = 0; u < unitsCount; u++)
	{
		UpdateUnit(&unitsData[u], deltaTime);
	}

	// Update AI
	UpdateAI();

	// Hero dead
	if (unitsData[heroIndex].health <= 0 || unitsData[hero2Index].health <= 0)
		Initialize();

	// Restart game
	if (IsKeyDown('R'))
		Initialize();
	
	// Go to another level
	if (IsKeyDown('1'))
	{
		levelIndex = 0;
		Initialize();
	}

	if (IsKeyDown('2'))
	{	
		levelIndex = 1;
		Initialize();		
	}
	
}

void Shutdown()
{
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