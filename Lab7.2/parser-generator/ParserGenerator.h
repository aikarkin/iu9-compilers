//
// Created by alex on 15.06.18.
//

#ifndef LAB7_PARSERGENERATOR_H
#define LAB7_PARSERGENERATOR_H


#include <fstream>
#include "../parser/Parser.h"
#include <functional>

#define NONTERMS 1
#define NONTERM_DEF 3

#define RULES 4
#define RULE_ALTS 5
#define RULE_ALT 7
#define TOKEN 9
#define TAG_AT 3

class ParserGenerator {
public:
    ParserGenerator() : axiom_tag(-1) {};
    void generate_by_syntax(char term_symb, char non_term_symb, const stl_tree::tree<gr_parser::st_data> &syntax);
    std::vector<std::string> get_errors();
    gr_parser::Parser get_parser();
    gr_lexer::Lexer get_lexer();
private:
    void def_non_terminals(gr_parser::st_iter node);
    void def_rules(gr_parser::st_iter node);

    void build_first();
    void build_follow();
    void build_table();

    void non_term_lookup(gr_parser::st_iter node, int tag, std::function<void(gr_parser::st_iter)> whenFound);
    void add_error(std::string str);

    std::vector<gr_parser::rule> rules;
    std::unordered_map<std::string, int> nt_tag_by_str;
    std::unordered_map<char, int> t_tag_by_ch;
    int axiom_tag;

    std::unordered_map<int, std::unordered_set<int> > first_sets;
    std::unordered_map<int, std::unordered_set<int> > follow_sets;

    std::unordered_set<char> reserved_symbols;

    std::unique_ptr<gr_parser::Parser> parser_ptr;
    std::unique_ptr<gr_lexer::Lexer> lexer_ptr;
    std::unique_ptr<std::vector<std::vector<int>>> ll1_table_ptr;
    std::vector<std::string> errors;

};


#endif //LAB7_PARSERGENERATOR_H
