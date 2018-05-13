package ru.bmstu.iu9.compilers.tokens;

import ru.bmstu.iu9.compilers.lexer.Fragment;

public class Operator extends BaseToken {
    private OperatorType value;

    public Operator(Fragment frag, OperatorType opType) {
        super(frag, DomainTag.OP);
        this.value = opType;
    }

    public OperatorType getType() {
        return this.value;
    }

    public enum OperatorType { PLUS, ARROW };

    @Override
    public String toString() {
        return String.format("%s: %s", value.name(), getFragment().toString());
    }
}
