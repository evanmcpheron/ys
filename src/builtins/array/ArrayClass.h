// ArrayClass.h
#ifndef ARRAYCLASS_H
#define ARRAYCLASS_H

#include "class/Class.h"

class ArrayClass : public Class {
public:
    ArrayClass();

    std::shared_ptr<Value> instantiate(const std::vector<std::shared_ptr<Value> > &arguments) override;

    void invokeMethod(const std::string &methodName, Object *target,
                      const std::vector<std::shared_ptr<Value> > &arguments) override;
};

#endif // ARRAYCLASS_H
