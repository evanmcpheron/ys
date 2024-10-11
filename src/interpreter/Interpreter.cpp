#include "Interpreter.h"

#include <cmath>
#include <iostream>

#include "environment/Environment.h"
#include "builtins/array/ArrayClass.h"
#include "object/Object.h"
#include "class/Class.h"
#include "function/Function.h"
#include "value/Value.h"

using namespace std;


Interpreter::Interpreter()
    : environment_(make_shared<Environment>()) // Initialize with a new Environment
{
    registerBuiltIns();
}


void Interpreter::interpret(const vector<unique_ptr<Statement> > &statements) {
    try {
        for (const auto &statement: statements) {
            statement->accept(*this);
            cout << "Statement Result: " << endl;
            this->lastValue->getValue()->printValue();
        }
    } catch (const exception &e) {
        cerr << "Runtime error: " << e.what() << endl;
    }
}

shared_ptr<Value> Interpreter::visitLiteralExpression(LiteralExpression *expression) {
    auto val = expression->getValue();
    auto value = std::make_shared<Value>(val.get());
    setLastValue(value);
    return value;
}


shared_ptr<Value> Interpreter::visitIdentifierExpression(IdentifierExpression *expression) {
    const std::string &variableName = expression->getName();
    shared_ptr<Value> value = environment_->get(variableName);
    setLastValue(value);
    return value;
}

shared_ptr<Value> Interpreter::visitBinaryExpression(BinaryExpression *expression) {
    // Step 1: Evaluate the left operand
    expression->getLeft()->accept(*this);

    const shared_ptr<Value> left = this->lastValue;

    // Step 2: Evaluate the right operand
    expression->getRight()->accept(*this);
    const shared_ptr<Value> right = this->lastValue;

    // Early exit for division by zero
    if (expression->getOperator() == BinaryExpression::Operator::DIVIDE && right->asDouble() == 0) {
        throw runtime_error("Division by zero");
    }

    // Step 3: Ensure both operands are numbers and convert to double if needed

    // Check if the left value is a number
    const double leftValue = left->asDouble();

    // Check if the right value is a number
    const double rightValue = right->asDouble();

    switch (expression->getOperator()) {
        case BinaryExpression::Operator::ADD: {
            this->lastValue = make_shared<Value>(Value(leftValue + rightValue));
            break;
        }
        case BinaryExpression::Operator::SUBTRACT: {
            this->lastValue = make_shared<Value>(Value(leftValue - rightValue));
            break;
        }
        case BinaryExpression::Operator::MULTIPLY: {
            this->lastValue = make_shared<Value>(Value(leftValue * rightValue));
            break;
        }
        case BinaryExpression::Operator::DIVIDE: {
            this->lastValue = make_shared<Value>(Value(leftValue / rightValue));
            break;
        }
        case BinaryExpression::Operator::MODULO: {
            this->lastValue = make_shared<Value>(Value(fmod(leftValue, rightValue)));
            break;
        }
        case BinaryExpression::Operator::EQUAL: {
            this->lastValue = make_shared<Value>(Value(leftValue == rightValue));
            break;
        }
        case BinaryExpression::Operator::NOT_EQUAL: {
            this->lastValue = make_shared<Value>(Value(leftValue != rightValue));
            break;
        }

        //TODO: Add strict equality and strict inequality

        case BinaryExpression::Operator::LESS: {
            this->lastValue = make_shared<Value>(Value(leftValue < rightValue));
            break;
        }
        case BinaryExpression::Operator::LESS_EQUAL: {
            this->lastValue = make_shared<Value>(Value(leftValue <= rightValue));
            break;
        }
        case BinaryExpression::Operator::GREATER: {
            this->lastValue = make_shared<Value>(Value(leftValue > rightValue));
            break;
        }
        case BinaryExpression::Operator::GREATER_EQUAL: {
            this->lastValue = make_shared<Value>(Value(leftValue >= rightValue));
            break;
        }
        case BinaryExpression::Operator::LOGICAL_AND: {
            this->lastValue = make_shared<Value>(Value(isTruthy(left) && isTruthy(right)));
            break;
        }
        case BinaryExpression::Operator::LOGICAL_OR: {
            this->lastValue = make_shared<Value>(Value(isTruthy(left) || isTruthy(right)));
            break;
        }
        default: {
            throw runtime_error("Unknown binary operator");
        }
    }
    return this->lastValue;
}


shared_ptr<Value> Interpreter::visitUnaryExpression(UnaryExpression *expression) {
    return shared_ptr<Value>();
}

shared_ptr<Value> Interpreter::visitAssignmentExpression(AssignmentExpression *expression) {
    string variableName = expression->getName();

    // Evaluate the right-hand side of the assignment
    expression->getValue()->accept(*this);
    shared_ptr<Value> value = lastValue;

    // Assign the evaluated value to the variable in the environment
    environment_->assign(variableName, value);

    return value;
}

shared_ptr<Value> Interpreter::visitLogicalExpression(LogicalExpression *expression) {
    return shared_ptr<Value>();
}

