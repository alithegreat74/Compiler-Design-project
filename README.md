
# Compiler for PL Language

This is a compiler for a programing language called PL. PL is a c like language which is a little bit simpler.


## Implemented part

- Lexical Analyzer

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
  - Clone the project
  - Open Cmake (either gui or in cmd)
  - Open the source code path in Cmake
  - Generate the code for you compiler
```


    
