#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


class Tokenizer {
private:
	static std::map<std::string, std::string> dictionary;
public:
	static void ReadTokens();
	static std::string GetToken(std::string id);

};