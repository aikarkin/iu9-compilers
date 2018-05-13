cmake_minimum_required(VERSION 3.10)
project(Lab7_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab7_1 main.cpp lexer/domain_tags.h lexer/TokenStream.cpp lexer/TokenStream.h lexer/position.h lexer/fragment.h lexer/token.h parser/RuleItem.h parser/NonTerm.h parser/Rule.h parser/Parser.cpp parser/Parser.h tree/tree.hpp tree/tree_util.hpp)