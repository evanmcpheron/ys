#ifndef NEWARRAYMETHOD_H
#define NEWARRAYMETHOD_H

#include "function/Function.h"

// Forward declaration of ArrayObject
class ArrayObject;

class CreateArrayMethod final : public Function {
public:
    std::shared_ptr<Value> call(const std::vector<std::shared_ptr<Value> > &args) override;
};

#endif //NEWARRAYMETHOD_H
