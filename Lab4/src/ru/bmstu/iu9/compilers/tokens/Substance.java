package ru.bmstu.iu9.compilers.tokens;

import ru.bmstu.iu9.compilers.lexer.Fragment;

public class Substance extends BaseToken {
    private String name;

    public Substance(Fragment frag, String name) {
        super(frag, DomainTag.SUBSTANCE);
        this.name = name;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return String.format("%s = %s: %s",
                getTag().name(),
                name,
                getFragment().toString()
        );
    }
}
