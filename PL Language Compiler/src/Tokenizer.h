#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using TokenCheckFunction = bool(*)(const std::string&);

class Tokenizer {
private:
	static const char* fileLocation;
	// A dictionary for the tokens that are static and can't change in the code
	static std::map<std::string, std::string> staticDictionary;	
	// A dictionary for the tokens that are dynamic and can change in the code
	static  std::map<std::string, TokenCheckFunction> dynamicDictionary;





public:
	static void Init(const char* location);
	static void ReadTokens();
	
	__forceinline static std::string GetToken(std::string id) {
		//If this is the first time the Get Token method is getting called, and the map is empty, read the file
		if (staticDictionary.empty()) {
			ReadTokens();
		}
		//If the key doesn't exist in the static map, check the dynamic map
		if (staticDictionary[id] == "") {
			for (const auto& pair : dynamicDictionary) {
				if (pair.second(id))
					return pair.first;
			}
			return "T_Id";
		}
		else
		{
			return staticDictionary[id];
		}

	};

};


