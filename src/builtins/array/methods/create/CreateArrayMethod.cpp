//
// Created by evan.mcpheron on 10/9/24.
//

#include "CreateArrayMethod.h"
#include "builtins/array/object/ArrayObject.h"

std::shared_ptr<Value> CreateArrayMethod::call(const std::vector<std::shared_ptr<Value> > &args) {
    return std::make_unique<Value>(std::make_shared<ArrayObject>());
}
