
# Compiler for PL Language

This is a compiler for a programing language called PL. PL is a c like language which is a little bit simpler.


## Implemented part

- Lexical Analyzer
- Syntaxical Analyzer
## Lexical Analyzer

This part will read your code and generate tokens for the syntax analyzer.

### Token Types

| Token Type | Examples     | 
| :-------- | :------- | 
| `static_tokens` | `e.g. semicolons, curly brackets` |
| `dyanmic_tokens` | `e.g. strings, characters` |

### Implemented lexemes
This program can analyze these types of lexemes

- Static Leximes in the file``` src/Compiler Tokens.txt```
- Strings
- Characters
- One line comments
- Decimal numbers
- Hexadecimal numbers 



## Tests

To test this project you can change the file in
```bash
  src/test.c
```


## How to Run the project

For building the program follow these steps using CMake

```bash
  - run git clone https://github.com/alithegreat74/Compiler-Design-project.git
  - makedir build
  - cd build
  - cmake ..
  - cmake --build .
```


    
## Syntaxical Analyzer

This part will Parse the generated tokens and checks the syntax of the code you wrote based on the context-free grammar written in
```
src/Grammar.txt 
```


