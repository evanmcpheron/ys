#include "Parser.h"
#include <iostream>

Parser::Parser(const vector<Token> &tokens)
    : tokens_(tokens), current_(0) {
}

vector<unique_ptr<Statement> > Parser::parse() {
    vector<unique_ptr<Statement> > statements;
    while (!isAtEnd()) {
        auto decl = declaration();
        if (decl) {
            statements.push_back(move(decl));
        }
    }
    return statements;
}

// Helper Methods

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOF_TOKEN;
}

const Token &Parser::peek() const {
    return tokens_[current_];
}

const Token &Parser::previous() const {
    return tokens_[current_ - 1];
}

const Token &Parser::advance() {
    if (!isAtEnd()) current_++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const vector<TokenType> &types) {
    for (TokenType type: types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const string &errorMessage) {
    if (check(type)) return advance();
    error(peek(), errorMessage);
    throw runtime_error("Parsing Error"); // Exception handling
}

// Parsing Methods

unique_ptr<Statement> Parser::declaration() {
    try {
        if (match({TokenType::VAR, TokenType::LET, TokenType::CONST})) {
            return variableDeclaration();
        } else if (match({TokenType::FUNCTION})) {
            return functionDeclaration();
        } else {
            return statement();
        }
    } catch (const runtime_error &) {
        synchronize();
        return nullptr;
    }
}

unique_ptr<Statement> Parser::variableDeclaration() {
    TokenType varType = previous().type; // VAR, LET, or CONST
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");

    string typeName = "var"; // Default type (can be adjusted as needed)
    if (match({TokenType::COLON})) {
        Token typeToken = consume(TokenType::IDENTIFIER, "Expected type name.");
        typeName = typeToken.value;
    }

    unique_ptr<Expression> initializer;
    if (match({TokenType::ASSIGN})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return make_unique<VariableDeclaration>(name.value, typeName, move(initializer));
}

unique_ptr<Statement> Parser::functionDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected function name.");

    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");

    vector<FunctionDeclaration::Parameter> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token paramName = consume(TokenType::IDENTIFIER, "Expected parameter name.");
            string paramTypeName = "var"; // Default parameter type
            if (match({TokenType::COLON})) {
                Token paramType = consume(TokenType::IDENTIFIER, "Expected parameter type.");
                paramTypeName = paramType.value;
            }
            parameters.push_back({paramName.value, paramTypeName});
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

    string returnTypeName = "void";
    if (match({TokenType::COLON})) {
        Token returnType = consume(TokenType::IDENTIFIER, "Expected return type.");
        returnTypeName = returnType.value;
    }

    consume(TokenType::LEFT_BRACE, "Expected '{' before function body.");

    auto body = unique_ptr<BlockStatement>(static_cast<BlockStatement *>(blockStatement().release()));

    return make_unique<FunctionDeclaration>(name.value, parameters, returnTypeName, move(body));
}

unique_ptr<Statement> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::LEFT_BRACE})) return blockStatement();
    return expressionStatement();
}

unique_ptr<Statement> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");

    auto thenBranch = statement();
    unique_ptr<Statement> elseBranch = nullptr;

    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return make_unique<IfStatement>(move(condition), move(thenBranch), move(elseBranch));
}

unique_ptr<Statement> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");

    auto body = statement();

    return make_unique<WhileStatement>(move(condition), move(body));
}

