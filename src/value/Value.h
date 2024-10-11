#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <memory>
#include "../../include/Token.h"
#include "../ast/AST.h"

class Class; // Forward declaration
class Function; // Forward declaration
class Object; // Forward declaration

using namespace std;


// Base class ValueType with clone and printValue methods
class ValueType {
public:
    virtual ~ValueType() = default;

    virtual void printValue() const = 0;

    [[nodiscard]] virtual ValueType *clone() const = 0;
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
    explicit BoolValue(const bool value) : value_(value) {
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

    [[nodiscard]] static nullptr_t getBaseValue() {
        return nullptr;
    }
};

class ClassValue final : public ValueType {
    shared_ptr<Class> value_;

public:
    explicit ClassValue(shared_ptr<Class> value) : value_(move(value)) {
    }

    void printValue() const override {
        cout << "<Class " << value_ << ">" << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new ClassValue(*this);
    }

    [[nodiscard]] shared_ptr<Class> getBaseValue() const {
        return value_;
    }
};

class FunctionValue final : public ValueType {
    shared_ptr<Function> value_;

public:
    explicit FunctionValue(shared_ptr<Function> value) : value_(move(value)) {
    }

    void printValue() const override {
        cout << "<Function>" << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new FunctionValue(*this);
    }

    [[nodiscard]] shared_ptr<Function> getBaseValue() const {
        return value_;
    }
};

class ObjectValue final : public ValueType {
    shared_ptr<Object> value_;

public:
    explicit ObjectValue(shared_ptr<Object> value) : value_(move(value)) {
    }

    void printValue() const override {
        cout << "<Object>" << endl;
    }

    [[nodiscard]] ValueType *clone() const override {
        return new ObjectValue(*this);
    }

    [[nodiscard]] shared_ptr<Object> getBaseValue() const {
        return value_;
    }
};

// Value class that holds a shared pointer to a ValueType
class Value {
public:
    Value() = default;

    explicit Value(double doubleValue);

    explicit Value(const std::string &stringValue);

    explicit Value(bool boolValue);

    explicit Value(std::shared_ptr<Class> classValue);

    explicit Value(std::shared_ptr<Function> functionValue);

    explicit Value(std::shared_ptr<Object> objectValue);


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
        : type(move(other.type)), value(move(other.value)) {
        other.value = nullptr;
    }

    // Move assignment operator
    Value &operator=(Value &&other) noexcept {
        if (this != &other) {
            type = move(other.type);
            value = move(other.value);
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
    [[nodiscard]] bool isNull() const { return type == TokenType::NULL_LITERAL; }
    [[nodiscard]] bool isBool() const { return dynamic_cast<BoolValue *>(value.get()) != nullptr; }
    [[nodiscard]] bool isDouble() const { return dynamic_cast<DoubleValue *>(value.get()) != nullptr; }
    [[nodiscard]] bool isString() const { return dynamic_cast<StringValue *>(value.get()) != nullptr; }

    [[nodiscard]] bool isClass() const;

    [[nodiscard]] bool isFunction() const;

    [[nodiscard]] bool isObject() const;


    // Getters for various value types
    [[nodiscard]] bool asBool() const {
        auto boolValue = dynamic_cast<BoolValue *>(value.get());
        if (boolValue) {
            return boolValue->getBaseValue();
        }
        throw runtime_error("Not a boolean value");
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

    std::shared_ptr<Class> asClass() const;

    std::shared_ptr<Function> asFunction() const;

    std::shared_ptr<Object> asObject() const;

private:
    TokenType type;
    std::shared_ptr<ValueType> value;

public:
    [[nodiscard]] shared_ptr<ValueType> getValue() const {
        return value;
    }

    [[nodiscard]] TokenType getType() const {
        return type;
    }
};

#endif // VALUE_H
