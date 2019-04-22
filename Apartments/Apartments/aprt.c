#include "aprt.h"
void findApt(int MinNumRooms, int MaxNumRooms, int MaxPrice, DATE date, int order, LIST *listApt, int days)
{
	LNODE *curr = listApt->head;
	APRT* arr;
	int lsize = 0, phsize = 1;
	BOOL found = FALSE;
	arr = (APRT*)malloc(phsize * sizeof(APRT));
	checkMemory(arr);

	while (curr != NULL)
	{
		if (curr->aprtInfo.numRooms >= MinNumRooms || MinNumRooms == 0)
		{
			if (curr->aprtInfo.numRooms <= MaxNumRooms || MaxNumRooms == 0)
			{
				if ((findAptDate(curr, date) == TRUE) || (date.day == 0 && date.month == 0 && date.year == 0))
				{
					if (curr->aprtInfo.price <= MaxPrice || MaxPrice == 0)
					{
						if (findLastDaysAprt(curr, days) == TRUE || days == 0)
						{
							found = TRUE;
							if (order == NO_ORDER)
								printAprtFromList(curr);

							else
							{
								arr[lsize] = curr->aprtInfo;
								lsize++;
								if (lsize == phsize)
								{
									phsize *= 2;
									arr = (APRT*)realloc(arr, phsize * sizeof(APRT));
									checkMemory(arr);
								}
							}
						}
					}
				}
			}
		}
		curr = curr->next;
	}

	if (found == FALSE)
		printf("There were no apartments match\n");
	else
	{
		if (order != NO_ORDER)
		{
			if (lsize < phsize)
			{
				arr = (APRT*)realloc(arr, lsize * sizeof(APRT));
				checkMemory(arr);
			}
			if (order == LOWER_TO_HIGHER)
				mergeSort(arr, lsize, LOWER_TO_HIGHER);
			else
				mergeSort(arr, lsize, HIGHER_TO_LOWER);
			printArr(arr, lsize);
		}
	}
	free(arr);
}

