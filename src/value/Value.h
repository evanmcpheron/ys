#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <memory>

using namespace std;

// Forward declarations
class Function;
class Object;

class Value {
public:
 using ValueType = variant<nullptr_t, bool, int, double, string, shared_ptr<Function>, shared_ptr<Object> >;

 // Constructors
 Value();

 explicit Value(bool boolean);

 explicit Value(int integer);

 explicit Value(double floating);

 explicit Value(const string &string);

 explicit Value(shared_ptr<Function> function);

 explicit Value(shared_ptr<Object> object);

 // Type checking methods
 bool isNull() const;

 bool isBool() const;

 bool isInt() const;

 bool isDouble() const;

 bool isNumber() const;

 bool isString() const;

 bool isFunction() const;

 bool isObject() const;

 // Getters
 bool asBool() const;

 int asInt() const;

 double asDouble() const;

 const string &asString() const;

 shared_ptr<Function> asFunction() const;

 shared_ptr<Object> asObject() const;

 // Operations
 Value add(const Value &other) const;

 Value subtract(const Value &other) const;

 Value multiply(const Value &other) const;

 Value divide(const Value &other) const;

 // Comparison
 bool equals(const Value &other) const;

 // Object property access
 Value getProperty(const string &name) const;

 // String representation
 string toString() const;

private:
 ValueType value_;
};

#endif // VALUE_H
