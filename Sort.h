#ifndef CONSTRUCTPARSER
#define CONSTRUCTPARSER
#include"ConstructParser.h"
#endif 

tokenList * sortTL(tokenList ** head)
{
char v[MAXTOKENSIZE];
tokenList * ptr1, *ptr2;

	// Don't try to sort empty or single-node lists
	if ((*head) == NULL || (*head)->next == NULL)
	{
		#ifdef DEBUG
		printf("Sort Done\n");
		#endif
	return *head;
	}


    ptr1=*head;
    while(ptr1->next!=NULL)
    {ptr2=ptr1;
        do
        {ptr2=ptr2->next;
        if(strcmp(ptr1->token,ptr2->token)>0)
        {

            strcpy(v,ptr1->token);
            strcpy(ptr1->token,ptr2->token);
            strcpy(ptr2->token,v);
        }

        }while(ptr2->next!=NULL);
    ptr1=ptr1->next;
    }
return *head;
}


/*
tokenList * sortTL(tokenList ** head)
{
	#ifdef DEBUG
	printf("Sort Called\n");
	#endif

	int done = 0;         // True if no swaps were made in a pass

	// Don't try to sort empty or single-node lists
	if ((*head) == NULL || (*head)->next == NULL)
	{
		#ifdef DEBUG
		printf("Sort Done\n");
		#endif
	return *head;
	}


if ((*head)->next->next == NULL)
	{
	#ifdef DEBUG17
		printf("SortPL for 2\n");
	#endif
	#ifdef DEBUG17
		int c= 0;
		tokenList *f = *head;
		while(f!=NULL){printf("%s %ld\t",f->token,f);c++;f = f->next;}
		printf("Num at first : %d\n",c);
	#endif
	tokenList *temp = *head;
	tokenList *temp1 = temp->next;


			if(strcmp(temp->token,temp1->token)>0)
		{
	temp1->next = temp;
	temp->next = NULL;
	head= &temp1;
	}
	#ifdef DEBUG17
		c= 0;
		f = *head;
		while(f!=NULL){printf("%s %ld\t",f->token,f);c++;f = f->next;}
		printf("Num left : %d\n",c);
	#endif
	return *head;	
	}

	if(((*head)->next)->next==NULL)
	{
		if(strcmp((*head)->token,((*head)->next)->token)>0)
		{
			char token[MAXTOKENSIZE];
			strcpy(token,(*head)->token);
			strcpy((*head)->token,((*head)->next)->token);
			strcpy(((*head)->next)->token,token);
		}
	return;
	}
        tokenList *fir = *head;                                    // current node in the list struct
    	while (!done) 
	{
        tokenList **pv = head;            // "source" of the pointer to the

        tokenList *nd = *head;            // local iterator pointer
        tokenList *nx = (*head)->next;  // local next pointer

        done = 1;
	int first =1;
	        while (nx) 
		{
		//if (cmp > 0)
		if(strcmp(nd->token,nx->token)>0) 
		{
	                nd->next = nx->next;
	                nx->next = nd;
	                if(first) fir = nx;
			*pv = nx;
	


		//pv = &(nd->next);
            	//nd = nx;
            	//nx = nx->next;

	                done = 0;
		}
		first = 0;
	           // pv = &(nd->next);
	           // nd = nx;
	            //nx = nx->next;
		pv = &(nd->next);
		nd=nx;
		nx = nx->next;
		//pv=&nd;
	        }
	}
	#ifdef DEBUG
	printf("Sort Done\n");
	#endif
	return *head;
}

*/

