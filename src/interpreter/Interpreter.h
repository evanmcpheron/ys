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
    Interpreter();

    // Executes a list of statements (the program)
    void interpret(const vector<unique_ptr<Statement> > &statements);

    //
    // Visitor methods for expressions
    std::shared_ptr<Value> visitLiteralExpression(LiteralExpression *expression) override;

    std::shared_ptr<Value> visitIdentifierExpression(IdentifierExpression *expression) override;

    std::shared_ptr<Value> visitBinaryExpression(BinaryExpression *expression) override;

    std::shared_ptr<Value> visitUnaryExpression(UnaryExpression *expression) override;

    std::shared_ptr<Value> visitAssignmentExpression(AssignmentExpression *expression) override;

    std::shared_ptr<Value> visitLogicalExpression(LogicalExpression *expression) override;

    std::shared_ptr<Value> visitFunctionCallExpression(FunctionCallExpression *expression) override;

    std::shared_ptr<Value> visitGetExpression(GetExpression *expression) override;

    // Visitor methods for statements
    void visitExpressionStatement(ExpressionStatement *statement) override;

    void visitVariableDeclaration(VariableDeclaration *statement) override;

    void visitBlockStatement(BlockStatement *statement) override;

    void visitIfStatement(IfStatement *statement) override;

    void visitWhileStatement(WhileStatement *statement) override;

    void visitReturnStatement(ReturnStatement *statement) override;

    void visitFunctionDeclaration(FunctionDeclaration *statement) override;

    void ReturnException(const Value &value);

    // Exception class for returning from functions
    class ReturnException : public exception {
    public:
        ReturnException(const Value &value);

        Value value;
    };

private:
    // The environment representing the current scope
    shared_ptr<Environment> environment_;

    std::shared_ptr<Value> lastValue = make_unique<Value>(Value());

    // Helper methods
    static void executeBlock(const vector<unique_ptr<Statement> > &statements,
                             const shared_ptr<Environment> &newEnvironment);

    static bool isTruthy(const shared_ptr<Value> &value);

    void setLastValue(const shared_ptr<Value> &value);

    void registerBuiltIns() const;
};

#endif // INTERPRETER_H
