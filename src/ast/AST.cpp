#include "AST.h"

#include <iostream>
#include <utility>
#include "../visitor/Visitor.h"

// ********************
// LiteralExpression
// ********************


std::shared_ptr<Value> LiteralExpression::accept(Visitor &visitor) {
    cout << "LiteralExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitLiteralExpression(this);
}

TokenType LiteralExpression::getType() const {
    return type_;
}


// ********************
// IdentifierExpression
// ********************

IdentifierExpression::IdentifierExpression(string name)
    : name_(move(name)) {
}

std::shared_ptr<Value> IdentifierExpression::accept(Visitor &visitor) {
    cout << "IdentifierExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitIdentifierExpression(this);
}

const string &IdentifierExpression::getName() const {
    return name_;
}

// ********************
// BinaryExpression
// ********************

BinaryExpression::BinaryExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right)
    : left_(move(left)), op_(op), right_(move(right)) {
}

std::shared_ptr<Value> BinaryExpression::accept(Visitor &visitor) {
    cout << "BinaryExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitBinaryExpression(this);
}

Expression *BinaryExpression::getLeft() const {
    return left_.get();
}

BinaryExpression::Operator BinaryExpression::getOperator() const {
    return op_;
}

Expression *BinaryExpression::getRight() const {
    return right_.get();
}

// ********************
// UnaryExpression
// ********************

UnaryExpression::UnaryExpression(Operator op, unique_ptr<Expression> right)
    : op_(op), right_(move(right)) {
}

std::shared_ptr<Value> UnaryExpression::accept(Visitor &visitor) {
    cout << "UnaryExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitUnaryExpression(this);
}

UnaryExpression::Operator UnaryExpression::getOperator() const {
    return op_;
}

Expression *UnaryExpression::getRight() const {
    return right_.get();
}

// ********************
// AssignmentExpression
// ********************

AssignmentExpression::AssignmentExpression(string name, unique_ptr<Expression> value, TokenType op)
    : name_(move(name)), value_(move(value)), op_(op) {
}

std::shared_ptr<Value> AssignmentExpression::accept(Visitor &visitor) {
    cout << "AssignmentExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitAssignmentExpression(this);
}

const string &AssignmentExpression::getName() const {
    return name_;
}

Expression *AssignmentExpression::getValue() const {
    return value_.get();
}

TokenType AssignmentExpression::getOperator() const {
    return op_;
}

// ********************
// LogicalExpression
// ********************

LogicalExpression::LogicalExpression(unique_ptr<Expression> left, Operator op, unique_ptr<Expression> right)
    : left_(move(left)), op_(op), right_(move(right)) {
}

std::shared_ptr<Value> LogicalExpression::accept(Visitor &visitor) {
    cout << "LogicalExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitLogicalExpression(this);
}

Expression *LogicalExpression::getLeft() const {
    return left_.get();
}

LogicalExpression::Operator LogicalExpression::getOperator() const {
    return op_;
}

Expression *LogicalExpression::getRight() const {
    return right_.get();
}

// ********************
// FunctionCallExpression
// ********************

FunctionCallExpression::FunctionCallExpression(unique_ptr<Expression> callee, vector<unique_ptr<Expression> > arguments)
    : callee_(move(callee)), arguments_(move(arguments)) {
}

std::shared_ptr<Value> FunctionCallExpression::accept(Visitor &visitor) {
    cout << "FunctionCallExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitFunctionCallExpression(this);
}

Expression *FunctionCallExpression::getCallee() const {
    return callee_.get();
}

const vector<unique_ptr<Expression> > &FunctionCallExpression::getArguments() const {
    return arguments_;
}

// ********************
// GetExpression
// ********************

GetExpression::GetExpression(unique_ptr<Expression> object, string name)
    : object_(move(object)), name_(move(name)) {
}

std::shared_ptr<Value> GetExpression::accept(Visitor &visitor) {
    cout << "GetExpression::accept(Visitor &visitor)" << endl;
    return visitor.visitGetExpression(this);
}

Expression *GetExpression::getObject() const {
    return object_.get();
}

const string &GetExpression::getName() const {
    return name_;
}

// ********************
// ExpressionStatement
// ********************

ExpressionStatement::ExpressionStatement(unique_ptr<Expression> expression)
    : expression_(move(expression)) {
}

