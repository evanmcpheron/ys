#include "Value.h"
#include "object/Object.h"
#include "class/Class.h"
#include "function/Function.h"

#include <sstream>


std::shared_ptr<Object> Value::asObject() const {
    if (isClass()) {
        return std::static_pointer_cast<Object>(asClass());
    }
    if (isFunction()) {
        return std::static_pointer_cast<Object>(asFunction());
    }
    auto objectValue = dynamic_cast<ObjectValue *>(value.get());
    if (objectValue) return objectValue->getBaseValue();
    throw std::runtime_error("Not an object value");
}

shared_ptr<Class> Value::asClass() const {
    auto classValue = dynamic_cast<ClassValue *>(value.get());
    if (classValue) return classValue->getBaseValue();
    throw std::runtime_error("Not a class value");
}

shared_ptr<Function> Value::asFunction() const {
    auto functionValue = dynamic_cast<FunctionValue *>(value.get());
    if (functionValue) return functionValue->getBaseValue();
    throw std::runtime_error("Not a function value");
}

bool Value::isClass() const {
    return dynamic_cast<ClassValue *>(value.get()) != nullptr;
}

bool Value::isFunction() const {
    return dynamic_cast<FunctionValue *>(value.get()) != nullptr;
}

bool Value::isObject() const {
    return dynamic_cast<ObjectValue *>(value.get()) != nullptr;
}

