#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include"constants.h"

using namespace std;

main(int argc, char * argv[])
{

	if(argc!=3)
	{
		cout<<"Usage: ./printGrammar <file name> <number of lines>"<<endl;
		cout<<"number of lines can be 'full' or a number"<<endl;
		exit(0);
	}

	ifstream grammar(argv[1],ios::in);

	if(!grammar)
	{
		cout<<"Fatal error. Input file not found."<<endl;
		exit(0);
	}

	cout<<endl<<"\t\tCONTEXT FREE GRAMMAR PRODUCTIONS"<<endl<<endl;

	char arrow[] = "->";

	string production;
	int len;

	if(!strcmp(argv[2],"full"))
		len= 1;
	else	
		len = atoi(argv[2]);

	while(getline(grammar,production)&&len)
	{
		char prod[MAXRULESIZE];
		strcpy(prod,production.c_str());
		
		int i = 0;

		while(prod[i]!=' ')
		{
			cout<<prod[i++];
		}

		cout<<" "<<arrow<<prod+i<<endl;

		if(strcmp(argv[2],"full"))
			len--;
	}
	cout<<endl;

}
