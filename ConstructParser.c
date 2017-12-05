#ifndef CONSTRUCTPARSER
#define CONSTRUCTPARSER
#include"ConstructParser.h"
#endif 

#define DEBUG20

#ifdef DEBUG20
#include"debug.h"
#endif

#ifdef DEBUG10
#include"debug.h"
#endif

#ifdef LIMITRESOURCES
#include <sys/resource.h>
#endif

#ifndef SORT
#define SORT
#include"Sort.h"
#endif


int removeTimes= 0;
FILE * states, * itemsets;


main(int argc, char * argv[])
{
	if(argc!=4)
	{
	printf("Usage: ./ConstructParser <input file> <token file> <nonterminals file>");
	exit(0);
	}
	

	#ifdef LIMITRESOURCES
	struct rlimit rl;
	rl.rlim_cur = 32000;
	rl.rlim_max = 32000;
	setrlimit(RLIMIT_DATA, &rl);
	setrlimit(RLIMIT_STACK, &rl);
	#endif


	#ifdef DEBUG17
	printf("Initializing token table,%d\n");
	#endif

	//Initialize token table from token file
	initTokenTable(argv[2]);

	#ifdef DEBUG17
	printf("Done initializing token table,%d\n",2*3);
	#endif

	//Initialize non-terminals table from Non Terminals file
	initNonterminalTable(argv[3]);

	//Open output file for parsing table
	FILE * parsingTable = fopen("Table","w");

	if(parsingTable==NULL)
	{
	printf("Error opening file. Aborting");
	exit(0);
	}

	//Output files for states and Item sets
	states = fopen("States","w");
	itemsets = fopen("ItemSets","w");

	//Read productions from input grammar file and store in a productionList
	productionList * prodList = readProductions(argv[1]);

	#ifdef DEBUG
	printf("Productions:\n");
	productionList* move = prodList;
		while(move!=NULL)
		{
		printf("%d. %s\n",move->p.number,move->p.rule);
		printf("\n");
		move = move->next;
		}
	#endif

	#ifdef DEBUG
	productionList* move = prodList;
		while(move!=NULL)
		{
		printf("Production Head: %s\n",move->p.head);
		printf("Production body: ");
		tokenList * body = move->p.beta;
		while(body!=NULL)
		{
			printf("%s ",body->token);
			body = body->next;
		}
		printf("\n");
		move = move->next;
		}
	#endif

	#ifdef DEBUG17
	printf("\n\nTokens: \n");
	displayTokenTable();

	printf("\n\nNonterminals: \n");
	displayNonterminalTable();
	#endif

	#ifdef DEBUG17
	testcontainsToken();
	printf("Finding first\n");
	testFIRST(prodList);
	printf("Done finding first\n");
	#endif

	#ifdef DEBUG18
	testsortTL();
	#endif	

	#ifdef DEBUG8
	testsameTokenList();
	#endif	

	#ifdef DEBUG20
	printf("Creating Itemsets...\n\n");
	#endif

	//Create Item sets and parsing table simultaneously
	itemSetList * isList = createItemSets(prodList,parsingTable);

	//Print Item sets to file
	printItemSetList(isList);

	//free memory
	freeProductions(prodList);
	
	#ifdef DEBUG20
	printf("\n\nDone creating itemsets.\nParse table has been constructed.\n");
	#endif

}

int itemnumber = 1;

void printItemSetList(itemSetList * list)
{
int i= 0;
while(list!=NULL)
{
printItemSet(list->i);
fprintf(itemsets,"\n\n");
list = list->next;
}
}


void printItemSet(itemSet i)
{
	fprintf(itemsets,"\nItem %d\n",i.number);
	processedProductionList *move = i.pl;
	while(move!=NULL)
	{
		int len = strlen(move->p.p.rule);
		fprintf(itemsets,"\tRule %d. %.*s\n\t\tdot at %d\n\t\tLookahead:  ",move->p.p.number,len-1,move->p.p.rule,move->p.point);
		tokenList * l = move->p.lookahead;
		while(l!=NULL)
		{
		fprintf(itemsets,"%s ",l->token);
		l = l->next;	
		}

		#ifdef DEBUG
		printf("Held rule: %s ",move->p.p.head);
		l = move->p.p.beta;
		while(l!=NULL)
		{
			printf("%s ",l->token);
			l = l->next;	
		}
		#endif

		fprintf(itemsets,"\n");
		move = move->next;
	}

fprintf(itemsets,"\n");
}

itemSetList * createItemSets(productionList *grammar, FILE * fp)
{
	/*Augment grammar
	This involves adding a special start state AUGMENTEDSTART
	The production AUGMENTEDSTART -> start is present in the first item
	*/

	itemSet * I0 = (itemSet*)malloc(sizeof(itemSet));
	I0->pl = (processedProductionList*)malloc(sizeof(processedProductionList));
	I0->number = 0;
	strcpymine(I0->pl->p.p.head,"AUGMENTEDSTART");
	I0->pl->p.p.beta = (tokenList*)malloc(SZ);
	strcpymine(I0->pl->p.p.beta->token,"start"); 
	I0->pl->p.p.beta->next=NULL;

	strcpyrule(I0->pl->p.p.rule,"AUGMENTEDSTART start\n");
	I0->pl->p.p.length = strlen("AUGMENTEDSTART start\n");
	I0->pl->p.p.number = 0;
	
	I0->pl->p.lookahead = (tokenList*)malloc(SZ);
	strcpymine(I0->pl->p.lookahead->token,"ENDSYMBOL");	
	I0->pl->p.lookahead->next =NULL;
	I0->pl->p.point = 0;
	
	productionList * augGrammar = (productionList*)malloc(sizeof(productionList));
	augGrammar->p = I0->pl->p.p;
	augGrammar->next = grammar;

	//Produce item sets

	//Take closure of initial state
	closure(I0,augGrammar);

	//Add this initial state to thelist of item sets 
	itemSetList * I = (itemSetList*)malloc(sizeof(itemSetList));
	I->i = *I0;
	I->next = NULL;
	

	#ifdef DEBUG6
	printf("Augmenting done.\n");
	#endif

	#ifdef DEBUG
	printf("Augmented grammar is: \n");
	productionList* move = augGrammar;
	while(move!=NULL)
	{
	printf("Production Head: %s\n",move->p.head);
	move = move->next;
	}
	#endif

	#ifdef DEBUG17
	printf("\n Adding states via addStates()\n");
	#endif

	//Add item sets to the list
	addStates(I, augGrammar, fp);	

	return I;	

}

