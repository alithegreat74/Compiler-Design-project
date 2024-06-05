#include "Syntaxical.h"
#include "Syntaxical.h"

// Declaring the static variables
std::unordered_map<std::string, std::vector<Production>> GrammarComputer::grammar=std::unordered_map<std::string, std::vector<Production>>();
std::unordered_map<std::string, std::unordered_set<std::string>> GrammarComputer::firsts=std::unordered_map<std::string, std::unordered_set<std::string>>();
std::unordered_map<std::string, std::unordered_set<std::string>> GrammarComputer::follows= std::unordered_map<std::string, std::unordered_set<std::string>>();
std::unordered_map<std::string, std::vector<Production>> GrammarComputer::parseTable = std::unordered_map<std::string, std::vector<Production>>();

//Defining constants
const std::string GRAMMAR_FILE_PATH = "src/Grammar.txt";
const std::string TOKENS_FILE_PATH = "src/Tokens Output.txt";

void GrammarComputer::Init()
{
	ReadGrammar(GRAMMAR_FILE_PATH);
	ComputeFirsts();
	ComputeFollows();
	ComputeParseTable();
}

void GrammarComputer::ComputeFirsts() {
	//Iterate over all the non terminals
	for (auto nonTerminal : grammar) {
		std::unordered_set<std::string> childFirst = std::unordered_set<std::string>();
		//Calculate and insert the firsts of the given non terminal
		GetNonTerminalFirst(nonTerminal.first);
	}
}
std::unordered_set<std::string> GrammarComputer::GetNonTerminalFirst(std::string nonTerminal)
{
	bool isEpsilonPresent = true;
	//Iterate the productions of the given non terminal
	for (auto production : grammar[nonTerminal]) {
		for (auto symbol : production.symbols) {
			//if the first symbol of the production is terminal, add it to the firsts
			if (grammar.find(symbol) == grammar.end()) {
				firsts[nonTerminal].insert(symbol);
				isEpsilonPresent = false;
			}
			//if it's a non terminal, recursivly find the firsts of that non terminal
			else {

				if (firsts[symbol].empty()) {
					auto childFirsts = GetNonTerminalFirst(symbol);
					//add the firsts of that non terminal to the firsts of the current non terminal
					firsts[nonTerminal].insert(childFirsts.begin(), childFirsts.end());
					if (childFirsts.find("0") == childFirsts.end())
						isEpsilonPresent = false;
					
				}
				else {
					firsts[nonTerminal].insert(firsts[symbol].begin(), firsts[symbol].end());
				}
				
			}
			//we break the loop only to calculate the first symbol of the production
			break;
		}
		if (isEpsilonPresent) {
			firsts[nonTerminal].insert("0");
		}
	}
	//return the found firsts
	return firsts[nonTerminal];
}

std::unordered_set<std::string> GrammarComputer::GetProductionFirsts(Production& production)
{
	std::unordered_set<std::string> result;
	std::string symbol = production.symbols[0];
	//If this symbol is a non terminal add it to the firsts
	if (grammar.find(symbol) == grammar.end()) {
		result.insert(symbol);
	}
	//If it's a non terminal find the firsts of that non terminal
	else
	{
		std::unordered_set<std::string> nonTerminalFirsts = GetNonTerminalFirst(symbol);
		result.insert(nonTerminalFirsts.begin(), nonTerminalFirsts.end());
		if (result.find("0") != result.end())
			result.erase("0");
	}
	//If there is no first for this product, then return epsilon
	if (result.empty())
		result.insert("0");

	return result;
	
}




void GrammarComputer::ComputeFollows()
{
	for (auto nonTerminal : grammar) {
		auto a=GetNonTerminalFollows(nonTerminal.first);
	}
}

void GrammarComputer::ComputeParseTable()
{
	//Iterate over all of non terminals
	for (auto nonTerminal : grammar) {
		//Iterate over all of the productions of that non terminal
		for (auto production : nonTerminal.second) {
			//Get the firsts of the product
			std::unordered_set<std::string>productionFirsts = GetProductionFirsts(production);
			for (auto symbol : productionFirsts) {
				//Case 1: if the symbol is not epsilon add the product to the table
				//The parse table structure like this: Non-Terminal terminal -> product
				if (symbol != "0") {
					parseTable[nonTerminal.first + " " + symbol].push_back(production);
				}
			}
			//Case 2: if the symbol is epsilon, then add all the follows of the non terminal to the table
			if (productionFirsts.find("0") != productionFirsts.end()) {
				std::unordered_set<std::string>nonTerminalFollows = GetNonTerminalFollows(nonTerminal.first);
				for (auto symbol : nonTerminalFollows) {
					parseTable[nonTerminal.first + "|" + symbol].push_back(production);
				}
			}
		}
	}
}

