package ru.bmstu.iu9.compilers.tokens;

import ru.bmstu.iu9.compilers.lexer.Fragment;

public class Coefficient extends BaseToken {
    private int value;

    public Coefficient(Fragment fragment, String val) {
        super(fragment, DomainTag.COEFFICIENT);
        this.value = Integer.valueOf(val);
    }

    public Coefficient(Fragment fragment, int val) {
        super(fragment, DomainTag.COEFFICIENT);
        this.value = val;
    }

    public int getValue() {
        return value;
    }

    @Override
    public String toString() {
        return String.format("%s = %d: %s", getTag().name(), value, getFragment().toString());
    }
}
