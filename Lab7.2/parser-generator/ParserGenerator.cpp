#include <iostream>
#include "ParserGenerator.h"

void ParserGenerator::generate_by_syntax(const stl_tree::tree<gr_parser::st_data> &syntax) {
    def_non_terminals(syntax.begin());
    def_rules(syntax.begin());

//    std::cout << "NONTERMS: " << std::endl;
//
//    for(auto&& [tok, tag] : nt_tag_by_str) {
//        std::cout << "-- " << tok << ": " << tag << std::endl;
//    }
//
//    std::cout << "AXIOM: " << axiom_tag << std::endl;
//
//    std::cout << "TERMS: " << std::endl;
//
//    for(auto&& [term, tag] : t_tag_by_ch) {
//        std::cout << "-- " << term << ": " << tag << std::endl;
//    }
//
//
//    for(auto& rule : rules) {
//        std::cout << rule.left.tag << " -> ";
//
//        for(auto& tok : rule.right) {
//            std::cout << (tok.type == gr_parser::TERM ? "TERM" : "NON_TERM")  << "(" << tok.tag << ") ";
//        }
//
//        std::cout << std::endl;
//    }
//
//
    build_first();
    build_follow();
//
//    std::cout << "FIRST:" << std::endl;
//    for(const auto& [nt, terms] : first_sets) {
//        std::cout << nt << ": ";
//        for(const auto& term: terms)
//            std::cout << term << " ";
//        std::cout << std::endl;
//    }
//
//    std::cout << "FOLLOW:" << std::endl;
//    for(const auto& [nt, terms] : follow_sets) {
//        std::cout << nt << ": ";
//        for(const auto& term: terms)
//            std::cout << term << " ";
//        std::cout << std::endl;
//    }
}

void ParserGenerator::nonterm_lookup(gr_parser::st_iter node, int tag, const std::function<void(gr_parser::st_iter)> whenFound) {
    std::queue<gr_parser::st_iter> queue;
    gr_parser::st_iter top;
    queue.emplace(node);
    int cc;

    while(!queue.empty()) {
        top = queue.front();
        queue.pop();
        cc = stl_tree::tree<gr_parser::st_data>::number_of_children(top);


        if(std::holds_alternative<int>(*top) && std::get<int>(*top) == tag) {
            whenFound(top);
        }

        for (int i = 0; i < cc; ++i) {
            queue.push(stl_tree::tree<gr_parser::st_data>::child(top, i));
        }
    }
}

void ParserGenerator::add_error(std::string str) {
    this->errors.push_back(str);
}

std::vector<std::string> ParserGenerator::get_errors() {
    return this->errors;
}

void ParserGenerator::def_non_terminals(gr_parser::st_iter node) {
    int nt_tag = 0;

    this->nonterm_lookup(node, NONTERM_DEF, [&](gr_parser::st_iter nt_node) {
        int cc = stl_tree::tree<gr_parser::st_data>::number_of_children(nt_node);
        Token tok, nt;
        bool isAxiom = false;

        for (int i = 0; i < cc; ++i) {
            auto child = stl_tree::tree<gr_parser::st_data>::child(nt_node, i);
            if(std::holds_alternative<Token>(*child)) {
                tok = std::get<Token>(*child);
                if(tok.tag == gr_lexer::TAG_NONTERM && tok.value.has_value()) {
                    nt_tag_by_str.emplace(tok.value.value(), nt_tag++);
                    nt = tok;
                } else if(tok.tag == 4) {
                    isAxiom = true;
                }
            }
        }

        if(isAxiom) {
            if(axiom_tag >= 0) {
                add_error("Duplicated axiom: " + nt.value.value());
            }

            axiom_tag = nt_tag_by_str.at(nt.value.value());
        }
    });
}

