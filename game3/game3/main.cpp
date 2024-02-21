#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <cctype>



int main()
{
	// Game settings
	const int lettersCount = 20;


	// Logic variables
	char letters[lettersCount];
	int step = 0;
	int errorCount = 0;
	char continuePlaying;

	// System setup
	time_t currentTime;
	time(&currentTime);
	srand(currentTime);
	

	do
	{

		// Initialization
		step = 0;
		for (int i = 0; i < lettersCount; i++)
		{
			letters[i] = 'a' + rand() % (('z' - 'a') + 1);
		}


		// Part I - "Prologue"
		system("cls");

		printf("\n\tReady?\n");

		// Input (just for pause)
		_getch();

		// Print 5 dots for simple animation
		printf("\t");
		for (int i = 0; i < 5; i++)
		{
			printf(".");
			Sleep(200);
		}

		printf("\n\tGo!");
		Sleep(350);


		time(&currentTime);

		// Part II - "Main loop"
		do
		{
			// Render
			system("cls");

			printf("\n\t");
			for (int i = 0; i < lettersCount; i++)
			{
				if (i < step)
				{
					printf("%c ", 176);
				}
				else
				{
					printf("%c ", toupper(letters[i])); // converting to upper case
				}
			}


			// Input - allowed to get upper case chars
			char inputChar = tolower(_getch());



			// Main logic
			if (inputChar == letters[step])
			{
				step++;
			}
			else
				errorCount++;


		} while (step < lettersCount);

		time_t finalTime;
		time(&finalTime);

		// time_t finalTime = time(0);

		int totalTime = int(finalTime - currentTime);

		// Part III - "Epilogue"
		system("cls");

		printf("\n\t");
		for (int i = 0; i < lettersCount; i++)
		{
			printf("%c ", 176);
		}


		float charInMinute = (float(lettersCount) / float(totalTime)) * 60.0;
		
		printf("\n\n\tYou failed %i times", errorCount);
		printf("\n\n\tWasted time %i sec", totalTime);
		printf("\n\n\tyour speed was %f letters in minute", charInMinute);
		printf("\n\n\tDo you want try again? y/n: ");
	
		continuePlaying = tolower(_getch());
		
	} while (continuePlaying == 'y');

	// Input (just for pause)
	_getch();


	// No errors
	return 0;

}