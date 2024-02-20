#include <cstdio>
#include <cstdlib>
#include <ctime>

void main()
{
	const int numberRange = 100;
	int computerNumber = 0;
	int playerNumber = 0;

	time_t currentTime = time(0);
	srand(currentTime);
	rand();
	computerNumber = rand();
	computerNumber = computerNumber % (numberRange + 1);

	//printf("Computer set number: %i Range: from %i to %i \n", computerNumber, 0, numberRange);
	
	do
	{
		printf("Input your number: ");
		scanf_s("%i", &playerNumber);

		//printf("User number: %i", playerNumber);

		if (playerNumber < computerNumber)
		{
			printf("Larger! \n");
		}
		else
		{
			if (playerNumber > computerNumber)
			{
				printf("Smaller! \n");
			}
			else
			{
				printf("GG! \n");
			}
		}
	} 
	while (playerNumber != computerNumber);

	printf("Press Enter to continue... ");
	getchar();
	getchar();

}