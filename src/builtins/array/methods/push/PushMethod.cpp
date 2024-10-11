//
// Created by evan.mcpheron on 10/9/24.
//

#include "PushMethod.h"
#include "builtins/array/object/ArrayObject.h"

std::shared_ptr<Value> PushMethod::call(const std::vector<std::shared_ptr<Value> > &args) {
    if (args.size() < 2) {
        throw std::runtime_error("push() expects at least 2 arguments");
    }

    // The first argument should be the array object
    std::shared_ptr<Value> arrayValue = args[0];
    if (!arrayValue->isObject()) {
        throw std::runtime_error("First argument to push() must be an array object");
    }

    auto arr = std::dynamic_pointer_cast<ArrayObject>(arrayValue->asObject());
    if (!arr) {
        throw std::runtime_error("Invalid object type for push method");
    }

    // Push the second argument into the array
    arr->values.push_back(args[1]);

    return std::shared_ptr<Value>(); // Return null or appropriate value
}
