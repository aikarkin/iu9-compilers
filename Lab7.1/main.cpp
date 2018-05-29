#include <iostream>
#include <fstream>
#include "lexer/TokenStream.h"
#include "parser/parser_types/symbol.h"
#include "parser/Parser.h"
#include "tree/tree.hpp"
#include "tree/tree_util.hpp"
#include <variant>

std::string replace_all(std::string str, std::string substr, std::string replacement) {
    size_t idx = 0;
    std::string str_copy = str;
    while(true) {
        idx = str.find(substr, idx);
        if (idx == std::string::npos) break;
        str.replace(idx, substr.length(), replacement);
        idx += substr.length();
    }

    return str;
}

void printSyntaxTree(stl_tree::tree<st_data> &syntax, std::ostream &str) {
    std::string NON_TERMINALS[] = {"S", "A", "B", "C", "D", "F", "E", "G", "F1"};
    std::string TERMINALS[] = {"$", "{", "NONTERM", "}", ",", "[", ":", "]", "TERM", "@"};

    int i = 0;
    std::queue<st_iter> nodes;
    std::queue<int> numbers;

    nodes.emplace(syntax.begin());
    numbers.emplace(i);

    str << "graph syntax_tree {" << std::endl;


    while(!nodes.empty()) {
        st_iter item = nodes.front();
        int n = numbers.front();
        nodes.pop();
        numbers.pop();


        std::string nodeVal;
        if(std::holds_alternative<Token>(item->data)) {
            Token tok = std::get<Token>(item->data);
            if(tok.tag == EMPTY) {
                nodeVal = "ε";
            } else {
                nodeVal = tok.value.has_value() ? tok.value.value() : TERMINALS[tok.tag];
                nodeVal = "'" + replace_all(nodeVal, "\"", "") + "'";
                nodeVal += "\n" + tok.frag.str();
            }
        } else if (std::holds_alternative<int>(item->data)) {
            nodeVal = NON_TERMINALS[std::get<int>(item->data)];
        }


        str << "\t" << n << "[label= \""<<  nodeVal << "\"";
        if(std::holds_alternative<Token>(item->data) && std::get<Token>(item->data).tag != EMPTY)
            str << ", shape=doublecircle";
        str <<"];" << std::endl;


        for (int j = syntax.number_of_children(item) - 1; j >= 0; --j) {
            auto child = syntax.child(item, j);
            i++;

            str << "\t" <<  n << " -- " << i << ";" << std::endl;
            nodes.emplace(child);
            numbers.emplace(i);
        }

//        for (int j = 0; j < syntax.number_of_children(item); ++j) {
//            auto child = syntax.child(item, j);
//            i++;
//
//            str << "\t" <<  n << " -- " << i << ";" << std::endl;
//            nodes.emplace(child);
//            numbers.emplace(i);
//        }
    }

    str << "}" << std::endl;

}


int main(int argc, char **argv) {

    if(argc < 1) {
        std::cout << "Invalid arguments. Source file is not matched" << std::endl;
        return 0;
    }


    std::ifstream sourcefile(argv[1]);
    std::string src, line;

    while(std::getline(sourcefile, line)) {
        src += line + "\n";
    }

    TokenStream tokens(src);
//
//    while(tokens.hasNext()) {
//        std::cout << tokens.next().str() << std::endl;
//    }


    Parser parser(tokens);
    stl_tree::tree<st_data> syntax = parser.parse();

    if(argc >= 2) {
        std::ofstream graphfile(argv[2]);
        printSyntaxTree(syntax, graphfile);
    } else {
        printSyntaxTree(syntax, std::cout);
    }

    if(!tokens.getErrors().empty() || !parser.getErrors().empty())
        std::cout << "Errors: " << std::endl;

    for(std::string& error : tokens.getErrors()) {
        std::cout << error << std::endl;
    }

    for(std::string& error : parser.getErrors()) {
        std::cout << error << std::endl;
    }

    return 0;

}