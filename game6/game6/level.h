#pragma once

//////////////////
// Include
#include "renderSystem.h"


////////////////////////////////////
// Constans
const int levelRows = screenRows;
const int levelColumns = screenColumns;
const int levelsCount = 2;

const int gravitySpeed = 38;

const unsigned char CellSymbol_Empty		= ' ';
const unsigned char CellSymbol_BrickWall	= '#';
const unsigned char CellSymbol_SteelWall	= '@';
const unsigned char CellSymbol_Abyss		= 'a';
const unsigned char CellSymbol_Pipe			= 'p';
const unsigned char CellSymbol_Crystal		= '0';
const unsigned char CellSymbol_Box			= '?';
const unsigned char CellSymbol_OpenedBox	= '&';
const unsigned char CellSymbol_Mushroom		= '^';
const unsigned char CellSymbol_MushroomBox  = '!';
const unsigned char CellSymbol_Hero			= 'h';
const unsigned char CellSymbol_Hero2		= 'l';
const unsigned char CellSymbol_Goomba		= 'g';
const unsigned char CellSymbol_Exit			= '|';
const unsigned char CellSymbol_Flag			= '>';
const unsigned char CellSymbol_Jump			= 'j';
const unsigned char CellSymbol_Koopa		= 'k';
const unsigned char CellSymbol_TurnedKoopa	= 't';

const unsigned char levelData0[levelsCount][levelRows][levelColumns + 1] =
{ 
	{
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
		"#         0                                                          g         #",
		"#      @                                                             @     |>  #",
		"#     @@     @                  ##?#        0 0 0                   @@     |   #",
		"#    @@@     @@      pppp                            pppp          @@@     |   #",
		"#   @@@@ g   @@@ k    pp   g            g g           pp    k     @@@@     |   #",
		"#  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@j@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#",
		"#                                                                              #",
		"#                   0                                      0 0                 #",
		"# @       #??#     ###            !               ?####   ######               #",
		"# @@                                                                           #",
		"# @@@                                                                          #",
		"# @@@@     ##      ######     ?   ?   ?     ?#    #               ##           #",
		"# @@@@@                                                                        #",
		"# @@@@@@               # g g g g            g     k       g g            #     #",
		"# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@j@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@##@    #",
		"#                                                                              #",
		"#               ?                                                            ###",
		"#                                                                              #",
		"#                                              pppp                     #!#    #",
		"#          ?  #!#?#                 pppp        pp        pppp          # #    #",
		"#                        pppp        pp         pp         pp                  #",
		"#  l h         g          pp         pp g       pp  k  g g pp        #   g     #",
		"# ##############################j#####################j############  #########j#",
		"#a#################################################################aa###########",
	},
	{
		"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
		"#                                                                              #",
		"# l h                                            ###?@@!@@                     #",
		"#@@@@@@                                                                        #",
		"#  @@@@                                ######         ###?#pppp        #!#     #",
		"#  @@@@@j           #!#?# ?     pppp              ####      pp         # #     #",
		"#                             g  pp    k  g        g  g     pp                 #",
		"#    @@@@@@@@@@@@@@@@@@@@@@@j@@@@pp@@@@@pppp##@@j########j##pp####  #   g      #",
		"#                                        pp  0                      #########j##",
		"#           0  0                         pp                                    #",
		"#            #!#  pppp                   pp#?##?#  ####          pppp   0  0   #",
		"#    ##g   g     g pp                    pp            ##         pp           #",
		"#  @@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@     #########pp########   #",
		"#                     #####                                                    #",
		"#                         #####                                                #",
		"#                                                                              #",
		"#                              #####################     @@@@@@@@@@@@@@@@@@@@@@#",
		"#                                                                              #",
		"#                                                     #########                #",
		"#                                                                          |>  #",
		"#    ###!###      pppp                             pppp       @@           |   #",
		"#                  pp                               pp   !   @@@        0  |   #",
		"#       #      g   pp    k     g     k              pp      @@@@           |   #",
		"# ###############################################################j#  ###########",
		"#a#################################################################aa###########",
	}
};

////////////////////////////////////////
// Functions
unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_Empty:			return ' ';
	case CellSymbol_BrickWall:		return 177;
	case CellSymbol_SteelWall:		return 178;
	case CellSymbol_Abyss:			return ' ';
	case CellSymbol_Pipe:			return 178;
	case CellSymbol_Crystal:		return 4;
	case CellSymbol_Box:			return '?';
	case CellSymbol_OpenedBox:		return 254;
	case CellSymbol_Mushroom:		return 6;
	case CellSymbol_MushroomBox:	return '?';
	case CellSymbol_Hero:			return 2;
	case CellSymbol_Hero2:			return 2;
	case CellSymbol_Goomba:			return 2;
	case CellSymbol_Exit:			return 222;
	case CellSymbol_Flag:			return 16;
	case CellSymbol_Jump:			return 30;
	case CellSymbol_Koopa:			return 2;
	case CellSymbol_TurnedKoopa:	return 1;
	}

	return '?';
}

ConsoleColor GetRenderCellSymbolColor(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_BrickWall:		return ConsoleColor_DarkYellow;
	case CellSymbol_SteelWall:		return ConsoleColor_White;
	case CellSymbol_Pipe:			return ConsoleColor_Green;
	case CellSymbol_Crystal:		return ConsoleColor_Cyan;
	case CellSymbol_Box:			return ConsoleColor_DarkRed;
	case CellSymbol_OpenedBox:		return ConsoleColor_Gray;
	case CellSymbol_Mushroom:		return ConsoleColor_Red;
	case CellSymbol_MushroomBox:	return ConsoleColor_DarkRed;
	case CellSymbol_Hero:			return ConsoleColor_Yellow;
	case CellSymbol_Hero2:			return ConsoleColor_DarkYellow;
	case CellSymbol_Goomba:			return ConsoleColor_DarkRed;
	case CellSymbol_Exit:			return ConsoleColor_Green;
	case CellSymbol_Flag:			return ConsoleColor_White;
	case CellSymbol_Jump:			return ConsoleColor_Magenta;
	case CellSymbol_Koopa:			return ConsoleColor_Green;
	case CellSymbol_TurnedKoopa:	return ConsoleColor_Green;
	}

	return ConsoleColor_Gray;
}

ConsoleColor GetRenderCellSymbolBackgroundColor(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_BrickWall:		return ConsoleColor_Red;
	case CellSymbol_SteelWall:		return ConsoleColor_DarkGray;
	case CellSymbol_Pipe:			return ConsoleColor_DarkGreen;
	case CellSymbol_Box:			return ConsoleColor_Yellow;
	case CellSymbol_OpenedBox:		return ConsoleColor_DarkGray;
	case CellSymbol_MushroomBox:	return ConsoleColor_Yellow;
	case CellSymbol_Jump:			return ConsoleColor_DarkYellow;
	}

	return ConsoleColor_Black;
}

ConsoleColor GetRenderHeroColor(int heroHealth)
{
	switch (heroHealth)
	{
	case 2: return ConsoleColor_Red;
	}

	return ConsoleColor_Yellow;
}

ConsoleColor GetRenderHero2Color(int heroHealth)
{
	switch (heroHealth)
	{
	case 2: return ConsoleColor_Magenta;
	}

	return ConsoleColor_DarkYellow;
}