cmake_minimum_required(VERSION 3.10)
project(Lab7_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab7_1 main.cpp lexer/TokenStream.cpp lexer/TokenStream.h lexer/position.h lexer/fragment.h lexer/token.h parser/parser_types/symbol.h parser/parser_types/rule.h tree/tree.hpp tree/tree_util.hpp parser/parser_types/symbol_type.h parser/Parser.cpp parser/Parser.h parser/parser_types/st_data.h)
#add_executable(Lab7_1 main.cpp lexer/TokenStream.cpp lexer/TokenStream.h lexer/position.h lexer/fragment.h lexer/token.h parser/symbol.h parser/rule.h tree/tree.hpp tree/tree_util.hpp parser/symbol_type.h)