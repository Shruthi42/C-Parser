lex lex.l
g++ -w parse.cpp lex.yy.c -DECHO -o parse
