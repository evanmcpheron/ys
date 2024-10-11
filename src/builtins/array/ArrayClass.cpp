// ArrayClass.cpp
#include "ArrayClass.h"

#include "object/ArrayObject.h"
#include "methods/push/PushMethod.h"
#include "methods/create/CreateArrayMethod.h"

ArrayClass::ArrayClass() {
    this->name = "Array";
    this->methods["push"] = std::make_shared<PushMethod>();
    // Add static methods
    this->staticMethods["create"] = std::make_shared<CreateArrayMethod>();
}

void ArrayClass::invokeMethod(const std::string &methodName, Object *target,
                              const std::vector<shared_ptr<Value> > &arguments) {
    if (methods.find(methodName) != methods.end()) {
        methods[methodName]->call(arguments);
    } else {
        throw std::runtime_error("Method not found: " + methodName);
    }
}

shared_ptr<Value> ArrayClass::instantiate(const std::vector<shared_ptr<Value> > &arguments) {
    return make_unique<Value>(std::make_shared<ArrayObject>());
}
