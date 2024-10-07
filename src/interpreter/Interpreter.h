#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>
#include <vector>
#include "../visitor/Visitor.h"

class Environment;
class Value;
class FunctionDeclaration;
class ReturnStatement;
class WhileStatement;
class IfStatement;
class BlockStatement;
class VariableDeclaration;
class ExpressionStatement;
class GetExpression;
class FunctionCallExpression;
class LogicalExpression;
class AssignmentExpression;
class UnaryExpression;
class BinaryExpression;
class IdentifierExpression;
class LiteralExpression;
class Statement;

class Interpreter : public Visitor {
public:
    virtual ~Interpreter();

    // Executes a list of statements (the program)
    void interpret(const vector<unique_ptr<Statement> > &statements);

    //
    // Visitor methods for expressions
    Value visitLiteralExpression(LiteralExpression *expression) override;

    Value visitIdentifierExpression(IdentifierExpression *expression) override;

    Value visitBinaryExpression(BinaryExpression *expression) override;

    Value visitUnaryExpression(UnaryExpression *expression) override;

    Value visitAssignmentExpression(AssignmentExpression *expression) override;

    Value visitLogicalExpression(LogicalExpression *expression) override;

    Value visitFunctionCallExpression(FunctionCallExpression *expression) override;

    Value visitGetExpression(GetExpression *expression) override;

    // Visitor methods for statements
    void visitExpressionStatement(ExpressionStatement *statement) override;

    void visitVariableDeclaration(VariableDeclaration *statement) override;

    void visitBlockStatement(BlockStatement *statement) override;

    void visitIfStatement(IfStatement *statement) override;

    void visitWhileStatement(WhileStatement *statement) override;

    void visitReturnStatement(ReturnStatement *statement) override;

    void visitFunctionDeclaration(FunctionDeclaration *statement) override;

    // Exception class for returning from functions
    class ReturnException : public exception {
    public:
        ReturnException(const Value &value);

        Value value;
    };

private:
    // The environment representing the current scope
    shared_ptr<Environment> environment_;

    Value lastValue = Value();

    // Helper methods
    void executeBlock(const vector<unique_ptr<Statement> > &statements, shared_ptr<Environment> newEnvironment);

    bool isTruthy(const Value &value);

    void setLastValue(const Value &value);
};

#endif // INTERPRETER_H
