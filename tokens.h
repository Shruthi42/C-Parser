#include<stdio.h>
#include<stdlib.h>

#ifndef STR
#define STR
#include"str.h"
#endif

typedef struct tt
{
	char token[MAXTOKENSIZE+1];
	struct tt *next;	
}TokenTable;

TokenTable table;
void initTokenTable(char *filename)
{
	FILE *fp = fopen(filename,"rt");

	TokenTable *head = &table;
	char buffer[1024];
	int i;
	while(fscanf(fp,"%s",buffer))
	{
		if(feof(fp)) break;
		if(buffer[strlen(buffer)-1]==' ') buffer[strlen(buffer)-1]='\0'; 
		strcpymine(head->token,buffer);
		head->next = (TokenTable *)malloc(sizeof(TokenTable));
		head = head->next;
//		puts(buffer);
		if(feof(fp))
			break;
	}
	head->next = NULL;
}
void displayTokenTable()
{
	static int i = 1;
	if(i)
	{
		initTokenTable("InputGrammarTokens");
			i = 0;
	}
	TokenTable *head = &table;
	while(head->token)
	{
		puts(head->token);
		head = head->next;		
	}
}

int isToken(char *word)
{
	//added not tested
	if(strlen(word)==1)return 1;
	//added not tested
	if(!strcmp(word,"ENDSYMBOL")) return 1;

	static int i  = 1;
	if(i)
	 {
		initTokenTable("InputGrammarTokens");
		i = 0;
	}

	TokenTable *head = &table;
	while(head->token!=NULL)
	{
		if(!strcmp(head->token,word))
				return 1;
		head = head->next;
	}
	return 0;
}

TokenTable table1;
void initNonterminalTable(char *filename)
{
	FILE *fp = fopen(filename,"rt");

	TokenTable *head = &table1;
	char buffer[1024];
	int i;
	while(fscanf(fp,"%s",buffer))
	{
		if(feof(fp)) break;
		if(buffer[strlen(buffer)-1]==' ') buffer[strlen(buffer)-1]='\0'; 
		strcpymine(head->token,buffer);
		head->next = (TokenTable *)malloc(sizeof(TokenTable));
		head = head->next;
//		puts(buffer);
		if(feof(fp))
			break;
	}
	head->next = NULL;
}
void displayNonterminalTable()
{
	static int i = 1;
	if(i)
	{
		initNonterminalTable("InputGrammarNonterminals");
			i = 0;
	}
	TokenTable *head = &table1;
	while(head->token)
	{
		puts(head->token);
		head = head->next;		
	}
}

int isNonterminal(char *word)
{

	//added not tested
	if(!strcmp(word,"AUGMENTEDSTART")) return 1;

	static int i  = 1;
	if(i)
	 {
		initNonterminalTable("InputGrammarNonterminals");
		i = 0;
	}

	TokenTable *head = &table1;
	while(head->token!=NULL)
	{
		if(!strcmp(head->token,word))
				return 1;
		head = head->next;
	}
	return 0;
}
