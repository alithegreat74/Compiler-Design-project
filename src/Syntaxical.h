#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//The symbols that a non terminal produces
struct Production
{
	std::vector<std::string> symbols;
};

//Give the file address of your grammar and get the information like first and follow
class GrammarComputer {
	
public:
	static void Init(std::string path);

private:
	//Gives the first of each non-terminal
	static void ComputeFirsts();
	//Gives the follow of each non-terminal
	static void ComputeFollows();
	//Prints the Grammar
	static void ShowGrammar();
	//Prints the firsts
	static void ShowFirsts();
	//Prints the follows
	static void ShowFollows();
	//Reads the Grammar File
	static void ReadGrammar(std::string path);
	//Recursive function that calculates the firsts of the given non terminal
	static std::unordered_set<std::string> GetNonTerminalFirst(std::string nonTerminal);
	//Recursive Function that calculates the follows of the given non terminal
	static std::unordered_set<std::string> GetNonTerminalFollows(std::string nonTerminal);
	static std::unordered_map<std::string, std::vector<Production>> grammar;
	static std::unordered_map<std::string, std::unordered_set<std::string>> firsts;
	static std::unordered_map<std::string, std::unordered_set<std::string>> follows;
};