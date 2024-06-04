#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

//Types of Grammar symbol
enum SymbolType
{
	TERMINAL,NON_TERMINAL
};
//The symbols that a non terminal produces
struct Production
{
	std::vector<std::string> symbols;
};

//Give the file address of your grammar and get the information like first and follow
class GrammarComputer {
	
public:
	static void Init(std::string path);
	//Gives the first of each non-terminal
	static void ComputeFirst();
	//Gives the follow of each non-terminal
	static void ComputeFollow();
	//Prints the Grammar
	static void ShowGrammar();
private:
	//Reads the Grammar File
	static void ReadGrammar(std::string path);
	static std::unordered_map<std::string, std::vector<Production>> grammar;
};