#include <iostream>
#include "Tokenizer.h"

int main() {

	std::cout << Tokenizer::GetToken(";") << "\n";
	std::cout << Tokenizer::GetToken(")") << "\n";
	std::cout << Tokenizer::GetToken("))") << "\n";
	return 0;
}