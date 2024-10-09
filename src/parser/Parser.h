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
    Parser(const vector<Token> &tokens);

    // Parses the entire input and returns a list of statements
    vector<unique_ptr<Statement> > parse();

private:
    const vector<Token> &tokens_;
    size_t current_;

    // Helper methods
    bool isAtEnd() const;

    const Token &peek() const;

    const Token &previous() const;

    const Token &advance();

    bool check(TokenType type) const;

    bool match(const vector<TokenType> &types);

    Token consume(TokenType type, const string &errorMessage);

    // Parsing methods
    unique_ptr<Statement> declaration();

    unique_ptr<Statement> variableDeclaration();

    unique_ptr<Statement> functionDeclaration();

    unique_ptr<Statement> statement();

    unique_ptr<Statement> ifStatement();

    unique_ptr<Statement> whileStatement();

    unique_ptr<Statement> forStatement();

    unique_ptr<Statement> returnStatement();

    unique_ptr<Statement> blockStatement();

    unique_ptr<Statement> expressionStatement();

    unique_ptr<Expression> expression();

    unique_ptr<Expression> assignment();

    unique_ptr<Expression> logicalOr();

    unique_ptr<Expression> logicalAnd();

    unique_ptr<Expression> equality();

    unique_ptr<Expression> comparison();

    unique_ptr<Expression> term();

    unique_ptr<Expression> factor();

    unique_ptr<Expression> unary();

    unique_ptr<Expression> call();

    unique_ptr<Expression> finishCall(unique_ptr<Expression> callee);

    unique_ptr<Expression> primary();

    // Error handling
    void synchronize();

    void error(const Token &token, const string &message);
};

#endif // PARSER_H
