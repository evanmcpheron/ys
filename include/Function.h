#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include <string>
#include <vector>

class Value;
class Interpreter;
class Environment;
class FunctionDeclaration;

using namespace std;

class Function {
public:
    Function(FunctionDeclaration *declaration, shared_ptr<Environment> closure);

    Value call(Interpreter *interpreter, const vector<Value> &arguments);

    int arity() const;

    string getName() const;

private:
    FunctionDeclaration *declaration_;
    shared_ptr<Environment> closure_;
};

#endif // FUNCTION_H
