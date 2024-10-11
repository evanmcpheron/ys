#ifndef PUSHMETHOD_H
#define PUSHMETHOD_H

#include "function/Function.h"

// Forward declaration of ArrayObject
class ArrayObject;

class PushMethod final : public Function {
public:
    std::shared_ptr<Value> call(const std::vector<std::shared_ptr<Value> > &args) override;
};

#endif // PUSHMETHOD_H
