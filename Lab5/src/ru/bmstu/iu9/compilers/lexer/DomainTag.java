package ru.bmstu.iu9.compilers.lexer;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public enum DomainTag {
    ERROR(-1),
    KEYWORD(5),
    IDENT(1, 2, 3, 4, 6),
    DECIMAL(7),
    STRING(10),
    WORD_BOUNDARY(8),
    ;

    private Set<Integer> states;

    DomainTag(Integer ...states) {
        this.states = new HashSet<>();
        this.states.addAll(Arrays.asList(states));
    }
    public Set<Integer> getStates() { return this.states; }
}