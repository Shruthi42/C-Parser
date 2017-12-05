#ifndef CONSTRUCTPARSER
#define CONSTRUCTPARSER
#include"ConstructParser.h"
#endif 


void testFIRST(productionList * prodList)
{

	tokenList *toFind = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(toFind->token,"codeblock");
	printf("isToken(\"%s\") is: %d\n",toFind->token, isToken(toFind->token));
	toFind->next = NULL;

	tokenList * first = FIRST(toFind,prodList);
	if(first==NULL)
	{
		printf("FIRST set is empty\n");
		return;
	}
	printf("First(%s) is: ",toFind->token);
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	printf("\n");
}

void testcontainsToken()
{

	tokenList *toFind = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(toFind->token,"expression");
	printf("isToken(\"%s\") is: %d\n",toFind->token, isToken(toFind->token));
	toFind->next = NULL;

	tokenList *list = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list->token,"expression");
	list->next = NULL;

	tokenList *list1 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list1->token,"term");
	list->next = NULL;

	tokenList *list2 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list2->token,"factor");
	list2->next = NULL;

	list->next = list1;
	list1->next =list2;

	printf("Result of containsToken(%s,list) is: %d",toFind->token,containsToken(toFind,list));
	printf("\n");
}

void testsortTL()
{

	tokenList *list = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list->token,";");
	list->next = NULL;

	tokenList *list1 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list1->token,"IDENTIFIER");
	list1->next = NULL;

	tokenList *list2 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list2->token,"IF");
	list2->next = NULL;

	tokenList *list3 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list3->token,";");
	list3->next = NULL;

	/*tokenList *list2 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list2->token,"factor");
	list2->next = NULL;
*/
	list->next = list1;
	list1->next =list2;
	list2->next =list3;


printf("before Sorting: ");

	tokenList * first = list;	
	while(first!=NULL)
	{
		printf("%s,\t",first->token);
		first = first->next;
	}
	
	printf("\n");


	list = sortTL(&list);
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s,\t",first->token);
		first = first->next;
	}
	
	printf("\n");
list = sortTL(&list);
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
list = sortTL(&list);
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
sortTL(&list);
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
sortTL(&list);

	printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
}

void testsameTokenList()
{

	tokenList *list = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list->token,"d");
	list->next = NULL;

	tokenList *list1 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list1->token,"c");
	list->next = NULL;

	tokenList *list2 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list2->token,"d");
	list2->next = NULL;

	tokenList *list3 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list3->token,"c");
	list3->next = NULL;

	list->next = list1;
	list2->next =list3;

printf("Sorted list: ");

	tokenList * first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");

	printf("sameTokenList(list,list): %d\n", sameTokenList(list,list2));
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
	printf("sameTokenList(list,list): %d\n", sameTokenList(list,list2));
	printf("sameTokenList(list,list): %d\n", sameTokenList(list,list2));
	printf("sameTokenList(list,list): %d\n", sameTokenList(list,list2));
	printf("sameTokenList(list,list): %d\n", sameTokenList(list,list2));
printf("Sorted list: ");

	first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");

}


void testsortPL()
{

	tokenList *list = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list->token,"expression");
	list->next = NULL;

	tokenList *list1 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list1->token,"term");
	list->next = NULL;

	tokenList *list2 = (tokenList*)malloc(sizeof(tokenList)); 
	strcpy(list2->token,"factor");
	list2->next = NULL;

	list->next = list1;
	list1->next =list2;

	sortTL(&list);

	printf("Sorted list: ");

	tokenList * first = list;	
	while(first!=NULL)
	{
		printf("%s\t",first->token);
		first = first->next;
	}
	
	printf("\n");
}

