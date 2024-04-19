#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class Tokenizer {
private:
    static std::map<std::string, std::string> staticDictionary;
    static std::map<std::string, bool(*)(const std::string&)> dynamicDictionary;

public:
    static void Init();
    static void ReadTokens();
    static std::string GetToken(const std::string& id);
};

class Scanner {
private:
    static char currentBuffer;
    static char nextBuffer;

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
public:
    void Init(Statemachine* stateMachine, Lexer* lexer); 
    virtual void Update(); 
};

//This is the base state for our statemachine
class NormalState :public State {
   
public:
    void Update()override;
};

//declaring an instance of all the states and the statemachine so we can start our state pattern design
class Lexer {
public:
    NormalState* normalState;
    Statemachine* stateMachine;
    Lexer();
    void Update();

};

