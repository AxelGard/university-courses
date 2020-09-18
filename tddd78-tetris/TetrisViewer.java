package se.liu.ida.axega544.tddd78.tetris;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

class TetrisViewer {
    private JFrame frame;
    private TetrisComponent component;
    private Board board;


    private int blockSize = 20;


    public TetrisViewer(Board board){
        frame = new JFrame();
        //make board
        this.board = board;
        component = new TetrisComponent(board);

        keyInput();
        board.addBoardListener(component);
        GameTimer(board);

    }

    public static void gameOver(){
        JOptionPane.showMessageDialog(null, "Game Over!", "end of game", JOptionPane.INFORMATION_MESSAGE);
        System.exit(0);
    }


    public static boolean shuldQuit(){
            if (askUser("Quit?") && askUser("Relly?")){
                return true;
            }
            else return false;
    }
    private static boolean askUser(String question){
        return JOptionPane.showConfirmDialog(null, question,
                "", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION;
    }

    public Dimension getPreferredSize(){
        int dimX = board.getWidth()*blockSize;
        int dimY = (board.getHeight()+3)*blockSize;
        return new Dimension(dimX,dimY);


    }



    private void GameTimer(Board board) {
        final Action doOneStep = new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                board.tick();

            }
        };

        frame.add(component, BorderLayout.CENTER);
        //frame.setSize(500, 800);
        //frame.setPreferredSize(getPreferredSize());
        frame.setSize(getPreferredSize());
        //frame.setVisible(true);

        final JMenuBar bar = new JMenuBar();

        final JButton end = new JButton("Close");

        end.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent event) {
                if(shuldQuit()){
                    System.exit(0);
                }
            }

        });
        bar.add(end);



        frame.setJMenuBar(bar);



        //frame.pack();
        frame.setVisible(true);


        final Timer clockTimer = new Timer(500, doOneStep);
        clockTimer.setCoalesce(true);

        clockTimer.start();
        while (true){}

        //clockTimer.stop();

    }


    private class moveLeftAction extends AbstractAction {
        public void actionPerformed(ActionEvent e) {
            board.moveFallingLeft();

        }
    }

    private class moveRhigtAction extends AbstractAction {
        public void actionPerformed(ActionEvent e) {
            board.moveFallingRhigt();

        }
    }

    private class RotateAction extends AbstractAction {
        public void actionPerformed(ActionEvent e) {
            board.rotateFalling();

        }
    }

    private class fastDown extends AbstractAction {
        public void actionPerformed(ActionEvent e) {

            board.moveFastDown();

        }
    }

    private void keyInput(){

        JComponent pane = frame.getRootPane();

        final InputMap in = pane.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW);
        in.put(KeyStroke.getKeyStroke("A"), "Left");

        final ActionMap act = pane.getActionMap();
        act.put("Left",  new moveLeftAction());


        in.put(KeyStroke.getKeyStroke("D"), "Rhigt");
        act.put("Rhigt",  new moveRhigtAction());

        in.put(KeyStroke.getKeyStroke("W"), "Rotate");
        act.put("Rotate",  new RotateAction());

        in.put(KeyStroke.getKeyStroke("S"), "Down");
        act.put("Down",  new fastDown());
    }

}

/*//leftover from old string tetris
    private void layout(){
        // layout
        frame.setLayout(new BorderLayout());
        frame.add(component, BorderLayout.CENTER);

        frame.pack();
        frame.setVisible(true);
    }
    */