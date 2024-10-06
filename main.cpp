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
    std::string sourceCode;

    // Check if a file was provided
    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Could not open file: " << argv[1] << std::endl;
            return 1;
        }

        // Read the file contents into the sourceCode string
        std::stringstream buffer;
        buffer << file.rdbuf();
        sourceCode = buffer.str();
        file.close();
    } else {
        // If no file is provided, fallback to standard input
        std::cout << "Enter your code > " << std::endl;
        std::getline(std::cin, sourceCode);
    }

    // 1. Tokenize the source code
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    // Optional: Print tokens for debugging
    for (const Token &token: tokens) {
        std::cout << "Token(Type: " << static_cast<int>(token.type) << ", Value: '" << token.value << "', Line: " << token.line << ", Column: " <<
                token.column << ")" << std::endl;
    }

    // 2. Parse the tokens into an AST
    std::cout << "\nStarting to parse tokens..." << std::endl;
    Parser parser(tokens);
    std::vector<std::unique_ptr<Statement> > statements;
    try {
        statements = parser.parse();
        std::cout << "Parsing successful!" << std::endl;
    } catch (const std::runtime_error &error) {
        std::cerr << "Parsing error: " << error.what() << std::endl;
        return 1;
    }

    // 3. Interpret the AST
    Interpreter interpreter;
    interpreter.interpret(statements);

    return 0;
}
