#include "Lexical.h"

// Define constants
const std::string NONE_TOKEN = "None";
const std::string TOKEN_FILE_PATH = "src/Compiler Tokens.txt";
const std::string OUTPUT_FILE = "src/Tokens Output.txt";

// Initialize static member variables
std::string Scanner::currentBuffer = NONE_TOKEN;
std::string Scanner::nextBuffer = NONE_TOKEN;

std::map<std::string, std::string> Tokenizer::dictionary;


void Tokenizer::ReadTokens() {
    //Open the Token's file
    std::ifstream file(TOKEN_FILE_PATH);
    if (!file.is_open()) {
        std::cerr << "Unable to open the file " << TOKEN_FILE_PATH << std::endl;
        return;
    }

    std::string buffer;
    //Read it line by line
    while (std::getline(file, buffer)) {

        std::stringstream ss(buffer);
        std::string key, token;
        //Seperate the key and the token which are seperated by a tab in the file
        std::getline(ss, key, '\t');
        std::getline(ss, token, '\t');
        //add them to the dictionary
        dictionary.insert(std::make_pair(key, token));
    }
    //add the whitespace token which can't be added in the file so we have to add it manualy
    dictionary.insert(std::make_pair(" ", "T_Whitespace"));
    file.close();
}

std::string Tokenizer::GetToken(const std::string& id) {
    if (dictionary.empty()) {
        ReadTokens();
    }
    auto staticToken = dictionary.find(id);
    if (staticToken != dictionary.end()) {
        return staticToken->second;
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

    std::ifstream file(fileLocation);
    
    Lexer lexer;
   

    if (!file.is_open()) {
        std::cerr << "Unable to open file at " << fileLocation << std::endl;
        return;
    }
    char buffer = '\0';
    while (file.get(buffer)) {
        //If it's the first time running the lexer
        if (currentBuffer == NONE_TOKEN) {
            currentBuffer = buffer;
            if(file.get(buffer))
                nextBuffer = buffer;
        }
        else
        {
            nextBuffer = buffer;
        }

        //Run the lexer for buffers
        lexer.Update(currentBuffer, nextBuffer);
        //increment the lexer line
        if (currentBuffer == "\n")
            lexer.NextLine();

        //swap the buffers
        currentBuffer = nextBuffer;
    }
    lexer.Update(currentBuffer, nextBuffer);
    file.close();

    //Write the output to the output file
    std::ofstream outputFile;
    outputFile.open(OUTPUT_FILE);
    if (!outputFile.is_open()) {
        std::cout << "Error while opennig the output file\n";
        return;
    }
    outputFile << lexer.output;
    outputFile.close();

}







void Statemachine::Init(State* firstState)
{
    currentState = firstState;
}

void Statemachine::ChangeState(State* newState)
{
    currentState = newState;
}

//Initialization for each state
//Each state needs a pointer to the statemachine and the lexer class
//This helps changing the states easier
void State::Init(Statemachine* stateMachine, Lexer* lexer)
{
    this->stateMachine = stateMachine;
    this->lexer = lexer;
}
//Giving each state it's own Update function which will be for analyzing each lexeme
void State::Update(std::string currentBuffer, std::string nextBuffer)
{
    if(currentBuffer!=NONE_TOKEN && currentBuffer!="\t" && currentBuffer!="\n")
        this->lexeme += currentBuffer;
}



//Printing the found token for each state to the output in the lexer class
void State::Print(std::string token) const
{
    if(token!="T_Whitespace" && token!="T_Comment")
        lexer->output += std::to_string(lexer->currentLine)+" : " + lexeme +" -> " + token + '\n';
}

bool State::Skip()
{
    if (skip)
    {
        this->skip = 0;
        return true;
    }
    return false;
}

//override function of the State::Update
void NormalState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);

    if (Tokenizer::GetToken(lexeme + nextBuffer) != NONE_TOKEN)
        return;
    //If the input is " then go to string state
    if (currentBuffer == "\"") {
        StateEnter(lexer->stringState);
        return;
    }
    
    //If the input is ' then go to character state
    if (currentBuffer == "'") {
        StateEnter(lexer->characterState);
        return;
    }
    //If the input begins with // then go to comment state
    if (currentBuffer + nextBuffer == "//")
    {
        StateEnter(lexer->commentState);
        return;
    }
    //if the input starts with 0x then go to hexadecimal state 
    if (currentBuffer + nextBuffer == "0X" ||
        currentBuffer + nextBuffer == "0x") {
        StateEnter(lexer->hexadecimalState);
        return;
    }

    //If the input is a number and there is nothing before it then go to number state
    if ((IsNumeric(currentBuffer) && lexeme.length() == 1)||
        (currentBuffer == "+" && IsNumeric(nextBuffer))||
        (currentBuffer == "-" && IsNumeric(nextBuffer)))
    {
        StateEnter(lexer->decimalState);
        return;
    }
    //If the input is a static lexeme then print it
    if (Tokenizer::GetToken(lexeme) != NONE_TOKEN)
    {
        Print(Tokenizer::GetToken(lexeme));
        lexeme = "";
        return;
    }
    
    if (Tokenizer::GetToken(nextBuffer) != NONE_TOKEN && lexeme.length() > 0) {
        Print("T_Id");
        lexeme = "";
        return;
    }
    
}

