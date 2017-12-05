#include<string.h>

void strcpymine(char*a,char*b)
{
strncpy(a,b,MAXTOKENSIZE-1);
a[MAXTOKENSIZE-1] = '\0';
}

void strcpyrule(char*a,char*b)
{
strncpy(a,b,MAXRULESIZE-1);
a[MAXRULESIZE-1] = '\0';
}
