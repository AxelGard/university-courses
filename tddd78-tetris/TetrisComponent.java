package se.liu.ida.axega544.tddd78.tetris;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class TetrisComponent extends JComponent implements BoardListener{

    private Board board;

    private int height;
    private int width;

    private List BoardListeners;

    private int blockSize = 20;

    public TetrisComponent(Board board) {
        this.board = board;
        height = super.getHeight();
        width = super.getWidth();
        BoardListeners = board.getBoardListeners();


    }

    public int getBlockSize() {
        return blockSize;
    }

    public void boardChanged(){
        repaint();

    }



    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        final Graphics2D g2d = (Graphics2D) g;



        for (int i = 0; i < board.getHeight(); i++) {
            for (int j = 0; j < board.getWidth(); j++) {
                SquareType type = board.getSquareAt(j, i);

                switch (type){
                    case EMPTY:
                        g2d.setColor(Color.WHITE);
                        break;
                    case O:
                        g2d.setColor(Color.YELLOW);
                        break;
                    case Z:
                        g2d.setColor(Color.RED);
                        break;
                    case S:
                        g2d.setColor(Color.GREEN);
                        break;
                    case T:
                        g2d.setColor(Color.MAGENTA);
                        break;
                    case I:
                        g2d.setColor(Color.CYAN);
                        break;
                    case L:
                        g2d.setColor(Color.ORANGE);
                        break;
                    case J:
                        g2d.setColor(Color.BLUE);
                        break;
                    default:
                        g2d.setColor(Color.BLACK);

                }


                g2d.fillRect(j*blockSize, i*blockSize, blockSize, blockSize);

            }

        }

    }



}
