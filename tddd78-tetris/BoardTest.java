package se.liu.ida.axega544.tddd78.tetris;

public class BoardTest {

    public static void main(String[] args) {
        Board testBoard;
        testBoard = new Board(10, 35);
        //System.out.println(BoardToTextConverter.ConvertToText(testBoard));
        new TetrisViewer(testBoard);
    }

}
