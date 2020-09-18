package se.liu.ida.axega544.tddd78.tetris;

import java.util.ArrayList;
import java.util.List;

class Board {
    private SquareType[][] squares;
    private int height;
    private int width;

    // poly
    private Poly falling = null;
    private int fallingPosX;
    private int fallingPosY;

    private int points = 0;


    //listener
    private List<BoardListener> listeners = new ArrayList<>();


    public Board(int gameWidth, int gameHeight) {
        this.height = gameHeight;
        this.width = gameWidth;
        this.squares = new SquareType[height + 4][width + 4];
        clear();
        notifyListeners();
    }

    private void clear() {
        for (int i = 0; i < getHeight() + 4; i++) {
            for (int j = 0; j < getWidth() + 4; j++) {
                if (i < 2 || i >= height + 2 || j < 2 || j >= width + 2) {
                    this.squares[i][j] = SquareType.OUTSIDE;
                } else this.squares[i][j] = SquareType.EMPTY;
            }
        }
    }

    private boolean gameOver = false;


    public void tick() {
        if (!gameOver) {
            if (getFalling() == null) {
                makeFalling();
                if (hasCollision()) {
                    gameOver = true;
                    //System.out.println("game over");
                    TetrisViewer.gameOver();

                }
            } else if (!hasCollision()) {
                setFallingY(getFallingY() + 1);

            } else {
                setFallingY(getFallingY());
                addFallingToBoard();

                falling = null;

            }

            notifyListeners();
        }
    }

    public int getPoints() {
        return points;
    }

    private void removeRow(){
        int numberOfRowsRemoved = 0;
        for (int i = 0; i < height; i++) {
            boolean rowFull = true;
            for (int j = 0; j < width; j++) {
               if (getType(j,i) == SquareType.EMPTY){
                   rowFull = false;
                   break;
               }
            }

            if (rowFull){
                for (int j = 0; j < width; j++) {
                    for (int k = 1; k < height-2; k++) {
                        if (getSquareAt(j,i-k+1) == SquareType.OUTSIDE) break;
                        else {
                            setTypeAtPos(j, i - k + 1, getSquareAt(j, i - k));
                        }
                    }
                }
                numberOfRowsRemoved++;
            }

        }

        points += numberOfRowsRemoved * 100;

    }

    private void addFallingToBoard(){
        for (int i = 0; i < falling.getLenOfPoly(); i++) {
            for (int j = 0; j < falling.getLenOfPoly(); j++) {

                if(falling.getType(j,i) != SquareType.EMPTY){

                    setTypeAtPos(j+getFallingX(), i+getFallingY()
                            , falling.getType(j,i));

                }


            }

        }
        removeRow();
    }

    public void rotateFalling(){
        falling = falling.rotateRight();
    }

    private void setTypeAtPos(int x, int y, SquareType type){
        this.squares
                [y+2][x+2] = type;
    }

    private void moveType(int x, int y, SquareType type){
        this.squares[y][x] = type;
    }

    public void moveFallingLeft() {
        setFallingX(getFallingX()-1);
        if(hasCollision()) {
            setFallingX(getFallingX() +1);
        }
        else setFallingX(getFallingX());
    }

    public void moveFallingRhigt() {
        setFallingX(getFallingX()+1);
        if(hasCollision()) {
            setFallingX(getFallingX()-1);
        }else setFallingX(getFallingX());
    }

    public void moveFastDown(){
        if (!hasCollision()){
            setFallingY(getFallingY() + 1);
        }
    }


    public void setHeight(int height) {
        notifyListeners();
        this.height = height;
    }

    public void setWidth(int width) {
        notifyListeners();
        this.width = width;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }


    private void makeFalling() {
        falling = new TetrominoMaker().getRandomPoly();
        fallingPosY = 1;
        fallingPosX = (int) getWidth() / 2;

    }


    public SquareType getSquareAt(int x, int y) {

        if (falling != null && fallingPosX <= x && x < fallingPosX + falling.getLenOfPoly() &&
                y < fallingPosY + falling.getLenOfPoly() &&
                fallingPosY <= y) {
            notifyListeners();
            SquareType type =  falling.getSquares()[x - fallingPosX][y - fallingPosY];
            if (type != SquareType.EMPTY){
                return type;
            }
        }
        notifyListeners();
        return getType(x,y);

    }

    public Poly getFalling() {
        return falling;
    }


    public int getFallingX() {
        return fallingPosX;
    }

    public int getFallingY() {
        return fallingPosY;
    }

    public void setFallingY(int fallingY) {
        this.fallingPosY = fallingY;
        notifyListeners();
    }

    public void setFallingX(int fallingX) {
        this.fallingPosX = fallingX;
        notifyListeners();
    }


    public List<BoardListener> getBoardListeners() {
        return listeners;
    }

    public void addBoardListener(BoardListener bl) {
        listeners.add(bl);

    }

    public void removeBoardListener(BoardListener bl) {
        listeners.remove(bl);

    }

    private void notifyListeners() {
        if (listeners != null) {
            listeners.forEach(BoardListener::boardChanged);
        }
    }

    public SquareType getType(int x, int y){

            return this.squares[y+2][x+2];
    }

    public boolean hasCollision() {
        if (falling != null) {
            for (int i = 0; i < falling.getLenOfPoly(); i++) {
                for (int j = 0; j < falling.getLenOfPoly(); j++) {
                    if (falling.getType(j, i) != SquareType.EMPTY &&
                            this.getType(j + getFallingX(), i + getFallingY()+1) != SquareType.EMPTY) {
                        return true;
                    }


                }

            }
        }
        return false;

    }





}
