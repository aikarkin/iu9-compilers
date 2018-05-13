package ru.bmstu.iu9.compilers.lexer;

public class Token {
    private DomainTag tag;
    private Fragment fragment;

    Token(Fragment frag, DomainTag tag) {
        this.fragment = frag;
        this.tag = tag;
    }

    public DomainTag getTag() {
        return tag;
    }

    public Fragment getFragment() {
        return fragment;
    }

    @Override
    public String toString() {
        return String.format("%s %s: \"%s\"", this.tag.name(), this.fragment.toString(), this.fragment.getTokenRepresentation());
    }
}
