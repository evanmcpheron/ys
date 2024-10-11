cmake_minimum_required(VERSION 3.29)
project(Yolo)

set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
include_directories(${SRC_DIR})


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
        src/environment/Environment.cpp
        src/interpreter/Interpreter.cpp
        src/value/Value.cpp
        src/function/Function.cpp
        src/object/Object.cpp
        src/class/Class.h
        src/class/Class.cpp
        src/builtins/array/ArrayClass.h
        src/builtins/array/ArrayClass.cpp
        src/builtins/array/methods/push/PushMethod.h
        src/builtins/array/methods/push/PushMethod.cpp
        src/builtins/array/methods/create/CreateArrayMethod.h
        src/builtins/array/methods/create/CreateArrayMethod.cpp
        src/builtins/array/object/ArrayObject.h
        src/builtins/array/object/ArrayObject.cpp)
