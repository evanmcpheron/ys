#ifndef ARRAYOBJECT_H
#define ARRAYOBJECT_H

#include <vector>
#include "object/Object.h"
#include "value/Value.h"

// Forward declaration of ArrayClass
class ArrayClass;

class ArrayObject : public Object {
public:
    std::vector<std::shared_ptr<Value> > values;

    ArrayObject();

    void push(const std::shared_ptr<Value> &value) {
        values.push_back(value);
    }
};

#endif //ARRAYOBJECT_H