shared_ptr<Value> Interpreter::visitFunctionCallExpression(FunctionCallExpression *expression) {
    // Evaluate the callee
    expression->getCallee()->accept(*this);
    shared_ptr<Value> calleeValue = lastValue;

    // Evaluate arguments
    std::vector<std::shared_ptr<Value> > arguments;
    for (const auto &argExpr: expression->getArguments()) {
        auto argValue = argExpr->accept(*this);
        arguments.push_back(argValue);
    }

    if (calleeValue->isFunction()) {
        auto function = calleeValue->asFunction();

        // For methods, the first argument can be the 'this' context
        shared_ptr<Value> result = function->call(arguments);
        setLastValue(result);
        return result;
    } else {
        throw std::runtime_error("Can only call functions.");
    }
}


shared_ptr<Value> Interpreter::visitGetExpression(GetExpression *expression) {
    // Evaluate the object
    expression->getObject()->accept(*this);
    shared_ptr<Value> objectValue = lastValue;

    const std::string &propertyName = expression->getName();

    if (objectValue->isClass()) {
        auto classObject = objectValue->asClass();
        // Check for static methods
        if (classObject->staticMethods.contains(propertyName)) {
            auto method = classObject->staticMethods[propertyName];
            shared_ptr<Value> value = make_shared<Value>(method);
            setLastValue(value);
            return value;
        }
        // Check for static properties
        if (classObject->staticProperties.contains(propertyName)) {
            shared_ptr<Value> value = classObject->staticProperties[propertyName];
            setLastValue(value);
            return value;
        }
    } else if (objectValue->isFunction()) {
        auto functionObject = objectValue->asFunction();
        // Check for properties on the function object
        if (functionObject->properties.contains(propertyName)) {
            shared_ptr<Value> value = functionObject->properties[propertyName];
            setLastValue(value);
            return value;
        }
    } else if (objectValue->isObject()) {
        auto object = objectValue->asObject();
        // Check for instance properties
        if (object->fields.contains(propertyName)) {
            shared_ptr<Value> value = object->fields[propertyName];
            setLastValue(value);
            return value;
        }
        // Check for instance methods
        auto classType = object->classType.get();
        if (classType->methods.contains(propertyName)) {
            auto method = classType->methods[propertyName];
            shared_ptr<Value> value = make_shared<Value>(method);
            setLastValue(value);
            return value;
        }
    } else {
        throw std::runtime_error("Only objects, classes, and functions have properties.");
    }

    throw std::runtime_error("Undefined property '" + propertyName + "'.");
}


void Interpreter::visitExpressionStatement(ExpressionStatement *statement) {
    statement->getExpression()->accept(*this);
}

void Interpreter::visitVariableDeclaration(VariableDeclaration *statement) {
    // Ensure that the statement is valid
    if (!statement) {
        throw runtime_error("Null pointer passed to visitVariableDeclaration.");
    }

    string variableName = statement->getName();

    // Ensure that the environment is valid
    if (!environment_) {
        throw runtime_error("Environment is not initialized.");
    }

    // Evaluate the initializer if it exists, or set it to a default value
    shared_ptr<Value> value;
    if (statement->hasInitializer()) {
        // Check that the initializer is not null
        if (!statement->getInitializer()) {
            throw runtime_error("Null initializer in VariableDeclaration.");
        }

        // Evaluate the initializer expression
        statement->getInitializer()->accept(*this);
        value = lastValue;
    } else {
        value = shared_ptr<Value>(); // Default value (could be null or undefined)
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

void Interpreter::executeBlock(const vector<unique_ptr<Statement> > &statements,
                               const shared_ptr<Environment> &newEnvironment) {
}

bool Interpreter::isTruthy(const shared_ptr<Value> &value) {
    switch (value->getType()) {
        case TokenType::BOOLEAN_LITERAL:
            return value->asBool(); // If it's a boolean, return its actual value (true or false)
        case TokenType::DOUBLE_LITERAL:
            return value->asDouble() != 0; // Numbers are truthy if they're not zero
        case TokenType::STRING_LITERAL:
            return !value->asString().empty(); // Non-empty strings are truthy
        default:
            return false; // Other cases could default to false, depending on your needs
    }
}

void Interpreter::setLastValue(const shared_ptr<Value> &value) {
    // Handle all the types your interpreter supports
    if (value->isDouble()) {
        this->lastValue = value; // Already a double, no conversion needed
    } else if (value->isBool()) {
        this->lastValue = value; // Set as a boolean
    } else if (value->isString()) {
        this->lastValue = value; // Set as a string
    } else if (value->isObject()) {
        this->lastValue = value; // Set as an object
    } else if (value->isFunction()) {
        this->lastValue = value; // Set as a function
    } else if (value->isClass()) {
        this->lastValue = value; // Set as a class
    } else if (value->isNull()) {
        this->lastValue = value; // Set as null
    } else {
        throw runtime_error("Unsupported type encountered");
    }
}

void Interpreter::registerBuiltIns() const {
    const auto arrayClass = std::make_shared<ArrayClass>();
    const auto arrayValue = make_shared<Value>(Value(std::static_pointer_cast<Class>(arrayClass)));
    environment_->define("Array", arrayValue, true);
}