Lexer::Lexer()
{
    this->currentLine = 1;
    stateMachine = new Statemachine();
    normalState = new NormalState();
    stringState = new StringState();
    characterState = new CharacterState();
    commentState = new CommentState();
    decimalState = new DecimalState();
    hexadecimalState = new HexadecimalState();
    normalState->Init(stateMachine, this);
    stringState->Init(stateMachine, this);
    characterState->Init(stateMachine, this);
    commentState->Init(stateMachine, this);
    decimalState->Init(stateMachine, this);
    hexadecimalState->Init(stateMachine, this);
    stateMachine->Init(normalState);
}

Lexer::~Lexer()
{
    delete normalState;
    delete stringState;
    delete characterState;
    delete commentState;
    delete decimalState;
    delete hexadecimalState;
}

void Lexer::Update(std::string currentBuffer, std::string nextBuffer)
{
    //Analyze input with the current state
    stateMachine->currentState->Update(currentBuffer,nextBuffer);
    
        
}

void StringState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);

    //If the skip flag is true don't analyze
    if (Skip())
        return;

    //If there is a normal double quotation we want to skip that and not see it as an end quote
    if (currentBuffer.at(0) == '\\' && nextBuffer.at(0) == '\"') {
        skip = 1;
        return;
    }

    //If you see another " means that the string is finished
    //So print the token and go back to normal state
    //If you see the close double quotation print token and change the state back to normal 
    if (currentBuffer.at(0) == '"') {
        StateExit("T_String");
        return;

    }

}

void CharacterState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);

    //If the skip flag is on, don't analyze
    if (Skip())
        return;
    //if the character is \' dont count it as '
    if (currentBuffer.at(0) == '\\' && nextBuffer.at(0) == '\'') {
        skip = 1;
        return;
    }
    //if character is ' then print the token and change state
    if (currentBuffer.at(0) == '\'') {
        StateExit("T_Character");
        return;
    }
    
}

void CommentState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);
    //If it's the next line
    if (currentBuffer == "\n"){
        StateExit("T_Comment");
        return;
    }
    
}

void DecimalState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);
    //If the current input is not a number then go back to nomal state
    if (!IsNumeric(nextBuffer)) {
        StateExit("T_Decimal");
        return;
    }
    
}

void HexadecimalState::Init(Statemachine* stateMachine, Lexer* lexer)
{
    State::Init(stateMachine, lexer);
    xFlag = 1;
}

void HexadecimalState::Update(std::string currentBuffer, std::string nextBuffer)
{
    State::Update(currentBuffer, nextBuffer);

    //If it's the beginning of the number 0x don't check the current buffer
    if (xFlag) {
        xFlag = false;
        return;
    }
    //If the input is not a number or ABCDE go back to normal state
    if (!IsHex(nextBuffer))
    {
        StateExit("T_Hexadecimal");
        return;
    }
}

bool IsHex(std::string id)
{
    for (const auto c : id) {
        if (!std::isdigit(c))
            if (c < 'A' || c > 'F')
                if (c < 'a' || c>'f')
                    return false;
    }
    return true;
}
