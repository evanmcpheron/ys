#ifndef CLASS_H
#define CLASS_H

#include <map>
#include <string>
#include <memory>
#include <vector>
#include "object/Object.h"

class Value; // Forward declaration
class Function; // Forward declaration

class Class : public Object {
public:
    std::string name;
    std::map<std::string, std::shared_ptr<Function> > methods;
    std::map<std::string, std::shared_ptr<Function> > staticMethods;
    std::map<std::string, std::shared_ptr<Value> > staticProperties;

    virtual std::shared_ptr<Value> instantiate(const std::vector<std::shared_ptr<Value> > &arguments) = 0;

    virtual void invokeMethod(const std::string &methodName, Object *target,
                              const std::vector<std::shared_ptr<Value> > &arguments) = 0;
};

#endif // CLASS_H
