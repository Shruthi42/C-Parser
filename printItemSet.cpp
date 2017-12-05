#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<vector>

using namespace std;

main(int argc, char * argv[])
{

if(argc!=2)
{
	cout<<"Usage: ./printItemSet <Item Number>"<<endl;
	exit(0);
}

ifstream itemSets("ItemSets",ios::in);
ifstream states("States",ios::in);

if(!itemSets || !states)
{
	cout<<"Fatal error. Input file not found."<<endl;
	exit(0);
}


	//Input last state from file states
	char c;
	states.seekg(-2, ios::end);
	std::streampos size = states.tellg();
	for(int i=2;i<=size && c!='\n';i++)
	{
		states.seekg(-i,std::ios::end);
		states.get(c);
	}

	char lastnum[20];
	states>>lastnum;

	int lastState = atoi(lastnum);

	if((atoi(argv[1])<0)||(atoi(argv[1])>lastState))
	{
	cout<<"This  state does not exist. Last state is state "<<lastState<<endl<<endl;
	exit(0);
	}

	//char dot[] = "\xe2\x9a\xab";
	//char dot[] = "\xe2\x88\x99";  //Unicode 2219
	char dot[] = "\xe2\x80\xa2";	//Unicode 2022

	string word;
	string itemNo;
	while(itemSets>>word)
	{
		if(word=="Item")
		{
			itemSets>>itemNo;
			if(atoi(itemNo.c_str())==atoi(argv[1]))
			{
				break;
			}
		}
	}

	char tab = '\t';
	vector<string> production;

	//print item set

	cout<<endl<<tab<<"ITEM SET "<<atoi(argv[1])<<endl<<endl;

	itemSets>>word; //read initial 'Rule'
	while(itemSets>>word)
	{
		cout<<endl<<tab<<word;
		itemSets>>word;
		cout<<word<<" -> ";
	
		production.clear();		

		while((itemSets>>word)&&word!="dot")
		{
			production.push_back(word);
		}
		itemSets>>word>>word;

		vector<string>::iterator it;	
		int i;	

		for(i = 0 , it = production.begin();i<atoi(word.c_str()) && it!=production.end();i++,it++)
		{
		cout<<*it<<" ";
		}

		cout<<dot<<" ";

		for(;it!=production.end();it++)
		{
		cout<<*it<<" ";
		}

		itemSets>>word;
		cout<<endl<<tab<<tab<<"Lookahead: ";
		

		itemSets>>word;
		cout<<word;
		while((itemSets>>word)&&word!="Rule"&&word!="Item")
		{
			cout<<" "<<word;
		}
		cout<<endl;
		if(word=="Item")
		{
			break;
		}

	}
	
	

	cout<<endl;

}