unique_ptr<Statement> Parser::forStatement() {
    // For simplicity, we'll desugar 'for' loops into 'while' loops
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

    // Initializer
    unique_ptr<Statement> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::VAR, TokenType::LET, TokenType::CONST})) {
        initializer = variableDeclaration();
    } else {
        initializer = expressionStatement();
    }

    // Condition
    unique_ptr<Expression> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

    // Increment
    unique_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after for clauses.");

    // Body
    auto body = statement();

    // Desugar to while loop
    if (increment) {
        auto bodyStatements = vector<unique_ptr<Statement> >();
        bodyStatements.push_back(move(body));
        bodyStatements.push_back(make_unique<ExpressionStatement>(move(increment)));
        body = make_unique<BlockStatement>(move(bodyStatements));
    }

    if (!condition) {
        condition = make_unique<LiteralExpression>(LiteralExpression::Type::Boolean, "true");
    }

    body = make_unique<WhileStatement>(move(condition), move(body));

    if (initializer) {
        auto statements = vector<unique_ptr<Statement> >();
        statements.push_back(move(initializer));
        statements.push_back(move(body));
        return make_unique<BlockStatement>(move(statements));
    } else {
        return body;
    }
}

unique_ptr<Statement> Parser::returnStatement() {
    Token keyword = previous();
    unique_ptr<Expression> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return make_unique<ReturnStatement>(move(value));
}

unique_ptr<Statement> Parser::blockStatement() {
    vector<unique_ptr<Statement> > statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return make_unique<BlockStatement>(move(statements));
}

unique_ptr<Statement> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return make_unique<ExpressionStatement>(move(expr));
}

unique_ptr<Expression> Parser::expression() {
    return assignment();
}

unique_ptr<Expression> Parser::assignment() {
    auto expr = logicalOr();

    if (match({
        TokenType::ASSIGN, TokenType::PLUS_ASSIGN, TokenType::MINUS_ASSIGN, TokenType::MULTIPLY_ASSIGN, TokenType::DIVIDE_ASSIGN,
        TokenType::MODULO_ASSIGN
    })) {
        Token op = previous();
        auto value = assignment();

        if (auto varExpr = dynamic_cast<IdentifierExpression *>(expr.get())) {
            string name = varExpr->getName();
            return make_unique<AssignmentExpression>(name, move(value), op.type);
        } else {
            error(op, "Invalid assignment target.");
        }
    }

    return expr;
}