processedProductionList * sortPL(processedProductionList ** head)
{
#ifdef DEBUG15
printf("SortPL Called for \n");
processedProductionList * s= *head;
while(s!=NULL){printf("%d\t",s->p.p.number);s= s->next;}
printf("\n");
#endif
	int done = 0;         // True if no swaps were made in a pass

    // Don't try to sort empty or single-node lists
    if ((*head) == NULL || (*head)->next == NULL)
	{
	#ifdef DEBUG15
	printf("SortPL done for single Done\n");
	#endif
	return *head;}


 if ((*head)->next->next == NULL)
	{
	#ifdef DEBUG17
		printf("SortPL for 2\n");
	#endif
	#ifdef DEBUG17
		int c= 0;
		processedProductionList *f = *head;
		while(f!=NULL){printf("%s %ld\t",f->p.p.rule,f);c++;f = f->next;}
		printf("Num at first : %d\n",c);
	#endif
	processedProductionList *temp = *head;
	processedProductionList *temp1 = temp->next;


			if(((temp->p.p.number)> (temp1->p.p.number))
	||(((temp->p.p.number)== (temp1->p.p.number))&&((temp->p.point)> (temp1->p.point)))
	||(((temp->p.p.number)== (temp1->p.p.number))&&((temp->p.point)== (temp1->p.point))&&(isGreaterLookahead(temp->p.lookahead,temp1->p.lookahead)))
	) 
		{

	temp1->next = temp;
	temp->next = NULL;
	head= &temp1;
	}
	#ifdef DEBUG17
		c= 0;
		f = *head;
		while(f!=NULL){printf("%s %ld\t",f->p.p.rule,f);c++;f = f->next;}
		printf("Num left : %d\n",c);
	#endif
	return *head;	
	}


    while (!done) {
        processedProductionList **pv = head;            // "source" of the pointer to the
                                            // current node in the list struct
        processedProductionList *nd = *head;            // local iterator pointer
        processedProductionList *nx = (*head)->next;  // local next pointer

        done = 1;
	int first = 1;

        while (nx) {
		processedProductionList * t1,*t2;
            //if (cmp > 0)
		
		int swap = 0;


		if(((nd->p.p.number)> (nx->p.p.number))
	||(((nd->p.p.number)== (nx->p.p.number))&&((nd->p.point)> (nx->p.point)))
	||(((nd->p.p.number)== (nx->p.p.number))&&((nd->p.point)== (nx->p.point))&&(isGreaterLookahead(nd->p.lookahead,nx->p.lookahead)))
	) 
		{

		swap = 1;
		t1 = nd;
		t2 = nx;

		#ifdef DEBUG15
		printf("nd is %d nx is %d, addr : %ld %ld %ld %ld %ld %ld %ld\n",nd->p.p.number,nx->p.p.number,nd,nx,nd->next,nx->next,t1,t2, *pv);
		#endif		

                nd->next = nx->next;
                nx->next = t1;
                nx->next = t1;
                *pv = nx;
		first =0;

		#ifdef DEBUG17
		if(nd->p.p.number==10)
		printf("swapped\n");	
		#endif

		#ifdef DEBUG15
		printf("1 nd is %d nx is %d, addr : %ld %ld %ld %ld %ld %ld %ld\n",nd->p.p.number,nx->p.p.number,nd,nx,nd->next,nx->next,t1,t2,*pv);
		#endif	

		pv = &(nd->next);
            	nd = nx;
            	nx = nx->next;

                done = 0;
                }
/*
		else if((nd->p.p.number)== (nx->p.p.number))
		{	if((nd->p.point)> (nx->p.point)) 
			{
                	nd->next = nx->next;
                	nx->next = nd;
                	*pv = nx;
	
	                done = 0;
	                }
			else if((nd->p.point)== (nx->p.point))
			{	if(isGreaterLookahead(nd->p.lookahead,nx->p.lookahead)) 
				{
        	        	nd->next = nx->next;
        	        	nx->next = nd;
        		        	*pv = nx;
		
	        	        done = 0;
	        	        }

		
			}

		
		}
*/

		#ifdef DEBUG15
		printf("2 nd is %d nx is %d, addr : %ld %ld %ld %ld\n",nd->p.p.number,nx->p.p.number,nd,nx,nd->next,nx->next);
		if(nx==nd) {printf("SortPL Error.\n");exit(0);}		
		#endif	
	pv = &(nd->next);
            nd = nx;
            nx = nx->next;
          //  pv = &(nd);

//	#ifdef DEBUG15
//	printf("nd is %d nx is %d, addr : %ld %ld\n",nd->p.p.number,nx->p.p.number,nd,nx);
//	#endif	

	#ifdef DEBUG15
	printf("1 iteration in SortPL Done\n");
	#endif
        }
	#ifdef DEBUG15
	printf("1 iteration of !done in SortPL Done\n");
	#endif
    }
	#ifdef DEBUG15
	printf("SortPL Done for >1\n");
	#endif
	return *head;
}

