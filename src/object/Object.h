#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <unordered_map>
#include <memory>
#include "../value/Value.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    Object();

    // Property access methods
    void set(const std::string &name, const Value &value);

    Value get(const std::string &name) const;

private:
    std::unordered_map<std::string, Value> properties_;
};

#endif // OBJECT_H
