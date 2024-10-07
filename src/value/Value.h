#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <memory>
#include "../../include/Token.h"
#include "../ast/AST.h"

using namespace std;

// Forward declarations
class Function;
class Object;

// Base class ValueType with clone and printValue methods
class ValueType {
public:
    virtual ~ValueType() = default;

    virtual void printValue() const = 0;

    [[nodiscard]] virtual ValueType *clone() const = 0;
};

// Derived classes for specific value types

class IntValue final : public ValueType {
    int value_;

public:
    explicit IntValue(int value) : value_(value) {
    }

    void printValue() const override {
        cout << value_ << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new IntValue(*this);
    }

    [[nodiscard]] int getBaseValue() const {
        return value_;
    }
};

class StringValue final : public ValueType {
    string value_;

public:
    explicit StringValue(string value) : value_(move(value)) {
    }

    void printValue() const override {
        cout << value_ << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new StringValue(*this);
    }

    [[nodiscard]] const string &getBaseValue() const {
        return value_;
    }
};

class BoolValue final : public ValueType {
    bool value_;

public:
    explicit BoolValue(bool value) : value_(value) {
    }

    void printValue() const override {
        cout << (value_ ? "true" : "false") << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new BoolValue(*this);
    }

    [[nodiscard]] bool getBaseValue() const {
        return value_;
    }
};

class DoubleValue final : public ValueType {
    double value_;

public:
    explicit DoubleValue(double value) : value_(value) {
    }

    void printValue() const override {
        cout << value_ << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new DoubleValue(*this);
    }

    [[nodiscard]] double getBaseValue() const {
        return value_;
    }
};

class NullValue final : public ValueType {
public:
    void printValue() const override {
        cout << "null" << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new NullValue(*this);
    }

    [[nodiscard]] nullptr_t getBaseValue() const {
        return nullptr;
    }
};

// Value class that holds a shared pointer to a ValueType
class Value {
public:
    // Constructors for various value types
    explicit Value(int integer, TokenType tokenType = TokenType::INTEGER_LITERAL)
        : type(tokenType), value(make_shared<IntValue>(integer)) {
    }

    explicit Value(double floating, TokenType tokenType = TokenType::DOUBLE_COLON)
        : type(tokenType), value(make_shared<DoubleValue>(floating)) {
    }

    explicit Value(const string &str, TokenType tokenType = TokenType::STRING_LITERAL)
        : type(tokenType), value(make_shared<StringValue>(str)) {
    }

    explicit Value(bool boolean, TokenType tokenType = TokenType::BOOLEAN_LITERAL)
        : type(tokenType), value(make_shared<BoolValue>(boolean)) {
    }

    explicit Value(TokenType tokenType = TokenType::NULL_LITERAL)
        : type(tokenType), value(make_shared<NullValue>()) {
    }

    // Constructor that accepts a shared_ptr<ValueType>
    explicit Value(TokenType tokenType, shared_ptr<ValueType> val)
        : type(tokenType), value(std::move(val)) {
    }

    // Copy constructor for deep copy
    Value(const Value &other)
        : type(other.type), value(other.value ? shared_ptr<ValueType>(other.value->clone()) : nullptr) {
    }

    // Assignment operator for deep copy
    Value &operator=(const Value &other) {
        if (this != &other) {
            type = other.type;
            value = other.value ? shared_ptr<ValueType>(other.value->clone()) : nullptr;
        }
        return *this;
    }

    // Move constructor
    Value(Value &&other) noexcept
        : type(std::move(other.type)), value(std::move(other.value)) {
        other.value = nullptr;
    }

    // Move assignment operator
    Value &operator=(Value &&other) noexcept {
        if (this != &other) {
            type = std::move(other.type);
            value = std::move(other.value);
            other.value = nullptr;
        }
        return *this;
    }

    // Equality operator
    bool operator==(const Value &other) const {
        if (this->type != other.type) {
            return false;
        }
        switch (type) {
            case TokenType::INTEGER_LITERAL:
                return this->asInt() == other.asInt();
            case TokenType::DOUBLE_LITERAL:
                return this->asDouble() == other.asDouble();
            case TokenType::STRING_LITERAL:
                return this->asString() == other.asString();
            case TokenType::BOOLEAN_LITERAL:
                return this->asBool() == other.asBool();
            case TokenType::NULL_LITERAL:
                return this->isNull() == other.isNull();
            default:
                return false;
        }
    }

    // Type checking methods
    bool isNull() const { return type == TokenType::NULL_LITERAL; }
    bool isBool() const { return dynamic_cast<BoolValue *>(value.get()) != nullptr; }
    bool isInt() const { return dynamic_cast<IntValue *>(value.get()) != nullptr; }
    bool isDouble() const { return dynamic_cast<DoubleValue *>(value.get()) != nullptr; }
    bool isString() const { return dynamic_cast<StringValue *>(value.get()) != nullptr; }

    // Getters for various value types
    [[nodiscard]] bool asBool() const {
        auto boolValue = dynamic_cast<BoolValue *>(value.get());
        if (boolValue) {
            return boolValue->getBaseValue();
        }
        throw runtime_error("Not a boolean value");
    }

    [[nodiscard]] int asInt() const {
        auto intValue = dynamic_cast<IntValue *>(value.get());
        if (intValue) {
            return intValue->getBaseValue();
        }
        throw runtime_error("Not an integer value");
    }

    [[nodiscard]] double asDouble() const {
        auto doubleValue = dynamic_cast<DoubleValue *>(value.get());
        if (doubleValue) {
            return doubleValue->getBaseValue();
        }
        throw runtime_error("Not a double value");
    }

    [[nodiscard]] string asString() const {
        auto stringValue = dynamic_cast<StringValue *>(value.get());
        if (stringValue) {
            return stringValue->getBaseValue();
        }
        throw runtime_error("Not a string value");
    }

private:
    TokenType type;
    shared_ptr<ValueType> value;

public:
    [[nodiscard]] shared_ptr<ValueType> getValue() const {
        return value;
    }

    [[nodiscard]] TokenType getType() const {
        return type;
    }
};

#endif // VALUE_H
