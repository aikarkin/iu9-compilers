#include <iostream>
#include <fstream>
#include "lexer/TokenStream.h"
#include "parser/RuleItem.h"
#include "parser/Parser.h"
#include "tree/tree.hpp"
#include "tree/tree_util.hpp"

void print_syntax_tree(const stl_tree::tree<RuleItem> &t, std::ostream &str) {
    using iter = stl_tree::tree<RuleItem>::iterator;
    int i = 0;
    std::queue<iter> nodes;
    std::queue<int> numbers;

    nodes.emplace(t.begin());
    numbers.emplace(i);

    str << "graph syntax_tree {" << std::endl;


    while(!nodes.empty()) {
        auto item = nodes.front();
        int n = numbers.front();
        nodes.pop();
        numbers.pop();

        str << "\t" << n << "[label= "<< item->toString();
        if(item->type == TERM && item->value.term != EPS)
            str << ", shape=doublecircle";
        str <<"];" << std::endl;


        for (int j = t.number_of_children(item) - 1; j >= 0; --j) {
            auto child = t.child(item, j);
            i++;

            str << "\t" <<  n << " -- " << i << ";" << std::endl;
            nodes.emplace(child);
            numbers.emplace(i);
        }
    }

    str << "}" << std::endl;
}

int main(int argc, char **argv) {
    if(argc < 1) {
        std::cout << "Invalid arguments. Source file is not matched" << std::endl;
        return 0;
    }
    std::ifstream sourcefile(argv[1]);

    std::string line;
    std::getline(sourcefile, line);

    TokenStream tokens(line);
    Parser parser(tokens);

    parser.parse();

    stl_tree::tree<RuleItem> tree = parser.getSyntaxTree();

    if(argc >= 2) {
        std::ofstream graphfile(argv[2]);
        print_syntax_tree(tree, graphfile);
    } else {
        print_syntax_tree(tree, std::cout);
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