package se.liu.ida.axega544.tddd78.tetris;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

import static java.util.concurrent.TimeUnit.SECONDS;

class TetrisViewer_v1 {
    private JFrame frame;
    private JTextArea textArea;
    private Board board;

    public TetrisViewer_v1(Board board){
        frame = new JFrame();
        textArea = new JTextArea();
        board = new Board(40, 50);

        //make board
        textArea.append(setText(board));
        updateFrame_old();
        //GameTimer(board);


    }

    private void layout(){
        // layout
        frame.setLayout(new BorderLayout());
        frame.add(textArea, BorderLayout.CENTER);

        textArea.setFont(new Font("Monospaced", Font.PLAIN, 20));
        frame.pack();
        frame.setVisible(true);
    }


    private void GameTimer(Board board) {
        final Action doOneStep = new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                FrameUpdate(boardUpdate(board));
            }
        };

        final Timer clockTimer = new Timer(500, doOneStep);
        clockTimer.setCoalesce(true);
        clockTimer.start();

        clockTimer.stop();


    }
    private Board boardUpdate(Board board){
        if (board.getFallingX() <= board.getWidth()){
            board.setFallingX(board.getFallingX() + 1);
            return board;
        }
        else return board;
    }

    private void FrameUpdate(Board board){
        while (true) {
            board = boardUpdate(board);
            textArea.selectAll();
            textArea.replaceSelection(setText(board));
            frame.add(textArea, BorderLayout.CENTER);
            layout();

        }

    }

    public void setBoard(Board board) {
        this.board = board;
    }

    private void updateFrame_old(){
        Board board = new Board(40, 50);
        while (true) {
            if (board.getFallingX() <= board.getWidth()){
                board.setFallingX(board.getFallingX() + 1);
            }
            textArea.selectAll();
            textArea.replaceSelection(setText(board));
            //textArea.append(setText(board));
            frame.add(textArea, BorderLayout.CENTER);
            layout();
            try {
                SECONDS.sleep(1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }


    private String setText(Board board){
        return BoardToTextConverter.ConvertToText(board);
    }

}
