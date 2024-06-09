#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <memory> //For smart pointers


//This class will return static tokens
class Tokenizer {
private:
    static std::map<std::string, std::string> dictionary;
public:
    static void ReadTokens();
    static std::string GetToken(const std::string& id);
};

//This class will read inputs and send them to our lexer
class Scanner {
private:
    static std::string currentBuffer;
    static std::string nextBuffer;
public:
    static void Scan(const char* fileLocation);
};

static bool IsHex(std::string id);

#pragma region Declare forwarding
class State;
class Statemachine;
class Lexer;
#pragma endregion

#pragma region Statemachine
// Creating a class to handle our States
class Statemachine {
public:
    //Keeping track of the current state
    State* currentState;
    void Init(State* firstState);
    void ChangeState(State* newState);
};


//Creating an abstract class to derive each state from it
class State {
protected:
    Statemachine* stateMachine;
    Lexer* lexer;
    bool skip;
public:
    std::string lexeme;
    virtual void Init(Statemachine* stateMachine, Lexer* lexer);
    virtual void Update(std::string currentBuffer, std::string nextBuffer);
    void Print(std::string token) const;
    bool Skip();
};

//This is the start state for analyzing each input
class NormalState :public State {
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//This is a state for whenever we are trying to read a string
class StringState :public State {
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//This is a state for whenever we are trying to read a character
class CharacterState :public State {
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//This is a state for whenever we are trying to read a comment;
class CommentState :public State {
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//This is a state for whenever we are trying to read a decimal number
class DecimalState :public State {
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//This is a state for whenever we are trying to read hexadeciam number
class HexadecimalState :public State {
private:
    bool xFlag;
public:
    void Init(Statemachine* stateMachine, Lexer* lexer)override;
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};
#pragma endregion

#define StateExit(token)    Print(token);\
                            lexeme = "";\
                            stateMachine->ChangeState(lexer->normalState);


#define StateEnter(state) lexeme = "";\
                        stateMachine->ChangeState(state);\
                        state->Update(currentBuffer, nextBuffer);

//declaring an instance of all the states and the statemachine so we can start our state pattern design
class Lexer {
public:
    //This is the string which will be displayed at the end
    std::string output;
    int currentLine;
    //States
    NormalState* normalState;
    StringState* stringState;
    CharacterState* characterState;
    CommentState* commentState;
    DecimalState* decimalState;
    HexadecimalState* hexadecimalState;
    //Statemachine
    Statemachine* stateMachine;
    Lexer();
    ~Lexer();
    void Update(std::string currentBuffer, std::string nextBuffer);
    void NextLine() { 
        currentLine++;
        if (stateMachine->currentState->lexeme != "" && stateMachine->currentState == normalState) {
            stateMachine->currentState->Print("T_Id");
            stateMachine->currentState->lexeme = "";
        }
    }
};

