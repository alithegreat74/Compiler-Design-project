#include "Syntaxical.h"

std::unordered_map<std::string, std::vector<Production>> GrammarComputer::grammar;

void GrammarComputer::Init(std::string path)
{
	ReadGrammar(path);
}

void GrammarComputer::ComputeFirst()
{
	//Finds the firsts of the grammar 
}

void GrammarComputer::ComputeFollow()
{
	//Finds the follows of the grammar
}

void GrammarComputer::ShowGrammar()
{
	for (auto g : grammar) {
		std::cout << g.first << " > ";
		for (auto p : g.second) {
			for (auto s : p.symbols) {
				std::cout << s<<" ";
			}
			std::cout << "| ";
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
