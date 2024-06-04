#include "Syntaxical.h"

// Declaring the static variables
std::unordered_map<std::string, std::vector<Production>> GrammarComputer::grammar;
std::unordered_map<std::string, std::unordered_set<std::string>> GrammarComputer::firsts;

void GrammarComputer::Init(std::string path)
{
	ReadGrammar(path);
	ComputeFirsts();
}

void GrammarComputer::ComputeFirsts() {
	//Iterate over all the non terminals
	for (auto nonTerminal : grammar) {
		std::unordered_set<std::string> childFirst = std::unordered_set<std::string>();
		//Calculate the firsts of the given non terminal
		childFirst = GetNonTerminalFirst(nonTerminal.first);
		//Insert the found firsts
		firsts[nonTerminal.first].insert(childFirst.begin(), childFirst.end());
	}
}
std::unordered_set<std::string> GrammarComputer::GetNonTerminalFirst(std::string nonTerminal)
{
	//Iterate the productions of the given non terminal
	for (auto production : grammar[nonTerminal]) {
		for (auto symbol : production.symbols) {
			//if the first symbol of the production is terminal, add it to the firsts
			if (grammar.find(symbol) == grammar.end()) {
				firsts[nonTerminal].insert(symbol);
			}
			//if it's a non terminal, recursivly find the firsts of that non terminal
			else {

				if (firsts[symbol].empty()) {
					auto childFirsts = GetNonTerminalFirst(symbol);
					//add the firsts of that non terminal to the firsts of the current non terminal
					firsts[nonTerminal].insert(childFirsts.begin(), childFirsts.end());
				}
				else {
					firsts[nonTerminal].insert(firsts[symbol].begin(), firsts[symbol].end());
				}
				
			}
			//we break the loop only to calculate the first symbol of the production
			break;
		}
	}
	//return the found firsts
	return firsts[nonTerminal];
}


void GrammarComputer::ComputeFollows()
{
	
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
}

