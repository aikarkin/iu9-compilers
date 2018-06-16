#!/usr/bin/python3

import sys

grfile = open(sys.argv[1], 'r')
rules = []

#                0   1   2   3    4   5   6   7   8    9
NON_TERMS    = ["S","A","C","A'","B","D","E","F","F'","G"]
#    -1   0   1      2          3   4   5   6   7   8   9  
TERMS = ["$","term","nonterm", "@","{","}",",","[",":","]"]

for rule in grfile:
    arrow = rule.split('->')
    if len(arrow) != 2:
        continue
    lss, rss = arrow
    lss = lss.replace(' ', '')
    rss = rss.split(' ')
    ls = -1
    rs = []
    
    if lss.count(lss) == 0:
        print("Error: unknown NonTerminal '" + lss + "'")
    else:
        ls = NON_TERMS.index(lss)

    for tok in rss:
        tok = tok.replace('\n', '')
        if tok != "":
            if TERMS.count(tok):
                rs.append({"type": "TERM", "tok": TERMS.index(tok)})
            elif NON_TERMS.count(tok):
                rs.append({"type": "NONTERM", "tok": NON_TERMS.index(tok)})
            elif tok == 'e':
                rs.append({"type": "TERM", "tok": -1})
    
    rules.append((ls, rs))

print(rules)
for rule in rules:
    ls, rs = rule
    rs_syms = list(map(lambda tok: "symbol(%s, %d)" % (tok['type'], tok['tok']), rs))
    print("\trule(%d, { %s })," % (ls, ",".join(rs_syms)))