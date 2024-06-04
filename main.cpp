#include <iostream>
#include <Lexical.h>
#include <Syntaxical.h>

int main() {
	/*Scanner::Scan("src/test.c");*/
	GrammarComputer::Init("src/Grammar.txt");
	GrammarComputer::ShowGrammar();
	return 0;
	
}

