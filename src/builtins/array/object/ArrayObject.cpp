//
// Created by evan.mcpheron on 10/9/24.
//

#include "ArrayObject.h"
#include "builtins/array/ArrayClass.h"

ArrayObject::ArrayObject() {
    this->classType = std::make_shared<ArrayClass>();
}
