package ru.bmstu.iu9.compilers.lexer;

import java.util.ArrayList;
import java.util.List;

public class MessageList {
    private List<Message> messages;

    public MessageList() {
        this.messages = new ArrayList<>();
    }

    public void addError(Position coord, String text) {
        this.addMessage(Message.Type.error, coord, text);
    }

    public void addWarn(Position coord, String text) {
        this.addMessage(Message.Type.warn, coord, text);
    }

    private void addMessage(Message.Type type, Position coord, String text) {
        this.messages.add(new Message(type, coord, text));
    }

    @Override
    public String toString() {
        List<String> messageStrings = new ArrayList<>();
        for(Message msg : messages)
            messageStrings.add(msg.toString());

        return String.join("\n", messageStrings);
    }
}
