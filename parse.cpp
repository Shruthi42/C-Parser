#include"ReturnToken.h"

#ifndef PARSE
#define PARSE
#include"parse.h"
#endif


main(int argc, char * argv[])
{
	if(argc!=2)
	{
		cout<<"Usage: ./parse <filename>"<<endl<<endl;
		exit(0);
	}

	
	init(argv[1]);
	
	//Initialize ACTION and GOTOtables from file
	readTable();

	//Write the beggining line to the log	
	log<<"Beginning Parsing: "<<endl;

	s.push(make_pair("ACCEPT",0));

	int which;	

	//Accept tokens one by one
	while(token = yylex())
	{
		#ifdef DEBUG
		printf("%d\n",token);
		#endif
	
		fprintf(tokenOut,"%s\n",returnToken(token));

		/*	Until a SHIFT action is performed, perform actions withthe same token
			On SHIFT, accept the next token from lex
		*/

		while((which = performAction(returnToken(token)))!=shift);

		c = column;
		l = lines;
	}

		inputConsumed = 1;

		while((which = performAction("ENDSYMBOL"))!=shift);

		cout<<"End of input. Error detected."<<endl;	

}

//Perform action appropriate to the current token
int performAction(char * token)
{
	int state = s.top().second;
	ACTION::iterator i1 = act.begin();

	log<<endl<<"Current state: "<<state<<" input token "<<token<<endl;

		for(;i1!=act.end();i1++)
		{
			if((state==i1->first)&&(string(token)==(i1->second.first)))
			{
			if(i1->second.second.first==accept)
			{
				log<<"Accepted"<<endl;
				if(!err)				
					cout<<endl<<tab<<"No error detected."<<endl<<endl;
				else
					cout<<endl<<tab<<"Program parsed with no further errors detected"<<endl<<endl;
				exit(0);
			}			
			else if(i1->second.second.first==shift)
			{
				log<<"SHIFT on token "<<token<<" to state "<<i1->second.second.second.first<<endl;
				s.push(make_pair(token,i1->second.second.second.first));
				return shift;
			}
			else
			{
				log<<"REDUCE on token "<<token<<" by rule "<<i1->second.second.second.first<<endl;
			
				for(int i = 0;i<(i1->second.second.first);i++)
				{
					s.pop();
				}
					goto_(i1->second.second.second.second,s.top().second);	
				return reduce;
			}

			}
		}
		log<<"No action possible on this input"<<endl;
		errorHandle(state,token);

}


//goto_() performs the goto action for a state after reduce
void goto_(string token, int state)
{
	int prevstate = s.top().second;
	GOTO::iterator i1 = gt.begin();

		for(;i1!=gt.end();i1++)
		{
			if((prevstate==i1->first)&&(string(token)==(i1->second.first)))
			{
				log<<endl<<"GOTO from state "<<prevstate<<" on token "<<token<<" to state "<<i1->second.second<<endl;
				s.push(make_pair(token,i1->second.second));
			}
		}
	

}

//Error handling routine
void errorHandle(int st, string tok)
{
	err = 1;
	
	int p;

	//If phrase handling can take care of this error, then return
	if((p = phraseHandled(st,tok))==1)
	{
		return;
	}


	if(p==0)	//Print out error and find expected symbol to give hint to user if phrase handled has not done so
	{	
	printError(0);

	vector<string> expected;
	expected.clear();
	ACTION::iterator i = act.begin();
	for(;i!=act.end();i++)
	{
		if((i->first==s.top().second))
		{
		if(std::find(expected.begin(), expected.end(), i->second.first) != expected.end())
		{
		//already in expected
		}
		else
		{
		expected.push_back(i->second.first);
		}
		}
	}
	
	//If any symbols found, output as hint
	if(!expected.empty())
	{
	cout<<tab<<tab<<arrow<<" Expected: ";
		while(!expected.empty())
		{
			if((expected.back()==string(";"))
				||(expected.back()==string("}"))
				||(expected.back()==string("{"))
				||(expected.back()==string("identifier"))
				||(expected.back()==string("constant"))
				||(expected.back()==string("string literal"))
				||(expected.back()==string("typename"))
				||(expected.back()==string("else"))
				||(expected.back()==string("return"))
				||(expected.back()==string(")"))
				||(expected.back()==string("("))
				||(expected.back()==string(";"))
				||(expected.back()==string("TYPE"))
			)
			{
			cout<<"'"<<expected.back()<<"'"<<" or ";
			}
			expected.pop_back();
		}
		if(!inputConsumed)
			cout<<"\b\b\b"<<" before token "<<"'"<<returnToken(token)<<"'"<<endl;
		else
			cout<<"\b\b\b"<<" before end of program"<<endl;
	}
	}	//end of finding expected

	//Proceed with error recovery
	int state = s.top().second;
		
	int done = 0;


	log<<"current TOS is: "<<state<<endl;
	
	GOTO::iterator i1 = gt.begin();

	//Examine stack
	while(!s.empty())
	{	
		int state = s.top().second;
		
		//Scan GOTO table to see if state has goto on stateent or declaration
		for(i1 = gt.begin();i1!=gt.end();i1++)
		{

			if((state==i1->first)&&(string("statement")==(i1->second.first)))
			{
			
			log<<endl<<"Error Recovery: GOTO from state "<<state<<" on nonterminal "<<"statement"
				<<" to state "<<i1->second.second<<endl;

			s.push(make_pair("statement",i1->second.second));
			done = 1;
			break;
			}
			else if((state==i1->first)&&(string("declaration")==(i1->second.first)))
			{
			
			log<<endl<<"Error Recovery: GOTO from state "<<state<<" on nonterminal "<<"declaration"
				<<" to state "<<i1->second.second<<endl;

			s.push(make_pair("declaration",i1->second.second));
			done = 1;
			break;
			}
		}
		
		if(done==1) break;

		log<<"Error Recovery: Popping state "<<state<<endl;
		s.pop();
	}	

	//If stack empty, parsing cannot continue
	//Write an appropriate message to log and exit 
	if(s.empty())
	{
		cout<<endl<<"No further errors found"<<endl<<endl;
		log<<"End of stack reached while recovering from error"<<endl;
		exit(0);
	}

	log<<"Acceptable state found. Trying to resume parsing"<<endl;
	
	ACTION::iterator j;


	//Scan input stack for symbol which can follow the state found
	while(1)
	{
		for(j = act.begin();j!=act.end();j++)
		{
			if((j->first==s.top().second)&&(j->second.first==string(returnToken(token))))
				{
				log<<"Exiting Error recovery, found token "<<returnToken(token)<<endl;
				
					if(performAction(returnToken(token))!=shift)
					{
						return;
					}
					else if(token = yylex())
					{
						return;
					}
					else
					{
						inputConsumed = 1;
						while((performAction("ENDSYMBOL"))!=shift);
						cout<<"End of input. Error detected."<<endl;	
						exit(0);
					}
				}
		}

		
		if(token = yylex())
		{
			c = column;
			l = lines;
		}
		else
		{
		cout<<endl<<"No further errors found"<<endl<<endl;
		log<<"End of input reached after recovery. Could not parse after error"<<endl;
		exit(0);
		}
	}

}

