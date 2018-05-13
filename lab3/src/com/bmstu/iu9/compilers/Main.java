package com.bmstu.iu9.compilers;

import com.sun.org.apache.xerces.internal.impl.xpath.regex.Match;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/*
    string literal

    1 opt
        "sdfsddfsdfx""abc""sdfgsdfgsdfgsdfg" - 1 string

    2 opt
        "sdfdsfsdfg" "dffsgsdfg" "sdfgsdfg" - 3 string

        (".*?"(?!"))


 */


public class Main {
    private static final Pattern STR_PTR = Pattern.compile("(\"(((\"\".*?\"\")|([\\\\]\\n)|.)*?)\")(?!\")", Pattern.MULTILINE);
    private static final Pattern IDENT_PTR = Pattern.compile("(\\p{L}[\\p{L}0-9$]*)");
    private static final Pattern NUMBER_PTR = Pattern.compile("(\\d+|[$][a-fA-F0-9]+)");
    private static final Pattern TOKEN_PTRS[] = {STR_PTR, IDENT_PTR, NUMBER_PTR};

    private static Position skipSpaces(Position pos) {
        Position newPos = new Position(pos);
        String src = pos.getSource();
        int offset = newPos.getOffset();
        if(offset >= src.length())
            return pos;

        char ch = src.charAt(offset);

        while(ch == ' ' || ch == '\t' || ch == '\n') {
            offset++;
            if(offset >= src.length())
                break;
            newPos = newPos.increase(1);
            ch = newPos.getSource().charAt(offset);
        }

        return newPos;
    }

    public static void main(String[] args) {
	    // write your code here

        try {
            String src = new Scanner(new File(args[0])).useDelimiter("\\A").next();
            String tail = src;
            Position pos = skipSpaces(new Position(src));

            while(tail.length() > 0) {
                Token tok = null;
                for (int i = 0; i < TOKEN_PTRS.length; i++) {
                    Pattern ptr = TOKEN_PTRS[i];
                    Matcher tokMatch = ptr.matcher(tail);
                    if(tokMatch.lookingAt()) {
                        if(tokMatch.group(1) != null) {
                            tok = new Token(pos, TokenType.values()[i], tokMatch.group(1));
                            System.out.println(tok.toString());
                            break;
                        }
                    }
                }
                if(tok == null) {
                    System.out.println("Syntax error at " + pos.toString());
                    pos = pos.increase(1);
                } else {
                    pos = pos.increase(tok.getValue().length());
                }
                pos = skipSpaces(pos);
                tail = src.substring(pos.getOffset());
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }


    }
}
