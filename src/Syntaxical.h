#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <stack>


//The symbols that a non terminal produces
struct Production
{
	std::vector<std::string> symbols;
};
struct Token
{
	std::string lexeme, token;
	int line;
};
//Give the file address of your grammar and get the information like first and follow
class GrammarComputer {
public:
	static void Init();
	static std::unordered_map<std::string, std::vector<Production>> grammar;
	static std::unordered_map<std::string, std::unordered_set<std::string>> firsts;
	static std::unordered_map<std::string, std::unordered_set<std::string>> follows;
	static std::unordered_map<std::string, std::vector<Production>> parseTable;
private:
	//Calculates the first of each non-terminal
	static void ComputeFirsts();
	//Calculates the follow of each non-terminal
	static void ComputeFollows();
	//Calculates and creates a parse table for the grammar;
	static void ComputeParseTable();
	//Prints the Grammar
	static void ShowGrammar();
	//Prints the firsts
	static void ShowFirsts();
	//Checks if each cell in the table has more than 1 value
	static void CheckParseTable();
	//Prints the follows
	static void ShowFollows();
	//Reads the Grammar File
	static void ReadGrammar(std::string path);
	//Recursive function that calculates the firsts of the given non terminal
	static std::unordered_set<std::string> GetNonTerminalFirst(std::string nonTerminal);
	//This function will calculate the firsts of a Production
	static std::unordered_set<std::string> GetProductionFirsts(const Production& production);
	//Recursive Function that calculates the follows of the given non terminal
	static std::unordered_set<std::string> GetNonTerminalFollows(const std::string& nonTerminal);
};

class Parser {
public:
	Parser();
	void Parse();
private:
	void ReadTokens(std::string filePath);
	std::vector<Token>tokens;
	bool Panic(int& increment,std::stack<std::string>&stack);
};