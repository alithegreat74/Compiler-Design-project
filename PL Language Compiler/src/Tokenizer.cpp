#include "Tokenizer.h"

// Define constants
const std::string NONE_TOKEN = "None";
const std::string FILE_PATH = "src/Compiler Tokens.txt";

// Initialize static member variables
char Scanner::currentBuffer = '\0';
char Scanner::nextBuffer = '\0';

std::map<std::string, std::string> Tokenizer::staticDictionary;
std::map<std::string, bool(*)(const std::string&)> Tokenizer::dynamicDictionary;

void Tokenizer::Init() {
    dynamicDictionary = {
        {"T_String",     [](const std::string& id) { return id.find('"') != std::string::npos; }},
        {"T_Character",  [](const std::string& id) { return id.find("'") != std::string::npos && id.find('"') == std::string::npos; }},
        {"T_Comment",    [](const std::string& id) { return id.find("//") != std::string::npos; }},
        {"T_Whitespace", [](const std::string& id) { return id == " "; }},
        {"T_Decimal",    [](const std::string& id) { return id.find(".") != std::string::npos &&
                                                          id.find("'") == std::string::npos && id.find('"') == std::string::npos; }},
        {"T_Hexadecimal",[](const std::string& id) { return id.find("0X") != std::string::npos &&
                                                          id.find("'") == std::string::npos && id.find('"') == std::string::npos; }}
    };
}

void Tokenizer::ReadTokens() {
    std::ifstream file(FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Unable to open the file " << FILE_PATH << std::endl;
        return;
    }

    std::string buffer;
    while (std::getline(file, buffer)) {
        std::stringstream ss(buffer);
        std::string key, token;
        std::getline(ss, key, '\t');
        std::getline(ss, token, '\t');
        staticDictionary.insert(std::make_pair(key, token));
    }
}

std::string Tokenizer::GetToken(const std::string& id) {
    if (staticDictionary.empty()) {
        ReadTokens();
    }
    auto staticToken = staticDictionary.find(id);
    if (staticToken != staticDictionary.end()) {
        return staticToken->second;
    }

    for (const auto& pair : dynamicDictionary) {
        if (pair.second(id)) {
            return pair.first;
        }
    }
    return NONE_TOKEN;
}

bool IsNumeric(std::string val) {
    for (const char c : val) {
        if (!std::isdigit(c) && c != '.') {
            return false;
        }
    }
    return true;
}

void Scanner::Scan(const char* fileLocation) {
    currentBuffer = '\0';
    nextBuffer = '\0';

    std::ifstream file(fileLocation);
    Tokenizer::Init();

    if (!file.is_open()) {
        std::cerr << "Unable to open file at " << fileLocation << std::endl;
        return;
    }

    std::string tokentest;
    while (file.get(nextBuffer)) {
        if (Tokenizer::GetToken(tokentest + currentBuffer + nextBuffer) == NONE_TOKEN) {
            tokentest += currentBuffer;
            currentBuffer = nextBuffer;
        }
        else {
            tokentest.clear();
        }
    }
}





void Statemachine::Init(State* firstState)
{
    currentState = firstState;
}

void Statemachine::ChangeState(State* newState)
{
    currentState = newState;
}

void State::Init(Statemachine* stateMachine, Lexer* lexer)
{
    this->stateMachine = stateMachine;
    this->lexer = lexer;
}

void State::Update()
{
}

void NormalState::Update()
{
    std::cout << "Normal State\n";
}

Lexer::Lexer()
{
    normalState = new NormalState();
    stateMachine = new Statemachine();
    normalState->Init(stateMachine, this);
    stateMachine->Init(normalState);
}

void Lexer::Update()
{
    stateMachine->currentState->Update();
}
