package ru.bmstu.iu9.compilers;

import ru.bmstu.iu9.compilers.lexer.TokenStream;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Main {
    public static void main(String[] args) {
        TokenStream stream = null;
        try {
            byte[] encoded = Files.readAllBytes(Paths.get(args[0]));
            String src = new String(encoded, "UTF-8");
            System.out.println("Source: ");
            System.out.println(src);
            stream = new TokenStream(src);
            System.out.println("\nTokens: ");
            while(stream.hasNext()) {
                System.out.println(stream.next().toString());
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(stream != null) {
                System.out.println("\nErrors: ");
                System.out.println(stream.getMessages());
            }
        }
    }
}
