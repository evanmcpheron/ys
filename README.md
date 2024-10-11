### build

```
cd build
cmake ..
make
```

### run file

make sure that this is run from the build directory

```
./Yolo ../examples/script.ys
```

- include
    - Token.h
- src
    - ast
        - AST.cpp
        - AST.h
    - builtins
        - array
            - methods
                - create
                    - CreateArrayMethod.cpp
                    - CreateArrayMethod.h
                - push
                    - PushArrayMethod.cpp
                    - PushArrayMethod.h
                - object
                    - ArrayObject.cpp
                    - ArrayObject.h
                - ArrayClass.cpp
                - ArrahClass.h
        - class
            - Class.cpp
            - Class.h
        - environment
            - Environment.cpp
            - Environment.h
        - interpreter
            - Interpreter.cpp
            - Interpreter.h
        - lexer
            - Lexer.cpp
            - Lexer.h
        - object
            - Object.cpp
            - Object.h
        - parser
            - Parser.cpp
            - Parser.h
        - value
            - Value.cpp
            - Value.h
        - visitor
            - visitor.h
- main.cpp
- CMakeLists.txt