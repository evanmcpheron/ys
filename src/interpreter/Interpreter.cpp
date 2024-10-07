#include "Interpreter.h"

#include <cmath>
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
        cout << "Statement Result: " << endl;
        this->lastValue.getValue()->printValue();
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
    return Value();
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
    if (left.isInt()) {
        leftValue = left.isInt() ? static_cast<double>(left.asInt()) : left.asDouble();
    } else {
        throw runtime_error("Left operand is not a number");
    }

    // Check if the right value is a number
    if (right.isInt()) {
        rightValue = right.isInt() ? static_cast<double>(right.asInt()) : right.asDouble();
    } else {
        throw runtime_error("Right operand is not a number");
    }

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
    return Value();
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
        case TokenType::INTEGER_LITERAL:
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
    if (value.isInt()) {
        this->lastValue = value; // Convert int to double for consistency
    } else if (value.isDouble()) {
        this->lastValue = value; // Already a double, no conversion needed
    } else if (value.isBool()) {
        this->lastValue = value; // Set as a boolean
    } else if (value.isString()) {
        this->lastValue = value; // Set as a string
    } else {
        throw runtime_error("Unsupported type encountered");
    }
}


