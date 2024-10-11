#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <string>
#include <memory>

class Class;
class Value; // Forward declaration

class Object {
public:
    virtual ~Object() = default;

    std::map<std::string, std::shared_ptr<Value> > fields;

    std::shared_ptr<Class> classType;
};

#endif // OBJECT_H
