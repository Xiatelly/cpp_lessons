#pragma once

//////////////////////////////////
// Include
#include "level.h"

enum WeaponType
{
	WeaponType_None,
	WeaponType_Fist,
	WeaponType_Stick,
	WeaponType_Club,
	WeaponType_Spear,
	WeaponType_Saber,
	WeaponType_Flail,
	WeaponType_Scythe,
	WeaponType_Bite,
	WeaponType_Claymore,
};

struct DamageRange 
{
	int min = 0;
	int max = 0;
};

const char* WeaponName_None		= "None";
const char* WeaponName_Fist		= "Fist";
const char* WeaponName_Stick	= "Stick";
const char* WeaponName_Club		= "Club";
const char* WeaponName_Spear	= "Spear";
const char* WeaponName_Saber	= "Saber";
const char* WeaponName_Flail	= "Flail";
const char* WeaponName_Scythe	= "Scythe";
const char* WeaponName_Claymore	= "Claymore";

const char* GetWeaponName(WeaponType weaponType)
{
	switch (weaponType)
	{
		case WeaponType_Fist:		return WeaponName_Fist;
		case WeaponType_Stick:		return WeaponName_Stick;
		case WeaponType_Club:		return WeaponName_Club;
		case WeaponType_Spear:		return WeaponName_Spear;
		case WeaponType_Saber:		return WeaponName_Saber;
		case WeaponType_Flail:		return WeaponName_Flail;
		case WeaponType_Scythe:		return WeaponName_Scythe;
		case WeaponType_Claymore:	return WeaponName_Claymore;
	}
	return WeaponName_None;
}

DamageRange GetWeaponDamage(WeaponType weaponType)
{
	DamageRange dmg;
	switch (weaponType)
	{
		case WeaponType_Fist: 
		{
			dmg.min = 1;
			dmg.max = 2;
			break;
		}

		case WeaponType_Stick:
		{
			dmg.min = 10;
			dmg.max = 16;
			break;
		}
		case WeaponType_Club:
		{
			dmg.min = 18;
			dmg.max = 24;
			break;
		}
		case WeaponType_Spear:
		{
			dmg.min = 28;
			dmg.max = 32;
			break;
		}
		case WeaponType_Saber:
		{
			dmg.min = 36;
			dmg.max = 40;
			break;
		}
		case WeaponType_Flail:
		{
			dmg.min = 42;
			dmg.max = 48;
			break;
		}
		case WeaponType_Scythe:
		{
			dmg.min = 54;
			dmg.max = 60;
			break;
		}
		case WeaponType_Claymore: {
			dmg.min = 94;
			dmg.max = 100;
			break;
		}
		case WeaponType_Bite:
		{
			dmg.min = 74;
			dmg.max = 80;
			break;
		}
	}

	return dmg;
}

int GenerateDamage(DamageRange dmg)
{
	int currentDamage = dmg.min + (rand() % (dmg.max - dmg.min + 1));
	return currentDamage;
}


WeaponType GetWeaponTypeFromCell(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
		case CellSymbol_Stick:		return WeaponType_Stick;
		case CellSymbol_Club:		return WeaponType_Club;
		case CellSymbol_Spear:		return WeaponType_Spear;
		case CellSymbol_Saber:		return WeaponType_Saber;
		case CellSymbol_Flail:		return WeaponType_Flail;
		case CellSymbol_Scythe:		return WeaponType_Scythe;
		case CellSymbol_Claymore:	return WeaponType_Claymore;

	}

	return WeaponType_None;
}