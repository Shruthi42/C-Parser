#include<iomanip>



void printActionTable(char* len)
{
	cout << setiosflags(ios::left);

	ACTION::iterator i1 = act.begin();

	if(!strcmp(len,"full")) //Output entire table
	{
		for(;i1!=act.end();i1++)
		{

			cout<<"State "<<setw(3)<<i1->first<<" on token \t\t"<<setw(15)<<i1->second.first;
			if(i1->second.second.first==accept)
			{
				cout<<" accept"<<endl;
			}			
			else if(i1->second.second.first==shift)
			{
				cout<<" SHIFT\t\t to state "<<i1->second.second.second.first<<endl;
			}
			else
			{
				cout<<" REDUCE\t\t by rule "<<i1->second.second.second.first<<endl;
			}
		}
	}
	else	//Output specified number of lines
	{
		int num = atoi(len);
		for(;i1!=act.end()&&num;i1++,num--)
		{

			cout<<"State "<<setw(3)<<i1->first<<" on token \t\t"<<setw(15)<<i1->second.first;
			if(i1->second.second.first==accept)
			{
				cout<<" ACCEPT"<<endl;
			}			
			else if(i1->second.second.first==shift)
			{
				cout<<" SHIFT\t\t to state "<<i1->second.second.second.first<<endl;
			}
			else
			{
				cout<<" REDUCE\t\t by rule "<<i1->second.second.second.first<<endl;
			}
		}
	}
}

void printGotoTable(char* len)
{
	GOTO::iterator i1 = gt.begin();

	cout << setiosflags(ios::left);

	if(!strcmp(len,"full")) //Output entire table
	{
		for(;i1!=gt.end();i1++)
		{
			cout<<"State "<<setw(3)<<i1->first<<" on nonTerminal \t\t"<<setw(30)<<i1->second.first;
			cout<<" GOTO\t\t State"<<i1->second.second<<endl;
		}
	}
	else	//Output specified number of lines
	{
		int num = atoi(len);
		for(;i1!=gt.end()&&num;i1++,num--)
		{
			cout<<"State "<<setw(3)<<i1->first<<" on nonTerminal \t\t"<<setw(30)<<i1->second.first;
			cout<<" GOTO\t\t State"<<i1->second.second<<endl;
		}
	}
}

void readTable()
{
	ifstream input("Table",ios::in);
	if(!input)
	{
		cout<<"Fatal error. No parse table"<<endl;
		exit(0);
	}

	char word1[MAXTOKENSIZE];
	char word2[MAXTOKENSIZE];
	char word3[MAXTOKENSIZE];
	char word4[MAXTOKENSIZE];
	char word5[MAXTOKENSIZE];
	char word6[MAXTOKENSIZE];

	while(input>>word1)
	{
		input>>word2>>word3;

		if(!strcmp(word1,"GOTO"))
		{
			input>>word4;
			gt.insert(make_pair(atoi(word2),make_pair(word3,atoi(word4))));
		}

		else if(!strcmp(word1,"SHIFT"))
		{
			input>>word4;
			act.insert(make_pair(atoi(word2),make_pair(word3,make_pair(shift,make_pair(atoi(word4),"NULL")))));
		}
		else if(!strcmp(word1,"REDUCE"))
		{
			input>>word4>>word5>>word6;
			act.insert(make_pair(atoi(word2),make_pair(word3,make_pair(atoi(word5),make_pair(atoi(word4),word6)))));
		}
		if(!strcmp(word1,"ACCEPT"))
		{
			act.insert(make_pair(atoi(word2),make_pair(word3,make_pair(accept,make_pair(0,string("NULL"))))));
		}

	}

}