BOOL findAptDate(LNODE* curr, DATE date)
{
	if (curr->aprtInfo.dateOfEntrance.year < date.year)
	{
		return TRUE;
	}

	else if (curr->aprtInfo.dateOfEntrance.year == date.year)
	{
		if (curr->aprtInfo.dateOfEntrance.month < date.month)
		{
			return TRUE;
		}
		else if (curr->aprtInfo.dateOfEntrance.month == date.month)
		{
			if (curr->aprtInfo.dateOfEntrance.day <= date.day)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void addApt(LIST* lst, char* address, int price, short int numRooms, DATE date)
{
	LNODE *newNode;
	DATE dateOfEntrnceToList;
	short int code;

	time_t theTime;
	struct tm *info;
	time(&theTime);
	info = localtime(&theTime);
	dateOfEntrnceToList.day = info->tm_mday;
	dateOfEntrnceToList.month = info->tm_mon + 1;
	dateOfEntrnceToList.year = info->tm_year - 100;

	if (isEmptyList(lst) == TRUE)
		code = 1;
	else
		code = lst->tail->aprtInfo.code + 1;


	newNode = creatNewListNode(address, price, numRooms, date, code, NULL, dateOfEntrnceToList);
	insertNodeToEndList(lst, newNode);
}


LNODE* creatNewListNode(char* address, int price, short int numRooms, DATE date, short int code, LNODE*next, DATE dateOfEntrnceToList)
{
	LNODE* res;
	res = (LNODE*)malloc(sizeof(LNODE));
	checkMemory(res);
	res->aprtInfo.address = (char*)malloc(strlen(address) * sizeof(char));
	checkMemory(res->aprtInfo.address);
	res->aprtInfo.address = address;
	res->aprtInfo.price = price;
	res->aprtInfo.numRooms = numRooms;
	res->aprtInfo.dateOfEntrance = date;
	res->aprtInfo.code = code;
	res->next = next;
	res->aprtInfo.dateOfEntranceToList = dateOfEntrnceToList;

	return res;
}

void insertNodeToEndList(LIST *lst, LNODE * tail)
{
	if (isEmptyList(lst) == TRUE)
	{
		lst->head = lst->tail = tail;
	}
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}


BOOL isEmptyList(LIST *lst)
{
	if (lst->head == NULL)
		return TRUE;
	else
		return FALSE;
}

void buyAprt(LIST* lst, short int code)
{
	LNODE* curr;
	BOOL found = FALSE;
	int place = 0;

	curr = lst->head;
	while (curr != NULL && !found)
	{
		if (code == curr->aprtInfo.code)
		{
			found = TRUE;
			deleteAprt(lst, curr, place);
		}
		curr = curr->next;
		place++;
	}
}

void deleteAprt(LIST* lst, LNODE* node, int place)
{
	int i;
	LNODE * curr, *saver;
	saver = node->next;

	if (lst->head == node)
	{
		lst->head = saver;
		free(node);
	}
	else
	{
		curr = lst->head;
		for (i = 0; i < place - 1; i++)
		{
			curr = curr->next;
		}

		free(node);
		curr->next = saver;
		if (saver == NULL)
			lst->tail = curr;
	}
}


void checkMemory(void* ptr)
{
	if (ptr == NULL)
	{
		puts("Memory allocation failed \n");
		exit(1);
	}
}

BOOL findLastDaysAprt(LNODE* curr, int num)
{
	time_t theTime;
	struct tm *info;
	DATE dateByUser;
	BOOL found = FALSE;

	time(&theTime);
	theTime = theTime - (num*(60 * 60 * 24));
	info = localtime(&theTime);

	dateByUser.day = info->tm_mday;
	dateByUser.month = info->tm_mon + 1;
	dateByUser.year = info->tm_year - 100;

	if (curr->aprtInfo.dateOfEntrance.year > dateByUser.year)
		found = TRUE;

	else if (curr->aprtInfo.dateOfEntrance.year == dateByUser.year)
	{
		if (curr->aprtInfo.dateOfEntrance.month > dateByUser.month)
			found = TRUE;

		else if (curr->aprtInfo.dateOfEntrance.month == dateByUser.month)
			if (curr->aprtInfo.dateOfEntrance.day >= dateByUser.day)
				found = TRUE;
	}

	return found;
}

void deleteAprtByDay(LIST* lst, int num)
{
	time_t theTime;
	struct tm *info;
	DATE dateByUser;
	BOOL found = FALSE;
	LNODE* curr = lst->head;
	int place = 0;

	time(&theTime);
	theTime = theTime - (num*(60 * 60 * 24));
	info = localtime(&theTime);

	dateByUser.day = info->tm_mday;
	dateByUser.month = info->tm_mon + 1;
	dateByUser.year = info->tm_year - 100;

	while (curr != NULL)
	{
		if (curr->aprtInfo.dateOfEntrance.year > dateByUser.year)
		{
			found = TRUE;
			deleteAprt(lst, curr, place);
		}

		else if (curr->aprtInfo.dateOfEntrance.year == dateByUser.year)
		{
			if (curr->aprtInfo.dateOfEntrance.month > dateByUser.month)
			{
				found = TRUE;
				deleteAprt(lst, curr, place);
			}
			else if (curr->aprtInfo.dateOfEntrance.month == dateByUser.month)
			{
				if (curr->aprtInfo.dateOfEntrance.day >= dateByUser.day)
				{
					found = TRUE;
					deleteAprt(lst, curr, place);
				}
			}
		}
		curr = curr->next;
		place++;
	}

	if (found == FALSE)
		printf("No apartment was deleted \n");
}

void resetShortTermHistory(char **arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		arr[i] = NULL;
	}
}

void enterCommandToArr(char ** arr, char * command, COLIST * long_term_history, int *sizeOfLong)
{
	int i, j;
	BOOL foundNull = FALSE;
	int saveIndex;
	char * oldestCommand;

	if (arr[0] == NULL) // if the first cells is NULL we will enter the command 
	{
		arr[0] = (char*)malloc((strlen(command) + 1) * sizeof(char));
		checkMemory(arr[0]);
		foundNull = TRUE;
		strcpy(arr[0], command);
		arr[0][strlen(command)] = '\0';

	}
	else // we are looking for the first cell that is NULL
	{
		for (i = 0; i < N && !foundNull; i++)
		{
			if (arr[i] == NULL)
			{
				foundNull = TRUE;
				saveIndex = i;
				for (j = saveIndex - 1; j >= 0; j--)
				{
					free(arr[j + 1]);
					arr[j + 1] = (char*)malloc((strlen(arr[j]) + 1) * sizeof(char));
					checkMemory(arr[j + 1]);
					strcpy(arr[j + 1], arr[j]);
					arr[j + 1][strlen(arr[j])] = '\0';
				}
				free(arr[0]);
				arr[0] = (char*)malloc((strlen(command) + 1) * sizeof(char));
				checkMemory(arr[0]);
				strcpy(arr[0], command);
				arr[0][strlen(command)] = '\0';
			}

		}
	}

	// if the array is full - we will move the oldest command to the other array and will enter the new one by moving the other cells forward
	if (foundNull == FALSE)
	{
		oldestCommand = (char*)malloc((strlen(arr[N - 1]) + 1) * sizeof(char));
		checkMemory(oldestCommand);
		strcpy(oldestCommand, arr[N - 1]);
		oldestCommand[strlen(arr[N - 1])] = '\0';


		for (i = N - 2; i >= 0; i--)
		{
			arr[i + 1] = (char*)malloc((strlen(arr[i]) + 1) * sizeof(char));
			checkMemory(arr[i + 1]);
			strcpy(arr[i + 1], arr[i]);
			arr[i + 1][strlen(arr[i])] = '\0';
		}

		arr[0] = (char*)malloc((strlen(command) + 1) * sizeof(char));
		checkMemory(arr[0]);
		strcpy(arr[0], command);
		arr[0][strlen(command)] = '\0';
		insertDataToEndList(oldestCommand, long_term_history, sizeOfLong);

	}
}

void makeEmptyCOlList(COLIST * lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

void makeEmptyList(LIST* lst)
{
	lst->head = NULL;
	lst->tail = NULL;
}

void insertDataToEndList(char* command, COLIST* long_term_history, int * size)
{
	int sizeOfLong = *size;
	COLNODE* newTail;
	newTail = createNewCoListNode(command, NULL);
	insertNodeToEndCoList(long_term_history, newTail);
	sizeOfLong++;
	*size = sizeOfLong;
}

COLNODE* createNewCoListNode(char* command, COLNODE* next)
{
	COLNODE* res;
	res = (COLNODE*)malloc(sizeof(COLNODE));
	checkMemory(res);
	res->command = command;
	res->next = next;
	return res;
}

void insertNodeToEndCoList(COLIST* long_term_history, COLNODE* newTail)
{
	if (long_term_history->head == NULL)
	{
		long_term_history->head = newTail;
		long_term_history->tail = newTail;
	}
	else
	{
		long_term_history->tail->next = newTail;
		long_term_history->tail = newTail;
	}
}

char* getInput(char** shortHisory, COLIST* longHistory, int* sizeOfLong, LIST apartments, char* fileName)
{
	char * command;
	char letter;
	int lsize = 0, phsize = 1;

	command = (char*)malloc(sizeof(char)*phsize);
	checkMemory(command);
	scanf("%c", &letter);
	while (letter != '\n')
	{
		command[lsize] = letter;
		lsize++;
		if (lsize == phsize)
		{
			phsize *= 2;
			command = (char*)realloc(command, phsize * sizeof(char));
			checkMemory(command);
		}
		scanf("%c", &letter);
	}
	command[lsize] = '\0';
	lsize++;

	if (lsize < phsize)
	{
		command = (char*)realloc(command, lsize * sizeof(char));
		checkMemory(command);
	}
	if ((strcmp(command, "history") != 0) && (strcmp(command, "short_history") != 0) && (command[0] != '!') && (strcmp(command, "exit") != 0))
	{
		enterCommandToArr(shortHisory, command, longHistory, sizeOfLong);
		enterCommandToTextFile(command, fileName);
	}

	return (command);

}

void checkCommand(char command[], char** shortHistory, COLIST* longHistory, int* sizeOfLong, LIST* apartments, char* fileName)
{
	char* str;
	int i;
	char*temp;

	temp = (char*)malloc((strlen(command) + 1) * sizeof(char));
	strcpy(temp, command);
	temp[strlen(command)] = '\0';

	str = strtok(temp, " ");

	if (strcmp(str, "short_history") == 0)
		printShortHistory(shortHistory, *sizeOfLong);


	else if (strcmp(str, "history") == 0)
	{
		printLongHistory(longHistory, *sizeOfLong);
		printShortHistory(shortHistory, *sizeOfLong);
	}

	else if (strcmp(str, "buy-apt") == 0)
	{
		int num;
		str = strtok(NULL, " ");
		sscanf(str, "%d", &num);
		buyAprt(apartments, num);
	}

	else if (strcmp(str, "delete-apt") == 0)
	{
		int num;
		str = strtok(NULL, " ");
		str = strtok(NULL, " ");
		sscanf(str, "%d", &num);
		deleteAprtByDay(apartments, num);
	}

	else if (strcmp(str, "!!") == 0)
	{
		checkCommand(shortHistory[0], shortHistory, longHistory, sizeOfLong, apartments, fileName);
		enterCommandToTextFile(shortHistory[0], fileName);

	}

	else if (strcmp(str, "find-apt") == 0)
	{
		int order = NO_ORDER;
		short int MinNumRooms = 0, MaxNumRooms = 0;
		int  MaxPrice = 0, numOfDays = 0;
		DATE date;

		date.day = date.month = date.year = 0;

		char temp1[3];

		str = strtok(NULL, " ");

		while (str != NULL)
		{
			if (strcmp(str, "-MinNumRooms") == 0)
			{
				str = strtok(NULL, " ");
				sscanf(str, "%hd", &MinNumRooms);
			}
			else if (strcmp(str, "-MaxPrice") == 0)
			{
				str = strtok(NULL, " ");
				sscanf(str, "%d", &MaxPrice);
			}
			else if (strcmp(str, "-MaxNumRooms") == 0)
			{
				str = strtok(NULL, " ");
				sscanf(str, "%hd", &MaxNumRooms);
			}
			else if (strcmp(str, "-Date") == 0)
			{
				str = strtok(NULL, " ");
				// finiding the day 
				for (i = 0; i < 2; i++)
					temp1[i] = str[i];
				temp1[i] = '\0';
				sscanf(temp1, "%hd", &date.day);
				// finding the month 
				for (i = 2; i < 4; i++)
					temp1[i - 2] = str[i];
				temp1[i - 2] = '\0';
				sscanf(temp1, "%hd", &date.month);
				//finding the year 
				for (i = 6; i < 8; i++)
					temp1[i - 6] = str[i];
				temp1[i - 6] = '\0';
				sscanf(temp1, "%hd", &date.year);
			}
			else if (strcmp(str, "-s"))
			{
				str = strtok(NULL, " ");
				order = LOWER_TO_HIGHER;
			}
			else if (strcmp(str, "-sr"))
			{
				str = strtok(NULL, " ");
				order = HIGHER_TO_LOWER;
			}
			else if (strcmp(str, "Enter"))
			{
				str = strtok(NULL, " ");
				sscanf(str, "%d", &numOfDays);
			}
			str = strtok(NULL, " ");
		}
		findApt(MinNumRooms, MaxNumRooms, MaxPrice, date, order, apartments, numOfDays);
	}
	else if (strcmp(str, "add-apt") == 0)
	{
		char* address;
		int price;
		short int rooms;
		DATE date;
		int index = 1;

		str = strtok(NULL, "\"");
		address = (char*)malloc((strlen(str) + 1) * sizeof(char));
		checkMemory(address);
		strcpy(address, str);
		address[strlen(str)] = '\0';

		while (str != NULL)
		{
			str = strtok(NULL, " ");
			if (index == 1)
				sscanf(str, "%d", &price);
			else if (index == 2)
				sscanf(str, "%hd", &rooms);
			else if (index == 3)
				sscanf(str, "%hd", &date.day);
			else if (index == 4)
				sscanf(str, "%hd", &date.month);
			else if (index == 5)
				sscanf(str, "%hd", &date.year);
			index++;
		}
		addApt(apartments, address, price, rooms, date);
	}

	else if (str[0] == '!')
	{
		int i = 0;
		BOOL found = FALSE;
		while ((command[i] != '\0') && !found)
		{
			if (command[i] == '^')
				found = TRUE;
			i++;
		}
		if (!found)
		{
			int commandNum;
			char*temp;

			temp = (char*)malloc((strlen(str) + 1) * sizeof(char));
			checkMemory(temp);
			strcpy(temp, str + 1);
			temp[strlen(str)] = '\0';
			sscanf(temp, "%d", &commandNum);
			if (*sizeOfLong < commandNum) // if the command number is smaller than sizeOflong, we will check the command in the shortHistory array.
			{
				char* newCommand;
				newCommand = (char*)malloc((strlen(shortHistory[(N - (commandNum - *sizeOfLong))]) + 1) * sizeof(char));
				strcpy(newCommand, shortHistory[(N - (commandNum - *sizeOfLong))]);
				newCommand[strlen(shortHistory[(N - (commandNum - *sizeOfLong))])] = '\0';
				enterCommandToArr(shortHistory, newCommand, longHistory, sizeOfLong); //enter the new command after all the changes into the shortHistory array 
				checkCommand(newCommand, shortHistory, longHistory, sizeOfLong, apartments, fileName);
				enterCommandToTextFile(newCommand, fileName);
			}
			else // we will look for the command in the longHisrtoy list
			{
				COLNODE* curr = longHistory->head;

				for (i = 1; i <= commandNum; i++)
				{
					if (i == commandNum)
					{
						char* newCommand;
						newCommand = (char*)malloc((strlen(curr->command) + 1) * sizeof(char));
						strcpy(newCommand, curr->command);
						newCommand[strlen(curr->command)] = '\0';
						enterCommandToArr(shortHistory, newCommand, longHistory, sizeOfLong); //enter the new command after all the changes into the shortHistory array
						checkCommand(newCommand, shortHistory, longHistory, sizeOfLong, apartments, fileName);
						enterCommandToTextFile(newCommand, fileName);
					}
					else
						curr = curr->next;
				}
			}
			free(temp);
		}
		else
		{
			int commandNum;
			char* str1, *str2;
			char* newCommand;

			str = strtok(command, "^");
			sscanf(str + 1, "%d", &commandNum);

			str = strtok(NULL, "^");
			str1 = (char*)malloc((strlen(str) + 1) * sizeof(char));
			checkMemory(str1);
			strcpy(str1, str);
			str1[strlen(str)] = '\0';

			str = strtok(NULL, "^");
			str2 = (char*)malloc((strlen(str) + 1) * sizeof(char));
			checkMemory(str2);
			strcpy(str2, str);
			str2[strlen(str)] = '\0';

			if (*sizeOfLong < commandNum) // if the command number is smaller than sizeOflong, we will check the command in the shortHistory array.
			{
				newCommand = checkAndReplaceCommand(shortHistory[(N - (commandNum - *sizeOfLong))], str1, str2, shortHistory, longHistory, sizeOfLong, apartments);
				enterCommandToTextFile(newCommand, fileName);

			}
			else // we will look for the command in the longHisrtoy list
			{
				COLNODE* curr = longHistory->head;

				for (i = 1; i <= commandNum; i++)
				{
					if (i == commandNum)
					{
						newCommand = checkAndReplaceCommand(curr->command, str1, str2, shortHistory, longHistory, sizeOfLong, apartments);
						(newCommand, fileName);
					}
					else
						curr = curr->next;
				}
			}
			enterCommandToArr(shortHistory, newCommand, longHistory, sizeOfLong); //enter the new command after all the changes into the shortHistory array 
			checkCommand(newCommand, shortHistory, longHistory, sizeOfLong, apartments, fileName);
			free(str1);
			free(str2);
		}
	}

	free(temp);
}

char* checkAndReplaceCommand(char command[], char* str1, char* str2, char** shortHisory, COLIST* longHistory, int* sizeOfLong, LIST* apartments)
{
	int commandSize = strlen(command);
	int str1Size = strlen(str1);
	int str2Size = strlen(str2);
	int ptrSize;
	int tempSize;
	int i, read, write;
	int count;
	char* res;
	char*ptr; // will hold the address of the string str1 in command

	count = HowManyTimesStrAppers(command, str1); // how many times str1 appers in command

	res = (char*)malloc((commandSize - str1Size + str2Size) * sizeof(char));
	checkMemory(res);

	ptr = strstr(command, str1);
	ptrSize = strlen(ptr);

	tempSize = commandSize - ptrSize;
	for (i = 0; i < tempSize; i++)
		res[i] = command[i];
	for (i = tempSize; i < tempSize + str2Size; i++)
		res[i] = str2[i - tempSize];

	read = str1Size;
	write = tempSize + str2Size;

	while (ptr[read] != '\0')
	{
		res[write] = ptr[read];
		write++;
		read++;
	}

	res[write] = '\0';
	count--;
	if (count > 0)
		res = checkAndReplaceCommand(res, str1, str2, shortHisory, longHistory, sizeOfLong, apartments);
	return res;
}

int HowManyTimesStrAppers(char* command, char * str1)
{
	int res = 0;
	char * ptr;
	char*temp;

	ptr = strstr(command, str1);

	while (ptr != NULL)
	{
		res++;
		temp = (char*)malloc((strlen(ptr) + 1) * sizeof(char));
		strcpy(temp, ptr + 1);
		temp[strlen(ptr)] = '\0';
		ptr = strstr(temp, str1);
		free(temp);
	}
	return res;
}

void printLongHistory(COLIST* longHistory, int sizeOfLong)
{
	COLNODE * curr = longHistory->head;
	int index = 1;

	while (curr != NULL)
	{
		printf("%d: %s\n", index, curr->command);
		curr = curr->next;
		index++;
	}
}

void printShortHistory(char **shortHistory, int sizeOfLong)
{
	int i;
	int write = 1;
	int commandNum = sizeOfLong;

	for (i = N - 1; i >= 0; i--)
	{
		if (shortHistory[i] != NULL)
		{
			printf("%d: %s\n", commandNum + write, shortHistory[i]);
			write++;
		}
	}
}

void mergeSort(APRT* apartments, int size, int ind)
{
	APRT *temp; //will hold a temporary array
	int i;//loop index

	if (size <= 1) //if the size of the araay is smaller or equal to 1 we do nothing
		return;

	else // (size>1)
	{
		mergeSort(apartments, size / 2, ind);//activates 'mergeSort' on the first half of the array
		mergeSort(apartments + size / 2, size - size / 2, ind);//activates 'mergeSort' on the second half of the array
		temp = (APRT*)malloc(size * sizeof(APRT));//initialization of 'temp'
		checkMemory(temp);
		sortedMerge(apartments, size / 2, apartments + size / 2, size - size / 2, temp, ind);//merges the two sorted arrays into one array
	}
	for (i = 0; i < size; i++)//copies the values from 'temp' to 'data'
		apartments[i] = temp[i];
	free(temp);
}

void sortedMerge(APRT* arr1, int size1, APRT* arr2, int size2, APRT* temp, int ind)
{
	int read1 = 0, read2 = 0, write = 0;

	if (ind == LOWER_TO_HIGHER) //from lower price to higher price
	{
		while ((read1 < size1) && (read2 < size2)) //going threw the arrays and copies the values to 'temp',from lower to higher
		{
			if (arr1[read1].price >= arr2[read2].price)
				temp[write++] = arr1[read1++];

			else
				temp[write++] = arr2[read2++];

		}//while ((read1 < size1) && (read2 < size2))

		while (read1 < size1)
			temp[write++] = arr1[read1++];

		while (read2 < size2)
			temp[write++] = arr2[read2++];
	}
	else // from higer to lower
	{
		while ((read1 < size1) && (read2 < size2)) //going threw the arrays and copies the values to 'temp',from higher to lower
		{
			if (arr1[read1].price <= arr2[read2].price)
				temp[write++] = arr1[read1++];

			else
				temp[write++] = arr2[read2++];

		}//while ((read1 < size1) && (read2 < size2))

		while (read1 < size1)
			temp[write++] = arr1[read1++];

		while (read2 < size2)
			temp[write++] = arr2[read2++];
	}

}//sortedMerge



void enterCommandToTextFile(char* command, char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "a");
	fileCheck(fp);

	fprintf(fp, "%s\n", command);
	fclose(fp);
}





void saveApartInBinFile(LIST aprt, char* fileName)
{
	FILE* fp;
	remove(fileName);
	fp = fopen(fileName, "wb");
	fileCheck(fp);
	LNODE* curr;
	short int addressLen;
	BYTE *arr;

	arr = (BYTE*)calloc(5, sizeof(BYTE));
	checkMemory(arr);

	curr = aprt.head;
	while (curr != NULL)
	{
		addressLen = strlen(curr->aprtInfo.address);

		fwrite(&(curr->aprtInfo.code), sizeof(short int), 1, fp);
		fwrite(&addressLen, sizeof(short int), 1, fp);
		fwrite(curr->aprtInfo.address, sizeof(char), addressLen, fp);
		fwrite(&(curr->aprtInfo.price), sizeof(int), 1, fp);
		shrinkData(arr, curr->aprtInfo.numRooms, curr->aprtInfo.dateOfEntrance, curr->aprtInfo.dateOfEntranceToList);
		fwrite(arr, sizeof(BYTE), 5, fp);
		curr = curr->next;

	}
	fclose(fp);
}

void shrinkData(BYTE* arr, short int numRooms, DATE dateOfEntrance, DATE dateOfEntranceToList)
{
	arr[0] = numRooms << 4;										//arr[0] = nnnn 0000
	arr[0] = arr[0] | (dateOfEntrance.day >> 1);				//arr[0] = nnnn dddd
	arr[1] = (dateOfEntrance.day << 7);							//arr[1] = d000 0000
	arr[1] = arr[1] | (dateOfEntrance.month << 3);				//arr[1] = dmmm m000
	arr[1] = arr[1] | (dateOfEntrance.year >> 4);				//arr[1] = dmmm myyy
	arr[2] = (dateOfEntrance.year << 4);						//arr[2] = yyyy 0000
	arr[3] = (dateOfEntranceToList.day << 3);					//arr[3] = dddd d000
	arr[3] = arr[3] | (dateOfEntranceToList.month >> 1);		//arr[3] = dddd dmmm
	arr[4] = (dateOfEntranceToList.month << 7);					//arr[4] = m000 0000
	arr[4] = arr[4] | (dateOfEntranceToList.year);				//arr[4] = myyy yyyy


		// arr =  nnnndddd dmmmmyyy yyyy0000 dddddmmm myyyyyyy

}

void printArr(APRT* arr, int size)
{
	int i;
	for (i = 0; i < size; i++)
		printAprtfromArr(arr[i]);
}

void printAprtfromArr(APRT apart)
{
	printf("Apt details:\n");
	printf("Code: %d\n", apart.code);
	printf("Address: %s\n", apart.address);
	printf("Number of rooms:%d\n", apart.numRooms);
	printf("Price: %d\n", apart.price);
	printf("Entry date: %d.%d.%d\n", apart.dateOfEntrance.day, apart.dateOfEntrance.month, apart.dateOfEntrance.year + 2000);
	printf("Database entry date: %d.%d.%d\n", apart.dateOfEntranceToList.day, apart.dateOfEntranceToList.month, apart.dateOfEntranceToList.year + 2000);
}

void printAprtFromList(LNODE* curr)
{
	printf("Apt details:\n");
	printf("Code: %d\n", curr->aprtInfo.code);
	printf("Address: %s\n", curr->aprtInfo.address);
	printf("Number of rooms: %d\n", curr->aprtInfo.numRooms);
	printf("Price: %d\n", curr->aprtInfo.price);
	printf("Entry date: %d.%d.%d\n", curr->aprtInfo.dateOfEntrance.day, curr->aprtInfo.dateOfEntrance.month, curr->aprtInfo.dateOfEntrance.year + 2000);
	printf("Database entry date: %d.%d.%d\n", curr->aprtInfo.dateOfEntranceToList.day, curr->aprtInfo.dateOfEntranceToList.month, curr->aprtInfo.dateOfEntranceToList.year + 2000);
}
void fileCheck(FILE* fp)
{
	if (fp == NULL)
	{
		puts("Memory allocation failed");
		exit(1);
	}
}//fileCheck

LIST readAprtsFromBinFile(char* fileName)
{
	LIST res;
	FILE* fp;
	LNODE* curr;
	short int addressLen, code;
	short int numRooms = 0;
	short int day = 0, month = 0, year = 0;
	short int dayOfEntranceToList = 0, monthOfEntranceToList = 0, yearOfEntranceToList = 0;
	int price;
	char* address;
	BYTE temp;
	DATE dateOfEntrace, dateOfEntranceToList;
	makeEmptyList(&res);
	fp = fopen(fileName, "ab");
	fileCheck(fp);
	fclose(fp);
	fp = fopen(fileName, "rb");
	fileCheck(fp);
	while (fread(&code, sizeof(short int), 1, fp) != 0)//going threw the file
	{
		fread(&addressLen, sizeof(short int), 1, fp);
		address = (char*)malloc((addressLen + 1) * sizeof(char));
		checkMemory(address);
		fread(address, sizeof(char), addressLen, fp);
		address[addressLen] = '\0';
		fread(&price, sizeof(int), 1, fp);

		fread(&temp, sizeof(BYTE), 1, fp); //temp = nnnn dddd
		numRooms = (temp);  //numRooms = 0000 nnnn
		numRooms >>= 4;  //numRooms = 0000 nnnn
		temp <<= 4; //temp= dddd 0000
		temp >>= 3; //temp = 000d ddd0
		day = (temp); //day= 000d ddd0

		fread(&temp, sizeof(BYTE), 1, fp); //temp= dmmm myyy
		day = day | (temp >> 7); //day = 000d dddd
		temp <<= 1; //temp = mmmm yyy0
		temp >>= 1; //temp = 0mmm myyy
		month = (temp >> 3); //month= 0000 mmmm
		temp <<= 5; //temp = yyy0 0000
		temp >>= 1; //temp = 0yyy 0000
		year = (temp); //year= 0yyy 0000

		fread(&temp, sizeof(BYTE), 1, fp); //temp= yyyy 0000
		year = year | (temp >> 4); //year= 0yyy yyyy

		fread(&temp, sizeof(BYTE), 1, fp); //temp=  dddd dmmm
		dayOfEntranceToList = (temp >> 3); //dayOfEntranceToList = 000d dddd
		temp <<= 5; //temp = mmm0 0000
		temp >>= 4; //temp = 0000 mmm0
		monthOfEntranceToList = temp; //monthOfEntranceToList = 0000 mmm0

		fread(&temp, sizeof(BYTE), 1, fp); //temp= myyy yyyy
		monthOfEntranceToList = monthOfEntranceToList | (temp >> 7); //monthOfEntranceToList= 0000 mmmm
		yearOfEntranceToList = (temp << 1); //yearOfEntranceToList = yyyy yyy0
		yearOfEntranceToList >>= 1; //yearOfEntranceToList = 0yyy yyyy

		dateOfEntrace.day = day;
		dateOfEntrace.month = month;
		dateOfEntrace.year = year;
		dateOfEntranceToList.day = dayOfEntranceToList;
		dateOfEntranceToList.month = monthOfEntranceToList;
		dateOfEntranceToList.year = yearOfEntranceToList;
		curr = creatNewListNode(address, price, numRooms, dateOfEntrace, code, NULL, dateOfEntranceToList);

		insertNodeToEndList(&res, curr);
	}
	return res;

}

void readCommandsFromTextFile(char* fileName, COLIST* longHistory, char** shortHisory, int *numOfCommands)
{
	FILE * fp;
	char * command;
	char ch;
	int size;
	long int fileSzize;
	long int place;

	//int i;
	COLNODE* curr;
	fp = fopen(fileName, "a");
	fclose(fp);

	fp = fopen(fileName, "r");
	fileCheck(fp);
	fileSzize = getFileSize(fp);

	while (ftell(fp) < fileSzize)
	{

		place = ftell(fp);
		size = 0;
		ch = fgetc(fp);

		while (ch != '\n')
		{
			size++;
			ch = fgetc(fp);
		}
		size++;

		fseek(fp, place, SEEK_SET);

		//get command
		command = (char*)malloc(sizeof(char)*size);
		checkMemory(command);
		fgets(command, size, fp);
		fgetc(fp);         //for the '\n'
		curr = createNewCoListNode(command, NULL);
		insertNodeToEndCoList(longHistory, curr);
		(*numOfCommands)++;
	}
	fclose(fp);
}

long int getFileSize(FILE* fp)
{
	long int res, saver;
	saver = ftell(fp);
	fseek(fp, 0, SEEK_END);
	res = ftell(fp);
	fseek(fp, saver, SEEK_SET);
	return res;

}

void freeShortTermHistory(char** shortHistory)
{
	int i = 0;
	for (i = 0; i < N; i++)
		free(shortHistory[i]);

}

void freeLongTermHistory(COLIST longHistory)
{
	COLNODE* curr;
	curr = longHistory.head;
	while (curr != NULL)
	{
		free(curr->command);
		curr = curr->next;
	}

}

void freeApartments(LIST apartments)
{
	LNODE* curr;
	curr = apartments.head;
	while (curr != NULL)
	{
		free(curr->aprtInfo.address);
		curr = curr->next;
	}
}


void enterLastCommandToShortHistory(char**shortHistory, int size, COLIST* longHistory, int* sizeOfLong)
{
	COLNODE * curr;
	COLNODE* savePtr;
	int read = 0;
	int startToCopy = size - 7;
	int i, j, saveIndex;
	BOOL foundNull = FALSE;
	BOOL flag = FALSE;

	curr = longHistory->head;

	if (size <= 7)
	{
		makeEmptyCOlList(longHistory);
		while (curr != NULL)
		{
			foundNull = FALSE;
			if (shortHistory[0] == NULL) // if the first cells is NULL we will enter the command 
			{
				shortHistory[0] = (char*)malloc((strlen(curr->command) + 1) * sizeof(char));
				checkMemory(shortHistory[0]);
				strcpy(shortHistory[0], curr->command);
				shortHistory[0][strlen(curr->command)] = '\0';
			}
			else // we are looking for the first cell that is NULL
			{
				for (i = 0; i < N && !foundNull; i++)
				{
					if (shortHistory[i] == NULL)
					{
						foundNull = TRUE;
						saveIndex = i;
						for (j = saveIndex - 1; j >= 0; j--)
						{
							free(shortHistory[j + 1]);
							shortHistory[j + 1] = (char*)malloc((strlen(shortHistory[j]) + 1) * sizeof(char));
							checkMemory(shortHistory[j + 1]);
							strcpy(shortHistory[j + 1], shortHistory[j]);
							shortHistory[j + 1][strlen(shortHistory[j])] = '\0';
						}
						free(shortHistory[0]);
						shortHistory[0] = (char*)malloc((strlen(curr->command) + 1) * sizeof(char));
						checkMemory(shortHistory[0]);
						strcpy(shortHistory[0], curr->command);
						shortHistory[0][strlen(curr->command)] = '\0';
					}
				}
			}
			curr = curr->next;
		}
	}
	while (curr != NULL)
	{
		savePtr = curr;

		if (read == startToCopy - 1)
		{
			longHistory->tail = curr;
			savePtr = curr->next;
			flag = TRUE;
			curr->next = NULL;
			*sizeOfLong = read + 1;
		}

		if (read >= startToCopy)
		{
			foundNull = FALSE;
			if (shortHistory[0] == NULL) // if the first cells is NULL we will enter the command 
			{
				shortHistory[0] = (char*)malloc((strlen(curr->command) + 1) * sizeof(char));
				checkMemory(shortHistory[0]);
				strcpy(shortHistory[0], curr->command);
				shortHistory[0][strlen(curr->command)] = '\0';
			}
			else // we are looking for the first cell that is NULL
			{
				for (i = 0; i < N && !foundNull; i++)
				{
					if (shortHistory[i] == NULL)
					{
						foundNull = TRUE;
						saveIndex = i;
						for (j = saveIndex - 1; j >= 0; j--)
						{
							free(shortHistory[j + 1]);
							shortHistory[j + 1] = (char*)malloc((strlen(shortHistory[j]) + 1) * sizeof(char));
							checkMemory(shortHistory[j + 1]);
							strcpy(shortHistory[j + 1], shortHistory[j]);
							shortHistory[j + 1][strlen(shortHistory[j])] = '\0';
						}
						free(shortHistory[0]);
						shortHistory[0] = (char*)malloc((strlen(curr->command) + 1) * sizeof(char));
						checkMemory(shortHistory[0]);
						strcpy(shortHistory[0], curr->command);
						shortHistory[0][strlen(curr->command)] = '\0';

					}
				}
			}
			savePtr = curr->next;
			free(curr);
		}

		if (!flag)
			curr = savePtr->next;
		else
			curr = savePtr;

		read++;
	}
}