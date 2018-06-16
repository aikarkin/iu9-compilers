//
// Created by alex on 15.06.18.
//

#ifndef LAB7_LEXER_H
#define LAB7_LEXER_H


#include <vector>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <map>

#include "../commons/token.h"

namespace gr_lexer {
    const int TAG_EMPTY = -1;
    const int TAG_EOP = 0;
    const int TAG_TERM = 1;
    const int TAG_NONTERM = 2;

    class TokenStream;

    class Lexer {
        friend TokenStream;
    public:
        Lexer(std::vector<char> tag_chars);
        TokenStream tokenize(std::string src);
    private:
        std::map<char, int> tag_by_char;
        std::map<int, char> char_to_tag;
    };

    class TokenStream {
    public:
        TokenStream(Lexer lexer, std::string &source);

        Token next();
        bool hasNext();
        void reset();
        std::vector<std::string> &getErrors();
        Lexer &lexer();

    private:
        Position skipSpaces();
        std::string parseString();
        std::string parseNonterm();

        void invalidToken(Position pos, char tok);

        Position pos;
        std::string source;
        std::shared_ptr<std::vector<std::string>> errors;
        Lexer relatedLexer;
    };
}

#endif //LAB7_LEXER_H