void ParserGenerator::def_rules(gr_parser::st_iter node) {
    int t_tag = 3;


    this->nonterm_lookup(node, RULE_ALTS, [&](gr_parser::st_iter rule_alts_node) {
        int cc = stl_tree::tree<gr_parser::st_data>::number_of_children(rule_alts_node);
        int ls;


        for(int i = cc - 1; i >=0; i--) {
            auto child = stl_tree::tree<gr_parser::st_data>::child(rule_alts_node, i);

            if(std::holds_alternative<Token>(*child)) {
                Token tok = std::get<Token>(*child);
                if(tok.tag == gr_lexer::TAG_NONTERM) {
                    if(tok.value.has_value() && nt_tag_by_str.count(tok.value.value())) {
                        ls = nt_tag_by_str.at(tok.value.value());
                    } else {
                        add_error("Non-terminal '" + tok.value.value_or("UNKNOWN") + "' is not defined.");
                    }
                }
            }
        }

        nonterm_lookup(rule_alts_node, RULE_ALT, [&](gr_parser::st_iter alt_node) {
            gr_parser::rule alt_rule(ls, std::vector<gr_parser::symbol>());

            nonterm_lookup(alt_node, TOKEN, [&](gr_parser::st_iter tok_node) {
                auto child = stl_tree::tree<gr_parser::st_data>::child(tok_node, 0);
                if(std::holds_alternative<Token>(*child)) {
                    Token tok = std::get<Token>(*child);

                    std::string tok_val = tok.value.value();

                    if (tok.tag == gr_lexer::TAG_NONTERM) {
                        alt_rule.right.emplace_back(gr_parser::NONTERM, nt_tag_by_str.at(tok_val));
                    } else if (tok.tag == gr_lexer::TAG_TERM) {
                        int tag;

                        if(tok_val.length() == 3 && tok_val.at(0) == '\"' && tok_val.at(2) == '\"') {
                            char ch = tok_val.at(1);

                            if(t_tag_by_ch.count(ch) == 0)
                                t_tag_by_ch.emplace(ch, t_tag++);

                            tag = t_tag_by_ch.at(ch);
                        } else {
                            tag = gr_lexer::TAG_TERM;
                        }

                        alt_rule.right.emplace_back(gr_parser::TERM, tag);
                    } else if(tok.tag == TAG_AT) {
                        alt_rule.right.emplace_back(gr_parser::TERM, gr_lexer::TAG_EMPTY);
                    }
                }
            });

            rules.push_back(alt_rule);
        });
    });

}

void ParserGenerator::build_first() {
    first_sets.clear();

    for(const auto& rule : rules) {
        first_sets.emplace(rule.left.tag, std::unordered_set<int>());
    }

    std::unordered_set<int> eps_prod;
    bool changed;

    do {
        changed = false;

        for(const auto &rule : rules) {
            gr_parser::symbol first = rule.right.at(0);
            int non_term = rule.left.tag;
            size_t prev_set_size = first_sets.at(non_term).size();

            if(first.type == gr_parser::TERM) {
                if(first.tag == gr_lexer::TAG_EMPTY) {
                    eps_prod.emplace(first.tag);
                }

                first_sets.at(non_term).emplace(first.tag);
            } else {
                int i = 1;
                do {
                    std::unordered_set<int> &rs_nts = first_sets.at(first.tag);
                    first_sets.at(non_term).insert(rs_nts.begin(), rs_nts.end());
                    first = rule.right[i++];
                } while(i < rule.right.size() && first.type == gr_parser::NONTERM && eps_prod.count(first.tag));

                if (i < rule.right.size() && rule.right[i].type == gr_parser::TERM) {
                    first_sets.at(non_term).emplace(rule.right[i].tag);
                }
            }

            if(!changed && prev_set_size != first_sets.at(non_term).size())
                changed = true;
        }
    } while(changed);
}

void ParserGenerator::build_follow() {
    follow_sets.clear();

    for(const auto& rule : rules) {
        follow_sets.emplace(rule.left.tag, std::unordered_set<int>());
    }

    follow_sets.at(axiom_tag).emplace(gr_lexer::TAG_EOP);

    for(const auto& rule: rules) {
        for (int i = 0; i < rule.right.size() - 1; ++i) {
            auto &cur_tok = rule.right[i];
            auto &next_tok = rule.right[i + 1];

            if(cur_tok.type == gr_parser::NONTERM) {
                if(next_tok.type == gr_parser::TERM && next_tok.tag != gr_lexer::TAG_EMPTY) {
                    follow_sets.at(cur_tok.tag).emplace(next_tok.tag);
                } else if(next_tok.type == gr_parser::NONTERM) {
                    for(const auto& firsts_tok : first_sets.at(next_tok.tag)) {
                        if(firsts_tok != gr_lexer::TAG_EMPTY)
                            follow_sets.at(cur_tok.tag).emplace(firsts_tok);
                    }
                }
            }
        }
    }

    bool changed;

    do {
        changed = false;
        for(const auto& rule: rules) {
            for (int i = 0; i < rule.right.size(); ++i) {
                auto& cur_tok = rule.right[i];

                if(cur_tok.type == gr_parser::NONTERM) {
                    size_t cur_follow_size = follow_sets.at(cur_tok.tag).size();
                    auto next_tok = i + 1 < rule.right.size() ? rule.right[i + 1] : gr_parser::symbol(gr_parser::TERM,
                                                                                                      gr_lexer::TAG_EMPTY);

                    if (next_tok.tag == gr_lexer::TAG_EMPTY || (next_tok.type == gr_parser::NONTERM && first_sets.at(next_tok.tag).count(gr_lexer::TAG_EMPTY))) {
                        auto &lsTokFollow = follow_sets.at(rule.left.tag);
                        follow_sets.at(cur_tok.tag).insert(lsTokFollow.begin(), lsTokFollow.end());
                    }
                    if (!changed && cur_follow_size != follow_sets.size()) {
                        changed = true;
                    }
                }
            }
        }
    } while(!changed);
}