/*
Function addStates finds a transition from the current state on every terminal and nonterminal
*/
void addStates(itemSetList *I, productionList * augGrammar, FILE * fp)
{
	itemSetList * cur = I;

	#ifdef DEBUG
	printf("Entered addStates()\n");
	#endif

	while(cur!=NULL)
	{

		TokenTable * v = &table1;

		//go through nonterminal table to see if a transition exists on any nonterminal
		while(v->token!=NULL)
		{
			#ifdef DEBUG17
			printf("addStates() calling goTo for ItemSet number %d and token %s\n", cur->i.number,v->token);
			#endif
		
			goTo(cur,v,I,augGrammar,fp);


			#ifdef DEBUG17
			printf("addStates() done calling goTo for ItemSet number %d and nonterminal %s\n", cur->i.number,v->token);
			#endif			

			v = v->next;
		}


		TokenTable * t = &table;

		//go through token table to see if a transition exists on any token
		while(t->token!=NULL)
		{
			#ifdef DEBUG17
			printf("addStates() calling goTo for ItemSet number %d and token %s\n", cur->i.number,t->token);
			#endif
		
			goTo(cur,t,I,augGrammar,fp);


			#ifdef DEBUG17
			printf("addStates() done calling goTo for ItemSet number %d and token %s\n", cur->i.number,t->token);
			#endif			

			t = t->next;
		}


	cur =cur->next;
	}
}	