/*
Phrase handling routine handles certain common errors
*/
int phraseHandled(int st,string tok)
{

	switch(st)
	{
	case 359:
		//missing parameter after , in paramenter list
		printError(0);
		recovery_action("TYPE","TYPE (ex. int, char, float, double, signed, unsigned)");
		return 1;
		break;

	case 446:
		//missing ; in for loop ex. for(-;-)
		printError(0);
		recovery_action(";","Another expression in for loop");
		return 1;
		break;

	case 1192:
		//missing ; after while(...) in do while
		printError(0);
		recovery_action(";","; after while()");
		return 1;
		break;

	case 277:
		//Missing expression in while()
		printError(0);
		recovery_Goto("expression",536,"expression");
		return 1;
		break;
	
	case 800:
		//Missing expression in do...while()
		printError(0);
		recovery_Goto("expression",1049,"expression");
		return 1;
		break;

	case 275:
		//Missing expression in if()
		printError(0);
		recovery_Goto("expression",534,"expression");
		return 1;
		break;

	case 69:
		//missing expression before identifier
		printError(0);
		recovery_Goto("primary_expression",97,"expression");
		return 1;
		break;

	case 253:
		//missing expression before ; (ex a = a+;)
		printError(0);
		recovery_Goto("primary_expression",440,"expression");
		return 1;
		break;

	case 797:
		//missing statement before else
		printError(0);
		recovery_Goto("statement",1025,"statement");
		return 1;
		break;

	default:
		if(returnToken(token)==string("DEFAULT"))
		{
		//default outside switch
		printError(-1);
		cout<<tab<<tab<<arrow<<" 'default' label not within a switch statement"<<endl;
		return -1;
		}
		if(returnToken(token)==string("CASE"))
		{
		//default outside switch
		printError(-1);
		cout<<tab<<tab<<arrow<<" 'case' label not within a switch statement"<<endl;
		return -1;
		}
		if(returnToken(token)==string("ELSE"))
		{
		//default outside switch
		printError(-1);
		cout<<tab<<tab<<arrow<<" 'else' without 'if'"<<endl;
		return -1;
		}
		return 0;
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

void recovery_Goto(char nonterminal[],int gotostate,char disp[])
{
	if(!inputConsumed)
		cout<<tab<<tab<<arrow<<" Expected: "<<(char)toupper(disp[0])<<disp+1<<" before token "<<"'"<<returnToken(token)<<"'"<<endl;
	else
		cout<<tab<<tab<<arrow<<" Expected: "<<(char)toupper(disp[0])<<disp+1<<" before end of program"<<endl;
	log<<"Error recovery inserted "<<nonterminal<<endl;
	s.push(make_pair(nonterminal,gotostate));
return;
}

void recovery_action(char inserttoken[],char disp[])
{
	if(!inputConsumed)
		cout<<tab<<tab<<arrow<<" Expected: "<<disp<<" before token "<<"'"<<returnToken(token)<<"'"<<endl;
	else
		cout<<tab<<tab<<arrow<<" Expected: "<<disp<<" before end of program"<<endl;
	log<<"Error recovery inserted "<<inserttoken<<endl;
	int temp = token;
	while(performAction(inserttoken)!=shift);
	token = temp;
return;
}

//Prints error message
void printError(int x)
{
	int l1,c1,curline1;

	if(x==0)
	{
		l1 =l;
		c1 = c;
	}
	else
	{
		l1 = lines;
		c1 = column;
	}

	cout<<endl<<tab<<warning<<" Error detected on line "<<l1<<" column "<<c1+1<<endl;

	for(int i = 0;i<l1;i++) fgets(curline,MAXLINESIZE,read);
	fseek(read,0,SEEK_SET);
	curline[strlen(curline)-1] = '\0';
	cout<<tab<<"In line "<<l1<<": "<<curline<<endl;

	
	cout<<tab;
	char temp[50] = "",ls[10]= "";
	sprintf(ls,"%d",l1);
	strcat(temp,"In line ");
	strcat(temp,ls);
	strcat(temp,": ");

	for(int i = 0;i<c1+strlen(temp);i++)
		cout<<" ";
	cout<<"^"<<endl;
}
