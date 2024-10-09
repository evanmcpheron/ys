#include "Interpreter.h"

#include <cmath>
#include <iostream>

#include "../environment/Environment.h"

using namespace std;


Interpreter::Interpreter()
    : environment_(make_shared<Environment>()) // Initialize with a new Environment
{
}


void Interpreter::interpret(const vector<unique_ptr<Statement> > &statements) {
    try {
        for (const auto &statement: statements) {
            statement->accept(*this);
            cout << "Statement Result: " << endl;
            this->lastValue.getValue()->printValue();
        }
    } catch (const exception &e) {
        cerr << "Runtime error: " << e.what() << endl;
    }
}

Value Interpreter::visitLiteralExpression(LiteralExpression *expression) {
    auto val = expression->getValue();
    auto type = expression->getType();
    setLastValue(Value(type, val));


    return Value(this->lastValue);
}


Value Interpreter::visitIdentifierExpression(IdentifierExpression *expression) {
    string variableName = expression->getName();

    // Look up the variable's value in the environment
    Value value = environment_->get(variableName);
    setLastValue(value);

    return value;
}

Value Interpreter::visitBinaryExpression(BinaryExpression *expression) {
    // Step 1: Evaluate the left operand
    expression->getLeft()->accept(*this);

    const Value left = this->lastValue;

    // Step 2: Evaluate the right operand
    expression->getRight()->accept(*this);
    const Value right = this->lastValue;

    // Early exit for division by zero
    if (expression->getOperator() == BinaryExpression::Operator::DIVIDE && right.asDouble() == 0) {
        throw runtime_error("Division by zero");
    }

    // Step 3: Ensure both operands are numbers and convert to double if needed
    double leftValue, rightValue;

    // Check if the left value is a number
    leftValue = left.asDouble();

    // Check if the right value is a number
    rightValue = right.asDouble();

    switch (expression->getOperator()) {
        case BinaryExpression::Operator::ADD: {
            this->lastValue = Value(leftValue + rightValue);
            break;
        }
        case BinaryExpression::Operator::SUBTRACT: {
            this->lastValue = Value(leftValue - rightValue);
            break;
        }
        case BinaryExpression::Operator::MULTIPLY: {
            this->lastValue = Value(leftValue * rightValue);
            break;
        }
        case BinaryExpression::Operator::DIVIDE: {
            this->lastValue = Value(leftValue / rightValue);
            break;
        }
        case BinaryExpression::Operator::MODULO: {
            this->lastValue = Value(fmod(leftValue, rightValue));
            break;
        }
        case BinaryExpression::Operator::EQUAL: {
            this->lastValue = Value(leftValue == rightValue);
            break;
        }
        case BinaryExpression::Operator::NOT_EQUAL: {
            this->lastValue = Value(leftValue != rightValue);
            break;
        }

        //TODO: Add strict equality and strict inequality

        case BinaryExpression::Operator::LESS: {
            this->lastValue = Value(leftValue < rightValue);
            break;
        }
        case BinaryExpression::Operator::LESS_EQUAL: {
            this->lastValue = Value(leftValue <= rightValue);
            break;
        }
        case BinaryExpression::Operator::GREATER: {
            this->lastValue = Value(leftValue > rightValue);
            break;
        }
        case BinaryExpression::Operator::GREATER_EQUAL: {
            this->lastValue = Value(leftValue >= rightValue);
            break;
        }
        case BinaryExpression::Operator::LOGICAL_AND: {
            this->lastValue = Value(isTruthy(left) && isTruthy(right));
            break;
        }
        case BinaryExpression::Operator::LOGICAL_OR: {
            this->lastValue = Value(isTruthy(left) || isTruthy(right));
            break;
        }
        default: {
            throw runtime_error("Unknown binary operator");
        }
    }
    return this->lastValue;
}


Value Interpreter::visitUnaryExpression(UnaryExpression *expression) {
    return Value();
}

Value Interpreter::visitAssignmentExpression(AssignmentExpression *expression) {
    string variableName = expression->getName();

    // Evaluate the right-hand side of the assignment
    expression->getValue()->accept(*this);
    Value value = lastValue;

    // Assign the evaluated value to the variable in the environment
    environment_->assign(variableName, value);

    return value;
}

Value Interpreter::visitLogicalExpression(LogicalExpression *expression) {
    return Value();
}

Value Interpreter::visitFunctionCallExpression(FunctionCallExpression *expression) {
    return Value();
}

Value Interpreter::visitGetExpression(GetExpression *expression) {
    return Value();
}

void Interpreter::visitExpressionStatement(ExpressionStatement *statement) {
    statement->getExpression()->accept(*this);
}

void Interpreter::visitVariableDeclaration(VariableDeclaration *statement) {
    // Ensure that the statement is valid
    if (!statement) {
        throw std::runtime_error("Null pointer passed to visitVariableDeclaration.");
    }

    string variableName = statement->getName();

    // Ensure that the environment is valid
    if (!environment_) {
        throw std::runtime_error("Environment is not initialized.");
    }

    // Evaluate the initializer if it exists, or set it to a default value
    Value value;
    if (statement->hasInitializer()) {
        // Check that the initializer is not null
        if (!statement->getInitializer()) {
            throw std::runtime_error("Null initializer in VariableDeclaration.");
        }

        // Evaluate the initializer expression
        statement->getInitializer()->accept(*this);
        value = lastValue;
    } else {
        value = Value(); // Default value (could be null or undefined)
    }
    bool isConst = statement->getTypeName() == "const";
    // Define the variable in the current environment
    environment_->define(variableName, value, isConst);
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

void Interpreter::executeBlock(const vector<unique_ptr<Statement> > &statements,
                               shared_ptr<Environment> newEnvironment) {
}

bool Interpreter::isTruthy(const Value &value) {
    switch (value.getType()) {
        case TokenType::BOOLEAN_LITERAL:
            return value.asBool(); // If it's a boolean, return its actual value (true or false)
        case TokenType::DOUBLE_LITERAL:
            return value.asDouble() != 0; // Numbers are truthy if they're not zero
        case TokenType::STRING_LITERAL:
            return !value.asString().empty(); // Non-empty strings are truthy
        case TokenType::UNKNOWN:
            return false; // Undefined or null-like values are falsey
        default:
            return false; // Other cases could default to false, depending on your needs
    }
}

void Interpreter::setLastValue(const Value &value) {
    // Handle all the types your interpreter supports
    if (value.isDouble()) {
        this->lastValue = value; // Already a double, no conversion needed
    } else if (value.isBool()) {
        this->lastValue = value; // Set as a boolean
    } else if (value.isString()) {
        this->lastValue = value; // Set as a string
    } else {
        throw runtime_error("Unsupported type encountered");
    }
}