/*
Function finds the closure of an item set
It appends any new productions to the productions which already exist in the set
*/
void closure(itemSet * I0,productionList * augGrammar)
{

#ifdef DEBUG17
printf("in Closure Called for itemset %d\n", I0->number);
#endif

#ifdef DEBUG
printf("Augmented grammar used by closure is: \n");
productionList* move = augGrammar;
while(move!=NULL)
{
	printf("Production Head: %s\n",move->p.head);
	printf("Production body: ");
	tokenList * body = move->p.beta;
	while(body!=NULL)
	{
		printf("%s ",body->token);
		body = body->next;
	}
	printf("\n");

	move = move->next;
}	
#endif


processedProductionList * cur = I0->pl;
char head[MAXTOKENSIZE];
char next[MAXTOKENSIZE];
tokenList * beta = NULL;

while(cur!=NULL)
{
cont: 	;
	int i;

	#ifdef DEBUG13
	printf("Before remove empty\n");
	#endif

	if(cur==NULL||cur->p.p.beta==NULL)
	{
		#ifdef DEBUG17
		printf("At end\n");
		#endif
		return;
	}
	#ifdef DEBUG13
	printf("removeempty 2 called in closure()\n");
	#endif
	removeTimes = 0;
	removeEmpty(&(cur->p.p.beta));
	#ifdef DEBUG13
	printf("After remove empty\n");
	#endif
	tokenList *B = cur->p.p.beta;


	#ifdef DEBUG18
	if(itemnumber==5)
	{
	printf("Current production: %s\n",cur->p.p.rule);
	}
	#endif

	#ifdef DEBUG18
	if(itemnumber==5)
	{
	printf("Production Head: %s\n",cur->p.p.head);
	printf("Production body: ");
	tokenList * body = cur->p.p.beta;
	while(body!=NULL)
	{
		printf("%s ,",body->token);
		body = body->next;
	}
	printf("\n");
	printf("Lookahead: ");
	body = cur->p.lookahead;
	while(body!=NULL)
	{
		printf("%s ",body->token);
		body = body->next;
	}
	printf("\npoint: %d\n",cur->p.point);
	}
	#endif

	for(i = 0;i<cur->p.point;i++) B = B->next;

	#ifdef DEBUG17
	if(B!=NULL)
	printf("Current symbol before dot is: %s\n",B->token);
	#endif
	
	//if dot is at the end of production, skip this production	
	if(B==NULL) 
	{
	#ifdef DEBUG18
	printf("B is NULL\n");
	#endif
	cur = cur->next;

	#ifdef DEBUG18
	printf("Going to next cur\n");
	#endif
	goto cont;
	}

	else
	{
		//If there exists a symbol after the dot, we process the production further 
		strcpymine(next,B->token);
		productionList * search = augGrammar;
		while(search!=NULL)
		{

			#ifdef DEBUG17
			printf("Closure() is comparing %s with %s\n", search->p.head,next);
			#endif

			if(!strcmp(search->p.head,next))
			{
				processedProductionList* temp1 = (processedProductionList*)malloc(sizeof(processedProductionList));

				temp1->next = NULL;
				temp1->p.point = 0;
				strcpymine(temp1->p.p.head,search->p.head);
				strcpyrule(temp1->p.p.rule,search->p.rule);
				temp1->p.p.length = search->p.length;
				temp1->p.p.number = search->p.number;
				temp1->p.p.beta = NULL;
				tokenList *copyTo, *Beta;
				copyTo = NULL;
				Beta = NULL;

				#ifdef DEBUG2
				printf("Original production body: ");
				tokenList * body1 = search->p.beta;

				while(body1!=NULL)
				{
					printf("%s ",body1->token);
					body1 = body1->next;
				}
				printf("\n");
				#endif	

				tokenList * cf = search->p.beta;
	
				while(cf!=NULL)
				{

					#ifdef DEBUG2
					printf("\tcopyfrom in closure is %s\n", cf->token);
					#endif

					copyTo = (tokenList*)malloc(SZ);

					strcpymine(copyTo->token,cf->token);
					copyTo->next = NULL;
					if(Beta==NULL)
						Beta = copyTo; 
					else
					{
					tokenList* end = Beta;
					while (end->next!=NULL) end = end->next;
					end->next = copyTo;
						
					}


					cf = cf->next;
	
				}
				temp1->p.p.beta = Beta;
				

				char t[MAXTOKENSIZE];
				if(Beta!=NULL)
				{
				strcpymine(t,Beta->token);

				temp1->p.p.beta=(tokenList *)malloc(SZ);
				if(temp1->p.p.beta==NULL) {printf("memory allocation error");exit(0);}

			
				int i = 0;
				for(;t[i]!='\0';i++) {
					(temp1->p.p.beta)->token[i] = t[i];

					}
				temp1->p.p.beta->token[i] = '\0';
				temp1->p.p.beta->next = Beta->next;
				}
				else
					temp1->p.p.beta = NULL;
				
		
				#ifdef DEBUG13
				printf("removeempty 1 called in closure()\n");
				#endif
				removeTimes = 0;
				removeEmpty(&(temp1->p.p.beta));			

				#ifdef DEBUG2
				printf("Copied production body: ");
				tokenList * body = temp1->p.p.beta;
				while(body!=NULL)
				{
					printf("%s ",body->token);
					body = body->next;
				}
				printf("\n");
				#endif	

				//add lookahead

				#ifdef DEBUG17
				printf("Adding lookahead in Closure()\n");
				#endif

				temp1->p.lookahead = NULL;
				tokenList * lookahead = cur->p.lookahead, * beta = NULL;	
				if(B!=NULL && B->next!=NULL)
				{
					//Find lookahead
					temp1->p.lookahead = NULL;
					#ifdef DEBUG17
					printf("Finding new lookahead in Closure()\n");
					int i;
					#endif

					beta = B->next;
					

					#ifdef DEBUG10
					if(B->next==NULL)
					{
						printf("No beta! Fatal error\n");
						exit(0);
					}
					#endif

					tokenList *l = lookahead;
					
					#ifdef DEBUG10
					if(l==NULL)
					{
						printf("No lookahead! Fatal error\n");
						exit(0);
					}
					#endif

					tokenList * last = B->next;
					while(last->next!=NULL) last = last->next;
 
					tokenList * add = (tokenList *)malloc(SZ);
					add->next = NULL;

					//shallow copy
					tokenList * tofind = B->next, *foundfirst = NULL;

						#ifdef DEBUG7
						printf("beta for rule %.*s point %d,%d is: ",(int)(strlen(cur->p.p.rule)-1),cur->p.p.rule,i,cur->p.point);
						tokenList * a = cur->p.p.beta;
						while(a!=NULL)
						{
							printf("%s ,",a->token);
							a = a->next;
						}
						printf("\n");
						#endif

					#ifdef DEBUG10
					printf("Reached Here1\n");
					#endif

					while(l!=NULL)
					{

						#ifdef DEBUG2
						printf("%d\n",i++);
						#endif
						
						strcpymine(add->token,l->token);
						last->next = add;
						

					#ifdef DEBUG10
					printf("Reached Herex\n");
					#endif
						
						foundfirst = FIRST(tofind,augGrammar);
						
							
						#ifdef DEBUG17
						printf("First for lookahead %s is: ",tofind->token);
						tokenList * w = foundfirst;
						while(w!=NULL)
						{
							printf("%s ",w->token);
							w = w->next;
						}
						printf("\n");
						#endif


						if(temp1->p.lookahead==NULL&& foundfirst!=NULL)
							temp1->p.lookahead = foundfirst;
						else if (foundfirst!=NULL)
						{
							tokenList * end = temp1->p.lookahead;						
							while(end->next!=NULL) end = end->next;
							end->next = foundfirst;
						}
						l = l->next;
					}	

					last->next = NULL;

					#ifdef DEBUG10
					printf("Reached Here2\n");
					#endif

					removeDuplicates(temp1->p.lookahead);
					
					#ifdef DEBUG16
					printf("New lookahead in closure() is: ");
					tokenList * write = temp1->p.lookahead;
					int kk =0;
					while(write!=NULL)
					{
						printf("%s ",write->token);
						write = write->next;
						kk++;
					}
					printf("%d\n",kk);
					#endif					
			
				}
				else if(B->next==NULL)
				{
					//copy old lookahead

					#ifdef DEBUG17
					printf("Using old lookahead in Closure()\n");
					#endif

					tokenList * Cf = cur->p.lookahead,*Copyto = NULL;	
					temp1->p.lookahead = NULL;			
					while(Cf!=NULL)
					{
						#ifdef DEBUG2
						printf("Copied token is: %s\n",Cf->token);
						#endif
						tokenList * Copyto = (tokenList*)malloc(SZ);
						strcpymine(Copyto->token,Cf->token);
						Copyto->next = temp1->p.lookahead;
						temp1->p.lookahead = Copyto;
						Cf = Cf->next;
					}


					#ifdef DEBUG7
					printf("Copied lookahead for closure is: ");
					tokenList * write = temp1->p.lookahead;
					while(write!=NULL)
					{
						printf("%s ",write->token);
						write = write->next;
					}
					printf("\n\n");
					#endif
				}			


				#ifdef DEBUG17
				printf("Done adding lookahead in Closure()\n");
				#endif
				#ifdef DEBUG19
				printf("Checking to see if production %s point exists in Closure()\n", temp1->p.point,temp1->p.p.rule);
				#endif

				

				if(notInClosureList(temp1,I0->pl))  
				{

					#ifdef DEBUG13
					printf("New production added to closure.\n");
					#endif
					//Adding temp to prodution list
					processedProductionList * lastItem = cur;
					while(lastItem->next!=NULL) lastItem = lastItem->next;
					lastItem->next = temp1;
				}

				else
				{
					#ifdef DEBUG18
					printf("Same production found in closure.\n");
					#endif
				}	

			} //end of if(!strcmp(search->p.head,next))
			else ;

		search = search->next;

		} //end of while

	} //end of else

cur = cur->next;

}

#ifdef DEBUG13
printf("Sort calledin closure()\n");
#endif
I0->pl = sortPL(&(I0->pl));
#ifdef DEBUG13
printf("Closure Done\n");
#endif

#ifdef DEBUG11
printItemSet(*I0);
printf("\n\n");
#endif
}



