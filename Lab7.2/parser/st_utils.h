#ifndef LAB7_ST_UTILS_H
#define LAB7_ST_UTILS_H

#include <vector>
#include "../tree/tree.hpp"


namespace gr_parser {
    std::string replace_all(std::string str, const std::string &substr, const std::string &replacement) {
        size_t idx = 0;
        std::string str_copy = str;
        while (true) {
            idx = str.find(substr, idx);
            if (idx == std::string::npos) break;
            str.replace(idx, substr.length(), replacement);
            idx += substr.length();
        }

        return str;
    }


    void printSyntaxTree(stl_tree::tree<st_data> &syntax, std::ostream &str) {
        /*  0         1           2             3          4        5            6           7           8            9  */
        std::string NON_TERMINALS[] = {"Start", "Nonterms", "NontermsTail", "NonTermDef", "Rules", "RuleAlts",
                                       "RuleAltsTail", "RuleAlt", "RuleAltTail'", "Token"};
//    std::string NON_TERMINALS[] = {"S","A","C","A'","B","D","E","F","F'","G"};

        int i = 0;
        std::queue<st_iter> nodes;
        std::queue<int> numbers;

        nodes.emplace(syntax.begin());
        numbers.emplace(i);

        str << "graph syntax_tree {" << std::endl;


        while (!nodes.empty()) {
            st_iter item = nodes.front();
            int n = numbers.front();
            nodes.pop();
            numbers.pop();


            std::string nodeVal;
            if (std::holds_alternative<Token>(*item)) {
                Token tok = std::get<Token>(*item);
                if (tok.tag == gr_lexer::TAG_EMPTY) {
                    nodeVal = "ε";
                } else {
                    nodeVal = replace_all(tok.str(), "\"", "");
                }
            } else if (std::holds_alternative<int>(*item)) {
                nodeVal = "[" + std::to_string(std::get<int>(*item)) + "] " + NON_TERMINALS[std::get<int>(*item)];
            }


            str << "\t" << n << "[label= \"" << nodeVal << "\"";
            if (std::holds_alternative<Token>(*item) && std::get<Token>(*item).tag != gr_lexer::TAG_EMPTY)
                str << ", shape=doublecircle";
            str << "];" << std::endl;


            for (int j = stl_tree::tree<st_data>::number_of_children(item) - 1; j >= 0; --j) {
                auto child = stl_tree::tree<st_data>::child(item, j);
                i++;

                str << "\t" << n << " -- " << i << ";" << std::endl;
                nodes.emplace(child);
                numbers.emplace(i);
            }

        }

        str << "}" << std::endl;

    }
}

#endif //LAB7_ST_UTILS_H
