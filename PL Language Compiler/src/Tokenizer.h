#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class Tokenizer {
private:
    static std::map<std::string, std::string> dictionary;


public:
    static void ReadTokens();
    static std::string GetToken(const std::string& id);
};

class Scanner {
private:
    static std::string currentBuffer;
    static std::string nextBuffer;

public:
    static void Scan(const char* fileLocation);

};



// Declare forwarding
class State;
class Statemachine;
class Lexer;
// Declare forwarding

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
    std::string lexeme;
public:
    void Init(Statemachine* stateMachine, Lexer* lexer); 
    virtual void Update(std::string currentBuffer,std::string nextBuffer);
    void Print(std::string token) const;
};

//This is the base state for our statemachine
class NormalState :public State {
   
public:
    void Update(std::string currentBuffer, std::string nextBuffer)override;
};

//declaring an instance of all the states and the statemachine so we can start our state pattern design
class Lexer {
public:
    //This is the string which will be displayed at the end
    std::string output;
    //This is the string which will be displayed at the end
    
    //States
    NormalState* normalState;
    //States
    
    //Statemachine
    Statemachine* stateMachine;
    //Statemachine

    Lexer();

    void Update(std::string currentBuffer, std::string nextBuffer);

};

