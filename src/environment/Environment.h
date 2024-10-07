#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <memory>
#include "../value/Value.h"


using namespace std;

class Environment {
public:
    explicit Environment(shared_ptr<Environment> enclosing = nullptr);

    void define(const string &name, const Value &value);

    void assign(const string &name, const Value &value);

    Value get(const string &name);

private:
    unordered_map<string, Value> values_;
    shared_ptr<Environment> enclosing_;
};

#endif // ENVIRONMENT_H
