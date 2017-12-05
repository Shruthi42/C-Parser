#ifndef INCLUDE
#define INCLUDE
#include<stdio.h>
#include<stdlib.h>
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include"constants.h"
#endif

#ifndef STRING
#define STRING
#include<string.h>
#endif

#ifndef STR
#define STR
#include"str.h"
#endif

#ifndef TOKEN
#define TOKEN
#include"tokens.h"
#endif

#define SZ sizeof(tokenList)

/*
tokenList can hold a list of terminals and non terminals
tokenList is used to hold the body of productions as well as FIRST, FOLLOW and lookahead during LR1 parse table construction
*/
struct tokenlist
	{
		char token[MAXTOKENSIZE+1];
		struct tokenlist* next;
		
	};

typedef struct tokenlist tokenList; 

/*
production holds a single production rule
It holds the entire rule as a string
In addition, head and beta respectively give the head and body of the production.
a unique number (number) is associated with each production
*/
typedef struct
	{	
		char rule[MAXRULESIZE+1];
		char head[MAXTOKENSIZE+1];
		tokenList *beta;
		short length;
		int number;
		tokenList *first; //not in use, for convenience only
		tokenList *follow; //not in use, for future use
	} production;

/*
A processed production holds a production along with the lookahead and the position of processing
lookahead isimplemented as a tokenList
*/
typedef struct
	{	
		production p;
		int point;
		tokenList* lookahead;			
	} processedProduction;

/*
productionList holds a list of productions
productionList is implemented as a linked list and each node holds an production
*/
struct productionlist
	{
		production p;
		struct productionlist* next; 
	};

typedef struct productionlist productionList;

/*
processedProductionList holds a list of processed productions
processedProductionList is implemented as a linked list and each node holds a processed production 
*/
struct processedproductionlist
	{
		processedProduction p;
		struct processedproductionlist* next; 
	};

typedef struct processedproductionlist processedProductionList;

/*
An item set holds the list of processed productions which exist in the item set
It also holds a unique item number
*/
typedef struct
	{
		processedProductionList * pl;
		int number;
	} itemSet;


/*
itemSetList holds a list of item states
itemSetList is implemented as a linked list, each node of which holds an item set
*/
struct itemsetlist
	{
		itemSet i;
		struct itemsetlist* next; 
	};

typedef struct itemsetlist itemSetList;

productionList * readProductions(char *);
void freeProductions(productionList *);
itemSetList * createItemSets(productionList *, FILE *);
tokenList * FIRST(tokenList *,productionList *);
void findFIRST(tokenList *,tokenList * *,productionList *, tokenList * *);
int containsEpsilon(tokenList*);
void removeDuplicates(tokenList*);
void addStates(itemSetList *, productionList *, FILE *);	
void goTo(itemSetList *, TokenTable*,itemSetList*,productionList *, FILE * fp);
void closure(itemSet*,productionList *);
int same(itemSetList *, itemSetList *);	
processedProductionList * sortPL(processedProductionList **);
tokenList * sortTL(tokenList **);
void addReduce(itemSet, FILE *);
int notInClosureList(processedProductionList *, processedProductionList *);
int compareProcessedProductions(processedProduction, processedProduction);
int containsToken(tokenList *, tokenList *);
int isGreaterLookahead(tokenList *, tokenList *);
void printItemSetList(itemSetList *);
void printItemSet(itemSet);
void removeEmpty(tokenList **);