std::unordered_set<std::string> GrammarComputer::GetNonTerminalFollows(std::string nonTerminal)
{
	//If the follows of this non terminal is computed, then return it
	if (follows[nonTerminal].size()>1) {
		return follows[nonTerminal];
	}
	//If it's the S non terminal add the $ symbol to the follows
	if (nonTerminal == "S") {
		follows[nonTerminal].insert("$");
	}
	//Iterate over the non terminals
	for (const auto& g : grammar) {
		//Iterate over the productions of that non terminal
		for (const auto& production : g.second) {
			//Iterate over the symbols of that production
			for (unsigned int i = 0; i < production.symbols.size(); ++i) {
				//If the symbol is the same as the non terminal
				if (production.symbols[i] == nonTerminal) {
					//Case 1: If the symbol is at the end of the production
					if (i + 1 == production.symbols.size()) {
						if (g.first != nonTerminal) { 
							//Add the follow of the left non terminal
							auto followLhs = GetNonTerminalFollows(g.first);
							follows[nonTerminal].insert(followLhs.begin(), followLhs.end());
						}
					}
					else {
						//Case 2: If the next symbol is a terminal
						const std::string& nextSymbol = production.symbols[i + 1];
						if (grammar.find(nextSymbol) == grammar.end()) 
							//Add the terminal to the follows
							follows[nonTerminal].insert(nextSymbol);
						//Case 3: If the next symbol is a non termina
						else {
							//Add the first of that non terminal to the follows
							auto firstNext = GetNonTerminalFirst(nextSymbol);
							firstNext.erase("0");
							follows[nonTerminal].insert(firstNext.begin(), firstNext.end());
							if (firstNext.find("0") != firstNext.end()) {
								auto followNext = GetNonTerminalFollows(nextSymbol);
								follows[nonTerminal].insert(followNext.begin(), followNext.end());
							}
						}
					}
				}
			}
		}
	}

	return follows[nonTerminal];
}

void GrammarComputer::ShowGrammar()
{
	//iterate all the non terminlas
	for (auto g : grammar) {
		std::cout << g.first << " > ";
		//Iterate all the productions
		for (auto p : g.second) {
			//Iterate and print all the symbols in that production
			for (auto s : p.symbols) {
				std::cout << s<<" ";
			}
			std::cout << "| ";
		}
		std::cout << "\n";
	}
}

void GrammarComputer::ShowFirsts()
{
	//Iterate all the non terminals
	for (auto nonTerminal : firsts) {
		std::cout << nonTerminal.first << " > ";
		//Iterate and print all the firsts in that non terminal
		for (auto symbol : nonTerminal.second) {
			std::cout << symbol << ", ";
		}
		std::cout << "\n";
	}
}

void GrammarComputer::ShowFollows()
{
	//Iterate all the non terminals
	for (auto nonTerminal : follows) {
		std::cout << nonTerminal.first << " > ";
		//Iterate and print all the follows in that non terminal
		for (auto symbol : nonTerminal.second) {
			std::cout << symbol << ", ";
		}
		std::cout << "\n";
	}
}

void GrammarComputer::ReadGrammar(std::string path)
{
	//Open the file and check if it's open
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Unable to open the file at: " << path << "\n";
		return;
	}
	//create a buffer and read each line
	std::string buffer;
	while (std::getline(file, buffer))
	{
		//Create a string stream to seperate symbols
		std::stringstream ss(buffer);
		//Create strings to store the left most non-terminal and the arrow and the production symbols
		std::string nonTerminal, arrow,symbol;
		ss >> nonTerminal;
		//Discard the not needed characters that appear at the begining of the file
		if(nonTerminal.length()>2)
			nonTerminal.erase(0, 3);
		//discard the arrow
		ss >> arrow;
		Production produciton;
		while (ss>>symbol)
		{
			//Add the production symbols to the grammar when you see |
			if (symbol == "|") {
				grammar[nonTerminal].push_back(produciton);
				produciton.symbols.clear();
			}
			//Add the symbols the production
			else {
				produciton.symbols.push_back(symbol);
			}
		}
		grammar[nonTerminal].push_back(produciton);
		produciton.symbols.clear();
	}
	file.close();
}


std::string Parser::ReadTokens(std::string filePath) const {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cout << "Error while openning the output file at: " << filePath << "\n";
		return "";
	}
	std::string buffer;
	while (std::getline(file,buffer))
	{
		std::stringstream ss;
	}
}
