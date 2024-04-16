#include <iostream>
#include "Tokenizer.h"

int main() {

	Tokenizer::Init("src/Compiler Tokens.txt");
	std::cout << Tokenizer::GetToken(";") << "\n";
	std::cout << Tokenizer::GetToken("\Hello") << "\n";
	std::cout << Tokenizer::GetToken("0.556") << "\n";
	std::cout << Tokenizer::GetToken("amb") << "\n";
	return 0;
}