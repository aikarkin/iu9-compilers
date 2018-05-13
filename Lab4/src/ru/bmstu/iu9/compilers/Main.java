package ru.bmstu.iu9.compilers;

import ru.bmstu.iu9.compilers.lexer.Scanner;
import ru.bmstu.iu9.compilers.tokens.BaseToken;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class Main {

    public static void main(String[] args) {
        try {
            String src = (new java.util.Scanner(new FileInputStream(args[0])).useDelimiter("\\A")).next();

            Scanner sc = new Scanner(src);

            System.out.println("Tokens:\n");
            while(sc.hasNext()) {
                BaseToken tok = sc.nextToken();

                System.out.println(tok == null ? "?" : tok.toString());
            }

            System.out.println("\n---------------------------------------------");
            System.out.println("\nMessages:\n");
            System.out.println(sc.getMessages().toString());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
