#ifndef __APRT_H
#define __APRT_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

typedef unsigned char BYTE;

typedef struct date {
	short int day;
	short int month;
	short int year;

}DATE;

typedef struct apartment {
	short int code;
	char *address;
	int price;
	short int numRooms;
	DATE dateOfEntrance;
	DATE dateOfEntranceToList;

}APRT;

typedef struct lnode {
	APRT aprtInfo;
	struct lnode *next;

}LNODE;

typedef struct list {
	LNODE * head;
	LNODE * tail;

}LIST;

typedef struct colnode {
	char* command;
	struct colnode *next;

}COLNODE;

typedef struct colist {
	COLNODE * head;
	COLNODE * tail;

}COLIST;

#define SEC_IN_DAY 86400
#define N 7
#define NO_ORDER 0
#define LOWER_TO_HIGHER 1
#define HIGHER_TO_LOWER 2

/*****************************************************ptint functions**************************************************/

//the funcion will print all the info of the apartment from the array 
void printAprtfromArr(APRT apart);
//the funcion will print all the info of the apartment from the list
void printAprtFromList(LNODE* curr);
//the function will print all the info in apratments array 
void printArr(APRT* arr, int size);
//the function will print the array 'short term history'
void printShortHistory(char **shortHistory, int sizeOfLong);
//the function will print the list 'longHistory'
void printLongHistory(COLIST* longHistory, int sizeOfLong);


/****************************************************find-command functions***************************************************/

//the function will find and print all the apartments that will be available till the date the user entered
BOOL findAptDate(LNODE* curr, DATE date);
//the function will print the codes of the apartments that were entered to the list at the last 'num' days
BOOL findLastDaysAprt(LNODE* curr, int num);
//the function will print the apartments according to the user's data
void findApt(int MinNumRooms, int MaxNumRooms, int MaxPrice, DATE date, int order, LIST* listApt, int days);


/*****************************************************list functions*********************************************************/

//the function will add a new node of apartment into the reservoir
void addApt(LIST* lst, char* address, int price, short int numRooms, DATE date);
//the function will creat a new node the will include all the infromation that the user entered about the apartment 
LNODE* creatNewListNode(char* address, int price, short int numRooms, DATE date, short int code, LNODE*next, DATE dateOfEntrnceToList);
//the function will indert the new node to the end of the reservoir
void insertNodeToEndList(LIST *lst, LNODE * tail);
//the function will check if the list is empty or not
BOOL isEmptyList(LIST *lst);
//the function gets list and a code of apartment and will remove from the list the apartment with the same code
void buyAprt(LIST* lst, short int code);
//the function will remove from the list an apartment
void deleteAprt(LIST* lst, LNODE* node, int place);
//the function will check if the memory alloction succeeded
void checkMemory(void* ptr);
//the function will delete the codes of the apartments that were entered to the list at the last 'num' days
void deleteAprtByDay(LIST* lst, int num);
//will make an empty list 
void makeEmptyCOlList(COLIST * lst);
void makeEmptyList(LIST* lst);
//insert the new command into a node and enters into the end of the list
void insertDataToEndList(char* command, COLIST* long_term_history, int * size);
//the function will create a new command-list node
COLNODE* createNewCoListNode(char* command, COLNODE* next);
//the function will enter a node to the end of the list
void insertNodeToEndCoList(COLIST* long_term_history, COLNODE* newTail);

/****************************************************BITS- function*************************************************/

//the function will shrink data into 5 byte
void shrinkData(BYTE* arr, short int numRooms, DATE dateOfEntrance, DATE dateOfEntranceToList);


/***************************************************checking commands + sortings ****************************************************/
//will get a command from the user and enter it into an array
char* getInput(char** shortHisory, COLIST* longHistory, int* sizeOfLong, LIST apartments, char* fileName);
//the function will check the command and will replace the data of str1 with the data of str2
char* checkAndReplaceCommand(char command[], char* str1, char* str2, char** shortHisory, COLIST* longHistory, int* sizeOfLong, LIST* apartments);
//the function will check the command given my the user
void checkCommand(char command[], char** shortHistory, COLIST* longHistory, int* sizeOfLong, LIST* apartments, char* fileName);
// will sort a list of apartments by price
void mergeSort(APRT* apartments, int size, int ind);
void sortedMerge(APRT* arr1, int size1, APRT* arr2, int size2, APRT* temp, int ind);


/*********************************************shortHistory & longHistory- functions****************************************/

//the function will check and return how many time the string str1 appers in string command
int HowManyTimesStrAppers(char* command, char * str1);
//the function will sort the command in the longhistory, so the 7 last command will be in the shorthistory array and will be deleted from the longhistory list
void enterLastCommandToShortHistory(char**shortHistory, int size, COLIST* longHistory, int* sizeOfLong);
//enter the last command to the array 'ShortTermHistory'
void enterCommandToArr(char ** arr, char * command, COLIST * long_term_history, int *sizeOfLong);
//the function will reset the arr's cells to NULL
void resetShortTermHistory(char **arr, int size);

/*****************************************************files- functions***********************************************/

//the function will enter the commands into a text file
void enterCommandToTextFile(char* command, char* fileName);
//the function will check if the file opening succeeded
void fileCheck(FILE* fp);
//the function will read the apartments from the binary file to the list 
LIST readAprtsFromBinFile(char* fileName);
//the function will enter the commands into a text file
void enterCommandToTextFile(char* command, char* fileName);
//the function will save all the apartments into a binary file 
void saveApartInBinFile(LIST aprt, char* fileName);
//the function will read the command from the text file and will enter them into the longhistory list 
void readCommandsFromTextFile(char* fileName, COLIST* longHistory, char** shortHisory, int *numOfCommands);
// the function will get the size of the FILE
long int getFileSize(FILE* fp);


/**************************************************free functions************************************************/

//the function will free all the command in the shorthistory array
void freeShortTermHistory(char** shortHistory);
// the function will free all the commands in the longhistory list
void freeLongTermHistory(COLIST longHistory);
//the function will free all the apartments from the aprt list 
void freeApartments(LIST apartments);


/****************************************************** end *****************************************************/
#endif
