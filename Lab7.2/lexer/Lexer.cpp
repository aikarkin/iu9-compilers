//
// Created by alex on 15.06.18.
//

#include "Lexer.h"

gr_lexer::Lexer::Lexer(std::vector<char> tag_chars) {
    for (int i = 0; i < tag_chars.size(); ++i) {
        tag_by_char.emplace(tag_chars.at(i), i + 3);
        char_to_tag.emplace(i + 3, tag_chars.at(i));
    }


}

gr_lexer::TokenStream gr_lexer::Lexer::tokenize(std::string src) {
    return TokenStream(*this, src);
}


gr_lexer::TokenStream::TokenStream(Lexer lexer, std::string &source) : relatedLexer(lexer), pos(source) {
    this->source = source;

    this->pos.offset = 0;
    this->pos.line = 1;
    this->pos.pos = 1;

    this->errors = std::make_shared< std::vector<std::string> >();
}

Token gr_lexer::TokenStream::next() {
    Token tok;
    this->pos = skipSpaces();

    // END OF PROGRAM
    if(this->pos.offset >= this->source.length()) {
        tok.frag.begin = this->pos;
        this->pos = this->pos.inc();
        tok.frag.follow = this->pos;
        tok.tag = TAG_EOP;
        tok.value = "$";
    } else {
        tok.frag.begin = this->pos;

        char ch = this->source.at(this->pos.offset);

        // single char term
        if(relatedLexer.tag_by_char.count(ch) > 0) {
            this->pos = this->pos.inc();
            tok.frag.follow = this->pos;
            tok.tag = relatedLexer.tag_by_char.at(ch);
            tok.value = {ch};
            // quoted term
        } else if(ch == '\"') {
            std::string term = this->parseString();
            tok.frag.follow = this->pos;
            tok.tag = TAG_TERM;
            tok.value = term;

            // NON TAG_TERM
        } else if(std::isupper(ch)) {
            std::string nonTerm = this->parseNonterm();
            tok.frag.follow = this->pos;
            tok.tag = TAG_NONTERM;
            tok.value = nonTerm;

            // INVALID TOKEN
        } else {
            this->invalidToken(pos, ch);
            this->pos = this->pos.inc();
        }
    }

    return tok;
}

bool gr_lexer::TokenStream::hasNext() {
    return this->pos.offset <= this->source.length();
}

void gr_lexer::TokenStream::invalidToken(Position pos, char tok) {
    std::stringstream ss;
    ss << "Lexical error: unknown token '" << tok << "' at " << pos.str() << std::endl;


    std::string error = ss.str();
    this->errors->push_back(error);

}

Position gr_lexer::TokenStream::skipSpaces() {
    Position pos = this->pos;
    if(this->pos.offset >= this->source.length())
        return pos;

    char ch = this->source.at(pos.offset);

    while(ch == ' ' || ch == '\t' || ch == '\n') {
        pos = pos.inc();
        if(pos.offset >= source.length())
            return pos;
        ch = this->source.at(pos.offset);
    }

    return pos;
}

std::vector<std::string> &gr_lexer::TokenStream::getErrors() {
    return *this->errors;
}

std::string gr_lexer::TokenStream::parseString() {
    std::string str = "\"";
    this->pos = this->pos.inc();
    char ch;

    while(this->pos.offset < this->source.length() && (ch = this->source.at(this->pos.offset)) != '\"') {
        str += ch;
        this->pos = this->pos.inc();
    }

    str += ch;

    if(this->pos.offset + 1 < this->source.length()) {
        this->pos = this->pos.inc();
    }

    return str;
}

std::string gr_lexer::TokenStream::parseNonterm() {
    if(this->pos.offset >= this->source.length())
        return "";

    std::string str = "";
    char ch;

    while(this->pos.offset < this->source.length()) {
        ch = this->source.at(this->pos.offset);
        if (ch != '\'' && !std::isalnum(ch)) {
            break;
        }

        str += ch;
        this->pos = this->pos.inc();
    }

    return str;
}

void gr_lexer::TokenStream::reset() {
    this->pos = Position();
    this->errors->clear();
}

gr_lexer::Lexer &gr_lexer::TokenStream::lexer() {
    return relatedLexer;
}
