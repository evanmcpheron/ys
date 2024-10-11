//
// Created by evan.mcpheron on 10/1/24.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

using namespace std;


enum class TokenType {
    // Keywords
    IF, ELSE, SWITCH, CASE, DEFAULT, FOR, WHILE, DO, BREAK, CONTINUE, RETURN, TRY, CATCH, FINALLY, THROW, NEW, DELETE,

    // Identifiers and Literals
    IDENTIFIER, DOUBLE_LITERAL, STRING_LITERAL, BOOLEAN_LITERAL, NULL_LITERAL, FUNCTION, CLASS, INTERFACE, EXTENDS,
    IMPLEMENTS, OBJECT, ARRAY, MAP, SET, ENUM,
    PACKAGE, IMPORT, EXPORT, AS, FROM, LET, CONST, VAR, STATIC,

    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO, PLUS_PLUS, MINUS_MINUS,
    ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN,
    EQUAL, NOT_EQUAL, STRICT_EQUAL, STRICT_NOT_EQUAL,
    LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL,
    LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT,
    BITWISE_AND, BITWISE_OR, BITWISE_XOR, BITWISE_NOT, LEFT_SHIFT, RIGHT_SHIFT, UNSIGNED_RIGHT_SHIFT,

    // Separators
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET,
    SEMICOLON, COMMA, DOT, COLON, DOUBLE_COLON, QUESTION_MARK,

    // Comments
    COMMENT,

    // End of File
    EOF_TOKEN,

    // Others
    UNKNOWN,
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};


#endif //TOKEN_H