std::shared_ptr<Value> ExpressionStatement::accept(Visitor &visitor) {
    cout << "ExpressionStatement::accept(Visitor &visitor)" << endl;
    visitor.visitExpressionStatement(this);
    return {};
}

Expression *ExpressionStatement::getExpression() const {
    return expression_.get();
}

// ********************
// VariableDeclaration
// ********************

VariableDeclaration::VariableDeclaration(string name, string typeName, unique_ptr<Expression> initializer)
    : name_(move(name)), typeName_(move(typeName)), initializer_(move(initializer)) {
}

std::shared_ptr<Value> VariableDeclaration::accept(Visitor &visitor) {
    cout << "VariableDeclaration::accept(Visitor &visitor)" << endl;
    visitor.visitVariableDeclaration(this);
    return {};
}

const string &VariableDeclaration::getName() const {
    return name_;
}

const string &VariableDeclaration::getTypeName() const {
    return typeName_;
}

Expression *VariableDeclaration::getInitializer() const {
    return initializer_.get();
}

// ********************
// BlockStatement
// ********************

BlockStatement::BlockStatement(vector<unique_ptr<Statement> > statements)
    : statements_(move(statements)) {
}

std::shared_ptr<Value> BlockStatement::accept(Visitor &visitor) {
    cout << "BlockStatement::accept(Visitor &visitor)" << endl;
    visitor.visitBlockStatement(this);
    return {};
}

const vector<unique_ptr<Statement> > &BlockStatement::getStatements() const {
    return statements_;
}

// ********************
// IfStatement
// ********************

IfStatement::IfStatement(unique_ptr<Expression> condition, unique_ptr<Statement> thenBranch,
                         unique_ptr<Statement> elseBranch)
    : condition_(move(condition)), thenBranch_(move(thenBranch)), elseBranch_(move(elseBranch)) {
}

std::shared_ptr<Value> IfStatement::accept(Visitor &visitor) {
    cout << "IfStatement::accept(Visitor &visitor)" << endl;
    visitor.visitIfStatement(this);
    return {};
}

Expression *IfStatement::getCondition() const {
    return condition_.get();
}

Statement *IfStatement::getThenBranch() const {
    return thenBranch_.get();
}

Statement *IfStatement::getElseBranch() const {
    return elseBranch_.get();
}

// ********************
// WhileStatement
// ********************

WhileStatement::WhileStatement(unique_ptr<Expression> condition, unique_ptr<Statement> body)
    : condition_(move(condition)), body_(move(body)) {
}

std::shared_ptr<Value> WhileStatement::accept(Visitor &visitor) {
    cout << "WhileStatement::accept(Visitor &visitor)" << endl;
    visitor.visitWhileStatement(this);
    return {};
}

Expression *WhileStatement::getCondition() const {
    return condition_.get();
}

Statement *WhileStatement::getBody() const {
    return body_.get();
}

// ********************
// ReturnStatement
// ********************

ReturnStatement::ReturnStatement(unique_ptr<Expression> value)
    : value_(move(value)) {
}

std::shared_ptr<Value> ReturnStatement::accept(Visitor &visitor) {
    cout << "ReturnStatement::accept(Visitor &visitor)" << endl;
    visitor.visitReturnStatement(this);
    return {};
}

Expression *ReturnStatement::getValue() const {
    return value_.get();
}

// ********************
// FunctionDeclaration
// ********************

FunctionDeclaration::FunctionDeclaration(string name, vector<Parameter> parameters, string returnTypeName,
                                         unique_ptr<BlockStatement> body)
    : name_(move(name)), parameters_(move(parameters)), returnTypeName_(move(returnTypeName)), body_(move(body)) {
}

std::shared_ptr<Value> FunctionDeclaration::accept(Visitor &visitor) {
    cout << "FunctionDeclaration::accept(Visitor &visitor)" << endl;
    visitor.visitFunctionDeclaration(this);
    return {};
}

const string &FunctionDeclaration::getName() const {
    return name_;
}

const vector<FunctionDeclaration::Parameter> &FunctionDeclaration::getParameters() const {
    return parameters_;
}

const string &FunctionDeclaration::getReturnTypeName() const {
    return returnTypeName_;
}

BlockStatement *FunctionDeclaration::getBody() const {
    return body_.get();
}
