#ifndef FUNCTION_H
#define FUNCTION_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "object/Object.h"

class Value; // Forward declaration

class Function : public Object {
public:
    virtual ~Function() = default;

    virtual std::shared_ptr<Value> call(const std::vector<std::shared_ptr<Value> > &args) = 0;

    std::map<std::string, std::shared_ptr<Value> > properties;
};

#endif // FUNCTION_H