//Checks to see if an item set already exists in item set list
int same(itemSetList * new, itemSetList * is)
{

	#ifdef DEBUG
	printf("in same()\n");
	#endif


	#ifdef DEBUG8
	printf("Beginning of same: \n");
	printItemSet(new->i);
	printf("");
	#endif

	itemSetList * temp = is;
	int flag = 0;

	processedProductionList * n = new->i.pl;
	
	#ifdef DEBUG13
	printf("removeempty() 4 called in same()\n");
	#endif	
	removeTimes = 0;
	removeEmpty(&(n->p.lookahead));

	new->i.pl = sortPL(&(new->i.pl));


	#ifdef DEBUG19
	int c= 0;
	processedProductionList *f = n;
	while(f!=NULL){printf("%s %ld\t",f->p.p.rule,f);c++;f = f->next;}
	printf("Num at first : %d\n",c);
	#endif

	
	while(temp!=NULL)
	{

		#ifdef DEBUG
		printf("same checking itemset %d with itemset %d\n",new->i.number,temp->i.number);
		#endif

		processedProductionList * t = temp->i.pl;
		n = new->i.pl;
		#ifdef DEBUG13
		printf("removeempty called in same()\n");
		#endif
		removeTimes = 0;
		removeEmpty(&(t->p.lookahead));



		#ifdef DEBUG8
		if(new->i.number==6){
		printf("End of same: \n");
		printItemSet(new->i);
		printf("");}
		#endif

		t = sortPL(&t);

		#ifdef DEBUG8
		if(new->i.number==6){
		printf("Compared with: \n");
		printItemSet(temp->i);
		printf("");}
		#endif



		flag = 0;
		while(t!=NULL&&n!=NULL)
		{

			if((!strcmp(t->p.p.rule,n->p.p.rule))&&(t->p.point==n->p.point)&&sameTokenList(t->p.lookahead,n->p.lookahead));

			else flag = 1;
			t = t->next;
			n = n->next;
		}
		if(t==NULL&&n==NULL&&flag==0) {new->i.number = temp->i.number;return 1;}
		
		temp = temp->next;
	}

	#ifdef DEBUG
	printf("left same()\n");
	#endif


	return 0;
}

int sameTokenList(tokenList *s, tokenList* t)
{
if(s==t) return 1;

t = sortTL(&t);
#ifdef DEBUG
tokenList * first;
printf("Sorted list in same: ");

	first = t;	
	while(first!=NULL)
	{
		printf("%s,\t",first->token);
		first = first->next;
	}
	
	printf("\n");
#endif
s = sortTL(&s);

#ifdef DEBUG
printf("Sorted list in same: ");

	first = s;	
	while(first!=NULL)
	{
		printf("%s,\t",first->token);
		first = first->next;
	}
	
	printf("\n");
#endif

tokenList * a = s, *b = t;

while(a!=NULL&&b!=NULL)
{
#ifdef DEBUG
printf("a: %s b: %s\n",a->token, b->token);
#endif

if(strcmp(a->token,b->token)) return 0;
a = a->next;
b = b->next;
}
if(a==NULL && b== NULL) return 1;
else return 0;
}

