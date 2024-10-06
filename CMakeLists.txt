cmake_minimum_required(VERSION 3.29)
project(Yolo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Yolo main.cpp
        include/Token.h
        src/lexer/Lexer.h
        src/lexer/Lexer.cpp
        src/ast/AST.h
        src/ast/AST.cpp
        src/parser/Parser.h
        src/parser/Parser.cpp
        src/visitor/Visitor.h
        src/interpreter/Interpreter.h
        include/Function.h
        include/Function.cpp
        src/environment/Environment.cpp
        src/interpreter/Interpreter.cpp
        src/value/Value.cpp
        src/function/Function.cpp
        src/object/Object.cpp)
