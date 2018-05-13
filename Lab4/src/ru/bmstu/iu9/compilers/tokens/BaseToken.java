package ru.bmstu.iu9.compilers.tokens;

import ru.bmstu.iu9.compilers.lexer.Fragment;

public class BaseToken {
    private DomainTag tag;
    private Fragment fragment;

    public BaseToken(Fragment fragment, DomainTag tag) {
        this.fragment = fragment;
        this.tag = tag;

    }

    public DomainTag getTag() {
        return tag;
    }

    public Fragment getFragment() {
        return fragment;
    }
}