/*
Function calculates the transition from an item set on an a given input symbol.
If this results in a new item set, it this item set is added to the item set list 
*/
void goTo(itemSetList* cur, TokenTable* t,itemSetList* I,productionList * augGrammar, FILE * fp)
{

	#ifdef DEBUG6
	printf("goTo() entered\n");
	#endif



	itemSet processing = cur->i;
	itemSetList *new = (itemSetList*)malloc(sizeof(itemSetList));
	new->next = NULL;
	new->i.pl = NULL;
	char token[MAXTOKENSIZE];



		#ifdef DEBUG18
		if(itemnumber==6){		
		printf("State list is: ");
		printItemSetList(I);
		}
		#endif


	strcpymine(token,t->token);
	
	itemSetList* temp = I;

	processedProductionList * prodList = processing.pl;

	//Go through production list to see if a transition can exist on the given symbol
	while(prodList!=NULL)
	{
		processedProduction p = prodList->p;

		#ifdef DEBUG16
		printf("\tExamining production %.*s, dot at %d in goTo() for token %s in state %d\n",(int)(strlen(p.p.rule)-1),p.p.rule,p.point, t->token,cur->i.number);
		#endif
	
		tokenList * beta = p.p.beta;
		int i;
		for(i = 0;i<p.point&&beta!=NULL;i++)
			beta = beta->next;

		//If dot at end of production, skip this production. Else process this production further
		if(beta!=NULL)
		{	

	
			#ifdef DEBUG17
			printf("Dot is before token %s\n",beta->token);
			#endif
	
			if(!strcmp(beta->token,token))
			{
				

			#ifdef DEBUG18
			printf("\tExamining production %.*s, dot at %d in goTo() for token %s in state %d\n",(int)(strlen	(p.p.rule)-1),p.p.rule,p.point, t->token,cur->i.number);
			#endif

				processedProductionList * shifted = (processedProductionList*)malloc(sizeof(processedProductionList));
				shifted->p.point = p.point+1;
				shifted->p.lookahead = NULL;		
				//strcpymine(shifted->p.lookahead,p.lookahead);

				#ifdef DEBUG13
				printf("RemoveEmpty() 1 called in goto()\n");				
				#endif
				removeTimes = 0;
				removeEmpty(&(p.lookahead));
				tokenList * Cf1 = p.lookahead;					
				while(Cf1!=NULL)
				{
					tokenList * Copyto = (tokenList*)malloc(SZ);

					strcpymine(Copyto->token,Cf1->token);
					Copyto->next = shifted->p.lookahead;
					shifted->p.lookahead = Copyto;
					Cf1 = Cf1->next;
				}

				#ifdef DEBUG18
					printf("Old lookahead is: ");
					tokenList * write = p.lookahead;
					while(write!=NULL)
					{
						printf("%s ",write->token);
						write = write->next;
					}
					printf("\n");

					printf("Copied lookahead is1: ");
					write = shifted->p.lookahead;
					while(write!=NULL)
					{
						printf("%s ",write->token);
						write = write->next;
					}
					
					printf("\n");				
				#endif
				

				//copy production
				
				//deep copy of p.p to shifted->p.p
				strcpyrule(shifted->p.p.rule,p.p.rule);
				strcpymine(shifted->p.p.head,p.p.head);				
				shifted->p.p.number = p.p.number;
				shifted->p.p.length = p.p.length;
				shifted->p.p.beta = NULL;

				tokenList * now = p.p.beta;
				
				while(now!=NULL)
				{	
					tokenList * t1 = (tokenList *)malloc(SZ);
					strcpymine(t1->token,now->token);
					t1->next = NULL;
					now = now->next;

					tokenList * last = shifted->p.p.beta;
					if(last==NULL)
					{
						shifted->p.p.beta = t1;
					}
					else
					{
						while(last->next!=NULL) last = last->next;
						last->next = t1;
					}
				}

					#ifdef DEBUG7
					printf("Old beta is: ");
					write = p.p.beta;
					while(write!=NULL)
					{
						printf("%s ,",write->token);
						write = write->next;
					}
					printf("\n");

					printf("Copied beta is: ");
					write = shifted->p.p.beta;
					while(write!=NULL)
					{
						printf("%s ,",write->token);
						write = write->next;
					}
					printf("\n");				
				#endif
				
				#ifdef DEBUG11
				printf("Removeempty() called in Goto()\n");
				#endif
				removeTimes = 0;
				removeEmpty(&(shifted->p.p.beta));
			
				//add at end of new's production list-->this appends shifted
				shifted->next = new->i.pl;
				new->i.pl = shifted;



				#ifdef DEBUG7

					printf("Copied lookahead is1: ");
					write = new->i.pl->p.lookahead;
					while(write!=NULL)
					{
						printf("%s ",write->token);
						write = write->next;
					}
					
					printf("\n");				
				#endif
				

			}
			else 
			{
			#ifdef DEBUG2
			printf("No dot found before token %s\n",token);
			#endif
			}

		}
		else 
		{
		#ifdef DEBUG2
		printf("No dot found before token as fully processed %s\n",beta->token);
		#endif
		}
	prodList = prodList->next;
	}
	

	#ifdef DEBUG13
	printf("GOTO calling closure for itemset %d\n",new->i.number);
	#endif


#ifdef DEBUG18
if(itemnumber==5)
{
#define DEBUG19
}
else
{
#undef DEBUG19
}
#endif


			#ifdef DEBUG18
		if(itemnumber==5){
		printf("new state before closure %d\n", new->i.number);
		printf("State is: ");
		printItemSet(new->i);}
		#endif

	//Find closure of this item set
	closure(&(new->i),augGrammar);

			#ifdef DEBUG18
		if(itemnumber==5){
		printf("new state after closure %d\n", new->i.number);
		printf("State is: ");
		printItemSet(new->i);}
		#endif

	#ifdef DEBUG7
	printf("Goto found: \n");
	printItemSet(new->i);
	printf("\n\n");
	#endif


	if(new->i.pl==NULL) 
	{
	#ifdef DEBUG6
	printf("No transition on token\n");
	#endif
	return;
	}

	new->i.number = itemnumber++;
	
	int s= same(new,I);
		#ifdef DEBUG18
		if(new->i.number==5){
		printf("new state after same() %d\n", new->i.number);
		printf("State is: ");
		printItemSet(new->i);}
		#endif

	//If item set already exists, do not append to list.
	//Add appropriate SHIFT and GOTO rules for this state
	if(s)
	{
	itemnumber--;
	#ifdef DEBUG8
	printf("State already exists\n");
	#endif

	if(isToken(token))
	fprintf(fp,"SHIFT %d %s %d\n", cur->i.number, token ,new->i.number);
	else if(isNonterminal(token))
	fprintf(fp,"GOTO %d %s %d\n", cur->i.number, token, new->i.number);	
	}

	//If item set does not already exist, append to item set list.
	//Add appropriate SHIFT, REDUCE and GOTO rules for this state
	else
	{
	#ifdef DEBUG7
	printf("Goto found: \n");
	printItemSet(new->i);
	printf("\n\n");
	#endif
		
	
		#ifdef DEBUG20
		printf("\nAdding state %d\n", new->i.number);
		#endif

		#ifdef DEBUG18
		if(new->i.number==5){		
		printf("State is: ");
		printItemSet(new->i);
		}
		#endif
	

		fprintf(states,"%d\n",new->i.number);

		itemSetList * temp21 = I;
		while(temp21->next!=NULL)temp21 = temp21->next;
		temp21->next= new;
		new->next = NULL;

	
		#ifdef DEBUG18
		printf("\nnew state is %d\n", new->i.number);
		#endif

		if(isToken(token))
		fprintf(fp,"SHIFT %d %s %d\n", cur->i.number, token, new->i.number);
		else if(isNonterminal(token))
		fprintf(fp,"GOTO %d %s %d\n", cur->i.number,token, new->i.number);
		addReduce(new->i,fp);



	#ifdef DEBUGNONE
	if(new->i.number>9)
	exit(0);
	#endif

	#ifdef DEBUG8
	printf("Goto found: \n");
	printItemSet(new->i);
	printf("");
	#endif

	}



	#ifdef DEBUG6
	printf("goTo() exited\n");
	#endif
}

