echo "Constructing parse table"
cc -o ConstructParser ConstructParser.c -O0
echo "Compiled successfully"
./ConstructParser InputGrammar InputGrammarTokens InputGrammarNonterminals
