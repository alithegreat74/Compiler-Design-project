#include <iostream>
#include <Lexical.h>
#include <Syntaxical.h>

int main() {
	Scanner::Scan("src/test.c");
	Parser parser;
	parser.Parse();
	return 0;
}