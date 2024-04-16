#include "Tokenizer.h"

std::map<std::string, std::string> Tokenizer::staticDictionary;
std::map<std::string, TokenCheckFunction> Tokenizer::dynamicDictionary;
const char* Tokenizer::fileLocation = "";

void Tokenizer::Init(const char* location)
{
	fileLocation = location;
	dynamicDictionary = {
		{"T_String",     [](const std::string& id) { return id.find('"') != std::string::npos; }},
		{"T_Character", [](const std::string& id) {return id.find("'") != std::string::npos && id.find('"') == std::string::npos; }},
		{"T_Comment",[](const std::string& id) {return id.find("//") != std::string::npos; }},
		{"T_Whitspace", [](const std::string& id) { return id == " "; }},
		{"T_Decimal",[](const std::string& id) {return id.find(".") != std::string::npos &&
												id.find("'") == std::string::npos && id.find('"') == std::string::npos; }},
		{"T_Hexadecimal",[](const std::string& id) {return id.find("0X") != std::string::npos &&
												id.find("'") == std::string::npos && id.find('"') == std::string::npos; }}
	};
}

void Tokenizer::ReadTokens()
{

	//Opening the token dictionary file
	std::ifstream file(fileLocation);

	//checking to see if the path is correct
	if (!file.is_open()) {
		std::cout << "Unable to open the file " << "\n";
		return;
	}
	std::string buffer;
	//Reading each line of the file and setting the key and value for each line
	//The lines are oriented as such 'Value in PL language'\t'PL language token'
	//e.g. ';'\t'T_Seⅿiⅽoⅼon'
	while (std::getline(file, buffer))
	{
		//using stringstream to seperate the words by the \t
		std::stringstream ss(buffer);
		std::string key;
		std::getline(ss, key, '\t');
		std::string token;
		std::getline(ss, token, '\t');
		//adding the key and the token to our dictionary
		staticDictionary.insert(std::make_pair(key, token));
	}

}