void findFIRST(tokenList * alpha,tokenList * * first, productionList * prodList, tokenList * * visited)
{
	if(alpha==NULL)
		{
		#ifdef DEBUG10
		printf("alpha is NULL, exiting\n");
		#endif
		return;
		}

	tokenList * cur = alpha;

	//add cur to visited. if already visted, return null
	if(containsToken(cur,*visited))
	{
		#ifdef DEBUG10
		printf("Finding FIRST(%s),recursion occurs\n",cur->token);
		#endif
		#ifdef DEBUG10 
		printf("foundFirst exited\n");
		#endif
		return;
	}

	else
	{
		#ifdef DEBUG10
		printf("Finding FIRST(%s),no recursion\n",cur->token);
		#endif

		if(cur==NULL) 
		{
		(*first) = NULL;
		
		#ifdef DEBUG10 
		printf("foundFirst1 exited\n");
		#endif
		return;
		}

	
		#ifdef DEBUG10 
		printf("yay!\n");
		#endif


		tokenList * add = (tokenList *)malloc(SZ);

		
		if(add==NULL)
		{
		#ifdef DEBUG10 
		printf("Memory Alloc error\n");
		exit(0);			
		#endif
		}

	
		#ifdef DEBUG10 
		printf("I Got here!\n");
		#endif

		strcpymine(add->token,cur->token);
		add->next = *visited;
		*visited = add;

		
		#ifdef DEBUG10 
		printf("Got here!\n");
		#endif

	while(cur!=NULL)
	{	
		#ifdef DEBUG10
		printf("isToken(%s) for token '%s' is: %d\n",cur->token,cur->token,isToken(cur->token));
		#endif

		if(isToken(cur->token))
		{


			#ifdef DEBUG10 
			printf("Got here333\n");			
			#endif

			tokenList *temp= (tokenList*)malloc(SZ);
			
			if(temp==NULL)
			{
			#ifdef DEBUG10 
			printf("Memory Alloc error\n");
			exit(0);			
			#endif
			}

			#ifdef DEBUG10 
			printf("Got here444\n");			
			#endif

			#ifdef DEBUG10 
			printf("%s\n",cur->token);			
			#endif

			strcpymine(temp->token,cur->token);
			temp->next = NULL;
			if(*first==NULL)
			{

			#ifdef DEBUG10 
			printf("Got here222\n");
			#endif

			*first = temp;

			}
			else
			{
		
			#ifdef DEBUG10 
			printf("Got here111\n");
			#endif
		
			temp->next = *first;
			*first = temp;
			}

			#ifdef DEBUG10 
			printf("%s is in FIRST\n",cur->token);
			#endif
			
			removeDuplicates(*first);

			#ifdef DEBUG10
			printf("Removed duplicates\n");
			#endif
			
			#ifdef DEBUG10 
			printf("foundFirst exited\n");
			#endif

			return;
		}
		else
		{
			int containsEps= 0;
			productionList* move = prodList;
			while(move!=NULL)
			{
			#ifdef DEBUG 
			printf("Current token: %s Production Head: %s\n",cur->token,move->p.head);
			#endif
			if(!strcmp(move->p.head,cur->token))
			{
				#ifdef DEBUG 
				printf("Match found for production head\n");
				#endif
				tokenList * firstAlpha = NULL;
				tokenList * beta = move->p.beta;
				
				findFIRST(beta,&firstAlpha,prodList,visited);
				if(!containsEpsilon(firstAlpha))
				{
					containsEps = 0;
					//copy firstAlpha to first
					if(firstAlpha==NULL)
						;
					else
					{
						tokenList *temp= firstAlpha;
						while(temp->next!=NULL)
						temp = temp->next;
						temp->next = *first;
						*first = firstAlpha;
					}
					
				}
				else
				{
				containsEps = 1;
				//grammar does not contain EPSILON so ignore
				}
			}
			else; //do nothing
			move = move->next;
			}

		removeDuplicates(*first);

		#ifdef DEBUG 
		printf("Removed duplicates\n");
		#endif
		
		if(!containsEps)
		{
			#ifdef DEBUG10 
			printf("foundFirst2 exited\n");
			#endif
			return;
		}
		else
		{
			cur = cur->next;
		}
		}
	}//end of while()
	#ifdef DEBUG10 
	printf("foundFirst3 exited\n");
	#endif
}
}

