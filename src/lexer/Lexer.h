#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../include/Token.h"

using namespace std;

class Lexer {
public:
    explicit Lexer(string source)
        : source(move(source)), position(0), line(1), column(1) {
    }

    Token nextToken();


    vector<Token> tokenize();

private:
    // Source code
    string source;
    size_t position;

    // Current position
    int line;
    int column;

    // Helper functions
    char peek() const;

    char peekNext();

    char advance();

    bool isAtEnd();

    bool match(char expected);

    void skipWhitespaceAndComments();

    // Token recognition functions
    Token identifierOrKeyword(char firstChar);

    bool isIdentifierStart(char firstChar);

    Token numberLiteral(char firstChar);

    bool isNumberLiteral(char firstChar);

    Token stringLiteral(char quoteType);

    Token operatorToken(char firstChar);

    bool isOperatorStart(char firstChar);

    Token separatorToken(char c);

    bool isSeparator(char c);

    // Error handling
    void error(const string &message);

    unordered_map<string, TokenType> operatorMap = {
        // Basic Arithmetic Operators
        {"+", TokenType::PLUS}, {"-", TokenType::MINUS}, {"*", TokenType::MULTIPLY}, {"/", TokenType::DIVIDE}, {"%", TokenType::MODULO},

        // Increment and Decrement Operators
        {"++", TokenType::PLUS_PLUS}, {"--", TokenType::MINUS_MINUS},

        // Assignment Operators
        {"=", TokenType::ASSIGN}, {"+=", TokenType::PLUS_ASSIGN}, {"-=", TokenType::MINUS_ASSIGN}, {"*=", TokenType::MULTIPLY_ASSIGN},
        {"/=", TokenType::DIVIDE_ASSIGN}, {"%=", TokenType::MODULO_ASSIGN},

        // Equality and Inequality Operators
        {"==", TokenType::EQUAL}, {"!=", TokenType::NOT_EQUAL}, {"===", TokenType::STRICT_EQUAL}, {"!==", TokenType::STRICT_NOT_EQUAL},

        // Comparison Operators
        {"<", TokenType::LESS_THAN}, {">", TokenType::GREATER_THAN}, {"<=", TokenType::LESS_EQUAL}, {">=", TokenType::GREATER_EQUAL},

        // Logical Operators
        {"&&", TokenType::LOGICAL_AND}, {"||", TokenType::LOGICAL_OR}, {"!", TokenType::LOGICAL_NOT},

        // Bitwise Operators
        {"&", TokenType::BITWISE_AND}, {"|", TokenType::BITWISE_OR}, {"^", TokenType::BITWISE_XOR}, {"~", TokenType::BITWISE_NOT},
        {"<<", TokenType::LEFT_SHIFT}, {">>", TokenType::RIGHT_SHIFT}, {">>>", TokenType::UNSIGNED_RIGHT_SHIFT},
    };

    unordered_map<string, TokenType> keywordMap = {
        {"if", TokenType::IF}, {"else", TokenType::ELSE}, {"switch", TokenType::SWITCH}, {"case", TokenType::CASE}, {"default", TokenType::DEFAULT},
        {"for", TokenType::FOR}, {"while", TokenType::WHILE}, {"do", TokenType::DO}, {"break", TokenType::BREAK}, {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN}, {"try", TokenType::TRY}, {"catch", TokenType::CATCH}, {"finally", TokenType::FINALLY},
        {"throw", TokenType::THROW}, {"new", TokenType::NEW}, {"delete", TokenType::DELETE},
    };
};
