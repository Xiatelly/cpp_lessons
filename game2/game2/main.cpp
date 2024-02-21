#include <cstdio>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>


int main() 
{
	// Game settings
	const int startingMoney = 500;
	const int turnCost = 50;
	const int doubleBonus = 90;
	const int tripleBonus = 200;

	// Logical variables
	int money = startingMoney;
	char panel0 = 'X';
	char panel1 = 'X';
	char panel2 = 'X';
	int diff = 0;
	// Initialization
	srand(time(0));

	// Main loop
	do
	{
		// Render
		system("cls");

		printf("\n\n");
		printf("\t###########\n");
		printf("\t# BANDITO #\n");
		printf("\t###########\n");
		printf("\t#         #\n");
		printf("\t#  %c %c %c  #\n", panel0, panel1, panel2);
		printf("\t#         #\n");
		printf("\t###########\n");
		printf("\n");
		if (diff != 0)
		{
		printf("\tBounty + %i \n", diff);
		}
		else 
		{
			printf("\n");
		}
		printf("\n");
		printf("\tMoney: %i$\n", money);

		// Input (just for pause)
		_getch();


		// Main logic
		money = money - turnCost;

		panel0 = 3 + (rand() % 4);
		panel1 = 3 + (rand() % 4);
		panel2 = 3 + (rand() % 4);

		// Three out of three
		if ((panel0 == panel1) && (panel0 == panel2))
		{
			money = money + tripleBonus;
			diff = tripleBonus - turnCost;
		}
		else
		{
			// Two out of three
			if ((panel0 == panel1) || (panel1 == panel2) || (panel0 == panel2))
			{
				money = money + doubleBonus;
				diff = doubleBonus - turnCost;
			}
			else 
			{
				diff = 0;
			}
		}

		// Diff
		

	} 
	while (money >= turnCost);

	// No errors
	return 0;
}