int containsToken(tokenList* tok, tokenList* first)
{
	tokenList *temp= first;
	while(temp!=NULL)
	{
		if(!strcmp(temp->token,tok->token))
			return 1;
		temp = temp->next;
	}
return 0;
}

int containsEpsilon(tokenList* first)
{
	tokenList *temp= first;
	while(temp!=NULL)
	{
		if(!strcmp(temp->token,"EPSILON"))
			return 1;
		temp = temp->next;
	}
return 0;
}

tokenList * FIRST(tokenList * alpha,productionList * prodList)
{
	#ifdef DEBUG10
	printf("Finding FIRST for token %s\n\n",alpha->token);
	#endif

	tokenList * first = NULL;
	tokenList * visited = NULL;	
	findFIRST(alpha,&first,prodList,&visited);
	return first;

	#ifdef DEBUG10
	printf("done finding FIRST for token %s\n\n",alpha->token);
	#endif

}

productionList * readProductions(char * inputFile)
{
	FILE * input = fopen(inputFile,"r");
	char * rule = (char*) malloc(MAXRULESIZE);
	int number = 1;

	productionList * startlist = NULL, * node = startlist, * temp;

	while(fgets(rule,MAXRULESIZE,input))
	{
		if(strcmp(rule,"\n"))
		{
			#ifdef DEBUG
			printf("%s",rule);
			#endif

			temp = (productionList *)malloc(sizeof(productionList));

			if(temp==NULL)
			{
				printf("Memory error.");
				exit(0);
			}

			strcpyrule(temp->p.rule,rule);
			temp->p.length = strlen(rule);
			temp->p.number = number++;
			int pos = 0,i = 0,head = 1;
			char tempToken[MAXTOKENSIZE];
			tokenList * tempTokenList = NULL,* start = NULL;

			while(pos<=temp->p.length)
			{
				if(rule[pos]!=' '&&rule[pos]!='\n'&&rule[pos]!='\0')
				tempToken[i] = rule[pos];
				else
				{
					tempToken[i] = '\0';
					if(head)
					{
						strcpymine(temp->p.head,tempToken);
						tempToken[0] = '\0';
						i = -1;
						head = 0;
					}
					else
					{
						tempTokenList = (tokenList*)malloc(SZ);
						strcpymine(tempTokenList->token,tempToken);
						tempTokenList->next = NULL;

						if(start==NULL)
							start = tempTokenList;
						else
						{
							tokenList *cur = start;
							while(cur->next!=NULL) cur = cur->next;
							cur->next = tempTokenList;
						}
						tempToken[0] = '\0';
						i = -1;	
					}
				}
				i++; pos++;
			}	
			temp->p.beta = start;
			temp->next= NULL;
			
			#ifdef DEBUG
			printf("%s",temp->p.head);
			tokenList * print = temp->p.beta;
			while(print!=NULL)
			{
				printf("\t%s",print->token);
				print = print->next;
			}
			printf("\n\n");
			#endif
		
			if(node==NULL)
			node= temp;
			else
			{
			temp->next = node;
			node= temp;
			}
		}

		#ifdef DEBUG
		else printf("Empty rule\n");
		#endif
	}

	startlist = node;
	return startlist;

}

void freeProductions(productionList *prodList)
{
	productionList * temp;
	while(prodList != NULL)
	{
	temp = prodList->next;
	free (prodList);
	prodList = temp;
	}
}


void removeEmpty(tokenList ** list)
{
#ifdef DEBUG13
printf("Removeempty() entered %d\n",removeTimes);
removeTimes++;
#endif

	if((*list)==NULL) return;
	if((*list)->next==NULL)
	{
		if(!strcmp((*list)->token,""))
		{
		(*list) = NULL;
		#ifdef DEBUG13
		printf("Removeempty() exited\n");
		#endif
		return;
		}
		else
		{
		#ifdef DEBUG13
		printf("Removeempty() exited\n");
		#endif
		return;
		}
	}


	tokenList *cur,*forward,*prev;
	cur = (*list);
	while(cur!=NULL&&cur->next!=NULL)
	{
		forward = cur->next;
		prev= cur;
		while(forward!=NULL)
		{
			if(!strcmp(forward->token,""))
			{
				prev->next = forward->next;
				free(forward);
				forward = prev->next;
			}
		
			else
			{
				prev = prev->next;
				forward = forward->next;
			}
		}
	cur = cur->next;	
	} 

	if(!strcmp((*list)->token,""))
	{
	(*list) = (*list)->next;
	#ifdef DEBUG13
	printf("Removeempty() exited\n");
	#endif
	return;
	}
	else
	{
	#ifdef DEBUG13
	printf("Removeempty() exited\n");
	#endif
	return;
	}

#ifdef DEBUG13
printf("Removeempty() exited\n");
#endif
}

