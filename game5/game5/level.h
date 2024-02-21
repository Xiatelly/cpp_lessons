#pragma once

///////////////////////////////
// Include
#include "consoleColor.h"

////////////////////////////
// Constants
const int rowsCount = 15;
const int columnsCount = 35;
const int levelsCount = 3;

const unsigned char CellSymbol_Empty = ' ';
const unsigned char	CellSymbol_Wall = '#';
const unsigned char	CellSymbol_Hero = 'h';
const unsigned char	CellSymbol_Exit = 'e';
const unsigned char	CellSymbol_Orc = 'o';
const unsigned char	CellSymbol_Spider = 'g';
const unsigned char	CellSymbol_Skeleton = 's';
const unsigned char	CellSymbol_Stick = '1';
const unsigned char	CellSymbol_Club = '2';
const unsigned char	CellSymbol_Spear = '3';
const unsigned char CellSymbol_Saber = '4';
const unsigned char CellSymbol_Flail = '5';
const unsigned char CellSymbol_Scythe = '6';
const unsigned char CellSymbol_Claymore = '7';
const unsigned char CellSymbol_Heart = 'a';


const unsigned char levelsData[levelsCount][rowsCount][columnsCount + 1] = {
	{
		"###################################",
		"#   # ##      o       a   s       #",
		"# # o  #o############## ######### #",
		"# ###### #   # #3  s  # ##s##   # #",
		"#  s       # # ###### # #s4 g #s# #",
		"###### # ### #      # # ##s##   # #",
		"##  o# # # # #### # #   ##### # # #",
		"#  #   # # o   a# ## ###### #a  # #",
		"######## # ####         #  s# # #s#",
		"#  2#    #      ####### # # #   # #",
		"# ### #### ######   o   # #   #   #",
		"#a o       #      ####### #########",
		"############ ######   #1# # s #  a#",
		"#h                  #   #   #s  # #",
		"#################################e#",
	},
	{
		"################################e##",
		"#s 5# a###   ### o# # g           #",
		"#  #     #     #  #     #    #a##s#",
		"#  #     #      o #a######## #s   #",
		"#o #   s # #####  ## s       #    #",
		"#  #     #     o# #   #      #    #",
		"#              #  # ## ###   #    #",
		"#  o##  ##     #  #     #      #6##",
		"#  #   # s        #     # ## g    #",
		"#  #     #####        # #       # #",
		"#o #a#          ### # # ### ##### #",
		"#   #   #   s#ss a# # #o          #",
		"##  #  ##   ###       #     ##### #",
		"#h  #   o       ###   #     o#a o #",
		"###################################",
	},
	{
		"###################################",
		"#s###   ##s#o         s #o      h #",
		"#  #s      #a o# g  s   # #       #",
		"#     #    ####### ###  # ##### a #",
		"#   ##  #   o#      o###  #   ## ##",
		"##  #   # ## #  o       o         #",
		"##  #g a#      ########   ####  # #",
		"#   ##### ###  #7a#o a#   s#o    o#",
		"#      s  #    #  #     #  # # #a #",
		"####     s#  # #  o     #      ## #",
		"# g# ######  # #o    o# ###### g# #",
		"#  # #a        ########     a#  # #",
		"#  a #  ### #      o  a##  #### # #",
		"e        #s #    ###o            s#",
		"###################################",
	}
};

unsigned char GetRenderCellSymbol(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_Empty:		return ' ';
	case CellSymbol_Heart:		return 3;
	case CellSymbol_Wall:		return 177;
	case CellSymbol_Hero:		return 2;
	case CellSymbol_Orc:		return 2;
	case CellSymbol_Skeleton:   return 2;
	case CellSymbol_Spider:     return 15;
	case CellSymbol_Stick:		return 47;
	case CellSymbol_Club:		return 33;
	case CellSymbol_Spear:		return 24;
	case CellSymbol_Saber:		return 108;
	case CellSymbol_Flail:		return 64;
	case CellSymbol_Scythe:		return 251;
	case CellSymbol_Claymore:	return 197;
	case CellSymbol_Exit:		return 178;
	}

	return '?';
}

ConsoleColor GetRenderCellSymbolColor(unsigned char cellSymbol)
{
	switch (cellSymbol)
	{
	case CellSymbol_Empty:		return ConsoleColor_Black;
	case CellSymbol_Wall:		return ConsoleColor_White;
	case CellSymbol_Heart:		return ConsoleColor_Red;
	case CellSymbol_Hero:		return ConsoleColor_Yellow;
	case CellSymbol_Orc:		return ConsoleColor_Green;
	case CellSymbol_Skeleton:   return ConsoleColor_White;
	case CellSymbol_Spider:     return ConsoleColor_DarkYellow;
	case CellSymbol_Stick:		return ConsoleColor_DarkYellow;
	case CellSymbol_Club:		return ConsoleColor_DarkRed;
	case CellSymbol_Spear:		return ConsoleColor_DarkCyan;
	case CellSymbol_Saber:		return ConsoleColor_Cyan;
	case CellSymbol_Flail:		return ConsoleColor_Blue;
	case CellSymbol_Scythe:		return ConsoleColor_Cyan;
	case CellSymbol_Claymore:	return ConsoleColor_Magenta;
	case CellSymbol_Exit:		return ConsoleColor_Red;
	}

	return ConsoleColor_Gray;
}