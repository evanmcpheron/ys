#ifndef VISITOR_H
#define VISITOR_H

#include "../ast/AST.h"
#include "../value/Value.h"

// Forward declaration
class Value;
class LiteralExpression;
class IdentifierExpression;
class BinaryExpression;
class UnaryExpression;
class AssignmentExpression;
class LogicalExpression;
class FunctionCallExpression;
class GetExpression;
class ExpressionStatement;
class VariableDeclaration;
class BlockStatement;
class IfStatement;
class WhileStatement;
class ReturnStatement;
class FunctionDeclaration;

class Visitor {
public:
    virtual ~Visitor() = default;

    // Expression visitors
    virtual shared_ptr<Value> visitLiteralExpression(LiteralExpression *expr) = 0;

    virtual shared_ptr<Value> visitIdentifierExpression(IdentifierExpression *expr) = 0;

    virtual shared_ptr<Value> visitBinaryExpression(BinaryExpression *expr) = 0;

    virtual shared_ptr<Value> visitUnaryExpression(UnaryExpression *expr) = 0;

    virtual shared_ptr<Value> visitAssignmentExpression(AssignmentExpression *expr) = 0;

    virtual shared_ptr<Value> visitLogicalExpression(LogicalExpression *expr) = 0;

    virtual shared_ptr<Value> visitFunctionCallExpression(FunctionCallExpression *expr) = 0;

    virtual shared_ptr<Value> visitGetExpression(GetExpression *expr) = 0;

    // Statement visitors
    virtual void visitExpressionStatement(ExpressionStatement *stmt) = 0;

    virtual void visitVariableDeclaration(VariableDeclaration *stmt) = 0;

    virtual void visitBlockStatement(BlockStatement *stmt) = 0;

    virtual void visitIfStatement(IfStatement *stmt) = 0;

    virtual void visitWhileStatement(WhileStatement *stmt) = 0;

    virtual void visitReturnStatement(ReturnStatement *stmt) = 0;

    virtual void visitFunctionDeclaration(FunctionDeclaration *stmt) = 0;
};

#endif // VISITOR_H
