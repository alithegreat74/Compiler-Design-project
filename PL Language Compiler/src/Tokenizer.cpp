#include "Tokenizer.h"

std::map<std::string, std::string> Tokenizer::dictionary;


void Tokenizer::ReadTokens()
{
	//Opening the token dictionary file
	std::ifstream file("src/Compiler Tokens.txt");

	//checking to see if the path is correct
	if (!file.is_open()) {
		std::cout << "Unable to open the file " << "\n";
		return;
	}
	std::string buffer;
	//Reading each line of the file and setting the key and value for each line
	//The lines are oriented as such 'Value in PL language'\t'PL language token'
	//e.g. ';'\t'T_Seⅿiⅽoⅼon'
	while (std::getline(file,buffer))
	{
		//using stringstream to seperate the words by the \t
		std::stringstream ss(buffer);
		std::string key;
		std::getline(ss, key, '\t');
		std::string token;
		std::getline(ss, token, '\t');
		//adding the key and the token to our dictionary
		dictionary.insert(std::make_pair(key,token));
	}
	
}

std::string Tokenizer::GetToken(std::string id)
{
	if (dictionary.empty()) {
		ReadTokens();
	}

	if (dictionary[id] == "") {
		std::cout << "Key not found\n";
		return NULL;
	}
	else
	{
		return dictionary[id];
	}

}

