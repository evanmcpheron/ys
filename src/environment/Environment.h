#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>
#include "../value/Value.h"

using namespace std;

class Environment {
public:
    Environment(shared_ptr<Environment> enclosing = nullptr)
        : enclosing_(enclosing) {
    }

    // Define a variable in the current environment with its const status
    void define(const string &name, const Value &value, bool isConst = false) {
        if (values_.find(name) != values_.end()) {
            throw runtime_error("Variable '" + name + "' is already defined.");
        }
        values_[name] = value;
        isConst_[name] = isConst; // Track whether this variable is const
    }

    // Get the value of a variable, looking in the current and outer environments
    Value get(const string &name) {
        if (values_.find(name) != values_.end()) {
            return values_[name];
        }
        if (enclosing_ != nullptr) {
            return enclosing_->get(name); // Look in the outer scope
        }
        throw runtime_error("Undefined variable '" + name + "'.");
    }

    // Assign a value to an existing variable, enforcing const rules
    void assign(const string &name, const Value &value) {
        if (values_.find(name) != values_.end()) {
            if (isConst_[name]) {
                throw runtime_error("Cannot reassign constant variable '" + name + "'.");
            }
            values_[name] = value;
        } else if (enclosing_ != nullptr) {
            enclosing_->assign(name, value);
        } else {
            throw runtime_error("Undefined variable '" + name + "'.");
        }
    }

private:
    unordered_map<string, Value> values_; // Stores variables and their values
    unordered_map<string, bool> isConst_; // Tracks whether a variable is const
    shared_ptr<Environment> enclosing_; // Enclosing (outer) scope
};

#endif // ENVIRONMENT_H
