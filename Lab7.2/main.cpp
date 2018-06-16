#include <iostream>
#include <fstream>
#include "parser/parser_types/symbol.h"
#include "parser/Parser.h"
#include "tree/tree.hpp"
#include "tree/tree_util.hpp"
#include "parser/st_utils.h"
#include "parser-generator/ParserGenerator.h"
#include <variant>

using namespace gr_parser;
using namespace gr_lexer;

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

    //                           3   4    5    6    7    8    9
    std::vector<char> spec_toks{'@','{', '}', ',', '[', ':', ']'};


    std::vector<rule> rules {
            rule(0, { symbol(NONTERM, 1),symbol(NONTERM, 4) }),
            rule(1, { symbol(NONTERM, 3),symbol(NONTERM, 2) }),
            rule(2, { symbol(TERM, 6),symbol(NONTERM, 3),symbol(NONTERM, 2) }),
            rule(2, { symbol(TERM, -1) }),
            rule(3, { symbol(TERM, 4),symbol(TERM, 2),symbol(TERM, 5) }),
            rule(3, { symbol(TERM, 2) }),
            rule(4, { symbol(TERM, 7),symbol(NONTERM, 5),symbol(TERM, 9),symbol(NONTERM, 4) }),
            rule(4, { symbol(TERM, -1) }),
            rule(5, { symbol(TERM, 2),symbol(TERM, 8),symbol(NONTERM, 7),symbol(NONTERM, 6) }),
            rule(6, { symbol(TERM, 8),symbol(NONTERM, 7),symbol(NONTERM, 6) }),
            rule(6, { symbol(TERM, -1) }),
            rule(7, { symbol(NONTERM, 9),symbol(NONTERM, 8) }),
            rule(8, { symbol(NONTERM, 9),symbol(NONTERM, 8) }),
            rule(8, { symbol(TERM, -1) }),
            rule(9, { symbol(TERM, 1) }),
            rule(9, { symbol(TERM, 2) }),
            rule(9, { symbol(TERM, 3) }),
    };


    std::vector<std::vector<int>> transitions{
            /*     $      TERM   NONTERM         @         {         }         ,         [         :         ]   */
/* 0  S  */  {    -1,       -1,        0,       -1,        0,       -1,       -1,       -1,       -1,       -1   },
/* 1  A  */  {    -1,       -1,        1,       -1,        1,       -1,       -1,       -1,       -1,       -1   },
/* 2  C  */  {     3,       -1,       -1,       -1,       -1,       -1,        2,        3,       -1,       -1   },
/* 3  A' */  {    -1,       -1,        5,       -1,        4,       -1,       -1,       -1,       -1,       -1   },
/* 4  B  */  {     7,       -1,       -1,       -1,       -1,       -1,       -1,        6,       -1,       -1   },
/* 5  D  */  {    -1,       -1,        8,       -1,       -1,       -1,       -1,       -1,       -1,       -1   },
/* 6  E  */  {    -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1,        9,       10   },
/* 7  F  */  {    -1,       11,       11,       11,       -1,       -1,       -1,       -1,       -1,       -1   },
/* 8  F' */  {    -1,       12,       12,       12,       -1,       -1,       -1,       -1,       13,       13   },
/* 9  G  */  {    -1,       14,       15,       16,       -1,       -1,       -1,       -1,       -1,       -1   },
    };


    Lexer lexer(spec_toks);
    TokenStream tokens = lexer.tokenize(src);
//
//    while(tokens.hasNext()) {
//        Token tok = tokens.next();
//        std::cout << tok.frag.str() << ": " << tok.str() << std::endl;
//    }
//
    tokens.reset();

    Parser parser(transitions, rules);
    stl_tree::tree<st_data> syntax = parser.parse(tokens);

    ParserGenerator parser_gen;
    parser_gen.generate_by_syntax('#', '~', syntax);

    Parser generated_parser = parser_gen.get_parser();
    Lexer generated_lexer = parser_gen.get_lexer();

    sourcefile = std::ifstream("/home/alex/dev/src/iu9/compilers/Lab7.2/res/arithmetic.txt");
    src = "";

    while(std::getline(sourcefile, line)) {
        src += line + "\n";
    }

    tokens = generated_lexer.tokenize(src);

    while(tokens.hasNext()) {
        Token tok = tokens.next();
        std::cout << tok.frag.str() << ": " << tok.str() << std::endl;
    }

    tokens.reset();

    std::cout << "-- NEW PARSE -- " << std::endl;
    stl_tree::tree<st_data> new_syntax = generated_parser.parse(tokens);

    std::cout << "Parser Generator Errors:" << std::endl;

    for (const auto &err : parser_gen.get_errors()) {
        std::cout << err << std::endl;
    }


    if(argc >= 2) {
        std::ofstream graphfile(argv[2]);
        printSyntaxTree(new_syntax, graphfile);
    } else {
        printSyntaxTree(new_syntax, std::cout);
    }

    if(!tokens.getErrors().empty() || !parser.getErrors().empty())
        std::cout << "Errors: " << std::endl;

    for(const auto &err: tokens.getErrors())

    for(std::string& error : tokens.getErrors()) {
        std::cout << error << std::endl;
    }

    return 0;

}