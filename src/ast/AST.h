#ifndef AST_H
#define AST_H

#include <string>
#include <utility>
#include <vector>
#include <memory>

#include "../visitor/Visitor.h"
#include "../../include/Token.h"


using namespace std;
class ValueType;
class Visitor;

// Forward declaration for Visitor

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void accept(Visitor &visitor) = 0;
};

// ********************
// Expression Classes
// ********************

class Expression : public ASTNode {
};

class LiteralExpression : public Expression {
public:
    LiteralExpression(const TokenType type, shared_ptr<ValueType> value)
        : type_(type), value_(move(value)) {
    }


    void accept(Visitor &visitor) override;

    [[nodiscard]] TokenType getType() const;

    [[nodiscard]] shared_ptr<ValueType> getValue() const {
        return value_;
    }

private:
    TokenType type_;
    shared_ptr<ValueType> value_;
};

// Identifier expressions
class IdentifierExpression final : public Expression {
public:
    explicit IdentifierExpression(string name);

    void accept(Visitor &visitor) override;

    [[nodiscard]] const string &getName() const;

private:
    string name_;
};

// Binary expressions
class BinaryExpression : public Expression {
public:
    enum class Operator {
        ADD, SUBTRACT, MULTIPLY, DIVIDE, MODULO,
        EQUAL, NOT_EQUAL, STRICT_EQUAL, STRICT_NOT_EQUAL,
        LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
        LOGICAL_AND, LOGICAL_OR,
        UNKNOWN
    };

    BinaryExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getLeft() const;

    [[nodiscard]] Operator getOperator() const;

    [[nodiscard]] Expression *getRight() const;

private:
    unique_ptr<Expression> left_;
    Operator op_;
    unique_ptr<Expression> right_;
};

// Unary expressions
class UnaryExpression : public Expression {
public:
    enum class Operator {
        Negate, // -
        Not // !
    };

    UnaryExpression(Operator op, unique_ptr<Expression> right);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Operator getOperator() const;

    [[nodiscard]] Expression *getRight() const;

private:
    Operator op_;
    unique_ptr<Expression> right_;
};

// Assignment expressions
class AssignmentExpression : public Expression {
public:
    AssignmentExpression(string name, unique_ptr<Expression> value, TokenType op);

    void accept(Visitor &visitor) override;

    [[nodiscard]] const string &getName() const;

    [[nodiscard]] Expression *getValue() const;

    [[nodiscard]] TokenType getOperator() const;

private:
    string name_;
    unique_ptr<Expression> value_;
    TokenType op_;
};

// Logical expressions
class LogicalExpression final : public Expression {
public:
    enum class Operator {
        And,
        Or
    };

    LogicalExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getLeft() const;

    [[nodiscard]] Operator getOperator() const;

    [[nodiscard]] Expression *getRight() const;

private:
    unique_ptr<Expression> left_;
    Operator op_;
    unique_ptr<Expression> right_;
};

// Function call expressions
class FunctionCallExpression : public Expression {
public:
    FunctionCallExpression(unique_ptr<Expression> callee, vector<unique_ptr<Expression> > arguments);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getCallee() const;

    [[nodiscard]] const vector<unique_ptr<Expression> > &getArguments() const;

private:
    unique_ptr<Expression> callee_;
    vector<unique_ptr<Expression> > arguments_;
};

// Get expressions (object property access)
class GetExpression : public Expression {
public:
    GetExpression(unique_ptr<Expression> object, string name);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getObject() const;

    [[nodiscard]] const basic_string<char> &getName() const;

private:
    unique_ptr<Expression> object_;
    string name_;
};

// ********************
// Statement Classes
// ********************

class Statement : public ASTNode {
};

// Expression statements
class ExpressionStatement : public Statement {
public:
    explicit ExpressionStatement(unique_ptr<Expression> expression);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getExpression() const;

private:
    unique_ptr<Expression> expression_;
};

// Variable declarations
class VariableDeclaration : public Statement {
public:
    VariableDeclaration(string name, string typeName, unique_ptr<Expression> initializer);

    void accept(Visitor &visitor) override;

    [[nodiscard]] const string &getName() const;

    [[nodiscard]] const string &getTypeName() const;

    [[nodiscard]] Expression *getInitializer() const;

    // Method to check if an initializer was provided
    [[nodiscard]] bool hasInitializer() const {
        return initializer_ != nullptr;
    }

private:
    string name_;
    string typeName_;
    unique_ptr<Expression> initializer_;
};

// Block statements
class BlockStatement : public Statement {
public:
    explicit BlockStatement(vector<unique_ptr<Statement> > statements);

    void accept(Visitor &visitor) override;

    [[nodiscard]] const vector<unique_ptr<Statement> > &getStatements() const;

private:
    vector<unique_ptr<Statement> > statements_;
};

// If statements
class IfStatement : public Statement {
public:
    IfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenBranch, unique_ptr<Statement> elseBranch);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getCondition() const;

    [[nodiscard]] Statement *getThenBranch() const;

    [[nodiscard]] Statement *getElseBranch() const;

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Statement> thenBranch_;
    unique_ptr<Statement> elseBranch_;
};

// While statements
class WhileStatement final : public Statement {
public:
    WhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getCondition() const;

    [[nodiscard]] auto getBody() const -> Statement *;

private:
    unique_ptr<Expression> condition_;
    unique_ptr<Statement> body_;
};

// Return statements
class ReturnStatement : public Statement {
public:
    explicit ReturnStatement(unique_ptr<Expression> value);

    void accept(Visitor &visitor) override;

    [[nodiscard]] Expression *getValue() const;

private:
    unique_ptr<Expression> value_;
};

// Function declarations
class FunctionDeclaration final : public Statement {
public:
    struct Parameter {
        string name;
        string typeName;
    };

    FunctionDeclaration(string name, vector<Parameter> parameters, string returnTypeName,
                        unique_ptr<BlockStatement> body);

    void accept(Visitor &visitor) override;

    [[nodiscard]] const string &getName() const;

    [[nodiscard]] const vector<Parameter> &getParameters() const;

    [[nodiscard]] const string &getReturnTypeName() const;

    [[nodiscard]] BlockStatement *getBody() const;

private:
    string name_;
    vector<Parameter> parameters_;
    string returnTypeName_;
    unique_ptr<BlockStatement> body_;
};

#endif // AST_H
