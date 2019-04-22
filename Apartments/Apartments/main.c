#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "aprt.h"
	
void main()
{
	char *short_term_history[N];
	COLIST long_term_history;
	int sizeOfLong = 0;
	LIST apartments; // list for apartments
	char* command;
	int numOfCommands = 0; // will hold the number of commands that are in the file "commandTextFile.txt"
	resetShortTermHistory(short_term_history, N);
	makeEmptyCOlList(&long_term_history);
	makeEmptyList(&apartments);

	//read the command from the text file and enter them to the short array and to the long list
	readCommandsFromTextFile("commandTextFile.txt", &long_term_history, short_term_history, &numOfCommands);
	enterLastCommandToShortHistory(short_term_history, numOfCommands, &long_term_history, &sizeOfLong);

	//read the apartments from the binary file and enter them into the list apartments
	apartments = readAprtsFromBinFile("apartBinFile.bin");

	printf("Please enter one of the following commands:\n");
	printf("add-apt, find-apt, buy-apt or delete-apt\n");
	printf("For reconstruction commands, please enter:\n");
	printf("!!, !num, history, short_history or !^str1^str2\n");

	//getting the first command from the user 
	command = getInput(short_term_history, &long_term_history, &sizeOfLong, apartments, "commandTextFile.txt");

	while (strcmp(command, "exit") != 0)
	{
		//checking the meaning of the command and do it 
		checkCommand(command, short_term_history, &long_term_history, &sizeOfLong, &apartments, "commandTextFile.txt");
		// free the last command that has been entered
		free(command);
		//getting the next command from the user 
		command = getInput(short_term_history, &long_term_history, &sizeOfLong, apartments, "commandTextFile.txt");
	}

	// saving the apartments in binary file
	saveApartInBinFile(apartments, "apartBinFile.bin");

	// free the shortHistory array + longHistory list + apartments list 
	freeShortTermHistory(short_term_history);
	freeLongTermHistory(long_term_history);
	freeApartments(apartments);

	// free the last command that has been entered
	free(command);

	printf("Good Bye!\n");
}
