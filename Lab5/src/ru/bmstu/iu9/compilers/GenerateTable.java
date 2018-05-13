package ru.bmstu.iu9.compilers;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class GenerateTable {
    public static void main(String[] args) {
        try {
            Scanner sc = new Scanner(new FileInputStream(args[0]));
            List<String> lines = new ArrayList<>();
            while(sc.hasNext()) {
                lines.add(sc.nextLine());
            }
            String[] stateNumbers = lines.get(0).split(",");
            String[] symbols = new String[lines.size()];
            String[][] states = new String[stateNumbers.length - 1][lines.size() - 1];
            for (int i = 1; i < lines.size(); i++) {
                String[] row = lines.get(i).split(",");
                symbols[i - 1] = row[0];
                for (int j = 1; j < row.length; j++) {
                    states[j - 1][i - 1] = (row[j] == null || row[j].length() == 0 ? "-1" : row[j]);
                }
            }


            System.out.println("int[][] table = {");
            System.out.println("\t\t\t\t/* " + String.join(", ", symbols) + " */");
            for (int i = 0; i < states.length; i++) {
                System.out.printf("\t/* state %d */ {%s},\n", i, String.join(",   ", states[i]));
            }
            System.out.println("};");

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
