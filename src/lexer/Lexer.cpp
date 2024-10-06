#include "Lexer.h"

#include <iostream>
#include <unordered_map>

using namespace std;

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    try {
        while (true) {
            Token token = nextToken();
            tokens.push_back(token);
            if (token.type == TokenType::EOF_TOKEN) {
                break;
            }
        }
    } catch (const runtime_error &e) {
        cerr << "Lexer error: " << e.what() << endl;
        // Handle lexer error as appropriate
    }
    return tokens;
}


Token Lexer::nextToken() {
    skipWhitespaceAndComments();
    if (isAtEnd()) {
        return Token{TokenType::EOF_TOKEN, "EndOfFile", line, column};
    }
    Token token;

    char c = source[position];

    if (isIdentifierStart(c)) {
        token = identifierOrKeyword(c);
    } else if (isNumberLiteral(c)) {
        token = numberLiteral(c);
    } else if (c == '\'' || c == '"') {
        token = stringLiteral(c);
    } else if (isOperatorStart(c)) {
        token = operatorToken(c);
    } else if (isSeparator(c)) {
        token = separatorToken(c);
    } else {
        // Handle unknown character
        error(string("Unexpected character: ") + c);
        token = Token{TokenType::UNKNOWN, string(1, c), line, column};
    }
    advance();
    return token;
}

char Lexer::peek() const {
    return source[position];
}

char Lexer::peekNext() {
    return source[position + 1];
}

char Lexer::advance() {
    column++;
    position++;
    return source[position];
}

bool Lexer::isAtEnd() {
    return position >= source.size();
}

bool Lexer::match(char expected) {
    if (isAtEnd()) {
        return false;
    }
    if (source[position] != expected) {
        return false;
    }

    advance();
    return true;
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();
        if (isspace(c)) {
            advance();
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        } else if (c == '/') {
            if (peekNext() == '/') {
                // Single-line comment
                while (!isAtEnd() && peek() != '\n') {
                    advance();
                }
            } else if (peekNext() == '*') {
                // Multi-line comment
                advance(); // Consume '/'
                advance(); // Consume '*'
                while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
                    if (peek() == '\n') {
                        line++;
                        column = 1;
                    }
                    advance();
                }
                // Consume '*/'
                if (!isAtEnd()) {
                    advance(); // Consume '*'
                    advance(); // Consume '/'
                }
            } else {
                break; // Not a comment
            }
        } else {
            break; // Not whitespace or comment
        }
    }
}

Token Lexer::identifierOrKeyword(char firstChar) {
    string lexeme(1, firstChar);
    while (!isAtEnd() && (isalnum(peekNext()) || peekNext() == '_')) {
        lexeme += advance();
    }

    TokenType type = TokenType::IDENTIFIER;

    // Check if lexeme is a keyword
    auto it = keywordMap.find(lexeme);
    if (it != keywordMap.end()) {
        type = it->second;
    }

    if (lexeme == "let") {
        return Token{TokenType::LET, lexeme, line, column - static_cast<int>(lexeme.length()) + 1};
    } else if (lexeme == "const") {
        return Token{TokenType::CONST, lexeme, line, column - static_cast<int>(lexeme.length()) + 1};
    } else if (lexeme == "var") {
        return Token{TokenType::VAR, lexeme, line, column - static_cast<int>(lexeme.length()) + 1};
    } else {
        return Token{type, lexeme, line, column - static_cast<int>(lexeme.length()) + 1};
    }
}

bool Lexer::isIdentifierStart(char firstChar) {
    return isalpha(firstChar) || firstChar == '_';
}

Token Lexer::numberLiteral(char firstChar) {
    string lexeme(1, firstChar);
    bool isFloat = false;

    while (!isAtEnd() && isdigit(peekNext())) {
        lexeme += advance();
    }

    if (peekNext() == '.' && isdigit(peek())) {
        isFloat = true;
        lexeme += advance(); // Consume '.'

        while (!isAtEnd() && isdigit(peekNext())) {
            lexeme += advance();
        }
    }

    // Handle exponential notation
    if (peek() == 'e' || peek() == 'E') {
        isFloat = true;
        lexeme += advance(); // Consume 'e' or 'E'

        if (peek() == '+' || peek() == '-') {
            lexeme += advance();
        }

        while (!isAtEnd() && isdigit(peek())) {
            lexeme += advance();
        }
    }

    TokenType type = isFloat ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL;

    return Token{type, lexeme, line, column - static_cast<int>(lexeme.length()) + 1};
}

bool Lexer::isNumberLiteral(char firstChar) {
    return isdigit(firstChar);
}

Token Lexer::stringLiteral(char quoteType) {
    string lexeme;
    int startColumn = column;

    while (!isAtEnd() && peekNext() != quoteType) {
        if (peek() == '\\') {
            advance(); // Consume '\'
            if (!isAtEnd()) {
                char escapedChar = advance();
                lexeme += '\\';
                lexeme += escapedChar;
                column += 2;
            }
        } else {
            lexeme += advance();
        }
    }

    if (isAtEnd()) {
        error("Unterminated string literal.");
        return Token{TokenType::UNKNOWN, lexeme, line, startColumn};
    }

    advance(); // Consume closing quote

    return Token{TokenType::STRING_LITERAL, lexeme, line, startColumn};
}


Token Lexer::operatorToken(char firstChar) {
    string lexeme(1, firstChar);
    int startColumn = column;

    // Check for multi-character operators
    char nextChar = peekNext();
    string twoCharOp = lexeme + nextChar;

    if (operatorMap.find(twoCharOp) != operatorMap.end()) {
        advance(); // Consume nextChar
        lexeme = twoCharOp;
    } else if (operatorMap.find(lexeme) == operatorMap.end()) {
        error("Unknown operator.");
        return Token{TokenType::UNKNOWN, lexeme, line, startColumn};
    }

    TokenType type = operatorMap[lexeme];
    return Token{type, lexeme, line, startColumn};
}

bool Lexer::isOperatorStart(char firstChar) {
    switch (firstChar) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case '!':
        case '<':
        case '>':
        case '&':
        case '|':
        case '^':
        case '~':
            return true;
        default:
            return false;
    }
}

Token Lexer::separatorToken(char c) {
    TokenType type;

    switch (c) {
        case '(': type = TokenType::LEFT_PAREN;
            break;
        case ')': type = TokenType::RIGHT_PAREN;
            break;
        case '{': type = TokenType::LEFT_BRACE;
            break;
        case '}': type = TokenType::RIGHT_BRACE;
            break;
        case '[': type = TokenType::LEFT_BRACKET;
            break;
        case ']': type = TokenType::RIGHT_BRACKET;
            break;
        case ';': type = TokenType::SEMICOLON;
            break;
        case ',': type = TokenType::COMMA;
            break;
        case '.': type = TokenType::DOT;
            break;
        case ':': type = TokenType::COLON;
            break;
        case '?': type = TokenType::QUESTION_MARK;
            break;
        default:
            error("Unknown separator.");
            return Token{TokenType::UNKNOWN, string(1, c), line, column};
    }

    return Token{type, string(1, c), line, column};
}

bool Lexer::isSeparator(char c) {
    switch (c) {
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
        case ',':
        case ';':
        case ':':
        case '.':
        case '?':
            return true;
        default:
            return false;
    }
}

void Lexer::error(const string &message) {
    throw runtime_error("Lexer error at line " + to_string(line) + ", column " + to_string(column) + ": " + message);
}
