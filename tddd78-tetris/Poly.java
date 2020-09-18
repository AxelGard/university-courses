package se.liu.ida.axega544.tddd78.tetris;


public class Poly {
    private SquareType[][] squares;
    
    public Poly(SquareType[][] sq){
        this.squares = sq;
    }

    public SquareType[][] getSquares(){
        return squares;
    }

    public int getLenOfPoly() {
        return squares.length;
    }

    public SquareType getType(int x, int y){return squares[x][y];}

    public Poly rotateRight() {

        Poly newPoly = new Poly(new SquareType[getLenOfPoly()][getLenOfPoly()]);

        for (int r = 0; r < getLenOfPoly(); r++) {
            for (int c = 0; c < getLenOfPoly(); c++){
                newPoly.squares[c][getLenOfPoly()-1-r] = this.squares[r][c];
            }
        }

        return newPoly;
    }


}