void removeDuplicates(tokenList * list)
{
	tokenList *cur,*forward,*prev;
	cur = list;
	while(cur!=NULL)
	{
		forward = cur->next;
		prev= cur;
		while(forward!=NULL)
		{
			if(!strcmp(forward->token,cur->token))
			{
				prev->next = forward->next;
				free(forward);
				forward = prev->next;
			}
		
			else
			{
				prev = prev->next;
				forward = forward->next;
			}
		}
	cur = cur->next;	
	} 

}

void addReduce(itemSet i, FILE * fp)
{
	#ifdef DEBUG2
	printf("addReduce() called\n");
	#endif

	processedProductionList * ppl = i.pl;
	while(ppl!=NULL)
	{
	

		#ifdef DEBUG2
		printf("rule: %s point at %d\n",ppl->p.p.rule,ppl->p.point);
		#endif
		tokenList *B = ppl->p.p.beta;
		int j;

		for(j = 0;j<ppl->p.point;j++) B = B->next;
			
		if(B==NULL)
		{
			tokenList * lookahead = ppl->p.lookahead;
			while(lookahead!=NULL)
			{

			#ifdef DEBUG2
			printf("Adding reduce rule %d %s %d %d\n", i.number,lookahead->token,ppl->p.p.number,j);
			#endif


			if(!strcmp(ppl->p.p.head,"AUGMENTEDSTART")&&!strcmp(lookahead->token,"ENDSYMBOL"))
			{
			fprintf(fp,"ACCEPT %d %s\n", i.number,lookahead->token);	
			lookahead = lookahead->next;
			}
			else
			{
			fprintf(fp,"REDUCE %d %s %d %d %s\n", i.number,lookahead->token,ppl->p.p.number,j,ppl->p.p.head);	
			lookahead = lookahead->next;
			}
			}

		}

		ppl = ppl->next;

	}

	#ifdef DEBUG2
	printf("addReduce() exited\n");
	#endif
}

int notInClosureList(processedProductionList *new, processedProductionList *list)
{
	#ifdef DEBUG5
	printf("RULE %s point at %d lookahead: \n",new->p.p.rule,new->p.point);
	tokenList * body = new->p.lookahead;
	while(body!=NULL)
	{
		printf("%s ",body->token);
		body = body->next;
	}
	printf("\n");

	#endif

processedProductionList * cur = list;

while(cur!=NULL)
{

	#ifdef DEBUG5
	printf("\t examined RULE \t%s point at %d lookahead: \n",cur->p.p.rule,cur->p.point);
	tokenList * body = cur->p.lookahead;
	while(body!=NULL)
	{
		printf("%s ",body->token);
		body = body->next;
	}
	printf("\n");
	#endif
	
	if(compareProcessedProductions(new->p,cur->p))
	{
	#ifdef DEBUG5
	printf("same production!\n");
	#endif
	return 0;
	}
cur = cur->next;
}
return 1;
}

int compareProcessedProductions(processedProduction a, processedProduction b)
{
if(a.point!=b.point)
return 0;
if(strcmp(a.p.rule,b.p.rule))
return 0;
//check if lookahead is the same
if(!sameTokenList(a.lookahead,b.lookahead))
return 0;

return 1;
}

int isGreaterLookahead(tokenList *a, tokenList * b)
{
int g;

#ifdef DEBUG14
printf("isgreaterlookahead() entered\n");
#endif

#ifdef DEBUG11
printf("removeempty called for a in isgreaterlookahead()\n");
#endif

removeEmpty(&a);
#ifdef DEBUG11
printf("removeempty called for b in isgreaterlookahead()\n");
#endif

removeEmpty(&b);

#ifdef DEBUG17
if(itemnumber==6){
printf("Before ");
tokenList * d = a;
while(d!=NULL){printf("%s\t",d->token);d = d->next;}
printf("\t\t");
d = b;
while(d!=NULL){printf("%s\t",d->token);d = d->next;}
printf("\n");
}
#endif

a = sortTL(&a);
b = sortTL(&b);
#ifdef DEBUG17
if(itemnumber==6){
printf("after ");
 tokenList *d = a;
while(d!=NULL){printf("%s\t",d->token);d = d->next;}
printf("\t\t");
d = b;
while(d!=NULL){printf("%s\t",d->token);d = d->next;}
printf("\n");
}
#endif
while(a!=NULL&&b!=NULL)
{
	g = strcmp(a->token,b->token);
	if(g>0)
	{
	#ifdef DEBUG14
	printf("isgreaterlookahead() exited\n");
	#endif
	return 1;
	}
	if(g<0)
	{
	#ifdef DEBUG14
	printf("isgreaterlookahead() exited\n");
	#endif	
	return 0;
	}
	a = a->next;
	b = b->next;
}
if(a==NULL&&b==NULL)
{
	#ifdef DEBUG14
	printf("isgreaterlookahead() exited\n");
	#endif
	return 0;
}
if(a==NULL)
{
	#ifdef DEBUG14
	printf("isgreaterlookahead() exited\n");
	#endif
return 0;
}

#ifdef DEBUG14
printf("isgreaterlookahead() exited\n");
#endif
return 1;
}
