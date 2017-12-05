#include"ReturnToken.h"
#include"printTables.h"
#ifndef PARSE
#define PARSE
#include"parse.h"
#endif

using namespace std;

main(int argc,char * argv[])
{

	if(argc!=3)
	{
		cout<<"Usage: ./printTables <table> <number of lines>"<<endl;
		cout<<"table may be ACTION, GOTO or ALL"<<endl;
		cout<<"number of lines may be a number or full"<<endl;
	}

	readTable();

	if(!strcmp(argv[1],"ACTION")||!strcmp(argv[1],"ALL")) //Output ACTION table
	{
		cout<<endl<<"ACTION Table:"<<endl;
		printActionTable(argv[2]);
	}
	if(!strcmp(argv[1],"GOTO")||!strcmp(argv[1],"ALL")) //Ouput GOTO Table
	{
		cout<<endl<<"GOTO Table:"<<endl;
		printGotoTable(argv[2]);
	}
	
	if(!(!strcmp(argv[1],"ACTION")||!strcmp(argv[1],"GOTO")||!strcmp(argv[1],"ALL")))
	{
		cout<<"Incorrect commend line argument for table"<<endl;
		cout<<"table may be ACTION, GOTO or ALL"<<endl;
	}


}
