#include "Interpreter.h"

#include <iostream>

#include "../environment/Environment.h"

using namespace std;


Interpreter::~Interpreter() {
}

void Interpreter::interpret(const vector<unique_ptr<Statement> > &statements) {
    try {
        for (const auto &statement: statements) {
            statement->accept(*this);
        }
    } catch (const exception &e) {
        cerr << "Runtime error: " << e.what() << endl;
    }
}

Value Interpreter::visitLiteralExpression(LiteralExpression *expression) {
}

Value Interpreter::visitIdentifierExpression(IdentifierExpression *expression) {
    return {};
}

Value Interpreter::visitBinaryExpression(BinaryExpression *expression) {
    return {};
}

Value Interpreter::visitUnaryExpression(UnaryExpression *expression) {
    return {};
}

Value Interpreter::visitAssignmentExpression(AssignmentExpression *expression) {
    return {};
}

Value Interpreter::visitLogicalExpression(LogicalExpression *expression) {
    return {};
}

Value Interpreter::visitFunctionCallExpression(FunctionCallExpression *expression) {
    return {};
}

Value Interpreter::visitGetExpression(GetExpression *expression) {
    return {};
}

void Interpreter::visitExpressionStatement(ExpressionStatement *statement) {
}

void Interpreter::visitVariableDeclaration(VariableDeclaration *statement) {
}

void Interpreter::visitBlockStatement(BlockStatement *statement) {
}

void Interpreter::visitIfStatement(IfStatement *statement) {
}

void Interpreter::visitWhileStatement(WhileStatement *statement) {
}

void Interpreter::visitReturnStatement(ReturnStatement *statement) {
}

void Interpreter::visitFunctionDeclaration(FunctionDeclaration *statement) {
}

Interpreter::ReturnException::ReturnException(const Value &value) {
}

void Interpreter::executeBlock(const std::vector<std::unique_ptr<Statement> > &statements, std::shared_ptr<Environment> newEnvironment) {
}

bool Interpreter::isTruthy(const Value &value) {
    return true;
}