unique_ptr<Expression> Parser::logicalOr() {
    auto expr = logicalAnd();

    while (match({TokenType::LOGICAL_OR})) {
        Token op = previous();
        auto right = logicalAnd();
        expr = make_unique<LogicalExpression>(move(expr), LogicalExpression::Operator::Or, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::logicalAnd() {
    auto expr = equality();

    while (match({TokenType::LOGICAL_AND})) {
        Token op = previous();
        auto right = equality();
        expr = make_unique<LogicalExpression>(move(expr), LogicalExpression::Operator::And, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::equality() {
    auto expr = comparison();

    while (match({TokenType::EQUAL, TokenType::NOT_EQUAL, TokenType::STRICT_EQUAL, TokenType::STRICT_NOT_EQUAL})) {
        Token op = previous();
        auto right = comparison();
        BinaryExpression::Operator binaryOp;
        switch (op.type) {
            case TokenType::EQUAL:
                binaryOp = BinaryExpression::Operator::Equal;
                break;
            case TokenType::NOT_EQUAL:
                binaryOp = BinaryExpression::Operator::NotEqual;
                break;
            case TokenType::STRICT_EQUAL:
                binaryOp = BinaryExpression::Operator::StrictEqual;
                break;
            case TokenType::STRICT_NOT_EQUAL:
                binaryOp = BinaryExpression::Operator::StrictNotEqual;
                break;
            default:
                binaryOp = BinaryExpression::Operator::Unknown;
                break;
        }
        expr = make_unique<BinaryExpression>(move(expr), binaryOp, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::comparison() {
    auto expr = term();

    while (match({TokenType::LESS_THAN, TokenType::LESS_EQUAL, TokenType::GREATER_THAN, TokenType::GREATER_EQUAL})) {
        Token op = previous();
        auto right = term();
        BinaryExpression::Operator binaryOp;
        switch (op.type) {
            case TokenType::LESS_THAN:
                binaryOp = BinaryExpression::Operator::Less;
                break;
            case TokenType::LESS_EQUAL:
                binaryOp = BinaryExpression::Operator::LessEqual;
                break;
            case TokenType::GREATER_THAN:
                binaryOp = BinaryExpression::Operator::Greater;
                break;
            case TokenType::GREATER_EQUAL:
                binaryOp = BinaryExpression::Operator::GreaterEqual;
                break;
            default:
                binaryOp = BinaryExpression::Operator::Unknown;
                break;
        }
        expr = make_unique<BinaryExpression>(move(expr), binaryOp, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::term() {
    auto expr = factor();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = factor();
        BinaryExpression::Operator binaryOp = (op.type == TokenType::PLUS) ? BinaryExpression::Operator::Add : BinaryExpression::Operator::Subtract;
        expr = make_unique<BinaryExpression>(move(expr), binaryOp, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::factor() {
    auto expr = unary();

    while (match({TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO})) {
        Token op = previous();
        auto right = unary();
        BinaryExpression::Operator binaryOp;
        switch (op.type) {
            case TokenType::MULTIPLY:
                binaryOp = BinaryExpression::Operator::Multiply;
                break;
            case TokenType::DIVIDE:
                binaryOp = BinaryExpression::Operator::Divide;
                break;
            case TokenType::MODULO:
                binaryOp = BinaryExpression::Operator::Modulo;
                break;
            default:
                binaryOp = BinaryExpression::Operator::Unknown;
                break;
        }
        expr = make_unique<BinaryExpression>(move(expr), binaryOp, move(right));
    }

    return expr;
}

unique_ptr<Expression> Parser::unary() {
    if (match({TokenType::LOGICAL_NOT, TokenType::MINUS})) {
        Token op = previous();
        auto right = unary();
        UnaryExpression::Operator unaryOp = (op.type == TokenType::LOGICAL_NOT) ? UnaryExpression::Operator::Not : UnaryExpression::Operator::Negate;
        return make_unique<UnaryExpression>(unaryOp, move(right));
    }

    return call();
}

unique_ptr<Expression> Parser::call() {
    auto expr = primary();

    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(move(expr));
        } else if (match({TokenType::DOT})) {
            Token name = consume(TokenType::IDENTIFIER, "Expected property name after '.'.");
            expr = make_unique<GetExpression>(move(expr), name.value);
        } else {
            break;
        }
    }

    return expr;
}

unique_ptr<Expression> Parser::finishCall(unique_ptr<Expression> callee) {
    vector<unique_ptr<Expression> > arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
    return make_unique<FunctionCallExpression>(move(callee), move(arguments));
}

unique_ptr<Expression> Parser::primary() {
    if (match({TokenType::BOOLEAN_LITERAL})) {
        return make_unique<LiteralExpression>(LiteralExpression::Type::Boolean, previous().value);
    }
    if (match({TokenType::NULL_LITERAL})) {
        return make_unique<LiteralExpression>(LiteralExpression::Type::Null, "null");
    }
    if (match({TokenType::INTEGER_LITERAL})) {
        return make_unique<LiteralExpression>(LiteralExpression::Type::Integer, previous().value);
    }
    if (match({TokenType::FLOAT_LITERAL})) {
        return make_unique<LiteralExpression>(LiteralExpression::Type::Float, previous().value);
    }
    if (match({TokenType::STRING_LITERAL})) {
        return make_unique<LiteralExpression>(LiteralExpression::Type::String, previous().value);
    }
    if (match({TokenType::IDENTIFIER})) {
        return make_unique<IdentifierExpression>(previous().value);
    }
    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return expr;
    }

    error(peek(), "Expected expression.");
    throw runtime_error("Parsing Error");
}

// Error Handling

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUNCTION:
            case TokenType::VAR:
            case TokenType::LET:
            case TokenType::CONST:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

void Parser::error(const Token &token, const string &message) {
    cerr << "[Line " << token.line << ", Column " << token.column << "] Error at '" << token.value << "': " << message << endl;
}
