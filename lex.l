L            [a-zA-Z_]
E            [Ee][+-]?[0-9]+
FS            (f|F|l|L)
IS            (u|U|l|L)*

%{
#include <stdio.h>
#include "typedef.h"
#include "defineLineSize.h"

void MultilineComment();
int checkiftype();
void update();
int init();
void singleLineComment();
void preprocessor();
int lines = 1;
int column = 0;
char fullline[MAXLINESIZE] = "";
%}

%%
"/*"            { MultilineComment(); }
"//"		{singleLineComment();}
"#" 		{preprocessor();}

"auto"            { update(); return(AUTO); }
"extern"        { update(); return(EXTERN); }
"register"        { update(); return(REGISTER); }
"static"        { update(); return(STATIC); }
"volatile"        { update(); return(VOLATILE); }

"enum"            { update(); return(ENUM); }
"struct"        { update(); return(STRUCT); }
"union"            { update(); return(UNION); }

"if"            { update(); return(IF); }
"else"            { update(); return(ELSE); }
"for"            { update(); return(FOR); }
"while"            { update(); return(WHILE); }
"do"            { update(); return(DO); }
"switch"        { update(); return(SWITCH); }
"case"            { update(); return(CASE); }
"break"            { update(); return(BREAK); }
"continue"        { update(); return(CONTINUE); }
"goto"            { update(); return(GOTO); }
"return"        { update(); return(RETURN); }

"const"            { update(); return(CONST); }
"default"        { update(); return(DEFAULT); }

"char"            { update(); return(TYPE); }
"double"        { update(); return(TYPE); }
"float"            { update(); return(TYPE); }
"int"            { update(); return(TYPE); }
"long"            { update(); return(TYPE); }
"short"            { update(); return(TYPE); }
"signed"        { update(); return(TYPE); }
"void"            { update(); return(TYPE); }
"unsigned"        { update(); return(TYPE); }

"sizeof"        { update(); return(SIZEOF); }
"typedef"        { update(); return(TYPEDEF); }

[a-zA-Z_][a-zA-Z0-9_]*        { update(); return(checkiftype()); }

0[xX][a-fA-F0-9]+{IS}?        { update(); return(CONSTANT); }
0[0-9]+{IS}?        { update(); return(CONSTANT); }
[0-9]+{IS}?        { update(); return(CONSTANT); }
[a-zA-Z_]?'(\\.|[^\\'])+'    { update(); return(CONSTANT); }

[0-9]+{E}{FS}?        { update(); return(CONSTANT); }
[0-9]*"."[0-9]+({E})?{FS}?    { update(); return(CONSTANT); }
[0-9]+"."[0-9]*({E})?{FS}?    { update(); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"    { update(); return(STRING_LITERAL); }

"..."            { update(); return(ELLIPSIS); }
">>="            { update(); return(ASSIGN); }
"<<="            { update(); return(ASSIGN); }
"+="            { update(); return(ASSIGN); }
"-="            { update(); return(ASSIGN); }
"*="            { update(); return(ASSIGN); }
"/="            { update(); return(ASSIGN); }
"%="            { update(); return(ASSIGN); }
"&="            { update(); return(ASSIGN); }
"^="            { update(); return(ASSIGN); }
"|="            { update(); return(ASSIGN); }
">>"            { update(); return(SHIFT_OP); }
"<<"            { update(); return(SHIFT_OP); }
"++"            { update(); return(INC_DEC_OP); }
"--"            { update(); return(INC_DEC_OP); }
"->"            { update(); return(PTR_OP); }
"&&"            { update(); return(AND_OP); }
"||"            { update(); return(OR_OP); }
"<="            { update(); return(LE_GE_OP); }
">="            { update(); return(LE_GE_OP); }
"=="            { update(); return(EQ_NE_OP); }
"!="            { update(); return(EQ_NE_OP); }
";"            { update(); return(';'); }
("{"|"<%")        { update(); return('{'); }
("}"|"%>")        { update(); return('}'); }
","            { update(); return(','); }
":"            { update(); return(':'); }
"="            { update(); return('='); }
"("            { update(); return('('); }
")"            { update(); return(')'); }
("["|"<:")        { update(); return('['); }
("]"|":>")        { update(); return(']'); }
"."            { update(); return('.'); }
"&"            { update(); return('&'); }
"!"            { update(); return(UN_OP); }
"~"            { update(); return(UN_OP); }
"-"            { update(); return(UN_OP); }
"+"            { update(); return('+'); }
"*"            { update(); return('*'); }
"/"            { update(); return('/'); }
"%"            { update(); return('%'); }
"<"            { update(); return('<'); }
">"            { update(); return('>'); }
"^"            { update(); return('^'); }
"|"            { update(); return('|'); }
"?"            { update(); return('?'); }

[ \t\v\f]        { update(); }
[\n]		{ update(); lines++;strcpy(fullline,"");}
.            { }

%%

int yywrap()
{
    return 1;
}


void singleLineComment()
{
    char c;

    while ((c = yyinput()) != '\n' && c != 0);
	lines++;
}

void preprocessor()
{
    char c;

    while ((c = yyinput()) != '\n' && c != 0);
	lines++;
}

void MultilineComment()
{
    char c, d;

again:
    while ((c = yyinput()) != '*' && c != 0)
        {
	if(c=='\n') {lines++;}	
	}
    if ((d = yyinput()) != '/' && c != 0)
    {
        goto again;
    }
}

void update()
{
    int i;


    for (i = 0; yytext[i] != '\0'; i++)
        if (yytext[i] == '\n')
            column = 0;
        else if (yytext[i] == '\t')
            column += 8 - (column % 8);
        else
            column++;

	
	strcat(fullline,yytext);

}


int checkiftype()
{
//depending on grammar may need to check whether it is a type name
//our grammar robust to this
return(IDENTIFIER);
}
