#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include <string>
#include <vector>

class Value;
class Interpreter;
class Environment;
class FunctionDeclaration;

class Function {
public:
    Function(FunctionDeclaration *declaration, std::shared_ptr<Environment> closure);

    Value call(Interpreter *interpreter, const std::vector<Value> &arguments);

    int arity() const;

    std::string getName() const;

private:
    FunctionDeclaration *declaration_;
    std::shared_ptr<Environment> closure_;
};

#endif // FUNCTION_H
