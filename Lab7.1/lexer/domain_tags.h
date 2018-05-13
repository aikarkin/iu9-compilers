//
// Created by alex on 03.05.18.
//

#ifndef LAB7_1_DOMAIN_TAGS_H
#define LAB7_1_DOMAIN_TAGS_H


enum DomainTag {
   /* 0 */  ADD,
   /* 1 */  MUL,
   /* 2 */  NUM,
   /* 3 */  LPAREN,
   /* 4 */  RPAREN,
   /* 5 */  EPS,
   /* 6 */  EOP,
   /* 7 */  UNKNOWN,
};

const std::string TAG_NAMES[] {"+", "*", "n", "(", ")", "ε", "$", "[UNRECOGNIZED]"};

#endif //LAB7_1_DOMAIN_TAGS_H