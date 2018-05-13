package ru.bmstu.iu9.compilers.lexer;

public class Message {
    private String msg;
    private Type type;
    private Position position;

    Message(Type type, String msg) {
        this.type = type;
        this.msg = msg;
    }

    Message(Type type, Position pos, String msg) {
        this(type, msg);
        this.position = pos;
    }

    public String getMessage() {
        return msg;
    }

    public Type getType() {
        return type;
    }

    public Position getPosition() {
        return position;
    }

    public void setPosition(Position position) {
        this.position = position;
    }

    public enum Type {error, warn, info};

    @Override
    public String toString() {
        return String.format("[%s] Error at %s: %s.", type.name(), position.toString(), msg);
    }
}
