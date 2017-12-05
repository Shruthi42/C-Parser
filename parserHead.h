#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"constants.h"
#include<map>
#include<vector>
#include<string>
#include<utility>
#include<fstream>
#include<iostream>
#include<stack>
#include<algorithm>
#include "defineLineSize.h"

#define accept 9999
#define shift 9998
#define reduce 9997

using namespace std;

extern FILE * yyin;
extern int yylex();
extern int lines,column;
int l = 0,c = 0;
char fl[MAXLINESIZE],curline[MAXLINESIZE];
FILE * read;

int token;
int err = 0;
ofstream log("Log",ios::out);
FILE * tokenOut = fopen("Output.c","w");

//Define maps to hold ACTION table, GOTO table and state stack
typedef multimap<int,pair<string,pair<int,pair<int,string> > > > ACTION;
typedef multimap<int, pair<string,int> > GOTO;
typedef stack<pair<string,int> > STACK;

//Declare  variables to hold the ACTION table, GOTO table and stack
ACTION act;
GOTO gt;
STACK s;

char arrow[] = "\xe2\x86\xb3";
char point[] = "\xe2\x96\xb7";
char warning[] = "\xe2\x9a\xa0";
char tab = '\t';

void readTable();
int performAction(char * token);
void errorHandle(int,string);
void goto_(string, int);
void printActionTable();
int phraseHandled(int, string);
void recovery_Goto(char[],int,char[]);
void recovery_action(char[],char[]);
void printError(int);

int inputConsumed = 0;

//Initializes file streams
int init(char * filename )
{
yyin = fopen(filename,"r");
read = fopen(filename,"r");
if(yyin==NULL)
{
	cout<<"Input file does not exist."<<endl<<"Exiting"<<endl<<endl;
	exit(0);
}
return 0;
}
