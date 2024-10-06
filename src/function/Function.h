#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>
#include <memory>
#include "../ast/AST.h"
#include "../environment/Environment.h"
#include "../value/Value.h"

class Interpreter;
class FunctionDeclaration;
class Environment;

class Function {
public:
    Function(FunctionDeclaration *declaration, std::shared_ptr<Environment> closure);

    Value call(Interpreter *interpreter, const std::vector<Value> &arguments);

    [[nodiscard]] int arity() const;

    [[nodiscard]] const std::string &getName() const;

private:
    FunctionDeclaration *declaration_;
    std::shared_ptr<Environment> closure_;
};

#endif // FUNCTION_H
