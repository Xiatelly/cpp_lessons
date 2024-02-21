#pragma once


///////////////////////////
// Include
#include"level.h"

enum UnitType
{
	UnitType_None,
	UnitType_Hero,
	UnitType_Hero2,
	UnitType_Goomba,
	UnitType_Mushroom,
	UnitType_Koopa,
	UnitType_TurnedKoopa
};

UnitType GetUnitTypeFromCell(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_Hero:			return UnitType_Hero;
	case CellSymbol_Hero2:			return UnitType_Hero2;
	case CellSymbol_Goomba:			return UnitType_Goomba;
	case CellSymbol_Mushroom:		return UnitType_Mushroom;
	case CellSymbol_Koopa:			return UnitType_Koopa;
	case CellSymbol_TurnedKoopa:	return UnitType_TurnedKoopa;
	}

	return UnitType_None;
}

int GetUnitSpeedX(UnitType unitType)
{
	switch (unitType)
	{
	case UnitType_Hero:			return 10;
	case UnitType_Hero2:		return 10;
	case UnitType_Goomba:		return 2;
	case UnitType_Mushroom:		return 7;
	case UnitType_Koopa:		return 2;
	case UnitType_TurnedKoopa:	return 10;
	}

	return 0;
}

int GetUnitJumpSpeed(UnitType unitType)
{
	switch (unitType)
	{
		case UnitType_Hero:			return 17;
		case UnitType_Hero2:		return 17;
	}

	return 0;
}