package se.liu.ida.axega544.tddd78.tetris;

class BoardToTextConverter {

    public static String ConvertToText(Board table){

        StringBuilder builder = new StringBuilder();

        for (int i = 0; i < table.getHeight(); i++){
            for (int j = 0; j < table.getWidth(); j++){


                switch (table.getSquareAt(j,i)){
                    case EMPTY:
                        builder.append("-");
                        break;
                    case I:
                        builder.append("I");
                        break;
                    case J:
                        builder.append("J");
                        break;
                    case L:
                        builder.append("L");
                        break;
                    case O:
                        builder.append("O");
                        break;
                    case S:
                        builder.append("S");
                        break;
                    case T:
                        builder.append("T");
                        break;
                    case Z:
                        builder.append("Z");
                        break;
                    default:
                        builder.append("Error!");
                }
                if (j == table.getWidth()-1)
                builder.append("\n");
            }
        }

        return builder.toString();

    }






}
