#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "src/lexer/Lexer.h"
#include "src/parser/Parser.h"
#include "src/interpreter/Interpreter.h"
#include "include/Token.h"
#include "src/ast/AST.h"
#include "src/ast/AST.h"
#include "src/interpreter/Interpreter.h"
#include "src/parser/Parser.h"

int main(int argc, char *argv[]) {
    string sourceCode;

    // Read from standard input
    cout << "Enter your code > " << endl;

    getline(cin, sourceCode);
    cout << sourceCode << endl;
    // 1. Tokenize the source code
    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    // Optional: Print tokens for debugging
    for (const Token &token: tokens) {
        cout << "Token(Type: " << static_cast<int>(token.type) << ", Value: '" << token.value << "', Line: " << token.line << ", Column: " << token.
                column << ")" << endl;
    }

    // 2. Parse the tokens into an AST
    cout << "\nStarting to parse tokens..." << endl;
    Parser parser(tokens);
    vector<unique_ptr<Statement> > statements;
    try {
        statements = parser.parse();
        cout << "Parsing successful!" << endl;
    } catch (const runtime_error &error) {
        cerr << "Parsing error: " << error.what() << endl;
        return 1;
    }

    cout << "\nParsed Statements:" << endl;


    //
    // // Optional: Implement a way to print or visualize the AST for debugging
    //
    // // 3. Interpret the AST
    Interpreter interpreter;
    interpreter.interpret(statements);

    return 0;
}
