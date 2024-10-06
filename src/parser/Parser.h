#ifndef PARSER_H
#define PARSER_H

#include "../../include/Token.h"
#include "../ast/AST.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

class Parser {
public:
    Parser(const std::vector<Token> &tokens);

    // Parses the entire input and returns a list of statements
    std::vector<std::unique_ptr<Statement> > parse();

private:
    const std::vector<Token> &tokens_;
    size_t current_;

    // Helper methods
    bool isAtEnd() const;

    const Token &peek() const;

    const Token &previous() const;

    const Token &advance();

    bool check(TokenType type) const;

    bool match(const std::vector<TokenType> &types);

    Token consume(TokenType type, const std::string &errorMessage);

    // Parsing methods
    std::unique_ptr<Statement> declaration();

    std::unique_ptr<Statement> variableDeclaration();

    std::unique_ptr<Statement> functionDeclaration();

    std::unique_ptr<Statement> statement();

    std::unique_ptr<Statement> ifStatement();

    std::unique_ptr<Statement> whileStatement();

    std::unique_ptr<Statement> forStatement();

    std::unique_ptr<Statement> returnStatement();

    std::unique_ptr<Statement> blockStatement();

    std::unique_ptr<Statement> expressionStatement();

    std::unique_ptr<Expression> expression();

    std::unique_ptr<Expression> assignment();

    std::unique_ptr<Expression> logicalOr();

    std::unique_ptr<Expression> logicalAnd();

    std::unique_ptr<Expression> equality();

    std::unique_ptr<Expression> comparison();

    std::unique_ptr<Expression> term();

    std::unique_ptr<Expression> factor();

    std::unique_ptr<Expression> unary();

    std::unique_ptr<Expression> call();

    std::unique_ptr<Expression> finishCall(std::unique_ptr<Expression> callee);

    std::unique_ptr<Expression> primary();

    // Error handling
    void synchronize();

    void error(const Token &token, const std::string &message);
};

#endif // PARSER_H
